using System;

namespace DataExchange
{
    public static class RTK_SVR_DATA
    {
        public static double[] pos = new double[3];
        public static int nbyte = new int();
        public static byte[] nbuff = new byte[1200];
        public static int len = new int();
        public static byte[] tcp_buff = new byte[1024];
    }
}
