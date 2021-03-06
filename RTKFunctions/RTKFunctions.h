#pragma comment(lib, "RTKLib.lib")
extern "C" {
    #include "defs.h"
    //#include "../RTKLib/rtklib.h"
}



using namespace System;
using namespace System::Collections::Generic;
using namespace DataExchange;

/* constants -----------------------------------------------------------------*/

#define PRUNIT_GPS  299792.458  /* rtcm ver.3 unit of gps pseudorange (m) */
#define PRUNIT_GLO  599584.916  /* rtcm ver.3 unit of glonass pseudorange (m) */
#define RANGE_MS    (CLIGHT*0.001)      /* range in 1 ms */

#define P2_10       0.0009765625          /* 2^-10 */
#define P2_34       5.820766091346740E-11 /* 2^-34 */
#define P2_46       1.421085471520200E-14 /* 2^-46 */
#define P2_59       1.734723475976810E-18 /* 2^-59 */
#define P2_66       1.355252715606880E-20 /* 2^-66 */

#define RTCM2PREAMB 0x66        /* rtcm ver.2 frame preamble */
#define RTCM3PREAMB 0xD3        /* rtcm ver.3 frame preamble */

/* type definition -----------------------------------------------------------*/

typedef struct {                    /* multi-signal-message header type */
    unsigned char iod;              /* issue of data station */
    unsigned char time_s;           /* cumulative session transmitting time */
    unsigned char clk_str;          /* clock steering indicator */
    unsigned char clk_ext;          /* external clock indicator */
    unsigned char smooth;           /* divergence free smoothing indicator */
    unsigned char tint_s;           /* soothing interval */
    unsigned char nsat, nsig;        /* number of satellites/signals */
    unsigned char sats[64];         /* satellites */
    unsigned char sigs[32];         /* signals */
    unsigned char cellmask[64];     /* cell mask */
} msm_h_t;

//static const double gpst0[] = { 1980,1, 6,0,0,0 }; /* gps time reference */
//static const double gst0[] = { 1999,8,22,0,0,0 }; /* galileo system time reference */
//static const double bdt0[] = { 2006,1, 1,0,0,0 }; /* beidou time reference */


//static double leaps[MAXLEAPS + 1][7] = { /* leap seconds (y,m,d,h,m,s,utc-gpst) */
//    {2017,1,1,0,0,0,-18},
//    {2015,7,1,0,0,0,-17},
//    {2012,7,1,0,0,0,-16},
//    {2009,1,1,0,0,0,-15},
//    {2006,1,1,0,0,0,-14},
//    {1999,1,1,0,0,0,-13},
//    {1997,7,1,0,0,0,-12},
//    {1996,1,1,0,0,0,-11},
//    {1994,7,1,0,0,0,-10},
//    {1993,7,1,0,0,0, -9},
//    {1992,7,1,0,0,0, -8},
//    {1991,1,1,0,0,0, -7},
//    {1990,1,1,0,0,0, -6},
//    {1988,1,1,0,0,0, -5},
//    {1985,7,1,0,0,0, -4},
//    {1983,7,1,0,0,0, -3},
//    {1982,7,1,0,0,0, -2},
//    {1981,7,1,0,0,0, -1},
//    {0}
//};

//static char codepris[7][MAXFREQ][16] = {  /* code priority table */
//
//    L1/E1      L2/B1        L5/E5a/L3 L6/LEX/B3 E5b/B2    E5(a+b)  S */
//    {"CPYWMNSL","PYWCMNDSLX","IQX"     ,""       ,""       ,""      ,""    }, /* GPS */
//    {"PC"      ,"PC"        ,"IQX"     ,""       ,""       ,""      ,""    }, /* GLO */
//    {"CABXZ"   ,""          ,"IQX"     ,"ABCXZ"  ,"IQX"    ,"IQX"   ,""    }, /* GAL */
//    {"CSLXZ"   ,"SLX"       ,"IQX"     ,"SLX"    ,""       ,""      ,""    }, /* QZS */
//    {"C"       ,""          ,"IQX"     ,""       ,""       ,""      ,""    }, /* SBS */
//    {"IQX"     ,"IQX"       ,"IQX"     ,"IQX"    ,"IQX"    ,""      ,""    }, /* BDS */
//    {""        ,""          ,"ABCX"    ,""       ,""       ,""      ,"ABCX"}  /* IRN */
//};

//const prcopt_t prcopt_default = ��{ /* defaults processing options */
//    PMODE_SINGLE,0,2,SYS_GPS,   /* mode,soltype,nf,navsys */
//    15.0 * D2R,{{0,0}},           /* elmin,snrmask */
//    0,1,1,1,                    /* sateph,modear,glomodear,bdsmodear */
//    5,0,10,1,                   /* maxout,minlock,minfix,armaxiter */
//    0,0,0,0,                    /* estion,esttrop,dynamics,tidecorr */
//    1,0,0,0,0,                  /* niter,codesmooth,intpref,sbascorr,sbassatsel */
//    0,0,                        /* rovpos,refpos */
//    {100.0,100.0},              /* eratio[] */
//    {100.0,0.003,0.003,0.0,1.0}, /* err[] */
//    {30.0,0.03,0.3},            /* std[] */
//    {1E-4,1E-3,1E-4,1E-1,1E-2,0.0}, /* prn[] */
//    5E-12,                      /* sclkstab */
//    {3.0,0.9999,0.25,0.1,0.05}, /* thresar */
//    0.0,0.0,0.05,               /* elmaskar,almaskhold,thresslip */
//    30.0,30.0,30.0,             /* maxtdif,maxinno,maxgdop */
//    {0},{0},{0},                /* baseline,ru,rb */
//    {"",""},                    /* anttype */
//    {{0}},{{0}},{0}             /* antdel,pcv,exsats */
//};
//const solopt_t solopt_default = { /* defaults solution output options */
//    SOLF_LLH,TIMES_GPST,1,3,    /* posf,times,timef,timeu */
//    0,1,0,0,0,0,0,              /* degf,outhead,outopt,outvel,datum,height,geoid */
//    0,0,0,                      /* solstatic,sstat,trace */
//    {0.0,0.0},                  /* nmeaintv */
//    " ",""                      /* separator/program name */
//};
/* msm signal id table -------------------------------------------------------*/
const char* msm_sig_gps[32] = {
    /* GPS: ref [13] table 3.5-87, ref [14][15] table 3.5-91 */
    ""  ,"1C","1P","1W","1Y","1M",""  ,"2C","2P","2W","2Y","2M", /*  1-12 */
    ""  ,""  ,"2S","2L","2X",""  ,""  ,""  ,""  ,"5I","5Q","5X", /* 13-24 */
    ""  ,""  ,""  ,""  ,""  ,"1S","1L","1X"                      /* 25-32 */
};
const char* msm_sig_glo[32] = {
    /* GLONASS: ref [13] table 3.5-93, ref [14][15] table 3.5-97 */
    ""  ,"1C","1P",""  ,""  ,""  ,""  ,"2C","2P",""  ,"3I","3Q",
    "3X",""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,
    ""  ,""  ,""  ,""  ,""  ,""  ,""  ,""
};
const char* msm_sig_gal[32] = {
    /* Galileo: ref [15] table 3.5-100 */
    ""  ,"1C","1A","1B","1X","1Z",""  ,"6C","6A","6B","6X","6Z",
    ""  ,"7I","7Q","7X",""  ,"8I","8Q","8X",""  ,"5I","5Q","5X",
    ""  ,""  ,""  ,""  ,""  ,""  ,""  ,""
};
const char* msm_sig_qzs[32] = {
    /* QZSS: ref [15] table 3.5-103 */
    ""  ,"1C",""  ,""  ,""  ,""  ,""  ,""  ,"6S","6L","6X",""  ,
    ""  ,""  ,"2S","2L","2X",""  ,""  ,""  ,""  ,"5I","5Q","5X",
    ""  ,""  ,""  ,""  ,""  ,"1S","1L","1X"
};
const char* msm_sig_sbs[32] = {
    /* SBAS: ref [13] table 3.5-T+005 */
    ""  ,"1C",""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,
    ""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,"5I","5Q","5X",
    ""  ,""  ,""  ,""  ,""  ,""  ,""  ,""
};
const char* msm_sig_cmp[32] = {
    /* BeiDou: ref [15] table 3.5-106 */
    ""  ,"1I","1Q","1X",""  ,""  ,""  ,"6I","6Q","6X",""  ,""  ,
    ""  ,"7I","7Q","7X",""  ,""  ,""  ,""  ,""  ,""  ,""  ,""  ,
    ""  ,""  ,""  ,""  ,""  ,""  ,""  ,""
};

/* ssr update intervals ------------------------------------------------------*/
static const double ssrudint[16] = {
    1,2,5,10,15,30,60,120,240,300,600,900,1800,3600,7200,10800
};

/* ssr signal and tracking mode ids ------------------------------------------*/
static const int codes_gps[] = {
    CODE_L1C,CODE_L1P,CODE_L1W,CODE_L1Y,CODE_L1M,CODE_L2C,CODE_L2D,CODE_L2S,
    CODE_L2L,CODE_L2X,CODE_L2P,CODE_L2W,CODE_L2Y,CODE_L2M,CODE_L5I,CODE_L5Q,
    CODE_L5X
};
static const int codes_glo[] = {
    CODE_L1C,CODE_L1P,CODE_L2C,CODE_L2P
};
static const int codes_gal[] = {
    CODE_L1A,CODE_L1B,CODE_L1C,CODE_L1X,CODE_L1Z,CODE_L5I,CODE_L5Q,CODE_L5X,
    CODE_L7I,CODE_L7Q,CODE_L7X,CODE_L8I,CODE_L8Q,CODE_L8X,CODE_L6A,CODE_L6B,
    CODE_L6C,CODE_L6X,CODE_L6Z
};
static const int codes_qzs[] = {
    CODE_L1C,CODE_L1S,CODE_L1L,CODE_L2S,CODE_L2L,CODE_L2X,CODE_L5I,CODE_L5Q,
    CODE_L5X,CODE_L6S,CODE_L6L,CODE_L6X,CODE_L1X
};
static const int codes_bds[] = {
    CODE_L1I,CODE_L1Q,CODE_L1X,CODE_L7I,CODE_L7Q,CODE_L7X,CODE_L6I,CODE_L6Q,
    CODE_L6X
};
static const int codes_sbs[] = {
    CODE_L1C,CODE_L5I,CODE_L5Q,CODE_L5X
};

namespace RTKFunctions {
    public  ref class CommonRTK {
    public:

        /* inner product ---------------------------------------------------------------
        * inner product of vectors
        * args   : double *a,*b     I   vector a,b (n x 1)
        *          int    n         I   size of vector a,b
        * return : a'*b
        *-----------------------------------------------------------------------------*/
        //double dot(const double* a, const double* b, int n);

        /* euclid norm -----------------------------------------------------------------
        * euclid norm of vector
        * args   : double *a        I   vector a (n x 1)
        *          int    n         I   size of vector a
        * return : || a ||
        *-----------------------------------------------------------------------------*/
        static double norm(const double* a, int n);


        /* satellite system+prn/slot number to satellite number ------------------------
        * convert satellite system+prn/slot number to satellite number
        * args   : int    sys       I   satellite system (SYS_GPS,SYS_GLO,...)
        *          int    prn       I   satellite prn/slot number
        * return : satellite number (0:error)
        *-----------------------------------------------------------------------------*/
        static int CommonRTK::satno(int sys, int prn);

        /* obs code to obs code string -------------------------------------------------
        * convert obs code to obs code string
        * args   : unsigned char code I obs code (CODE_???)
        *          int    *freq  IO     frequency (NULL: no output)
        *                               (1:L1/E1, 2:L2/B1, 3:L5/E5a/L3, 4:L6/LEX/B3,
                                         5:E5b/B2, 6:E5(a+b), 7:S)
        * return : obs code string ("1C","1P","1P",...)
        * notes  : obs codes are based on reference [6] and qzss extension
        *-----------------------------------------------------------------------------*/
        static char* code2obs(unsigned char code, int* freq);

        /* obs type string to obs code -------------------------------------------------
        * convert obs code type string to obs code
        * args   : char   *str   I      obs code string ("1C","1P","1Y",...)
        *          int    *freq  IO     frequency (1:L1,2:L2,3:L5,4:L6,5:L7,6:L8,0:err)
        *                               (NULL: no output)
        * return : obs code (CODE_???)
        * notes  : obs codes are based on reference [6] and qzss extension
        *-----------------------------------------------------------------------------*/
        static unsigned char obs2code(const char* obs, int* freq);

        /* get code priority -----------------------------------------------------------
        * get code priority for multiple codes in a frequency
        * args   : int    sys     I     system (SYS_???)
        *          unsigned char code I obs code (CODE_???)
        *          char   *opt    I     code options (NULL:no option)
        * return : priority (15:highest-1:lowest,0:error)
        *-----------------------------------------------------------------------------*/
        static int getcodepri(int sys, unsigned char code, const char* opt);

        /* satellite number to satellite system ----------------------------------------
        * convert satellite number to satellite system
        * args   : int    sat       I   satellite number (1-MAXSAT)
        *          int    *prn      IO  satellite prn/slot number (NULL: no output)
        * return : satellite system (SYS_GPS,SYS_GLO,...)
        *-----------------------------------------------------------------------------*/
        static int satsys(int sat, int* prn);

        /* satellite carrier wave length -----------------------------------------------
        * get satellite carrier wave lengths
        * args   : int    sat       I   satellite number
        *          int    frq       I   frequency index (0:L1,1:L2,2:L5/3,...)
        *          nav_t  *nav      I   navigation messages
        * return : carrier wave length (m) (0.0: error)
        *-----------------------------------------------------------------------------*/
        static double satwavelen(int sat, int frq, const nav_t* nav);

        /* add time --------------------------------------------------------------------
        * add time to gtime_t struct
        * args   : gtime_t t        I   gtime_t struct
        *          double sec       I   time to add (s)
        * return : gtime_t struct (t+sec)
        *-----------------------------------------------------------------------------*/
        static gtime_t timeadd(gtime_t t, double sec);

        /* time difference -------------------------------------------------------------
        * difference between gtime_t structs
        * args   : gtime_t t1,t2    I   gtime_t structs
        * return : time difference (t1-t2) (s)
        *-----------------------------------------------------------------------------*/
        static double timediff(gtime_t t1, gtime_t t2);

        /* get current time in utc -----------------------------------------------------
        * get current time in utc
        * args   : none
        * return : current time in utc
        *-----------------------------------------------------------------------------*/
        static gtime_t timeget(void);

        /* convert calendar day/time to time -------------------------------------------
        * convert calendar day/time to gtime_t struct
        * args   : double *ep       I   day/time {year,month,day,hour,min,sec}
        * return : gtime_t struct
        * notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
        *-----------------------------------------------------------------------------*/
        static gtime_t epoch2time(const double* ep);

        /* gpstime to utc --------------------------------------------------------------
        * convert gpstime to utc considering leap seconds
        * args   : gtime_t t        I   time expressed in gpstime
        * return : time expressed in utc
        * notes  : ignore slight time offset under 100 ns
        *-----------------------------------------------------------------------------*/
        static gtime_t  gpst2utc(gtime_t t);

        /* gpstime to bdt --------------------------------------------------------------
        * convert gpstime to bdt (beidou navigation satellite system time)
        * args   : gtime_t t        I   time expressed in gpstime
        * return : time expressed in bdt
        * notes  : ref [8] 3.3, 2006/1/1 00:00 BDT = 2006/1/1 00:00 UTC
        *          no leap seconds in BDT
        *          ignore slight time offset under 100 ns
        *-----------------------------------------------------------------------------*/
        static gtime_t gpst2bdt(gtime_t t);

        /* time to beidouo time (bdt) --------------------------------------------------
        * convert gtime_t struct to week and tow in beidou time (bdt)
        * args   : gtime_t t        I   gtime_t struct
        *          int    *week     IO  week number in bdt (NULL: no output)
        * return : time of week in bdt (s)
        *-----------------------------------------------------------------------------*/
        static double time2bdt(gtime_t t, int* week);

        /* utc to gpstime --------------------------------------------------------------
        * convert utc to gpstime considering leap seconds
        * args   : gtime_t t        I   time expressed in utc
        * return : time expressed in gpstime
        * notes  : ignore slight time offset under 100 ns
        *-----------------------------------------------------------------------------*/
        static gtime_t utc2gpst(gtime_t t);

        /* time to gps time ------------------------------------------------------------
        * convert gtime_t struct to week and tow in gps time
        * args   : gtime_t t        I   gtime_t struct
        *          int    *week     IO  week number in gps time (NULL: no output)
        * return : time of week in gps time (s)
        *-----------------------------------------------------------------------------*/
        static double time2gpst(gtime_t t, int* week);

        /* gps time to time ------------------------------------------------------------
        * convert week and tow in gps time to gtime_t struct
        * args   : int    week      I   week number in gps time
        *          double sec       I   time of week in gps time (s)
        * return : gtime_t struct
        *-----------------------------------------------------------------------------*/
        static gtime_t  gpst2time(int week, double sec);

        /* time to string --------------------------------------------------------------
        * convert gtime_t struct to string
        * args   : gtime_t t        I   gtime_t struct
        *          char   *s        O   string ("yyyy/mm/dd hh:mm:ss.ssss")
        *          int    n         I   number of decimals
        * return : none
        *-----------------------------------------------------------------------------*/
        static void time2str(gtime_t t, char* s, int n);

        /* time to calendar day/time ---------------------------------------------------
        * convert gtime_t struct to calendar day/time
        * args   : gtime_t t        I   gtime_t struct
        *          double *ep       O   day/time {year,month,day,hour,min,sec}
        * return : none
        * notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
        *-----------------------------------------------------------------------------*/
        static void time2epoch(gtime_t t, double* ep);


        /* adjust gps week number ------------------------------------------------------
        * adjust gps week number using cpu time
        * args   : int   week       I   not-adjusted gps week number
        * return : adjusted gps week number
        *-----------------------------------------------------------------------------*/
        static int adjgpsweek(int week);

        /* bdt to gpstime --------------------------------------------------------------
        * convert bdt (beidou navigation satellite system time) to gpstime
        * args   : gtime_t t        I   time expressed in bdt
        * return : time expressed in gpstime
        * notes  : see gpst2bdt()
        *-----------------------------------------------------------------------------*/
        static gtime_t bdt2gpst(gtime_t t);

        /* beidou time (bdt) to time ---------------------------------------------------
        * convert week and tow in beidou time (bdt) to gtime_t struct
        * args   : int    week      I   week number in bdt
        *          double sec       I   time of week in bdt (s)
        * return : gtime_t struct
        *-----------------------------------------------------------------------------*/
        static gtime_t bdt2time(int week, double sec);

        /* extract unsigned/signed bits ------------------------------------------------
        * extract unsigned/signed bits from byte data
        * args   : unsigned char *buff I byte data
        *          int    pos    I      bit position from start of data (bits)
        *          int    len    I      bit length (bits) (len<=32)
        * return : extracted unsigned/signed bits
        *-----------------------------------------------------------------------------*/
        static int getbitu(array<unsigned char>^ buff, int pos, int len);  // const unsigned char* buff, int pos, int len);
        //static int getbitu(unsigned char* buff, int pos, int len);
        static int getbits(array<unsigned char>^ buff, int pos, int len); //const unsigned char* buff, int pos, int len);

        /* decode navigation data word -------------------------------------------------
       * check party and decode navigation data word
       * args   : unsigned int word I navigation data word (2+30bit)
       *                              (previous word D29*-30* + current word D1-30)
       *          unsigned char *data O decoded navigation data without parity
       *                              (8bitx3)
       * return : status (1:ok,0:parity error)
       * notes  : see reference [1] 20.3.5.2 user parity algorithm
       *-----------------------------------------------------------------------------*/
        static int decode_word(unsigned int word, unsigned char* data);

        /* inner product ---------------------------------------------------------------
        * inner product of vectors
        * args   : double *a,*b     I   vector a,b (n x 1)
        *          int    n         I   size of vector a,b
        * return : a'*b
        *-----------------------------------------------------------------------------*/
        static double dot(const double* a, const double* b, int n);

        /* transform ecef to geodetic postion ------------------------------------------
        * transform ecef position to geodetic position
        * args   : double *r        I   ecef position {x,y,z} (m)
        *          double *pos      O   geodetic position {lat,lon,h} (rad,m)
        * return : none
        * notes  : WGS84, ellipsoidal height
        *-----------------------------------------------------------------------------*/
        static void ecef2pos(const double* r, double* pos);

        /* crc-24q parity --------------------------------------------------------------
        * compute crc-24q parity for sbas, rtcm3
        * args   : unsigned char *buff I data
        *          int    len    I      data length (bytes)
        * return : crc-24Q parity
        * notes  : see reference [2] A.4.3.3 Parity
        *-----------------------------------------------------------------------------*/
        static unsigned int rtk_crc24q(int len); //const unsigned char* buff, int len);
        
        
    };

    public ref class DecodeRTCM
    {

    public:
        

        /* decode receiver raw/rtcm data ---------------------------------------------*/
        int decoderaw();//unsigned char svr_bytes[], int index);
        void dataexchange_double();


    private:
        /* input rtcm 2 message from stream --------------------------------------------
        * fetch next rtcm 2 message and input a message from byte stream
        * args   : rtcm_t *rtcm IO   rtcm control struct
        *          unsigned char data I stream data (1 byte)
        * return : status (-1: error message, 0: no message, 1: input observation data,
        *                  2: input ephemeris, 5: input station pos/ant parameters,
        *                  6: input time parameter, 7: input dgps corrections,
        *                  9: input special message)
        * notes  : before firstly calling the function, time in rtcm control struct has
        *          to be set to the approximate time within 1/2 hour in order to resolve
        *          ambiguity of time in rtcm messages.
        *          supported msgs RTCM ver.2: 1,3,9,14,16,17,18,19,22
        *          refer [1] for RTCM ver.2
        *-----------------------------------------------------------------------------*/
        int input_rtcm2(rtcm_t* rtcm, unsigned char data);

        /* input receiver raw data from stream -----------------------------------------
        * fetch next receiver raw data and input a message from stream
        * args   : raw_t  *raw   IO     receiver raw data control struct
        *          int    format I      receiver raw data format (STRFMT_???)
        *          unsigned char data I stream data (1 byte)
        * return : status (-1: error message, 0: no message, 1: input observation data,
        *                  2: input ephemeris, 3: input sbas message,
        *                  9: input ion/utc parameter, 31: input lex message)
        *-----------------------------------------------------------------------------*/
        //int input_raw(raw_t* raw, int format, unsigned char data)
        //{
        //    trace(5, "input_raw: format=%d data=0x%02x\n", format, data);

        //    switch (format) {
        //    case STRFMT_OEM4: return input_oem4(raw, data);
        //    case STRFMT_OEM3: return input_oem3(raw, data);
        //    case STRFMT_UBX: return input_ubx(raw, data);
        //    case STRFMT_SS2: return input_ss2(raw, data);
        //    case STRFMT_CRES: return input_cres(raw, data);
        //    case STRFMT_STQ: return input_stq(raw, data);
        //    case STRFMT_GW10: return input_gw10(raw, data);
        //    case STRFMT_JAVAD: return input_javad(raw, data);
        //    case STRFMT_NVS: return input_nvs(raw, data);
        //    case STRFMT_BINEX: return input_bnx(raw, data);
        //    case STRFMT_RT17: return input_rt17(raw, data);
        //    case STRFMT_SEPT: return input_sbf(raw, data);
        //    case STRFMT_CMR: return input_cmr(raw, data);
        //        //case STRFMT_TERSUS: return input_tersus(raw,data);
        //    case STRFMT_LEXR: return input_lexr(raw, data);
        //    }
        //    return 0;
        //}

        /* input rtcm 3 message from stream --------------------------------------------
        * fetch next rtcm 3 message and input a message from byte stream
        * args   : rtcm_t *rtcm IO   rtcm control struct
        *          unsigned char data I stream data (1 byte)
        * return : status (-1: error message, 0: no message, 1: input observation data,
        *                  2: input ephemeris, 5: input station pos/ant parameters,
        *                  10: input ssr messages)
        * notes  : before firstly calling the function, time in rtcm control struct has
        *          to be set to the approximate time within 1/2 week in order to resolve
        *          ambiguity of time in rtcm messages.
        *
        *          to specify input options, set rtcm->opt to the following option
        *          strings separated by spaces.
        *
        *          -EPHALL  : input all ephemerides
        *          -STA=nnn : input only message with STAID=nnn
        *          -GLss    : select signal ss for GPS MSM (ss=1C,1P,...)
        *          -RLss    : select signal ss for GLO MSM (ss=1C,1P,...)
        *          -ELss    : select signal ss for GAL MSM (ss=1C,1B,...)
        *          -JLss    : select signal ss for QZS MSM (ss=1C,2C,...)
        *          -CLss    : select signal ss for BDS MSM (ss=2I,7I,...)
        *          -GALINAV : input only I/NAV for galileo ephemeris
        *          -GALFNAV : input only F/NAV for galileo ephemeris
        *
        *          supported RTCM 3 messages
        *             (ref [2][3][4][5][6][7][8][9][10][11][12][13][14][15][16][17])
        *
        *            TYPE       GPS     GLOASS    GALILEO    QZSS     BEIDOU     SBAS
        *         ----------------------------------------------------------------------
        *          OBS C-L1  : 1001~     1009~       -         -         -         -
        *              F-L1  : 1002      1010        -         -         -         -
        *              C-L12 : 1003~     1011~       -         -         -         -
        *              F-L12 : 1004      1012        -         -         -         -
        *
        *          NAV       : 1019      1020      1045      1044      1042        -
        *                        -         -       1046        -         63*       -
        *
        *          MSM 1     : 1071~     1081~     1091~     1111~     1121~     1101~
        *              2     : 1072~     1082~     1092~     1112~     1122~     1102~
        *              3     : 1073~     1083~     1093~     1113~     1123~     1103~
        *              4     : 1074      1084      1094      1114      1124      1104
        *              5     : 1075      1085      1095      1115      1125      1105
        *              6     : 1076      1086      1096      1116      1126      1106
        *              7     : 1077      1087      1097      1117      1127      1107
        *
        *          SSR OBT   : 1057      1063      1240*     1246*     1258*       -
        *              CLK   : 1058      1064      1241*     1247*     1259*       -
        *              BIAS  : 1059      1065      1242*     1248*     1260*       -
        *              OBTCLK: 1060      1066      1243*     1249*     1261*       -
        *              URA   : 1061      1067      1244*     1250*     1262*       -
        *              HRCLK : 1062      1068      1245*     1251*     1263*       -
        *
        *          ANT INFO  : 1005 1006 1007 1008 1033
        *         ----------------------------------------------------------------------
        *                                                    (* draft, ~ only encode)
        *
        *          for MSM observation data with multiple signals for a frequency,
        *          a signal is selected according to internal priority. to select
        *          a specified signal, use the input options.
        *
        *          rtcm3 message format:
        *            +----------+--------+-----------+--------------------+----------+
        *            | preamble | 000000 |  length   |    data message    |  parity  |
        *            +----------+--------+-----------+--------------------+----------+
        *            |<-- 8 --->|<- 6 -->|<-- 10 --->|<--- length x 8 --->|<-- 24 -->|
        *
        *-----------------------------------------------------------------------------*/
        int input_rtcm3(unsigned char data);//rtcm_t* rtcm, unsigned char data);



        /* get sign-magnitude bits ---------------------------------------------------*/
        double getbitg(array<unsigned char>^ buff, int pos, int len); //const unsigned char* buff, int pos, int len);

        /* adjust weekly rollover of gps time ----------------------------------------*/
        static void adjweek(rtcm_t* rtcm, double tow);

        /* adjust weekly rollover of bdt time ----------------------------------------*/
        static int adjbdtweek(int week);

        /* adjust daily rollover of glonass time -------------------------------------*/
        static void adjday_glot(rtcm_t* rtcm, double tod);

        /* adjust carrier-phase rollover ---------------------------------------------*/
        double adjcp(rtcm_t* rtcm, int sat, int freq, double cp);

        /* loss-of-lock indicator ----------------------------------------------------*/
        int lossoflock(rtcm_t* rtcm, int sat, int freq, int lock);

        /* s/n ratio -----------------------------------------------------------------*/
        unsigned char snratio(double snr);

        /* get observation data index ------------------------------------------------*/
        int obsindex(int sat);//obs_t* obs, gtime_t time, int sat);

        /* test station id consistency -----------------------------------------------*/
        static int test_staid(int staid);//rtcm_t* rtcm, int staid);

        /* decode type 1001-1004 message header --------------------------------------*/
        int decode_head1001(rtcm_t* rtcm, int* sync);

        /* decode type 1001: L1-only gps rtk observation -----------------------------*/
        int decode_type1001(rtcm_t* rtcm);

        /* decode type 1002: extended L1-only gps rtk observables --------------------*/
        int decode_type1002(rtcm_t* rtcm);

        /* decode type 1003: L1&L2 gps rtk observables -------------------------------*/
        int decode_type1003(rtcm_t* rtcm);

        /* decode type 1004: extended L1&L2 gps rtk observables ----------------------*/
        int decode_type1004(rtcm_t* rtcm);


        /* get signed 38bit field ----------------------------------------------------*/
        //double getbits_38(unsigned char* buff, int pos);

        /* get signed 38bit field ----------------------------------------------------*/
        double getbits_38(array<unsigned char>^ buff, int pos);//const unsigned char* buff, int pos);

        

        /* decode type 1005: stationary rtk reference station arp --------------------*/
        int decode_type1005();// rtcm_t* rtcm);

        /* decode type 1006: stationary rtk reference station arp with height --------*/
        int decode_type1006();// rtcm_t* rtcm);

        /* decode type 1007: antenna descriptor --------------------------------------*/
        int decode_type1007(rtcm_t* rtcm);

        /* decode type 1008: antenna descriptor & serial number ----------------------*/
        int decode_type1008(rtcm_t* rtcm);

        /* decode type 1009-1012 message header --------------------------------------*/
        int decode_head1009(rtcm_t* rtcm, int* sync);

        /* decode type 1009: L1-only glonass rtk observables -------------------------*/
        int decode_type1009(rtcm_t* rtcm);

        /* decode type 1010: extended L1-only glonass rtk observables ----------------*/
        int decode_type1010(rtcm_t* rtcm);

        /* decode type 1011: L1&L2 glonass rtk observables ---------------------------*/
        int decode_type1011(rtcm_t* rtcm);

        /* decode type 1012: extended L1&L2 glonass rtk observables ------------------*/
        int decode_type1012(rtcm_t* rtcm);

        /* decode type 1013: system parameters ---------------------------------------*/
        int decode_type1013(rtcm_t* rtcm);

        /* decode type 1019: gps ephemerides -----------------------------------------*/
        int decode_type1019(rtcm_t* rtcm);

        /* decode type 1020: glonass ephemerides -------------------------------------*/
        int decode_type1020(rtcm_t* rtcm);

        /* decode type 1021: helmert/abridged molodenski -----------------------------*/
        int decode_type1021(rtcm_t* rtcm);

        /* decode type 1022: moledenski-badekas transfromation -----------------------*/
        int decode_type1022(rtcm_t* rtcm);

        /* decode type 1023: residual, ellipoidal grid representation ----------------*/
        int decode_type1023(rtcm_t* rtcm);

        /* decode type 1024: residual, plane grid representation ---------------------*/
        int decode_type1024(rtcm_t* rtcm);

        /* decode type 1025: projection (types except LCC2SP,OM) ---------------------*/
        int decode_type1025(rtcm_t* rtcm);

        /* decode type 1026: projection (LCC2SP - lambert conic conformal (2sp)) -----*/
        int decode_type1026(rtcm_t* rtcm);

        /* decode type 1027: projection (type OM - oblique mercator) -----------------*/
        int decode_type1027(rtcm_t* rtcm);

        /* decode type 1029: unicode text string -------------------------------------*/
        int decode_type1029(rtcm_t* rtcm);

        /* decode type 1030: network rtk residual ------------------------------------*/
        int decode_type1030(rtcm_t* rtcm);

        /* decode type 1031: glonass network rtk residual ----------------------------*/
        int decode_type1031(rtcm_t* rtcm);

        /* decode type 1032: physical reference station position information ---------*/
        int decode_type1032(rtcm_t* rtcm);

        /* decode type 1033: receiver and antenna descriptor -------------------------*/
        int decode_type1033(rtcm_t* rtcm);

        /* decode type 1034: gps network fkp gradient --------------------------------*/
        int decode_type1034(rtcm_t* rtcm);

        /* decode type 1035: glonass network fkp gradient ----------------------------*/
        int decode_type1035(rtcm_t* rtcm);

        /* decode type 1037: glonass network rtk ionospheric correction difference ---*/
        int decode_type1037(rtcm_t* rtcm);

        /* decode type 1038: glonass network rtk geometic correction difference ------*/
        int decode_type1038(rtcm_t* rtcm);

        /* decode type 1039: glonass network rtk combined correction difference ------*/
        int decode_type1039(rtcm_t* rtcm);

        /* decode type 1044: qzss ephemerides (ref [15]) -----------------------------*/
        int decode_type1044(rtcm_t* rtcm);

        /* decode type 1045: galileo F/NAV satellite ephemerides (ref [15]) ----------*/
        int decode_type1045(rtcm_t* rtcm);

        /* decode type 1046: galileo I/NAV satellite ephemerides (ref [17]) ----------*/
        int decode_type1046(rtcm_t* rtcm);

        /* decode type 1042/63: beidou ephemerides -----------------------------------*/
        int decode_type1042(rtcm_t* rtcm);

        /* decode ssr 1,4 message header ---------------------------------------------*/
        int decode_ssr1_head(int sys, int* sync, int* iod, double* udint, int* refd, int* hsize);//rtcm_t* rtcm, int sys, int* sync, int* iod, double* udint, int* refd, int* hsize);

        /* decode ssr 2,3,5,6 message header -----------------------------------------*/
        int decode_ssr2_head(int sys, int* sync, int* iod, double* udint, int* hsize);//rtcm_t* rtcm, int sys, int* sync, int* iod, double* udint, int* hsize);

        /* decode ssr 1: orbit corrections -------------------------------------------*/
        int decode_ssr1(int sys);//rtcm_t* rtcm, int sys);

        /* decode ssr 2: clock corrections -------------------------------------------*/
        int decode_ssr2(int sys);

        /* decode ssr 3: satellite code biases ---------------------------------------*/
        int decode_ssr3(int sys);

        /* decode ssr 4: combined orbit and clock corrections ------------------------*/
        int decode_ssr4(int sys);

        /* decode ssr 5: ura ---------------------------------------------------------*/
        int decode_ssr5(int sys);

        /* decode ssr 6: high rate clock correction ----------------------------------*/
        int decode_ssr6(int sys);

        /* decode ssr 7 message header -----------------------------------------------*/
        int decode_ssr7_head(int sys, int* sync, int* iod, double* udint, int* dispe, int* mw, int* hsize);//rtcm_t* rtcm, int sys, int* sync, int* iod, double* udint, int* dispe, int* mw, int* hsize);

        /* decode ssr 7: phase bias --------------------------------------------------*/
        int decode_ssr7(int sys);

        /* get signal index ----------------------------------------------------------*/
        void sigindex(int sys, const unsigned char* code, const int* freq, int n, const char* opt, int* ind);

        /* save obs data in msm message ----------------------------------------------*/
        void save_msm_obs(int sys, msm_h_t* h, const double* r, const double* pr, const double* cp, const double* rr, const double* rrf, const double* cnr, const int* lock, const int* ex, const int* half);
        //rtcm_t* rtcm, int sys, msm_h_t* h, const double* r, const double* pr, const double* cp, const double* rr, const double* rrf, const double* cnr, const int* lock, const int* ex, const int* half);

        /* decode type msm message header --------------------------------------------*/
        static int decode_msm_head(int sys, int* sync, int* iod, msm_h_t* h, int* hsize);
        //rtcm_t* rtcm, int sys, int* sync, int* iod, msm_h_t* h, int* hsize);

        /* decode unsupported msm message --------------------------------------------*/
        static int decode_msm0(rtcm_t* rtcm, int sys);

        /* decode msm 4: full pseudorange and phaserange plus cnr --------------------*/
        int decode_msm4(int sys);

        /* decode msm 5: full pseudorange, phaserange, phaserangerate and cnr --------*/
        int decode_msm5(int sys);

        /* decode msm 6: full pseudorange and phaserange plus cnr (high-res) ---------*/
        int decode_msm6(int sys);

        /* decode msm 7: full pseudorange, phaserange, phaserangerate and cnr (h-res) */
        int decode_msm7(int sys);

        /* decode type 1230: glonass L1 and L2 code-phase biases ---------------------*/
        int decode_type1230(rtcm_t* rtcm);

        /* decode rtcm ver.3 message -------------------------------------------------*/
        int decode_rtcm3();//rtcm_t* rtcm);

        /* adjust hourly rollover of rtcm 2 time -------------------------------------*/
        void adjhour(rtcm_t* rtcm, double zcnt);
        
        
        
        /* decode type 1/9: differential gps correction/partial correction set -------*/
        int decode_type1(rtcm_t* rtcm);
        
        /* decode type 3: reference station parameter --------------------------------*/
        int decode_type3(rtcm_t* rtcm);
        
        /* decode type 14: gps time of week ------------------------------------------*/
        int decode_type14(rtcm_t* rtcm);
        
        /* decode type 16: gps special message ---------------------------------------*/
        int decode_type16(rtcm_t* rtcm);

        /* decode type 17: gps ephemerides -------------------------------------------*/
        int decode_type17(rtcm_t* rtcm);
        
        /* decode type 18: rtk uncorrected carrier-phase -----------------------------*/
        int decode_type18(rtcm_t* rtcm);
        
        /* decode type 19: rtk uncorrected pseudorange -------------------------------*/
        int decode_type19(rtcm_t* rtcm);
        
        /* decode type 22: extended reference station parameter ----------------------*/
        int decode_type22(rtcm_t* rtcm);
        
        /* decode type 23: antenna type definition record ----------------------------*/
        int decode_type23(rtcm_t* rtcm);
        
        /* decode type 24: antenna reference point (arp) -----------------------------*/
        int decode_type24(rtcm_t* rtcm);

        /* decode type 31: differential glonass correction ---------------------------*/
        int decode_type31(rtcm_t* rtcm);

        /* decode type 32: differential glonass reference station parameters ---------*/
        int decode_type32(rtcm_t* rtcm);
        
        /* decode type 34: glonass partial differential correction set ---------------*/
        int decode_type34(rtcm_t* rtcm);

        /* decode type 36: glonass special message -----------------------------------*/
        int decode_type36(rtcm_t* rtcm);
     
        /* decode type 37: gnss system time offset -----------------------------------*/
        int decode_type37(rtcm_t* rtcm);

        /* decode type 59: proprietary message ---------------------------------------*/
        int decode_type59(rtcm_t* rtcm);
        
        /* decode rtcm ver.2 message -------------------------------------------------*/
        int decode_rtcm2(rtcm_t* rtcm);
       
    };

    
}


