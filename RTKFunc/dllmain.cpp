// dllmain.cpp : Defines the entry point for the DLL application.


#include "pch.h"






//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//                     )
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//
////	std::cout << "Hello World!\n";
//    return TRUE;
//}


extern __declspec(dllexport) void __stdcall Init(void)
{
	/*
	char* p, * argv[32], buff[1024], file[1024] = "rtknavi.exe";


	//Option Reciver
	SvrCycle = SvrBuffSize = 0;
	SolBuffSize = 1000;
	for (int i = 0; i < 8; i++) {
		StreamC[i] = Stream[i] = Format[i] = 0;
		if (i >= 3) {
			continue;
		}
		else {
			CmdEna[i][0] = CmdEna[i][1] = CmdEna[i][2] = 0;
		}

	}
	TimeSys = SolType = PlotType1 = PlotType2 = FreqType1 = FreqType2 = 0;
	TrkType1 = TrkType2 = 0;
	TrkScale1 = TrkScale2 = 5;
	BLMode1 = BLMode2 = BLMode3 = BLMode4 = 0;
	PSol = PSolS = PSolE = Nsat[0] = Nsat[1] = 0;
	NMapPnt = 0;
	OpenPort = 0;
	Time = NULL;
	SolStat = Nvsat = NULL;
	SolCurrentStat = 0;
	SolRov = SolRef = Qr = VelRov = Age = Ratio = NULL;
	for (int i = 0; i < 2; i++) for (int j = 0; j < MAXSAT; j++) {
		Sat[i][j] = Vsat[i][j] = 0;
		Az[i][j] = El[i][j] = 0.0;
		for (int k = 0; k < NFREQ; k++) Snr[i][j][k] = 0;
	}
	*/
	PrcOpt = prcopt_default;
	SolOpt = solopt_default;
	/*
	TLEData.n = TLEData.nmax = 0;
	TLEData.data = NULL;

	for (int i = 0; i < 3; i++) {
		TrkOri[i] = 0.0;
	}

	if (!(p = strrchr(file, '.'))) p = file + strlen(file);
	strcpy(p, ".ini");
	IniFile = file;
	*/
	InitSolBuff();
	strinitcom();

	//strcpy(buff, GetCommandLine());

	StreamC[0] = 0;
	StreamC[1] = 1;
	StreamC[2] = 0;
	Stream[0] = 0;
	Stream[1] = 1;
	Stream[2] = 0;
	Format[0] = NULL;
	Format[1] = STRFMT_RTCM3;
	Format[2] = NULL;
	Paths[0][2] = "";
	Paths[1][2] = "";
	Paths[2][2] = "";
	Paths[1][1] = ":@192.168.0.186:5018/:";
	NmeaReq = 0;
	InTimeTag = 0;
	InTimeSpeed = "x1";
	InTimeStart = "0";
	InTime64Bit = 0;
	NmeaPos[0] = 0;//str2dbl
	NmeaPos[1] = 0;
	NmeaPos[2] = 0;
	MaxBL = 0;
	ResetCmd = "";
	DCBFileF = "";

	rtksvrinit(&rtksvr);
	strinit(&monistr);

	LocalDirectory = "C:\\Temp";
	SvrStart();
	//Option Reciver
	while (true) {
		Timer();
		Sleep(1000);
	}

	
}


extern __declspec(dllexport) void __stdcall Timer(void)
{
	static int n = 0, inactive = 0;
	sol_t* sol;
	int i, update = 0;
	unsigned char buff[8];

	//trace(4, "TimerTimer\n");

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
		UpdatePos();
		inactive = 0;
	}



	if (!(++n % 5)) UpdatePlot();
	//UpdateStr();

	if (OpenPort) {
		buff[0] = '\r';
		strwrite(&monistr, buff, 1);
	}
}

// update stream status indicators ------------------------------------------
extern __declspec(dllexport) void __stdcall UpdateStr(void)
{
	//TColor color[] = { clRed,clWindow,CLORANGE,clGreen,clLime };
	//TPanel *ind[MAXSTRRTK] = { Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8 };
	int i, sstat[MAXSTRRTK] = { 0 };
	char msg[MAXSTRMSG] = "";

	trace(4, "UpdateStr\n");

	rtksvrsstat(&rtksvr, sstat, msg);
	for (i = 0; i < MAXSTRRTK; i++) {
		//ind[i]->Color = color[sstat[i] + 1];
		if (sstat[i]) {
			printf(msg);
			//Message->Caption = msg;
			//Message->Parent->Hint = Message->Caption;
		}
	}
}

extern __declspec(dllexport) void __stdcall SvrStart(void)
{
	// Локальные переменные
	char* s;
	solopt_t solopt[2];

	rtcm_t rtcm;



	double pos[3], nmeapos[3];
	int itype[] = {
		STR_SERIAL,STR_TCPCLI,STR_TCPSVR,STR_NTRIPCLI,STR_FILE,STR_FTP,STR_HTTP
	};
	int otype[] = {
		STR_SERIAL,STR_TCPCLI,STR_TCPSVR,STR_NTRIPSVR,STR_NTRIPC_C,STR_FILE
	};
	int i, strs[MAXSTRRTK] = { 0 }, sat, ex, stropt[8] = { 0 };
	char* paths[8], * cmds[3] = { 0 }, * cmds_periodic[3] = { 0 }, * rcvopts[3] = { 0 };
	char buff[1024], * p;
	char file[1024], * type = 0, errmsg[20148];
	int  j, format;
	char tstr[64] = "-", mstr1[1024] = "", mstr2[1024] = "", * p1 = mstr1, * p2 = mstr2;
	FILE* fp;
	gtime_t time = timeget();
	pcvs_t pcvr = { 0 }, pcvs = { 0 };
	pcv_t* pcv;
	//char buf_cpy[1024];
	// Локальные переменные

	rtksvrlock(&rtksvr);
	format = rtksvr.format[1];
	rtcm = rtksvr.rtcm[1];
	rtksvrunlock(&rtksvr);


	if (RovPosTypeF <= 2) { // LLH,XYZ
		PrcOpt.rovpos = POSOPT_POS;
		PrcOpt.ru[0] = RovPos[0];
		PrcOpt.ru[1] = RovPos[1];
		PrcOpt.ru[2] = RovPos[2];
	}
	else { // RTCM position
		PrcOpt.rovpos = POSOPT_RTCM;
		for (i = 0; i < 3; i++) PrcOpt.ru[i] = 0.0;
	}
	if (RefPosTypeF <= 2) { // LLH,XYZ
		PrcOpt.refpos = POSOPT_POS;
		PrcOpt.rb[0] = RefPos[0];
		PrcOpt.rb[1] = RefPos[1];
		PrcOpt.rb[2] = RefPos[2];
	}
	else if (RefPosTypeF == 3) { // RTCM position
		PrcOpt.refpos = POSOPT_RTCM;
		for (i = 0; i < 3; i++) PrcOpt.rb[i] = 0.0;
	}
	else if (RefPosTypeF == 4) { // raw position
		PrcOpt.refpos = POSOPT_RAW;
		for (i = 0; i < 3; i++) PrcOpt.rb[i] = 0.0;
	}
	else { // average of single position
		PrcOpt.refpos = POSOPT_SINGLE;
		for (i = 0; i < 3; i++) PrcOpt.rb[i] = 0.0;
	}
	for (i = 0; i < MAXSAT; i++) {
		PrcOpt.exsats[i] = 0;
	}

	if (ExSats != "") { // excluded satellites
		strcpy(buff, ExSats.c_str());
		for (p = strtok(buff, " "); p; p = strtok(NULL, " ")) {
			if (*p == '+') { ex = 2; p++; }
			else ex = 1;
			if (!(sat = satid2no(p))) continue;
			PrcOpt.exsats[sat - 1] = ex;
		}
		//memset(buff, 0, sizeof(buff));
	}
	if ((RovAntPcvF || RefAntPcvF) && !readpcv(AntPcvFileF.c_str(), &pcvr)) {
		//printf_s("rcv ant file read error %s", AntPcvFileF);
		//Message->Caption = s.sprintf("rcv ant file read error %s", AntPcvFileF);
		//Message->Parent->Hint = Message->Caption;
		return;
	}
	if (RovAntPcvF) {
		strcpy(type, RovAntF.c_str());
		//type = RovAntF.c_str();
		if ((pcv = searchpcv(0, type, time, &pcvr))) {
			PrcOpt.pcvr[0] = *pcv;
		}
		else {
			printf_s("no antenna pcv %s", type);
			//Message->Caption = s.sprintf("no antenna pcv %s", type);
			//Message->Parent->Hint = Message->Caption;
		}
		for (i = 0; i < 3; i++) PrcOpt.antdel[0][i] = RovAntDel[i];
	}
	if (RefAntPcvF) {
		strcpy(type, RefAntF.c_str());
		//type = RefAntF.c_str();
		if ((pcv = searchpcv(0, type, time, &pcvr))) {
			PrcOpt.pcvr[1] = *pcv;
		}
		else {
			//printf_s("no antenna pcv %s", type);
			//Message->Caption = s.sprintf("no antenna pcv %s", type);
			//Message->Parent->Hint = Message->Caption;
		}
		for (i = 0; i < 3; i++) PrcOpt.antdel[1][i] = RefAntDel[i];
	}
	if (RovAntPcvF || RefAntPcvF) {
		free(pcvr.pcv);
	}
	if (PrcOpt.sateph == EPHOPT_PREC || PrcOpt.sateph == EPHOPT_SSRCOM) {
		if (!readpcv(SatPcvFileF.c_str(), &pcvs)) {
			//printf_s("sat ant file read error %s", SatPcvFileF);
			//Message->Caption = s.sprintf("sat ant file read error %s", SatPcvFileF);
			//Message->Parent->Hint = Message->Caption;
			return;
		}
		for (i = 0; i < MAXSAT; i++) {
			if (!(pcv = searchpcv(i + 1, "", time, &pcvs))) continue;
			rtksvr.nav.pcvs[i] = *pcv;
		}
		free(pcvs.pcv);
	}
	if (BaselineC) {
		PrcOpt.baseline[0] = Baseline[0];
		PrcOpt.baseline[1] = Baseline[1];
	}
	else {
		PrcOpt.baseline[0] = 0.0;
		PrcOpt.baseline[1] = 0.0;
	}

	for (i = 0; i < 3; i++) strs[i] = StreamC[i] ? itype[Stream[i]] : STR_NONE;
	for (i = 3; i < 5; i++) strs[i] = StreamC[i] ? otype[Stream[i]] : STR_NONE;
	for (i = 5; i < 8; i++) strs[i] = StreamC[i] ? otype[Stream[i]] : STR_NONE;
	for (i = 0; i < 8; i++) {
		if (strs[i] == STR_NONE) paths[i] = (char*)"";
		else if (strs[i] == STR_SERIAL) {
			paths[i] = new char[Paths[i][1].size() + 1];
			strcpy(paths[i], Paths[i][0].c_str());//paths[i] = Paths[i][0].c_str();

		}
		else if (strs[i] == STR_FILE) {
			paths[i] = new char[Paths[i][1].size() + 1];
			strcpy(paths[i], Paths[i][2].c_str());
		}
		else if (strs[i] == STR_FTP || strs[i] == STR_HTTP) {
			paths[i] = new char[Paths[i][1].size() + 1];
			strcpy(paths[i], Paths[i][3].c_str());
		}
		else {
			paths[i] = new char[Paths[i][1].size() + 1];
			strcpy(paths[i], Paths[i][1].c_str());//TCP

		}

	}
	for (i = 0; i < 3; i++) {
		if (strs[i] == STR_SERIAL) {
			if (CmdEna[i][0]) {
				cmds[i] = new char[Cmds[i][0].size() + 1];
				strcpy(cmds[i], Cmds[i][0].c_str());
			}
			if (CmdEna[i][2]) {
				cmds_periodic[i] = new char[Cmds[i][2].size() + 1];
				strcpy(cmds_periodic[i], Cmds[i][2].c_str());
			}
		}
		else if (strs[i] == STR_TCPCLI || strs[i] == STR_TCPSVR ||
			strs[i] == STR_NTRIPCLI) {
			if (CmdEnaTcp[i][0]) {
				cmds[i] = new char[Cmds[i][0].size() + 1];
				strcpy(cmds[i], CmdsTcp[i][0].c_str());
			}
			if (CmdEnaTcp[i][2]) {
				cmds_periodic[i] = new char[Cmds[i][2].size() + 1];
				strcpy(cmds_periodic[i], CmdsTcp[i][2].c_str());
			}
		}
		rcvopts[i] = new char[RcvOpt[i].size() + 1];
		strcpy(rcvopts[i], RcvOpt[i].c_str());

	}
	NmeaCycle = NmeaCycle < 1000 ? 1000 : NmeaCycle;
	pos[0] = NmeaPos[0] * D2R;
	pos[1] = NmeaPos[1] * D2R;
	pos[2] = NmeaPos[2];
	pos2ecef(pos, nmeapos);

	strsetdir(LocalDirectory.c_str());
	strsetproxy(ProxyAddr.c_str());

	for (i = 3; i < 8; i++) {
		if (strs[i] == STR_FILE && !ConfOverwrite(paths[i])) return;
	}
	if (DebugTraceF > 0) {
		traceopen(TRACEFILE);
		tracelevel(DebugTraceF);
	}
	if (DebugStatusF > 0) {
		rtkopenstat(STATFILE, DebugStatusF);
	}
	if (SolOpt.geoid > 0 && GeoidDataFileF != "") {
		opengeoid(SolOpt.geoid, GeoidDataFileF.c_str());
	}
	if (DCBFileF != "") {
		readdcb(DCBFileF.c_str(), &rtksvr.nav, NULL);
	}
	for (i = 0; i < 2; i++) {
		solopt[i] = SolOpt;
		solopt[i].posf = Format[i + 3];
	}
	stropt[0] = TimeoutTime;
	stropt[1] = ReconTime;
	stropt[2] = 1000;
	stropt[3] = SvrBuffSize;
	stropt[4] = FileSwapMargin;
	strsetopt(stropt);
	strcpy(rtksvr.cmd_reset, ResetCmd.c_str());
	rtksvr.bl_reset = MaxBL;

	// start rtk server
	if (!rtksvrstart(&rtksvr, SvrCycle, SvrBuffSize, strs, paths, Format, NavSelect,
		cmds, cmds_periodic, rcvopts, NmeaCycle, NmeaReq, nmeapos,
		&PrcOpt, solopt, &monistr, errmsg)) {
		trace(2, "rtksvrstart error %s\n", errmsg);
		traceclose();
		return;
	}
	/*PSol = PSolS = PSolE = 0;
	SolStat[0] = Nvsat[0] = 0;
	for (i = 0; i < 3; i++) SolRov[i] = SolRef[i] = VelRov[i] = 0.0;
	for (i = 0; i < 9; i++) Qr[i] = 0.0;
	Age[0] = Ratio[0] = 0.0;
	Nsat[0] = Nsat[1] = 0;
	UpdatePos();
	UpdatePlot();*/
	/*
	BtnStart->Visible = false;
	BtnOpt->Enabled = false;
	BtnExit->Enabled = false;
	BtnInputStr->Enabled = false;
	MenuStart->Enabled = false;
	MenuExit->Enabled = false;
	ScbSol->Enabled = false;
	BtnStop->Visible = true;
	MenuStop->Enabled = true;
	Svr->Color = CLORANGE;
	SetTrayIcon(0);
	*/
}



// confirm overwrite --------------------------------------------------------
extern __declspec(dllexport) int __stdcall ConfOverwrite(const char* path)
{
	char* s;
	FILE* fp;
	int itype[] = {
		STR_SERIAL,STR_TCPCLI,STR_TCPSVR,STR_NTRIPCLI,STR_FILE,STR_FTP,STR_HTTP
	};
	int i;
	char buff1[1024], buff2[1024], * p;

	trace(3, "ConfOverwrite\n");

	strcpy(buff1, path);

	if ((p = strstr(buff1, "::"))) *p = '\0';

	if (!(fp = fopen(buff1, "r"))) return 1; // file not exists
	fclose(fp);

	// check overwrite input files
	for (i = 0; i < 3; i++) {
		if (!StreamC[i] || itype[Stream[i]] != STR_FILE) continue;

		strcpy(buff2, Paths[i][2].c_str());
		if ((p = strstr(buff2, "::"))) *p = '\0';

		if (!strcmp(buff1, buff2)) {
			printf_s("invalid output %s", buff1);
			//Message->Caption = s.sprintf("invalid output %s", buff1);
			//Message->Parent->Hint = Message->Caption;
			return 0;
		}
	}
	//ConfDialog->Label2->Caption = buff1;
	return 1;
	//return ConfDialog->ShowModal() == mrOk;
}

// update solution plot ------------------------------------------------------
extern __declspec(dllexport) void __stdcall UpdatePlot(void)
{
	/*
	if (Panel22->Visible) {
		DrawPlot(Plot1, PlotType1, FreqType1);
		Disp1->Canvas->CopyRect(Panel22->ClientRect, Plot1->Canvas, Panel22->ClientRect);
	}
	if (Panel23->Visible) {
		DrawPlot(Plot2, PlotType2, FreqType2);
		Disp2->Canvas->CopyRect(Panel23->ClientRect, Plot2->Canvas, Panel23->ClientRect);
	}
	if (Panel24->Visible) {
		DrawPlot(Plot3, PlotType3, FreqType3);
		Disp3->Canvas->CopyRect(Panel24->ClientRect, Plot3->Canvas, Panel24->ClientRect);
	}
	if (Panel25->Visible) {
		DrawPlot(Plot4, PlotType4, FreqType4);
		Disp4->Canvas->CopyRect(Panel25->ClientRect, Plot4->Canvas, Panel25->ClientRect);
	}
	*/
}

// convert degree to deg-min-sec --------------------------------------------
static void degtodms(double deg, double* dms)
{
	double sgn = 1.0;
	if (deg < 0.0) { deg = -deg; sgn = -1.0; }
	dms[0] = floor(deg);
	dms[1] = floor((deg - dms[0]) * 60.0);
	dms[2] = (deg - dms[0] - dms[1] / 60.0) * 3600;
	dms[0] *= sgn;
}

// update solution display --------------------------------------------------
extern __declspec(dllexport) void __stdcall UpdatePos(void)
{
	//TLabel *label[] = { Plabel1,Plabel2,Plabel3,Pos1,Pos2,Pos3,LabelStd,LabelNSat };
	wstring  sol[] = { L"----",L"FIX",L"FLOAT",L"SBAS",L"DGPS",L"SINGLE",L"PPP" };
	//UnicodeString s[9], ext = L"";
	wstring s[9], ext = L"";
	//TColor color[] = { clSilver,clGreen,CLORANGE,clFuchsia,clBlue,clRed,clTeal };
	gtime_t time;
	double* rr = SolRov + PSol * 3, * rb = SolRef + PSol * 3, * qr = Qr + PSol * 9, pos[3] = { 0 }, Qe[9] = { 0 };
	double dms1[3] = { 0 }, dms2[3] = { 0 }, bl[3] = { 0 }, enu[3] = { 0 }, pitch = 0.0, yaw = 0.0, len;
	int i, stat = SolStat[PSol];

	trace(4, "UpdatePos\n");

	if (rtksvr.rtk.opt.mode == PMODE_STATIC || rtksvr.rtk.opt.mode == PMODE_PPP_STATIC) {
		ext = L" (S)";
	}
	else if (rtksvr.rtk.opt.mode == PMODE_FIXED || rtksvr.rtk.opt.mode == PMODE_PPP_FIXED) {
		ext = L" (F)";
	}
	//PlabelA->Caption = L"Solution" + ext + L":";
	//Solution->Caption = sol[stat];
	//Solution->Font->Color = rtksvr.state ? color[stat] : clGray;
	//IndSol->Color = rtksvr.state&&stat ? color[stat] : clWhite;
	if (norm(rr, 3) > 0.0 && norm(rb, 3) > 0.0) {
		for (i = 0; i < 3; i++) bl[i] = rr[i] - rb[i];
	}

	wchar_t* _buf_char = new wchar_t[100];
	len = norm(bl, 3);
	if (SolType == 0) {
		if (norm(rr, 3) > 0.0) {
			ecef2pos(rr, pos); covenu(pos, qr, Qe);
			degtodms(pos[0] * R2D, dms1);
			degtodms(pos[1] * R2D, dms2);
			if (SolOpt.height == 1) pos[2] -= geoidh(pos); /* geodetic */
		}


		s[0] = pos[0] < 0 ? wstring(L"S:") : wstring(L"N:");
		s[1] = pos[1] < 0 ? L"W:" : L"E:";
		s[2] = SolOpt.height == 1 ? L"H:" : L"He:";
		//_swprintf((wchar_t*)s[3].c_str(), L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[0]), CHARDEG, dms1[1], dms1[2]);
		//_buf_char = new wchar_t[100];
		_swprintf(_buf_char, L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[0]), CHARDEG, dms1[1], dms1[2]);
		s[3] = _buf_char;
		//s[3].sprintf(L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[0]), CHARDEG, dms1[1], dms1[2]);
		_swprintf(_buf_char, L"%.0f%c %02.0f' %07.4f\"", fabs(dms2[0]), CHARDEG, dms2[1], dms2[2]);
		s[4] = _buf_char;
		//s[4].sprintf(L"%.0f%c %02.0f' %07.4f\"", fabs(dms2[0]), CHARDEG, dms2[1], dms2[2]);
		_swprintf(_buf_char, L"%.3f m", pos[2]);
		s[5] = _buf_char;
		//s[5].sprintf(L"%.3f m", pos[2]);
		_swprintf(_buf_char, L"N:%6.3f E:%6.3f U:%6.3f m", SQRT(Qe[4]), SQRT(Qe[0]), SQRT(Qe[8]));
		s[6] = _buf_char;
		//s[6].sprintf(L"N:%6.3f E:%6.3f U:%6.3f m", SQRT(Qe[4]), SQRT(Qe[0]), SQRT(Qe[8]));
	}
	else if (SolType == 1) {
		if (norm(rr, 3) > 0.0) {
			ecef2pos(rr, pos); covenu(pos, qr, Qe);
			if (SolOpt.height == 1) pos[2] -= geoidh(pos); /* geodetic */
		}
		s[0] = pos[0] < 0 ? L"S:" : L"N:"; s[1] = pos[1] < 0 ? L"W:" : L"E:";
		s[2] = SolOpt.height == 1 ? L"H:" : L"He:";
		_swprintf(_buf_char, L"%.8f %c", fabs(pos[0]) * R2D, CHARDEG);
		s[3] = _buf_char;
		//s[3].sprintf(L"%.8f %c", fabs(pos[0])*R2D, CHARDEG);
		_swprintf(_buf_char, L"%.8f %c", fabs(pos[1]) * R2D, CHARDEG);
		s[4] = _buf_char;
		//s[4].sprintf(L"%.8f %c", fabs(pos[1])*R2D, CHARDEG);
		_swprintf(_buf_char, L"%.3f m", pos[2]);
		s[5] = _buf_char;
		//s[5].sprintf(L"%.3f m", pos[2]);
		_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
		s[6] = _buf_char;
		//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	}
	else if (SolType == 2) {
		s[0] = L"X:"; s[1] = L"Y:"; s[2] = L"Z:";
		_swprintf(_buf_char, L"%.3f m", rr[0]);
		s[3] = _buf_char;
		//s[3].sprintf(L"%.3f m", rr[0]);
		_swprintf(_buf_char, L"%.3f m", rr[1]);
		s[4] = _buf_char;
		//s[4].sprintf(L"%.3f m", rr[1]);
		_swprintf(_buf_char, L"%.3f m", rr[2]);
		s[5] = _buf_char;
		//s[5].sprintf(L"%.3f m", rr[2]);
		_swprintf(_buf_char, L"X:%6.3f Y:%6.3f Z:%6.3f m", SQRT(qr[0]), SQRT(qr[4]), SQRT(qr[8]));
		s[6] = _buf_char;
		//s[6].sprintf(L"X:%6.3f Y:%6.3f Z:%6.3f m", SQRT(qr[0]), SQRT(qr[4]), SQRT(qr[8]));
	}
	else if (SolType == 3) {
		if (len > 0.0) {
			ecef2pos(rb, pos); ecef2enu(pos, bl, enu); covenu(pos, qr, Qe);
		}
		s[0] = L"E:"; s[1] = L"N:"; s[2] = L"U:";
		_swprintf(_buf_char, L"%.3f m", enu[0]);
		s[3] = _buf_char;
		//s[3].sprintf(L"%.3f m", enu[0]);
		_swprintf(_buf_char, L"%.3f m", enu[1]);
		s[4] = _buf_char;
		//s[4].sprintf(L"%.3f m", enu[1]);
		_swprintf(_buf_char, L"%.3f m", enu[2]);
		s[5] = _buf_char;
		//s[5].sprintf(L"%.3f m", enu[2]);
		_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
		s[6] = _buf_char;
		//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	}
	else {
		if (len > 0.0) {
			ecef2pos(rb, pos); ecef2enu(pos, bl, enu); covenu(pos, qr, Qe);
			pitch = asin(enu[2] / len);
			yaw = atan2(enu[0], enu[1]); if (yaw < 0.0) yaw += 2.0 * PI;
		}
		s[0] = L"P:"; s[1] = L"Y:"; s[2] = L"L:";
		_swprintf(_buf_char, L"%.3f %c", pitch * R2D, CHARDEG);
		s[3] = _buf_char;
		//s[3].sprintf(L"%.3f %c", pitch*R2D, CHARDEG);
		_swprintf(_buf_char, L"%.3f %c", yaw * R2D, CHARDEG);
		s[4] = _buf_char;
		//s[4].sprintf(L"%.3f %c", yaw*R2D, CHARDEG);
		_swprintf(_buf_char, L"%.3f m", len);
		s[5] = _buf_char;
		//s[5].sprintf(L"%.3f m", len);
		_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
		s[6] = _buf_char;
		//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[0]), SQRT(Qe[4]), SQRT(Qe[8]));
	}
	_swprintf(_buf_char, L"Age:%4.1f s Ratio:%4.1f #Sat:%2d", Age[PSol], Ratio[PSol], Nvsat[PSol]);
	s[7] = _buf_char;
	//s[7].sprintf(L"Age:%4.1f s Ratio:%4.1f #Sat:%2d", Age[PSol], Ratio[PSol], Nvsat[PSol]);
	if (Ratio[PSol] > 0.0) s[8] = _swprintf(_buf_char, L" R:%4.1f", Ratio[PSol]);
	/*
	for (i = 0; i < 8; i++) label[i]->Caption = s[i];
	*/
	for (i = 0; i < 8; i++) {
		wcout << s[i] << '\n';
	}
	wcout << rtksvr.rtcm[1].sta.pos[0] << "\t" << rtksvr.rtcm[1].sta.pos[1] << "\t" << rtksvr.rtcm[1].sta.pos[2] << "\n"; //




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


// initialize solution buffer -----------------------------------------------
extern __declspec(dllexport) void __stdcall InitSolBuff(void)
{
	double ep[] = { 2000,1,1,0,0,0 };
	int i, j;

	trace(3, "InitSolBuff\n");

	delete[] Time;   delete[] SolStat; delete[] Nvsat;  delete[] SolRov;
	delete[] SolRef; delete[] Qr;      delete[] VelRov; delete[] Age;
	delete[] Ratio;

	if (SolBuffSize <= 0) SolBuffSize = 1;
	Time = new gtime_t[SolBuffSize];
	SolStat = new int[SolBuffSize];
	Nvsat = new int[SolBuffSize];
	SolRov = new double[SolBuffSize * 3];
	SolRef = new double[SolBuffSize * 3];
	VelRov = new double[SolBuffSize * 3];
	Qr = new double[SolBuffSize * 9];
	Age = new double[SolBuffSize];
	Ratio = new double[SolBuffSize];
	PSol = PSolS = PSolE = 0;
	for (i = 0; i < SolBuffSize; i++) {
		Time[i] = epoch2time(ep);
		SolStat[i] = Nvsat[i] = 0;
		for (j = 0; j < 3; j++) SolRov[j + i * 3] = SolRef[j + i * 3] = VelRov[j + i * 3] = 0.0;
		for (j = 0; j < 9; j++) Qr[j + i * 9] = 0.0;
		Age[i] = Ratio[i] = 0.0;
	}
	//ScbSol->Max = 0; 
	//ScbSol->Position = 0;
}

