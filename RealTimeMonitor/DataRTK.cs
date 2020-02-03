

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


        //#endif


        /*
        #else
                //C++ TO C# CONVERTER TODO TASK: #define macros defined in multiple preprocessor conditionals can only be replaced within the scope of the preprocessor conditional:
                //C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
                //ORIGINAL LINE: #define thread_t pthread_t
        #define thread_t
                //C++ TO C# CONVERTER TODO TASK: #define macros defined in multiple preprocessor conditionals can only be replaced within the scope of the preprocessor conditional:
                //C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
                //ORIGINAL LINE: #define lock_t pthread_mutex_t
        #define lock_t
                //C++ TO C# CONVERTER TODO TASK: #define macros defined in multiple preprocessor conditionals can only be replaced within the scope of the preprocessor conditional:
                //C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
                //ORIGINAL LINE: #define initlock(f) pthread_mutex_init(f,NULL)
        #define initlock
                //C++ TO C# CONVERTER TODO TASK: #define macros defined in multiple preprocessor conditionals can only be replaced within the scope of the preprocessor conditional:
                //C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
                //ORIGINAL LINE: #define lock(f) pthread_mutex_lock(f)
        #define lock
                //C++ TO C# CONVERTER TODO TASK: #define macros defined in multiple preprocessor conditionals can only be replaced within the scope of the preprocessor conditional:
                //C++ TO C# CONVERTER NOTE: The following #define macro was replaced in-line:
                //ORIGINAL LINE: #define unlock(f) pthread_mutex_unlock(f)
        #define unlock
        #define FILEPATHSEP
                    */


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


        const int NEXOBS = 0;
        const int NFREQ = 3;
        const int MAXANT = 64;
        const int MAXSAT = 255;
        const int MAXNIGP = 201;
        const int MAXCODE = 55;
        const int MAXRCV = 64;
        const int MAXSTA = 255;
        const int MAXPRNGLO = 27;
        const int MAXBAND = 10;
        const int MAXERRMSG = 4096;
        const int MAXSTRPATH = 1024;
        const int MAXSOLMSG = 8191;
        const int MAXOBSTYPE = 64;
        const int MAXCOMMENT = 10;
        const int MAXOBS = 64;
        const int MAXRAWLEN = 4096;
        const int MAXSTRMSG = 1024;
        const int MAXRCVCMD = 4096;
        const int MAXSOLBUF = 256;
        const int MAXGISLAYER = 32;
        const int MAXOBSBUF = 128;




        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]   /* time struct */
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct gtime_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public Int64 time;                                          /* time (s) expressed by standard time_t */
            public double sec;                                          /* fraction of second under 1 s */
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]   /* observation data record */
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct obsd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
            public gtime_t time;                                        /* receiver sampling time (GPST) */
            public byte sat, rcv;                                       /* satellite/receiver number */
            public fixed byte SNR[NFREQ + NEXOBS];                            /* signal strength (0.25 dBHz) */
            public fixed byte LLI[NFREQ + NEXOBS];                            /* loss of lock indicator */
            public fixed byte code[NFREQ + NEXOBS];                           /* code indicator (CODE_???) */
            public fixed double L[NFREQ + NEXOBS];                            /* observation data carrier-phase (cycle) */
            public fixed double P[NFREQ + NEXOBS];                            /* observation data pseudorange (m) */
            public fixed float D[NFREQ + NEXOBS];                             /* observation data doppler frequency (Hz) */
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* observation data */
#pragma warning disable IDE1006 // Naming Styles
        public struct obs_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int n, nmax;                                         /* number of obervation data/allocated */
            public unsafe obsd_t* data;                                         /* observation data records */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* earth rotation parameter data type */
#pragma warning disable IDE1006 // Naming Styles
        public struct erpd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
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
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int n, nmax;                                         /* number and max number of data */
            public unsafe erpd_t* data;                                         /* earth rotation parameter data */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* antenna parameter type */
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct pcv_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 27)]
            public int sat;                                             /* antenna type */
            public fixed char type[MAXANT];                                   /* serial number or satellite code */
            public fixed char code[MAXANT];                                   /* valid time start and end */
            public gtime_t ts, te;                                      /* phase center offset e/n/u or x/y/z (m) */
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
            public fixed double off[NFREQ*3];                               /* phase center offset e/n/u or x/y/z (m) */
            public fixed double var[NFREQ*19];                                       /* phase center variation (m) */
                                                                        /* el=90,85,...,0 or nadir=0,1,2,3,... (deg) */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* antenna parameters type */
#pragma warning disable IDE1006 // Naming Styles
        public struct pcvs_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public int n, nmax;                                         /* number of data/allocated */
            public unsafe pcv_t* pcv;                                          /* antenna parameters data */

        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]    /* almanac type */
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct alm
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
            public int sat;                                             /* antenna type */
            public fixed char type[MAXANT];                                   /* serial number or satellite code */
            public fixed char code[MAXANT];                                   /* valid time start and end */
            public gtime_t ts, te;                                      /* phase center offset e/n/u or x/y/z (m) */
            public fixed double off[NFREQ*3];                               /* phase center offset e/n/u or x/y/z (m) */
            public fixed double var[NFREQ*19];                                      /* phase center variation (m) */
            /* el=90,85,...,0 or nadir=0,1,2,3,... (deg) */

        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct alm_t                                                 /* almanac type */
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]
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
        public unsafe struct eph_t
#pragma warning restore IDE1006 // Naming Styles
        {                                                               /* GPS/QZS/GAL broadcast ephemeris type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 34)]
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
            public fixed double tgd[4];      /* group delay parameters */
                /* GPS/QZS:tgd[0]=TGD */
                /* GAL    :tgd[0]=BGD E5a/E1,tgd[1]=BGD E5b/E1 */
                /* CMP    :tgd[0]=BGD1,tgd[1]=BGD2 */
            public double Adot, ndot;   /* Adot,ndot for CNAV */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct geph_t
#pragma warning restore IDE1006 // Naming Styles
        {        /* GLONASS broadcast ephemeris type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 14)]
            public int sat;            /* satellite number */
            public int iode;           /* IODE (0-6 bit of tb field) */
            public int frq;            /* satellite frequency number */
            public int svh, sva, age;    /* satellite health, accuracy, age of operation */
            public gtime_t toe;        /* epoch of epherides (gpst) */
            public gtime_t tof;        /* message frame time (gpst) */
            public fixed double pos[3];      /* satellite position (ecef) (m) */
            public fixed double vel[3];      /* satellite velocity (ecef) (m/s) */
            public fixed double acc[3];      /* satellite acceleration (ecef) (m/s^2) */
            public double taun, gamn;   /* SV clock bias (s)/relative freq bias */
            public double dtaun;       /* delay between L1 and L2 (s) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct peph_t
#pragma warning restore IDE1006 // Naming Styles
        {        /* precise ephemeris type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public gtime_t time;       /* time (GPST) */
            public int index;          /* ephemeris index for multiple files */
            public fixed double pos[MAXSAT*4]; /* satellite position/clock (ecef) (m|s) */
            public fixed float std[MAXSAT*4]; /* satellite position/clock std (m|s) */
            public fixed double vel[MAXSAT*4]; /* satellite velocity/clk-rate (m/s|s/s) */
            public fixed float vst[MAXSAT*4]; /* satellite velocity/clk-rate std (m/s|s/s) */
            public fixed float cov[MAXSAT*3]; /* satellite position covariance (m^2) */
            public fixed float vco[MAXSAT*3]; /* satellite velocity covariance (m^2) */
        } ;

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct pclk_t
#pragma warning restore IDE1006 // Naming Styles
        {
            /* precise clock type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public gtime_t time;       /* time (GPST) */
            public int index;          /* clock index for multiple files */
            public fixed double clk[MAXSAT]; /* satellite clock (s) */
            public fixed float std[MAXSAT]; /* satellite clock std (s) */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct seph_t
#pragma warning restore IDE1006 // Naming Styles
        {        /* SBAS ephemeris type */
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
            public int sat;            /* satellite number */
            public gtime_t t0;         /* reference epoch time (GPST) */
            public gtime_t tof;        /* time of message frame (GPST) */
            public int sva;            /* SV accuracy (URA index) */
            public int svh;            /* SV health (0:ok) */
            public fixed double pos[3];      /* satellite position (m) (ecef) */
            public fixed double vel[3];      /* satellite velocity (m/s) (ecef) */
            public fixed double acc[3];      /* satellite acceleration (m/s^2) (ecef) */
            public double af0, af1;     /* satellite clock-offset/drift (s,s/s) */
        };


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct tled_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18)]
            /* norad two line element data type */
            public fixed char name[32];     /* common name */
            public fixed char alias[32];     /* alias name */
            public fixed char satno[16];     /* satellilte catalog number */
            public char satclass;      /* classification */
            public fixed char desig[16];     /* international designator */
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
                public unsafe tled_t* data;       /* norad two line element data */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct tec_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            /* TEC grid type */
            public gtime_t time;       /* epoch time (GPST) */
            public fixed int ndata[3];       /* TEC grid data size {nlat,nlon,nhgt} */
            public double rb;          /* earth radius (km) */
            public fixed double lats[3];     /* latitude start/interval (deg) */
            public fixed double lons[3];     /* longitude start/interval (deg) */
            public fixed double hgts[3];     /* heights start/interval (km) */
            public unsafe double* data;       /* TEC grid data (tecu) */
            public unsafe float* rms;         /* RMS values (tecu) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct fcbd_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            /* satellite fcb data type */
            public gtime_t ts, te;      /* start/end time (GPST) */
            public fixed double bias[MAXSAT*3]; /* fcb value   (cyc) */
            public fixed double std[MAXSAT*3]; /* fcb std-dev (cyc) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct sbsmsg_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            /* SBAS message type */
            public int week, tow;       /* receiption time */
            public int prn;            /* SBAS satellite PRN number */
            public fixed byte msg[29]; /* SBAS message (226bit) padded by 0 */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct sbs_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)] 
            /* SBAS messages type */
            public int n, nmax;         /* number of SBAS messages/allocated */
            public unsafe sbsmsg_t* msgs;     /* SBAS messages */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsfcorr_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
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
        public unsafe struct sbslcorr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]        
            /* SBAS long term satellite error correction type */
            public gtime_t t0;         /* correction time */
            public int iode;           /* IODE (issue of date ephemeris) */
            public fixed double dpos[3];     /* delta position (m) (ecef) */
            public fixed double dvel[3];     /* delta velocity (m/s) (ecef) */
            public  double daf0, daf1;   /* delta clock-offset/drift (s,s/s) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct sbssatp_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        
            /* SBAS satellite correction type */
            public int sat;            /* satellite number */
            public sbsfcorr_t fcorr;   /* fast correction */
            public sbslcorr_t lcorr;   /* long term correction */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct sbssat_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]        
            /* SBAS satellite corrections type */
            public int iodp;           /* IODP (issue of date mask) */
            public int nsat;           /* number of satellites */
            public int tlat;           /* system latency (s) */
            public unsafe sbssatp_t* sat; /* satellite correction */
        } ;

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct sbsigp_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]        
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
        public unsafe struct sbsion_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        
            /* SBAS ionospheric corrections type */
            public int iodi;           /* IODI (issue of date ionos corr) */
            public int nigp;           /* number of igps */
            public unsafe sbsigp_t* igp;      /* ionospheric correction */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct dgps_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]        
            /* DGPS/GNSS correction type */
            public gtime_t t0;         /* correction time */
            public double prc;         /* pseudorange correction (PRC) (m) */
            public double rrc;         /* range rate correction (RRC) (m/s) */
            public int iod;            /* issue of data (IOD) */
            public double udre;        /* UDRE */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct ssr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 17)]        
            /* SSR correction type */
            public unsafe gtime_t* t0;      /* epoch time (GPST) {eph,clk,hrclk,ura,bias,pbias} */
            public fixed double udi[6];      /* SSR update interval (s) */
            public fixed int iod[6];         /* iod ssr {eph,clk,hrclk,ura,bias,pbias} */
            public int iode;           /* issue of data */
            public int iodcrc;         /* issue of data crc for beidou/sbas */
            public int ura;            /* URA indicator */
            public int refd;           /* sat ref datum (0:ITRF,1:regional) */
            public fixed double deph[3];    /* delta orbit {radial,along,cross} (m) */
            public fixed double ddeph[3];    /* dot delta orbit {radial,along,cross} (m/s) */
            public fixed double dclk[3];    /* delta clock {c0,c1,c2} (m,m/s,m/s^2) */
            public double hrclk;       /* high-rate clock corection (m) */
            public fixed float cbias[MAXCODE]; /* code biases (m) */
            public fixed double pbias[MAXCODE]; /* phase biases (m) */
            public fixed float stdpb[MAXCODE]; /* std-dev of phase biases (m) */
            public double yaw_ang, yaw_rate; /* yaw angle and yaw rate (deg,deg/s) */
            public byte update; /* update flag (0:no update,1:update) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct lexmsg_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]        
            /* QZSS LEX message type */
            public int prn;            /* satellite PRN number */
            public int type;           /* message type */
            public int alert;          /* alert flag */
            public byte stat; /* signal tracking status */
            public byte snr;  /* signal C/N0 (0.25 dBHz) */
            public int ttt;   /* tracking time (ms) */
            public fixed byte msg[212]; /* LEX message data part 1695 bits */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct lex_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* QZSS LEX messages type */
            public int n, nmax;         /* number of LEX messages and allocated */
            public unsafe lexmsg_t *msgs;     /* LEX messages */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct lexeph_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* QZSS LEX ephemeris type */
            public gtime_t toe;        /* epoch time (GPST) */
            public gtime_t tof;        /* message frame time (GPST) */
            public int sat;            /* satellite number */
            public byte health; /* signal health (L1,L2,L1C,L5,LEX) */
            public byte ura;  /* URA index */
            public fixed double pos[3];      /* satellite position (m) */
            public fixed double vel[3];      /* satellite velocity (m/s) */
            public fixed double acc[3];      /* satellite acceleration (m/s2) */
            public fixed double jerk[3];     /* satellite jerk (m/s3) */
            public double af0, af1;     /* satellite clock bias and drift (s,s/s) */
            public double tgd;         /* TGD */
            public fixed double isc[8];      /* ISC */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct lexion_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]        /* QZSS LEX ionosphere correction type */
            public gtime_t t0;         /* epoch time (GPST) */
            public double tspan;       /* valid time span (s) */
            public fixed double pos0[2];     /* reference position {lat,lon} (rad) */
            public fixed double coef[3*2];  /* coefficients lat x lon (3 x 2) */
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct stec_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]        /* stec data type */
            public gtime_t time;       /* time (GPST) */
            public byte sat;  /* satellite number */
            public double ion;         /* slant ionos delay (m) */
            public float std;          /* std-dev (m) */
            public fixed float azel[2];      /* azimuth/elevation (rad) */
            public byte flag; /* fix flag */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct trop_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* trop data type */
            public gtime_t time;       /* time (GPST) */
            public fixed double trp[3];      /* zenith tropos delay/gradient (m) */
            public fixed float std[3];       /* std-dev (m) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct pppcorr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]        /* ppp corrections type */
            public int nsta;           /* number of stations */
            public fixed char stas[MAXSTA*8]; /* station names */
            public fixed double rr[MAXSTA*3]; /* station ecef positions (m) */
            public fixed int ns[MAXSTA], nsmax[MAXSTA]; /* number of stec data */
            public fixed int nt[MAXSTA], ntmax[MAXSTA]; /* number of trop data */
            public unsafe stec_t* stec; /* stec data */
            public unsafe trop_t* trop; /* trop data */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct nav_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 53)]        /* navigation data type */
            public int n, nmax;         /* number of broadcast ephemeris */
            public int ng, ngmax;       /* number of glonass ephemeris */
            public int ns, nsmax;       /* number of sbas ephemeris */
            public int ne, nemax;       /* number of precise ephemeris */
            public int nc, ncmax;       /* number of precise clock */
            public int na, namax;       /* number of almanac data */
            public int nt, ntmax;       /* number of tec grid data */
            public int nf, nfmax;       /* number of satellite fcb data */
            public unsafe eph_t* eph;         /* GPS/QZS/GAL ephemeris */
            public unsafe geph_t* geph;       /* GLONASS ephemeris */
            public unsafe seph_t seph;       /* SBAS ephemeris */
            public unsafe peph_t* peph;       /* precise ephemeris */
            public unsafe pclk_t* pclk;       /* precise clock */
            public unsafe alm_t* alm;         /* almanac data */
            public unsafe tec_t* tec;         /* tec grid data */
            public unsafe fcbd_t* fcb;        /* satellite fcb data */
            public erp_t erp;         /* earth rotation parameters */
            public fixed double utc_gps[4];  /* GPS delta-UTC parameters {A0,A1,T,W} */
            public fixed double utc_glo[4];  /* GLONASS UTC GPS time parameters */
            public fixed double utc_gal[4];  /* Galileo UTC GPS time parameters */
            public fixed double utc_qzs[4];  /* QZS UTC GPS time parameters */
            public fixed double utc_cmp[4];  /* BeiDou UTC parameters */
            public fixed double utc_irn[4];  /* IRNSS UTC parameters */
            public fixed double utc_sbs[4];  /* SBAS UTC parameters */
            public fixed double ion_gps[8];  /* GPS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            public fixed double ion_gal[4];  /* Galileo iono model parameters {ai0,ai1,ai2,0} */
            public fixed double ion_qzs[8];  /* QZSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            public fixed double ion_cmp[8];  /* BeiDou iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            public fixed double ion_irn[8];  /* IRNSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
            public int leaps;          /* leap seconds (s) */
            public fixed double lam[MAXSAT*NFREQ]; /* carrier wave lengths (m) */
            public fixed double cbias[MAXSAT*3]; /* satellite dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
            public fixed double rbias[MAXRCV*2*3]; /* receiver dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
            public fixed double wlbias[MAXSAT];   /* wide-lane bias (cycle) */
            public fixed double glo_cpbias[4];    /* glonass code-phase bias {1C,1P,2C,2P} (m) */
            public fixed char glo_fcn[MAXPRNGLO + 1]; /* glonass frequency channel number + 8 */
            public unsafe pcv_t* pcvs; /* satellite antenna pcv */
            public sbssat_t sbssat;    /* SBAS satellite corrections */
            public unsafe sbsion_t* sbsion; /* SBAS ionosphere corrections */
            public unsafe dgps_t* dgps; /* DGPS corrections */
            public unsafe ssr_t* ssr;  /* SSR corrections */
            public unsafe lexeph_t* lexeph; /* LEX ephemeris */
            public lexion_t lexion;    /* LEX ionosphere correction */
            public pppcorr_t pppcorr;  /* ppp corrections */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct sta_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 13)]        /* station parameter type */
            public fixed char name[MAXANT]; /* marker name */
            public fixed char marker[MAXANT]; /* marker number */
            public fixed char antdes[MAXANT]; /* antenna descriptor */
            public fixed char antsno[MAXANT]; /* antenna serial number */
            public fixed char rectype[MAXANT]; /* receiver type descriptor */
            public fixed char recver[MAXANT]; /* receiver firmware version */
            public fixed char recsno[MAXANT]; /* receiver serial number */
            public int antsetup;       /* antenna setup id */
            public int itrf;           /* ITRF realization year */
            public int deltype;        /* antenna delta type (0:enu,1:xyz) */
            public fixed double pos[3];      /* station position (ecef) (m) */
            public fixed double del[3];      /* antenna position delta (e/n/u or x/y/z) (m) */
            public double hgt;         /* antenna height (m) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct sol_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 11)]        /* solution type */
                
            public gtime_t time;       /* time (GPST) */
            public fixed double rr[6];       /* position/velocity (m|m/s) */
                /* {x,y,z,vx,vy,vz} or {e,n,u,ve,vn,vu} */
            public fixed float qr[6];       /* position variance/covariance (m^2) */
                /* {c_xx,c_yy,c_zz,c_xy,c_yz,c_zx} or */
                /* {c_ee,c_nn,c_uu,c_en,c_nu,c_ue} */
            public fixed float qv[6];       /* velocity variance/covariance (m^2/s^2) */
            public fixed double dtr[6];      /* receiver clock bias to time systems (s) */
            public byte type; /* type (0:xyz-ecef,1:enu-baseline) */
            public byte stat; /* solution status (SOLQ_???) */
            public byte ns;   /* number of valid satellites */
            public float age;          /* age of differential (s) */
            public float ratio;        /* AR ratio factor for valiation */
            public float thres;        /* AR ratio threshold for valiation */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct solbuf_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]        /* solution buffer type */
            public int n, nmax;         /* number of solution/max number of buffer */
            public int cyclic;         /* cyclic buffer flag */
            public int start, end;      /* start/end index */
            public gtime_t time;       /* current solution time */
            public unsafe sol_t* data;        /* solution data */
            public fixed double rb[3];       /* reference position {x,y,z} (ecef) (m) */
            public fixed byte buff[MAXSOLMSG + 1]; /* message buffer */
            public int nb;             /* number of byte in message buffer */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct solstat_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 13)]        /* solution status type */
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
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* solution status buffer type */
            public int n, nmax;         /* number of solution/max number of buffer */
            public unsafe solstat_t* data;    /* solution status data */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct rtcm_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 28)]        /* RTCM control struct type */
            public int staid;          /* station id */
            public int stah;           /* station health */
            public int seqno;          /* sequence number for rtcm 2 or iods msm */
            public int outtype;        /* output message type */
            public gtime_t time;       /* message time */
            public gtime_t time_s;     /* message start time */
            public obs_t obs;          /* observation data (uncorrected) */
            public nav_t nav;          /* satellite ephemerides */
            public sta_t sta;          /* station parameters */
            public unsafe dgps_t* dgps;       /* output of dgps corrections */
            public unsafe ssr_t* ssr;  /* output of ssr corrections */
            public fixed char msg[128];      /* special message */
            public fixed char msgtype[256];  /* last message type */
            public fixed char msmtype[6*128]; /* msm signal types */
            public int obsflag;        /* obs data complete flag (1:ok,0:not complete) */
            public int ephsat;         /* update satellite of ephemeris */
            public fixed double cp[MAXSAT*(NFREQ+NEXOBS)]; /* carrier-phase measurement */
            public fixed short time_lock[MAXSAT*(NFREQ+NEXOBS)]; /* lock time */
            public fixed short loss[MAXSAT*(NFREQ+NEXOBS)]; /* loss of lock count */
            public unsafe gtime_t** lltime; /* last lock time */
            public int nbyte;          /* number of bytes in message buffer */
            public int nbit;           /* number of bits in word buffer */
            public int len;            /* message length (bytes) */
            public fixed byte buff[1200]; /* message buffer */
            public uint word;  /* word buffer for rtcm 2 */
            public fixed uint nmsg2[100]; /* message count of RTCM 2 (1-99:1-99,0:other) */
            public fixed uint nmsg3[400]; /* message count of RTCM 3 (1-299:1001-1299,300-399:2000-2099,0:ohter) */
            public fixed char opt[256];      /* RTCM dependent options */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct rnxctr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 11)]        /* rinex control struct type */
            public gtime_t time;       /* message time */
            public double ver;         /* rinex version */
            public char type;        /* rinex file type ('O','N',...) */
            public int sys;         /* navigation system */
            public int tsys;        /* time system */
            public fixed char tobs[7*MAXOBSTYPE*4]; /* rinex obs types */
            public obs_t obs;         /* observation data */
            public nav_t nav;         /* navigation data */
            public sta_t sta;         /* station info */
            public int ephsat;      /* ephemeris satellite number */
            public fixed char opt[256];    /* rinex dependent options */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct url_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]        /* download url type */
            public fixed char type[32];      /* data type */
            public fixed char path[1024];    /* url path */
            public fixed char dir[1024];    /* local directory */
            public double tint;        /* time interval (s) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct opt_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]        /* option type */
            public string name;   /* option name */
            public int format;         /* option format (0:int,1:double,2:string,3:enum) */
            public IntPtr var;          /* pointer to option variable */
            public char[] comment; /* option comment/enum labels/unit */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct exterr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]        /* extended receiver error model */
            public fixed int ena[4];         /* model enabled */
            public fixed double cerr[4*NFREQ*2]; /* code errors (m) */
            public fixed double perr[4*NFREQ*2]; /* carrier-phase errors (m) */
            public fixed double gpsglob[NFREQ]; /* gps-glonass h/w bias (m) */
            public fixed double gloicb[NFREQ]; /* glonass interchannel bias (m/fn) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct snrmask_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]        /* SNR mask type */
            public fixed int ena[2];         /* enable flag {rover,base} */
            public fixed double mask[NFREQ*9]; /* mask (dBHz) at 5,10,...85 deg */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct prcopt_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 54)]        /* processing options type */
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
           public fixed double eratio[NFREQ]; /* code/phase error ratio */
           public fixed double err[5];      /* measurement error factor */
                    /* [0]:reserved */
                    /* [1-3]:error factor a/b/c of phase (m) */
                    /* [4]:doppler frequency (hz) */
           public fixed double std[3];      /* initial-state std [0]bias,[1]iono [2]trop */
           public fixed double prn[6];      /* process-noise std [0]bias,[1]iono [2]trop [3]acch [4]accv [5] pos */
           public double sclkstab;    /* satellite clock stability (sec/sec) */
           public fixed double thresar[8];  /* AR validation threshold */
           public double elmaskar;    /* elevation mask of AR for rising satellite (deg) */
           public double elmaskhold;  /* elevation mask to hold ambiguity (deg) */
           public double thresslip;   /* slip threshold of geometry-free phase (m) */
           public double maxtdiff;    /* max difference of time (sec) */
           public double maxinno;     /* reject threshold of innovation (m) */
           public double maxgdop;     /* reject threshold of gdop */
           public fixed double baseline[2]; /* baseline length constraint {const,sigma} (m) */
           public fixed double ru[3];       /* rover position for fixed mode {x,y,z} (ecef) (m) */
           public fixed double rb[3];       /* base position for relative mode {x,y,z} (ecef) (m) */
           public fixed char anttype[2*MAXANT]; /* antenna types {rover,base} */
           public fixed double antdel[2*3]; /* antenna delta {{rov_e,rov_n,rov_u},{ref_e,ref_n,ref_u}} */
           public unsafe pcv_t* pcvr;      /* receiver antenna parameters {rov,base} */
           public fixed char exsats[MAXSAT]; /* excluded satellites (1:excluded,2:included) */
           public int maxaveep;      /* max averaging epoches */
           public int initrst;       /* initialize by restart */
           public int outsingle;     /* output single by dgps/float/fix/ppp outage */
           public fixed char rnxopt[2*256]; /* rinex options {rover,base} */
           public fixed int posopt[6];     /* positioning options */
           public int syncsol;       /* solution sync mode (0:off,1:on) */
           public fixed double odisp[2*6*11]; /* ocean tide loading parameters {rov,base} */
           public exterr_t exterr;    /* extended receiver error model */
           public int freqopt;        /* disable L2-AR */
           public fixed char pppopt[256];   /* ppp option */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct solopt_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18)]        /* solution options type */
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
            public fixed double nmeaintv[2]; /* nmea output interval (s) (<0:no,0:all) */
                /* nmeaintv[0]:gprmc,gpgga,nmeaintv[1]:gpgsv */
            public fixed char sep[64];       /* field separator */
            public fixed char prog[64];      /* program name */
            public double maxsolstd;   /* max std-dev for solution output (m) (0:all) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct filopt_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]        /* file options type */
             public fixed char satantp[MAXSTRPATH]; /* satellite antenna parameters file */
             public fixed char rcvantp[MAXSTRPATH]; /* receiver antenna parameters file */
             public fixed char stapos[MAXSTRPATH]; /* station positions file */
             public fixed char geoid[MAXSTRPATH]; /* external geoid data file */
             public fixed char iono[MAXSTRPATH]; /* ionosphere data file */
             public fixed char dcb[MAXSTRPATH]; /* dcb data file */
             public fixed char eop[MAXSTRPATH]; /* eop data file */
             public fixed char blq[MAXSTRPATH]; /* ocean tide loading blq file */
             public fixed char tempdir[MAXSTRPATH]; /* ftp/http temporaly directory */
             public fixed char geexe[MAXSTRPATH]; /* google earth exec file */
             public fixed char solstat[MAXSTRPATH]; /* solution statistics file */
             public fixed char trace[MAXSTRPATH]; /* debug trace file */
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
            public fixed char mask[7*64];   /* code mask {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
            public fixed char staid[32];    /* station id for rinex file name */
            public fixed char prog[32];    /* program */
            public fixed char runby[32];    /* run-by */
            public fixed char marker[64];    /* marker name */
            public fixed char markerno[32];  /* marker number */
            public fixed char markertype[32]; /* marker type (ver.3) */
            public fixed char name[2*32];   /* observer/agency */
            public fixed char rec[3*32];   /* receiver #/type/vers */
            public fixed char ant[3*32];   /* antenna #/type */
            public fixed double apppos[3];   /* approx position x/y/z */
            public fixed double antdel[3];   /* antenna delta h/e/n */
            public fixed char comment[MAXCOMMENT*4]; /* comments */
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
            public fixed char tobs[7*MAXOBSTYPE*4]; /* obs types {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
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
            public fixed int phase_lock [NFREQ];   /* lock counter of phase */
            public fixed int outc[NFREQ]; /* obs outage counter of phase */
            public fixed int slipc[NFREQ]; /* cycle-slip counter */
            public fixed int rejc[NFREQ]; /* reject counter */
            public double gf;         /* geometry-free phase L1-L2 (m) */
            public double gf2;        /* geometry-free phase L1-L5 (m) */
            public double mw;         /* MW-LC (m) */
            public double phw;        /* phase windup (cycle) */
            public unsafe gtime_t* pt; /* previous carrier-phase time */
            public fixed double ph[2*NFREQ]; /* previous carrier-phase observable (cycle) */
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
        public unsafe struct rtk_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]        /* RTK control/result type */
            public sol_t sol;         /* RTK solution */
            public fixed double rb[6];       /* base position/velocity (ecef) (m|m/s) */
            public int nx, na;          /* number of float states/fixed states */
            public double tt;          /* time difference between current and previous (s) */
            public double* x,  P;      /* float states and their covariance */
            public double* xa, Pa;     /* fixed states and their covariance */
            public int nfix;           /* number of continuous fixes of ambiguity */
            public unsafe ambc_t* ambc; /* ambibuity control */
            public unsafe ssat_t* ssat; /* satellite status */
            public int neb;            /* bytes in error message buffer */
            public fixed char errbuf[MAXERRMSG]; /* error message buffer */
            public prcopt_t opt;       /* processing options */
        };

           
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public struct half_cyc_t
#pragma warning restore IDE1006 // Naming Styles
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]   /* half-cycle correction list type */
            public byte sat;  /* satellite number */
            public byte freq; /* frequency number (0:L1,1:L2,2:L5) */
            public byte valid; /* half-cycle valid flag */
            public char corr;          /* half-cycle corrected (x 0.5 cyc) */
            public gtime_t ts, te;      /* time start, time end */
            public unsafe half_cyc_t* next; /* pointer to next correction */
        };

        
    
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct raw_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 28)]        /* receiver raw data control type */
            public gtime_t time;       /* message time */
            public unsafe gtime_t* tobs;//[MAXSAT*(NFREQ+NEXOBS)]; /* observation data time */
            public obs_t obs;          /* observation data */
            public obs_t obuf;         /* observation data buffer */
            public nav_t nav;          /* satellite ephemerides */
            public sta_t sta;          /* station parameters */
            public int ephsat;         /* sat number of update ephemeris (0:no satellite) */
            public sbsmsg_t sbsmsg;    /* SBAS message */
            public fixed char msgtype[256];  /* last message type */
            public fixed char subfrm[MAXSAT*380];  /* subframe buffer */
            public lexmsg_t lexmsg;    /* LEX message */
            public fixed double lockt[MAXSAT*(NFREQ+NEXOBS)]; /* lock time (s) */
            public fixed double icpp[MAXSAT], off[MAXSAT]; 
            public double icpc; /* carrier params for ss2 */
            public fixed double prCA[MAXSAT], dpCA[MAXSAT]; /* L1/CA pseudrange/doppler for javad */
            public fixed byte halfc[MAXSAT*(NFREQ+NEXOBS)]; /* half-cycle add flag */
            public fixed char freqn[MAXOBS]; /* frequency number for javad */
            public int nbyte;          /* number of bytes in message buffer */
            public int len;            /* message length (bytes) */
            public int iod;            /* issue of data */
            public int tod;            /* time of day (ms) */
            public int tbase;          /* time base (0:gpst,1:utc(usno),2:glonass,3:utc(su) */
            public int flag;           /* general purpose flag */
            public int outtype;        /* output message type */
            public fixed byte buff[MAXRAWLEN]; /* message buffer */
            public fixed char opt[256];      /* receiver dependent options */
            public half_cyc_t half_cyc; /* half-cycle correction list */

            public int format;         /* receiver stream format */
            public IntPtr rcv_data;     /* receiver dependent data */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct stream_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]        /* stream type */
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
            public fixed char path[MAXSTRPATH]; /* stream path */
            public fixed char msg[MAXSTRMSG];  /* stream message */
        };

        
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct strconv_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 11)]        /* stream converter type */
            public int itype, otype;    /* input and output stream type */
            public int nmsg;           /* number of output messages */
            public fixed int msgs[32];       /* output message types */
            public fixed double tint[32];    /* output message intervals (s) */
            public fixed int tick[32]; /* cycle tick of output message */
            public fixed int ephsat[32];     /* satellites of output ephemeris */
            public int stasel;         /* station info selection (0:remote,1:local) */
            public rtcm_t rtcm;        /* rtcm input data buffer */
            public raw_t raw;          /* raw  input data buffer */
            public rtcm_t _out;         /* rtcm output data buffer */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct strsvr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]        /* stream server type */
            public int state;          /* server state (0:stop,1:running) */
            public int cycle;          /* server cycle (ms) */
            public int buffsize;       /* input/monitor buffer size (bytes) */
            public int nmeacycle;      /* NMEA request cycle (ms) (0:no) */
            public int relayback;      /* relay back of output streams (0:no) */
            public int nstr;           /* number of streams (1 input + (nstr-1) outputs */
            public int npb;            /* data length in peek buffer (bytes) */
            public fixed char cmds_periodic[16*MAXRCVCMD]; /* periodic commands */
            public fixed double nmeapos[3];  /* NMEA request position (ecef) (m) */
            public byte* buff; /* input buffers */
            public byte* pbuf; /* peek buffer */
            public uint tick;  /* start tick */
            public unsafe stream_t* stream; /* input/output streams */
            public unsafe strconv_t* conv; /* stream converter */
            public IntPtr thread;    /* server thread */
            public CRITICAL_SECTION lock_flag;        /* lock flag */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct rtksvr_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 39)]        /* RTK server type */
            public int state;          /* server state (0:stop,1:running) */
            public int cycle;          /* processing cycle (ms) */
            public int nmeacycle;      /* NMEA request cycle (ms) (0:no req) */
            public int nmeareq;        /* NMEA request (0:no,1:nmeapos,2:single sol) */
            public fixed double nmeapos[3];  /* NMEA request position (ecef) (m) */
            public int buffsize;       /* input buffer size (bytes) */
            public fixed int format[3];      /* input format {rov,base,corr} */
            public unsafe solopt_t* solopt; /* output solution options {sol1,sol2} */
            public int navsel;         /* ephemeris select (0:all,1:rover,2:base,3:corr) */
            public int nsbs;           /* number of sbas message */
            public int nsol;           /* number of solution buffer */
            public rtk_t rtk;          /* RTK control/result struct */
            public fixed int nb[3];         /* bytes in input buffers {rov,base} */
            public fixed int nsb[2];         /* bytes in soulution buffers */
            public fixed int npb[3];         /* bytes in input peek buffers */
            public fixed byte buff[3]; /* input buffers {rov,base,corr} */
            public fixed byte sbuf[2]; /* output buffers {sol1,sol2} */
            public fixed byte pbuf[3]; /* peek buffers {rov,base,corr} */
            public unsafe sol_t* solbuf; /* solution buffer */
            public fixed int nmsg[3*10]; /* input message counts */
            public unsafe raw_t* raw;     /* receiver raw control {rov,base,corr} */
            public unsafe rtcm_t* rtcm;     /* RTCM control {rov,base,corr} */
            public unsafe gtime_t* ftime;   /* download time {rov,base,corr} */
            public fixed char files[3*MAXSTRPATH]; /* download paths {rov,base,corr} */
            public unsafe obs_t** obs; /* observation data {rov,base,corr} */
            public unsafe nav_t nav;          /* navigation data */
            public unsafe sbsmsg_t* sbsmsg; /* SBAS message buffer */
            public unsafe stream_t* stream; /* streams {rov,base,corr,sol1,sol2,logr,logb,logc} */
            public unsafe stream_t* moni;     /* monitor stream */
            public uint tick;  /* start tick */
            public IntPtr thread;    /* server thread */
            public int cputime;        /* CPU time (ms) for a processing cycle */
            public int prcout;         /* missing observation data count */
            public int nave;           /* number of averaging base pos */
            public fixed double rb_ave[3];   /* averaging base pos */
            public fixed char cmds_periodic[3*MAXRCVCMD]; /* periodic commands */
            public fixed char cmd_reset[MAXRCVCMD]; /* reset command */
            public double bl_reset;    /* baseline length to reset (km) */
            public CRITICAL_SECTION lock_flag;        /* lock flag */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct gis_pnt_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1)]        /* gis data point type */
            public fixed double pos[3];      /* point data {lat,lon,height} (rad,m) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct gis_poly_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* gis data polyline type */
            public int npnt;           /* number of points */
            public fixed double bound[4];    /* boundary {lat0,lat1,lon0,lon1} */
            public double* pos;        /* position data (3 x npnt) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct gis_polygon_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* gis data polygon type */
            public int npnt;           /* number of points */
            public fixed double bound[4];    /* boundary {lat0,lat1,lon0,lon1} */
            public double* pos;        /* position data (3 x npnt) */
        };



        #region ВОПРОС
#pragma warning disable IDE1006 // Naming Styles
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] public struct gisd_t
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
        public unsafe struct gis_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]        /* gis type */
            public fixed char name[MAXGISLAYER*256]; /* name */
            public fixed int flag[MAXGISLAYER];     /* flag */
            public unsafe gisd_t* data; /* gis data list */
            public fixed double bound[4];    /* boundary {lat0,lat1,lon0,lon1} */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct imud_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]        /* imu data type */
            public gtime_t time;       /* time */
            public int stat;           /* status */
            public int seqno;          /* sequence number */
            public float temp;         /* temperature (C) */
            public fixed double rot[3];      /* rotation rate {x,y,z} (rad/s) */
            public fixed double acc[3];      /* acceleration data {x,y,z} (m/s^2) */
        };

                
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)] 
#pragma warning disable IDE1006 // Naming Styles
        public unsafe struct imu_t
#pragma warning restore IDE1006 // Naming Styles
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]        /* imu type */
            public imud_t data;        /* imu data */
            public int nbyte;          /* bytes in imu data buffer */
            public fixed byte buff[256]; /* imu data buffer */
        };


    }
}
