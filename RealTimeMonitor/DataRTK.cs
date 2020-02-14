

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Threading;




namespace RealTimeMonitor
{
    public class DataRTK
    {
        //CriticalHandle CriticalHandle = new CriticalHandle()



        [StructLayout(LayoutKind.Sequential)]
        public struct CRITICAL_SECTION
        {
            public IntPtr DebugInfo;
            public int LockCount;
            public int RecursionCount;
            public IntPtr OwningThread;
            public IntPtr LockSemaphore;
            public UIntPtr SpinCount;
        }



        internal static class DefineConstants
        {
#if WIN32
	public const char FILEPATHSEP = '\\';
#else
            // public const char FILEPATHSEP = '/';
#endif
        }

        #region Константы
        public const int MAXSCALE = 18;
        public const int MAXMAPPNT = 10;

        public const string PRGNAME = "RTKNAVI";         // program name
        /*public const int TRACEFILE   "rtknavi_%Y%m%d%h%M.trace" // debug trace file
        public const int STATFILE    "rtknavi_%Y%m%d%h%M.stat"  // solution status file
        public const int CLORANGE    (TColor)0x00AAFF
        public const int CLLGRAY     (TColor)0xDDDDDD
        */
        public const int CHARDEG = 0x00B0;// character code of degree
        public const int SATSIZE = 20;// satellite circle size in skyplot
        public const int MINSNR = 10;// minimum snr
        public const int MAXSNR = 60;// maximum snr
        public const int KEYF6 = 0x75;// code of function key F6
        public const int KEYF7 = 0x76;// code of function key F7
        public const int KEYF8 = 0x77;// code of function key F8
        public const int KEYF9 = 0x78;// code of function key F9
        public const int KEYF10 = 0x79;// code of function key F10
        /*
            public const int POSFONTNAME "Palatino Linotype"
        public const int POSFONTSIZE 12
        public const int MINBLLEN    0.01                // minimum baseline length to show
        */
        public const int KACYCLE = 1000;// keep alive cycle (ms)
        public const int TIMEOUT = 10000;// inactive timeout time (ms)
        public const int DEFAULTPORT = 52001;// default monitor port number
        public const int MAXPORTOFF = 9;// max port number offset
        public const int MAXTRKSCALE = 23;// track scale
        public const int SPLITTER_WIDTH = 6;// splitter width
        public const int MAXPANELMODE = 7;// max panel mode


        public const double PI = 3.1415926535897932;/* pi */
        public const double D2R = (PI / 180.0);/* deg to rad */
        public const double R2D = (180.0 / PI);/* rad to deg */
        public const double CLIGHT = 299792458.0;/* speed of light (m/s) */
        public const double SC2RAD = 3.1415926535898;/* semi-circle to radian (IS-GPS) */
        public const double AU = 149597870691.0;/* 1 AU (m) */
        public const double AS2R = (D2R / 3600.0);/* arc sec to radian */

        public const double OMGE = 7.2921151467E-5;/* earth angular velocity (IS-GPS) (rad/s) */

        public const double RE_WGS84 = 6378137.0;/* earth semimajor axis (WGS84) (m) */
        public const double FE_WGS84 = (1.0 / 298.257223563);/* earth flattening (WGS84) */

        public const double HION = 350000.0;/* ionosphere height (m) */

        public const int MAXFREQ = 7;/* max NFREQ */

        public const double FREQ1 = 1.57542E9;/* L1/E1  frequency (Hz) */
        public const double FREQ2 = 1.22760E9;/* L2     frequency (Hz) */
        public const double FREQ5 = 1.17645E9;/* L5/E5a frequency (Hz) */
        public const double FREQ6 = 1.27875E9;/* E6/LEX frequency (Hz) */
        public const double FREQ7 = 1.20714E9;/* E5b    frequency (Hz) */
        public const double FREQ8 = 1.191795E9;/* E5a+b  frequency (Hz) */
        public const double FREQ9 = 2.492028E9;/* S      frequency (Hz) */
        public const double FREQ1_GLO = 1.60200E9;/* GLONASS G1 base frequency (Hz) */
        public const double DFRQ1_GLO = 0.56250E6;/* GLONASS G1 bias frequency (Hz/n) */
        public const double FREQ2_GLO = 1.24600E9;/* GLONASS G2 base frequency (Hz) */
        public const double DFRQ2_GLO = 0.43750E6;/* GLONASS G2 bias frequency (Hz/n) */
        public const double FREQ3_GLO = 1.202025E9;/* GLONASS G3 frequency (Hz) */
        public const double FREQ1_CMP = 1.561098E9;/* BeiDou B1 frequency (Hz) */
        public const double FREQ2_CMP = 1.20714E9;/* BeiDou B2 frequency (Hz) */
        public const double FREQ3_CMP = 1.26852E9;/* BeiDou B3 frequency (Hz) */

        public const double EFACT_GPS = 1.0;/* error factor: GPS */
        public const double EFACT_GLO = 1.5;/* error factor: GLONASS */
        public const double EFACT_GAL = 1.0;/* error factor: Galileo */
        public const double EFACT_QZS = 1.0;/* error factor: QZSS */
        public const double EFACT_CMP = 1.0;/* error factor: BeiDou */
        public const double EFACT_IRN = 1.5;/* error factor: IRNSS */
        public const double EFACT_SBS = 3.0;/* error factor: SBAS */

        public const int SYS_NONE = 0x00;/* navigation system: none */
        public const int SYS_GPS = 0x01;/* navigation system: GPS */
        public const int SYS_SBS = 0x02;/* navigation system: SBAS */
        public const int SYS_GLO = 0x04;/* navigation system: GLONASS */
        public const int SYS_GAL = 0x08;/* navigation system: Galileo */
        public const int SYS_QZS = 0x10;/* navigation system: QZSS */
        public const int SYS_CMP = 0x20;/* navigation system: BeiDou */
        public const int SYS_IRN = 0x40;/* navigation system: IRNS */
        public const int SYS_LEO = 0x80;/* navigation system: LEO */
        public const int SYS_ALL = 0xFF;/* navigation system: all */

        public const int TSYS_GPS = 0;/* time system: GPS time */
        public const int TSYS_UTC = 1;/* time system: UTC */
        public const int TSYS_GLO = 2;/* time system: GLONASS time */
        public const int TSYS_GAL = 3;/* time system: Galileo time */
        public const int TSYS_QZS = 4;/* time system: QZSS time */
        public const int TSYS_CMP = 5;/* time system: BeiDou time */
        public const int TSYS_IRN = 6;/* time system: IRNSS time */

#if !NFREQ
        public const int NFREQ = 3;/* number of carrier frequencies */
#endif
        public const int NFREQGLO = 2;/* number of carrier frequencies of GLONASS */

#if !NEXOBS
        public const int NEXOBS = 0;/* number of extended obs codes */
#endif

        public const int MINPRNGPS = 1;/* min satellite PRN number of GPS */
        public const int MAXPRNGPS = 32;/* max satellite PRN number of GPS */
        public const int NSATGPS = (MAXPRNGPS - MINPRNGPS + 1);/* number of GPS satellites */
        public const int NSYSGPS = 1;

#if ENAGLO
                public const int MINPRNGLO = 1;/* min satellite slot number of GLONASS */
                public const int MAXPRNGLO = 27;/* max satellite slot number of GLONASS */
                public const int NSATGLO = (MAXPRNGLO - MINPRNGLO + 1);/* number of GLONASS satellites */
                public const int NSYSGLO = 1;
#else
        public const int MINPRNGLO = 0;
        public const int MAXPRNGLO = 0;
        public const int NSATGLO = 0;
        public const int NSYSGLO = 0;
#endif
#if ENAGAL
                public const int MINPRNGAL = 1;/* min satellite PRN number of Galileo */
                public const int MAXPRNGAL = 36;/* max satellite PRN number of Galileo */
                public const int NSATGAL = (MAXPRNGAL - MINPRNGAL + 1); /* number of Galileo satellites */
                public const int NSYSGAL = 1;
#else
        public const int MINPRNGAL = 0;
        public const int MAXPRNGAL = 0;
        public const int NSATGAL = 0;
        public const int NSYSGAL = 0;
#endif
#if ENAQZS
                public const int MINPRNQZS = 193;/* min satellite PRN number of QZSS */
                public const int MAXPRNQZS = 202;/* max satellite PRN number of QZSS */
                public const int MINPRNQZS_S = 183;                 /* min satellite PRN number of QZSS SAIF */
                public const int MAXPRNQZS_S = 191;/* max satellite PRN number of QZSS SAIF */
                public const int NSATQZS = (MAXPRNQZS - MINPRNQZS + 1);/* number of QZSS satellites */
                public const int NSYSQZS = 1;
#else
        public const int MINPRNQZS = 0;
        public const int MAXPRNQZS = 0;
        public const int MINPRNQZS_S = 0;
        public const int MAXPRNQZS_S = 0;
        public const int NSATQZS = 0;
        public const int NSYSQZS = 0;
#endif
#if ENACMP
                public const int MINPRNCMP   = 1                   /* min satellite sat number of BeiDou */
                public const int MAXPRNCMP = 37;/* max satellite sat number of BeiDou */
                public const int NSATCMP = (MAXPRNCMP - MINPRNCMP + 1); /* number of BeiDou satellites */
                public const int NSYSCMP = 1;
#else
        public const int MINPRNCMP = 0;
        public const int MAXPRNCMP = 0;
        public const int NSATCMP = 0;
        public const int NSYSCMP = 0;
#endif
#if ENAIRN
                public const int MINPRNIRN = 1;/* min satellite sat number of IRNSS */
                public const int MAXPRNIRN = 7;/* max satellite sat number of IRNSS */
                public const int NSATIRN = (MAXPRNIRN - MINPRNIRN + 1); /* number of IRNSS satellites */
                public const int NSYSIRN = 1;
#else
        public const int MINPRNIRN = 0;
        public const int MAXPRNIRN = 0;
        public const int NSATIRN = 0;
        public const int NSYSIRN = 0;
#endif
#if ENALEO
                public const int MINPRNLEO = 1;/* min satellite sat number of LEO */
                public const int MAXPRNLEO = 10;/* max satellite sat number of LEO */
                public const int NSATLEO = (MAXPRNLEO - MINPRNLEO + 1); /* number of LEO satellites */
                public const int NSYSLEO = 1;
#else
        public const int MINPRNLEO = 0;
        public const int MAXPRNLEO = 0;
        public const int NSATLEO = 0;
        public const int NSYSLEO = 0;
#endif
        public const int NSYS = (NSYSGPS + NSYSGLO + NSYSGAL + NSYSQZS + NSYSCMP + NSYSIRN + NSYSLEO); /* number of systems */

        public const int MINPRNSBS = 120;/* min satellite PRN number of SBAS */
        public const int MAXPRNSBS = 142;/* max satellite PRN number of SBAS */
        public const int NSATSBS = (MAXPRNSBS - MINPRNSBS + 1);/* number of SBAS satellites */

        public const int MAXSAT = (NSATGPS + NSATGLO + NSATGAL + NSATQZS + NSATCMP + NSATIRN + NSATSBS + NSATLEO);
        /* max satellite number (1 to MAXSAT) */
        public const int MAXSTA = 255;

#if !MAXOBS
        public const int MAXOBS = 64;/* max number of obs in an epoch */
#endif
        public const int MAXRCV = 64;/* max receiver number (1 to MAXRCV) */
        public const int MAXOBSTYPE = 64;/* max number of obs type in RINEX */
#if OBS_100HZ
                public const double DTTOL = 0.005;/* tolerance of time difference (s) */
#else
        public const double DTTOL = 0.025;/* tolerance of time difference (s) */
#endif
        public const double MAXDTOE = 7200.0;/* max time difference to GPS Toe (s) */
        public const double MAXDTOE_QZS = 7200.0;/* max time difference to QZSS Toe (s) */
        public const double MAXDTOE_GAL = 14400.0;/* max time difference to Galileo Toe (s) */
        public const double MAXDTOE_CMP = 21600.0;/* max time difference to BeiDou Toe (s) */
        public const double MAXDTOE_GLO = 1800.0;/* max time difference to GLONASS Toe (s) */
        public const double MAXDTOE_SBS = 360.0;/* max time difference to SBAS Toe (s) */
        public const double MAXDTOE_S = 86400.0;/* max time difference to ephem toe (s) for other */
        public const double MAXGDOP = 300.0;/* max GDOP */

        public const double double_SWAP_TRAC = 86400.0;/* swap doubleerval of trace file (s) */
        public const double double_SWAP_STAT = 86400.0;/* swap doubleerval of solution status file (s) */

        public const double MAXEXFILE = 1024;/* max number of expanded files */
        public const double MAXSBSAGEF = 30.0;/* max age of SBAS fast correction (s) */
        public const double MAXSBSAGEL = 1800.0;/* max age of SBAS long term corr (s) */
        public const int MAXSBSURA = 8;/* max URA of SBAS satellite */
        public const int MAXBAND = 10;/* max SBAS band of IGP */
        public const int MAXNIGP = 201;/* max number of IGP in SBAS band */
        public const int MAXNGEO = 4;/* max number of GEO satellites */
        public const int MAXCOMMENT = 10;/* max number of RINEX comments */
        public const int MAXSTRPATH = 1024;/* max length of stream path */
        public const int MAXSTRMSG = 1024;/* max length of stream message */
        public const int MAXSTRRTK = 8;/* max number of stream in RTK server */
        public const int MAXSBSMSG = 32;/* max number of SBAS msg in RTK server */
        public const int MAXSOLMSG = 8191;/* max length of solution message */
        public const int MAXRAWLEN = 4096;/* max length of receiver raw message */
        public const int MAXERRMSG = 4096;/* max length of error/warning message */
        public const int MAXANT = 64;/* max length of station name/antenna type */
        public const int MAXSOLBUF = 256;/* max number of solution buffer */
        public const int MAXOBSBUF = 128;/* max number of observation data buffer */
        public const int MAXNRPOS = 16;/* max number of reference positions */
        public const int MAXLEAPS = 64;/* max number of leap seconds table */
        public const int MAXGISLAYER = 32;/* max number of GIS data layers */
        public const int MAXRCVCMD = 4096;/* max length of receiver commands */

        public const double RNX2VER = 2.10;/* RINEX ver.2 default output version */
        public const double RNX3VER = 3.00;/* RINEX ver.3 default output version */

        public const int OBSTYPE_PR = 0x01;/* observation type: pseudorange */
        public const int OBSTYPE_CP = 0x02;/* observation type: carrier-phase */
        public const int OBSTYPE_DOP = 0x04;/* observation type: doppler-freq */
        public const int OBSTYPE_SNR = 0x08;/* observation type: SNR */
        public const int OBSTYPE_ALL = 0xFF;/* observation type: all */

        public const int FREQTYPE_L1 = 0x01;/* frequency type: L1/E1 */
        public const int FREQTYPE_L2 = 0x02;/* frequency type: L2/B1 */
        public const int FREQTYPE_L5 = 0x04;/* frequency type: L5/E5a/L3 */
        public const int FREQTYPE_L6 = 0x08;/* frequency type: E6/LEX/B3 */
        public const int FREQTYPE_L7 = 0x10;/* frequency type: E5b/B2 */
        public const int FREQTYPE_L8 = 0x20;/* frequency type: E5(a+b) */
        public const int FREQTYPE_L9 = 0x40;/* frequency type: S */
        public const int FREQTYPE_ALL = 0xFF;/* frequency type: all */

        public const int CODE_NONE = 0;/* obs code: none or unknown */
        public const int CODE_L1C = 1;/* obs code: L1C/A,G1C/A,E1C (GPS,GLO,GAL,QZS,SBS) */
        public const int CODE_L1P = 2;/* obs code: L1P,G1P    (GPS,GLO) */
        public const int CODE_L1W = 3;/* obs code: L1 Z-track (GPS) */
        public const int CODE_L1Y = 4;/* obs code: L1Y        (GPS) */
        public const int CODE_L1M = 5;/* obs code: L1M        (GPS) */
        public const int CODE_L1N = 6;/* obs code: L1codeless (GPS) */
        public const int CODE_L1S = 7;/* obs code: L1C(D)     (GPS,QZS) */
        public const int CODE_L1L = 8;/* obs code: L1C(P)     (GPS,QZS) */
        public const int CODE_L1E = 9;/* (not used) */
        public const int CODE_L1A = 10;/* obs code: E1A        (GAL) */
        public const int CODE_L1B = 11;/* obs code: E1B        (GAL) */
        public const int CODE_L1X = 12;/* obs code: E1B+C,L1C(D+P) (GAL,QZS) */
        public const int CODE_L1Z = 13;/* obs code: E1A+B+C,L1SAIF (GAL,QZS) */
        public const int CODE_L2C = 14;/* obs code: L2C/A,G1C/A (GPS,GLO) */
        public const int CODE_L2D = 15;/* obs code: L2 L1C/A-(P2-P1) (GPS) */
        public const int CODE_L2S = 16;/* obs code: L2C(M)     (GPS,QZS) */
        public const int CODE_L2L = 17;/* obs code: L2C(L)     (GPS,QZS) */
        public const int CODE_L2X = 18;/* obs code: L2C(M+L),B1I+Q (GPS,QZS,CMP) */
        public const int CODE_L2P = 19;/* obs code: L2P,G2P    (GPS,GLO) */
        public const int CODE_L2W = 20;/* obs code: L2 Z-track (GPS) */
        public const int CODE_L2Y = 21;/* obs code: L2Y        (GPS) */
        public const int CODE_L2M = 22;/* obs code: L2M        (GPS) */
        public const int CODE_L2N = 23;/* obs code: L2codeless (GPS) */
        public const int CODE_L5I = 24;/* obs code: L5/E5aI    (GPS,GAL,QZS,SBS) */
        public const int CODE_L5Q = 25;/* obs code: L5/E5aQ    (GPS,GAL,QZS,SBS) */
        public const int CODE_L5X = 26;/* obs code: L5/E5aI+Q/L5B+C (GPS,GAL,QZS,IRN,SBS) */
        public const int CODE_L7I = 27;/* obs code: E5bI,B2I   (GAL,CMP) */
        public const int CODE_L7Q = 28;/* obs code: E5bQ,B2Q   (GAL,CMP) */
        public const int CODE_L7X = 29;/* obs code: E5bI+Q,B2I+Q (GAL,CMP) */
        public const int CODE_L6A = 30;/* obs code: E6A        (GAL) */
        public const int CODE_L6B = 31;/* obs code: E6B        (GAL) */
        public const int CODE_L6C = 32;/* obs code: E6C        (GAL) */
        public const int CODE_L6X = 33;/* obs code: E6B+C,LEXS+L,B3I+Q (GAL,QZS,CMP) */
        public const int CODE_L6Z = 34;/* obs code: E6A+B+C    (GAL) */
        public const int CODE_L6S = 35;/* obs code: LEXS       (QZS) */
        public const int CODE_L6L = 36;/* obs code: LEXL       (QZS) */
        public const int CODE_L8I = 37;/* obs code: E5(a+b)I   (GAL) */
        public const int CODE_L8Q = 38;/* obs code: E5(a+b)Q   (GAL) */
        public const int CODE_L8X = 39;/* obs code: E5(a+b)I+Q (GAL) */
        public const int CODE_L2I = 40;/* obs code: B1I        (BDS) */
        public const int CODE_L2Q = 41;/* obs code: B1Q        (BDS) */
        public const int CODE_L6I = 42;/* obs code: B3I        (BDS) */
        public const int CODE_L6Q = 43;/* obs code: B3Q        (BDS) */
        public const int CODE_L3I = 44;/* obs code: G3I        (GLO) */
        public const int CODE_L3Q = 45;/* obs code: G3Q        (GLO) */
        public const int CODE_L3X = 46;/* obs code: G3I+Q      (GLO) */
        public const int CODE_L1I = 47;/* obs code: B1I        (BDS) */
        public const int CODE_L1Q = 48;/* obs code: B1Q        (BDS) */
        public const int CODE_L5A = 49;/* obs code: L5A SPS    (IRN) */
        public const int CODE_L5B = 50;/* obs code: L5B RS(D)  (IRN) */
        public const int CODE_L5C = 51;/* obs code: L5C RS(P)  (IRN) */
        public const int CODE_L9A = 52;/* obs code: SA SPS     (IRN) */
        public const int CODE_L9B = 53;/* obs code: SB RS(D)   (IRN) */
        public const int CODE_L9C = 54;/* obs code: SC RS(P)   (IRN) */
        public const int CODE_L9X = 55;/* obs code: SB+C       (IRN) */
        public const int MAXCODE = 55;/* max number of obs code */

        public const int PMODE_SINGLE = 0;/* positioning mode: single */
        public const int PMODE_DGPS = 1;/* positioning mode: DGPS/DGNSS */
        public const int PMODE_KINEMA = 2;/* positioning mode: kinematic */
        public const int PMODE_STATIC = 3;/* positioning mode: static */
        public const int PMODE_MOVEB = 4;/* positioning mode: moving-base */
        public const int PMODE_FIXED = 5;/* positioning mode: fixed */
        public const int PMODE_PPP_KINEMA = 6;/* positioning mode: PPP-kinemaric */
        public const int PMODE_PPP_STATIC = 7;/* positioning mode: PPP-static */
        public const int PMODE_PPP_FIXED = 8;/* positioning mode: PPP-fixed */

        public const int SOLF_LLH = 0;/* solution format: lat/lon/height */
        public const int SOLF_XYZ = 1;/* solution format: x/y/z-ecef */
        public const int SOLF_ENU = 2;/* solution format: e/n/u-baseline */
        public const int SOLF_NMEA = 3;/* solution format: NMEA-183 */
        public const int SOLF_STAT = 4;/* solution format: solution status */
        public const int SOLF_GSIF = 5;/* solution format: GSI F1/F2 */

        public const int SOLQ_NONE = 0;/* solution status: no solution */
        public const int SOLQ_FIX = 1;/* solution status: fix */
        public const int SOLQ_FLOAT = 2;/* solution status: float */
        public const int SOLQ_SBAS = 3;/* solution status: SBAS */
        public const int SOLQ_DGPS = 4;/* solution status: DGPS/DGNSS */
        public const int SOLQ_SINGLE = 5;/* solution status: single */
        public const int SOLQ_PPP = 6;/* solution status: PPP */
        public const int SOLQ_DR = 7;/* solution status: dead reconing */
        public const int MAXSOLQ = 7;/* max number of solution status */

        public const int TIMES_GPST = 0;/* time system: gps time */
        public const int TIMES_UTC = 1;/* time system: utc */
        public const int TIMES_JST = 2;/* time system: jst */

        public const int IONOOPT_OFF = 0;/* ionosphere option: correction off */
        public const int IONOOPT_BRDC = 1;/* ionosphere option: broadcast model */
        public const int IONOOPT_SBAS = 2;/* ionosphere option: SBAS model */
        public const int IONOOPT_IFLC = 3;/* ionosphere option: L1/L2 or L1/L5 iono-free LC */
        public const int IONOOPT_EST = 4;/* ionosphere option: estimation */
        public const int IONOOPT_TEC = 5;/* ionosphere option: IONEX TEC model */
        public const int IONOOPT_QZS = 6;/* ionosphere option: QZSS broadcast model */
        public const int IONOOPT_LEX = 7;/* ionosphere option: QZSS LEX ionospehre */
        public const int IONOOPT_STEC = 8;/* ionosphere option: SLANT TEC model */

        public const int TROPOPT_OFF = 0;/* troposphere option: correction off */
        public const int TROPOPT_SAAS = 1;/* troposphere option: Saastamoinen model */
        public const int TROPOPT_SBAS = 2;/* troposphere option: SBAS model */
        public const int TROPOPT_EST = 3;/* troposphere option: ZTD estimation */
        public const int TROPOPT_ESTG = 4;/* troposphere option: ZTD+grad estimation */
        public const int TROPOPT_ZTD = 5;/* troposphere option: ZTD correction */

        public const int EPHOPT_BRDC = 0;/* ephemeris option: broadcast ephemeris */
        public const int EPHOPT_PREC = 1;/* ephemeris option: precise ephemeris */
        public const int EPHOPT_SBAS = 2;/* ephemeris option: broadcast + SBAS */
        public const int EPHOPT_SSRAPC = 3;/* ephemeris option: broadcast + SSR_APC */
        public const int EPHOPT_SSRCOM = 4;/* ephemeris option: broadcast + SSR_COM */
        public const int EPHOPT_LEX = 5;/* ephemeris option: QZSS LEX ephemeris */

        public const int ARMODE_OFF = 0;/* AR mode: off */
        public const int ARMODE_CONT = 1;/* AR mode: continuous */
        public const int ARMODE_INST = 2;/* AR mode: instantaneous */
        public const int ARMODE_FIXHOLD = 3;/* AR mode: fix and hold */
        public const int ARMODE_WLNL = 4;/* AR mode: wide lane/narrow lane */
        public const int ARMODE_TCAR = 5;/* AR mode: triple carrier ar */

        public const int SBSOPT_LCORR = 1;/* SBAS option: long term correction */
        public const int SBSOPT_FCORR = 2;/* SBAS option: fast correction */
        public const int SBSOPT_ICORR = 4;/* SBAS option: ionosphere correction */
        public const int SBSOPT_RANGE = 8;/* SBAS option: ranging */

        public const int POSOPT_POS = 0;/* pos option: LLH/XYZ */
        public const int POSOPT_SINGLE = 1;/* pos option: average of single pos */
        public const int POSOPT_FILE = 2;/* pos option: read from pos file */
        public const int POSOPT_RINEX = 3;/* pos option: rinex header pos */
        public const int POSOPT_RTCM = 4;/* pos option: rtcm station pos */
        public const int POSOPT_RAW = 5;/* pos option: raw station pos */

        public const int STR_NONE = 0;/* stream type: none */
        public const int STR_SERIAL = 1;/* stream type: serial */
        public const int STR_FILE = 2;/* stream type: file */
        public const int STR_TCPSVR = 3;/* stream type: TCP server */
        public const int STR_TCPCLI = 4;/* stream type: TCP client */
        public const int STR_NTRIPSVR = 6;/* stream type: NTRIP server */
        public const int STR_NTRIPCLI = 7;/* stream type: NTRIP client */
        public const int STR_FTP = 8;/* stream type: ftp */
        public const int STR_HTTP = 9;/* stream type: http */
        public const int STR_NTRIPC_S = 10;/* stream type: NTRIP caster server */
        public const int STR_NTRIPC_C = 11;/* stream type: NTRIP caster client */
        public const int STR_UDPSVR = 12;/* stream type: UDP server */
        public const int STR_UDPCLI = 13;/* stream type: UDP server */
        public const int STR_MEMBUF = 14;/* stream type: memory buffer */

        public const int STRFMT_RTCM2 = 0;/* stream format: RTCM 2 */
        public const int STRFMT_RTCM3 = 1;/* stream format: RTCM 3 */
        public const int STRFMT_OEM4 = 2;/* stream format: NovAtel OEMV/4 */
        public const int STRFMT_OEM3 = 3;/* stream format: NovAtel OEM3 */
        public const int STRFMT_UBX = 4;/* stream format: u-blox LEA-*T */
        public const int STRFMT_SS2 = 5;/* stream format: NovAtel Superstar II */
        public const int STRFMT_CRES = 6;/* stream format: Hemisphere */
        public const int STRFMT_STQ = 7;/* stream format: SkyTraq S1315F */
        public const int STRFMT_GW10 = 8;/* stream format: Furuno GW10 */
        public const int STRFMT_JAVAD = 9;/* stream format: JAVAD GRIL/GREIS */
        public const int STRFMT_NVS = 10;/* stream format: NVS NVC08C */
        public const int STRFMT_BINEX = 11;/* stream format: BINEX */
        public const int STRFMT_RT17 = 12;/* stream format: Trimble RT17 */
        public const int STRFMT_SEPT = 13;/* stream format: Septentrio */
        public const int STRFMT_CMR = 14;/* stream format: CMR/CMR+ */
        public const int STRFMT_TERSUS = 15;/* stream format: TERSUS */
        public const int STRFMT_LEXR = 16;/* stream format: Furuno LPY-10000 */
        public const int STRFMT_RINEX = 17;/* stream format: RINEX */
        public const int STRFMT_SP3 = 18;/* stream format: SP3 */
        public const int STRFMT_RNXCLK = 19;/* stream format: RINEX CLK */
        public const int STRFMT_SBAS = 20;/* stream format: SBAS messages */
        public const int STRFMT_NMEA = 21;/* stream format: NMEA 0183 */
#if !EXTLEX
        public const int MAXRCVFMT = 15;                 /* max number of receiver format */
#else
                public const int MAXRCVFMT = 16;
#endif

        public const int STR_MODE_R = 0x1;/* stream mode: read */
        public const int STR_MODE_W = 0x2;/* stream mode: write */
        public const int STR_MODE_RW = 0x3;/* stream mode: read/write */

        public const int GEOID_EMBEDDED = 0;/* geoid model: embedded geoid */
        public const int GEOID_EGM96_M150 = 1;/* geoid model: EGM96 15x15" */
        public const int GEOID_EGM2008_M25 = 2;/* geoid model: EGM2008 2.5x2.5" */
        public const int GEOID_EGM2008_M10 = 3;/* geoid model: EGM2008 1.0x1.0" */
        public const int GEOID_GSI2000_M15 = 4;/* geoid model: GSI geoid 2000 1.0x1.5" */
        public const int GEOID_RAF09 = 5;/* geoid model: IGN RAF09 for France 1.5"x2" */

        public const string COMMENTH = "%";                 /* comment line indicator for solution */
        public const string MSG_DISCONN = "$_DISCONNECT\r\n";  /* disconnect message */

        public const int DLOPT_FORCE = 0x01;/* download option: force download existing */
        public const int DLOPT_KEEPCMP = 0x02;/* download option: keep compressed file */
        public const int DLOPT_HOLDERR = 0x04;/* download option: hold on error file */
        public const int DLOPT_HOLDLST = 0x08;/* download option: hold on listing file */

        public const int LLI_SLIP = 0x01;/* LLI: cycle-slip */
        public const int LLI_HALFC = 0x02;/* LLI: half-cycle not resovled */
        public const int LLI_BOCTRK = 0x04;/* LLI: boc tracking of mboc signal */
        public const int LLI_HALFA = 0x40;/* LLI: half-cycle added */
        public const int LLI_HALFS = 0x80;/* LLI: half-cycle subtracted */

        public const int IMUFMT_KVH = 1;             /* imu data format KVH */

        public const double P2_5 = 0.03125;/* 2^-5 */
        public const double P2_6 = 0.015625;/* 2^-6 */
        public const double P2_11 = 4.882812500000000E-04;/* 2^-11 */
        public const double P2_15 = 3.051757812500000E-05;/* 2^-15 */
        public const double P2_17 = 7.629394531250000E-06;/* 2^-17 */
        public const double P2_19 = 1.907348632812500E-06;/* 2^-19 */
        public const double P2_20 = 9.536743164062500E-07;/* 2^-20 */
        public const double P2_21 = 4.768371582031250E-07;/* 2^-21 */
        public const double P2_23 = 1.192092895507810E-07;/* 2^-23 */
        public const double P2_24 = 5.960464477539063E-08;/* 2^-24 */
        public const double P2_27 = 7.450580596923828E-09;/* 2^-27 */
        public const double P2_29 = 1.862645149230957E-09;/* 2^-29 */
        public const double P2_30 = 9.313225746154785E-10;/* 2^-30 */
        public const double P2_31 = 4.656612873077393E-10;/* 2^-31 */
        public const double P2_32 = 2.328306436538696E-10;/* 2^-32 */
        public const double P2_33 = 1.164153218269348E-10;/* 2^-33 */
        public const double P2_35 = 2.910383045673370E-11; /* 2^-35 */
        public const double P2_38 = 3.637978807091710E-12;/* 2^-38 */
        public const double P2_39 = 1.818989403545856E-12;/* 2^-39 */
        public const double P2_40 = 9.094947017729280E-13;/* 2^-40 */
        public const double P2_43 = 1.136868377216160E-13;/* 2^-43 */
        public const double P2_48 = 3.552713678800501E-15; /* 2^-48 */
        public const double P2_50 = 8.881784197001252E-16; /* 2^-50 */
        public const double P2_55 = 2.775557561562891E-17; /* 2^-55 */


        #endregion


        #region Структуры
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]   /* time struct */
#pragma warning disable IDE1006 // Naming Styles
        public struct gtime_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public IntPtr time;                                          /* time (s) expressed by standard time_t */
            public double sec;                                          /* fraction of second under 1 s */
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]   /* observation data record */
#pragma warning disable IDE1006 // Naming Styles
        public struct obsd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public gtime_t time;                                        /* receiver sampling time (GPST) */
            public byte sat, rcv;                                       /* satellite/receiver number */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ + NEXOBS)]
            public byte[] SNR;                            /* signal strength (0.25 dBHz) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ + NEXOBS)]
            public byte[] LLI;                            /* loss of lock indicator */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ + NEXOBS)]
            public byte[] code;                           /* code indicator (CODE_???) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ + NEXOBS)]
            public double[] L;                            /* observation data carrier-phase (cycle) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ + NEXOBS)] 
            public double[] P;                            /* observation data pseudorange (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ + NEXOBS)]
            public float[] D;                             /* observation data doppler frequency (Hz) */
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* observation data */
#pragma warning disable IDE1006 // Naming Styles
        public struct obs_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public int n, nmax;                                         /* number of obervation data/allocated */
            public obsd_t data;                                         /* observation data records */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* earth rotation parameter data type */
#pragma warning disable IDE1006 // Naming Styles
        public struct erpd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public double mjd;                                           /* mjd (days) */
            public double xp, yp;                                        /* pole offset (rad) */
            public double xpr, ypr;                                      /* pole offset rate (rad/day) */
            public double ut1_utc;                                       /* ut1-utc (s) */
            public double lod;                                           /* length of day (s/day) */
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* earth rotation parameter type */
#pragma warning disable IDE1006 // Naming Styles
        public struct erp_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public int n, nmax;                                         /* number and max number of data */
            public erpd_t data;                                         /* earth rotation parameter data */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* antenna parameter type */
#pragma warning disable IDE1006 // Naming Styles
        public struct pcv_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public int sat;                                             /* antenna type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] type;                                   /* serial number or satellite code */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] code;                                   /* valid time start and end */
            public gtime_t ts, te;                                      /* phase center offset e/n/u or x/y/z (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ * 3)]
            public double[] off;                               /* phase center offset e/n/u or x/y/z (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ * 19)]
            public double[] var;                                       /* phase center variation (m) */
            /* el=90,85,...,0 or nadir=0,1,2,3,... (deg) */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* antenna parameters type */
#pragma warning disable IDE1006 // Naming Styles
        public struct pcvs_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public int n, nmax;                                         /* number of data/allocated */
            public pcv_t pcv;                                          /* antenna parameters data */

        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* almanac type */
#pragma warning disable IDE1006 // Naming Styles
        public struct alm
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public int sat;                                             /* antenna type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] type;                                   /* serial number or satellite code */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] code;                                   /* valid time start and end */
            public gtime_t ts, te;                                      /* phase center offset e/n/u or x/y/z (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ * 3)]
            public double[] off;                               /* phase center offset e/n/u or x/y/z (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ * 19)]
            public double[] var;                                      /* phase center variation (m) */
            /* el=90,85,...,0 or nadir=0,1,2,3,... (deg) */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct alm_t                                                 /* almanac type */
#pragma warning restore IDE1006 // Naming Styles
        {
            
            public int sat;            /* satellite number */
            public int svh;            /* sv health (0:ok) */
            public int svconf;         /* as and sv config */
            public int week;           /* GPS/QZS: gps week, GAL: galileo week */
            public gtime_t toa;        /* Toa */
            /* SV orbit parameters */
            public double A, e, i0, OMG0, omg, M0, OMGd;
            public double toas;        /* Toa (s) in week */
            public double f0, f1;       /* SV clock parameters (af0,af1) */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public  struct eph_t
#pragma warning restore IDE1006 // Naming Styles
        {                                                               /* GPS/QZS/GAL broadcast ephemeris type */
            
            public int sat;            /* satellite number */
            public int iode, iodc;      /* IODE,IODC */
            public int sva;            /* SV accuracy (URA index) */
            public int svh;            /* SV health (0:ok) */
            public int week;           /* GPS/QZS: gps week, GAL: galileo week */
            public int code;           /* GPS/QZS: code on L2 */
            /* GAL: data source defined as rinex 3.03 */
            /* BDS: data source (0:unknown,1:B1I,2:B1Q,3:B2I,4:B2Q,5:B3I,6:B3Q) */
            public int flag;           /* GPS/QZS: L2 P data flag */
            /* BDS: nav type (0:unknown,1:IGSO/MEO,2:GEO) */
            public gtime_t toe, toc, ttr; /* Toe,Toc,T_trans */
            /* SV orbit parameters */
            public double A, e, i0, OMG0, omg, M0, deln, OMGd, idot;
            public double crc, crs, cuc, cus, cic, cis;
            public double toes;        /* Toe (s) in week */
            public double fit;         /* fit interval (h) */
            public double f0, f1, f2;    /* SV clock parameters (af0,af1,af2) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] tgd;      /* group delay parameters */
            /* GPS/QZS:tgd[0]=TGD */
            /* GAL    :tgd[0]=BGD E5a/E1,tgd[1]=BGD E5b/E1 */
            /* CMP    :tgd[0]=BGD1,tgd[1]=BGD2 */
            public double Adot, ndot;   /* Adot,ndot for CNAV */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct geph_t
#pragma warning restore IDE1006 // Naming Styles
        {        /* GLONASS broadcast ephemeris type */
            
            public int sat;            /* satellite number */
            public int iode;           /* IODE (0-6 bit of tb field) */
            public int frq;            /* satellite frequency number */
            public int svh, sva, age;    /* satellite health, accuracy, age of operation */
            public gtime_t toe;        /* epoch of epherides (gpst) */
            public gtime_t tof;        /* message frame time (gpst) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] pos;      /* satellite position (ecef) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] vel;      /* satellite velocity (ecef) (m/s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] acc;      /* satellite acceleration (ecef) (m/s^2) */
            public double taun, gamn;   /* SV clock bias (s)/relative freq bias */
            public double dtaun;       /* delay between L1 and L2 (s) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct peph_t
#pragma warning restore IDE1006 // Naming Styles
        {        /* precise ephemeris type */
            
            public gtime_t time;       /* time (GPST) */
            public int index;          /* ephemeris index for multiple files */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 4)]
            public double[] pos; /* satellite position/clock (ecef) (m|s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 4)]
            public float[] std; /* satellite position/clock std (m|s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 4)]
            public double[] vel; /* satellite velocity/clk-rate (m/s|s/s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 4)]
            public float[] vst; /* satellite velocity/clk-rate std (m/s|s/s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 3)]
            public float[] cov; /* satellite position covariance (m^2) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 3)]
            public float[] vco; /* satellite velocity covariance (m^2) */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct pclk_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* precise clock type */
            
            public gtime_t time;       /* time (GPST) */
            public int index;          /* clock index for multiple files */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] clk; /* satellite clock (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public float[] std; /* satellite clock std (s) */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct seph_t
#pragma warning restore IDE1006 // Naming Styles
        {        /* SBAS ephemeris type */
            
            public int sat;            /* satellite number */
            public gtime_t t0;         /* reference epoch time (GPST) */
            public gtime_t tof;        /* time of message frame (GPST) */
            public int sva;            /* SV accuracy (URA index) */
            public int svh;            /* SV health (0:ok) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] pos;      /* satellite position (m) (ecef) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] vel;      /* satellite velocity (m/s) (ecef) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] acc;      /* satellite acceleration (m/s^2) (ecef) */
            public double af0, af1;     /* satellite clock-offset/drift (s,s/s) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct tled_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* norad two line element data type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public char[] name;     /* common name */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public char[] alias;     /* alias name */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public char[] satno;     /* satellilte catalog number */
            public char satclass;      /* classification */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public char[] desig;     /* international designator */
            public gtime_t epoch;      /* element set epoch (UTC) */
            public double ndot;        /* 1st derivative of mean motion */
            public double nddot;       /* 2st derivative of mean motion */
            public double bstar;       /* B* drag term */
            public int etype;          /* element set type */
            public int eleno;          /* element number */
            public double inc;         /* orbit inclination (deg) */
            public double OMG;         /* right ascension of ascending node (deg) */
            public double ecc;         /* eccentricity */
            public double omg;         /* argument of perigee (deg) */
            public double M;           /* mean anomaly (deg) */
            public double n;           /* mean motion (rev/day) */
            public int rev;            /* revolution number at epoch */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct tle_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            /* norad two line element type */
            public int n, nmax;         /* number/max number of two line element data */
            public tled_t data;       /* norad two line element data */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct tec_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* TEC grid type */
            public gtime_t time;       /* epoch time (GPST) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] ndata;       /* TEC grid data size {nlat,nlon,nhgt} */
            public double rb;          /* earth radius (km) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] lats;     /* latitude start/interval (deg) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] lons;     /* longitude start/interval (deg) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] hgts;     /* heights start/interval (km) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] data;       /* ???????TEC grid data (tecu) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public float[] rms;         /* ????RMS values (tecu) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct fcbd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* satellite fcb data type */
            public gtime_t ts, te;      /* start/end time (GPST) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 3)]
            public double[] bias; /* fcb value   (cyc) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 3)]
            public double[] std; /* fcb std-dev (cyc) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsmsg_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS message type */
            public int week, tow;       /* receiption time */
            public int prn;            /* SBAS satellite PRN number */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 29)]
            public byte[] msg; /* SBAS message (226bit) padded by 0 */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbs_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS messages type */
            public int n, nmax;         /* number of SBAS messages/allocated */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public sbsmsg_t[] msgs;     /* SBAS messages */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsfcorr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS fast correction type */
            public gtime_t t0;         /* time of applicability (TOF) */
            public double prc;         /* pseudorange correction (PRC) (m) */
            public double rrc;         /* range-rate correction (RRC) (m/s) */
            public double dt;          /* range-rate correction delta-time (s) */
            public int iodf;           /* IODF (issue of date fast corr) */
            public short udre;         /* UDRE+1 */
            public short ai;           /* degradation factor indicator */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbslcorr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS long term satellite error correction type */
            public gtime_t t0;         /* correction time */
            public int iode;           /* IODE (issue of date ephemeris) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] dpos;     /* delta position (m) (ecef) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] dvel;     /* delta velocity (m/s) (ecef) */
            public double daf0, daf1;   /* delta clock-offset/drift (s,s/s) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbssatp_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS satellite correction type */
            public int sat;            /* satellite number */
            public sbsfcorr_t fcorr;   /* fast correction */
            public sbslcorr_t lcorr;   /* long term correction */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbssat_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS satellite corrections type */
            public int iodp;           /* IODP (issue of date mask) */
            public int nsat;           /* number of satellites */
            public int tlat;           /* system latency (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public sbssatp_t[] sat; /* satellite correction */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsigp_t
#pragma warning restore IDE1006 // Naming Styles
        {
           
            /* SBAS ionospheric correction type */
            public gtime_t t0;         /* correction time */
            public short lat, lon;      /* latitude/longitude (deg) */
            public short give;         /* GIVI+1 */
            public float delay;        /* vertical delay estimate (m) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsigpband_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            /* IGP band type */
            public short x;            /* longitude/latitude (deg) */
            public IntPtr y;     /* latitudes/longitudes (deg) */
            public byte bits; /* IGP mask start bit */
            public byte bite; /* IGP mask end bit */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsion_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SBAS ionospheric corrections type */
            public int iodi;           /* IODI (issue of date ionos corr) */
            public int nigp;           /* number of igps */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXNIGP)]
            public sbsigp_t[] igp;      /* ionospheric correction */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct dgps_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* DGPS/GNSS correction type */
            public gtime_t t0;         /* correction time */
            public double prc;         /* pseudorange correction (PRC) (m) */
            public double rrc;         /* range rate correction (RRC) (m/s) */
            public int iod;            /* issue of data (IOD) */
            public double udre;        /* UDRE */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct ssr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* SSR correction type */
            public gtime_t t0;      /* epoch time (GPST) {eph,clk,hrclk,ura,bias,pbias} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public double[] udi;      /* SSR update interval (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public int[] iod;         /* iod ssr {eph,clk,hrclk,ura,bias,pbias} */
            public int iode;           /* issue of data */
            public int iodcrc;         /* issue of data crc for beidou/sbas */
            public int ura;            /* URA indicator */
            public int refd;           /* sat ref datum (0:ITRF,1:regional) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] deph;    /* delta orbit {radial,along,cross} (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] ddeph;    /* dot delta orbit {radial,along,cross} (m/s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] dclk;    /* delta clock {c0,c1,c2} (m,m/s,m/s^2) */
            public double hrclk;       /* high-rate clock corection (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXCODE)]
            public float[] cbias; /* code biases (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXCODE)]
            public double[] pbias; /* phase biases (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXCODE)]
            public float[] stdpb; /* std-dev of phase biases (m) */
            public double yaw_ang, yaw_rate; /* yaw angle and yaw rate (deg,deg/s) */
            public byte update; /* update flag (0:no update,1:update) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct lexmsg_t
#pragma warning restore IDE1006 // Naming Styles
        {
            
            /* QZSS LEX message type */
            public int prn;            /* satellite PRN number */
            public int type;           /* message type */
            public int alert;          /* alert flag */
            public byte stat; /* signal tracking status */
            public byte snr;  /* signal C/N0 (0.25 dBHz) */
            public int ttt;   /* tracking time (ms) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 212)]
            public byte[] msg; /* LEX message data part 1695 bits */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct lex_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* QZSS LEX messages type */
            public int n, nmax;         /* number of LEX messages and allocated */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public lexmsg_t[] msgs;     /* LEX messages */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct lexeph_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* QZSS LEX ephemeris type */
            public gtime_t toe;        /* epoch time (GPST) */
            public gtime_t tof;        /* message frame time (GPST) */
            public int sat;            /* satellite number */
            public byte health; /* signal health (L1,L2,L1C,L5,LEX) */
            public byte ura;  /* URA index */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] pos;      /* satellite position (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] vel;      /* satellite velocity (m/s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] acc;      /* satellite acceleration (m/s2) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] jerk;     /* satellite jerk (m/s3) */
            public double af0, af1;     /* satellite clock bias and drift (s,s/s) */
            public double tgd;         /* TGD */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] isc;      /* ISC */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct lexion_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* QZSS LEX ionosphere correction type */
            public gtime_t t0;         /* epoch time (GPST) */
            public double tspan;       /* valid time span (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] pos0;     /* reference position {lat,lon} (rad) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * 2)]
            public double[] coef;  /* coefficients lat x lon (3 x 2) */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct stec_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* stec data type */
            public gtime_t time;       /* time (GPST) */
            public byte sat;  /* satellite number */
            public double ion;         /* slant ionos delay (m) */
            public float std;          /* std-dev (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public float[] azel;      /* azimuth/elevation (rad) */
            public byte flag; /* fix flag */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct trop_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* trop data type */
            public gtime_t time;       /* time (GPST) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] trp;      /* zenith tropos delay/gradient (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] std;       /* std-dev (m) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct pppcorr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* ppp corrections type */
            public int nsta;           /* number of stations */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA * 8)]
            public char[] stas; /* station names */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA * 3)]
            public double[] rr; /* station ecef positions (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA)]
            public int[] ns;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA)]
            public int[] nsmax; /* number of stec data */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA)]
            public int[] nt;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA)]
            public int[] ntmax; /* number of trop data */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA)]
            public stec_t[] stec; /* stec data */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTA)]
            public trop_t[] trop; /* trop data */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct nav_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* navigation data type */
            public int n, nmax;         /* number of broadcast ephemeris */
            public int ng, ngmax;       /* number of glonass ephemeris */
            public int ns, nsmax;       /* number of sbas ephemeris */
            public int ne, nemax;       /* number of precise ephemeris */
            public int nc, ncmax;       /* number of precise clock */
            public int na, namax;       /* number of almanac data */
            public int nt, ntmax;       /* number of tec grid data */
            public int nf, nfmax;       /* number of satellite fcb data */
            public eph_t eph;         /* GPS/QZS/GAL ephemeris */
            public geph_t geph;       /* GLONASS ephemeris */
            public seph_t seph;       /* SBAS ephemeris */
            public peph_t peph;       /* precise ephemeris */
            public pclk_t pclk;       /* precise clock */
            public alm_t alm;         /* almanac data */
            public tec_t tec;         /* tec grid data */
            public fcbd_t fcb;        /* satellite fcb data */
            public erp_t erp;         /* earth rotation parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] utc_gps;  /* GPS delta-UTC parameters {A0,A1,T,W} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] utc_glo;  /* GLONASS UTC GPS time parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] 
            public double[] utc_gal;  /* Galileo UTC GPS time parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] utc_qzs;  /* QZS UTC GPS time parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] utc_cmp;  /* BeiDou UTC parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] utc_irn;  /* IRNSS UTC parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] utc_sbs;  /* SBAS UTC parameters */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] ion_gps;  /* GPS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] ion_gal;  /* Galileo iono model parameters {ai0,ai1,ai2,0} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] ion_qzs;  /* QZSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] ion_cmp;  /* BeiDou iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] ion_irn;  /* IRNSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            public int leaps;          /* leap seconds (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * NFREQ)]
            public double[] lam; /* carrier wave lengths (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 3)]
            public double[] cbias; /* satellite dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXRCV * 2 * 3)]
            public double[] rbias; /* receiver dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] wlbias;   /* wide-lane bias (cycle) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] glo_cpbias;    /* glonass code-phase bias {1C,1P,2C,2P} (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXPRNGLO + 1)]
            public char[] glo_fcn; /* glonass frequency channel number + 8 */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public pcv_t[] pcvs; /* satellite antenna pcv */
            public sbssat_t sbssat;    /* SBAS satellite corrections */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXBAND + 1)]
            public sbsion_t[] sbsion; /* SBAS ionosphere corrections */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public dgps_t[] dgps; /* DGPS corrections */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public ssr_t[] ssr;  /* SSR corrections */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public lexeph_t[] lexeph; /* LEX ephemeris */
            public lexion_t lexion;   /* LEX ionosphere correction */
            public pppcorr_t pppcorr; /* ppp corrections */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sta_t
#pragma warning restore IDE1006 // Naming Styles
        {
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 13)]        /* station parameter type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] name; /* marker name */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] marker; /* marker number */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] antdes; /* antenna descriptor */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] antsno; /* antenna serial number */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] rectype; /* receiver type descriptor */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] recver; /* receiver firmware version */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXANT)]
            public char[] recsno; /* receiver serial number */
            public int antsetup;       /* antenna setup id */
            public int itrf;           /* ITRF realization year */
            public int deltype;        /* antenna delta type (0:enu,1:xyz) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] pos;      /* station position (ecef) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] del;      /* antenna position delta (e/n/u or x/y/z) (m) */
            public double hgt;         /* antenna height (m) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct sol_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* solution type */

            public gtime_t time;       /* time (GPST) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public double[] rr;       /* position/velocity (m|m/s) */
            /* {x,y,z,vx,vy,vz} or {e,n,u,ve,vn,vu} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public float[] qr;       /* position variance/covariance (m^2) */
            /* {c_xx,c_yy,c_zz,c_xy,c_yz,c_zx} or */
            /* {c_ee,c_nn,c_uu,c_en,c_nu,c_ue} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public float[] qv;       /* velocity variance/covariance (m^2/s^2) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public double[] dtr;      /* receiver clock bias to time systems (s) */
            public byte type; /* type (0:xyz-ecef,1:enu-baseline) */
            public byte stat; /* solution status (SOLQ_???) */
            public byte ns;   /* number of valid satellites */
            public float age;          /* age of differential (s) */
            public float ratio;        /* AR ratio factor for valiation */
            public float thres;        /* AR ratio threshold for valiation */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct solbuf_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* solution buffer type */
            public int n, nmax;         /* number of solution/max number of buffer */
            public int cyclic;         /* cyclic buffer flag */
            public int start, end;      /* start/end index */
            public gtime_t time;       /* current solution time */
            public sol_t data;        /* solution data */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] rb;       /* reference position {x,y,z} (ecef) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSOLMSG + 1)]
            public byte[] buff; /* message buffer */
            public int nb;             /* number of byte in message buffer */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct solstat_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* solution status type */
            public gtime_t time;       /* time (GPST) */
            public char sat;  /* satellite number */
            public char frq;  /* frequency (1:L1,2:L2,...) */
            public float az, el;        /* azimuth/elevation angle (rad) */
            public float resp;         /* pseudorange residual (m) */
            public float resc;         /* carrier-phase residual (m) */
            public char flag; /* flags: (vsat<<5)+(slip<<3)+fix */
            public char snr;  /* signal strength (0.25 dBHz) */
            public short var_lock;  /* lock counter */
            public short outc;  /* outage counter */
            public short slipc; /* slip counter */
            public short rejc;  /* reject counter */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct solstatbuf_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* solution status buffer type */
            public int n, nmax;         /* number of solution/max number of buffer */
            public solstat_t data;    /* solution status data */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct rtcm_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* RTCM control struct type */
            public int staid;          /* station id */
            public int stah;           /* station health */
            public int seqno;          /* sequence number for rtcm 2 or iods msm */
            public int outtype;        /* output message type */
            public gtime_t time;       /* message time */
            public gtime_t time_s;     /* message start time */
            public obs_t obs;          /* observation data (uncorrected) */
            public nav_t nav;          /* satellite ephemerides */
            public sta_t sta;          /* station parameters */
            public dgps_t dgps;       /* output of dgps corrections */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public ssr_t[] ssr;  /* output of ssr corrections */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
            public char[] msg;      /* special message */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public char[] msgtype;  /* last message type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6 * 128)]
            public char[] msmtype; /* msm signal types */
            public int obsflag;        /* obs data complete flag (1:ok,0:not complete) */
            public int ephsat;         /* update satellite of ephemeris */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public double[] cp; /* carrier-phase measurement */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public short[] time_lock; /* lock time */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public short[] loss; /* loss of lock count */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public gtime_t[] lltime; /* last lock time */
            public int nbyte;          /* number of bytes in message buffer */
            public int nbit;           /* number of bits in word buffer */
            public int len;            /* message length (bytes) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1200)]
            public byte[] buff; /* message buffer */
            public uint word;  /* word buffer for rtcm 2 */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
            public uint[] nmsg2; /* message count of RTCM 2 (1-99:1-99,0:other) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 400)]
            public uint[] nmsg3; /* message count of RTCM 3 (1-299:1001-1299,300-399:2000-2099,0:ohter) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public char[] opt;      /* RTCM dependent options */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct rnxctr_t
#pragma warning restore IDE1006 // Naming Styles
        {
                    /* rinex control struct type */
            public gtime_t time;       /* message time */
            public double ver;         /* rinex version */
            public char type;        /* rinex file type ('O','N',...) */
            public int sys;         /* navigation system */
            public int tsys;        /* time system */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7 * MAXOBSTYPE * 4)]
            public char[] tobs; /* rinex obs types */
            public obs_t obs;         /* observation data */
            public nav_t nav;         /* navigation data */
            public sta_t sta;         /* station info */
            public int ephsat;      /* ephemeris satellite number */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public char[] opt;    /* rinex dependent options */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct url_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]        /* download url type */
            public char[] type;      /* data type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
            public char[] path;    /* url path */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
            public char[] dir;    /* local directory */
            public double tint;        /* time interval (s) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct opt_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* option type */
            public string name;   /* option name */
            public int format;         /* option format (0:int,1:double,2:string,3:enum) */
            public IntPtr var;          /* pointer to option variable */
            public char[] comment; /* option comment/enum labels/unit */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct exterr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* extended receiver error model */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]        
            public int[] ena;         /* model enabled */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4 * NFREQ * 2)]
            public double[] cerr; /* code errors (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4 * NFREQ * 2)]
            public double[] perr; /* carrier-phase errors (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ)]
            public double[] gpsglob; /* gps-glonass h/w bias (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ)]
            public double[] gloicb; /* glonass interchannel bias (m/fn) */


        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct snrmask_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]        /* SNR mask type */
            public int[] ena;         /* enable flag {rover,base} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ * 9)]
            public double[] mask; /* mask (dBHz) at 5,10,...85 deg */


            public snrmask_t(int ena0, int ena1)
            {
                ena = new int[2];
                mask = new double[NFREQ * 9];
                ena[0] = ena0;
                ena[1] = ena1;

                for(int i = 0; i < NFREQ * 9; i++)
                {
                    mask[i] = 0.00;
                }
            }


            
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct prcopt_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* processing options type */
            public int mode;           /* positioning mode (PMODE_???) */
            public int soltype;        /* solution type (0:forward,1:backward,2:combined) */
            public int nf;             /* number of frequencies (1:L1,2:L1+L2,3:L1+L2+L5) */
            public int navsys;         /* navigation system */
            public double elmin;       /* elevation mask angle (rad) */
            public snrmask_t snrmask;  /* SNR mask */
            public int sateph;         /* satellite ephemeris/clock (EPHOPT_???) */
            public int modear;         /* AR mode (0:off,1:continuous,2:instantaneous,3:fix and hold,4:ppp-ar) */
            public int glomodear;      /* GLONASS AR mode (0:off,1:on,2:auto cal,3:ext cal) */
            public int bdsmodear;      /* BeiDou AR mode (0:off,1:on) */
            public int maxout;         /* obs outage count to reset bias */
            public int minlock;        /* min lock count to fix ambiguity */
            public int minfix;         /* min fix count to hold ambiguity */
            public int armaxiter;      /* max iteration to resolve ambiguity */
            public int ionoopt;        /* ionosphere option (IONOOPT_???) */
            public int tropopt;        /* troposphere option (TROPOPT_???) */
            public int dynamics;       /* dynamics model (0:none,1:velociy,2:accel) */
            public int tidecorr;       /* earth tide correction (0:off,1:solid,2:solid+otl+pole) */
            public int niter;          /* number of filter iteration */
            public int codesmooth;     /* code smoothing window size (0:none) */
            public int intpref;        /* interpolate reference obs (for post mission) */
            public int sbascorr;       /* SBAS correction options */
            public int sbassatsel;     /* SBAS satellite selection (0:all) */
            public int rovpos;         /* rover position for fixed mode */
            public int refpos;         /* base position for relative mode */
            /* (0:pos in prcopt,  1:average of single pos, */
            /*  2:read from file, 3:rinex header, 4:rtcm pos) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = NFREQ)]
            public double[] eratio; /* code/phase error ratio */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
            public double[] err;      /* measurement error factor */
            /* [0]:reserved */
            /* [1-3]:error factor a/b/c of phase (m) */
            /* [4]:doppler frequency (hz) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] std;      /* initial-state std [0]bias,[1]iono [2]trop */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public double[] prn;      /* process-noise std [0]bias,[1]iono [2]trop [3]acch [4]accv [5] pos */
            public double sclkstab;    /* satellite clock stability (sec/sec) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] thresar;  /* AR validation threshold */
            public double elmaskar;    /* elevation mask of AR for rising satellite (deg) */
            public double elmaskhold;  /* elevation mask to hold ambiguity (deg) */
            public double thresslip;   /* slip threshold of geometry-free phase (m) */
            public double maxtdiff;    /* max difference of time (sec) */
            public double maxinno;     /* reject threshold of innovation (m) */
            public double maxgdop;     /* reject threshold of gdop */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] baseline; /* baseline length constraint {const,sigma} (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] ru;       /* rover position for fixed mode {x,y,z} (ecef) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] rb;       /* base position for relative mode {x,y,z} (ecef) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2 * MAXANT)]
            public char[] anttype; /* antenna types {rover,base} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2 * 3)]
            public double[] antdel; /* antenna delta {{rov_e,rov_n,rov_u},{ref_e,ref_n,ref_u}} */
            public pcv_t pcvr;      /* receiver antenna parameters {rov,base} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public char[] exsats; /* excluded satellites (1:excluded,2:included) */
            public int maxaveep;      /* max averaging epoches */
            public int initrst;       /* initialize by restart */
            public int outsingle;     /* output single by dgps/float/fix/ppp outage */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2 * 256)]
            public char[] rnxopt; /* rinex options {rover,base} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public int[] posopt;     /* positioning options */
            public int syncsol;       /* solution sync mode (0:off,1:on) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2 * 6 * 11)]
            public double[] odisp; /* ocean tide loading parameters {rov,base} */
            public exterr_t exterr;    /* extended receiver error model */
            public int freqopt;        /* disable L2-AR */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public char[] pppopt;   /* ppp option */


        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct solopt_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* solution options type */
            public int posf;           /* solution format (SOLF_???) */
            public int times;          /* time system (TIMES_???) */
            public int timef;          /* time format (0:sssss.s,1:yyyy/mm/dd hh:mm:ss.s) */
            public int timeu;          /* time digits under decimal point */
            public int degf;           /* latitude/longitude format (0:ddd.ddd,1:ddd mm ss) */
            public int outhead;        /* output header (0:no,1:yes) */
            public int outopt;         /* output processing options (0:no,1:yes) */
            public int outvel;         /* output velocity options (0:no,1:yes) */
            public int datum;          /* datum (0:WGS84,1:Tokyo) */
            public int height;         /* height (0:ellipsoidal,1:geodetic) */
            public int geoid;          /* geoid model (0:EGM96,1:JGD2000) */
            public int solstatic;      /* solution of static mode (0:all,1:single) */
            public int sstat;          /* solution statistics level (0:off,1:states,2:residuals) */
            public int trace;          /* debug trace level (0:off,1-5:debug) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] nmeaintv; /* nmea output interval (s) (<0:no,0:all) */
            /* nmeaintv[0]:gprmc,gpgga,nmeaintv[1]:gpgsv */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
            public char[] sep;       /* field separator */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
            public char[] prog;      /* program name */
            public double maxsolstd;   /* max std-dev for solution output (m) (0:all) */


        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct filopt_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* file options type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]        
            public char[] satantp; /* satellite antenna parameters file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] rcvantp; /* receiver antenna parameters file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] stapos; /* station positions file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] geoid; /* external geoid data file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] iono; /* ionosphere data file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] dcb; /* dcb data file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] eop; /* eop data file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] blq; /* ocean tide loading blq file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] tempdir; /* ftp/http temporaly directory */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] geexe; /* google earth exec file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] solstat; /* solution statistics file */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] trace; /* debug trace file */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct rnxopt_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 36)]        /* RINEX options type */
            public gtime_t ts, te;      /* time start/end */
            public double tint;        /* time interval (s) */
            public double ttol;        /* time tolerance (s) */
            public double tunit;       /* time unit for multiple-session (s) */
            public double rnxver;      /* RINEX version */
            public int navsys;         /* navigation system */
            public int obstype;        /* observation type */
            public int freqtype;       /* frequency type */
            public fixed char mask[7 * 64];   /* code mask {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
            public fixed char staid[32];    /* station id for rinex file name */
            public fixed char prog[32];    /* program */
            public fixed char runby[32];    /* run-by */
            public fixed char marker[64];    /* marker name */
            public fixed char markerno[32];  /* marker number */
            public fixed char markertype[32]; /* marker type (ver.3) */
            public fixed char name[2 * 32];   /* observer/agency */
            public fixed char rec[3 * 32];   /* receiver #/type/vers */
            public fixed char ant[3 * 32];   /* antenna #/type */
            public fixed double apppos[3];   /* approx position x/y/z */
            public fixed double antdel[3];   /* antenna delta h/e/n */
            public fixed char comment[MAXCOMMENT * 4]; /* comments */
            public fixed char rcvopt[256];   /* receiver dependent options */
            public fixed byte exsats[MAXSAT]; /* excluded satellites */
            public int scanobs;        /* scan obs types */
            public int outiono;        /* output iono correction */
            public int outtime;        /* output time system correction */
            public int outleaps;       /* output leap seconds */
            public int autopos;        /* auto approx position */
            public int halfcyc;        /* half cycle correction */
            public int sep_nav;        /* separated nav files */
            public gtime_t tstart;     /* first obs time */
            public gtime_t tend;       /* last obs time */
            public gtime_t trtcm;      /* approx log start time for rtcm */
            public fixed char tobs[7 * MAXOBSTYPE * 4]; /* obs types {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
            public fixed int nobs[7];        /* number of obs types {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct ssat_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]        /* satellite status type */
            public byte sys;  /* navigation system */
            public byte vs;   /* valid satellite flag single */
            public fixed double azel[2];     /* azimuth/elevation angles {az,el} (rad) */
            public fixed double resp[NFREQ]; /* residuals of pseudorange (m) */
            public fixed double resc[NFREQ]; /* residuals of carrier-phase (m) */
            public fixed byte vsat[NFREQ]; /* valid satellite flag */
            public fixed byte snr[NFREQ]; /* signal strength (0.25 dBHz) */
            public fixed byte fix[NFREQ]; /* ambiguity fix flag (1:fix,2:float,3:hold) */
            public fixed byte slip[NFREQ]; /* cycle-slip flag */
            public fixed byte half[NFREQ]; /* half-cycle valid flag */
            public fixed int phase_lock[NFREQ];   /* lock counter of phase */
            public fixed int outc[NFREQ]; /* obs outage counter of phase */
            public fixed int slipc[NFREQ]; /* cycle-slip counter */
            public fixed int rejc[NFREQ]; /* reject counter */
            public double gf;         /* geometry-free phase L1-L2 (m) */
            public double gf2;        /* geometry-free phase L1-L5 (m) */
            public double mw;         /* MW-LC (m) */
            public double phw;        /* phase windup (cycle) */
            public unsafe gtime_t* pt; /* previous carrier-phase time */
            public fixed double ph[2 * NFREQ]; /* previous carrier-phase observable (cycle) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct ambc_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]        /* ambiguity control type */
            public unsafe gtime_t* epoch;   /* last epoch */
            public fixed int n[4];           /* number of epochs */
            public fixed double LC[4];      /* linear combination average */
            public fixed double LCv[4];      /* linear combination variance */
            public int fixcnt;         /* fix count */
            public fixed char flags[MAXSAT]; /* fix flags */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct rtk_t
#pragma warning restore IDE1006 // Naming Styles
        {
                   /* RTK control/result type */
            public sol_t sol;           /* RTK solution */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public double[] rb;         /* base position/velocity (ecef) (m|m/s) */
            public int nx, na;          /* number of float states/fixed states */
            public double tt;           /* time difference between current and previous (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] x, P;       /* float states and their covariance */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] xa, Pa;     /* fixed states and their covariance */
            public int nfix;            /* number of continuous fixes of ambiguity */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            //public ambc_t[] ambc;       /* ambibuity control */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            //public ssat_t[] ssat;       /* satellite status */
            public int neb;             /* bytes in error message buffer */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXERRMSG)]
            public char[] errbuf;       /* error message buffer */
            public prcopt_t opt;         /* processing options */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct half_cyc_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* half-cycle correction list type */
            public byte sat;  /* satellite number */
            public byte freq; /* frequency number (0:L1,1:L2,2:L5) */
            public byte valid; /* half-cycle valid flag */
            public char corr;          /* half-cycle corrected (x 0.5 cyc) */
            public gtime_t ts, te;      /* time start, time end */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = )]
            public unsafe half_cyc_t* next; /* pointer to next correction */
        };



        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public  struct raw_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* receiver raw data control type */
            public gtime_t time;       /* message time */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public gtime_t[] tobs;     /* observation data time */
            public obs_t obs;          /* observation data */
            public obs_t obuf;         /* observation data buffer */
            public nav_t nav;          /* satellite ephemerides */
            public sta_t sta;          /* station parameters */
            public int ephsat;         /* sat number of update ephemeris (0:no satellite) */
            public sbsmsg_t sbsmsg;    /* SBAS message */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public char[] msgtype;  /* last message type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * 380)]
            public char[] subfrm;  /* subframe buffer */
            public lexmsg_t lexmsg;    /* LEX message */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public double[] lockt; /* lock time (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] icpp;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] off;
            public double icpc; /* carrier params for ss2 */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] prCA;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT)]
            public double[] dpCA; /* L1/CA pseudrange/doppler for javad */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSAT * (NFREQ + NEXOBS))]
            public byte[] halfc; /* half-cycle add flag */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXOBS)]
            public char[] freqn; /* frequency number for javad */
            public int nbyte;          /* number of bytes in message buffer */
            public int len;            /* message length (bytes) */
            public int iod;            /* issue of data */
            public int tod;            /* time of day (ms) */
            public int tbase;          /* time base (0:gpst,1:utc(usno),2:glonass,3:utc(su) */
            public int flag;           /* general purpose flag */
            public int outtype;        /* output message type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXRAWLEN)]
            public byte[] buff; /* message buffer */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXOBS)]
            public char[] opt;      /* receiver dependent options */
            public half_cyc_t half_cyc; /* half-cycle correction list */

            public int format;         /* receiver stream format */
            public IntPtr rcv_data;     /* receiver dependent data */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct stream_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* stream type */
            public int type;           /* type (STR_???) */
            public int mode;           /* mode (STR_MODE_?) */
            public int state;          /* state (-1:error,0:close,1:open) */
            public uint inb, inr;   /* input bytes/rate */
            public uint outb, outr; /* output bytes/rate */
            public uint tick_i; /* input tick tick */
            public uint tick_o; /* output tick */
            public uint tact;  /* active tick */
            public uint inbt, outbt; /* input/output bytes at tick */
            public CRITICAL_SECTION lock_flag;        /* lock flag */
            public IntPtr port;         /* type dependent port control struct */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRPATH)]
            public char[] path; /* stream path */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSTRMSG)]
            public char[] ms;  /* stream message */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct strconv_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* stream converter type */
            public int itype, otype;    /* input and output stream type */
            public int nmsg;           /* number of output messages */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public int[] msgs;       /* output message types */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public double[] tint;    /* output message intervals (s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public int[] tick; /* cycle tick of output message */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public int[] ephsat;     /* satellites of output ephemeris */
            public int stasel;         /* station info selection (0:remote,1:local) */
            public rtcm_t rtcm;        /* rtcm input data buffer */
            public raw_t raw;          /* raw  input data buffer */
            public rtcm_t _out;         /* rtcm output data buffer */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct strsvr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* stream server type */
            public int state;          /* server state (0:stop,1:running) */
            public int cycle;          /* server cycle (ms) */
            public int buffsize;       /* input/monitor buffer size (bytes) */
            public int nmeacycle;      /* NMEA request cycle (ms) (0:no) */
            public int relayback;      /* relay back of output streams (0:no) */
            public int nstr;           /* number of streams (1 input + (nstr-1) outputs */
            public int npb;            /* data length in peek buffer (bytes) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16 * MAXRCVCMD)]
            public char[] cmds_periodic; /* periodic commands */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] nmeapos;  /* NMEA request position (ecef) (m) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
            public byte[] buff; /* input buffers */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
            public byte[] pbuf; /* peek buffer */
            public uint tick;  /* start tick */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public stream_t[] stream; /* input/output streams */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public strconv_t[] conv; /* stream converter */
            public IntPtr thread;    /* server thread */
            public CRITICAL_SECTION lock_flag;        /* lock flag */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 0)]
#pragma warning disable IDE1006 // Naming Styles
        public struct rtksvr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* RTK server type */
            public int state;          /* server state (0:stop,1:running) */
            public int cycle;          /* processing cycle (ms) */
            public int nmeacycle;      /* NMEA request cycle (ms) (0:no req) */
            public int nmeareq;        /* NMEA request (0:no,1:nmeapos,2:single sol) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] nmeapos;  /* NMEA request position (ecef) (m) */
            public int buffsize;       /* input buffer size (bytes) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] format;      /* input format {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public solopt_t[] solopt; /* output solution options {sol1,sol2} */
            public int navsel;         /* ephemeris select (0:all,1:rover,2:base,3:corr) */
            public int nsbs;           /* number of sbas message */
            public int nsol;           /* number of solution buffer */
            public rtk_t rtk;          /* RTK control/result struct */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] nb;         /* bytes in input buffers {rov,base} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public int[] nsb;         /* bytes in soulution buffers */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] npb;         /* bytes in input peek buffers */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public IntPtr[] buff; /* input buffers {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public IntPtr[] sbuf; /* output buffers {sol1,sol2} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public IntPtr[] pbuf; /* peek buffers {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSOLBUF)]
            public sol_t[] solbuf; /* solution buffer */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * 10)]
            public int[] nmsg; /* input message counts */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public raw_t[] raw;     /* receiver raw control {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public rtcm_t[] rtcm;     /* RTCM control {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public gtime_t[] ftime;   /* download time {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * MAXSTRPATH)]
            public char[] files; /* download paths {rov,base,corr} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * MAXOBSBUF)]
            public obs_t[] obs; /* observation data {rov,base,corr} */
            public nav_t nav;          /* navigation data */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSBSMSG)]
            public sbsmsg_t[] sbsmsg; /* SBAS message buffer */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public stream_t[] stream; /* streams {rov,base,corr,sol1,sol2,logr,logb,logc} */
            public IntPtr moni;     /* monitor stream */
            public uint tick;  /* start tick */
            public IntPtr thread;    /* server thread */
            public int cputime;        /* CPU time (ms) for a processing cycle */
            public int prcout;         /* missing observation data count */
            public int nave;           /* number of averaging base pos */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] rb_ave;   /* averaging base pos */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * MAXRCVCMD)]
            public char[] cmds_periodic; /* periodic commands */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXRCVCMD)]
            public char[] cmd_reset; /* reset command */
            public double bl_reset;    /* baseline length to reset (km) */
            public CRITICAL_SECTION lock_flag;        /* lock flag */

            public static explicit operator rtksvr_t(rtksvr_cls v)
            {
                throw new NotImplementedException();
            }
        };

        public class rtksvr_cls
        {
            /* RTK server type */
            public int state;          /* server state (0:stop,1:running) */
            public int cycle;          /* processing cycle (ms) */
            public int nmeacycle;      /* NMEA request cycle (ms) (0:no req) */
            public int nmeareq;        /* NMEA request (0:no,1:nmeapos,2:single sol) */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] nmeapos;  /* NMEA request position (ecef) (m) */
            public int buffsize;       /* input buffer size (bytes) */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] format;      /* input format {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = 2)]
            public solopt_t[] solopt; /* output solution options {sol1,sol2} */
            public int navsel;         /* ephemeris select (0:all,1:rover,2:base,3:corr) */
            public int nsbs;           /* number of sbas message */
            public int nsol;           /* number of solution buffer */
            public rtk_t rtk;          /* RTK control/result struct */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] nb;         /* bytes in input buffers {rov,base} */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public int[] nsb;         /* bytes in soulution buffers */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int[] npb;         /* bytes in input peek buffers */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public byte[] buff; /* input buffers {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public byte[] sbuf; /* output buffers {sol1,sol2} */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public byte[] pbuf; /* peek buffers {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXSOLBUF)]
            public sol_t[] solbuf; /* solution buffer */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * 10)]
            public int[] nmsg; /* input message counts */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = 3)]
            public raw_t[] raw;     /* receiver raw control {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = 3)]
            public rtcm_t[] rtcm;     /* RTCM control {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = 3)]
            public gtime_t[] ftime;   /* download time {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * MAXSTRPATH)]
            public char[] files; /* download paths {rov,base,corr} */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = 3 * MAXOBSBUF)]
            public obs_t[] obs; /* observation data {rov,base,corr} */
            public nav_t nav;          /* navigation data */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = MAXSBSMSG)]
            public sbsmsg_t[] sbsmsg; /* SBAS message buffer */
            //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.LPStruct, SizeConst = 8)]
            public stream_t[] stream; /* streams {rov,base,corr,sol1,sol2,logr,logb,logc} */
            public IntPtr moni;     /* monitor stream */
            public uint tick;  /* start tick */
            public IntPtr thread;    /* server thread */
            public int cputime;        /* CPU time (ms) for a processing cycle */
            public int prcout;         /* missing observation data count */
            public int nave;           /* number of averaging base pos */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] rb_ave;   /* averaging base pos */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * MAXRCVCMD)]
            public char[] cmds_periodic; /* periodic commands */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXRCVCMD)]
            public char[] cmd_reset; /* reset command */
            public double bl_reset;    /* baseline length to reset (km) */
            public CRITICAL_SECTION lock_flag;        /* lock flag */
        
    }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct gis_pnt_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* gis data point type */
            public double[] pos;      /* point data {lat,lon,height} (rad,m) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct gis_poly_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* gis data polyline type */
            public int npnt;           /* number of points */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] bound;    /* boundary {lat0,lat1,lon0,lon1} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] pos;        /* position data (3 x npnt) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct gis_polygon_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* gis data polygon type */
            public int npnt;           /* number of points */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] bound;    /* boundary {lat0,lat1,lon0,lon1} */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] pos;        /* position data (3 x npnt) */
        };



        #region ВОПРОС
#pragma warning disable IDE1006 // Naming Styles
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
        public struct gisd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            /* gis data list type */
            public int type;           /* data type (1:point,2:polyline,3:polygon) */
            public IntPtr data;         /* data body */
            public unsafe gisd_t* next; /* pointer to next */
        };
        #endregion


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct gis_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXGISLAYER * 256)]        /* gis type */
            public char name; /* name */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXGISLAYER)]
            public int flag;     /* flag */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = MAXGISLAYER)]
            public gisd_t[] data; /* gis data list */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public double[] bound;    /* boundary {lat0,lat1,lon0,lon1} */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct imud_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* imu data type */
            public gtime_t time;       /* time */
            public int stat;           /* status */
            public int seqno;          /* sequence number */
            public float temp;         /* temperature (C) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] rot;      /* rotation rate {x,y,z} (rad/s) */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public double[] acc;      /* acceleration data {x,y,z} (m/s^2) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
#pragma warning disable IDE1006 // Naming Styles
        public struct imu_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* imu type */
            public imud_t data;        /* imu data */
            public int nbyte;          /* bytes in imu data buffer */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public byte[] buff; /* imu data buffer */
        };

        #endregion


#region Инициализация структур по-умолчанию
#if DEBUG

        
        public static prcopt_t prcopt_default = new prcopt_t
        { /* defaults processing options */
            mode = PMODE_SINGLE,
            soltype = 0,
            nf = 2,
            navsys = SYS_GPS,   /* mode,soltype,nf,navsys */
            elmin = 15.0 * D2R,
            snrmask = new snrmask_t(0,0),           /* elmin,snrmask */

            sateph = 0,
            modear = 1,
            glomodear = 1,
            bdsmodear = 1,                    /* sateph,modear,glomodear,bdsmodear */
            maxout = 5,
            minlock = 0,
            minfix = 10,
            armaxiter = 1,                   /* maxout,minlock,minfix,armaxiter */
            ionoopt = 0,
            tropopt = 0,
            dynamics = 0,
            tidecorr = 0,
            niter = 1,
            codesmooth = 0,
            intpref = 0,
            sbascorr = 0,
            sbassatsel = 0,
            rovpos = 0,
            refpos = 0,
            eratio = new double[] { 100.0, 100.0 },              /* eratio[] */
            err = new double[] { 100.0, 0.003, 0.003, 0.0, 1.0 },
            std = new double[] { 30.0, 0.03, 0.3 },            /* std[] */
            prn = new double[] { 1E-4, 1E-3, 1E-4, 1E-1, 1E-2, 0.0 }, /* prn[] */
            sclkstab = 5E-12,                      /* sclkstab */
            thresar = new double[] { 3.0, 0.9999, 0.25, 0.1, 0.05 }, /* thresar */
            elmaskar = 0.0,
            elmaskhold = 0.0,
            thresslip = 0.05,               /* elmaskar,almaskhold,thresslip */
            maxtdiff = 30.0,
            maxinno = 30.0,
            maxgdop = 30.0,             /* maxtdif,maxinno,maxgdop */
            baseline = new double[] { 0 },
            ru = new double[] { 0 },
            rb = new double[] { 0 },                /* baseline,ru,rb */
            anttype = Array.Empty<char>(),                    /* anttype */
            antdel = new double[] { 0 },
            exterr = new exterr_t(),
            exsats = Array.Empty<char>()             /* antdel,pcv,exsats */
        };



        public static solopt_t solopt_default = new solopt_t
        { /* defaults solution output options */
            posf = SOLF_LLH,
            times = TIMES_GPST,
            timef = 1,
            timeu = 3,    /* posf,times,timef,timeu */
            degf = 0,
            outhead = 1,
            outopt = 0,
            outvel = 0,
            datum = 0,
            height = 0,
            geoid = 0,              /* degf,outhead,outopt,outvel,datum,height,geoid */
            solstatic = 0,
            sstat = 0,
            trace = 0,                      /* solstatic,sstat,trace */
            nmeaintv = new double[] { 0.00, 0.00 },                  /* nmeaintv */
            sep = new char[] { Convert.ToChar(65) },
            prog = Array.Empty<char>(),                      /* separator/program name */
            maxsolstd = 0.00

        };
#endif
#endregion


#region Функции из библиотеки
        [DllImport(@"C:\Users\zharkov.v\source\repos\RealTimeMonitor\RTKLib\debug\RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "epoch2time")] //, EntryPoint = "epoch2time"
        public static extern gtime_t epoch2time(double[] ep);
        
        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "strinitcom")]
        public static extern void strinitcom();

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "rtksvrinit")]
        public static extern int rtksvrinit(IntPtr svr);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "strinit")]
        public static extern void strinit(ref stream_t stream);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "timeget")]
        public static extern gtime_t timeget();

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "rtksvrlock")]
        public static extern void rtksvrlock(IntPtr svr);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "rtksvrunlock")]
        public static extern void rtksvrunlock(IntPtr svr);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "readpcv")]
        public static extern int readpcv(string file, ref pcvs_t pcvs);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "pos2ecef")]
        public static extern void pos2ecef(double[] pos, double[] r);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "strsetdir")]
        public static extern void strsetdir(string dir);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "strsetproxy")]
        public static extern void strsetproxy(string addr);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "traceopen")]
        public static extern void traceopen(string file);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "tracelevel")]
        public static extern void tracelevel(int level);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "rtkopenstat")]
        public static extern int rtkopenstat(string file, int level);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "opengeoid")]
        public static extern int opengeoid(int model, string file);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "readdcb")]
        public static extern int readdcb(string file, ref nav_t nav, ref sta_t sta);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "strsetopt")]
        public static extern void strsetopt(int[] opt);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "rtksvrstart")]
        public static extern int rtksvrstart(IntPtr svr, int cycle, int buffsize, int[] strs,
                         string[] paths, int[] formats, int navsel, string[] cmds,
                         string[] cmds_periodic, string[] rcvopts, int nmeacycle,
                         int nmeareq, double[] nmeapos, ref prcopt_t prcopt,
                         solopt_t[] solopt, ref stream_t moni, string errmsg);

        //[DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "trace")]
        //public static extern void trace(int level, string format, ...);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "traceclose")]
        public static extern void traceclose();

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "ecef2pos")]
        public static extern void ecef2pos(double[] r, double[] pos);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "ecef2enu")]
        public static extern void ecef2enu(double[] pos, double[] r, double[] e);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "covenu")]
        public static extern void covenu(double[] pos, double[] P, double[] Q);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "geoidh")]
        public static extern double geoidh(double[] pos);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "norm")]
        public static extern double norm(double[] a, int n);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "rtksvrsstat")]
        public static extern void rtksvrsstat(IntPtr svr, int[] sstat, char[] msg);

        [DllImport("RTKLib.dll", CharSet = CharSet.Ansi, EntryPoint = "strwrite")]
        public static extern int strwrite(ref stream_t stream, byte[] buff, int n);



#endregion
    }
}
