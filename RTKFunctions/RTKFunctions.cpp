#include "pch.h"

#include "RTKFunctions.h"

namespace RTKFunctions {
	

    /* satellite system+prn/slot number to satellite number ------------------------
    * convert satellite system+prn/slot number to satellite number
    * args   : int    sys       I   satellite system (SYS_GPS,SYS_GLO,...)
    *          int    prn       I   satellite prn/slot number
    * return : satellite number (0:error)
    *-----------------------------------------------------------------------------*/
    int CommonRTK::satno(int sys, int prn)
    {
        if (prn <= 0) return 0;
        switch (sys) {
        case SYS_GPS:
            if (prn < MINPRNGPS || MAXPRNGPS < prn) return 0;
            return prn - MINPRNGPS + 1;
        case SYS_GLO:
            if (prn < MINPRNGLO || MAXPRNGLO < prn) return 0;
            return NSATGPS + prn - MINPRNGLO + 1;
        case SYS_GAL:
            if (prn < MINPRNGAL || MAXPRNGAL < prn) return 0;
            return NSATGPS + NSATGLO + prn - MINPRNGAL + 1;
        case SYS_QZS:
            if (prn < MINPRNQZS || MAXPRNQZS < prn) return 0;
            return NSATGPS + NSATGLO + NSATGAL + prn - MINPRNQZS + 1;
        case SYS_CMP:
            if (prn < MINPRNCMP || MAXPRNCMP < prn) return 0;
            return NSATGPS + NSATGLO + NSATGAL + NSATQZS + prn - MINPRNCMP + 1;
        case SYS_IRN:
            if (prn < MINPRNIRN || MAXPRNIRN < prn) return 0;
            return NSATGPS + NSATGLO + NSATGAL + NSATQZS + NSATCMP + prn - MINPRNIRN + 1;
        case SYS_LEO:
            if (prn < MINPRNLEO || MAXPRNLEO < prn) return 0;
            return NSATGPS + NSATGLO + NSATGAL + NSATQZS + NSATCMP + NSATIRN +
                prn - MINPRNLEO + 1;
        case SYS_SBS:
            if (prn < MINPRNSBS || MAXPRNSBS < prn) return 0;
            return NSATGPS + NSATGLO + NSATGAL + NSATQZS + NSATCMP + NSATIRN + NSATLEO +
                prn - MINPRNSBS + 1;
        }
        return 0;
    }

    /* obs code to obs code string -------------------------------------------------
    * convert obs code to obs code string
    * args   : unsigned char code I obs code (CODE_???)
    *          int    *freq  IO     frequency (NULL: no output)
    *                               (1:L1/E1, 2:L2/B1, 3:L5/E5a/L3, 4:L6/LEX/B3,
                                     5:E5b/B2, 6:E5(a+b), 7:S)
    * return : obs code string ("1C","1P","1P",...)
    * notes  : obs codes are based on reference [6] and qzss extension
    *-----------------------------------------------------------------------------*/
    char* CommonRTK::code2obs(unsigned char code, int* freq)
    {
        if (freq) *freq = 0;
        if (code <= CODE_NONE || MAXCODE < code) return "";
        if (freq) *freq = obsfreqs[code];
        return obscodes[code];
    }

    /* obs type string to obs code -------------------------------------------------
    * convert obs code type string to obs code
    * args   : char   *str   I      obs code string ("1C","1P","1Y",...)
    *          int    *freq  IO     frequency (1:L1,2:L2,3:L5,4:L6,5:L7,6:L8,0:err)
    *                               (NULL: no output)
    * return : obs code (CODE_???)
    * notes  : obs codes are based on reference [6] and qzss extension
    *-----------------------------------------------------------------------------*/
    unsigned char CommonRTK::obs2code(const char* obs, int* freq)
    {
        int i;
        if (freq) *freq = 0;
        for (i = 1; *obscodes[i]; i++) {
            if (strcmp(obscodes[i], obs)) continue;
            if (freq) *freq = obsfreqs[i];
            return (unsigned char)i;
        }
        return CODE_NONE;
    }

    /* get code priority -----------------------------------------------------------
    * get code priority for multiple codes in a frequency
    * args   : int    sys     I     system (SYS_???)
    *          unsigned char code I obs code (CODE_???)
    *          char   *opt    I     code options (NULL:no option)
    * return : priority (15:highest-1:lowest,0:error)
    *-----------------------------------------------------------------------------*/
    int CommonRTK::getcodepri(int sys, unsigned char code, const char* opt)
    {
        const char* p, * optstr;
        char* obs, str[8] = "";
        int i, j;

        switch (sys) {
        case SYS_GPS: i = 0; optstr = "-GL%2s"; break;
        case SYS_GLO: i = 1; optstr = "-RL%2s"; break;
        case SYS_GAL: i = 2; optstr = "-EL%2s"; break;
        case SYS_QZS: i = 3; optstr = "-JL%2s"; break;
        case SYS_SBS: i = 4; optstr = "-SL%2s"; break;
        case SYS_CMP: i = 5; optstr = "-CL%2s"; break;
        case SYS_IRN: i = 6; optstr = "-IL%2s"; break;
        default: return 0;
        }
        obs = CommonRTK::code2obs(code, &j);

        /* parse code options */
        for (p = opt; p && (p = strchr(p, '-')); p++) {
            if (sscanf(p, optstr, str) < 1 || str[0] != obs[0]) continue;
            return str[1] == obs[1] ? 15 : 0;
        }
        /* search code priority */
        return (p = strchr(codepris[i][j - 1], obs[1])) ? 14 - (int)(p - codepris[i][j - 1]) : 0;
    }

    /* satellite number to satellite system ----------------------------------------
    * convert satellite number to satellite system
    * args   : int    sat       I   satellite number (1-MAXSAT)
    *          int    *prn      IO  satellite prn/slot number (NULL: no output)
    * return : satellite system (SYS_GPS,SYS_GLO,...)
    *-----------------------------------------------------------------------------*/
    int CommonRTK::satsys(int sat, int* prn)
    {
        int sys = SYS_NONE;
        if (sat <= 0 || MAXSAT < sat) sat = 0;
        else if (sat <= NSATGPS) {
            sys = SYS_GPS; sat += MINPRNGPS - 1;
        }
        else if ((sat -= NSATGPS) <= NSATGLO) {
            sys = SYS_GLO; sat += MINPRNGLO - 1;
        }
        else if ((sat -= NSATGLO) <= NSATGAL) {
            sys = SYS_GAL; sat += MINPRNGAL - 1;
        }
        else if ((sat -= NSATGAL) <= NSATQZS) {
            sys = SYS_QZS; sat += MINPRNQZS - 1;
        }
        else if ((sat -= NSATQZS) <= NSATCMP) {
            sys = SYS_CMP; sat += MINPRNCMP - 1;
        }
        else if ((sat -= NSATCMP) <= NSATIRN) {
            sys = SYS_IRN; sat += MINPRNIRN - 1;
        }
        else if ((sat -= NSATIRN) <= NSATLEO) {
            sys = SYS_LEO; sat += MINPRNLEO - 1;
        }
        else if ((sat -= NSATLEO) <= NSATSBS) {
            sys = SYS_SBS; sat += MINPRNSBS - 1;
        }
        else sat = 0;
        if (prn) *prn = sat;
        return sys;
    }

    /* satellite carrier wave length -----------------------------------------------
    * get satellite carrier wave lengths
    * args   : int    sat       I   satellite number
    *          int    frq       I   frequency index (0:L1,1:L2,2:L5/3,...)
    *          nav_t  *nav      I   navigation messages
    * return : carrier wave length (m) (0.0: error)
    *-----------------------------------------------------------------------------*/
    double CommonRTK::satwavelen(int sat, int frq, const nav_t* nav)
    {
        const double freq_glo[] = { FREQ1_GLO,FREQ2_GLO };
        const double dfrq_glo[] = { DFRQ1_GLO,DFRQ2_GLO };
        int i, sys = CommonRTK::satsys(sat, NULL);

        if (sys == SYS_GLO) {
            if (0 <= frq && frq <= 1) { /* L1,L2 */
                for (i = 0; i < nav->ng; i++) {
                    if (nav->geph[i].sat != sat) continue;
                    return CLIGHT / (freq_glo[frq] + dfrq_glo[frq] * nav->geph[i].frq);
                }
            }
            else if (frq == 2) { /* L3 */
                return CLIGHT / FREQ3_GLO;
            }
        }
        else if (sys == SYS_CMP) {
            if (frq == 0) return CLIGHT / FREQ1_CMP; /* B1 */
            else if (frq == 1) return CLIGHT / FREQ2_CMP; /* B2 */
            else if (frq == 2) return CLIGHT / FREQ3_CMP; /* B3 */
        }
        else if (sys == SYS_GAL) {
            if (frq == 0) return CLIGHT / FREQ1; /* E1 */
            else if (frq == 1) return CLIGHT / FREQ7; /* E5b */
            else if (frq == 2) return CLIGHT / FREQ5; /* E5a */
            else if (frq == 3) return CLIGHT / FREQ6; /* E6 */
            else if (frq == 5) return CLIGHT / FREQ8; /* E5ab */
        }
        else { /* GPS,QZS */
            if (frq == 0) return CLIGHT / FREQ1; /* L1 */
            else if (frq == 1) return CLIGHT / FREQ2; /* L2 */
            else if (frq == 2) return CLIGHT / FREQ5; /* L5 */
            else if (frq == 3) return CLIGHT / FREQ6; /* L6/LEX */
            else if (frq == 6) return CLIGHT / FREQ9; /* S */
        }
        return 0.0;
    }

    /* add time --------------------------------------------------------------------
    * add time to gtime_t struct
    * args   : gtime_t t        I   gtime_t struct
    *          double sec       I   time to add (s)
    * return : gtime_t struct (t+sec)
    *-----------------------------------------------------------------------------*/
    gtime_t CommonRTK::timeadd(gtime_t t, double sec)
    {
        double tt;

        t.sec += sec; tt = floor(t.sec); t.time += (int)tt; t.sec -= tt;
        return t;
    }

    /* time difference -------------------------------------------------------------
    * difference between gtime_t structs
    * args   : gtime_t t1,t2    I   gtime_t structs
    * return : time difference (t1-t2) (s)
    *-----------------------------------------------------------------------------*/
    double CommonRTK::timediff(gtime_t t1, gtime_t t2)
    {
        return difftime(t1.time, t2.time) + t1.sec - t2.sec;
    }

    /* get current time in utc -----------------------------------------------------
    * get current time in utc
    * args   : none
    * return : current time in utc
    *-----------------------------------------------------------------------------*/
    static double timeoffset_ = 0.0;        /* time offset (s) */

   gtime_t  CommonRTK::timeget(void)
    {
        gtime_t time;
        double ep[6] = { 0 };
//#ifdef WIN32
        SYSTEMTIME ts;

        GetSystemTime(&ts); /* utc */
        ep[0] = ts.wYear; ep[1] = ts.wMonth;  ep[2] = ts.wDay;
        ep[3] = ts.wHour; ep[4] = ts.wMinute; ep[5] = ts.wSecond + ts.wMilliseconds * 1E-3;
//#else
//        struct timeval tv;
//        struct tm* tt;
//
//        if (!gettimeofday(&tv, NULL) && (tt = gmtime(&tv.tv_sec))) {
//            ep[0] = tt->tm_year + 1900; ep[1] = tt->tm_mon + 1; ep[2] = tt->tm_mday;
//            ep[3] = tt->tm_hour; ep[4] = tt->tm_min; ep[5] = tt->tm_sec + tv.tv_usec * 1E-6;
//        }
//#endif
        time = CommonRTK::epoch2time(ep);

//#ifdef CPUTIME_IN_GPST /* cputime operated in gpst */
//        time = gpst2utc(time);
//#endif
        return CommonRTK::timeadd(time, timeoffset_);
    }

    /* convert calendar day/time to time -------------------------------------------
    * convert calendar day/time to gtime_t struct
    * args   : double *ep       I   day/time {year,month,day,hour,min,sec}
    * return : gtime_t struct
    * notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
    *-----------------------------------------------------------------------------*/
    gtime_t CommonRTK::epoch2time(const double* ep)
    {
        const int doy[] = { 1,32,60,91,121,152,182,213,244,274,305,335 };
        gtime_t time = { 0 };
        int days, sec, year = (int)ep[0], mon = (int)ep[1], day = (int)ep[2];

        if (year < 1970 || 2099 < year || mon < 1 || 12 < mon) return time;

        /* leap year if year%4==0 in 1901-2099 */
        days = (year - 1970) * 365 + (year - 1969) / 4 + doy[mon - 1] + day - 2 + (year % 4 == 0 && mon >= 3 ? 1 : 0);
        sec = (int)floor(ep[5]);
        time.time = (time_t)days * 86400 + (int)ep[3] * 3600 + (int)ep[4] * 60 + sec;
        time.sec = ep[5] - sec;
        return time;
    }

    /* time to string --------------------------------------------------------------
    * convert gtime_t struct to string
    * args   : gtime_t t        I   gtime_t struct
    *          char   *s        O   string ("yyyy/mm/dd hh:mm:ss.ssss")
    *          int    n         I   number of decimals
    * return : none
    *-----------------------------------------------------------------------------*/
    void CommonRTK::time2str(gtime_t t, char* s, int n)
    {
        double ep[6];

        if (n < 0) n = 0; else if (n > 12) n = 12;
        if (1.0 - t.sec < 0.5 / pow(10.0, n)) { t.time++; t.sec = 0.0; };
        CommonRTK::time2epoch(t, ep);
        sprintf(s, "%04.0f/%02.0f/%02.0f %02.0f:%02.0f:%0*.*f", ep[0], ep[1], ep[2],
            ep[3], ep[4], n <= 0 ? 2 : n + 3, n <= 0 ? 0 : n, ep[5]);
    }

    /* time to calendar day/time ---------------------------------------------------
    * convert gtime_t struct to calendar day/time
    * args   : gtime_t t        I   gtime_t struct
    *          double *ep       O   day/time {year,month,day,hour,min,sec}
    * return : none
    * notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
    *-----------------------------------------------------------------------------*/
    void CommonRTK::time2epoch(gtime_t t, double* ep)
    {
        const int mday[] = { /* # of days in a month */
            31,28,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31,
            31,29,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31
        };
        int days, sec, mon, day;

        /* leap year if year%4==0 in 1901-2099 */
        days = (int)(t.time / 86400);
        sec = (int)(t.time - (time_t)days * 86400);
        for (day = days % 1461, mon = 0; mon < 48; mon++) {
            if (day >= mday[mon]) day -= mday[mon]; else break;
        }
        ep[0] = 1970 + days / 1461 * 4 + mon / 12; ep[1] = mon % 12 + 1; ep[2] = day + 1;
        ep[3] = sec / 3600; ep[4] = sec % 3600 / 60; ep[5] = sec % 60 + t.sec;
    }

    /* gpstime to utc --------------------------------------------------------------
    * convert gpstime to utc considering leap seconds
    * args   : gtime_t t        I   time expressed in gpstime
    * return : time expressed in utc
    * notes  : ignore slight time offset under 100 ns
    *-----------------------------------------------------------------------------*/
    gtime_t  CommonRTK::gpst2utc(gtime_t t)
    {
        gtime_t tu;
        int i;

        for (i = 0; leaps[i][0] > 0; i++) {
            tu = CommonRTK::timeadd(t, leaps[i][6]);
            if (CommonRTK::timediff(tu, CommonRTK::epoch2time(leaps[i])) >= 0.0) return tu;
        }
        return t;
    }

    /* time to beidouo time (bdt) --------------------------------------------------
    * convert gtime_t struct to week and tow in beidou time (bdt)
    * args   : gtime_t t        I   gtime_t struct
    *          int    *week     IO  week number in bdt (NULL: no output)
    * return : time of week in bdt (s)
    *-----------------------------------------------------------------------------*/
    double CommonRTK::time2bdt(gtime_t t, int* week)
    {
        gtime_t t0 = CommonRTK::epoch2time(bdt0);
        time_t sec = t.time - t0.time;
        int w = (int)(sec / (86400 * 7));

        if (week) *week = w;
        return (double)(sec - (double)w * 86400 * 7) + t.sec;
    }

    /* gpstime to bdt --------------------------------------------------------------
    * convert gpstime to bdt (beidou navigation satellite system time)
    * args   : gtime_t t        I   time expressed in gpstime
    * return : time expressed in bdt
    * notes  : ref [8] 3.3, 2006/1/1 00:00 BDT = 2006/1/1 00:00 UTC
    *          no leap seconds in BDT
    *          ignore slight time offset under 100 ns
    *-----------------------------------------------------------------------------*/
    gtime_t CommonRTK::gpst2bdt(gtime_t t)
    {
        return CommonRTK::timeadd(t, -14.0);
    }


    /* time to gps time ------------------------------------------------------------
    * convert gtime_t struct to week and tow in gps time
    * args   : gtime_t t        I   gtime_t struct
    *          int    *week     IO  week number in gps time (NULL: no output)
    * return : time of week in gps time (s)
    *-----------------------------------------------------------------------------*/
    double CommonRTK::time2gpst(gtime_t t, int* week)
    {
        gtime_t t0 = CommonRTK::epoch2time(gpst0);
        time_t sec = t.time - t0.time;
        int w = (int)(sec / (86400 * 7));

        if (week) *week = w;
        return (double)(sec - (double)w * 86400 * 7) + t.sec;
    }

    /* gps time to time ------------------------------------------------------------
    * convert week and tow in gps time to gtime_t struct
    * args   : int    week      I   week number in gps time
    *          double sec       I   time of week in gps time (s)
    * return : gtime_t struct
    *-----------------------------------------------------------------------------*/
     gtime_t  CommonRTK::gpst2time(int week, double sec)
    {
        gtime_t t = CommonRTK::epoch2time(gpst0);

        if (sec < -1E9 || 1E9 < sec) sec = 0.0;
        t.time += (time_t)86400 * 7 * week + (int)sec;
        t.sec = sec - (int)sec;
        return t;
    }

    /* bdt to gpstime --------------------------------------------------------------
    * convert bdt (beidou navigation satellite system time) to gpstime
    * args   : gtime_t t        I   time expressed in bdt
    * return : time expressed in gpstime
    * notes  : see gpst2bdt()
    *-----------------------------------------------------------------------------*/
    gtime_t CommonRTK::bdt2gpst(gtime_t t)
    {
        return CommonRTK::timeadd(t, 14.0);
    }

    /* beidou time (bdt) to time ---------------------------------------------------
    * convert week and tow in beidou time (bdt) to gtime_t struct
    * args   : int    week      I   week number in bdt
    *          double sec       I   time of week in bdt (s)
    * return : gtime_t struct
    *-----------------------------------------------------------------------------*/
    gtime_t CommonRTK::bdt2time(int week, double sec)
    {
        gtime_t t = CommonRTK::epoch2time(bdt0);

        if (sec < -1E9 || 1E9 < sec) sec = 0.0;
        t.time += (time_t)86400 * 7 * week + (int)sec;
        t.sec = sec - (int)sec;
        return t;
    }


    /* utc to gpstime --------------------------------------------------------------
    * convert utc to gpstime considering leap seconds
    * args   : gtime_t t        I   time expressed in utc
    * return : time expressed in gpstime
    * notes  : ignore slight time offset under 100 ns
    *-----------------------------------------------------------------------------*/
    gtime_t  CommonRTK::utc2gpst(gtime_t t)
    {
        int i;

        for (i = 0; leaps[i][0] > 0; i++) {
            if (CommonRTK::timediff(t, CommonRTK::epoch2time(leaps[i])) >= 0.0) return CommonRTK::timeadd(t, -leaps[i][6]);
        }
        return t;
    }
    
    /* adjust gps week number ------------------------------------------------------
    * adjust gps week number using cpu time
    * args   : int   week       I   not-adjusted gps week number
    * return : adjusted gps week number
    *-----------------------------------------------------------------------------*/
    int CommonRTK::adjgpsweek(int week)
    {
        int w;
        (void)CommonRTK::time2gpst(CommonRTK::utc2gpst(CommonRTK::timeget()), &w);
        if (w < 1560) w = 1560; /* use 2009/12/1 if time is earlier than 2009/12/1 */
        return week + (w - week + 512) / 1024 * 1024;
    }

    /* extract unsigned/signed bits ------------------------------------------------
    * extract unsigned/signed bits from byte data
    * args   : unsigned char *buff I byte data
    *          int    pos    I      bit position from start of data (bits)
    *          int    len    I      bit length (bits) (len<=32)
    * return : extracted unsigned/signed bits
    *-----------------------------------------------------------------------------*/
    int  CommonRTK::getbitu(const unsigned char* buff, int pos, int len)
    {
        unsigned int bits = 0;
        int i;
        for (i = pos; i < pos + len; i++) bits = (bits << 1) + ((buff[i / 8] >> (7 - i % 8)) & 1u);
        return bits;
    }
    int CommonRTK::getbits(const unsigned char* buff, int pos, int len)
    {
        unsigned int bits = CommonRTK::getbitu(buff, pos, len);
        if (len <= 0 || 32 <= len || !(bits & (1u << (len - 1)))) return (int)bits;
        return (int)(bits | (~0u << len)); /* extend sign */
    }

    /* decode navigation data word -------------------------------------------------
    * check party and decode navigation data word
    * args   : unsigned int word I navigation data word (2+30bit)
    *                              (previous word D29*-30* + current word D1-30)
    *          unsigned char *data O decoded navigation data without parity
    *                              (8bitx3)
    * return : status (1:ok,0:parity error)
    * notes  : see reference [1] 20.3.5.2 user parity algorithm
    *-----------------------------------------------------------------------------*/
    int CommonRTK::decode_word(unsigned int word, unsigned char* data)
    {
        const unsigned int hamming[] = {
            0xBB1F3480,0x5D8F9A40,0xAEC7CD00,0x5763E680,0x6BB1F340,0x8B7A89C0
        };
        unsigned int parity = 0, w;
        int i;

        //trace(5, "decodeword: word=%08x\n", word);

        if (word & 0x40000000) word ^= 0x3FFFFFC0;

        for (i = 0; i < 6; i++) {
            parity <<= 1;
            for (w = (word & hamming[i]) >> 6; w; w >>= 1) parity ^= w & 1;
        }
        if (parity != (word & 0x3F)) return 0;

        for (i = 0; i < 3; i++) data[i] = (unsigned char)(word >> (22 - i * 8));
        return 1;
    }

    /* inner product ---------------------------------------------------------------
    * inner product of vectors
    * args   : double *a,*b     I   vector a,b (n x 1)
    *          int    n         I   size of vector a,b
    * return : a'*b
    *-----------------------------------------------------------------------------*/
    double CommonRTK::dot(const double* a, const double* b, int n)
    {
        double c = 0.0;

        while (--n >= 0) c += a[n] * b[n];
        return c;
    }

    /* transform ecef to geodetic postion ------------------------------------------
    * transform ecef position to geodetic position
    * args   : double *r        I   ecef position {x,y,z} (m)
    *          double *pos      O   geodetic position {lat,lon,h} (rad,m)
    * return : none
    * notes  : WGS84, ellipsoidal height
    *-----------------------------------------------------------------------------*/
    void CommonRTK::ecef2pos(const double* r, double* pos)
    {
        double e2 = FE_WGS84 * (2.0 - FE_WGS84), r2 = CommonRTK::dot(r, r, 2), z, zk, v = RE_WGS84, sinp;

        for (z = r[2], zk = 0.0; fabs(z - zk) >= 1E-4;) {
            zk = z;
            sinp = z / sqrt(r2 + z * z);
            v = RE_WGS84 / sqrt(1.0 - e2 * sinp * sinp);
            z = r[2] + v * e2 * sinp;
        }
        pos[0] = r2 > 1E-12 ? atan(z / sqrt(r2)) : (r[2] > 0.0 ? PI / 2.0 : -PI / 2.0);
        pos[1] = r2 > 1E-12 ? atan2(r[1], r[0]) : 0.0;
        pos[2] = sqrt(r2 + z * z) - v;
    }
    
    /* decode receiver raw/rtcm data ---------------------------------------------*/
    int DecodeRTCM::decoderaw(rtksvr_t* svr, int index) {
        obs_t* obs;
        nav_t* nav;
        sbsmsg_t* sbsmsg = NULL;
        int i, ret, sat, fobs = 0;

        ////tracet(4, "decoderaw: index=%d\n", index);

        ///rtksvrlock(svr);

        for (i = 0; i < svr->nb[index]; i++) {

            /* input rtcm/receiver raw data from stream */
            if (svr->format[index] == STRFMT_RTCM2) {
                ret = input_rtcm2(svr->rtcm + index, svr->buff[index][i]);
                obs = &svr->rtcm[index].obs;
                nav = &svr->rtcm[index].nav;
                sat = svr->rtcm[index].ephsat;
            }
            else if (svr->format[index] == STRFMT_RTCM3) {
                ret = input_rtcm3(svr->rtcm + index, svr->buff[index][i]);
                obs = &svr->rtcm[index].obs;
                nav = &svr->rtcm[index].nav;
                sat = svr->rtcm[index].ephsat;
            }
            /*else {
                ret = input_raw(svr->raw + index, svr->format[index], svr->buff[index][i]);
                obs = &svr->raw[index].obs;
                nav = &svr->raw[index].nav;
                sat = svr->raw[index].ephsat;
                sbsmsg = &svr->raw[index].sbsmsg;
            }*/
#if 0 /* record for receiving tick */
            if (ret == 1) {
                //trace(0, "%d %10d T=%s NS=%2d\n", index, tickget(),
                    time_str(obs->data[0].time, 0), obs->n);
            }
#endif
            /* update cmr rover observations cache */
            //if (svr->format[1] == STRFMT_CMR && index == 0 && ret == 1) {
            //    update_cmr(&svr->raw[1], svr, obs);
            //}
            ///* update rtk server */
            //if (ret > 0) updatesvr(svr, ret, obs, nav, sat, sbsmsg, index, fobs);

            /* observation data received */
            if (ret == 1) {
                if (fobs < MAXOBSBUF) fobs++; else svr->prcout++;
            }
        }
        svr->nb[index] = 0;

        //rtksvrunlock(svr);

        return fobs;
	}

    /* crc-24q parity --------------------------------------------------------------
    * compute crc-24q parity for sbas, rtcm3
    * args   : unsigned char *buff I data
    *          int    len    I      data length (bytes)
    * return : crc-24Q parity
    * notes  : see reference [2] A.4.3.3 Parity
    *-----------------------------------------------------------------------------*/
    unsigned int CommonRTK::rtk_crc24q(const unsigned char* buff, int len)
    {
        unsigned int crc = 0;
        int i;

        //trace(4, "rtk_crc24q: len=%d\n", len);

        for (i = 0; i < len; i++) crc = ((crc << 8) & 0xFFFFFF) ^ tbl_CRC24Q[(crc >> 16) ^ buff[i]];
        return crc;
    }


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
    int DecodeRTCM::input_rtcm2(rtcm_t* rtcm, unsigned char data) {
        unsigned char preamb;
            int i;

            ////trace(5, "input_rtcm2: data=%02x\n", data);

            if ((data & 0xC0) != 0x40) return 0; /* ignore if upper 2bit != 01 */

            for (i = 0; i < 6; i++, data >>= 1) { /* decode 6-of-8 form */
                rtcm->word = (rtcm->word << 1) + (data & 1);

                /* synchronize frame */
                if (rtcm->nbyte == 0) {
                    preamb = (unsigned char)(rtcm->word >> 22);
                    if (rtcm->word & 0x40000000) preamb ^= 0xFF; /* decode preamble */
                    if (preamb != RTCM2PREAMB) continue;

                    /* check parity */
                    if (!CommonRTK::decode_word(rtcm->word, rtcm->buff)) continue;
                    rtcm->nbyte = 3; rtcm->nbit = 0;
                    continue;
                }
                if (++rtcm->nbit < 30) continue; else rtcm->nbit = 0;

                /* check parity */
                if (!CommonRTK::decode_word(rtcm->word, rtcm->buff + rtcm->nbyte)) {
                    //trace(2, "rtcm2 partity error: i=%d word=%08x\n", i, rtcm->word);
                    rtcm->nbyte = 0; rtcm->word &= 0x3;
                    continue;
                }
                rtcm->nbyte += 3;
                if (rtcm->nbyte == 6) rtcm->len = (rtcm->buff[5] >> 3) * 3 + 6;
                if (rtcm->nbyte < rtcm->len) continue;
                rtcm->nbyte = 0; rtcm->word &= 0x3;

                /* decode rtcm2 message */
                return DecodeRTCM::decode_rtcm2(rtcm);
            }
            return 0;
    }



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
    int DecodeRTCM::input_rtcm3(rtcm_t* rtcm, unsigned char data) {
        ////trace(5, "input_rtcm3: data=%02x\n", data);

        /* synchronize frame */
        if (rtcm->nbyte == 0) {
            if (data != RTCM3PREAMB) return 0;
            rtcm->buff[rtcm->nbyte++] = data;
            return 0;
        }
        rtcm->buff[rtcm->nbyte++] = data;

        if (rtcm->nbyte == 3) {
            rtcm->len = CommonRTK::getbitu(rtcm->buff, 14, 10) + 3; /* length without parity */
        }
        if (rtcm->nbyte < 3 || rtcm->nbyte < rtcm->len + 3) return 0;
        rtcm->nbyte = 0;

        /* check parity */
        if (CommonRTK::rtk_crc24q(rtcm->buff, rtcm->len) != CommonRTK::getbitu(rtcm->buff, rtcm->len * 8, 24)) {
            //trace(2, "rtcm3 parity error: len=%d\n", rtcm->len);
            return 0;
        }
        /* decode rtcm3 message */
        return decode_rtcm3(rtcm);
    }

    /* get sign-magnitude bits ---------------------------------------------------*/
    double DecodeRTCM::getbitg(const unsigned char* buff, int pos, int len)
    {
        double value = CommonRTK::getbitu(buff, pos + 1, len - 1);
        return CommonRTK::getbitu(buff, pos, 1) ? -value : value;
    }

    /* adjust weekly rollover of gps time ----------------------------------------*/
    void DecodeRTCM::adjweek(rtcm_t* rtcm, double tow)
    {
        double tow_p;
        int week;

        /* if no time, get cpu time */
        if (rtcm->time.time == 0) rtcm->time = CommonRTK::utc2gpst(CommonRTK::timeget());
        tow_p = CommonRTK::time2gpst(rtcm->time, &week);
        if (tow < tow_p - 302400.0) tow += 604800.0;
        else if (tow > tow_p + 302400.0) tow -= 604800.0;
        rtcm->time = CommonRTK::gpst2time(week, tow);
    }

    int DecodeRTCM::adjbdtweek(int week)
    {
        int w;
        (void)CommonRTK::time2bdt(CommonRTK::gpst2bdt(CommonRTK::utc2gpst(CommonRTK::timeget())), &w);
        if (w < 1) w = 1; /* use 2006/1/1 if time is earlier than 2006/1/1 */
        return week + (w - week + 512) / 1024 * 1024;
    }

    void DecodeRTCM::adjday_glot(rtcm_t* rtcm, double tod)
    {
        gtime_t time;
        double tow, tod_p;
        int week;

        if (rtcm->time.time == 0) rtcm->time = CommonRTK::utc2gpst(CommonRTK::timeget());
        time = CommonRTK::timeadd(CommonRTK::gpst2utc(rtcm->time), 10800.0); /* glonass time */
        tow = CommonRTK::time2gpst(time, &week);
        tod_p = fmod(tow, 86400.0); tow -= tod_p;
        if (tod < tod_p - 43200.0) tod += 86400.0;
        else if (tod > tod_p + 43200.0) tod -= 86400.0;
        time = CommonRTK::gpst2time(week, tow + tod);
        rtcm->time = CommonRTK::utc2gpst(CommonRTK::timeadd(time, -10800.0));
    }

    double DecodeRTCM::adjcp(rtcm_t* rtcm, int sat, int freq, double cp)
    {
        if (rtcm->cp[sat - 1][freq] == 0.0);
        else if (cp < rtcm->cp[sat - 1][freq] - 750.0) cp += 1500.0;
        else if (cp > rtcm->cp[sat - 1][freq] + 750.0) cp -= 1500.0;
        rtcm->cp[sat - 1][freq] = cp;
        return cp;
    }

    int DecodeRTCM::lossoflock(rtcm_t* rtcm, int sat, int freq, int lock)
    {
        int lli = (!lock && !rtcm->lock[sat - 1][freq]) || lock < rtcm->lock[sat - 1][freq];
        rtcm->lock[sat - 1][freq] = (unsigned short)lock;
        return lli;
    }

    unsigned char DecodeRTCM::snratio(double snr)
    {
        return (unsigned char)(snr <= 0.0 || 255.5 <= snr ? 0.0 : snr * 4.0 + 0.5);
    }

    int obsindex(obs_t* obs, gtime_t time, int sat)
    {
        int i, j;

        for (i = 0; i < obs->n; i++) {
            if (obs->data[i].sat == sat) return i; /* field already exists */
        }
        if (i >= MAXOBS) return -1; /* overflow */

        /* add new field */
        obs->data[i].time = time;
        obs->data[i].sat = sat;
        for (j = 0; j < NFREQ + NEXOBS; j++) {
            obs->data[i].L[j] = obs->data[i].P[j] = 0.0;
            obs->data[i].D[j] = 0.0;
            obs->data[i].SNR[j] = obs->data[i].LLI[j] = obs->data[i].code[j] = 0;
        }
        obs->n++;
        return i;
    }

    int DecodeRTCM::test_staid(rtcm_t* rtcm, int staid)
    {
        char* p;
        int type, id;

        /* test station id option */
        if ((p = strstr(rtcm->opt, "-STA=")) && sscanf(p, "-STA=%d", &id) == 1) {
            if (staid != id) return 0;
        }
        /* save station id */
        if (rtcm->staid == 0 || rtcm->obsflag) {
            rtcm->staid = staid;
        }
        else if (staid != rtcm->staid) {
            type = CommonRTK::getbitu(rtcm->buff, 24, 12);
            //trace(2, "rtcm3 %d staid invalid id=%d %d\n", type, staid, rtcm->staid);

            /* reset station id if station id error */
            rtcm->staid = 0;
            return 0;
        }
        return 1;
    }

    int DecodeRTCM::decode_head1001(rtcm_t* rtcm, int* sync)
    {
        double tow;
        char* msg, tstr[64];
        int i = 24, staid, nsat, type;

        type = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12;

        if (i + 52 <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12);       i += 12;
            tow = CommonRTK::getbitu(rtcm->buff, i, 30) * 0.001; i += 30;
            *sync = CommonRTK::getbitu(rtcm->buff, i, 1);       i += 1;
            nsat = CommonRTK::getbitu(rtcm->buff, i, 5);
        }
        else {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        /* test station id */
        if (!DecodeRTCM::test_staid(rtcm, staid)) return -1;

        DecodeRTCM::adjweek(rtcm, tow);

        CommonRTK::time2str(rtcm->time, tstr, 2);
        //trace(4, "decode_head1001: time=%s nsat=%d sync=%d\n", tstr, nsat, *sync);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d %s nsat=%2d sync=%d", staid, tstr, nsat, *sync);
        }
        return nsat;
    }

    int DecodeRTCM::decode_type1001(rtcm_t* rtcm)
    {
        int sync;
        if (DecodeRTCM::decode_head1001(rtcm, &sync) < 0) return -1;
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    /* decode type 1002: extended L1-only gps rtk observables --------------------*/
    int DecodeRTCM::decode_type1002(rtcm_t* rtcm)
    {
        double pr1, cnr1, tt, cp1;
        int i = 24 + 64, j, index, nsat, sync, prn, code, sat, ppr1, lock1, amb, sys;

        if ((nsat = DecodeRTCM::decode_head1001(rtcm, &sync)) < 0) return -1;

        for (j = 0; j < nsat && rtcm->obs.n < MAXOBS && i + 74 <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6); i += 6;
            code = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            pr1 = CommonRTK::getbitu(rtcm->buff, i, 24); i += 24;
            ppr1 = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            lock1 = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            amb = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            cnr1 = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (prn < 40) {
                sys = SYS_GPS;
            }
            else {
                sys = SYS_SBS; prn += 80;
            }
            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 1002 satellite number error: prn=%d\n", prn);
                continue;
            }
            tt = CommonRTK::timediff(rtcm->obs.data[0].time, rtcm->time);
            if (rtcm->obsflag || fabs(tt) > 1E-9) {
                rtcm->obs.n = rtcm->obsflag = 0;
            }
            if ((index = obsindex(&rtcm->obs, rtcm->time, sat)) < 0) continue;
            pr1 = pr1 * 0.02 + amb * PRUNIT_GPS;
            if (ppr1 != (int)0xFFF80000) {
                rtcm->obs.data[index].P[0] = pr1;
                cp1 = adjcp(rtcm, sat, 0, ppr1 * 0.0005 / lam_carr[0]);
                rtcm->obs.data[index].L[0] = pr1 / lam_carr[0] + cp1;
            }
            rtcm->obs.data[index].LLI[0] = DecodeRTCM::lossoflock(rtcm, sat, 0, lock1);
            rtcm->obs.data[index].SNR[0] = DecodeRTCM::snratio(cnr1 * 0.25);
            rtcm->obs.data[index].code[0] = code ? CODE_L1P : CODE_L1C;
        }
        return sync ? 0 : 1;
    }

    int DecodeRTCM::decode_type1003(rtcm_t* rtcm)
    {
        int sync;
        if (DecodeRTCM::decode_head1001(rtcm, &sync) < 0) return -1;
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }

    int DecodeRTCM::decode_type1004(rtcm_t* rtcm)
    {
        const int L2codes[] = { CODE_L2X,CODE_L2P,CODE_L2D,CODE_L2W };
        double pr1, cnr1, cnr2, tt, cp1, cp2;
        int i = 24 + 64, j, index, nsat, sync, prn, sat, code1, code2, pr21, ppr1, ppr2;
        int lock1, lock2, amb, sys;

        if ((nsat = DecodeRTCM::decode_head1001(rtcm, &sync)) < 0) return -1;

        for (j = 0; j < nsat && rtcm->obs.n < MAXOBS && i + 125 <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6); i += 6;
            code1 = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            pr1 = CommonRTK::getbitu(rtcm->buff, i, 24); i += 24;
            ppr1 = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            lock1 = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            amb = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            cnr1 = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            code2 = CommonRTK::getbitu(rtcm->buff, i, 2); i += 2;
            pr21 = CommonRTK::getbits(rtcm->buff, i, 14); i += 14;
            ppr2 = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            lock2 = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            cnr2 = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (prn < 40) {
                sys = SYS_GPS;
            }
            else {
                sys = SYS_SBS; prn += 80;
            }
            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 1004 satellite number error: sys=%d prn=%d\n", sys, prn);
                continue;
            }
            tt = CommonRTK::timediff(rtcm->obs.data[0].time, rtcm->time);
            if (rtcm->obsflag || fabs(tt) > 1E-9) {
                rtcm->obs.n = rtcm->obsflag = 0;
            }
            if ((index = obsindex(&rtcm->obs, rtcm->time, sat)) < 0) continue;
            pr1 = pr1 * 0.02 + amb * PRUNIT_GPS;
            if (ppr1 != (int)0xFFF80000) {
                rtcm->obs.data[index].P[0] = pr1;
                cp1 = adjcp(rtcm, sat, 0, ppr1 * 0.0005 / lam_carr[0]);
                rtcm->obs.data[index].L[0] = pr1 / lam_carr[0] + cp1;
            }
            rtcm->obs.data[index].LLI[0] = lossoflock(rtcm, sat, 0, lock1);
            rtcm->obs.data[index].SNR[0] = snratio(cnr1 * 0.25);
            rtcm->obs.data[index].code[0] = code1 ? CODE_L1P : CODE_L1C;

            if (pr21 != (int)0xFFFFE000) {
                rtcm->obs.data[index].P[1] = pr1 + pr21 * 0.02;
            }
            if (ppr2 != (int)0xFFF80000) {
                cp2 = adjcp(rtcm, sat, 1, ppr2 * 0.0005 / lam_carr[1]);
                rtcm->obs.data[index].L[1] = pr1 / lam_carr[1] + cp2;
            }
            rtcm->obs.data[index].LLI[1] = lossoflock(rtcm, sat, 1, lock2);
            rtcm->obs.data[index].SNR[1] = snratio(cnr2 * 0.25);
            rtcm->obs.data[index].code[1] = L2codes[code2];
        }
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }

    double DecodeRTCM::getbits_38(const unsigned char* buff, int pos)
    {
        return (double)CommonRTK::getbits(buff, pos, 32) * 64.0 + CommonRTK::getbitu(buff, pos + 32, 6);
    }

    int DecodeRTCM::decode_type1005(rtcm_t* rtcm)
    {
        double rr[3], re[3], pos[3];
        char* msg;
        int i = 24 + 12, j, staid, itrf;

        if (i + 140 == rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12;
            itrf = CommonRTK::getbitu(rtcm->buff, i, 6); i += 6 + 4;
            rr[0] = getbits_38(rtcm->buff, i); i += 38 + 2;
            rr[1] = getbits_38(rtcm->buff, i); i += 38 + 2;
            rr[2] = getbits_38(rtcm->buff, i);
        }
        else {
            //trace(2, "rtcm3 1005 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            for (j = 0; j < 3; j++) re[j] = rr[j] * 0.0001;
            CommonRTK::ecef2pos(re, pos);
            sprintf(msg, " staid=%4d pos=%.8f %.8f %.3f", staid, pos[0] * R2D, pos[1] * R2D,
                pos[2]);
        }
        /* test station id */
        if (!test_staid(rtcm, staid)) return -1;

        rtcm->sta.deltype = 0; /* xyz */
        for (j = 0; j < 3; j++) {
            rtcm->sta.pos[j] = rr[j] * 0.0001;
            rtcm->sta.del[j] = 0.0;
        }
        rtcm->sta.hgt = 0.0;
        rtcm->sta.itrf = itrf;
        return 5;
    }

    /* decode type 1006: stationary rtk reference station arp with height --------*/
    int DecodeRTCM::decode_type1006(rtcm_t* rtcm)
    {
        double rr[3], re[3], pos[3], anth;
        char* msg;
        int i = 24 + 12, j, staid, itrf;

        if (i + 156 <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12;
            itrf = CommonRTK::getbitu(rtcm->buff, i, 6); i += 6 + 4;
            rr[0] = getbits_38(rtcm->buff, i); i += 38 + 2;
            rr[1] = getbits_38(rtcm->buff, i); i += 38 + 2;
            rr[2] = getbits_38(rtcm->buff, i); i += 38;
            anth = CommonRTK::getbitu(rtcm->buff, i, 16);
        }
        else {
            //trace(2, "rtcm3 1006 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            for (j = 0; j < 3; j++) re[j] = rr[j] * 0.0001;
            CommonRTK::ecef2pos(re, pos);
            sprintf(msg, " staid=%4d pos=%.8f %.8f %.3f anth=%.3f", staid, pos[0] * R2D,
                pos[1] * R2D, pos[2], anth);
        }
        /* test station id */
        if (!test_staid(rtcm, staid)) return -1;

        rtcm->sta.deltype = 1; /* xyz */
        for (j = 0; j < 3; j++) {
            rtcm->sta.pos[j] = rr[j] * 0.0001;
            rtcm->sta.del[j] = 0.0;
        }
        rtcm->sta.hgt = anth * 0.0001;
        rtcm->sta.itrf = itrf;
        return 5;
    }

    int DecodeRTCM::decode_type1007(rtcm_t* rtcm)
    {
        char des[32] = "";
        char* msg;
        int i = 24 + 12, j, staid, n, setup;

        n = CommonRTK::getbitu(rtcm->buff, i + 12, 8);

        if (i + 28 + 8 * n <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12 + 8;
            for (j = 0; j < n && j < 31; j++) {
                des[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
            setup = CommonRTK::getbitu(rtcm->buff, i, 8);
        }
        else {
            //trace(2, "rtcm3 1007 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d", staid);
        }
        /* test station id */
        if (!test_staid(rtcm, staid)) return -1;

        strncpy(rtcm->sta.antdes, des, n); rtcm->sta.antdes[n] = '\0';
        rtcm->sta.antsetup = setup;
        rtcm->sta.antsno[0] = '\0';
        return 5;
    }

    int DecodeRTCM::decode_type1008(rtcm_t* rtcm)
    {
        char des[32] = "", sno[32] = "";
        char* msg;
        int i = 24 + 12, j, staid, n, m, setup;

        n = CommonRTK::getbitu(rtcm->buff, i + 12, 8);
        m = CommonRTK::getbitu(rtcm->buff, i + 28 + 8 * n, 8);

        if (i + 36 + 8 * (n + m) <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12 + 8;
            for (j = 0; j < n && j < 31; j++) {
                des[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
            setup = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8 + 8;
            for (j = 0; j < m && j < 31; j++) {
                sno[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
        }
        else {
            //trace(2, "rtcm3 1008 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d", staid);
        }
        /* test station id */
        if (!test_staid(rtcm, staid)) return -1;

        strncpy(rtcm->sta.antdes, des, n); rtcm->sta.antdes[n] = '\0';
        rtcm->sta.antsetup = setup;
        strncpy(rtcm->sta.antsno, sno, m); rtcm->sta.antsno[m] = '\0';
        return 5;
    }

    /* decode type 1009-1012 message header --------------------------------------*/
    int DecodeRTCM::decode_head1009(rtcm_t* rtcm, int* sync)
    {
        double tod;
        char* msg, tstr[64];
        int i = 24, staid, nsat, type;

        type = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12;

        if (i + 49 <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12);       i += 12;
            tod = CommonRTK::getbitu(rtcm->buff, i, 27) * 0.001; i += 27; /* sec in a day */
            *sync = CommonRTK::getbitu(rtcm->buff, i, 1);       i += 1;
            nsat = CommonRTK::getbitu(rtcm->buff, i, 5);
        }
        else {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        /* test station id */
        if (!DecodeRTCM::test_staid(rtcm, staid)) return -1;

        DecodeRTCM::adjday_glot(rtcm, tod);

        CommonRTK::time2str(rtcm->time, tstr, 2);
        //trace(4, "decode_head1009: time=%s nsat=%d sync=%d\n", tstr, nsat, *sync);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d %s nsat=%2d sync=%d", staid, tstr, nsat, *sync);
        }
        return nsat;
    }

    /* decode type 1009: L1-only glonass rtk observables -------------------------*/
    int DecodeRTCM::decode_type1009(rtcm_t* rtcm)
    {
        int sync;
        if (DecodeRTCM::decode_head1009(rtcm, &sync) < 0) return -1;
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }

    /* decode type 1010: extended L1-only glonass rtk observables ----------------*/
    int DecodeRTCM::decode_type1010(rtcm_t* rtcm)
    {
        double pr1, cnr1, tt, cp1, lam1;
        int i = 24 + 61, j, index, nsat, sync, prn, sat, code, freq, ppr1, lock1, amb, sys = SYS_GLO;

        if ((nsat = DecodeRTCM::decode_head1009(rtcm, &sync)) < 0) return -1;

        for (j = 0; j < nsat && rtcm->obs.n < MAXOBS && i + 79 <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6); i += 6;
            code = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            freq = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5;
            pr1 = CommonRTK::getbitu(rtcm->buff, i, 25); i += 25;
            ppr1 = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            lock1 = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            amb = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            cnr1 = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 1010 satellite number error: prn=%d\n", prn);
                continue;
            }
            tt = CommonRTK::timediff(rtcm->obs.data[0].time, rtcm->time);
            if (rtcm->obsflag || fabs(tt) > 1E-9) {
                rtcm->obs.n = rtcm->obsflag = 0;
            }
            if ((index = obsindex(&rtcm->obs, rtcm->time, sat)) < 0) continue;
            pr1 = pr1 * 0.02 + amb * PRUNIT_GLO;
            if (ppr1 != (int)0xFFF80000) {
                rtcm->obs.data[index].P[0] = pr1;
                lam1 = CLIGHT / (FREQ1_GLO + DFRQ1_GLO * (freq - 7));
                cp1 = adjcp(rtcm, sat, 0, ppr1 * 0.0005 / lam1);
                rtcm->obs.data[index].L[0] = pr1 / lam1 + cp1;
            }
            rtcm->obs.data[index].LLI[0] = lossoflock(rtcm, sat, 0, lock1);
            rtcm->obs.data[index].SNR[0] = snratio(cnr1 * 0.25);
            rtcm->obs.data[index].code[0] = code ? CODE_L1P : CODE_L1C;
        }
        return sync ? 0 : 1;
    }

    /* decode type 1011: L1&L2 glonass rtk observables ---------------------------*/
    int DecodeRTCM::decode_type1011(rtcm_t* rtcm)
    {
        int sync;
        if (DecodeRTCM::decode_head1009(rtcm, &sync) < 0) return -1;
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }

    /* decode type 1012: extended L1&L2 glonass rtk observables ------------------*/
    int DecodeRTCM::decode_type1012(rtcm_t* rtcm)
    {
        double pr1, cnr1, cnr2, tt, cp1, cp2, lam1, lam2;
        int i = 24 + 61, j, index, nsat, sync, prn, sat, freq, code1, code2, pr21, ppr1, ppr2;
        int lock1, lock2, amb, sys = SYS_GLO;

        if ((nsat = DecodeRTCM::decode_head1009(rtcm, &sync)) < 0) return -1;

        for (j = 0; j < nsat && rtcm->obs.n < MAXOBS && i + 130 <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6); i += 6;
            code1 = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            freq = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5;
            pr1 = CommonRTK::getbitu(rtcm->buff, i, 25); i += 25;
            ppr1 = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            lock1 = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            amb = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            cnr1 = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            code2 = CommonRTK::getbitu(rtcm->buff, i, 2); i += 2;
            pr21 = CommonRTK::getbits(rtcm->buff, i, 14); i += 14;
            ppr2 = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            lock2 = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            cnr2 = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 1012 satellite number error: sys=%d prn=%d\n", sys, prn);
                continue;
            }
            tt = CommonRTK::timediff(rtcm->obs.data[0].time, rtcm->time);
            if (rtcm->obsflag || fabs(tt) > 1E-9) {
                rtcm->obs.n = rtcm->obsflag = 0;
            }
            if ((index = obsindex(&rtcm->obs, rtcm->time, sat)) < 0) continue;
            pr1 = pr1 * 0.02 + amb * PRUNIT_GLO;
            if (ppr1 != (int)0xFFF80000) {
                lam1 = CLIGHT / (FREQ1_GLO + DFRQ1_GLO * (freq - 7));
                rtcm->obs.data[index].P[0] = pr1;
                cp1 = adjcp(rtcm, sat, 0, ppr1 * 0.0005 / lam1);
                rtcm->obs.data[index].L[0] = pr1 / lam1 + cp1;
            }
            rtcm->obs.data[index].LLI[0] = lossoflock(rtcm, sat, 0, lock1);
            rtcm->obs.data[index].SNR[0] = snratio(cnr1 * 0.25);
            rtcm->obs.data[index].code[0] = code1 ? CODE_L1P : CODE_L1C;

            if (pr21 != (int)0xFFFFE000) {
                rtcm->obs.data[index].P[1] = pr1 + pr21 * 0.02;
            }
            if (ppr2 != (int)0xFFF80000) {
                lam2 = CLIGHT / (FREQ2_GLO + DFRQ2_GLO * (freq - 7));
                cp2 = adjcp(rtcm, sat, 1, ppr2 * 0.0005 / lam2);
                rtcm->obs.data[index].L[1] = pr1 / lam2 + cp2;
            }
            rtcm->obs.data[index].LLI[1] = lossoflock(rtcm, sat, 1, lock2);
            rtcm->obs.data[index].SNR[1] = snratio(cnr2 * 0.25);
            rtcm->obs.data[index].code[1] = code2 ? CODE_L2P : CODE_L2C;
        }
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }

    /* decode type 1013: system parameters ---------------------------------------*/
    int DecodeRTCM::decode_type1013(rtcm_t* rtcm)
    {
        return 0;
    }

    /* decode type 1019: gps ephemerides -----------------------------------------*/
    int DecodeRTCM::decode_type1019(rtcm_t* rtcm)
    {
        eph_t eph = { 0 };
        double toc, sqrtA;
        char* msg;
        int i = 24 + 12, prn, sat, week, sys = SYS_GPS;

        if (i + 476 <= rtcm->len * 8) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            week = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.sva = CommonRTK::getbitu(rtcm->buff, i, 4);              i += 4;
            eph.code = CommonRTK::getbitu(rtcm->buff, i, 2);              i += 2;
            eph.idot = CommonRTK::getbits(rtcm->buff, i, 14) * P2_43 * SC2RAD; i += 14;
            eph.iode = CommonRTK::getbitu(rtcm->buff, i, 8);              i += 8;
            toc = CommonRTK::getbitu(rtcm->buff, i, 16) * 16.0;         i += 16;
            eph.f2 = CommonRTK::getbits(rtcm->buff, i, 8) * P2_55;        i += 8;
            eph.f1 = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43;        i += 16;
            eph.f0 = CommonRTK::getbits(rtcm->buff, i, 22) * P2_31;        i += 22;
            eph.iodc = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.crs = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.deln = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43 * SC2RAD; i += 16;
            eph.M0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cuc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.e = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_33;        i += 32;
            eph.cus = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            sqrtA = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_19;        i += 32;
            eph.toes = CommonRTK::getbitu(rtcm->buff, i, 16) * 16.0;         i += 16;
            eph.cic = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.OMG0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cis = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.i0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.crc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.omg = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.OMGd = CommonRTK::getbits(rtcm->buff, i, 24) * P2_43 * SC2RAD; i += 24;
            eph.tgd[0] = CommonRTK::getbits(rtcm->buff, i, 8) * P2_31;        i += 8;
            eph.svh = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            eph.flag = CommonRTK::getbitu(rtcm->buff, i, 1);              i += 1;
            eph.fit = CommonRTK::getbitu(rtcm->buff, i, 1) ? 0.0 : 4.0; /* 0:4hr,1:>4hr */
        }
        else {
            //trace(2, "rtcm3 1019 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (prn >= 40) {
            sys = SYS_SBS; prn += 80;
        }
        //trace(4, "decode_type1019: prn=%d iode=%d toe=%.0f\n", prn, eph.iode, eph.toes);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " prn=%2d iode=%3d iodc=%3d week=%d toe=%6.0f toc=%6.0f svh=%02X",
                prn, eph.iode, eph.iodc, week, eph.toes, toc, eph.svh);
        }
        if (!(sat = CommonRTK::satno(sys, prn))) {
            //trace(2, "rtcm3 1019 satellite number error: prn=%d\n", prn);
            return -1;
        }
        eph.sat = sat;
        eph.week = CommonRTK::adjgpsweek(week);
        eph.toe = CommonRTK::gpst2time(eph.week, eph.toes);
        eph.toc = CommonRTK::gpst2time(eph.week, toc);
        eph.ttr = rtcm->time;
        eph.A = sqrtA * sqrtA;
        if (!strstr(rtcm->opt, "-EPHALL")) {
            if (eph.iode == rtcm->nav.eph[sat - 1].iode) return 0; /* unchanged */
        }
        rtcm->nav.eph[sat - 1] = eph;
        rtcm->ephsat = sat;
        return 2;
    }

    /* decode type 1020: glonass ephemerides -------------------------------------*/
    int DecodeRTCM::decode_type1020(rtcm_t* rtcm)
    {
        geph_t geph = { 0 };
        double tk_h, tk_m, tk_s, toe, tow, tod, tof;
        char* msg;
        int i = 24 + 12, prn, sat, week, tb, bn, sys = SYS_GLO;

        if (i + 348 <= rtcm->len * 8) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6);           i += 6;
            geph.frq = CommonRTK::getbitu(rtcm->buff, i, 5) - 7;         i += 5 + 2 + 2;
            tk_h = CommonRTK::getbitu(rtcm->buff, i, 5);           i += 5;
            tk_m = CommonRTK::getbitu(rtcm->buff, i, 6);           i += 6;
            tk_s = CommonRTK::getbitu(rtcm->buff, i, 1) * 30.0;      i += 1;
            bn = CommonRTK::getbitu(rtcm->buff, i, 1);           i += 1 + 1;
            tb = CommonRTK::getbitu(rtcm->buff, i, 7);           i += 7;
            geph.vel[0] = getbitg(rtcm->buff, i, 24) * P2_20 * 1E3; i += 24;
            geph.pos[0] = getbitg(rtcm->buff, i, 27) * P2_11 * 1E3; i += 27;
            geph.acc[0] = getbitg(rtcm->buff, i, 5) * P2_30 * 1E3; i += 5;
            geph.vel[1] = getbitg(rtcm->buff, i, 24) * P2_20 * 1E3; i += 24;
            geph.pos[1] = getbitg(rtcm->buff, i, 27) * P2_11 * 1E3; i += 27;
            geph.acc[1] = getbitg(rtcm->buff, i, 5) * P2_30 * 1E3; i += 5;
            geph.vel[2] = getbitg(rtcm->buff, i, 24) * P2_20 * 1E3; i += 24;
            geph.pos[2] = getbitg(rtcm->buff, i, 27) * P2_11 * 1E3; i += 27;
            geph.acc[2] = getbitg(rtcm->buff, i, 5) * P2_30 * 1E3; i += 5 + 1;
            geph.gamn = getbitg(rtcm->buff, i, 11) * P2_40;     i += 11 + 3;
            geph.taun = getbitg(rtcm->buff, i, 22) * P2_30;
        }
        else {
            //trace(2, "rtcm3 1020 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (!(sat = CommonRTK::satno(sys, prn))) {
            //trace(2, "rtcm3 1020 satellite number error: prn=%d\n", prn);
            return -1;
        }
        //trace(4, "decode_type1020: prn=%d tk=%02.0f:%02.0f:%02.0f\n", prn, tk_h, tk_m, tk_s);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " prn=%2d tk=%02.0f:%02.0f:%02.0f frq=%2d bn=%d tb=%d",
                prn, tk_h, tk_m, tk_s, geph.frq, bn, tb);
        }
        geph.sat = sat;
        geph.svh = bn;
        geph.iode = tb & 0x7F;
        if (rtcm->time.time == 0) rtcm->time = CommonRTK::utc2gpst(CommonRTK::timeget());
        tow = CommonRTK::time2gpst(CommonRTK::gpst2utc(rtcm->time), &week);
        tod = fmod(tow, 86400.0); tow -= tod;
        tof = tk_h * 3600.0 + tk_m * 60.0 + tk_s - 10800.0; /* lt->utc */
        if (tof < tod - 43200.0) tof += 86400.0;
        else if (tof > tod + 43200.0) tof -= 86400.0;
        geph.tof = CommonRTK::utc2gpst(CommonRTK::gpst2time(week, tow + tof));
        toe = tb * 900.0 - 10800.0; /* lt->utc */
        if (toe < tod - 43200.0) toe += 86400.0;
        else if (toe > tod + 43200.0) toe -= 86400.0;
        geph.toe = CommonRTK::utc2gpst(CommonRTK::gpst2time(week, tow + toe)); /* utc->gpst */

        if (!strstr(rtcm->opt, "-EPHALL")) {
            if (fabs(CommonRTK::timediff(geph.toe, rtcm->nav.geph[prn - 1].toe)) < 1.0 &&
                geph.svh == rtcm->nav.geph[prn - 1].svh) return 0; /* unchanged */
        }
        rtcm->nav.geph[prn - 1] = geph;
        rtcm->ephsat = sat;
        return 2;
    }

    /* decode type 1021: helmert/abridged molodenski -----------------------------*/
    int DecodeRTCM::decode_type1021(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1021: not supported message\n");
        return 0;
    }

    /* decode type 1022: moledenski-badekas transfromation -----------------------*/
    int DecodeRTCM::decode_type1022(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1022: not supported message\n");
        return 0;
    }
    
    /* decode type 1023: residual, ellipoidal grid representation ----------------*/
    int DecodeRTCM::decode_type1023(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1023: not supported message\n");
        return 0;
    }
    
    /* decode type 1024: residual, plane grid representation ---------------------*/
    int DecodeRTCM::decode_type1024(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1024: not supported message\n");
        return 0;
    }
    
    /* decode type 1025: projection (types except LCC2SP,OM) ---------------------*/
    int DecodeRTCM::decode_type1025(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1025: not supported message\n");
        return 0;
    }
    
    /* decode type 1026: projection (LCC2SP - lambert conic conformal (2sp)) -----*/
    int DecodeRTCM::decode_type1026(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1026: not supported message\n");
        return 0;
    }
    
    /* decode type 1027: projection (type OM - oblique mercator) -----------------*/
    int DecodeRTCM::decode_type1027(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1027: not supported message\n");
        return 0;
    }

    /* decode type 1029: unicode text string -------------------------------------*/
    int DecodeRTCM::decode_type1029(rtcm_t* rtcm)
    {
        char* msg;
        int i = 24 + 12, j, staid, mjd, tod, nchar, cunit;

        if (i + 60 <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12;
            mjd = CommonRTK::getbitu(rtcm->buff, i, 16); i += 16;
            tod = CommonRTK::getbitu(rtcm->buff, i, 17); i += 17;
            nchar = CommonRTK::getbitu(rtcm->buff, i, 7); i += 7;
            cunit = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
        }
        else {
            //trace(2, "rtcm3 1029 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (i + nchar * 8 > rtcm->len * 8) {
            //trace(2, "rtcm3 1029 length error: len=%d nchar=%d\n", rtcm->len, nchar);
            return -1;
        }
        for (j = 0; j < nchar && j < 126; j++) {
            rtcm->msg[j] = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
        }
        rtcm->msg[j] = '\0';

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d text=%s", staid, rtcm->msg);
        }
        return 0;
    }

    /* decode type 1030: network rtk residual ------------------------------------*/
    int DecodeRTCM::decode_type1030(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1030: not supported message\n");
        return 0;
    }

    /* decode type 1031: glonass network rtk residual ----------------------------*/
    int DecodeRTCM::decode_type1031(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1031: not supported message\n");
        return 0;
    }

    /* decode type 1032: physical reference station position information ---------*/
    int DecodeRTCM::decode_type1032(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1032: not supported message\n");
        return 0;
    }

    /* decode type 1033: receiver and antenna descriptor -------------------------*/
    int DecodeRTCM::decode_type1033(rtcm_t* rtcm)
    {
        char des[32] = "", sno[32] = "", rec[32] = "", ver[32] = "", rsn[32] = "";
        char* msg;
        int i = 24 + 12, j, staid, n, m, n1, n2, n3, setup;

        n = CommonRTK::getbitu(rtcm->buff, i + 12, 8);
        m = CommonRTK::getbitu(rtcm->buff, i + 28 + 8 * n, 8);
        n1 = CommonRTK::getbitu(rtcm->buff, i + 36 + 8 * (n + m), 8);
        n2 = CommonRTK::getbitu(rtcm->buff, i + 44 + 8 * (n + m + n1), 8);
        n3 = CommonRTK::getbitu(rtcm->buff, i + 52 + 8 * (n + m + n1 + n2), 8);

        if (i + 60 + 8 * (n + m + n1 + n2 + n3) <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12 + 8;
            for (j = 0; j < n && j < 31; j++) {
                des[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
            setup = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8 + 8;
            for (j = 0; j < m && j < 31; j++) {
                sno[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
            i += 8;
            for (j = 0; j < n1 && j < 31; j++) {
                rec[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
            i += 8;
            for (j = 0; j < n2 && j < 31; j++) {
                ver[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
            i += 8;
            for (j = 0; j < n3 && j < 31; j++) {
                rsn[j] = (char)CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            }
        }
        else {
            //trace(2, "rtcm3 1033 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d", staid);
        }
        /* test station id */
        if (!test_staid(rtcm, staid)) return -1;

        strncpy(rtcm->sta.antdes, des, n); rtcm->sta.antdes[n] = '\0';
        rtcm->sta.antsetup = setup;
        strncpy(rtcm->sta.antsno, sno, m); rtcm->sta.antsno[m] = '\0';
        strncpy(rtcm->sta.rectype, rec, n1); rtcm->sta.rectype[n1] = '\0';
        strncpy(rtcm->sta.recver, ver, n2); rtcm->sta.recver[n2] = '\0';
        strncpy(rtcm->sta.recsno, rsn, n3); rtcm->sta.recsno[n3] = '\0';

        //trace(3, "rtcm3 1033: ant=%s:%s rec=%s:%s:%s\n", des, sno, rec, ver, rsn);
        return 5;
    }

    /* decode type 1034: gps network fkp gradient --------------------------------*/
    int DecodeRTCM::decode_type1034(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1034: not supported message\n");
        return 0;
    }
    /* decode type 1035: glonass network fkp gradient ----------------------------*/
    int DecodeRTCM::decode_type1035(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1035: not supported message\n");
        return 0;
    }
    /* decode type 1037: glonass network rtk ionospheric correction difference ---*/
    int DecodeRTCM::decode_type1037(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1037: not supported message\n");
        return 0;
    }
    /* decode type 1038: glonass network rtk geometic correction difference ------*/
    int DecodeRTCM::decode_type1038(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1038: not supported message\n");
        return 0;
    }
    /* decode type 1039: glonass network rtk combined correction difference ------*/
    int DecodeRTCM::decode_type1039(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1039: not supported message\n");
        return 0;
    }

    /* decode type 1044: qzss ephemerides (ref [15]) -----------------------------*/
    int DecodeRTCM::decode_type1044(rtcm_t* rtcm)
    {
        eph_t eph = { 0 };
        double toc, sqrtA;
        char* msg;
        int i = 24 + 12, prn, sat, week, sys = SYS_QZS;

        if (i + 473 <= rtcm->len * 8) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 4) + 192;          i += 4;
            toc = CommonRTK::getbitu(rtcm->buff, i, 16) * 16.0;         i += 16;
            eph.f2 = CommonRTK::getbits(rtcm->buff, i, 8) * P2_55;        i += 8;
            eph.f1 = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43;        i += 16;
            eph.f0 = CommonRTK::getbits(rtcm->buff, i, 22) * P2_31;        i += 22;
            eph.iode = CommonRTK::getbitu(rtcm->buff, i, 8);              i += 8;
            eph.crs = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.deln = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43 * SC2RAD; i += 16;
            eph.M0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cuc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.e = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_33;        i += 32;
            eph.cus = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            sqrtA = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_19;        i += 32;
            eph.toes = CommonRTK::getbitu(rtcm->buff, i, 16) * 16.0;         i += 16;
            eph.cic = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.OMG0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cis = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.i0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.crc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.omg = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.OMGd = CommonRTK::getbits(rtcm->buff, i, 24) * P2_43 * SC2RAD; i += 24;
            eph.idot = CommonRTK::getbits(rtcm->buff, i, 14) * P2_43 * SC2RAD; i += 14;
            eph.code = CommonRTK::getbitu(rtcm->buff, i, 2);              i += 2;
            week = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.sva = CommonRTK::getbitu(rtcm->buff, i, 4);              i += 4;
            eph.svh = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            eph.tgd[0] = CommonRTK::getbits(rtcm->buff, i, 8) * P2_31;        i += 8;
            eph.iodc = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.fit = CommonRTK::getbitu(rtcm->buff, i, 1) ? 0.0 : 2.0; /* 0:2hr,1:>2hr */
        }
        else {
            //trace(2, "rtcm3 1044 length error: len=%d\n", rtcm->len);
            return -1;
        }
        //trace(4, "decode_type1044: prn=%d iode=%d toe=%.0f\n", prn, eph.iode, eph.toes);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " prn=%3d iode=%3d iodc=%3d week=%d toe=%6.0f toc=%6.0f svh=%02X",
                prn, eph.iode, eph.iodc, week, eph.toes, toc, eph.svh);
        }
        if (!(sat = CommonRTK::satno(sys, prn))) {
            //trace(2, "rtcm3 1044 satellite number error: prn=%d\n", prn);
            return -1;
        }
        eph.sat = sat;
        eph.week = CommonRTK::adjgpsweek(week);
        eph.toe = CommonRTK::gpst2time(eph.week, eph.toes);
        eph.toc = CommonRTK::gpst2time(eph.week, toc);
        eph.ttr = rtcm->time;
        eph.A = sqrtA * sqrtA;
        if (!strstr(rtcm->opt, "-EPHALL")) {
            if (eph.iode == rtcm->nav.eph[sat - 1].iode &&
                eph.iodc == rtcm->nav.eph[sat - 1].iodc) return 0; /* unchanged */
        }
        rtcm->nav.eph[sat - 1] = eph;
        rtcm->ephsat = sat;
        return 2;
    }

    /* decode type 1045: galileo F/NAV satellite ephemerides (ref [15]) ----------*/
    int DecodeRTCM::decode_type1045(rtcm_t* rtcm)
    {
        eph_t eph = { 0 };
        double toc, sqrtA;
        char* msg;
        int i = 24 + 12, prn, sat, week, e5a_hs, e5a_dvs, rsv, sys = SYS_GAL;

        if (i + 484 <= rtcm->len * 8) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            week = CommonRTK::getbitu(rtcm->buff, i, 12);              i += 12; /* gst-week */
            eph.iode = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.sva = CommonRTK::getbitu(rtcm->buff, i, 8);              i += 8;
            eph.idot = CommonRTK::getbits(rtcm->buff, i, 14) * P2_43 * SC2RAD; i += 14;
            toc = CommonRTK::getbitu(rtcm->buff, i, 14) * 60.0;         i += 14;
            eph.f2 = CommonRTK::getbits(rtcm->buff, i, 6) * P2_59;        i += 6;
            eph.f1 = CommonRTK::getbits(rtcm->buff, i, 21) * P2_46;        i += 21;
            eph.f0 = CommonRTK::getbits(rtcm->buff, i, 31) * P2_34;        i += 31;
            eph.crs = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.deln = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43 * SC2RAD; i += 16;
            eph.M0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cuc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.e = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_33;        i += 32;
            eph.cus = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            sqrtA = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_19;        i += 32;
            eph.toes = CommonRTK::getbitu(rtcm->buff, i, 14) * 60.0;         i += 14;
            eph.cic = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.OMG0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cis = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.i0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.crc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.omg = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.OMGd = CommonRTK::getbits(rtcm->buff, i, 24) * P2_43 * SC2RAD; i += 24;
            eph.tgd[0] = CommonRTK::getbits(rtcm->buff, i, 10) * P2_32;        i += 10; /* E5a/E1 */
            e5a_hs = CommonRTK::getbitu(rtcm->buff, i, 2);              i += 2; /* OSHS */
            e5a_dvs = CommonRTK::getbitu(rtcm->buff, i, 1);              i += 1; /* OSDVS */
            rsv = CommonRTK::getbitu(rtcm->buff, i, 7);
        }
        else {
            //trace(2, "rtcm3 1045 length error: len=%d\n", rtcm->len);
            return -1;
        }
        //trace(4, "decode_type1045: prn=%d iode=%d toe=%.0f\n", prn, eph.iode, eph.toes);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " prn=%2d iode=%3d week=%d toe=%6.0f toc=%6.0f hs=%d dvs=%d",
                prn, eph.iode, week, eph.toes, toc, e5a_hs, e5a_dvs);
        }
        if (!(sat = CommonRTK::satno(sys, prn))) {
            //trace(2, "rtcm3 1045 satellite number error: prn=%d\n", prn);
            return -1;
        }
        if (strstr(rtcm->opt, "-GALINAV")) {
            return 0;
        }
        eph.sat = sat;
        eph.week = week + 1024; /* gal-week = gst-week + 1024 */
        eph.toe = CommonRTK::gpst2time(eph.week, eph.toes);
        eph.toc = CommonRTK::gpst2time(eph.week, toc);
        eph.ttr = rtcm->time;
        eph.A = sqrtA * sqrtA;
        eph.svh = (e5a_hs << 4) + (e5a_dvs << 3);
        eph.code = (1 << 1) | (1 << 8); /* data source = f/nav e5a + af0-2,toc,sisa for e5a-e1 */
        if (!strstr(rtcm->opt, "-EPHALL")) {
            if (eph.iode == rtcm->nav.eph[sat - 1].iode) return 0; /* unchanged */
        }
        rtcm->nav.eph[sat - 1] = eph;
        rtcm->ephsat = sat;
        return 2;
    }
    /* decode type 1046: galileo I/NAV satellite ephemerides (ref [17]) ----------*/
    int DecodeRTCM::decode_type1046(rtcm_t* rtcm)
    {
        eph_t eph = { 0 };
        double toc, sqrtA;
        char* msg;
        int i = 24 + 12, prn, sat, week, e5b_hs, e5b_dvs, e1_hs, e1_dvs, sys = SYS_GAL;

        if (i + 492 <= rtcm->len * 8) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            week = CommonRTK::getbitu(rtcm->buff, i, 12);              i += 12;
            eph.iode = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.sva = CommonRTK::getbitu(rtcm->buff, i, 8);              i += 8;
            eph.idot = CommonRTK::getbits(rtcm->buff, i, 14) * P2_43 * SC2RAD; i += 14;
            toc = CommonRTK::getbitu(rtcm->buff, i, 14) * 60.0;         i += 14;
            eph.f2 = CommonRTK::getbits(rtcm->buff, i, 6) * P2_59;        i += 6;
            eph.f1 = CommonRTK::getbits(rtcm->buff, i, 21) * P2_46;        i += 21;
            eph.f0 = CommonRTK::getbits(rtcm->buff, i, 31) * P2_34;        i += 31;
            eph.crs = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.deln = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43 * SC2RAD; i += 16;
            eph.M0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cuc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.e = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_33;        i += 32;
            eph.cus = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            sqrtA = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_19;        i += 32;
            eph.toes = CommonRTK::getbitu(rtcm->buff, i, 14) * 60.0;         i += 14;
            eph.cic = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.OMG0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cis = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.i0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.crc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.omg = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.OMGd = CommonRTK::getbits(rtcm->buff, i, 24) * P2_43 * SC2RAD; i += 24;
            eph.tgd[0] = CommonRTK::getbits(rtcm->buff, i, 10) * P2_32;        i += 10; /* E5a/E1 */
            eph.tgd[1] = CommonRTK::getbits(rtcm->buff, i, 10) * P2_32;        i += 10; /* E5b/E1 */
            e5b_hs = CommonRTK::getbitu(rtcm->buff, i, 2);              i += 2; /* E5b OSHS */
            e5b_dvs = CommonRTK::getbitu(rtcm->buff, i, 1);              i += 1; /* E5b OSDVS */
            e1_hs = CommonRTK::getbitu(rtcm->buff, i, 2);              i += 2; /* E1 OSHS */
            e1_dvs = CommonRTK::getbitu(rtcm->buff, i, 1);              i += 1; /* E1 OSDVS */
        }
        else {
            //trace(2, "rtcm3 1046 length error: len=%d\n", rtcm->len);
            return -1;
        }
        //trace(4, "decode_type1046: prn=%d iode=%d toe=%.0f\n", prn, eph.iode, eph.toes);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " prn=%2d iode=%3d week=%d toe=%6.0f toc=%6.0f hs=%d %d dvs=%d %d",
                prn, eph.iode, week, eph.toes, toc, e5b_hs, e1_hs, e5b_dvs, e1_dvs);
        }
        if (!(sat = CommonRTK::satno(sys, prn))) {
            //trace(2, "rtcm3 1046 satellite number error: prn=%d\n", prn);
            return -1;
        }
        if (strstr(rtcm->opt, "-GALFNAV")) {
            return 0;
        }
        eph.sat = sat;
        eph.week = week + 1024; /* gal-week = gst-week + 1024 */
        eph.toe = CommonRTK::gpst2time(eph.week, eph.toes);
        eph.toc = CommonRTK::gpst2time(eph.week, toc);
        eph.ttr = rtcm->time;
        eph.A = sqrtA * sqrtA;
        eph.svh = (e5b_hs << 7) + (e5b_dvs << 6) + (e1_hs << 1) + (e1_dvs << 0);
        eph.code = (1 << 0) | (1 << 9); /* data source = i/nav e1b + af0-2,toc,sisa for e5b-e1 */
        if (!strstr(rtcm->opt, "-EPHALL")) {
            if (eph.iode == rtcm->nav.eph[sat - 1].iode) return 0; /* unchanged */
        }
        rtcm->nav.eph[sat - 1] = eph;
        rtcm->ephsat = sat;
        return 2;
    }
    /* decode type 1042/63: beidou ephemerides -----------------------------------*/
    int DecodeRTCM::decode_type1042(rtcm_t* rtcm)
    {
        eph_t eph = { 0 };
        double toc, sqrtA;
        char* msg;
        int i = 24 + 12, prn, sat, week, sys = SYS_CMP;

        if (i + 499 <= rtcm->len * 8) {
            prn = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            week = CommonRTK::getbitu(rtcm->buff, i, 13);              i += 13;
            eph.sva = CommonRTK::getbitu(rtcm->buff, i, 4);              i += 4;
            eph.idot = CommonRTK::getbits(rtcm->buff, i, 14) * P2_43 * SC2RAD; i += 14;
            eph.iode = CommonRTK::getbitu(rtcm->buff, i, 5);              i += 5; /* AODE */
            toc = CommonRTK::getbitu(rtcm->buff, i, 17) * 8.0;          i += 17;
            eph.f2 = CommonRTK::getbits(rtcm->buff, i, 11) * P2_66;        i += 11;
            eph.f1 = CommonRTK::getbits(rtcm->buff, i, 22) * P2_50;        i += 22;
            eph.f0 = CommonRTK::getbits(rtcm->buff, i, 24) * P2_33;        i += 24;
            eph.iodc = CommonRTK::getbitu(rtcm->buff, i, 5);              i += 5; /* AODC */
            eph.crs = CommonRTK::getbits(rtcm->buff, i, 18) * P2_6;         i += 18;
            eph.deln = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43 * SC2RAD; i += 16;
            eph.M0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cuc = CommonRTK::getbits(rtcm->buff, i, 18) * P2_31;        i += 18;
            eph.e = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_33;        i += 32;
            eph.cus = CommonRTK::getbits(rtcm->buff, i, 18) * P2_31;        i += 18;
            sqrtA = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_19;        i += 32;
            eph.toes = CommonRTK::getbitu(rtcm->buff, i, 17) * 8.0;          i += 17;
            eph.cic = CommonRTK::getbits(rtcm->buff, i, 18) * P2_31;        i += 18;
            eph.OMG0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cis = CommonRTK::getbits(rtcm->buff, i, 18) * P2_31;        i += 18;
            eph.i0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.crc = CommonRTK::getbits(rtcm->buff, i, 18) * P2_6;         i += 18;
            eph.omg = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.OMGd = CommonRTK::getbits(rtcm->buff, i, 24) * P2_43 * SC2RAD; i += 24;
            eph.tgd[0] = CommonRTK::getbits(rtcm->buff, i, 10) * 1E-10;        i += 10;
            eph.tgd[1] = CommonRTK::getbits(rtcm->buff, i, 10) * 1E-10;        i += 10;
            eph.svh = CommonRTK::getbitu(rtcm->buff, i, 1);              i += 1;
        }
        else {
            //trace(2, "rtcm3 1042 length error: len=%d\n", rtcm->len);
            return -1;
        }
        //trace(4, "decode_type1042: prn=%d iode=%d toe=%.0f\n", prn, eph.iode, eph.toes);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " prn=%2d iode=%3d iodc=%3d week=%d toe=%6.0f toc=%6.0f svh=%02X",
                prn, eph.iode, eph.iodc, week, eph.toes, toc, eph.svh);
        }
        if (!(sat = CommonRTK::satno(sys, prn))) {
            //trace(2, "rtcm3 1042 satellite number error: prn=%d\n", prn);
            return -1;
        }
        eph.sat = sat;
        eph.week = DecodeRTCM::adjbdtweek(week);
        eph.toe = CommonRTK::bdt2gpst(CommonRTK::bdt2time(eph.week, eph.toes)); /* bdt -> gpst */
        eph.toc = CommonRTK::bdt2gpst(CommonRTK::bdt2time(eph.week, toc));      /* bdt -> gpst */
        eph.ttr = rtcm->time;
        eph.A = sqrtA * sqrtA;
        if (!strstr(rtcm->opt, "-EPHALL")) {
            if (CommonRTK::timediff(eph.toe, rtcm->nav.eph[sat - 1].toe) == 0.0 &&
                eph.iode == rtcm->nav.eph[sat - 1].iode &&
                eph.iodc == rtcm->nav.eph[sat - 1].iodc) return 0; /* unchanged */
        }
        rtcm->nav.eph[sat - 1] = eph;
        rtcm->ephsat = sat;
        return 2;
    }
    /* decode ssr 1,4 message header ---------------------------------------------*/
    int DecodeRTCM::decode_ssr1_head(rtcm_t* rtcm, int sys, int* sync, int* iod,
        double* udint, int* refd, int* hsize)
    {
        double tod, tow;
        char* msg, tstr[64];
        int i = 24 + 12, nsat, udi, provid = 0, solid = 0, ns;

        ns = sys == SYS_QZS ? 4 : 6;

        if (i + (sys == SYS_GLO ? 53 : 50 + ns) > rtcm->len * 8) return -1;

        if (sys == SYS_GLO) {
            tod = CommonRTK::getbitu(rtcm->buff, i, 17); i += 17;
            DecodeRTCM::adjday_glot(rtcm, tod);
        }
        else {
            tow = CommonRTK::getbitu(rtcm->buff, i, 20); i += 20;
            DecodeRTCM::adjweek(rtcm, tow);
        }
        udi = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        *sync = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        *refd = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1; /* satellite ref datum */
        *iod = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4; /* iod */
        provid = CommonRTK::getbitu(rtcm->buff, i, 16); i += 16; /* provider id */
        solid = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4; /* solution id */
        nsat = CommonRTK::getbitu(rtcm->buff, i, ns); i += ns;
        *udint = ssrudint[udi];

        CommonRTK::time2str(rtcm->time, tstr, 2);
        //trace(4, "decode_ssr1_head: time=%s sys=%d nsat=%d sync=%d iod=%d provid=%d solid=%d\n",tstr, sys, nsat, *sync, *iod, provid, solid);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " %s nsat=%2d iod=%2d udi=%2d sync=%d", tstr, nsat, *iod, udi,
                *sync);
        }
        *hsize = i;
        return nsat;
    }
    /* decode ssr 2,3,5,6 message header -----------------------------------------*/
    int DecodeRTCM::decode_ssr2_head(rtcm_t* rtcm, int sys, int* sync, int* iod,
        double* udint, int* hsize)
    {
        double tod, tow;
        char* msg, tstr[64];
        int i = 24 + 12, nsat, udi, provid = 0, solid = 0, ns;

        ns = sys == SYS_QZS ? 4 : 6;

        if (i + (sys == SYS_GLO ? 52 : 49 + ns) > rtcm->len * 8) return -1;

        if (sys == SYS_GLO) {
            tod = CommonRTK::getbitu(rtcm->buff, i, 17); i += 17;
            adjday_glot(rtcm, tod);
        }
        else {
            tow = CommonRTK::getbitu(rtcm->buff, i, 20); i += 20;
            DecodeRTCM::adjweek(rtcm, tow);
        }
        udi = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        *sync = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        *iod = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        provid = CommonRTK::getbitu(rtcm->buff, i, 16); i += 16; /* provider id */
        solid = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4; /* solution id */
        nsat = CommonRTK::getbitu(rtcm->buff, i, ns); i += ns;
        *udint = ssrudint[udi];

        CommonRTK::time2str(rtcm->time, tstr, 2);
        //trace(4, "decode_ssr2_head: time=%s sys=%d nsat=%d sync=%d iod=%d provid=%d solid=%d\n",           tstr, sys, nsat, *sync, *iod, provid, solid);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " %s nsat=%2d iod=%2d udi=%2d sync=%d", tstr, nsat, *iod, udi,
                *sync);
        }
        *hsize = i;
        return nsat;
    }

    /* decode ssr 1: orbit corrections -------------------------------------------*/
    int DecodeRTCM::decode_ssr1(rtcm_t* rtcm, int sys)
    {
        double udint, deph[3], ddeph[3];
        int i, j, k, type, sync, iod, nsat, prn, sat, iode, iodcrc, refd = 0, np, ni, nj, offp;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr1_head(rtcm, sys, &sync, &iod, &udint, &refd, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; ni = 8; nj = 0; offp = 0; break;
        case SYS_GLO: np = 5; ni = 8; nj = 0; offp = 0; break;
        case SYS_GAL: np = 6; ni = 10; nj = 0; offp = 0; break;
        case SYS_QZS: np = 4; ni = 8; nj = 0; offp = 192; break;
        case SYS_CMP: np = 6; ni = 10; nj = 24; offp = 1; break;
        case SYS_SBS: np = 6; ni = 9; nj = 24; offp = 120; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 121 + np + ni + nj <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            iode = CommonRTK::getbitu(rtcm->buff, i, ni);      i += ni;
            iodcrc = CommonRTK::getbitu(rtcm->buff, i, nj);      i += nj;
            deph[0] = CommonRTK::getbits(rtcm->buff, i, 22) * 1E-4; i += 22;
            deph[1] = CommonRTK::getbits(rtcm->buff, i, 20) * 4E-4; i += 20;
            deph[2] = CommonRTK::getbits(rtcm->buff, i, 20) * 4E-4; i += 20;
            ddeph[0] = CommonRTK::getbits(rtcm->buff, i, 21) * 1E-6; i += 21;
            ddeph[1] = CommonRTK::getbits(rtcm->buff, i, 19) * 4E-6; i += 19;
            ddeph[2] = CommonRTK::getbits(rtcm->buff, i, 19) * 4E-6; i += 19;

            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[0] = rtcm->time;
            rtcm->ssr[sat - 1].udi[0] = udint;
            rtcm->ssr[sat - 1].iod[0] = iod;
            rtcm->ssr[sat - 1].iode = iode;     /* sbas/bds: toe/t0 modulo */
            rtcm->ssr[sat - 1].iodcrc = iodcrc; /* sbas/bds: iod crc */
            rtcm->ssr[sat - 1].refd = refd;

            for (k = 0; k < 3; k++) {
                rtcm->ssr[sat - 1].deph[k] = deph[k];
                rtcm->ssr[sat - 1].ddeph[k] = ddeph[k];
            }
            rtcm->ssr[sat - 1].update = 1;
        }
        return sync ? 0 : 10;
    }
    /* decode ssr 2: clock corrections -------------------------------------------*/
    int DecodeRTCM::decode_ssr2(rtcm_t* rtcm, int sys)
    {
        double udint, dclk[3];
        int i, j, k, type, sync, iod, nsat, prn, sat, np, offp;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr2_head(rtcm, sys, &sync, &iod, &udint, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; offp = 0; break;
        case SYS_GLO: np = 5; offp = 0; break;
        case SYS_GAL: np = 6; offp = 0; break;
        case SYS_QZS: np = 4; offp = 192; break;
        case SYS_CMP: np = 6; offp = 1; break;
        case SYS_SBS: np = 6; offp = 120; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 70 + np <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            dclk[0] = CommonRTK::getbits(rtcm->buff, i, 22) * 1E-4; i += 22;
            dclk[1] = CommonRTK::getbits(rtcm->buff, i, 21) * 1E-6; i += 21;
            dclk[2] = CommonRTK::getbits(rtcm->buff, i, 27) * 2E-8; i += 27;

            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[1] = rtcm->time;
            rtcm->ssr[sat - 1].udi[1] = udint;
            rtcm->ssr[sat - 1].iod[1] = iod;

            for (k = 0; k < 3; k++) {
                rtcm->ssr[sat - 1].dclk[k] = dclk[k];
            }
            rtcm->ssr[sat - 1].update = 1;
        }
        return sync ? 0 : 10;
    }
    /* decode ssr 3: satellite code biases ---------------------------------------*/
    int DecodeRTCM::decode_ssr3(rtcm_t* rtcm, int sys)
    {
        const int* codes;
        double udint, bias, cbias[MAXCODE];
        int i, j, k, type, mode, sync, iod, nsat, prn, sat, nbias, np, offp, ncode;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr2_head(rtcm, sys, &sync, &iod, &udint, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; offp = 0; codes = codes_gps; ncode = 17; break;
        case SYS_GLO: np = 5; offp = 0; codes = codes_glo; ncode = 4; break;
        case SYS_GAL: np = 6; offp = 0; codes = codes_gal; ncode = 19; break;
        case SYS_QZS: np = 4; offp = 192; codes = codes_qzs; ncode = 13; break;
        case SYS_CMP: np = 6; offp = 1; codes = codes_bds; ncode = 9; break;
        case SYS_SBS: np = 6; offp = 120; codes = codes_sbs; ncode = 4; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 5 + np <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            nbias = CommonRTK::getbitu(rtcm->buff, i, 5);      i += 5;

            for (k = 0; k < MAXCODE; k++) cbias[k] = 0.0;
            for (k = 0; k < nbias && i + 19 <= rtcm->len * 8; k++) {
                mode = CommonRTK::getbitu(rtcm->buff, i, 5);      i += 5;
                bias = CommonRTK::getbits(rtcm->buff, i, 14) * 0.01; i += 14;
                if (mode <= ncode) {
                    cbias[codes[mode] - 1] = (float)bias;
                }
                else {
                    //trace(2, "rtcm3 %d not supported mode: mode=%d\n", type, mode);
                }
            }
            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[4] = rtcm->time;
            rtcm->ssr[sat - 1].udi[4] = udint;
            rtcm->ssr[sat - 1].iod[4] = iod;

            for (k = 0; k < MAXCODE; k++) {
                rtcm->ssr[sat - 1].cbias[k] = (float)cbias[k];
            }
            rtcm->ssr[sat - 1].update = 1;
        }
        return sync ? 0 : 10;
    }
    /* decode ssr 4: combined orbit and clock corrections ------------------------*/
    int DecodeRTCM::decode_ssr4(rtcm_t* rtcm, int sys)
    {
        double udint, deph[3], ddeph[3], dclk[3];
        int i, j, k, type, nsat, sync, iod, prn, sat, iode, iodcrc, refd = 0, np, ni, nj, offp;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr1_head(rtcm, sys, &sync, &iod, &udint, &refd, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; ni = 8; nj = 0; offp = 0; break;
        case SYS_GLO: np = 5; ni = 8; nj = 0; offp = 0; break;
        case SYS_GAL: np = 6; ni = 10; nj = 0; offp = 0; break;
        case SYS_QZS: np = 4; ni = 8; nj = 0; offp = 192; break;
        case SYS_CMP: np = 6; ni = 10; nj = 24; offp = 1; break;
        case SYS_SBS: np = 6; ni = 9; nj = 24; offp = 120; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 191 + np + ni + nj <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            iode = CommonRTK::getbitu(rtcm->buff, i, ni);      i += ni;
            iodcrc = CommonRTK::getbitu(rtcm->buff, i, nj);      i += nj;
            deph[0] = CommonRTK::getbits(rtcm->buff, i, 22) * 1E-4; i += 22;
            deph[1] = CommonRTK::getbits(rtcm->buff, i, 20) * 4E-4; i += 20;
            deph[2] = CommonRTK::getbits(rtcm->buff, i, 20) * 4E-4; i += 20;
            ddeph[0] = CommonRTK::getbits(rtcm->buff, i, 21) * 1E-6; i += 21;
            ddeph[1] = CommonRTK::getbits(rtcm->buff, i, 19) * 4E-6; i += 19;
            ddeph[2] = CommonRTK::getbits(rtcm->buff, i, 19) * 4E-6; i += 19;

            dclk[0] = CommonRTK::getbits(rtcm->buff, i, 22) * 1E-4; i += 22;
            dclk[1] = CommonRTK::getbits(rtcm->buff, i, 21) * 1E-6; i += 21;
            dclk[2] = CommonRTK::getbits(rtcm->buff, i, 27) * 2E-8; i += 27;

            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[0] = rtcm->ssr[sat - 1].t0[1] = rtcm->time;
            rtcm->ssr[sat - 1].udi[0] = rtcm->ssr[sat - 1].udi[1] = udint;
            rtcm->ssr[sat - 1].iod[0] = rtcm->ssr[sat - 1].iod[1] = iod;
            rtcm->ssr[sat - 1].iode = iode;
            rtcm->ssr[sat - 1].iodcrc = iodcrc;
            rtcm->ssr[sat - 1].refd = refd;

            for (k = 0; k < 3; k++) {
                rtcm->ssr[sat - 1].deph[k] = deph[k];
                rtcm->ssr[sat - 1].ddeph[k] = ddeph[k];
                rtcm->ssr[sat - 1].dclk[k] = dclk[k];
            }
            rtcm->ssr[sat - 1].update = 1;
        }
        return sync ? 0 : 10;
    }
    /* decode ssr 5: ura ---------------------------------------------------------*/
    int DecodeRTCM::decode_ssr5(rtcm_t* rtcm, int sys)
    {
        double udint;
        int i, j, type, nsat, sync, iod, prn, sat, ura, np, offp;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr2_head(rtcm, sys, &sync, &iod, &udint, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; offp = 0; break;
        case SYS_GLO: np = 5; offp = 0; break;
        case SYS_GAL: np = 6; offp = 0; break;
        case SYS_QZS: np = 4; offp = 192; break;
        case SYS_CMP: np = 6; offp = 1; break;
        case SYS_SBS: np = 6; offp = 120; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 6 + np <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            ura = CommonRTK::getbitu(rtcm->buff, i, 6);      i += 6;

            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[3] = rtcm->time;
            rtcm->ssr[sat - 1].udi[3] = udint;
            rtcm->ssr[sat - 1].iod[3] = iod;
            rtcm->ssr[sat - 1].ura = ura;
            rtcm->ssr[sat - 1].update = 1;
        }
        return sync ? 0 : 10;
    }
    /* decode ssr 6: high rate clock correction ----------------------------------*/
    int DecodeRTCM::decode_ssr6(rtcm_t* rtcm, int sys)
    {
        double udint, hrclk;
        int i, j, type, nsat, sync, iod, prn, sat, np, offp;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr2_head(rtcm, sys, &sync, &iod, &udint, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; offp = 0; break;
        case SYS_GLO: np = 5; offp = 0; break;
        case SYS_GAL: np = 6; offp = 0; break;
        case SYS_QZS: np = 4; offp = 192; break;
        case SYS_CMP: np = 6; offp = 1; break;
        case SYS_SBS: np = 6; offp = 120; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 22 + np <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            hrclk = CommonRTK::getbits(rtcm->buff, i, 22) * 1E-4; i += 22;

            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[2] = rtcm->time;
            rtcm->ssr[sat - 1].udi[2] = udint;
            rtcm->ssr[sat - 1].iod[2] = iod;
            rtcm->ssr[sat - 1].hrclk = hrclk;
            rtcm->ssr[sat - 1].update = 1;
        }
        return sync ? 0 : 10;
    }
    /* decode ssr 7 message header -----------------------------------------------*/
    int DecodeRTCM::decode_ssr7_head(rtcm_t* rtcm, int sys, int* sync, int* iod, double* udint, int* dispe, int* mw, int* hsize)
    {
        double tod, tow;
        char* msg, tstr[64];
        int i = 24 + 12, nsat, udi, provid = 0, solid = 0, ns;

        ns = sys == SYS_QZS ? 4 : 6;

        if (i + (sys == SYS_GLO ? 54 : 51 + ns) > rtcm->len * 8) return -1;

        if (sys == SYS_GLO) {
            tod = CommonRTK::getbitu(rtcm->buff, i, 17); i += 17;
            DecodeRTCM::adjday_glot(rtcm, tod);
        }
        else {
            tow = CommonRTK::getbitu(rtcm->buff, i, 20); i += 20;
            DecodeRTCM::adjweek(rtcm, tow);
        }
        udi = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        *sync = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        *iod = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        provid = CommonRTK::getbitu(rtcm->buff, i, 16); i += 16; /* provider id */
        solid = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4; /* solution id */
        *dispe = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1; /* dispersive bias consistency ind */
        *mw = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1; /* MW consistency indicator */
        nsat = CommonRTK::getbitu(rtcm->buff, i, ns); i += ns;
        *udint = ssrudint[udi];

        CommonRTK::time2str(rtcm->time, tstr, 2);
        //trace(4, "decode_ssr7_head: time=%s sys=%d nsat=%d sync=%d iod=%d provid=%d solid=%d\n",            tstr, sys, nsat, *sync, *iod, provid, solid);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " %s nsat=%2d iod=%2d udi=%2d sync=%d", tstr, nsat, *iod, udi,
                *sync);
        }
        *hsize = i;
        return nsat;
    }
    /* decode ssr 7: phase bias --------------------------------------------------*/
    int DecodeRTCM::decode_ssr7(rtcm_t* rtcm, int sys)
    {
        const int* codes;
        double udint, bias, std, pbias[MAXCODE], stdpb[MAXCODE];
        int i, j, k, type, mode, sync, iod, nsat, prn, sat, nbias, ncode, np, mw, offp, sii, swl;
        int dispe, sdc, yaw_ang, yaw_rate;

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        if ((nsat = DecodeRTCM::decode_ssr7_head(rtcm, sys, &sync, &iod, &udint, &dispe, &mw, &i)) < 0) {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        switch (sys) {
        case SYS_GPS: np = 6; offp = 0; codes = codes_gps; ncode = 17; break;
        case SYS_GLO: np = 5; offp = 0; codes = codes_glo; ncode = 4; break;
        case SYS_GAL: np = 6; offp = 0; codes = codes_gal; ncode = 19; break;
        case SYS_QZS: np = 4; offp = 192; codes = codes_qzs; ncode = 13; break;
        case SYS_CMP: np = 6; offp = 1; codes = codes_bds; ncode = 9; break;
        default: return sync ? 0 : 10;
        }
        for (j = 0; j < nsat && i + 5 + 17 + np <= rtcm->len * 8; j++) {
            prn = CommonRTK::getbitu(rtcm->buff, i, np) + offp; i += np;
            nbias = CommonRTK::getbitu(rtcm->buff, i, 5);      i += 5;
            yaw_ang = CommonRTK::getbitu(rtcm->buff, i, 9);      i += 9;
            yaw_rate = CommonRTK::getbits(rtcm->buff, i, 8);      i += 8;

            for (k = 0; k < MAXCODE; k++) pbias[k] = stdpb[k] = 0.0;
            for (k = 0; k < nbias && i + 49 <= rtcm->len * 8; k++) {
                mode = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5;
                sii = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1; /* integer-indicator */
                swl = CommonRTK::getbitu(rtcm->buff, i, 2); i += 2; /* WL integer-indicator */
                sdc = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4; /* discontinuity counter */
                bias = CommonRTK::getbits(rtcm->buff, i, 20); i += 20; /* phase bias (m) */
                std = CommonRTK::getbitu(rtcm->buff, i, 17); i += 17; /* phase bias std-dev (m) */
                if (mode <= ncode) {
                    pbias[codes[mode] - 1] = bias * 0.0001; /* (m) */
                    stdpb[codes[mode] - 1] = std * 0.0001; /* (m) */
                }
                else {
                    //trace(2, "rtcm3 %d not supported mode: mode=%d\n", type, mode);
                }
            }
            if (!(sat = CommonRTK::satno(sys, prn))) {
                //trace(2, "rtcm3 %d satellite number error: prn=%d\n", type, prn);
                continue;
            }
            rtcm->ssr[sat - 1].t0[5] = rtcm->time;
            rtcm->ssr[sat - 1].udi[5] = udint;
            rtcm->ssr[sat - 1].iod[5] = iod;
            rtcm->ssr[sat - 1].yaw_ang = yaw_ang / 256.0 * 180.0; /* (deg) */
            rtcm->ssr[sat - 1].yaw_rate = yaw_rate / 8192.0 * 180.0; /* (deg/s) */

            for (k = 0; k < MAXCODE; k++) {
                rtcm->ssr[sat - 1].pbias[k] = pbias[k];
                rtcm->ssr[sat - 1].stdpb[k] = (float)stdpb[k];
            }
        }
        return 20;
    }
    /* get signal index ----------------------------------------------------------*/
    void DecodeRTCM::sigindex(int sys, const unsigned char* code, const int* freq, int n, const char* opt, int* ind)
    {
        int i, nex, pri, pri_h[8] = { 0 }, index[8] = { 0 }, ex[32] = { 0 };

        /* test code priority */
        for (i = 0; i < n; i++) {
            if (!code[i]) continue;

            if (freq[i] > NFREQ) { /* save as extended signal if freq > NFREQ */
                ex[i] = 1;
                continue;
            }
            /* code priority */
            pri = CommonRTK::getcodepri(sys, code[i], opt);

            /* select highest priority signal */
            if (pri > pri_h[freq[i] - 1]) {
                if (index[freq[i] - 1]) ex[index[freq[i] - 1] - 1] = 1;
                pri_h[freq[i] - 1] = pri;
                index[freq[i] - 1] = i + 1;
            }
            else ex[i] = 1;
        }
        /* signal index in obs data */
        for (i = nex = 0; i < n; i++) {
            if (ex[i] == 0) ind[i] = freq[i] - 1;
            else if (nex < NEXOBS) ind[i] = NFREQ + nex++;
            else { /* no space in obs data */
                //trace(2, "rtcm msm: no space in obs data sys=%d code=%d\n", sys, code[i]);
                ind[i] = -1;
            }
#if 0
            //trace(2, "sig pos: sys=%d code=%d ex=%d ind=%d\n", sys, code[i], ex[i], ind[i]);
#endif
        }
    }
    /* save obs data in msm message ----------------------------------------------*/
    void DecodeRTCM::save_msm_obs(rtcm_t* rtcm, int sys, msm_h_t* h, const double* r,
        const double* pr, const double* cp, const double* rr,
        const double* rrf, const double* cnr, const int* lock,
        const int* ex, const int* half)
    {
        const char* sig[32];
        double tt, wl;
        unsigned char code[32];
        char* msm_type = "", * q = NULL;
        int i, j, k, type, prn, sat, fn, index = 0, freq[32], ind[32];

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        switch (sys) {
        case SYS_GPS: msm_type = q = rtcm->msmtype[0]; break;
        case SYS_GLO: msm_type = q = rtcm->msmtype[1]; break;
        case SYS_GAL: msm_type = q = rtcm->msmtype[2]; break;
        case SYS_QZS: msm_type = q = rtcm->msmtype[3]; break;
        case SYS_SBS: msm_type = q = rtcm->msmtype[4]; break;
        case SYS_CMP: msm_type = q = rtcm->msmtype[5]; break;
        }
        /* id to signal */
        for (i = 0; i < h->nsig; i++) {
            switch (sys) {
            case SYS_GPS: sig[i] = msm_sig_gps[h->sigs[i] - 1]; break;
            case SYS_GLO: sig[i] = msm_sig_glo[h->sigs[i] - 1]; break;
            case SYS_GAL: sig[i] = msm_sig_gal[h->sigs[i] - 1]; break;
            case SYS_QZS: sig[i] = msm_sig_qzs[h->sigs[i] - 1]; break;
            case SYS_SBS: sig[i] = msm_sig_sbs[h->sigs[i] - 1]; break;
            case SYS_CMP: sig[i] = msm_sig_cmp[h->sigs[i] - 1]; break;
            default: sig[i] = ""; break;
            }
            /* signal to rinex obs type */
            code[i] = CommonRTK::obs2code(sig[i], freq + i);

            /* freqency index for beidou and galileo */
            if (sys == SYS_CMP) {
                if (freq[i] == 5) freq[i] = 2; /* B2 */
                else if (freq[i] == 4) freq[i] = 3; /* B3 */
            }
            else if (sys == SYS_GAL) {
                if (freq[i] == 5) freq[i] = 2; /* E5b */
            }
            if (code[i] != CODE_NONE) {
                if (q) q += sprintf(q, "L%s%s", sig[i], i < h->nsig - 1 ? "," : "");
            }
            else {
                if (q) q += sprintf(q, "(%d)%s", h->sigs[i], i < h->nsig - 1 ? "," : "");

                //trace(2, "rtcm3 %d: unknown signal id=%2d\n", type, h->sigs[i]);
            }
        }
        //trace(3, "rtcm3 %d: signals=%s\n", type, msm_type);

        /* get signal index */
        DecodeRTCM::sigindex(sys, code, freq, h->nsig, rtcm->opt, ind);

        for (i = j = 0; i < h->nsat; i++) {

            prn = h->sats[i];
            if (sys == SYS_QZS) prn += MINPRNQZS - 1;
            else if (sys == SYS_SBS) prn += MINPRNSBS - 1;

            if ((sat = CommonRTK::satno(sys, prn))) {
                tt = CommonRTK::timediff(rtcm->obs.data[0].time, rtcm->time);
                if (rtcm->obsflag || fabs(tt) > 1E-9) {
                    rtcm->obs.n = rtcm->obsflag = 0;
                }
                index = obsindex(&rtcm->obs, rtcm->time, sat);
            }
            else {
                //trace(2, "rtcm3 %d satellite error: prn=%d\n", type, prn);
            }
            for (k = 0; k < h->nsig; k++) {
                if (!h->cellmask[k + i * h->nsig]) continue;

                if (sat && index >= 0 && ind[k] >= 0) {

                    /* satellite carrier wave length */
                    wl = CommonRTK::satwavelen(sat, freq[k] - 1, &rtcm->nav);

                    /* glonass wave length by extended info */
                    if (sys == SYS_GLO && ex && ex[i] <= 13) {
                        fn = ex[i] - 7;
                        wl = CLIGHT / ((freq[k] == 2 ? FREQ2_GLO : FREQ1_GLO) +
                            (freq[k] == 2 ? DFRQ2_GLO : DFRQ1_GLO) * fn);
                    }
                    /* pseudorange (m) */
                    if (r[i] != 0.0 && pr[j] > -1E12) {
                        rtcm->obs.data[index].P[ind[k]] = r[i] + pr[j];
                    }
                    /* carrier-phase (cycle) */
                    if (r[i] != 0.0 && cp[j] > -1E12 && wl > 0.0) {
                        rtcm->obs.data[index].L[ind[k]] = (r[i] + cp[j]) / wl;
                    }
                    /* doppler (hz) */
                    if (rr && rrf && rrf[j] > -1E12 && wl > 0.0) {
                        rtcm->obs.data[index].D[ind[k]] = (float)(-(rr[i] + rrf[j]) / wl);
                    }
                    rtcm->obs.data[index].LLI[ind[k]] =
                        lossoflock(rtcm, sat, ind[k], lock[j]) + (half[j] ? 3 : 0);
                    rtcm->obs.data[index].SNR[ind[k]] = (unsigned char)(cnr[j] * 4.0);
                    rtcm->obs.data[index].code[ind[k]] = code[k];
                }
                j++;
            }
        }
    }
    /* decode type msm message header --------------------------------------------*/
    int DecodeRTCM::decode_msm_head(rtcm_t* rtcm, int sys, int* sync, int* iod, msm_h_t* h, int* hsize)
    {
        msm_h_t h0 = { 0 };
        double tow, tod;
        char* msg, tstr[64];
        int i = 24, j, dow, mask, staid, type, ncell = 0;

        type = CommonRTK::getbitu(rtcm->buff, i, 12); i += 12;

        *h = h0;
        if (i + 157 <= rtcm->len * 8) {
            staid = CommonRTK::getbitu(rtcm->buff, i, 12);       i += 12;

            if (sys == SYS_GLO) {
                dow = CommonRTK::getbitu(rtcm->buff, i, 3);       i += 3;
                tod = CommonRTK::getbitu(rtcm->buff, i, 27) * 0.001; i += 27;
                adjday_glot(rtcm, tod);
            }
            else if (sys == SYS_CMP) {
                tow = CommonRTK::getbitu(rtcm->buff, i, 30) * 0.001; i += 30;
                tow += 14.0; /* BDT -> GPST */
                DecodeRTCM::adjweek(rtcm, tow);
            }
            else {
                tow = CommonRTK::getbitu(rtcm->buff, i, 30) * 0.001; i += 30;
                DecodeRTCM::adjweek(rtcm, tow);
            }
            *sync = CommonRTK::getbitu(rtcm->buff, i, 1);       i += 1;
            *iod = CommonRTK::getbitu(rtcm->buff, i, 3);       i += 3;
            h->time_s = CommonRTK::getbitu(rtcm->buff, i, 7);       i += 7;
            h->clk_str = CommonRTK::getbitu(rtcm->buff, i, 2);       i += 2;
            h->clk_ext = CommonRTK::getbitu(rtcm->buff, i, 2);       i += 2;
            h->smooth = CommonRTK::getbitu(rtcm->buff, i, 1);       i += 1;
            h->tint_s = CommonRTK::getbitu(rtcm->buff, i, 3);       i += 3;
            for (j = 1; j <= 64; j++) {
                mask = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
                if (mask) h->sats[h->nsat++] = j;
            }
            for (j = 1; j <= 32; j++) {
                mask = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
                if (mask) h->sigs[h->nsig++] = j;
            }
        }
        else {
            //trace(2, "rtcm3 %d length error: len=%d\n", type, rtcm->len);
            return -1;
        }
        /* test station id */
        if (!DecodeRTCM::test_staid(rtcm, staid)) return -1;

        if (h->nsat * h->nsig > 64) {
            //trace(2, "rtcm3 %d number of sats and sigs error: nsat=%d nsig=%d\n",                type, h->nsat, h->nsig);
            return -1;
        }
        if (i + h->nsat * h->nsig > rtcm->len * 8) {
            //trace(2, "rtcm3 %d length error: len=%d nsat=%d nsig=%d\n", type,                rtcm->len, h->nsat, h->nsig);
            return -1;
        }
        for (j = 0; j < h->nsat * h->nsig; j++) {
            h->cellmask[j] = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            if (h->cellmask[j]) ncell++;
        }
        *hsize = i;

        CommonRTK::time2str(rtcm->time, tstr, 2);
        //trace(4, "decode_head_msm: time=%s sys=%d staid=%d nsat=%d nsig=%d sync=%d iod=%d ncell=%d\n",            tstr, sys, staid, h->nsat, h->nsig, *sync, *iod, ncell);

        if (rtcm->outtype) {
            msg = rtcm->msgtype + strlen(rtcm->msgtype);
            sprintf(msg, " staid=%4d %s nsat=%2d nsig=%2d iod=%2d ncell=%2d sync=%d",
                staid, tstr, h->nsat, h->nsig, *iod, ncell, *sync);
        }
        return ncell;
    }
    /* decode unsupported msm message --------------------------------------------*/
    int DecodeRTCM::decode_msm0(rtcm_t* rtcm, int sys)
    {
        msm_h_t h = { 0 };
        int i, sync, iod;
        if (DecodeRTCM::decode_msm_head(rtcm, sys, &sync, &iod, &h, &i) < 0) return -1;
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    /* decode msm 4: full pseudorange and phaserange plus cnr --------------------*/
    int DecodeRTCM::decode_msm4(rtcm_t* rtcm, int sys)
    {
        msm_h_t h = { 0 };
        double r[64], pr[64], cp[64], cnr[64];
        int i, j, type, sync, iod, ncell, rng, rng_m, prv, cpv, lock[64], half[64];

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        /* decode msm header */
        if ((ncell = DecodeRTCM::decode_msm_head(rtcm, sys, &sync, &iod, &h, &i)) < 0) return -1;

        if (i + h.nsat * 18 + ncell * 48 > rtcm->len * 8) {
            //trace(2, "rtcm3 %d length error: nsat=%d ncell=%d len=%d\n", type, h.nsat,                ncell, rtcm->len);
            return -1;
        }
        for (j = 0; j < h.nsat; j++) r[j] = 0.0;
        for (j = 0; j < ncell; j++) pr[j] = cp[j] = -1E16;

        /* decode satellite data */
        for (j = 0; j < h.nsat; j++) { /* range */
            rng = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (rng != 255) r[j] = rng * RANGE_MS;
        }
        for (j = 0; j < h.nsat; j++) {
            rng_m = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
            if (r[j] != 0.0) r[j] += rng_m * P2_10 * RANGE_MS;
        }
        /* decode signal data */
        for (j = 0; j < ncell; j++) { /* pseudorange */
            prv = CommonRTK::getbits(rtcm->buff, i, 15); i += 15;
            if (prv != -16384) pr[j] = prv * P2_24 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* phaserange */
            cpv = CommonRTK::getbits(rtcm->buff, i, 22); i += 22;
            if (cpv != -2097152) cp[j] = cpv * P2_29 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* lock time */
            lock[j] = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        }
        for (j = 0; j < ncell; j++) { /* half-cycle ambiguity */
            half[j] = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        }
        for (j = 0; j < ncell; j++) { /* cnr */
            cnr[j] = CommonRTK::getbitu(rtcm->buff, i, 6) * 1.0; i += 6;
        }
        /* save obs data in msm message */
        DecodeRTCM::save_msm_obs(rtcm, sys, &h, r, pr, cp, NULL, NULL, cnr, lock, NULL, half);

        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    
    /* decode msm 5: full pseudorange, phaserange, phaserangerate and cnr --------*/
    int DecodeRTCM::decode_msm5(rtcm_t* rtcm, int sys)
    {
        msm_h_t h = { 0 };
        double r[64], rr[64], pr[64], cp[64], rrf[64], cnr[64];
        int i, j, type, sync, iod, ncell, rng, rng_m, rate, prv, cpv, rrv, lock[64];
        int ex[64], half[64];

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        /* decode msm header */
        if ((ncell = DecodeRTCM::decode_msm_head(rtcm, sys, &sync, &iod, &h, &i)) < 0) return -1;

        if (i + h.nsat * 36 + ncell * 63 > rtcm->len * 8) {
            //trace(2, "rtcm3 %d length error: nsat=%d ncell=%d len=%d\n", type, h.nsat,                 ncell, rtcm->len);
            return -1;
        }
        for (j = 0; j < h.nsat; j++) {
            r[j] = rr[j] = 0.0; ex[j] = 15;
        }
        for (j = 0; j < ncell; j++) pr[j] = cp[j] = rrf[j] = -1E16;

        /* decode satellite data */
        for (j = 0; j < h.nsat; j++) { /* range */
            rng = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (rng != 255) r[j] = rng * RANGE_MS;
        }
        for (j = 0; j < h.nsat; j++) { /* extended info */
            ex[j] = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        }
        for (j = 0; j < h.nsat; j++) {
            rng_m = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
            if (r[j] != 0.0) r[j] += rng_m * P2_10 * RANGE_MS;
        }
        for (j = 0; j < h.nsat; j++) { /* phaserangerate */
            rate = CommonRTK::getbits(rtcm->buff, i, 14); i += 14;
            if (rate != -8192) rr[j] = rate * 1.0;
        }
        /* decode signal data */
        for (j = 0; j < ncell; j++) { /* pseudorange */
            prv = CommonRTK::getbits(rtcm->buff, i, 15); i += 15;
            if (prv != -16384) pr[j] = prv * P2_24 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* phaserange */
            cpv = CommonRTK::getbits(rtcm->buff, i, 22); i += 22;
            if (cpv != -2097152) cp[j] = cpv * P2_29 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* lock time */
            lock[j] = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        }
        for (j = 0; j < ncell; j++) { /* half-cycle ambiguity */
            half[j] = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        }
        for (j = 0; j < ncell; j++) { /* cnr */
            cnr[j] = CommonRTK::getbitu(rtcm->buff, i, 6) * 1.0; i += 6;
        }
        for (j = 0; j < ncell; j++) { /* phaserangerate */
            rrv = CommonRTK::getbits(rtcm->buff, i, 15); i += 15;
            if (rrv != -16384) rrf[j] = rrv * 0.0001;
        }
        /* save obs data in msm message */
        DecodeRTCM::save_msm_obs(rtcm, sys, &h, r, pr, cp, rr, rrf, cnr, lock, ex, half);

        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    /* decode msm 6: full pseudorange and phaserange plus cnr (high-res) ---------*/
    int DecodeRTCM::decode_msm6(rtcm_t* rtcm, int sys)
    {
        msm_h_t h = { 0 };
        double r[64], pr[64], cp[64], cnr[64];
        int i, j, type, sync, iod, ncell, rng, rng_m, prv, cpv, lock[64], half[64];

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        /* decode msm header */
        if ((ncell = DecodeRTCM::decode_msm_head(rtcm, sys, &sync, &iod, &h, &i)) < 0) return -1;

        if (i + h.nsat * 18 + ncell * 65 > rtcm->len * 8) {
            //trace(2, "rtcm3 %d length error: nsat=%d ncell=%d len=%d\n", type, h.nsat,                ncell, rtcm->len);
            return -1;
        }
        for (j = 0; j < h.nsat; j++) r[j] = 0.0;
        for (j = 0; j < ncell; j++) pr[j] = cp[j] = -1E16;

        /* decode satellite data */
        for (j = 0; j < h.nsat; j++) { /* range */
            rng = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (rng != 255) r[j] = rng * RANGE_MS;
        }
        for (j = 0; j < h.nsat; j++) {
            rng_m = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
            if (r[j] != 0.0) r[j] += rng_m * P2_10 * RANGE_MS;
        }
        /* decode signal data */
        for (j = 0; j < ncell; j++) { /* pseudorange */
            prv = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            if (prv != -524288) pr[j] = prv * P2_29 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* phaserange */
            cpv = CommonRTK::getbits(rtcm->buff, i, 24); i += 24;
            if (cpv != -8388608) cp[j] = cpv * P2_31 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* lock time */
            lock[j] = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
        }
        for (j = 0; j < ncell; j++) { /* half-cycle ambiguity */
            half[j] = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        }
        for (j = 0; j < ncell; j++) { /* cnr */
            cnr[j] = CommonRTK::getbitu(rtcm->buff, i, 10) * 0.0625; i += 10;
        }
        /* save obs data in msm message */
        save_msm_obs(rtcm, sys, &h, r, pr, cp, NULL, NULL, cnr, lock, NULL, half);

        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    
    /* decode msm 7: full pseudorange, phaserange, phaserangerate and cnr (h-res) */
    int DecodeRTCM::decode_msm7(rtcm_t* rtcm, int sys)
    {
        msm_h_t h = { 0 };
        double r[64], rr[64], pr[64], cp[64], rrf[64], cnr[64];
        int i, j, type, sync, iod, ncell, rng, rng_m, rate, prv, cpv, rrv, lock[64];
        int ex[64], half[64];

        type = CommonRTK::getbitu(rtcm->buff, 24, 12);

        /* decode msm header */
        if ((ncell = DecodeRTCM::decode_msm_head(rtcm, sys, &sync, &iod, &h, &i)) < 0) return -1;

        if (i + h.nsat * 36 + ncell * 80 > rtcm->len * 8) {
            //trace(2, "rtcm3 %d length error: nsat=%d ncell=%d len=%d\n", type, h.nsat,                ncell, rtcm->len);
            return -1;
        }
        for (j = 0; j < h.nsat; j++) {
            r[j] = rr[j] = 0.0; ex[j] = 15;
        }
        for (j = 0; j < ncell; j++) pr[j] = cp[j] = rrf[j] = -1E16;

        /* decode satellite data */
        for (j = 0; j < h.nsat; j++) { /* range */
            rng = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            if (rng != 255) r[j] = rng * RANGE_MS;
        }
        for (j = 0; j < h.nsat; j++) { /* extended info */
            ex[j] = CommonRTK::getbitu(rtcm->buff, i, 4); i += 4;
        }
        for (j = 0; j < h.nsat; j++) {
            rng_m = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
            if (r[j] != 0.0) r[j] += rng_m * P2_10 * RANGE_MS;
        }
        for (j = 0; j < h.nsat; j++) { /* phaserangerate */
            rate = CommonRTK::getbits(rtcm->buff, i, 14); i += 14;
            if (rate != -8192) rr[j] = rate * 1.0;
        }
        /* decode signal data */
        for (j = 0; j < ncell; j++) { /* pseudorange */
            prv = CommonRTK::getbits(rtcm->buff, i, 20); i += 20;
            if (prv != -524288) pr[j] = prv * P2_29 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* phaserange */
            cpv = CommonRTK::getbits(rtcm->buff, i, 24); i += 24;
            if (cpv != -8388608) cp[j] = cpv * P2_31 * RANGE_MS;
        }
        for (j = 0; j < ncell; j++) { /* lock time */
            lock[j] = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
        }
        for (j = 0; j < ncell; j++) { /* half-cycle amiguity */
            half[j] = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
        }
        for (j = 0; j < ncell; j++) { /* cnr */
            cnr[j] = CommonRTK::getbitu(rtcm->buff, i, 10) * 0.0625; i += 10;
        }
        for (j = 0; j < ncell; j++) { /* phaserangerate */
            rrv = CommonRTK::getbits(rtcm->buff, i, 15); i += 15;
            if (rrv != -16384) rrf[j] = rrv * 0.0001;
        }
        /* save obs data in msm message */
        save_msm_obs(rtcm, sys, &h, r, pr, cp, rr, rrf, cnr, lock, ex, half);

        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    /* decode type 1230: glonass L1 and L2 code-phase biases ---------------------*/
    int DecodeRTCM::decode_type1230(rtcm_t* rtcm)
    {
        //trace(2, "rtcm3 1230: not supported message\n");
        return 0;
    }
    /* decode rtcm ver.3 message -------------------------------------------------*/
    int DecodeRTCM::decode_rtcm3(rtcm_t* rtcm)
    {
        double tow;
        int ret = 0, type = CommonRTK::getbitu(rtcm->buff, 24, 12), week;

        //trace(3, "decode_rtcm3: len=%3d type=%d\n", rtcm->len, type);

        if (rtcm->outtype) {
            sprintf(rtcm->msgtype, "RTCM %4d (%4d):", type, rtcm->len);
        }
        /* real-time input option */
        if (strstr(rtcm->opt, "-RT_INP")) {
            tow = CommonRTK::time2gpst(CommonRTK::utc2gpst(CommonRTK::timeget()), &week);
            rtcm->time = CommonRTK::gpst2time(week, floor(tow));
        }
        switch (type) {
        case 1001: ret = DecodeRTCM::decode_type1001(rtcm); break; /* not supported */
        case 1002: ret = DecodeRTCM::decode_type1002(rtcm); break;
        case 1003: ret = DecodeRTCM::decode_type1003(rtcm); break; /* not supported */
        case 1004: ret = DecodeRTCM::decode_type1004(rtcm); break;
        case 1005: ret = DecodeRTCM::decode_type1005(rtcm); break;
        case 1006: ret = DecodeRTCM::decode_type1006(rtcm); break;
        case 1007: ret = DecodeRTCM::decode_type1007(rtcm); break;
        case 1008: ret = DecodeRTCM::decode_type1008(rtcm); break;
        case 1009: ret = DecodeRTCM::decode_type1009(rtcm); break; /* not supported */
        case 1010: ret = DecodeRTCM::decode_type1010(rtcm); break;
        case 1011: ret = DecodeRTCM::decode_type1011(rtcm); break; /* not supported */
        case 1012: ret = DecodeRTCM::decode_type1012(rtcm); break;
        case 1013: ret = DecodeRTCM::decode_type1013(rtcm); break; /* not supported */
        case 1019: ret = DecodeRTCM::decode_type1019(rtcm); break;
        case 1020: ret = DecodeRTCM::decode_type1020(rtcm); break;
        case 1021: ret = DecodeRTCM::decode_type1021(rtcm); break; /* not supported */
        case 1022: ret = DecodeRTCM::decode_type1022(rtcm); break; /* not supported */
        case 1023: ret = DecodeRTCM::decode_type1023(rtcm); break; /* not supported */
        case 1024: ret = DecodeRTCM::decode_type1024(rtcm); break; /* not supported */
        case 1025: ret = DecodeRTCM::decode_type1025(rtcm); break; /* not supported */
        case 1026: ret = DecodeRTCM::decode_type1026(rtcm); break; /* not supported */
        case 1027: ret = DecodeRTCM::decode_type1027(rtcm); break; /* not supported */
        case 1029: ret = DecodeRTCM::decode_type1029(rtcm); break;
        case 1030: ret = DecodeRTCM::decode_type1030(rtcm); break; /* not supported */
        case 1031: ret = DecodeRTCM::decode_type1031(rtcm); break; /* not supported */
        case 1032: ret = DecodeRTCM::decode_type1032(rtcm); break; /* not supported */
        case 1033: ret = DecodeRTCM::decode_type1033(rtcm); break;
        case 1034: ret = DecodeRTCM::decode_type1034(rtcm); break; /* not supported */
        case 1035: ret = DecodeRTCM::decode_type1035(rtcm); break; /* not supported */
        case 1037: ret = DecodeRTCM::decode_type1037(rtcm); break; /* not supported */
        case 1038: ret = DecodeRTCM::decode_type1038(rtcm); break; /* not supported */
        case 1039: ret = DecodeRTCM::decode_type1039(rtcm); break; /* not supported */
        case 1044: ret = DecodeRTCM::decode_type1044(rtcm); break;
        case 1045: ret = DecodeRTCM::decode_type1045(rtcm); break;
        case 1046: ret = DecodeRTCM::decode_type1046(rtcm); break;
        case   63: ret = DecodeRTCM::decode_type1042(rtcm); break; /* RTCM draft */
        case 1042: ret = DecodeRTCM::decode_type1042(rtcm); break;
        case 1057: ret = DecodeRTCM::decode_ssr1(rtcm, SYS_GPS); break;
        case 1058: ret = DecodeRTCM::decode_ssr2(rtcm, SYS_GPS); break;
        case 1059: ret = DecodeRTCM::decode_ssr3(rtcm, SYS_GPS); break;
        case 1060: ret = DecodeRTCM::decode_ssr4(rtcm, SYS_GPS); break;
        case 1061: ret = DecodeRTCM::decode_ssr5(rtcm, SYS_GPS); break;
        case 1062: ret = DecodeRTCM::decode_ssr6(rtcm, SYS_GPS); break;
        case 1063: ret = DecodeRTCM::decode_ssr1(rtcm, SYS_GLO); break;
        case 1064: ret = DecodeRTCM::decode_ssr2(rtcm, SYS_GLO); break;
        case 1065: ret = DecodeRTCM::decode_ssr3(rtcm, SYS_GLO); break;
        case 1066: ret = DecodeRTCM::decode_ssr4(rtcm, SYS_GLO); break;
        case 1067: ret = DecodeRTCM::decode_ssr5(rtcm, SYS_GLO); break;
        case 1068: ret = DecodeRTCM::decode_ssr6(rtcm, SYS_GLO); break;
        case 1071: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GPS); break; /* not supported */
        case 1072: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GPS); break; /* not supported */
        case 1073: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GPS); break; /* not supported */
        case 1074: ret = DecodeRTCM::decode_msm4(rtcm, SYS_GPS); break;
        case 1075: ret = DecodeRTCM::decode_msm5(rtcm, SYS_GPS); break;
        case 1076: ret = DecodeRTCM::decode_msm6(rtcm, SYS_GPS); break;
        case 1077: ret = DecodeRTCM::decode_msm7(rtcm, SYS_GPS); break;
        case 1081: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GLO); break; /* not supported */
        case 1082: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GLO); break; /* not supported */
        case 1083: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GLO); break; /* not supported */
        case 1084: ret = DecodeRTCM::decode_msm4(rtcm, SYS_GLO); break;
        case 1085: ret = DecodeRTCM::decode_msm5(rtcm, SYS_GLO); break;
        case 1086: ret = DecodeRTCM::decode_msm6(rtcm, SYS_GLO); break;
        case 1087: ret = DecodeRTCM::decode_msm7(rtcm, SYS_GLO); break;
        case 1091: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GAL); break; /* not supported */
        case 1092: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GAL); break; /* not supported */
        case 1093: ret = DecodeRTCM::decode_msm0(rtcm, SYS_GAL); break; /* not supported */
        case 1094: ret = DecodeRTCM::decode_msm4(rtcm, SYS_GAL); break;
        case 1095: ret = DecodeRTCM::decode_msm5(rtcm, SYS_GAL); break;
        case 1096: ret = DecodeRTCM::decode_msm6(rtcm, SYS_GAL); break;
        case 1097: ret = DecodeRTCM::decode_msm7(rtcm, SYS_GAL); break;
        case 1101: ret = DecodeRTCM::decode_msm0(rtcm, SYS_SBS); break; /* not supported */
        case 1102: ret = DecodeRTCM::decode_msm0(rtcm, SYS_SBS); break; /* not supported */
        case 1103: ret = DecodeRTCM::decode_msm0(rtcm, SYS_SBS); break; /* not supported */
        case 1104: ret = DecodeRTCM::decode_msm4(rtcm, SYS_SBS); break;
        case 1105: ret = DecodeRTCM::decode_msm5(rtcm, SYS_SBS); break;
        case 1106: ret = DecodeRTCM::decode_msm6(rtcm, SYS_SBS); break;
        case 1107: ret = DecodeRTCM::decode_msm7(rtcm, SYS_SBS); break;
        case 1111: ret = DecodeRTCM::decode_msm0(rtcm, SYS_QZS); break; /* not supported */
        case 1112: ret = DecodeRTCM::decode_msm0(rtcm, SYS_QZS); break; /* not supported */
        case 1113: ret = DecodeRTCM::decode_msm0(rtcm, SYS_QZS); break; /* not supported */
        case 1114: ret = DecodeRTCM::decode_msm4(rtcm, SYS_QZS); break;
        case 1115: ret = DecodeRTCM::decode_msm5(rtcm, SYS_QZS); break;
        case 1116: ret = DecodeRTCM::decode_msm6(rtcm, SYS_QZS); break;
        case 1117: ret = DecodeRTCM::decode_msm7(rtcm, SYS_QZS); break;
        case 1121: ret = DecodeRTCM::decode_msm0(rtcm, SYS_CMP); break; /* not supported */
        case 1122: ret = DecodeRTCM::decode_msm0(rtcm, SYS_CMP); break; /* not supported */
        case 1123: ret = DecodeRTCM::decode_msm0(rtcm, SYS_CMP); break; /* not supported */
        case 1124: ret = DecodeRTCM::decode_msm4(rtcm, SYS_CMP); break;
        case 1125: ret = DecodeRTCM::decode_msm5(rtcm, SYS_CMP); break;
        case 1126: ret = DecodeRTCM::decode_msm6(rtcm, SYS_CMP); break;
        case 1127: ret = DecodeRTCM::decode_msm7(rtcm, SYS_CMP); break;
        case 1230: ret = DecodeRTCM::decode_type1230(rtcm);     break; /* not supported */
        case 1240: ret = DecodeRTCM::decode_ssr1(rtcm, SYS_GAL); break;
        case 1241: ret = DecodeRTCM::decode_ssr2(rtcm, SYS_GAL); break;
        case 1242: ret = DecodeRTCM::decode_ssr3(rtcm, SYS_GAL); break;
        case 1243: ret = DecodeRTCM::decode_ssr4(rtcm, SYS_GAL); break;
        case 1244: ret = DecodeRTCM::decode_ssr5(rtcm, SYS_GAL); break;
        case 1245: ret = DecodeRTCM::decode_ssr6(rtcm, SYS_GAL); break;
        case 1246: ret = DecodeRTCM::decode_ssr1(rtcm, SYS_QZS); break;
        case 1247: ret = DecodeRTCM::decode_ssr2(rtcm, SYS_QZS); break;
        case 1248: ret = DecodeRTCM::decode_ssr3(rtcm, SYS_QZS); break;
        case 1249: ret = DecodeRTCM::decode_ssr4(rtcm, SYS_QZS); break;
        case 1250: ret = DecodeRTCM::decode_ssr5(rtcm, SYS_QZS); break;
        case 1251: ret = DecodeRTCM::decode_ssr6(rtcm, SYS_QZS); break;
        case 1252: ret = DecodeRTCM::decode_ssr1(rtcm, SYS_SBS); break;
        case 1253: ret = DecodeRTCM::decode_ssr2(rtcm, SYS_SBS); break;
        case 1254: ret = DecodeRTCM::decode_ssr3(rtcm, SYS_SBS); break;
        case 1255: ret = DecodeRTCM::decode_ssr4(rtcm, SYS_SBS); break;
        case 1256: ret = DecodeRTCM::decode_ssr5(rtcm, SYS_SBS); break;
        case 1257: ret = DecodeRTCM::decode_ssr6(rtcm, SYS_SBS); break;
        case 1258: ret = DecodeRTCM::decode_ssr1(rtcm, SYS_CMP); break;
        case 1259: ret = DecodeRTCM::decode_ssr2(rtcm, SYS_CMP); break;
        case 1260: ret = DecodeRTCM::decode_ssr3(rtcm, SYS_CMP); break;
        case 1261: ret = DecodeRTCM::decode_ssr4(rtcm, SYS_CMP); break;
        case 1262: ret = DecodeRTCM::decode_ssr5(rtcm, SYS_CMP); break;
        case 1263: ret = DecodeRTCM::decode_ssr6(rtcm, SYS_CMP); break;
        case   11: ret = DecodeRTCM::decode_ssr7(rtcm, SYS_GLO); break; /* tentative */
        case   12: ret = DecodeRTCM::decode_ssr7(rtcm, SYS_GAL); break; /* tentative */
        case   13: ret = DecodeRTCM::decode_ssr7(rtcm, SYS_QZS); break; /* tentative */
        case   14: ret = DecodeRTCM::decode_ssr7(rtcm, SYS_CMP); break; /* tentative */
        }
        if (ret >= 0) {
            type -= 1000;
            if (1 <= type && type <= 299) rtcm->nmsg3[type]++; /* 1001-1299 */
            else if (1000 <= type && type <= 1099) rtcm->nmsg3[type - 700]++; /* 2000-2099 */
            else rtcm->nmsg3[0]++;
        }
        return ret;
    }

    /* adjust hourly rollover of rtcm 2 time -------------------------------------*/
    void DecodeRTCM::adjhour(rtcm_t* rtcm, double zcnt)
    {
        double tow, hour, sec;
        int week;

        /* if no time, get cpu time */
        if (rtcm->time.time == 0) rtcm->time = CommonRTK::utc2gpst(CommonRTK::timeget());
        tow = CommonRTK::time2gpst(rtcm->time, &week);
        hour = floor(tow / 3600.0);
        sec = tow - hour * 3600.0;
        if (zcnt < sec - 1800.0) zcnt += 3600.0;
        else if (zcnt > sec + 1800.0) zcnt -= 3600.0;
        rtcm->time = CommonRTK::gpst2time(week, hour * 3600 + zcnt);
    }
    /* get observation data index ------------------------------------------------*/
    int DecodeRTCM::obsindex(obs_t* obs, gtime_t time, int sat)
    {
        int i, j;

        for (i = 0; i < obs->n; i++) {
            if (obs->data[i].sat == sat) return i; /* field already exists */
        }
        if (i >= MAXOBS) return -1; /* overflow */

        /* add new field */
        obs->data[i].time = time;
        obs->data[i].sat = sat;
        for (j = 0; j < NFREQ; j++) {
            obs->data[i].L[j] = obs->data[i].P[j] = 0.0;
            obs->data[i].D[j] = 0.0;
            obs->data[i].SNR[j] = obs->data[i].LLI[j] = obs->data[i].code[j] = 0;
        }
        obs->n++;
        return i;
    }
    /* decode type 1/9: differential gps correction/partial correction set -------*/
    int DecodeRTCM::decode_type1(rtcm_t* rtcm)
    {
        int i = 48, fact, udre, prn, sat, iod;
        double prc, rrc;

        //trace(4, "decode_type1: len=%d\n", rtcm->len);

        while (i + 40 <= rtcm->len * 8) {
            fact = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            udre = CommonRTK::getbitu(rtcm->buff, i, 2); i += 2;
            prn = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5;
            prc = CommonRTK::getbits(rtcm->buff, i, 16); i += 16;
            rrc = CommonRTK::getbits(rtcm->buff, i, 8); i += 8;
            iod = CommonRTK::getbits(rtcm->buff, i, 8); i += 8;
            if (prn == 0) prn = 32;
            if (prc == 0x80000000 || rrc == 0xFFFF8000) {
                //trace(2, "rtcm2 1 prc/rrc indicates satellite problem: prn=%d\n", prn);
                continue;
            }
            if (rtcm->dgps) {
                sat = CommonRTK::satno(SYS_GPS, prn);
                rtcm->dgps[sat - 1].t0 = rtcm->time;
                rtcm->dgps[sat - 1].prc = prc * (fact ? 0.32 : 0.02);
                rtcm->dgps[sat - 1].rrc = rrc * (fact ? 0.032 : 0.002);
                rtcm->dgps[sat - 1].iod = iod;
                rtcm->dgps[sat - 1].udre = udre;
            }
        }
        return 7;
    }
    /* decode type 3: reference station parameter --------------------------------*/
    int DecodeRTCM::decode_type3(rtcm_t* rtcm)
    {
        int i = 48;

        //trace(4, "decode_type3: len=%d\n", rtcm->len);

        if (i + 96 <= rtcm->len * 8) {
            rtcm->sta.pos[0] = CommonRTK::getbits(rtcm->buff, i, 32) * 0.01; i += 32;
            rtcm->sta.pos[1] = CommonRTK::getbits(rtcm->buff, i, 32) * 0.01; i += 32;
            rtcm->sta.pos[2] = CommonRTK::getbits(rtcm->buff, i, 32) * 0.01;
        }
        else {
            //trace(2, "rtcm2 3 length error: len=%d\n", rtcm->len);
            return -1;
        }
        return 5;
    }
    /* decode type 14: gps time of week ------------------------------------------*/
    int DecodeRTCM::decode_type14(rtcm_t* rtcm)
    {
        double zcnt;
        int i = 48, week, hour, leaps;

        //trace(4, "decode_type14: len=%d\n", rtcm->len);

        zcnt = CommonRTK::getbitu(rtcm->buff, 24, 13);
        if (i + 24 <= rtcm->len * 8) {
            week = CommonRTK::getbitu(rtcm->buff, i, 10); i += 10;
            hour = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
            leaps = CommonRTK::getbitu(rtcm->buff, i, 6);
        }
        else {
            //trace(2, "rtcm2 14 length error: len=%d\n", rtcm->len);
            return -1;
        }
        week = CommonRTK::adjgpsweek(week);
        rtcm->time = CommonRTK::gpst2time(week, hour * 3600.0 + zcnt * 0.6);
        rtcm->nav.leaps = leaps;
        return 6;
    }
    /* decode type 16: gps special message ---------------------------------------*/
    int DecodeRTCM::decode_type16(rtcm_t* rtcm)
    {
        int i = 48, n = 0;

        //trace(4, "decode_type16: len=%d\n", rtcm->len);

        while (i + 8 <= rtcm->len * 8 && n < 90) {
            rtcm->msg[n++] = CommonRTK::getbitu(rtcm->buff, i, 8); i += 8;
        }
        rtcm->msg[n] = '\0';

        //trace(3, "rtcm2 16 message: %s\n", rtcm->msg);
        return 9;
    }
    /* decode type 17: gps ephemerides -------------------------------------------*/
    int DecodeRTCM::decode_type17(rtcm_t* rtcm)
    {
        eph_t eph = { 0 };
        double toc, sqrtA;
        int i = 48, week, prn, sat;

        //trace(4, "decode_type17: len=%d\n", rtcm->len);

        if (i + 480 <= rtcm->len * 8) {
            week = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.idot = CommonRTK::getbits(rtcm->buff, i, 14) * P2_43 * SC2RAD; i += 14;
            eph.iode = CommonRTK::getbitu(rtcm->buff, i, 8);              i += 8;
            toc = CommonRTK::getbitu(rtcm->buff, i, 16) * 16.0;         i += 16;
            eph.f1 = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43;        i += 16;
            eph.f2 = CommonRTK::getbits(rtcm->buff, i, 8) * P2_55;        i += 8;
            eph.crs = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.deln = CommonRTK::getbits(rtcm->buff, i, 16) * P2_43 * SC2RAD; i += 16;
            eph.cuc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.e = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_33;        i += 32;
            eph.cus = CommonRTK::getbits(rtcm->buff, i, 16);              i += 16;
            sqrtA = CommonRTK::getbitu(rtcm->buff, i, 32) * P2_19;        i += 32;
            eph.toes = CommonRTK::getbitu(rtcm->buff, i, 16);              i += 16;
            eph.OMG0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cic = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.i0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.cis = CommonRTK::getbits(rtcm->buff, i, 16) * P2_29;        i += 16;
            eph.omg = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.crc = CommonRTK::getbits(rtcm->buff, i, 16) * P2_5;         i += 16;
            eph.OMGd = CommonRTK::getbits(rtcm->buff, i, 24) * P2_43 * SC2RAD; i += 24;
            eph.M0 = CommonRTK::getbits(rtcm->buff, i, 32) * P2_31 * SC2RAD; i += 32;
            eph.iodc = CommonRTK::getbitu(rtcm->buff, i, 10);              i += 10;
            eph.f0 = CommonRTK::getbits(rtcm->buff, i, 22) * P2_31;        i += 22;
            prn = CommonRTK::getbitu(rtcm->buff, i, 5);              i += 5 + 3;
            eph.tgd[0] = CommonRTK::getbits(rtcm->buff, i, 8) * P2_31;        i += 8;
            eph.code = CommonRTK::getbitu(rtcm->buff, i, 2);              i += 2;
            eph.sva = CommonRTK::getbitu(rtcm->buff, i, 4);              i += 4;
            eph.svh = CommonRTK::getbitu(rtcm->buff, i, 6);              i += 6;
            eph.flag = CommonRTK::getbitu(rtcm->buff, i, 1);
        }
        else {
            //trace(2, "rtcm2 17 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (prn == 0) prn = 32;
        sat = CommonRTK::satno(SYS_GPS, prn);
        eph.sat = sat;
        eph.week = CommonRTK::adjgpsweek(week);
        eph.toe = CommonRTK::gpst2time(eph.week, eph.toes);
        eph.toc = CommonRTK::gpst2time(eph.week, toc);
        eph.ttr = rtcm->time;
        eph.A = sqrtA * sqrtA;
        rtcm->nav.eph[sat - 1] = eph;
        rtcm->ephsat = sat;
        return 2;
    }
    /* decode type 18: rtk uncorrected carrier-phase -----------------------------*/
    int DecodeRTCM::decode_type18(rtcm_t* rtcm)
    {
        gtime_t time;
        double usec, cp, tt;
        int i = 48, index, freq, sync = 1, code, sys, prn, sat, loss;

        //trace(4, "decode_type18: len=%d\n", rtcm->len);

        if (i + 24 <= rtcm->len * 8) {
            freq = CommonRTK::getbitu(rtcm->buff, i, 2); i += 2 + 2;
            usec = CommonRTK::getbitu(rtcm->buff, i, 20); i += 20;
        }
        else {
            //trace(2, "rtcm2 18 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (freq & 0x1) {
            //trace(2, "rtcm2 18 not supported frequency: freq=%d\n", freq);
            return -1;
        }
        freq >>= 1;

        while (i + 48 <= rtcm->len * 8 && rtcm->obs.n < MAXOBS) {
            sync = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            code = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            sys = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            prn = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5 + 3;
            loss = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5;
            cp = CommonRTK::getbits(rtcm->buff, i, 32); i += 32;
            if (prn == 0) prn = 32;
            if (!(sat = CommonRTK::satno(sys ? SYS_GLO : SYS_GPS, prn))) {
                //trace(2, "rtcm2 18 satellite number error: sys=%d prn=%d\n", sys, prn);
                continue;
            }
            time = CommonRTK::timeadd(rtcm->time, usec * 1E-6);
            if (sys) time = CommonRTK::utc2gpst(time); /* convert glonass time to gpst */

            tt = CommonRTK::timediff(rtcm->obs.data[0].time, time);
            if (rtcm->obsflag || fabs(tt) > 1E-9) {
                rtcm->obs.n = rtcm->obsflag = 0;
            }
            if ((index = obsindex(&rtcm->obs, time, sat)) >= 0) {
                rtcm->obs.data[index].L[freq] = -cp / 256.0;
                rtcm->obs.data[index].LLI[freq] = rtcm->loss[sat - 1][freq] != loss;
                rtcm->obs.data[index].code[freq] =
                    !freq ? (code ? CODE_L1P : CODE_L1C) : (code ? CODE_L2P : CODE_L2C);
                rtcm->loss[sat - 1][freq] = loss;
            }
        }
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    /* decode type 19: rtk uncorrected pseudorange -------------------------------*/
    int DecodeRTCM::decode_type19(rtcm_t* rtcm)
    {
        gtime_t time;
        double usec, pr, tt;
        int i = 48, index, freq, sync = 1, code, sys, prn, sat;

        //trace(4, "decode_type19: len=%d\n", rtcm->len);

        if (i + 24 <= rtcm->len * 8) {
            freq = CommonRTK::getbitu(rtcm->buff, i, 2); i += 2 + 2;
            usec = CommonRTK::getbitu(rtcm->buff, i, 20); i += 20;
        }
        else {
            //trace(2, "rtcm2 19 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (freq & 0x1) {
            //trace(2, "rtcm2 19 not supported frequency: freq=%d\n", freq);
            return -1;
        }
        freq >>= 1;

        while (i + 48 <= rtcm->len * 8 && rtcm->obs.n < MAXOBS) {
            sync = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            code = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            sys = CommonRTK::getbitu(rtcm->buff, i, 1); i += 1;
            prn = CommonRTK::getbitu(rtcm->buff, i, 5); i += 5 + 8;
            pr = CommonRTK::getbitu(rtcm->buff, i, 32); i += 32;
            if (prn == 0) prn = 32;
            if (!(sat = CommonRTK::satno(sys ? SYS_GLO : SYS_GPS, prn))) {
                //trace(2, "rtcm2 19 satellite number error: sys=%d prn=%d\n", sys, prn);
                continue;
            }
            time = CommonRTK::timeadd(rtcm->time, usec * 1E-6);
            if (sys) time = CommonRTK::utc2gpst(time); /* convert glonass time to gpst */

            tt = CommonRTK::timediff(rtcm->obs.data[0].time, time);
            if (rtcm->obsflag || fabs(tt) > 1E-9) {
                rtcm->obs.n = rtcm->obsflag = 0;
            }
            if ((index = obsindex(&rtcm->obs, time, sat)) >= 0) {
                rtcm->obs.data[index].P[freq] = pr * 0.02;
                rtcm->obs.data[index].code[freq] =
                    !freq ? (code ? CODE_L1P : CODE_L1C) : (code ? CODE_L2P : CODE_L2C);
            }
        }
        rtcm->obsflag = !sync;
        return sync ? 0 : 1;
    }
    /* decode type 22: extended reference station parameter ----------------------*/
    int DecodeRTCM::decode_type22(rtcm_t* rtcm)
    {
        double del[2][3] = { {0} }, hgt = 0.0;
        int i = 48, j, noh;

        //trace(4, "decode_type22: len=%d\n", rtcm->len);

        if (i + 24 <= rtcm->len * 8) {
            del[0][0] = CommonRTK::getbits(rtcm->buff, i, 8) / 25600.0; i += 8;
            del[0][1] = CommonRTK::getbits(rtcm->buff, i, 8) / 25600.0; i += 8;
            del[0][2] = CommonRTK::getbits(rtcm->buff, i, 8) / 25600.0; i += 8;
        }
        else {
            //trace(2, "rtcm2 22 length error: len=%d\n", rtcm->len);
            return -1;
        }
        if (i + 24 <= rtcm->len * 8) {
            i += 5; noh = CommonRTK::getbits(rtcm->buff, i, 1); i += 1;
            hgt = noh ? 0.0 : CommonRTK::getbitu(rtcm->buff, i, 18) / 25600.0;
            i += 18;
        }
        if (i + 24 <= rtcm->len * 8) {
            del[1][0] = CommonRTK::getbits(rtcm->buff, i, 8) / 1600.0; i += 8;
            del[1][1] = CommonRTK::getbits(rtcm->buff, i, 8) / 1600.0; i += 8;
            del[1][2] = CommonRTK::getbits(rtcm->buff, i, 8) / 1600.0;
        }
        rtcm->sta.deltype = 1; /* xyz */
        for (j = 0; j < 3; j++) rtcm->sta.del[j] = del[0][j];
        rtcm->sta.hgt = hgt;
        return 5;
    }
    /* decode type 23: antenna type definition record ----------------------------*/
    int DecodeRTCM::decode_type23(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 24: antenna reference point (arp) -----------------------------*/
    int DecodeRTCM::decode_type24(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 31: differential glonass correction ---------------------------*/
    int DecodeRTCM::decode_type31(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 32: differential glonass reference station parameters ---------*/
    int DecodeRTCM::decode_type32(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 34: glonass partial differential correction set ---------------*/
    int DecodeRTCM::decode_type34(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 36: glonass special message -----------------------------------*/
    int DecodeRTCM::decode_type36(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 37: gnss system time offset -----------------------------------*/
    int DecodeRTCM::decode_type37(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode type 59: proprietary message ---------------------------------------*/
    int DecodeRTCM::decode_type59(rtcm_t* rtcm)
    {
        return 0;
    }
    /* decode rtcm ver.2 message -------------------------------------------------*/
    int DecodeRTCM::decode_rtcm2(rtcm_t* rtcm)
    {
        double zcnt;
        int staid, seqno, stah, ret = 0, type = CommonRTK::getbitu(rtcm->buff, 8, 6);

        //trace(3, "decode_rtcm2: type=%2d len=%3d\n", type, rtcm->len);

        if ((zcnt = CommonRTK::getbitu(rtcm->buff, 24, 13) * 0.6) >= 3600.0) {
            //trace(2, "rtcm2 modified z-count error: zcnt=%.1f\n", zcnt);
            return -1;
        }
        adjhour(rtcm, zcnt);
        staid = CommonRTK::getbitu(rtcm->buff, 14, 10);
        seqno = CommonRTK::getbitu(rtcm->buff, 37, 3);
        stah = CommonRTK::getbitu(rtcm->buff, 45, 3);
        if (seqno - rtcm->seqno != 1 && seqno - rtcm->seqno != -7) {
            //trace(2, "rtcm2 message outage: seqno=%d->%d\n", rtcm->seqno, seqno);
        }
        rtcm->seqno = seqno;
        rtcm->stah = stah;

        if (rtcm->outtype) {
            sprintf(rtcm->msgtype, "RTCM %2d (%4d) zcnt=%7.1f staid=%3d seqno=%d",
                type, rtcm->len, zcnt, staid, seqno);
        }
        if (type == 3 || type == 22 || type == 23 || type == 24) {
            if (rtcm->staid != 0 && staid != rtcm->staid) {
                //trace(2, "rtcm2 station id changed: %d->%d\n", rtcm->staid, staid);
            }
            rtcm->staid = staid;
        }
        if (rtcm->staid != 0 && staid != rtcm->staid) {
            //trace(2, "rtcm2 station id invalid: %d %d\n", staid, rtcm->staid);
            return -1;
        }
        switch (type) {
        case  1: ret = DecodeRTCM::decode_type1(rtcm); break;
        case  3: ret = DecodeRTCM::decode_type3(rtcm); break;
        case  9: ret = DecodeRTCM::decode_type1(rtcm); break;
        case 14: ret = DecodeRTCM::decode_type14(rtcm); break;
        case 16: ret = DecodeRTCM::decode_type16(rtcm); break;
        case 17: ret = DecodeRTCM::decode_type17(rtcm); break;
        case 18: ret = DecodeRTCM::decode_type18(rtcm); break;
        case 19: ret = DecodeRTCM::decode_type19(rtcm); break;
        case 22: ret = DecodeRTCM::decode_type22(rtcm); break;
        case 23: ret = DecodeRTCM::decode_type23(rtcm); break; /* not supported */
        case 24: ret = DecodeRTCM::decode_type24(rtcm); break; /* not supported */
        case 31: ret = DecodeRTCM::decode_type31(rtcm); break; /* not supported */
        case 32: ret = DecodeRTCM::decode_type32(rtcm); break; /* not supported */
        case 34: ret = DecodeRTCM::decode_type34(rtcm); break; /* not supported */
        case 36: ret = DecodeRTCM::decode_type36(rtcm); break; /* not supported */
        case 37: ret = DecodeRTCM::decode_type37(rtcm); break; /* not supported */
        case 59: ret = DecodeRTCM::decode_type59(rtcm); break; /* not supported */
        }
        if (ret >= 0) {
            if (1 <= type && type <= 99) rtcm->nmsg2[type]++; else rtcm->nmsg2[0]++;
        }
        return ret;
    }


    /* adjust weekly rollover of gps time ----------------------------------------*/
   /* gtime_t BINEX::adjweek(gtime_t time, double tow)
    {
        double tow_p;
        int week;
        tow_p = CommonRTK::time2gpst(time, &week);
        if (tow < tow_p - 302400.0) tow += 604800.0;
        else if (tow > tow_p + 302400.0) tow -= 604800.0;
        return CommonRTK::gpst2time(week, tow);
    }*/
}

  
