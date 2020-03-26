#include "ConsoleRTKLib.h"
#include <iostream>

//Структура для хранения данных
static strsvr_t strsvr;
// Максимальное число потоков
#define MAXSTR 4 

#define MAXHIST		20		   // max number of histories
#define TSTARTMARGIN 60.0	   // time margin for file name replacement
#define TRACEFILE	"rtkconv.trace" // trace file


static int abortf = 0;



int main() {

	time_t rawtime;
	struct tm* timeinfo;

	//Инициализация IP-адреса
	ip_address = ":@192.168.0.186:5018/:";

	//Инициализация формата "сырых" данных
	format = STRFMT_RTCM3;

	//Инициализация активного потока
	stream_type = STR_TCPCLI;

	//Загрузка параметров процесса по-умолчанию
	PrcOpt = prcopt_default;

	//Загрузка параметров решения по-умолчанию
	SolOpt = solopt_default;

	//Инициализация потока сбора данных по-умолчанию
	strsvrinit(&strsvr,3);

	//Параметры по-умолчанию для потока записи RTCM
	int optdef[] = { 10000,10000,1000,32768,10,0 };
	for (int i = 0; i < 6; i++) {
		SvrOpt[i] = optdef[i];
	}


	//Параметры для конвертации
	OutFileEna3 = sep_nav && (NavSys & SYS_GLO);
	OutFileEna4 = sep_nav && (NavSys & SYS_SBS);
	OutFileEna5 = sep_nav && (NavSys & SYS_QZS);
	OutFileEna6 = sep_nav && (NavSys & SYS_GAL);
	OutFileEna7 = sep_nav && (NavSys & SYS_CMP);
	OutFileEna8 = sep_nav && (NavSys & SYS_IRN);
	OutFileEna9 = !rnx;
	//OutDir = OutDirEna;
	//LabelOutDir = OutDirEna;
	OutFile1 = OutFileEna1;
	OutFile2 = OutFileEna2;
	OutFile3 = OutFileEna3 && sep_nav && (NavSys & SYS_GLO);
	OutFile4 = OutFileEna4 && sep_nav && (NavSys & SYS_SBS);
	OutFile5 = OutFileEna5 && sep_nav && (NavSys & SYS_QZS);
	OutFile6 = OutFileEna6 && sep_nav && (NavSys & SYS_GAL);
	OutFile7 = OutFileEna7 && sep_nav && (NavSys & SYS_CMP);
	OutFile8 = OutFileEna8 && sep_nav && (NavSys & SYS_IRN);
	OutFile9 = OutFileEna9 && !rnx;

	if (Nav1) navsys |= SYS_GPS;
	if (Nav2) navsys |= SYS_GLO;
	if (Nav3) navsys |= SYS_GAL;
	if (Nav4) navsys |= SYS_QZS;
	if (Nav5) navsys |= SYS_SBS;
	if (Nav6) navsys |= SYS_CMP;
	if (Nav7) navsys |= SYS_IRN;
	if (Obs1) obstype |= OBSTYPE_PR;
	if (Obs2) obstype |= OBSTYPE_CP;
	if (Obs3) obstype |= OBSTYPE_DOP;
	if (Obs4) obstype |= OBSTYPE_SNR;
	if (Freq1) freqtype |= FREQTYPE_L1;
	if (Freq2) freqtype |= FREQTYPE_L2;
	if (Freq3) freqtype |= FREQTYPE_L5;
	if (Freq4) freqtype |= FREQTYPE_L6;
	if (Freq5) freqtype |= FREQTYPE_L7;
	if (Freq6) freqtype |= FREQTYPE_L8;
	if (Freq7) freqtype |= FREQTYPE_L9;
	FileSwapMargin = 30;

	//AntPos = 0;

	//Инициализация структуры для сбора данных 
	rtksvrinit(&rtksvr);

	//Инициализация потока для сбора данных
	strinit(&monistr);

	//Указание локальной дериктории
	local_dir = "C:\\Temp";

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	if (timeinfo->tm_sec == 0) {
		SvrStopStream();
		printf("Minute is out...\n");
	}
	Sleep(1000);
	
	char buff[4];
	_itoa(timeinfo->tm_year - 100 + 2000, buff, 10);
	strcat(pathString, buff);
	strcat(pathString, "-");
	strcat(fileString, buff);
	strcat(fileString, "-");
	_itoa(timeinfo->tm_mon + 1, buff, 10);
	strcat(pathString, buff);
	strcat(pathString, "-");
	strcat(fileString, buff);
	strcat(fileString, "-");
	_itoa(timeinfo->tm_mday, buff, 10);
	strcat(pathString, buff);
	strcat(pathString, "_");
	strcat(fileString, buff);
	strcat(fileString, "-");
	_itoa(timeinfo->tm_hour, buff, 10);
	strcat(pathString, buff);
	strcat(fileString, buff);
	//strcat(fileString, "-");
	strcat(pathString, ".rtcm3");

	//Запуск службы сбора
	SvrStartStream(pathString);

	while (true) {
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		
		
		if (timeinfo->tm_sec == 0) {
			SvrStopStream();
			
			printf("Minute is out...\n");
			Sleep(1000);
			SetOutFiles(pathString);
			ConvertFile(pathString);


			//RTCM
			pathString[0] = 0;
			fileString[0] = 0;
			strcat(pathString, "C:\\distr\\data\\0001_");
			strcat(fileString, "0001_");
			char buff[4];
			_itoa(timeinfo->tm_year - 100 + 2000, buff, 10);
			strcat(pathString, buff);
			strcat(pathString, "-");
			strcat(fileString, buff);
			strcat(fileString, "-");

			_itoa(timeinfo->tm_mon + 1, buff, 10);
			strcat(pathString, buff);
			strcat(pathString, "-");
			strcat(fileString, buff);
			strcat(fileString, "-");
			_itoa(timeinfo->tm_mday, buff, 10);
			strcat(pathString, buff);
			strcat(pathString, "_");
			strcat(fileString, buff);
			strcat(fileString, "-");
			_itoa(timeinfo->tm_min, buff, 10);
			strcat(pathString, buff);
			strcat(fileString, buff);
			//strcat(fileString, "-");
			strcat(pathString, ".rtcm3");





			SvrStartStream(pathString);
		}
		
	}

}



void __fastcall SvrStart(void)
{
	//Переменная для хранения сообщений об ошибках
	char errmsg[20148];

	/* baseline length to reset (km) */
	double MaxBL = 0;

	//Параметры потока
	int stropt[8] = { 0 };

	//Переменные для хранения параметров выходных решений
	solopt_t solopt[2];
	
	//Структура для данных в формате RTCM
	rtcm_t rtcm;
	
	//Переменные для хранения координат 
	double pos[3], nmeapos[3];
	
	//Инициализация переменной для указания типа потока для всех входящих и исходящих данных
	int	strs[MAXSTRRTK] = { 0 };
	
	//Пути к каждому потоку данных (IP, файл)
	char* paths[8];
	
	//Команды для старта входящих потоков
	char* cmds[3] = { 0 };
	
	//Периодические команды для входящих потоков
	char* cmds_periodic[3] = { 0 };
	
	//Параметры ресивера
	char* rcvopts[3] = { 0 };
	
	//Текущее время
	gtime_t time = timeget();
	
	/* antenna parameters type */
	pcvs_t pcvr = { 0 }, pcvs = { 0 };
	
	/* antenna parameter type */
	pcv_t* pcv;
	

	rtksvrlock(&rtksvr);
	//Задание формата для базовой станции
	format = rtksvr.format[1] = format;
	//Получение структуры 
	rtcm = rtksvr.rtcm[1];
	rtksvrunlock(&rtksvr);


	//Обнуление всех типов потоков
	for (int i = 0; i < 8; i++)
	{
		strs[i] = STR_NONE;
		Format[i] = NULL;
		if (i == 1) {
			//Присвоение базовой станции выбранного адреса
			paths[1] = new char[ip_address.size() + 1];
			strcpy(paths[1], ip_address.c_str());
		}
		else {
			paths[i] = (char*)"";
		}
		
		if (i < 3) {
			rcvopts[i] = new char[RcvOpt[i].size() + 1];
			strcpy(rcvopts[i], RcvOpt[i].c_str());
		}
	}
	//Присвоение базовой станции выбранного потока
	strs[1] = stream_type;
	//Присвоение базовой станции выбранного формата
	Format[1] = format;
	
	
	
	//Установка периода для цикла опроса NMEA
	nmea_cycle = nmea_cycle < 1000 ? 1000 : nmea_cycle;
	//Пересчет координат
	pos[0] = nmeapos[0] * D2R;
	pos[1] = nmeapos[1] * D2R;
	pos[2] = nmeapos[2];
	//Преобразование координат
	pos2ecef(pos, nmeapos);

	//Установка рабочей директории
	strsetdir(local_dir.c_str());
	//strsetproxy(ProxyAddr.c_str());

	for (int i = 0; i < 2; i++) {
		solopt[i] = SolOpt;
		solopt[i].posf = Format[i + 3];
	}

	//Первичные параметры потока
	stropt[0] = 0;
	stropt[1] = 0;
	stropt[2] = 1000;
	stropt[3] = 0;
	stropt[4] = 0;
	//Установка первоначальных параметров потока
	strsetopt(stropt);
	//Копирование команды сброса
	strcpy(rtksvr.cmd_reset, ResetCmd.c_str());
	/* baseline length to reset (km) */
	rtksvr.bl_reset = MaxBL;


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




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
		wcout << "XXX    " << rtksvr.rtcm[1].sta.pos[0] << "YYY     " << rtksvr.rtcm[1].sta.pos[1] << "ZZZ    " << rtksvr.rtcm[1].sta.pos[2] << endl;
		wcout << "OBS    " << rtksvr.rtcm[1].obs.data[0].code << "RCV    " << rtksvr.rtcm[1].obs.data[0].rcv << endl;
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


// start stream server ------------------------------------------------------
void __fastcall SvrStartStream(char pathString[1024])
{
	//Конвертер
	strconv_t* conv[3] = { 0 };
	//Переменная для инициализации путей
	static char str[MAXSTR][1024];
	
	//Тип потока
	int strs[4] = { 0 };
	//Параметры потока
	int	opt[8] = { 0 };
	//Пути и адреса для потоков
	char *paths[MAXSTR];
	//Команды на старт потока
	char* cmds[MAXSTR] = { 0 }; 
	//Периодические команды
	char* cmds_periodic[MAXSTR] = { 0 };
	//Временные переменные для хранения промежуточных данных
	char filepath[1024], buff[1024];
	char* p;
	FILE* fp;

	
	for (int i = 0; i < 4; i++) paths[i] = str[i];

	strs[0] = STR_TCPCLI;
	strs[1] = STR_FILE;
	strs[2] = STR_NONE;
	strs[3] = STR_NONE;

	strcpy(paths[0], ip_address.c_str());
	strcpy(paths[1], pathString);
	strcpy(paths[2], "");
	strcpy(paths[3], "");

	
	for (int i = 0; i < 5; i++) {
		opt[i] = SvrOpt[i];
	}
	opt[5] = NmeaReq ? SvrOpt[5] : 0;
	opt[6] = FileSwapMargin;
	opt[7] = RelayBack;

	for (int i = 1; i < MAXSTR; i++) { // for each out stream
		if (strs[i] != STR_FILE) continue;
		strcpy(filepath, paths[i]);
		if (strstr(filepath, "::A")) continue;
		if ((p = strstr(filepath, "::"))) *p = '\0';
		if (!(fp = fopen(filepath, "r"))) continue;
		fclose(fp);
		
	}
	strsetdir(local_dir.c_str());
	//strsetproxy(ProxyAddress.c_str());

	

	/* start stream server ---------------------------------------------------------
	* start stream server
	* args   : strsvr_t *svr    IO  stream sever struct
	*          int    *opts     I   stream options
	*              opts[0]= inactive timeout (ms)
	*              opts[1]= interval to reconnect (ms)
	*              opts[2]= averaging time of data rate (ms)
	*              opts[3]= receive/send buffer size (bytes);
	*              opts[4]= server cycle (ms)
	*              opts[5]= nmea request cycle (ms) (0:no)
	*              opts[6]= file swap margin (s)
	*              opts[7]= relay back of output stream (0:no)
	*          int    *strs     I   stream types (STR_???)
	*              strs[0]= input stream
	*              strs[1]= output stream 1
	*              strs[2]= output stream 2
	*              strs[3]= output stream 3
	*          char   **paths   I   stream paths
	*              paths[0]= input stream
	*              paths[1]= output stream 1
	*              paths[2]= output stream 2
	*              paths[3]= output stream 3
	*          strcnv **conv    I   stream converter
	*              conv[0]= output stream 1 converter
	*              conv[1]= output stream 2 converter
	*              conv[2]= output stream 3 converter
	*          char   **cmds    I   start commands (NULL: no cmd)
	*              cmds[0]= input stream command
	*              cmds[1]= output stream 1 command
	*              cmds[2]= output stream 2 command
	*              cmds[3]= output stream 3 command
	*          char   **cmds_periodic I periodic commands (NULL: no cmd)
	*              cmds[0]= input stream command
	*              cmds[1]= output stream 1 command
	*              cmds[2]= output stream 2 command
	*              cmds[3]= output stream 3 command
	*          double *nmeapos  I   nmea request position (ecef) (m) (NULL: no)
	* return : status (0:error,1:ok)
	*-----------------------------------------------------------------------------*/
	if (!strsvrstart(&strsvr, opt, strs, paths, conv, cmds, cmds_periodic, AntPos)) {
		return;
	}


	/* set ntrip source table for stream server ------------------------------------
	* set ntrip source table for stream server
	* args   : strsvr_t *svr    IO  stream server struct
	*          char  *file      I   source table file
	* return : none
	*-----------------------------------------------------------------------------*/
	// set ntrip source table
	//strsvrsetsrctbl(&strsvr, SrcTblFile.c_str());

	
}
// stop stream server -------------------------------------------------------
void __fastcall SvrStopStream(void)
{
	
	char* cmds[MAXSTR] = { 0 };
	int strs[MAXSTR];

	strs[0] = STR_TCPCLI;
	strs[1] = STR_FILE;
	strs[2] = STR_NONE;
	strs[3] = STR_NONE;

	
	strsvrstop(&strsvr, cmds);

	

	/*for (int i = 0; i < MAXSTR - 1; i++) {
		strconvfree(strsvr.conv[i]);
	}*/
	//if (TraceLevel > 0) traceclose();
}


void __fastcall LoadOpt(void)
{
	int optdef[] = { 10000,10000,1000,32768,10,0 };
	
	NmeaReq = 0;
	FileSwapMargin = 30;
	RelayBack = 0;
	
	for (int i = 0; i < 6; i++) {
		SvrOpt[i] = optdef[i];
	}
	for (int i = 0; i < 3; i++) {
		AntPos[i] = 0.0;
		AntOff[i] = 0.0;
	}
	RnxTime.time = 0;
	
	
	
}


// set output file paths ----------------------------------------------------
void __fastcall SetOutFiles(string infile)
{
	/*TEdit* edit[] = {
		OutFile1,OutFile2,OutFile3,OutFile4,OutFile5,OutFile6,OutFile7,
		OutFile8,OutFile9
	};*/
	string Format_Text;//= Format;
	string OutDir_Text = OutDir;
	char ofile[10][1024], *code,*p;
	//string in_file = infile;
	int i, lex = strstr(Format_Text.c_str(), "LEX") != NULL;

	if (!EventEna) return;


	if (OutDirEna) {
		//size_t found = infile.rfind('\\');
		//if (found != std::string::npos) in_file.substr(found,in_file.length()) ; //else p = infile.c_str();
		sprintf(ofile[0], "%s%s", OutDir_Text.c_str(),fileString);
	}
	else {
		strcpy(ofile[0], fileString);
	}
	/*for (p = ofile[0]; *p; p++) {
		if (*p == '*' || *p == '?') *p = '0';
	}*/
	if (!RnxFile) {
		if ((p = strrchr(ofile[0], '.'))) *p = '\0';
		sprintf(ofile[1], "%s.obs", ofile[0]);
		sprintf(ofile[2], "%s.nav", ofile[0]);
		sprintf(ofile[3], "%s.gnav", ofile[0]);
		sprintf(ofile[4], "%s.hnav", ofile[0]);
		sprintf(ofile[5], "%s.qnav", ofile[0]);
		sprintf(ofile[6], "%s.lnav", ofile[0]);
		sprintf(ofile[7], "%s.cnav", ofile[0]);
		sprintf(ofile[8], "%s.inav", ofile[0]);
		sprintf(ofile[9], lex ? "%s.lex" : "%s.sbs", ofile[0]);

		/*sprintf(ofile[1], "%s.obs", fileString);
		sprintf(ofile[2], "%s.nav", fileString);
		sprintf(ofile[3], "%s.gnav", fileString);
		sprintf(ofile[4], "%s.hnav", fileString);
		sprintf(ofile[5], "%s.qnav", fileString);
		sprintf(ofile[6], "%s.lnav", fileString);
		sprintf(ofile[7], "%s.cnav", fileString);
		sprintf(ofile[8], "%s.inav", fileString);
		sprintf(ofile[9], lex ? "%s.lex" : "%s.sbs", fileString);*/
	}
	else {
		if ((p = strrchr(ofile[0], '\\'))) *(p + 1) = '\0';
		else ofile[0][0] = '\0';
		sprintf(ofile[1], "%s%%r%%n0.%%yO", ofile[0]);
		if (RnxVer >= 3 && NavSys && (NavSys != SYS_GPS)) { /* ver.3 and mixed system */
			sprintf(ofile[2], "%s%%r%%n0.%%yP", ofile[0]);
		}
		else {
			sprintf(ofile[2], "%s%%r%%n0.%%yN", ofile[0]);
		}
		sprintf(ofile[3], "%s%%r%%n0.%%yG", ofile[0]);
		sprintf(ofile[4], "%s%%r%%n0.%%yH", ofile[0]);
		sprintf(ofile[5], "%s%%r%%n0.%%yQ", ofile[0]);
		sprintf(ofile[6], "%s%%r%%n0.%%yL", ofile[0]);
		sprintf(ofile[7], "%s%%r%%n0.%%yC", ofile[0]);
		sprintf(ofile[8], "%s%%r%%n0.%%yI", ofile[0]);
		sprintf(ofile[9], lex ? "%s%%r%%n0_%%y.lex" : "%s%%r%%n0_%%y.sbs", ofile[0]);
	}
	for (i = 0; i < 9; i++) {
		if (!strcmp(ofile[i + 1], fileString)) strcat(ofile[i + 1], "_");
		//edit[i]->Text = ofile[i + 1];
	}
	OutFile1 = ofile[1];
	OutFile2 = ofile[2];
	OutFile3 = ofile[3];
	OutFile4 = ofile[4];
	OutFile5 = ofile[5];
	OutFile6 = ofile[6];
	OutFile7 = ofile[7];
	OutFile8 = ofile[8];
	OutFile9 = ofile[9];
}


// convert file -------------------------------------------------------------
void __fastcall ConvertFile(char pathString[1024])
{
	rnxopt_t rnxopt = { 0 };
	//string InFile_Text = InFile->Text;
	string InFile_Text = pathString;
	string OutFile1_Text = OutFile1, OutFile2_Text = OutFile2;
	string OutFile3_Text = OutFile3, OutFile4_Text = OutFile4;
	string OutFile5_Text = OutFile5, OutFile6_Text = OutFile6;
	string OutFile7_Text = OutFile7, OutFile8_Text = OutFile8;
	string OutFile9_Text = OutFile9;
	int i, format, sat;
	char file[1024] = "", * ofile[9], ofile_[9][1024] = { "" }, msg[256], * p;
	char buff[256], tstr[32];
	double RNXVER[] = { 2.10,2.11,2.12,3.00,3.01,3.02,3.03 };
	FILE* fp;

	for (i = 0; i < 9; i++) ofile[i] = ofile_[i];

	// recognize input file format
	strcpy(file, InFile_Text.c_str());
	if (!(p = strrchr(file, '.'))) p = file;
	if (FormatConv == 0) { // auto
		if (!strcmp(p, ".rtcm2")) format = STRFMT_RTCM2;
		else if (!strcmp(p, ".rtcm3")) format = STRFMT_RTCM3;
		else if (!strcmp(p, ".gps")) format = STRFMT_OEM4;
		else if (!strcmp(p, ".ubx")) format = STRFMT_UBX;
		else if (!strcmp(p, ".log")) format = STRFMT_SS2;
		else if (!strcmp(p, ".bin")) format = STRFMT_CRES;
		else if (!strcmp(p, ".jps")) format = STRFMT_JAVAD;
		else if (!strcmp(p, ".bnx")) format = STRFMT_BINEX;
		else if (!strcmp(p, ".binex")) format = STRFMT_BINEX;
		else if (!strcmp(p, ".rt17")) format = STRFMT_RT17;
		else if (!strcmp(p, ".cmr")) format = STRFMT_CMR;
		else if (!strcmp(p, ".trs")) format = STRFMT_TERSUS;
		else if (!strcmp(p, ".obs")) format = STRFMT_RINEX;
		else if (!strcmp(p, ".OBS")) format = STRFMT_RINEX;
		else if (!strcmp(p, ".nav")) format = STRFMT_RINEX;
		else if (!strcmp(p, ".NAV")) format = STRFMT_RINEX;
		else if (!strcmp(p + 2, "nav")) format = STRFMT_RINEX;
		else if (!strcmp(p + 2, "NAV")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "o")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "O")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "n")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "N")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "p")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "P")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "g")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "G")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "h")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "H")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "q")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "Q")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "l")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "L")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "c")) format = STRFMT_RINEX;
		else if (!strcmp(p + 3, "C")) format = STRFMT_RINEX;
		else {
			//showmsg("file format can not be recognized");
			return;
		}
	}
	//else {
	//	for (i = 0; formatstrs[i]; i++) {
	//		//if (Format->Text == formatstrs[i]) break;
	//	}
	//	if (formatstrs[i]) format = i; else return;
	//}
	rnxopt.rnxver = RNXVER[RnxVer];

	if (format == STRFMT_RTCM2 || format == STRFMT_RTCM3 || format == STRFMT_RT17 ||
		format == STRFMT_CMR) {

		// input start date/time for rtcm 2, rtcm 3, RT17 or CMR
		//StartDialog->FileName = file;
		//if (StartDialog->ShowModal() != mrOk) return;
		GetFileTime(file);
		rnxopt.trtcm = Time;
	}
	if (OutFile1 != "" && OutFileEna1) strcpy(ofile[0], OutFile1_Text.c_str());
	if (OutFile2 != "" && OutFileEna2) strcpy(ofile[1], OutFile2_Text.c_str());
	if (OutFile3 != "" && OutFileEna3) strcpy(ofile[2], OutFile3_Text.c_str());
	if (OutFile4 != "" && OutFileEna4) strcpy(ofile[3], OutFile4_Text.c_str());
	if (OutFile5 != "" && OutFileEna5) strcpy(ofile[4], OutFile5_Text.c_str());
	if (OutFile6 != "" && OutFileEna6) strcpy(ofile[5], OutFile6_Text.c_str());
	if (OutFile7 != "" && OutFileEna7) strcpy(ofile[6], OutFile7_Text.c_str());
	if (OutFile8 != "" && OutFileEna8) strcpy(ofile[7], OutFile8_Text.c_str());
	if (OutFile9 != "" && OutFileEna9) strcpy(ofile[8], OutFile9_Text.c_str());

	// check overwrite output file
	for (i = 0; i < 9; i++) {
		if (!*ofile[i] || !(fp = fopen(ofile[i], "r"))) continue;
		fclose(fp);
		//ConfDialog->Label2->Caption = ofile[i];
		//if (ConfDialog->ShowModal() != mrOk) return;
	}
	GetTime(&rnxopt.ts, &rnxopt.te, &rnxopt.tint, &rnxopt.tunit);
	strncpy(rnxopt.staid, RnxCode.c_str(), 31);
	sprintf(rnxopt.prog, "%s %s %s", PRGNAME, VER_RTKLIB, PATCH_LEVEL);
	strncpy(rnxopt.runby, RunBy.c_str(), 31);
	strncpy(rnxopt.marker, Marker.c_str(), 63);
	strncpy(rnxopt.markerno, MarkerNo.c_str(), 31);
	strncpy(rnxopt.markertype, MarkerType.c_str(), 31);
	for (i = 0; i < 2; i++) strncpy(rnxopt.name[i], Name[i].c_str(), 31);
	for (i = 0; i < 3; i++) strncpy(rnxopt.rec[i], Rec[i].c_str(), 31);
	for (i = 0; i < 3; i++) strncpy(rnxopt.ant[i], Ant[i].c_str(), 31);
	if (AutoPos) {
		for (i = 0; i < 3; i++) rnxopt.apppos[i] = AppPos[i];
	}
	for (i = 0; i < 3; i++) rnxopt.antdel[i] = AntDel[i];
	strncpy(rnxopt.rcvopt, RcvOption.c_str(), 255);
	rnxopt.navsys = NavSys;
	rnxopt.obstype = ObsType;
	rnxopt.freqtype = FreqType;
	p = rnxopt.comment[0];
	sprintf(p, "log: %-53.53s", file);
	p = rnxopt.comment[1];
	p += sprintf(p, "format: %s", formatstrs[format]);
	if (*rnxopt.rcvopt) sprintf(p, ", option: %s", rnxopt.rcvopt);
	for (i = 0; i < 2; i++) strncpy(rnxopt.comment[i + 2], Comment[i].c_str(), 63);
	for (i = 0; i < 7; i++) strcpy(rnxopt.mask[i], CodeMask[i].c_str());
	rnxopt.autopos = AutoPos;
	rnxopt.scanobs = ScanObs;
	rnxopt.halfcyc = HalfCyc;
	rnxopt.outiono = OutIono;
	rnxopt.outtime = OutTime;
	rnxopt.outleaps = OutLeaps;
	rnxopt.sep_nav = SepNav;
	rnxopt.ttol = TimeTol;

	strcpy(buff, ExSats.c_str());
	for (p = strtok(buff, " "); p; p = strtok(NULL, " ")) {
		if (!(sat = satid2no(p))) continue;
		rnxopt.exsats[sat - 1] = 1;
	}
	abortf = 0;
	

	if (TraceLevel > 0) {
		traceopen(TRACEFILE);
		tracelevel(TraceLevel);
	}
	// convert to rinex
	(void)convrnx(format, &rnxopt, file, ofile);

	if (TraceLevel > 0) {
		traceclose();
	}
	

#if 0
	// set time-start/end if time not specified
	if (!TimeStartF->Checked && rnxopt.tstart.time != 0) {
		time2str(rnxopt.tstart, tstr, 0);
		tstr[10] = '\0';
		TimeY1->Text = tstr;
		TimeH1->Text = tstr + 11;
	}
	if (!TimeEndF->Checked && rnxopt.tend.time != 0) {
		time2str(rnxopt.tend, tstr, 0);
		tstr[10] = '\0';
		TimeY2->Text = tstr;
		TimeH2->Text = tstr + 11;
	}
#endif
	RnxTime = rnxopt.tstart;

	///AddHist(InFile);
}

// get time -----------------------------------------------------------------
void __fastcall GetTime(gtime_t* ts, gtime_t* te, double* tint,	double* tunit)
{


	string TimeY1_Text = TimeY1, TimeH1_Text = TimeH1;
	string TimeY2_Text = TimeY2, TimeH2_Text = TimeH2;
	string TimeInt_Text = TimeInt, TimeUnit_Text = TimeUnit;
	double eps[] = { 2000,1,1,0,0,0 }, epe[] = { 2000,1,1,0,0,0 };

	if (TimeStartF) {
		sscanf(TimeY1_Text.c_str(), "%lf/%lf/%lf", eps, eps + 1, eps + 2);
		sscanf(TimeH1_Text.c_str(), "%lf:%lf:%lf", eps + 3, eps + 4, eps + 5);
		*ts = epoch2time(eps);
	}
	if (TimeEndF) {
		sscanf(TimeY2_Text.c_str(), "%lf/%lf/%lf", epe, epe + 1, epe + 2);
		sscanf(TimeH2_Text.c_str(), "%lf:%lf:%lf", epe + 3, epe + 4, epe + 5);
		*te = epoch2time(epe);
	}
	if (TimeIntF) {
		sscanf(TimeInt_Text.c_str(), "%lf", tint);
	}
	if (TimeUnitF) {
		if (sscanf(TimeUnit_Text.c_str(), "%lf", tunit) >= 1) *tunit *= 3600.0;
	}
}


//---------------------------------------------------------------------------
void __fastcall GetFileTime(const char* fileName)
{
	FILETIME tc, ta, tw;
	SYSTEMTIME st;
	string s;
	HANDLE h;

	if ((h = CreateFileA(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0)) == INVALID_HANDLE_VALUE) {
		return;
	}
	GetFileTime(h, &tc, &ta, &tw);
	CloseHandle(h);
	FileTimeToSystemTime(&tc, &st); // file create time
	char buff[1000];
	sprintf(buff, "%04d/%02d/%02d", st.wYear, st.wMonth, st.wDay);
	TimeY1 = buff;
	sprintf(buff, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	TimeH1 = buff;

	//AnsiString TimeY1_Text = TimeY1->Text, TimeH1_Text = TimeH1->Text;
	double ep[] = { 2000,1,1,0,0,0 };
	sscanf(TimeY1.c_str(), "%lf/%lf/%lf", ep, ep + 1, ep + 2);
	sscanf(TimeH1.c_str(), "%lf:%lf:%lf", ep + 3, ep + 4, ep + 5);
	Time = epoch2time(ep);
	//strftime(TimeY1.c_str(), sizeof(TimeY1) / sizeof(TimeY1[0]), "%04d/%02d/%02d", st.wYear, st.wMonth, st.wDay);
	//TimeY1 = s.sprintf("%04d/%02d/%02d", st.wYear, st.wMonth, st.wDay);
	//TimeH1 = s.sprintf("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
}
//---------------------------------------------------------------------------