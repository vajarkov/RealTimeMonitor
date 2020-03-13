using System;

namespace DataExchange
{

    

    public static class RTK_SVR_DATA
    {

        const int NFREQ = 3;
        const int NEXOBS = 0;



        /// <summary>
        /// POS_DATA
        /// </summary>
        public static double[] pos_WGS84 = new double[3];
        public static double[] pos = new double[3];
        public static int nbyte = new int();
        public static byte[] nbuff = new byte[1200];
        public static int len = new int();
        public static byte[] tcp_buff = new byte[1024];
        public static gtime_class gtime = new gtime_class();
        public static char[,] msmtype = new char[6, 128];


        /// <summary>
        /// OBS_DATA
        /// </summary>
        public static int n, nmax =  new int();
        public static byte sat, rcv = new byte(); /* satellite/receiver number */
        public static byte[] SNR = new byte[NFREQ + NEXOBS]; /* signal strength (0.25 dBHz) */
        public static byte[] LLI = new byte[NFREQ + NEXOBS]; /* loss of lock indicator */
        public static byte[] code = new byte[NFREQ + NEXOBS]; /* code indicator (CODE_???) */
        public static double[] L = new double[NFREQ + NEXOBS]; /* observation data carrier-phase (cycle) */
        public static double[] P = new double[NFREQ + NEXOBS]; /* observation data pseudorange (m) */
        public static float[] D = new float[NFREQ + NEXOBS]; /* observation data doppler frequency (Hz) */

        //public static obs[,] OBS = new obs[3,NFREQ];//???????
    };


    public class gtime_class 
    {        /* time struct */
        public Int64 time;        /* time (s) expressed by standard time_t */
        public double sec;         /* fraction of second under 1 s */
    };

    public class obs_data
    {
        const int NFREQ = 3;
        const int NEXOBS = 0;
        
        public static byte sat, rcv; /* satellite/receiver number */
        public static byte[] SNR = new byte[NFREQ + NEXOBS]; /* signal strength (0.25 dBHz) */
        public static byte[] LLI = new byte[NFREQ + NEXOBS]; /* loss of lock indicator */
        public static byte[] code = new byte[NFREQ + NEXOBS]; /* code indicator (CODE_???) */
        public static double[] L = new double[NFREQ + NEXOBS]; /* observation data carrier-phase (cycle) */
        public static double[] P = new double[NFREQ + NEXOBS]; /* observation data pseudorange (m) */
        public static float[] D = new float[NFREQ + NEXOBS]; /* observation data doppler frequency (Hz) */
    }

    public class obs
    {
        public int n, nmax;
        public obs_data data;
    }

}
