// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cmath>

#include "framework.h"
#include "rtklib.h"

#define MAXSCALE	18
#define MAXMAPPNT	10

#define PRGNAME     "RTKNAVI"           // program name
#define TRACEFILE   "rtknavi_%Y%m%d%h%M.trace" // debug trace file
#define STATFILE    "rtknavi_%Y%m%d%h%M.stat"  // solution status file
#define CLORANGE    (TColor)0x00AAFF
#define CLLGRAY     (TColor)0xDDDDDD
#define CHARDEG     0x00B0              // character code of degree
#define SATSIZE     20                  // satellite circle size in skyplot
#define MINSNR      10                  // minimum snr
#define MAXSNR      60                  // maximum snr
#define KEYF6       0x75                // code of function key F6
#define KEYF7       0x76                // code of function key F7
#define KEYF8       0x77                // code of function key F8
#define KEYF9       0x78                // code of function key F9
#define KEYF10      0x79                // code of function key F10
#define POSFONTNAME "Palatino Linotype"
#define POSFONTSIZE 12
#define MINBLLEN    0.01                // minimum baseline length to show

#define KACYCLE     1000                // keep alive cycle (ms)
#define TIMEOUT     10000               // inactive timeout time (ms)
#define DEFAULTPORT 52001               // default monitor port number
#define MAXPORTOFF  9                   // max port number offset
#define MAXTRKSCALE 23                  // track scale
#define SPLITTER_WIDTH 6                // splitter width
#define MAXPANELMODE 7                  // max panel mode

#define SQRT(x)     ((x)<0.0||(x)!=(x)?0.0:sqrt(x))
#define MIN(x,y)    ((x)<(y)?(x):(y))


void __fastcall UpdatePos(void);
int  __fastcall ConfOverwrite(const char* path);
void __fastcall UpdatePlot(void);
void __fastcall SvrStart(void);
void __fastcall InitSolBuff(void);
void __fastcall Timer(void);
void __fastcall UpdateStr(void);


rtksvr_t rtksvr;
stream_t monistr;
using namespace std;
//Глобальные переменные
int RovPosTypeF, RefPosTypeF, RovAntPcvF, RefAntPcvF;
prcopt_t PrcOpt;
solopt_t SolOpt;
double RovAntDel[3], RefAntDel[3], RovPos[3], RefPos[3], NmeaPos[3];
//string ExSats = ""; 
string AntPcvFileF, RovAntF, RefAntF, SatPcvFileF;
//int BaselineC;
double Baseline[2];
int Stream[MAXSTRRTK], StreamC[MAXSTRRTK], Format[MAXSTRRTK];
string Paths[MAXSTRRTK][4], Cmds[3][3], CmdsTcp[3][3];
int CmdEna[3][3], CmdEnaTcp[3][3];
int NmeaReq, NmeaCycle, InTimeTag, InTime64Bit;
string TLESatFileF, LocalDirectory, PntName[MAXMAPPNT];
string RcvOpt[3] = { "", "", "" }, ProxyAddr;
int DebugTraceF, DebugStatusF, OutputGeoidF, BaselineC;
string GeoidDataFileF, StaPosFileF, DCBFileF, EOPFileF, TLEFileF;
int TimeoutTime, ReconTime, SbasCorr, DgpsCorr, TideCorr, FileSwapMargin;
int SvrCycle, SvrBuffSize, Scale, SolBuffSize, NavSelect, SavedSol;
string OutSwapInterval, LogSwapInterval, ResetCmd;
double TrkOri[3], MaxBL;
int PSol, PSolS, PSolE, Nsat[2], SolCurrentStat;
int* SolStat, * Nvsat;
double* SolRov, * SolRef, * Qr, * VelRov, * Age, * Ratio;
int TimeSys, SolType;
string InTimeStart, InTimeSpeed, ExSats;
int PlotType1, FreqType1, PlotType2, FreqType2;
int PlotType3, FreqType3, PlotType4, FreqType4;
int TrkType1, TrkType2, TrkType3, TrkType4;
int TrkScale1, TrkScale2, TrkScale3, TrkScale4;
int BLMode1, BLMode2, BLMode3, BLMode4;
int MoniPort, OpenPort;
int NMapPnt;
gtime_t* Time;

int Sat[2][MAXSAT], Snr[2][MAXSAT][NFREQ], Vsat[2][MAXSAT];
double Az[2][MAXSAT], El[2][MAXSAT];
tle_t TLEData;

string IniFile;

#endif //PCH_H
