
#include "RTKLibAdapter.h"


/*

const char MinCharacter{ 'A' };
const char MaxCharacter{ 'z' };

////Public section

MemoryWriter::MemoryWriter(const wstring& name, size_t size) : m_memoryName(name), m_memorySize(size) {
	createSharedMemory();
}


string MemoryWriter::createRandomData() const {
	string data;

	for (size_t i = 0; i < m_memorySize; i++) {
		data += getRandomCharacter();
	}

	return data;
}


void MemoryWriter::write(const string& data) {
	if (!m_shmHandler) {
		return;
	}
	
	auto buffer = MapViewOfFile(m_shmHandler, FILE_MAP_ALL_ACCESS, 0, 0, m_memorySize);

	if (NULL == buffer) {
		std::cerr << "Cannot use MapViewOfFile: null buffer." << std::endl;
		return;
	}

	CopyMemory(buffer, data.c_str(), data.size());
}

/////Private section

char MemoryWriter::getRandomCharacter() const {
	return MinCharacter + rand() % 24;
}


void MemoryWriter::createSharedMemory() {
	m_shmHandler = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_memoryName.c_str());

	if (!m_shmHandler) {
		m_shmHandler = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_memorySize, m_memoryName.c_str());
	}
}
*/

extern __declspec(dllexport) void __stdcall Init(void)
{
	
	//char* p, * argv[32], buff[1024], file[1024] = "rtknavi.exe";


	//Option Reciver
	/*SvrCycle = SvrBuffSize = 0;
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
	SolCurrentStat = 0;*/
	//SolRov = SolRef = Qr = VelRov = Age = Ratio = NULL;
	/*for (int i = 0; i < 2; i++) for (int j = 0; j < MAXSAT; j++) {
		Sat[i][j] = Vsat[i][j] = 0;
		Az[i][j] = El[i][j] = 0.0;
		for (int k = 0; k < NFREQ; k++) Snr[i][j][k] = 0;
	}*/

	PrcOpt = prcopt_default;
	SolOpt = solopt_default;

	//TLEData.n = TLEData.nmax = 0;
	//TLEData.data = NULL;

	//for (int i = 0; i < 3; i++) {
	//	TrkOri[i] = 0.0;
	//}

	//if (!(p = strrchr(file, '.'))) p = file + strlen(file);
	//strcpy(p, ".ini");
	//IniFile = file;

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
}


void  SvrStart(void)
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
	}
	if ((RovAntPcvF || RefAntPcvF) && !readpcv(AntPcvFileF.c_str(), &pcvr)) {
		return;
	}
	if (RovAntPcvF) {
		strcpy(type, RovAntF.c_str());
		//type = RovAntF.c_str();
		if ((pcv = searchpcv(0, type, time, &pcvr))) {
			PrcOpt.pcvr[0] = *pcv;
		}
		}
		for (i = 0; i < 3; i++) PrcOpt.antdel[0][i] = RovAntDel[i];
	//}
	if (RefAntPcvF) {
		strcpy(type, RefAntF.c_str());
		if ((pcv = searchpcv(0, type, time, &pcvr))) {
			PrcOpt.pcvr[1] = *pcv;
		}
		else {
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
}



// confirm overwrite --------------------------------------------------------
int ConfOverwrite(const char* path)
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
			//printf_s("invalid output %s", buff1);
			return 0;
		}
	}
	return 1;
}


// Get position for base station coordinate
extern __declspec(dllexport) double __stdcall getpos() {
	rtksvrlock(&rtksvr);
	double pos = 0.0;
	if (&rtksvr.rtcm[1].sta.pos != nullptr) {
		pos = rtksvr.rtcm[1].sta.pos[0];
	}
	
	
	rtksvrunlock(&rtksvr);
	return pos;
}

// Get position for base station coordinate by num station and pos
extern __declspec(dllexport) double __stdcall getpos(int num_rov, int num_pos) {
	rtksvrlock(&rtksvr);
	double pos = 0.0;
	if (&rtksvr.rtcm[num_rov].sta.pos != nullptr) {
		pos = rtksvr.rtcm[num_rov].sta.pos[num_pos];
	}
	

	rtksvrunlock(&rtksvr);
	return pos;
}


// Get positions for base station coordinates
extern __declspec(dllexport) void __stdcall get_positions(int num_rov, double* pos) {
	rtksvrlock(&rtksvr);
	//double* pos;
	if (&rtksvr.rtcm[num_rov].sta.pos != nullptr) {
		pos = rtksvr.rtcm[num_rov].sta.pos;
	}
	else {
		pos = new double[3] { 0.0, 0.0, 0.0 };
	}

	rtksvrunlock(&rtksvr);
	//return pos;
}


// initialize solution buffer -----------------------------------------------
void InitSolBuff(void)
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
	for(i = 0; i < SolBuffSize; i++) {
		Time[i] = epoch2time(ep);
		SolStat[i] = Nvsat[i] = 0;
		for (j = 0; j < 3; j++) SolRov[j + i * 3] = SolRef[j + i * 3] = VelRov[j + i * 3] = 0.0;
		for (j = 0; j < 9; j++) Qr[j + i * 9] = 0.0;
		Age[i] = Ratio[i] = 0.0;
	}

	// strop rtk server ---------------------------------------------------------
	
	//ScbSol->Max = 0; 
	//ScbSol->Position = 0;
}
extern __declspec(dllexport) void __stdcall SvrStop(void)
{
	char* cmds[3] = { 0 };
	int i, n, m, str;

	trace(3, "SvrStop\n");

	/*for (i = 0; i < 3; i++) {
		str = rtksvr.stream[i].type;

		if (str == STR_SERIAL) {
			if (CmdEna[i][1]) cmds[i] = Cmds[i][1].c_str();
		}
		else if (str == STR_TCPCLI || str == STR_TCPSVR || str == STR_NTRIPCLI) {
			if (CmdEnaTcp[i][1]) cmds[i] = CmdsTcp[i][1].c_str();
		}
	}*/
	rtksvrstop(&rtksvr, cmds);

	/*n = PSolE - PSolS; if (n < 0) n += SolBuffSize;
	m = PSol - PSolS;  if (m < 0) m += SolBuffSize;
	if (n > 0) {
		ScbSol->Max = n - 1; ScbSol->Position = m;
	}*/
	

	if (DebugTraceF > 0) traceclose();
	if (DebugStatusF > 0) rtkclosestat();
	if (OutputGeoidF > 0 && GeoidDataFileF != "") closegeoid();
}