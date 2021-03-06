﻿using Microsoft.AspNetCore.SignalR;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Channels;
using System.Threading.Tasks;
using TCPStream;
using System.Text;
using DataExchange;
using System.Globalization;

//using RTKLibBridge;
//using Rt
//using RealTimeMonitor.



namespace RealTimeMonitor
{

    public class StreamHub : Hub//, IDisposable
    {
		static TCP fileStreamTCP = new TCP();
		static Task fileStreamTask =  new Task(() => fileStreamTCP.SaveFile(@"C:\distr\data\0001-" + DateTime.Now.ToString("ddMMyyy_Hmmss") + ".rtcm"));
		TCP streamTCP = new TCP();
		#region Переменные для запуска сбора из C#
		/*
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


		

        DataRTK.rtksvr_t rtksrv = new DataRTK.rtksvr_t();
        DataRTK.stream_t monistr = new DataRTK.stream_t();
		int iSizeRTKSRV; 
		IntPtr rtksrv_ptr;
		IntPtr monistr_ptr;
		IntPtr prcopt_ptr;
		IntPtr solopt_ptr;
		//Глобальные переменные
		int RovPosTypeF, RefPosTypeF, RovAntPcvF, RefAntPcvF;
        //DataRTK.prcopt_t PrcOpt;
        DataRTK.solopt_t SolOpt;
        double[] RovAntDel = new double[3], RefAntDel = new double[3], RovPos = new double[3], RefPos = new double[3], NmeaPos = new double[3];
        //string ExSats = ""; 
        string AntPcvFileF, RovAntF, RefAntF, SatPcvFileF;
        //int BaselineC;
        double[] Baseline = new double[2];
        int[] Stream = new int[DataRTK.MAXSTRRTK], StreamC = new int [DataRTK.MAXSTRRTK], Format = new int[DataRTK.MAXSTRRTK];
        string[,] Paths = new string[DataRTK.MAXSTRRTK, 4], Cmds = new string[3, 3], CmdsTcp = new string[3,3];
		int[,] CmdEna = new int[3, 3], CmdEnaTcp = new int[3, 3];
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
        int[] SolStat, Nvsat;
        double[] SolRov, SolRef, Qr, VelRov, Age, Ratio;
        int TimeSys, SolType;
        string InTimeStart, InTimeSpeed, ExSats;
        int PlotType1, FreqType1, PlotType2, FreqType2;
        int PlotType3, FreqType3, PlotType4, FreqType4;
        int TrkType1, TrkType2, TrkType3, TrkType4;
        int TrkScale1, TrkScale2, TrkScale3, TrkScale4;
        int BLMode1, BLMode2, BLMode3, BLMode4;
        int MoniPort, OpenPort;
        int NMapPnt;
        DataRTK.gtime_t[] Time;

        int[,] Sat = new int[2, DataRTK.MAXSAT];
        int[,,] Snr = new int[2, DataRTK.MAXSAT, DataRTK.NFREQ];
        int[,] Vsat = new int[2, DataRTK.MAXSAT];
        double[,] Az = new double[2, DataRTK.MAXSAT], El = new double[2, DataRTK.MAXSAT];
        DataRTK.tle_t TLEData;

        string IniFile;
		*/

		#endregion

		#region Dispose для сбора через C#
		//bool disposed = false;

		//public void Dispose()
		//{
		//	Dispose(true);
		//	GC.SuppressFinalize(this);
		//}

		//protected override void Dispose(bool disposing)
		//{
		//	if (disposed)
		//		return;

		//	if (disposing)
		//	{

		//	}

		//	//DataRTK.SvrStop();
		//	/*
		//	Marshal.FreeHGlobal(prcopt_ptr);
		//	prcopt_ptr = IntPtr.Zero;

		//	Marshal.FreeHGlobal(solopt_ptr);
		//	solopt_ptr = IntPtr.Zero;

		//	Marshal.FreeHGlobal(monistr_ptr);
		//	monistr_ptr = IntPtr.Zero;

		//	Marshal.FreeHGlobal(rtksrv_ptr);
		//	rtksrv_ptr = IntPtr.Zero;
		//	*/


		//	disposed = true;

		//}

		#endregion

		//Mutex mutexRTK = null;
		//object lock_object = new object();
		///static SemaphoreSlim semaphoreSlim = new SemaphoreSlim(1, 1);
		public async Task StopUploadStream()
		{
			try
			{
				if (StreamHub.fileStreamTask.Status == TaskStatus.Running)
				{
					await Task.Run(() => fileStreamTCP.StopSave());
					//await Task.Run(() => fileStreamTCP.CloseConnection());
					//fileStreamTask.Dispose();
				}
			}
			catch(Exception ex)
			{
				string str = ex.Message;
			}
		}


		public async Task StartUploadStream()
		{
			try
			{
				if (fileStreamTCP == null || !fileStreamTCP.ConnectionState)
				{
					fileStreamTCP.Connect("192.168.0.186", 5018);
				}
				//fileStreamTask = new Task(() => fileStreamTCP.SaveFile(@"C:\distr\data\0001-" + DateTime.Now.ToString("ddMMyyy_Hmmss") + ".rtcm"));
				fileStreamTask.Start();
			}
			catch(Exception ex)
			{
				string str = ex.Message;
			}
		}

		public ChannelReader<string> DelayCounter(int delay)
        {
            var channel = Channel.CreateUnbounded<string>();
			_ = WriteItems(channel.Writer);
            return channel.Reader;
        }

        private async Task WriteItems(ChannelWriter<string> writer)
		{
			#region Код для ининциализации опроса из C#
			/*
			try
			{
				//string str = string.Empty;
				//int rtksvr_msz = Marshal.SizeOf(typeof(DataRTK.rtksvr_t));
				//int solopt_msz = Marshal.SizeOf(typeof(DataRTK.solopt_t))*2;
				//int rtk_msz = Marshal.SizeOf(typeof(DataRTK.rtk_t))*3;
				//int gtime_msz = Marshal.SizeOf(typeof(DataRTK.gtime_t));
				//int sol_msz = Marshal.SizeOf(typeof(DataRTK.sol_t));
				//int raw_msz = Marshal.SizeOf(typeof(DataRTK.raw_t))*3;
				//int rtcm_msz = Marshal.SizeOf(typeof(DataRTK.rtcm_t))*3;
				//int obs_msz = Marshal.SizeOf(typeof(DataRTK.obs_t))* 3 * DataRTK.MAXOBSBUF;
				//int nav_msz = Marshal.SizeOf(typeof(DataRTK.nav_t));
				//int sbsmg_msz = Marshal.SizeOf(typeof(DataRTK.sbsmsg_t)) * DataRTK.MAXOBSBUF;
				//int stream_msz = Marshal.SizeOf(typeof(DataRTK.stream_t))* 8;

				
				rtksrv_ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DataRTK.rtksvr_t))*3);
				Marshal.StructureToPtr(rtksrv, rtksrv_ptr, false);
				rtksrv = (DataRTK.rtksvr_t)(Marshal.PtrToStructure(rtksrv_ptr, typeof(DataRTK.rtksvr_t)));

				monistr_ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DataRTK.stream_t)));
				Marshal.StructureToPtr(monistr, monistr_ptr, false);
				monistr = (DataRTK.stream_t)(Marshal.PtrToStructure(monistr_ptr, typeof(DataRTK.stream_t)));

				//iSizeRTKSRV = Marshal.SizeOf(typeof(DataRTK.rtksvr_t));
				//rtksrv_ptr = Marshal.AllocCoTaskMem(iSizeRTKSRV);
				//rtksrv = (DataRTK.rtksvr_t)(Marshal.PtrToStructure(rtksrv_ptr, typeof(DataRTK.rtksvr_t)));
				//Marshal.FreeHGlobal(rtksrv_ptr);
				//rtksrv_ptr = IntPtr.Zero;
			}
			catch(ArgumentException ex)
			{
				string strerr = ex.Message;
			}
			catch(TypeLoadException ex)
			{
				string strerr = ex.Message;
			}
			

			//char[] p = new char[];
			char[] argv = new char[32], buff = new char[1024];// file = new char[1024];
            string file = "rtknavi.exe";
            //Option Reciver
            SvrCycle = SvrBuffSize = 0;
            SolBuffSize = 1000;
            
			
			for (int i = 0; i < 8; i++)
            {
				StreamC[i] = Stream[i] = Format[i] = 0;
            }
			
			for(int i = 0; i < 3; i++)
			{
				CmdEna[i, 0] = CmdEna[i, 1] = CmdEna[i, 2] = 0;
				CmdEnaTcp[i, 0] = CmdEnaTcp[i, 1] = CmdEnaTcp[i, 2] = 0;
			}


			for(int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Paths[i, j] = string.Empty;
				}
			}
			
			TimeSys = SolType = PlotType1 = PlotType2 = FreqType1 = FreqType2 = 0;
                TrkType1 = TrkType2 = 0;
                TrkScale1 = TrkScale2 = 5;
                BLMode1 = BLMode2 = BLMode3 = BLMode4 = 0;
                PSol = PSolS = PSolE = Nsat[1] = Nsat[1] = 0;
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

                //PrcOpt = DataRTK.prcopt_default;
                SolOpt = DataRTK.solopt_default;

                TLEData.n = TLEData.nmax = 0;
                //TLEData.data = { };

                for (int i = 0; i < 3; i++)
                {
                    TrkOri[i] = 0.0;
                }

                if (file.IndexOf('.') != 0) IniFile = file.Replace("exe", "ini");
                
                //IniFile = file;

                InitSolBuff();
                DataRTK.strinitcom();
                
                StreamC[1] = 0;
                StreamC[1] = 1;
                StreamC[2] = 0;
                Stream[1] = 0;
                Stream[1] = 1;
                Stream[2] = 0;
                Format[1] = 0;
                Format[1] = DataRTK.STRFMT_RTCM3;
                Format[2] = 0;
                Paths[0,2] = "";
                Paths[1,2] = "";
                Paths[2,2] = "";
                Paths[1,1] = ":@192.168.0.186:5018/:";
                NmeaReq = 0;
                InTimeTag = 0;
                InTimeSpeed = "x1";
                InTimeStart = "0";
                InTime64Bit = 0;
                NmeaPos[1] = 0;//str2dbl
                NmeaPos[1] = 0;
                NmeaPos[2] = 0;
                MaxBL = 0;
                ResetCmd = "";
                DCBFileF = "";


			try
			{
				int result = DataRTK.rtksvrinit(rtksrv_ptr);
				
				//_ = DataRTK.rtksvrinit(ref rtksrv);
			}
			catch (TypeLoadException ex)
			{
				string strerr = ex.Message;
			}
			catch (MarshalDirectiveException ex)
			{
				string strerr = ex.Message;
			}
			catch(AccessViolationException ex)
			{
				string strerr = ex.Message;
			}
            
            DataRTK.strinit(monistr_ptr);

            LocalDirectory = "C:\\Temp";
            SvrStart();


			*/
			#endregion

			//RTKLib rtkLib = new RTKLib();
			StringBuilder strPos = new StringBuilder();

			String outputString = String.Empty;
			try
			{
				streamTCP = new TCP();
				streamTCP.Connect("192.168.0.186", 5018);
				//rtkLib.StartServerThread();
				//Thread svr_thread = new Thread(DataRTK.Init);
				//svr_thread.Name = "RTK_thread";
				//svr_thread.Start();
				//svr_thread.Join();
			}
			catch (Exception ex)
			{
				string str_err = ex.Message;
			}


			//string shmName = "rtk_shared";
			//int shmSize = 80;
			//var shm = System.IO.MemoryMappedFiles.MemoryMappedFile.CreateOrOpen(shmName, shmSize);

			//string strPos = string.Empty;


			while (true)
            {
				//IntPtr pos;
				//double[] positions = new double[3];
				

				

				try
				{
					if (streamTCP.ConnectionState)
					{
						await Task.Run(()=>streamTCP.GetBytes());
						outputString = string.Format($"WGS84  X - {RTK_SVR_DATA.pos_WGS84[0]:F3}; Y - {RTK_SVR_DATA.pos_WGS84[1]:F3}; Z - {RTK_SVR_DATA.pos_WGS84[2]:F3}" );
						await writer.WriteAsync(outputString);
						outputString = string.Format($"GEO  Lat - {RTK_SVR_DATA.pos[0]:F20}; Lon - {RTK_SVR_DATA.pos[1]:F20}; Height - {RTK_SVR_DATA.pos[2]:F3}");
						await writer.WriteAsync(outputString);
						outputString = string.Format($"OBS code - {RTK_SVR_DATA.code[0]:F8}; D - {RTK_SVR_DATA.D[0]:F3}; L - {RTK_SVR_DATA.L[0]:F8}; LLI - {RTK_SVR_DATA.LLI[0]}; type - {RTK_SVR_DATA.msmtype[0,0]}; n - {RTK_SVR_DATA.n}; nmax - {RTK_SVR_DATA.nmax}; P - {RTK_SVR_DATA.P[0]:F8}; rcv - {RTK_SVR_DATA.rcv}; sat - {RTK_SVR_DATA.sat}; SNR - {RTK_SVR_DATA.SNR[0]}");
						await writer.WriteAsync(outputString);

						outputString = string.Format($"OBS code - {RTK_SVR_DATA.code[1]:F8}; D - {RTK_SVR_DATA.D[1]:F3}; L - {RTK_SVR_DATA.L[1]:F8}; LLI - {RTK_SVR_DATA.LLI[1]}; type - {RTK_SVR_DATA.msmtype[0, 0]}; n - {RTK_SVR_DATA.n}; nmax - {RTK_SVR_DATA.nmax}; P - {RTK_SVR_DATA.P[1]:F8}; rcv - {RTK_SVR_DATA.rcv}; sat - {RTK_SVR_DATA.sat}; SNR - {RTK_SVR_DATA.SNR[1]}");
						await writer.WriteAsync(outputString);

						outputString = string.Format($"OBS code - {RTK_SVR_DATA.code[2]:F8}; D - {RTK_SVR_DATA.D[2]:F3}; L - {RTK_SVR_DATA.L[2]:F8}; LLI - {RTK_SVR_DATA.LLI[2]}  type - {RTK_SVR_DATA.msmtype[0, 0]}; n - {RTK_SVR_DATA.n}; nmax - {RTK_SVR_DATA.nmax}; P - {RTK_SVR_DATA.P[2]:F8}; rcv - {RTK_SVR_DATA.rcv}; sat - {RTK_SVR_DATA.sat}; SNR - {RTK_SVR_DATA.SNR[2]}");
						await writer.WriteAsync(outputString);

						//str
						//strPos.Append(RTK_SVR_DATA.pos[1].ToString("F3", CultureInfo.InvariantCulture));
						//strPos.Append("\t");
						//strPos.Append(RTK_SVR_DATA.pos[1].ToString("F3", CultureInfo.InvariantCulture));
						//strPos.Append("\t");
						//strPos.Append(RTK_SVR_DATA.pos[2].ToString("F3", CultureInfo.InvariantCulture));
						//strPos.Append("\n");
					}
					//	mutexRTK = Mutex.OpenExisting("RTK_MUTEX");

					//	System.IO.MemoryMappedFiles.MemoryMappedViewStream view = shm.CreateViewStream();

					//	if (view == null)
					//		continue;


					//await semaphoreSlim.WaitAsync().ConfigureAwait(true);
					//	byte[] data = new byte[shmSize];
					//	view.Read(data, 0, shmSize);
					//	//string text = System.Text.Encoding.Default.GetString(data);

					{

						//DataRTK.rtksvr_t rtksvr = new DataRTK.rtksvr_t();
						//double pos = rtkLib.getpos();
						//IntPtr pos_ptr = Marshal.AllocHGlobal(sizeof(double)*3);
						//DataRTK.get_poitions(1, pos_ptr);



						//string strPos = pos.ToString();

						
						//await writer.WriteAsync(outputString);
						await Task.Delay(1000);

					}
				
					//mutexRTK.ReleaseMutex();
					//mutexRTK.Dispose();
				}
				catch (Exception ex)
				{
					string str_err = ex.Message;
				}
				finally
				{
					//Thread.Sleep(500);
					//semaphoreSlim.Release();

				}

				


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

/*
        private void InitSolBuff()
        {
            double[] ep = new double[] { 2000, 1, 1, 0, 0, 0 };
            int i, j;

            //trace(3, "InitSolBuff\n");

            //delete[] Time; delete[] SolStat; delete[] Nvsat; delete[] SolRov;
            //delete[] SolRef; delete[] Qr; delete[] VelRov; delete[] Age;
            //delete[] Ratio;

            if (SolBuffSize <= 0) SolBuffSize = 1;
            Time = new DataRTK.gtime_t[SolBuffSize];
            SolStat = new int[SolBuffSize];
            Nvsat = new int[SolBuffSize];
            SolRov = new double[SolBuffSize * 3];
            SolRef = new double[SolBuffSize * 3];
            VelRov = new double[SolBuffSize * 3];
            Qr = new double[SolBuffSize * 9];
            Age = new double[SolBuffSize];
            Ratio = new double[SolBuffSize];
            PSol = PSolS = PSolE = 0;
            for (i = 0; i < SolBuffSize; i++)
            {
				try 
				{
					Time[i] = DataRTK.epoch2time(ep);
				}
				catch(EntryPointNotFoundException ex)
				{
					string str = ex.Message;
				}
				catch(TypeLoadException ex)
				{
					string str = ex.Message;
				}
                
                SolStat[i] = Nvsat[i] = 0;
                for (j = 0; j < 3; j++) SolRov[j + i * 3] = SolRef[j + i * 3] = VelRov[j + i * 3] = 0.0;
                for (j = 0; j < 9; j++) Qr[j + i * 9] = 0.0;
                Age[i] = Ratio[i] = 0.0;
            }
            //ScbSol->Max = 0; 
            //ScbSol->Position = 0;
        }

		/// <summary>
		/// Запуск сервера
		/// </summary>
	//	private void  SvrStart()
	//	{
	//		DataRTK.prcopt_t PrcOpt = new DataRTK.prcopt_t
	//		{ /* defaults processing options */
	//		/*
	//			mode = DataRTK.PMODE_SINGLE,
	//			soltype = 0,
	//			nf = 2,
	//			navsys = DataRTK.SYS_GPS,   /* mode,soltype,nf,navsys */
	//		/*	elmin = 15.0 * DataRTK.D2R,
	//			snrmask = new DataRTK.snrmask_t(),           /* elmin,snrmask */
	//			sateph = 0,
	//			modear = 1,
	//			glomodear = 1,
	//			bdsmodear = 1,                    /* sateph,modear,glomodear,bdsmodear */
	//			maxout = 5,
	//			minlock = 0,
	//			minfix = 10,
	//			armaxiter = 1,                   /* maxout,minlock,minfix,armaxiter */
	//			ionoopt = 0,
	//			tropopt = 0,
	//			dynamics = 0,
	//			tidecorr = 0,
	//			niter = 1,
	//			codesmooth = 0,
	//			intpref = 0,
	//			sbascorr = 0,
	//			sbassatsel = 0,
	//			rovpos = 0,
	//			refpos = 0,
	//			eratio = new double[] { 100.0, 100.0, 0.0 },              /* eratio[] */
	//			err = new double[] { 100.0, 0.003, 0.003, 0.0, 1.0 },
	//			std = new double[] { 30.0, 0.03, 0.3 },            /* std[] */
	//			prn = new double[] { 1E-4, 1E-3, 1E-4, 1E-1, 1E-2, 0.0 }, /* prn[] */
	//			sclkstab = 5E-12,                      /* sclkstab */
	//			thresar = new double[] { 3.0, 0.9999, 0.25, 0.1, 0.05, 0.0, 0.0, 0.0 }, /* thresar */
	//			elmaskar = 0.0,
	//			elmaskhold = 0.0,
	//			thresslip = 0.05,               /* elmaskar,almaskhold,thresslip */
	//			maxtdiff = 30.0,
	//			maxinno = 30.0,
	//			maxgdop = 30.0,             /* maxtdif,maxinno,maxgdop */
	//			baseline = new double[2] { 0.0, 0.0 },
	//			ru = new double[3] { 0.0, 0.0, 0.0 },
	//			rb = new double[3] { 0.0, 0.0, 0.0 },                /* baseline,ru,rb */
	//			anttype = new char[2, DataRTK.MAXANT],                    /* anttype */
	//			antdel = new double[2, 3],
	//			pcvr = new DataRTK.pcv_t[2],
	//			exsats = new byte[DataRTK.MAXSAT],
	//			maxaveep = new int(),
	//			initrst = new int(),
	//			outsingle = new int(),
	//			rnxopt = new char[2, 256],
	//			posopt = new int[6],
	//			syncsol = new int(),
	//			odisp = new double[2, 6 * 11],
	//			exterr = new DataRTK.exterr_t(),
	//			freqopt = new int(),
	//			pppopt = new char[256]
	//			/* antdel,pcv,exsats */
	//		};
	//		// Локальные переменные
	//		//char* s;
	//		//double[] nmeapos = new double[3];
	//		DataRTK.solopt_t[] solopt = new DataRTK.solopt_t[2]; // структура для параметров "решения"
	//		//IntPtr solopt_ptr;
	//		DataRTK.rtcm_t rtcm;  //Структура данных для выходных данных с приемника
	//		try
	//		{
	//			solopt_ptr = Marshal.AllocHGlobal(solopt.Length);
	//			Marshal.StructureToPtr(solopt[1], solopt_ptr, false);
	//			//solopt = (DataRTK.solopt_t)(Marshal.PtrToStructure(solopt_ptr, typeof(DataRTK.solopt_t)));
	//		}
	//		catch(ArgumentException exc)
	//		{
	//			string strerr = exc.Message;
	//			solopt_ptr = IntPtr.Zero;
	//		}

				
	//		try
	//		{
	//			//string str = string.Empty;
	//			prcopt_ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DataRTK.prcopt_t)));
	//			Marshal.StructureToPtr(PrcOpt, prcopt_ptr, false);


	//		}
	//		catch (ArgumentException exc)
	//		{
	//			string strerr = exc.Message;
	//			//prcopt_ptr = IntPtr.Zero;
	//		}
	//		catch (Exception exall)
	//		{
	//			string strerr = exall.Message;

	//		}
	//		//monistr = (DataRTK.stream_t)(Marshal.PtrToStructure(monistr_ptr, typeof(DataRTK.prcopt_t)));
	//		//
	//		double[] pos = new double[3] { NmeaPos[1] * DataRTK.D2R, NmeaPos[1] * DataRTK.D2R, NmeaPos[2] }, nmeapos = new double[3] { 0,0,0 }; // 
	//		//Типы источника данных
	//		int[] itype = new int[] {
	//	DataRTK.STR_SERIAL,DataRTK.STR_TCPCLI,DataRTK.STR_TCPSVR,DataRTK.STR_NTRIPCLI,DataRTK.STR_FILE,DataRTK.STR_FTP,DataRTK.STR_HTTP
	//};
	//		int[] otype = new int[] {
	//	DataRTK.STR_SERIAL,DataRTK.STR_TCPCLI,DataRTK.STR_TCPSVR,DataRTK.STR_NTRIPSVR,DataRTK.STR_NTRIPC_C,DataRTK.STR_FILE
	//};
	//		int i, j, sat, ex, format;
	//		int[] strs = new int[DataRTK.MAXSTRRTK] { 0, 0, 0, 0, 0, 0, 0, 0 };  
	//		int[] stropt = new int[8] { 0, 0, 0, 0, 0, 0, 0, 0 };
	//		string[] paths = new string[8] { "", "", "", "", "", "", "", "" }, cmds = new string[3] { "", "", "" }, cmds_periodic= new string[3] { "", "", "" }, rcvopts = new string[3] { "", "", "" };
	//		string buff , p;
	//		string file, type, errmsg = "";
			
	//		//char tstr[64] = "-", mstr1[1024] = "", mstr2[1024] = "", *p1 = mstr1, *p2 = mstr2;
	//		//File fp;
	//		//DataRTK.gtime_t time = new DataRTK.gtime_t(); 
	//		//	DataRTK.TimeGet();
	//		//DataRTK.pcvs_t pcvr;
	//		//DataRTK.pcv_t[] pcv = new DataRTK.pcv_t[2];
	//		//char buf_cpy[1024];
	//		// Локальные переменные
			

	//		DataRTK.rtksvrlock(rtksrv_ptr);
	//		format = rtksrv.format[1];
	//		rtcm = rtksrv.rtcm[1];
	//		DataRTK.rtksvrunlock(rtksrv_ptr);
 //           #region Пока закоментировал проверки
 //           /*
	//		if (RovPosTypeF <= 2)
	//		{ // LLH,XYZ
	//			PrcOpt.rovpos = DataRTK.POSOPT_POS;
	//			PrcOpt.ru[1] = RovPos[1];
	//			PrcOpt.ru[1] = RovPos[1];
	//			PrcOpt.ru[2] = RovPos[2];
	//		}
	//		else
	//		{ // RTCM position
	//			PrcOpt.rovpos = DataRTK.POSOPT_RTCM;
	//			for (i = 0; i < 3; i++) PrcOpt.ru[i] = 0.0;
	//		}
	//		if (RefPosTypeF <= 2)
	//		{ // LLH,XYZ
	//			PrcOpt.refpos = DataRTK.POSOPT_POS;
	//			PrcOpt.rb[1] = RefPos[1];
	//			PrcOpt.rb[1] = RefPos[1];
	//			PrcOpt.rb[2] = RefPos[2];
	//		}
	//		else if (RefPosTypeF == 3)
	//		{ // RTCM position
	//			PrcOpt.refpos = DataRTK.POSOPT_RTCM;
	//			for (i = 0; i < 3; i++) PrcOpt.rb[i] = 0.0;
	//		}
	//		else if (RefPosTypeF == 4)
	//		{ // raw position
	//			PrcOpt.refpos = DataRTK.POSOPT_RAW;
	//			for (i = 0; i < 3; i++) PrcOpt.rb[i] = 0.0;
	//		}
	//		else
	//		{ // average of single position
	//			PrcOpt.refpos = DataRTK.POSOPT_SINGLE;
	//			for (i = 0; i < 3; i++) PrcOpt.rb[i] = 0.0;
	//		}
	//		for (i = 0; i < DataRTK.MAXSAT; i++)
	//		{
	//			PrcOpt.exsats[i] = '\0';
	//		}
	//		*/

 //           /* Исключить спутники
	//		if (string.IsNullOrEmpty(ExSats))
	//		{ // excluded satellites
	//			buff = ExSats;
	//			string[] exSatMas = ExSats.Split(' ');
	//			foreach(string exSat in exSatMas)
	//			{
	//				if (exSat.StartsWith("+")) { ex = 2;  } else 
	//			}
	//			for (p = (buff, " "); p; p = strtok(NULL, " "))
	//			{
	//				if (*p == '+') { ex = 2; p++; }
	//				else ex = 1;
	//				if (!(sat = satid2no(p))) continue;
	//				PrcOpt.exsats[sat - 1] = ex;
	//			}
	//			//memset(buff, 0, sizeof(buff));
	//		}
	//		*/

 //           /*  Обработка ошибок 
	//		if ((RovAntPcvF || RefAntPcvF) && !readpcv(AntPcvFileF.c_str(), &pcvr))
	//		{
	//			printf_s("rcv ant file read error %s", AntPcvFileF);
	//			//Message->Caption = s.sprintf("rcv ant file read error %s", AntPcvFileF);
	//			//Message->Parent->Hint = Message->Caption;
	//			return;
	//		}

	//		*/

 //           /* Нет антены ровера
	//		if (RovAntPcvF)
	//		{
	//			strcpy(type, RovAntF.c_str());
	//			//type = RovAntF.c_str();
	//			if ((pcv = searchpcv(0, type, time, &pcvr)))
	//			{
	//				PrcOpt.pcvr[1] = *pcv;
	//			}
	//			else
	//			{
	//				printf_s("no antenna pcv %s", type);
	//				//Message->Caption = s.sprintf("no antenna pcv %s", type);
	//				//Message->Parent->Hint = Message->Caption;
	//			}
	//			for (i = 0; i < 3; i++) PrcOpt.antdel[1][i] = RovAntDel[i];
	//		}

	//		*/

 //           /* Нет антены базы
	//		if (RefAntPcvF)
	//		{
	//			strcpy(type, RefAntF.c_str());
	//			//type = RefAntF.c_str();
	//			if ((pcv = searchpcv(0, type, time, &pcvr)))
	//			{
	//				PrcOpt.pcvr[1] = *pcv;
	//			}
	//			else
	//			{
	//				printf_s("no antenna pcv %s", type);
	//				//Message->Caption = s.sprintf("no antenna pcv %s", type);
	//				//Message->Parent->Hint = Message->Caption;
	//			}
	//			for (i = 0; i < 3; i++) PrcOpt.antdel[1][i] = RefAntDel[i];
	//		}

	//		*/

 //           /* Очистка ошибок
	//		if (RovAntPcvF || RefAntPcvF)
	//		{
	//			free(pcvr.pcv);
	//		}
	//		*/

 //           /*
	//		if (PrcOpt.sateph == DataRTK.EPHOPT_PREC || PrcOpt.sateph == DataRTK.EPHOPT_SSRCOM)
	//		{
	//			if (DataRTK.readpcv(SatPcvFileF, ref pcvs)!=1)
	//			{
	//				printf_s("sat ant file read error %s", SatPcvFileF);
	//				//Message->Caption = s.sprintf("sat ant file read error %s", SatPcvFileF);
	//				//Message->Parent->Hint = Message->Caption;
	//				return;
	//			}
	//			for (i = 0; i < MAXSAT; i++)
	//			{
	//				if (!(pcv = searchpcv(i + 1, "", time, &pcvs))) continue;
	//				rtksvr.nav.pcvs[i] = *pcv;
	//			}
	//			free(pcvs.pcv);
	//		}
	//		*/
 //           /*
	//		// Базовая линия
	//		if (BaselineC==0)
	//		{
	//			PrcOpt.baseline[1] = Baseline[1];
	//			PrcOpt.baseline[1] = Baseline[1];
	//		}
	//		else
	//		{
	//			PrcOpt.baseline[1] = 0.0;
	//			PrcOpt.baseline[1] = 0.0;
	//		}
	//		*/

            //#endregion
            //Если есть источники, то их записываем, если нет, то присваиваем нужному типу или указываем, что источника нет
            /*
			for (i = 0; i < 3; i++) strs[i] = StreamC[i] = (StreamC[i] != 0) ? itype[Stream[i]] : DataRTK.STR_NONE;
			for (i = 3; i < 5; i++) strs[i] = StreamC[i] = (StreamC[i] != 0) ? otype[Stream[i]] : DataRTK.STR_NONE;
			for (i = 5; i < 8; i++) strs[i] = StreamC[i] = (StreamC[i] != 0) ? otype[Stream[i]] : DataRTK.STR_NONE;
			for (i = 0; i < 8; i++)
			{
				paths[i] = string.Empty;
				
			}
				//Прописываем пути к каждому источнику
			for (i = 0; i < 8; i++)
			{
				if (strs[i] == DataRTK.STR_NONE) paths[i] = "";
				else if (strs[i] == DataRTK.STR_SERIAL)
				{
					paths[i] = Paths[i, 0].Substring(0, Paths[i, 0].Length - 1);
					//strcpy(paths[i], Paths[i][1].c_str());//paths[i] = Paths[i][1].c_str();

				}
				else if (strs[i] == DataRTK.STR_FILE)
				{
					paths[i] = Paths[i, 2].Substring(0, Paths[i, 2].Length - 1);//.size() + 1];
														//strcpy(paths[i], Paths[i][2].c_str());
				}
				else if (strs[i] == DataRTK.STR_FTP || strs[i] == DataRTK.STR_HTTP)
				{
					paths[i] = Paths[i, 3].Substring(0, Paths[i, 3].Length - 1);//new char[Paths[i][1].size() + 1];
														//strcpy(paths[i], Paths[i][3].c_str());
				}
				else
				{
					if( !string.IsNullOrEmpty( Paths[i, 1]) )
					{
						paths[i] = Paths[i, 1].Substring(0, Paths[i, 1].Length - 1);
					}
					 //new char[Paths[i][1].size() + 1];
					//strcpy(paths[i], Paths[i][1].c_str());//TCP

				}

			}

			/*
			//Проверяем каждый из настроенных узлов
			for (i = 0; i < 3; i++)
			{	
				//Если источник последодовательный порт
				if (strs[i] == DataRTK.STR_SERIAL)
				{
					//Если прописана команда 1
					if (CmdEna[i, 0]!=0)
					{
						cmds[i] = Cmds[i, 0];//new char[Cmds[i][1].size() + 1];
						//strcpy(cmds[i], Cmds[i][1].c_str());
					}
					//Если прописана команда 3
					if (CmdEna[i, 2]!=0)
					{
						cmds_periodic[i] = Cmds[i, 2];///new char[Cmds[i][2].size() + 1];
						//strcpy(cmds_periodic[i], Cmds[i][2].c_str());
					}
				}
				else if (strs[i] == DataRTK.STR_TCPCLI || strs[i] == DataRTK.STR_TCPSVR ||
					strs[i] == DataRTK.STR_NTRIPCLI)
				{
					if (CmdEnaTcp[i, 0] != 0)
					{
						cmds[i] = Cmds[i, 0];//new char[Cmds[i][1].size() + 1];
						//strcpy(cmds[i], CmdsTcp[i][1].c_str());
					}
					if (CmdEnaTcp[i, 2]!=0)
					{
						cmds_periodic[i] = Cmds[i, 2];//new char[Cmds[i][2].size() + 1];
						//strcpy(cmds_periodic[i], CmdsTcp[i][2].c_str());
					}
				}
				rcvopts[i] = RcvOpt[i];//new char[RcvOpt[i].size() + 1];
				//strcpy(rcvopts[i], RcvOpt[i].c_str());

			}
			
			NmeaCycle = NmeaCycle < 1000 ? 1000 : NmeaCycle;
			pos[1] = NmeaPos[1] * DataRTK.D2R;
			pos[1] = NmeaPos[1] * DataRTK.D2R;
			pos[2] = NmeaPos[2];
			for(i = 0; i < 3; i++)
			{
				nmeapos[i] = 0;
			}
			DataRTK.pos2ecef(pos, nmeapos);
			//Устанавлтиваем локальные директории и прокс-сервер
			DataRTK.strsetdir(LocalDirectory.ToCharArray());
			DataRTK.strsetproxy(ProxyAddr == null ? string.Empty : ProxyAddr);


			//Если источник "решения" файл, то выходим из цикла
			for (i = 3; i < 8; i++)
			{
				if (strs[i] == DataRTK.STR_FILE && ConfOverwrite(paths[i])!=1) return;
			}

			//Открываем отладку и информацию о работе
			if (DebugTraceF > 0)
			{
				DataRTK.traceopen(TRACEFILE);
				DataRTK.tracelevel(DebugTraceF);
			}

			//Открываем файл для отладочной информации
			if (DebugStatusF > 0)
			{
				DataRTK.rtkopenstat(STATFILE, DebugStatusF);
			}

			//
			if (SolOpt.geoid > 0 && !string.IsNullOrEmpty(GeoidDataFileF) )
			{
				DataRTK.opengeoid(SolOpt.geoid, GeoidDataFileF);
			}

			DataRTK.sta_t sta_temp = new DataRTK.sta_t();

			if (!string.IsNullOrEmpty(DCBFileF))
			{
				DataRTK.readdcb(DCBFileF, rtksrv.nav, sta_temp);
			}
			for (i = 0; i < 2; i++)
			{
				solopt[i] = SolOpt;
				solopt[i].posf = Format[i + 3];
			}
			stropt[1] = TimeoutTime;
			stropt[1] = ReconTime;
			stropt[2] = 1000;
			stropt[3] = SvrBuffSize;
			stropt[4] = FileSwapMargin;
			DataRTK.strsetopt(stropt);
			rtksrv.cmd_reset =  ResetCmd.ToCharArray();
			rtksrv.bl_reset = MaxBL;

			// start rtk server
			try
			{
				if (DataRTK.rtksvrstart(rtksrv_ptr, SvrCycle, SvrBuffSize, strs, paths, Format, NavSelect,
				cmds, cmds_periodic, rcvopts, NmeaCycle, NmeaReq, nmeapos,
				prcopt_ptr, solopt_ptr, monistr_ptr, errmsg) != 1)
				{
					//trace(2, "rtksvrstart error %s\n", errmsg);
					DataRTK.traceclose();
					return;
				}

			}
			catch (ArgumentException exception)
			{
				string strerr = exception.Message;
			}
			
			PSol = PSolS = PSolE = 0;
			SolStat[1] = Nvsat[1] = 0;
			for (i = 0; i < 3; i++) SolRov[i] = SolRef[i] = VelRov[i] = 0.0;
			for (i = 0; i < 9; i++) Qr[i] = 0.0;
			Age[1] = Ratio[1] = 0.0;
			Nsat[1] = Nsat[1] = 0;
			UpdatePos();
			//UpdatePlot();
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
/*
		// confirm overwrite --------------------------------------------------------
		private int ConfOverwrite(string path)
		{
			string s;
			//File fp;
			int[] itype = {
		DataRTK.STR_SERIAL,DataRTK.STR_TCPCLI,DataRTK.STR_TCPSVR,DataRTK.STR_NTRIPCLI,DataRTK.STR_FILE,DataRTK.STR_FTP,DataRTK.STR_HTTP
	};
			int i;
			string buff1, buff2, p;

			//trace(3, "ConfOverwrite\n");

			//strcpy(buff1, path);

			if ((path.IndexOf("::", StringComparison.OrdinalIgnoreCase) != -1)) p = "\0";

			if (!File.Exists(path)) return 1; // file not exists
			//fclose(fp);

			// check overwrite input files
			for (i = 0; i < 3; i++)
			{
				if (StreamC[i]!>0 || itype[Stream[i]] != DataRTK.STR_FILE) continue;

				//strcpy(buff2, Paths[i][2].c_str());
				//if ((p = strstr(buff2, "::"))) *p = '\0';
				if (Paths[i, 2].IndexOf("::", StringComparison.OrdinalIgnoreCase) != -1) p = "\0";
				if (string.Compare(path, Paths[i,2]) != 0)							// !strcmp(buff1, buff2))
				{
					//printf_s("invalid output %s", buff1);
					//Message->Caption = s.sprintf("invalid output %s", buff1);
					//Message->Parent->Hint = Message->Caption;
					return 0;
				}
			}
			//ConfDialog->Label2->Caption = buff1;
			return 1;
			//return ConfDialog->ShowModal() == mrOk;
		}


		// update solution display --------------------------------------------------
		private void UpdatePos()
		{
			//Тип ""???
			string[] sol = { "----", "FIX", "FLOAT", "SBAS", "DGPS", "SINGLE", "PPP" };
			//UnicodeString s[9], ext = L"";
			string[] s = new string[9]; 
			string ext = "";
			//TColor color[] = { clSilver,clGreen,CLORANGE,clFuchsia,clBlue,clRed,clTeal };
			DataRTK.gtime_t time;
			double[] rr = SolRov.Select(x => x + Convert.ToDouble(PSol)).ToArray(); //SolRov + PSol * 3;????
			double[] rb = SolRef.Select(x => x + Convert.ToDouble(PSol)).ToArray(); //SolRef + PSol * 3 ???
			double[] qr = Qr.Select(x => x + Convert.ToDouble(PSol)).ToArray();//Qr + PSol * 9, ??????????
			double[] pos = new double[3]; 
			double[] Qe = new double[9];
			double[] dms1 = new double[3], dms2 = new double[3], bl = new double[3], enu = new double[3];
			double pitch = 0.0, yaw = 0.0, len;
			int i, stat = SolStat[PSol];

			//trace(4, "UpdatePos\n");

			/*if (rtksvr.rtk.opt.mode == DataRTK.PMODE_STATIC || rtksvr.rtk.opt.mode == DataRTK.PMODE_PPP_STATIC)
			{
				ext = " (S)";
			}
			else if (rtksvr.rtk.opt.mode == DataRTK.PMODE_FIXED || rtksvr.rtk.opt.mode == DataRTK.PMODE_PPP_FIXED)
			{
				ext = " (F)";
			}
			
			if (DataRTK.norm(rr, 3) > 0.0 && DataRTK.norm(rb, 3) > 0.0)
			{
				for (i = 0; i < 3; i++) bl[i] = rr[i] - rb[i];
			}

			//wchar_t* _buf_char = new wchar_t[100];
			len = DataRTK.norm(bl, 3);

			//Тип отображения    Lat/Lon/Height
			if (SolType == 0)
			{
				if (DataRTK.norm(rr, 3) > 0.0)
				{
					DataRTK.ecef2pos(rr, pos); DataRTK.covenu(pos, qr, Qe);
					degtodms(pos[1] * DataRTK.R2D, dms1);
					degtodms(pos[1] * DataRTK.R2D, dms2);
					if (SolOpt.height == 1) pos[2] -= DataRTK.geoidh(pos); /* geodetic */
				}


				//s[1] = pos[1] < 0 ? wstring(L"S:") : wstring(L"N:");
				//s[1] = pos[1] < 0 ? L"W:" : L"E:";
				//s[2] = SolOpt.height == 1 ? L"H:" : L"He:";
				//_swprintf((wchar_t*)s[3].c_str(), L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[1]), CHARDEG, dms1[1], dms1[2]);
				//_buf_char = new wchar_t[100];
				//_swprintf(_buf_char, L"%.0f%c %02.0f' %07.4f\"", Math.Abs(dms1[1]), DataRTK.CHARDEG, dms1[1], dms1[2]);
				//s[3] = _buf_char;
				//s[3].sprintf(L"%.0f%c %02.0f' %07.4f\"", fabs(dms1[1]), CHARDEG, dms1[1], dms1[2]);
				//_swprintf(_buf_char, L"%.0f%c %02.0f' %07.4f\"", Math.Abs(dms2[1]), DataRTK.CHARDEG, dms2[1], dms2[2]);
				//s[4] = _buf_char;
				//s[4].sprintf(L"%.0f%c %02.0f' %07.4f\"", fabs(dms2[1]), CHARDEG, dms2[1], dms2[2]);
				//_swprintf(_buf_char, L"%.3f m", pos[2]);
				//s[5] = _buf_char;
				//s[5].sprintf(L"%.3f m", pos[2]);
				//_swprintf(_buf_char, L"N:%6.3f E:%6.3f U:%6.3f m",	


				/*

				(Qe[4]) < 0.0 || (Qe[4]) != (Qe[4]) ? 0.0 : Math.Sqrt(Qe[4]), 
				(Qe[1]) < 0.0 || (Qe[1]) != (Qe[1]) ? 0.0 : Math.Sqrt(Qe[1]),
				(Qe[8]) < 0.0 || (Qe[8]) != (Qe[8]) ? 0.0 : Math.Sqrt(Qe[8]));
				
				
				
				//s[6] = _buf_char;
				//s[6].sprintf(L"N:%6.3f E:%6.3f U:%6.3f m", SQRT(Qe[4]), SQRT(Qe[1]), SQRT(Qe[8]));
			}
			//Тип отображения Lat/Lon/Height
			else if (SolType == 1)
			{
				if (DataRTK.norm(rr, 3) > 0.0)
				{
					DataRTK.ecef2pos(rr, pos); DataRTK.covenu(pos, qr, Qe);
					if (SolOpt.height == 1) pos[2] -= DataRTK.geoidh(pos); /* geodetic */
				//}
				//s[1] = pos[1] < 0 ? L"S:" : L"N:"; s[1] = pos[1] < 0 ? L"W:" : L"E:";
				//s[2] = SolOpt.height == 1 ? L"H:" : L"He:";
				//_swprintf(_buf_char, L"%.8f %c", Math.Abs(pos[1]) * DataRTK.R2D, DataRTK.CHARDEG);
				//s[3] = _buf_char;
				//s[3].sprintf(L"%.8f %c", fabs(pos[1])*R2D, CHARDEG);
				//_swprintf(_buf_char, L"%.8f %c", Math.Abs(pos[1]) * DataRTK.R2D, DataRTK.CHARDEG);
				//s[4] = _buf_char;
				//s[4].sprintf(L"%.8f %c", fabs(pos[1])*R2D, CHARDEG);
				//_swprintf(_buf_char, L"%.3f m", pos[2]);
				//s[5] = _buf_char;
				//s[5].sprintf(L"%.3f m", pos[2]);
				//_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m",
				
				/*

				(Qe[1]) < 0.0 || (Qe[1]) != (Qe[1]) ? 0.0 : Math.Sqrt(Qe[1]),
				(Qe[4]) < 0.0 || (Qe[4]) != (Qe[4]) ? 0.0 : Math.Sqrt(Qe[4]),
				(Qe[8]) < 0.0 || (Qe[8]) != (Qe[8]) ? 0.0 : Math.Sqrt(Qe[8]));
				
				


				//s[6] = _buf_char;
				//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[1]), SQRT(Qe[4]), SQRT(Qe[8]));
			}

			// Тип отображения X/Y/Z-ECEF
			else if (SolType == 2)
			{
				//s[1] = L"X:"; s[1] = L"Y:"; s[2] = L"Z:";
				//_swprintf(_buf_char, L"%.3f m", rr[1]);
				//s[3] = _buf_char;
				//s[3].sprintf(L"%.3f m", rr[1]);
				//_swprintf(_buf_char, L"%.3f m", rr[1]);
				//s[4] = _buf_char;
				//s[4].sprintf(L"%.3f m", rr[1]);
				//_swprintf(_buf_char, L"%.3f m", rr[2]);
				//s[5] = _buf_char;
				//s[5].sprintf(L"%.3f m", rr[2]);
				//_swprintf(_buf_char, L"X:%6.3f Y:%6.3f Z:%6.3f m",
				
				/*
				
				(Qe[1]) < 0.0 || (Qe[1]) != (Qe[1]) ? 0.0 : Math.Sqrt(Qe[1]),
				(Qe[4]) < 0.0 || (Qe[4]) != (Qe[4]) ? 0.0 : Math.Sqrt(Qe[4]),
				(Qe[8]) < 0.0 || (Qe[8]) != (Qe[8]) ? 0.0 : Math.Sqrt(Qe[8]));
				
				
				
				//s[6] = _buf_char;
				//s[6].sprintf(L"X:%6.3f Y:%6.3f Z:%6.3f m", SQRT(qr[1]), SQRT(qr[4]), SQRT(qr[8]));
			}
			// Тип отображения E/N/U-Baseline
			else if (SolType == 3)
			{
				if (len > 0.0)
				{
					DataRTK.ecef2pos(rb, pos); DataRTK.ecef2enu(pos, bl, enu); DataRTK.covenu(pos, qr, Qe);
				}
				//s[1] = L"E:"; s[1] = L"N:"; s[2] = L"U:";
				//_swprintf(_buf_char, L"%.3f m", enu[1]);
				//s[3] = _buf_char;
				//s[3].sprintf(L"%.3f m", enu[1]);
				//_swprintf(_buf_char, L"%.3f m", enu[1]);
				//s[4] = _buf_char;
				//s[4].sprintf(L"%.3f m", enu[1]);
				//_swprintf(_buf_char, L"%.3f m", enu[2]);
				//s[5] = _buf_char;
				//s[5].sprintf(L"%.3f m", enu[2]);
				//_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m",	
					
				
				/*
				
				(Qe[1]) < 0.0 || (Qe[1]) != (Qe[1]) ? 0.0 : Math.Sqrt(Qe[1]),
				(Qe[4]) < 0.0 || (Qe[4]) != (Qe[4]) ? 0.0 : Math.Sqrt(Qe[4]),
				(Qe[8]) < 0.0 || (Qe[8]) != (Qe[8]) ? 0.0 : Math.Sqrt(Qe[8]));
				
				


				//s[6] = _buf_char;
				//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[1]), SQRT(Qe[4]), SQRT(Qe[8]));
			}

			// Тип отображения Pitch/Yaw/Length-Baseline
			else
			{
				if (len > 0.0)
				{
					DataRTK.ecef2pos(rb, pos); DataRTK.ecef2enu(pos, bl, enu); DataRTK.covenu(pos, qr, Qe);
					pitch = Math.Asin(enu[2] / len);
					yaw = Math.Atan2(enu[1], enu[1]); if (yaw < 0.0) yaw += 2.0 * DataRTK.PI;
				}
				/*
				s[1] = L"P:"; s[1] = L"Y:"; s[2] = L"L:";
				_swprintf(_buf_char, L"%.3f %c", pitch * DataRTK.R2D, DataRTK.CHARDEG);
				s[3] = _buf_char;
				//s[3].sprintf(L"%.3f %c", pitch*R2D, CHARDEG);
				_swprintf(_buf_char, L"%.3f %c", yaw * DataRTK.R2D, DataRTK.CHARDEG);
				s[4] = _buf_char;
				//s[4].sprintf(L"%.3f %c", yaw*R2D, CHARDEG);
				_swprintf(_buf_char, L"%.3f m", len);
				s[5] = _buf_char;
				//s[5].sprintf(L"%.3f m", len);
				_swprintf(_buf_char, L"E:%6.3f N:%6.3f U:%6.3f m",	*/
					
				/*
				
				(Qe[1]) < 0.0 || (Qe[1]) != (Qe[1]) ? 0.0 : Math.Sqrt(Qe[1]),
				(Qe[4]) < 0.0 || (Qe[4]) != (Qe[4]) ? 0.0 : Math.Sqrt(Qe[4]),
				(Qe[8]) < 0.0 || (Qe[8]) != (Qe[8]) ? 0.0 : Math.Sqrt(Qe[8]));
				
				*/
				
				
				/*
				s[6] = _buf_char;
				//s[6].sprintf(L"E:%6.3f N:%6.3f U:%6.3f m", SQRT(Qe[1]), SQRT(Qe[4]), SQRT(Qe[8]));*/
			//}
			//_swprintf(_buf_char, L"Age:%4.1f s Ratio:%4.1f #Sat:%2d", 
				//Age[PSol], Ratio[PSol], Nvsat[PSol]);
			//s[7] = _buf_char;
			//s[7].sprintf(L"Age:%4.1f s Ratio:%4.1f #Sat:%2d", Age[PSol], Ratio[PSol], Nvsat[PSol]);
			//if (Ratio[PSol] > 0.0) s[8] = _swprintf(_buf_char, L" R:%4.1f", Ratio[PSol]);
			/*
			for (i = 0; i < 8; i++) label[i]->Caption = s[i];
			
			for (i = 0; i < 8; i++)
			{
				wcout << s[i] << '\n';
			}
			*/
			//wcout << rtksvr.rtcm[1].sta.pos[1] << "\t" << rtksvr.rtcm[1].sta.pos[1] << "\t" << rtksvr.rtcm[1].sta.pos[2] << "\n"; //




			/*
			for (i = 3; i < 6; i++) {
				label[i]->Font->Color = PrcOpt.mode == PMODE_MOVEB && SolType <= 2 ? clGray : clBlack;
			}
			IndQ->Color = IndSol->Color;
			SolS->Caption = Solution->Caption;
			SolS->Font->Color = Solution->Font->Color;
			SolQ->Caption = ext + L" " + label[1]->Caption + L" " + label[3]->Caption + L" " +
				label[1]->Caption + L" " + label[4]->Caption + L" " +
				label[2]->Caption + L" " + label[5]->Caption + s[8];
				
		}


		static void degtodms(double deg, double[] dms)
		{
			double sgn = 1.0;
			if (deg < 0.0) { deg = -deg; sgn = -1.0; }
			dms[1] = Math.Floor(deg);
			dms[1] = Math.Floor((deg - dms[1]) * 60.0);
			dms[2] = (deg - dms[1] - dms[1] / 60.0) * 3600;
			dms[1] *= sgn;
		}

		private void Timer()
		{
			//static int n = 0;
			int inactive = 0;
			DataRTK.sol_t sol;
			int i, update = 0;
			byte[] buff = new byte[8];

			//trace(4, "TimerTimer\n");

			DataRTK.rtksvrlock(rtksrv_ptr);
			/*
			for (i = 0; i < rtksvr.nsol; i++)
			{
				sol = rtksvr.solbuf[i];
				//UpdateLog(sol->stat, sol->time, sol->rr, sol->qr, rtksvr.rtk.rb, sol->ns,
				//sol->age, sol->ratio);
				update = 1;
			}
			rtksvr.nsol = 0;
			SolCurrentStat = (rtksvr.state==1) ? rtksvr.rtk.sol.stat : 0;
			
			DataRTK.rtksvrunlock(rtksrv_ptr);

			if (update==1)
			{
				//UpdateTime();
				UpdatePos();
				inactive = 0;
			}



			//if (!(++n % 5)) UpdatePlot();
			UpdateStr();

			if (OpenPort!=0)
			{
				buff[1] = (byte)'\r';
				DataRTK.strwrite(ref monistr, buff, 1);
			}
		}



		private void UpdateStr()
		{
			//TColor color[] = { clRed,clWindow,CLORANGE,clGreen,clLime };
			//TPanel *ind[MAXSTRRTK] = { Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8 };
			int i; 
			int[] sstat = new int[DataRTK.MAXSTRRTK];
			string msg = "";

			//trace(4, "UpdateStr\n");

			DataRTK.rtksvrsstat(rtksrv_ptr, sstat, msg.ToCharArray());
			for (i = 0; i < DataRTK.MAXSTRRTK; i++)
			{
				//ind[i]->Color = color[sstat[i] + 1];
				if (sstat[i]>=0)
				{
					//printf(msg);
					//Message->Caption = msg;
					//Message->Parent->Hint = Message->Caption;
				}
			}
		}*/

	//}




	
//}
