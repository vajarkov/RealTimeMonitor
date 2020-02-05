using Microsoft.AspNetCore.SignalR;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Channels;
using System.Threading.Tasks;
//using RealTimeMonitor.



namespace RealTimeMonitor
{
    public class StreamHub : Hub 
    {
        private const int MAXSCALE = 18;
        private const int MAXMAPPNT	= 10;

        private const string PRGNAME = "RTKNAVI";                       // program name
        private const string TRACEFILE = "rtknavi_%Y%m%d%h%M.trace";    // debug trace file
        private const string STATFILE = "rtknavi_%Y%m%d%h%M.stat";      // solution status file
        //#define CLORANGE    (TColor)0x00AAFF
        //#define CLLGRAY     (TColor)0xDDDDDD
        //#define CHARDEG     0x00B0                                    // character code of degree
        private const int SATSIZE = 20;                                 // satellite circle size in skyplot
        private const int MINSNR = 10;                                  // minimum snr
        private const int MAXSNR = 60;                                  // maximum snr
        private const byte KEYF6 = 0x75;                                // code of function key F6
        private const byte KEYF7 = 0x76;                                // code of function key F7
        private const byte KEYF8 = 0x77;                                // code of function key F8
        private const byte KEYF9 = 0x78;                                // code of function key F9
        private const byte KEYF10 = 0x79;                               // code of function key F10
        private const string POSFONTNAME = "Palatino Linotype";
        private const int POSFONTSIZE = 12;
        private const double MINBLLEN = 0.01;                              // minimum baseline length to show
        private const int KACYCLE = 1000;                                  // keep alive cycle (ms)
        private const int TIMEOUT = 10000;                                 // inactive timeout time (ms)
        private const int DEFAULTPORT = 52001;                             // default monitor port number
        private const int MAXPORTOFF = 9;                                  // max port number offset
        private const int MAXTRKSCALE = 23;                                // track scale
        private const int SPLITTER_WIDTH = 6;                              // splitter width
        private const int MAXPANELMODE = 7;                                // max panel mode


        DataRTK.rtksvr_t rtksvr;
        DataRTK.stream_t monistr;

        //Глобальные переменные
        int RovPosTypeF, RefPosTypeF, RovAntPcvF, RefAntPcvF;
        DataRTK.prcopt_t PrcOpt;
        DataRTK.solopt_t SolOpt;
        double[] RovAntDel = new double[3], RefAntDel = new double[3], RovPos = new double[3], RefPos = new double[3], NmeaPos = new double[3];
        //string ExSats = ""; 
        string AntPcvFileF, RovAntF, RefAntF, SatPcvFileF;
        //int BaselineC;
        double[] Baseline = new double[2];
        int[] Stream = new int[DataRTK.MAXSTRRTK], StreamC = new int [DataRTK.MAXSTRRTK], Format = new int[DataRTK.MAXSTRRTK];
        string[,] Paths = new string[DataRTK.MAXSTRRTK, 4], Cmds = new string[3, 3], CmdsTcp = new string[3,3];
        int[,] CmdEna = new int[3, 3], CmdEnaTcp = new int [3, 3];
        int NmeaReq, NmeaCycle, InTimeTag, InTime64Bit;
    
        string TLESatFileF, LocalDirectory;
        string[] PntName = new string[DataRTK.MAXMAPPNT];
        string[] RcvOpt = { "", "", "" };
        string ProxyAddr;
        int DebugTraceF, DebugStatusF, OutputGeoidF, BaselineC;
        string GeoidDataFileF, StaPosFileF, DCBFileF, EOPFileF, TLEFileF;
        int TimeoutTime, ReconTime, SbasCorr, DgpsCorr, TideCorr, FileSwapMargin;
        int SvrCycle, SvrBuffSize, Scale, SolBuffSize, NavSelect, SavedSol;
        string OutSwapInterval, LogSwapInterval, ResetCmd;
        double[] TrkOri = new double[3]; 
        double MaxBL;
        int PSol, PSolS, PSolE,  SolCurrentStat;
        int[] Nsat = new int[2];
        int? SolStat, Nvsat;
        double? SolRov, SolRef, Qr, VelRov, Age, Ratio;
        int TimeSys, SolType;
        string InTimeStart, InTimeSpeed, ExSats;
        int PlotType1, FreqType1, PlotType2, FreqType2;
        int PlotType3, FreqType3, PlotType4, FreqType4;
        int TrkType1, TrkType2, TrkType3, TrkType4;
        int TrkScale1, TrkScale2, TrkScale3, TrkScale4;
        int BLMode1, BLMode2, BLMode3, BLMode4;
        int MoniPort, OpenPort;
        int NMapPnt;
        DataRTK.gtime_t? Time;

        int[,] Sat = new int[2, DataRTK.MAXSAT];
        int[,,] Snr = new int[2, DataRTK.MAXSAT, DataRTK.NFREQ];
        int[,] Vsat = new int[2, DataRTK.MAXSAT];
        double[,] Az = new double[2, DataRTK.MAXSAT], El = new double[2, DataRTK.MAXSAT];
        DataRTK.tle_t TLEData;

        string IniFile;



    public ChannelReader<int> DelayCounter(int delay)
        {
            var channel = Channel.CreateUnbounded<int>();
            _ = WriteItems(channel.Writer, 200, delay);
            return channel.Reader;
        }

        private async Task WriteItems(ChannelWriter<int> writer, int count, int delay)
        {
            while (true)
            {
                char[] p = new char[];
                char[] argv = new char[32], buff = new char[1024];// file = new char[1024];
                string file = "rtknavi.exe";
                //Option Reciver
                SvrCycle = SvrBuffSize = 0;
                SolBuffSize = 1000;
                for (int i = 0; i < 8; i++)
                {
                    StreamC[i] = Stream[i] = Format[i] = CmdEna[i,0] = CmdEna[i,1] = CmdEna[i,2] = 0;
                }
                TimeSys = SolType = PlotType1 = PlotType2 = FreqType1 = FreqType2 = 0;
                TrkType1 = TrkType2 = 0;
                TrkScale1 = TrkScale2 = 5;
                BLMode1 = BLMode2 = BLMode3 = BLMode4 = 0;
                PSol = PSolS = PSolE = Nsat[0] = Nsat[1] = 0;
                NMapPnt = 0;
                OpenPort = 0;
                Time = null;
                SolStat = Nvsat = null;
                SolCurrentStat = 0;
                SolRov = SolRef = Qr = VelRov = Age = Ratio = null;
                for (int i = 0; i < 2; i++) for (int j = 0; j < DataRTK.MAXSAT; j++)
                    {
                        Sat[i, j] = Vsat[i, j] = 0;
                        Az[i, j] = El[i, j] = 0.0;
                        for (int k = 0; k < DataRTK.NFREQ; k++) Snr[i, j, k] = 0;
                    }

                PrcOpt = prcopt_default;
                SolOpt = solopt_default;

                TLEData.n = TLEData.nmax = 0;
                TLEData.data = NULL;

                for (int i = 0; i < 3; i++)
                {
                    TrkOri[i] = 0.0;
                }

                if (!(p = strrchr(file, '.'))) p = file + strlen(file);
                strcpy(p, ".ini");
                IniFile = file;

                InitSolBuff();
                strinitcom();


                await writer.WriteAsync(i);
                await Task.Delay(delay);
            }
            /*
            for(var i = 0; i < count;  i++)
            {
                
                if (i % 5 == 0)
                    delay = delay * 2;

                await writer.WriteAsync(i);
                await Task.Delay(delay);
            }
            */
            writer.TryComplete();
        }
    }
}
