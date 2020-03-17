#include "ConsoleRTKLib.h"
#include <iostream>


int main() {

	//������������� IP-������
	ip_address = ":@192.168.0.186:5018/:";

	//������������� ������� "�����" ������
	format = STRFMT_RTCM3;

	//������������� ��������� ������
	stream_type = STR_TCPCLI;

	//�������� ���������� �������� ��-���������
	PrcOpt = prcopt_default;

	//�������� ���������� ������� ��-���������
	SolOpt = solopt_default;

	//������������� ������ ����� ������ ��-���������
	strinitcom();

	//������������� ��������� ��� ����� ������ 
	rtksvrinit(&rtksvr);

	//������������� ������ ��� ����� ������
	strinit(&monistr);

	//�������� ��������� ����������
	local_dir = "C:\\Temp";

	//������ ������ �����
	SvrStart();

	while (true) {
		Timer();
		Sleep(1000);
	}

}



void __fastcall SvrStart(void)
{
	//���������� ��� �������� ��������� �� �������
	char errmsg[20148];

	/* baseline length to reset (km) */
	double MaxBL = 0;

	//��������� ������
	int stropt[8] = { 0 };

	//���������� ��� �������� ���������� �������� �������
	solopt_t solopt[2];
	
	//��������� ��� ������ � ������� RTCM
	rtcm_t rtcm;
	
	//���������� ��� �������� ��������� 
	double pos[3], nmeapos[3];
	
	//������������� ���������� ��� �������� ���� ������ ��� ���� �������� � ��������� ������
	int	strs[MAXSTRRTK] = { 0 };
	
	//���� � ������� ������ ������ (IP, ����)
	char* paths[8];
	
	//������� ��� ������ �������� �������
	char* cmds[3] = { 0 };
	
	//������������� ������� ��� �������� �������
	char* cmds_periodic[3] = { 0 };
	
	//��������� ��������
	char* rcvopts[3] = { 0 };
	
	//������� �����
	gtime_t time = timeget();
	
	/* antenna parameters type */
	pcvs_t pcvr = { 0 }, pcvs = { 0 };
	
	/* antenna parameter type */
	pcv_t* pcv;
	

	rtksvrlock(&rtksvr);
	//������� ������� ��� ������� �������
	format = rtksvr.format[1] = format;
	//��������� ��������� 
	rtcm = rtksvr.rtcm[1];
	rtksvrunlock(&rtksvr);


	//��������� ���� ����� �������
	for (int i = 0; i < 8; i++)
	{
		strs[i] = STR_NONE;
		Format[i] = NULL;
		//paths[i] = NULL;
		if (i < 3) {
			rcvopts[i] = new char[RcvOpt[i].size() + 1];
			strcpy(rcvopts[i], RcvOpt[i].c_str());
		}
	}
	//���������� ������� ������� ���������� ������
	strs[1] = stream_type;
	//���������� ������� ������� ���������� �������
	Format[1] = format;
	//���������� ������� ������� ���������� ������
	paths[1] = new char[ip_address.size() + 1];
	strcpy(paths[1], ip_address.c_str());
	
	
	//��������� ������� ��� ����� ������ NMEA
	nmea_cycle = nmea_cycle < 1000 ? 1000 : nmea_cycle;
	//�������� ���������
	pos[0] = nmeapos[0] * D2R;
	pos[1] = nmeapos[1] * D2R;
	pos[2] = nmeapos[2];
	//�������������� ���������
	pos2ecef(pos, nmeapos);

	//��������� ������� ����������
	strsetdir(local_dir.c_str());
	//strsetproxy(ProxyAddr.c_str());

	

	//��������� ��������� ������
	stropt[0] = 0;
	stropt[1] = 0;
	stropt[2] = 1000;
	stropt[3] = 0;
	stropt[4] = 0;
	//��������� �������������� ���������� ������
	strsetopt(stropt);
	//����������� ������� ������
	strcpy(rtksvr.cmd_reset, ResetCmd.c_str());
	/* baseline length to reset (km) */
	rtksvr.bl_reset = MaxBL;

	
	/* start rtk server ------------------------------------------------------------
	* start rtk server thread
	* args   : rtksvr_t *svr    IO rtk server
	*          int     cycle    I  server cycle (ms)
	*          int     buffsize I  input buffer size (bytes)
	*          int     *strs    I  stream types (STR_???)
	*                              types[0]=input stream rover
	*                              types[1]=input stream base station
	*                              types[2]=input stream correction
	*                              types[3]=output stream solution 1
	*                              types[4]=output stream solution 2
	*                              types[5]=log stream rover
	*                              types[6]=log stream base station
	*                              types[7]=log stream correction
	*          char    *paths   I  input stream paths
	*          int     *format  I  input stream formats (STRFMT_???)
	*                              format[0]=input stream rover
	*                              format[1]=input stream base station
	*                              format[2]=input stream correction
	*          int     navsel   I  navigation message select
	*                              (0:rover,1:base,2:ephem,3:all)
	*          char    **cmds   I  input stream start commands
	*                              cmds[0]=input stream rover (NULL: no command)
	*                              cmds[1]=input stream base (NULL: no command)
	*                              cmds[2]=input stream corr (NULL: no command)
	*          char    **cmds_periodic I input stream periodic commands
	*                              cmds[0]=input stream rover (NULL: no command)
	*                              cmds[1]=input stream base (NULL: no command)
	*                              cmds[2]=input stream corr (NULL: no command)
	*          char    **rcvopts I receiver options
	*                              rcvopt[0]=receiver option rover
	*                              rcvopt[1]=receiver option base
	*                              rcvopt[2]=receiver option corr
	*          int     nmeacycle I nmea request cycle (ms) (0:no request)
	*          int     nmeareq  I  nmea request type
	*                              (0:no,1:base pos,2:single sol,3:reset and single)
	*          double *nmeapos  I  transmitted nmea position (ecef) (m)
	*          prcopt_t *prcopt I  rtk processing options
	*          solopt_t *solopt I  solution options
	*                              solopt[0]=solution 1 options
	*                              solopt[1]=solution 2 options
	*          stream_t *moni   I  monitor stream (NULL: not used)
	*          char   *errmsg   O  error message
	* return : status (1:ok 0:error)
	*-----------------------------------------------------------------------------*/
	if (!rtksvrstart(&rtksvr, 0, 0, strs, paths, Format, 3, cmds, cmds_periodic, rcvopts, nmea_cycle, 0, nmeapos, &PrcOpt, solopt, &monistr, errmsg)) {
		trace(2, "rtksvrstart error %s\n", errmsg);
		traceclose();
		return;
	}
	
	
}


void __fastcall Timer(void)
{
	static int n = 0, inactive = 0;
	sol_t* sol;
	int i, update = 0;
	unsigned char buff[8];

	trace(4, "TimerTimer\n");

	rtksvrlock(&rtksvr);

	for (i = 0; i < rtksvr.nsol; i++) {
		sol = rtksvr.solbuf + i;
		//UpdateLog(sol->stat, sol->time, sol->rr, sol->qr, rtksvr.rtk.rb, sol->ns,
			//sol->age, sol->ratio);
		update = 1;
	}
	rtksvr.nsol = 0;
	SolCurrentStat = rtksvr.state ? rtksvr.rtk.sol.stat : 0;

	rtksvrunlock(&rtksvr);

	if (update) {
		//UpdateTime();
		//UpdatePos();
		wcout << "XXX    " << rtksvr.rtcm->sta.pos[0] << "YYY     " << rtksvr.rtcm->sta.pos[1] << "ZZZ    " << rtksvr.rtcm->sta.pos[2] << endl;
		wcout << "OBS    " << rtksvr.rtcm->obs.data[0].code << "SAT     " << rtksvr.rtcm->obs.data[0].sat << "RCV    " << rtksvr.rtcm->obs.data[0].rcv << endl;
		inactive = 0;
	}



	/*if (!(++n % 5)) UpdatePlot();
	UpdateStr();

	if (OpenPort) {
		buff[0] = '\r';
		strwrite(&monistr, buff, 1);
	}*/
}

void __fastcall UpdatePos(void)
{
	//TLabel *label[] = { Plabel1,Plabel2,Plabel3,Pos1,Pos2,Pos3,LabelStd,LabelNSat };
	//wstring  sol[] = { L"----",L"FIX",L"FLOAT",L"SBAS",L"DGPS",L"SINGLE",L"PPP" };
	//UnicodeString s[9], ext = L"";
	//wstring s[9], ext = L"";
	//TColor color[] = { clSilver,clGreen,CLORANGE,clFuchsia,clBlue,clRed,clTeal };
	/*gtime_t time;
	double* rr = SolRov + PSol * 3, * rb = SolRef + PSol * 3, * qr = Qr + PSol * 9, pos[3] = { 0 }, Qe[9] = { 0 };
	double dms1[3] = { 0 }, dms2[3] = { 0 }, bl[3] = { 0 }, enu[3] = { 0 }, pitch = 0.0, yaw = 0.0, len;*/
	//int i, stat = SolStat[PSol];

	//trace(4, "UpdatePos\n");

	//if (rtksvr.rtk.opt.mode == PMODE_STATIC || rtksvr.rtk.opt.mode == PMODE_PPP_STATIC) {
	//	ext = L" (S)";
	//}
	//else if (rtksvr.rtk.opt.mode == PMODE_FIXED || rtksvr.rtk.opt.mode == PMODE_PPP_FIXED) {
	//	ext = L" (F)";
	//}
	//PlabelA->Caption = L"Solution" + ext + L":";
	//Solution->Caption = sol[stat];
	//Solution->Font->Color = rtksvr.state ? color[stat] : clGray;
	//IndSol->Color = rtksvr.state&&stat ? color[stat] : clWhite;
	//if (norm(rr, 3) > 0.0 && norm(rb, 3) > 0.0) {
	//	for (i = 0; i < 3; i++) bl[i] = rr[i] - rb[i];
	//}

	//wchar_t* _buf_char = new wchar_t[100];
	//len = norm(bl, 3);
	//if (SolType == 0) {
	//	if (norm(rr, 3) > 0.0) {
	//		ecef2pos(rr, pos); covenu(pos, qr, Qe);
	//		degtodms(pos[0] * R2D, dms1);
	//		degtodms(pos[1] * R2D, dms2);
	//		if (SolOpt.height == 1) pos[2] -= geoidh(pos); /* geodetic */
	//	}


	//	s[0] = pos[0] < 0 ? wstring(L"S:") : wstring(L"N:");
	//	s[1] = pos[1] < 0 ? L"W:" : L"E:";
	//	s[2] = SolOpt.height == 1 ? L"H:" : L"He:";
		//_swprintf((wchar_t*)s[3].c_str(), L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[0]), CHARDEG, dms1[1], dms1[2]);
		//_buf_char = new wchar_t[100];
	//	_swprintf(_buf_char, L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[0]), CHARDEG, dms1[1], dms1[2]);
	//	s[3] = _buf_char;
	//	//s[3].sprintf(L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[0]), CHARDEG, dms1[1], dms1[2]);
	//	_swprintf(_buf_char, L"%.0f%c %02.0f' %07.4f\"", fabs(dms2[0]), CHARDEG, dms2[1], dms2[2]);
	//	s[4] = _buf_char;
	//	//s[4].sprintf(L"%.0f%c %02.0f' %07.4f\"", fabs(dms2[0]), CHARDEG, dms2[1], dms2[2]);
	//	_swprintf(_buf_char, L"%.3f m", pos[2]);
	//	s[5] = _buf_char;
	//	//s[5].sprintf(L"%.3f m", pos[2]);
	//	_swprintf(_buf_char, L"N:%6.3f E:%6.3f U:%6.3f m", SQRT(Qe[4]), SQRT(Qe[0]), SQRT(Qe[8]));
	//	s[6] = _buf_char;
	//	//s[6].sprintf(L"N:%6.3f E:%6.3f U:%6.3f m", SQRT(Qe[4]), SQRT(Qe[0]), SQRT(Qe[8]));
	//}
	//else if (SolType == 1) {
	//	if (norm(rr, 3) > 0.0) {
	//		ecef2pos(rr, pos); covenu(pos, qr, Qe);
	//		if (SolOpt.height == 1) pos[2] -= geoidh(pos); /* geodetic */
	//	}
	//	s[0] = pos[0] < 0 ? L"S:" : L"N:"; s[1] = pos[1] < 0 ? L"W:" : L"E:";
	//	s[2] = SolOpt.height == 1 ? L"H:" : L"He:";
	//	_swprintf(_buf_char, L"%.8f %c", fabs(pos[0]) * R2D, CHARDEG);
	//	s[3] = _buf_char;
	//	//s[3].sprintf(L"%.8f %c", fabs(pos[0])*R2D, CHARDEG);
	//	_swprintf(_buf_char, L"%.8f %c", fabs(pos[1]) * R2D, CHARDEG);
	//	s[4] = _buf_char;
	//	//s[4].sprintf(L"%.8f %c", fabs(pos[1])*R2D, CHARDEG);
	//	_swprintf(_buf_char, L"%.3f m", pos[2]);
	//	s[5] = _buf_char;
	//	//s[5].sprintf(L"%.3f m", pos[2]);
	//	_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	//	s[6] = _buf_char;
	//	//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	//}
	//else if (SolType == 2) {
	//	s[0] = L"X:"; s[1] = L"Y:"; s[2] = L"Z:";
	//	_swprintf(_buf_char, L"%.3f m", rr[0]);
	//	s[3] = _buf_char;
	//	//s[3].sprintf(L"%.3f m", rr[0]);
	//	_swprintf(_buf_char, L"%.3f m", rr[1]);
	//	s[4] = _buf_char;
	//	//s[4].sprintf(L"%.3f m", rr[1]);
	//	_swprintf(_buf_char, L"%.3f m", rr[2]);
	//	s[5] = _buf_char;
	//	//s[5].sprintf(L"%.3f m", rr[2]);
	//	_swprintf(_buf_char, L"X:%6.3f Y:%6.3f Z:%6.3f m", SQRT(qr[0]), SQRT(qr[4]), SQRT(qr[8]));
	//	s[6] = _buf_char;
	//	//s[6].sprintf(L"X:%6.3f Y:%6.3f Z:%6.3f m", SQRT(qr[0]), SQRT(qr[4]), SQRT(qr[8]));
	//}
	//else if (SolType == 3) {
	//	if (len > 0.0) {
	//		ecef2pos(rb, pos); ecef2enu(pos, bl, enu); covenu(pos, qr, Qe);
	//	}
	//	s[0] = L"E:"; s[1] = L"N:"; s[2] = L"U:";
	//	_swprintf(_buf_char, L"%.3f m", enu[0]);
	//	s[3] = _buf_char;
	//	//s[3].sprintf(L"%.3f m", enu[0]);
	//	_swprintf(_buf_char, L"%.3f m", enu[1]);
	//	s[4] = _buf_char;
	//	//s[4].sprintf(L"%.3f m", enu[1]);
	//	_swprintf(_buf_char, L"%.3f m", enu[2]);
	//	s[5] = _buf_char;
	//	//s[5].sprintf(L"%.3f m", enu[2]);
	//	_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	//	s[6] = _buf_char;
	//	//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	//}
	//else {
	//	if (len > 0.0) {
	//		ecef2pos(rb, pos); ecef2enu(pos, bl, enu); covenu(pos, qr, Qe);
	//		pitch = asin(enu[2] / len);
	//		yaw = atan2(enu[0], enu[1]); if (yaw < 0.0) yaw += 2.0 * PI;
	//	}
	//	s[0] = L"P:"; s[1] = L"Y:"; s[2] = L"L:";
	//	_swprintf(_buf_char, L"%.3f %c", pitch * R2D, CHARDEG);
	//	s[3] = _buf_char;
	//	//s[3].sprintf(L"%.3f %c", pitch*R2D, CHARDEG);
	//	_swprintf(_buf_char, L"%.3f %c", yaw * R2D, CHARDEG);
	//	s[4] = _buf_char;
	//	//s[4].sprintf(L"%.3f %c", yaw*R2D, CHARDEG);
	//	_swprintf(_buf_char, L"%.3f m", len);
	//	s[5] = _buf_char;
	//	//s[5].sprintf(L"%.3f m", len);
	//	_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	//	s[6] = _buf_char;
	//	//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	//}
	//_swprintf(_buf_char, L"Age:%4.1f s Ratio:%4.1f #Sat:%2d", Age[PSol], Ratio[PSol], Nvsat[PSol]);
	//s[7] = _buf_char;
	////s[7].sprintf(L"Age:%4.1f s Ratio:%4.1f #Sat:%2d", Age[PSol], Ratio[PSol], Nvsat[PSol]);
	//if (Ratio[PSol] > 0.0) s[8] = _swprintf(_buf_char, L" R:%4.1f", Ratio[PSol]);
	///*
	//for (i = 0; i < 8; i++) label[i]->Caption = s[i];
	//*/
	//for (i = 0; i < 8; i++) {
	//	wcout << s[i] << '\n';
	//}
	//wcout << rtksvr.rtcm[1].sta.pos[0] << "\t" << rtksvr.rtcm[1].sta.pos[1] << "\t" << rtksvr.rtcm[1].sta.pos[2] << "\n"; //




	/*
	for (i = 3; i < 6; i++) {
		label[i]->Font->Color = PrcOpt.mode == PMODE_MOVEB && SolType <= 2 ? clGray : clBlack;
	}
	IndQ->Color = IndSol->Color;
	SolS->Caption = Solution->Caption;
	SolS->Font->Color = Solution->Font->Color;
	SolQ->Caption = ext + L" " + label[0]->Caption + L" " + label[3]->Caption + L" " +
		label[1]->Caption + L" " + label[4]->Caption + L" " +
		label[2]->Caption + L" " + label[5]->Caption + s[8];
		*/
}