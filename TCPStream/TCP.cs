using System;
using System.Net.Sockets;
using System.Text;
using System.IO;
using RTKFunctions;
using System.Threading;
using DataExchange;

namespace TCPStream
{
    public class TCP
    {
        private const int portTCP = 5018;
        private const string serverTCP = "192.168.0.186";

        private TcpClient client = new TcpClient();
        private byte[] data = new byte[1024];
        private DecodeRTCM RTCM = new DecodeRTCM();
        private static bool fileWriteStream;
        private NetworkStream stream;
        

        public void Connect(string server, int port)
        {
            //Пока что отладочный код
            
            try
            {
                client.Connect(server, port);
                //client.Connect(server, port);
            }
            catch (SocketException e)
            {
                throw e;
            }
            catch (Exception e)
            {
                throw e;
            }
        }


        public unsafe void GetBytes()
        {
            //RTCM.dataexchange_double();
            //RTK_SVR_DATA.pos[0] = 0; 
            
            stream = client.GetStream();
            do
            {
                int bytes = stream.Read(data, 0, data.Length);
                {
                    fixed(byte* p = data)
                    {
                        RTCM.decoderaw(p, 0);
                    }
                    
                }
                
            }
            while (stream.DataAvailable);
            //stream.Close();*/
            //return data;
        } 


        public string GetData()
        {
            
            StringBuilder response = new StringBuilder();
            stream = client.GetStream();
            do
            {
                int bytes = stream.Read(data, 0, data.Length);
                response.Append(Encoding.ASCII.GetString(data, 0, bytes));
            }
            while (stream.DataAvailable);
            stream.Close();
            return response.ToString();
        }

        public void SaveFile(string path)
        {
            fileWriteStream = true;
            stream = client.GetStream();
            using (FileStream fileStream = new FileStream(path, FileMode.Create))
            { 
                do
                {
                    stream.CopyTo(fileStream);
                }
                while (fileWriteStream && stream.DataAvailable); 
            }
            client.Close();
        }

        public void StopSave()
        {
            fileWriteStream = false;
            Thread.Sleep(1000);
            stream.Close();
            
        }


        public void CloseConnection()
        {
            try
            {
                client.Close();
            }
            catch(Exception ex)
            {
                string str = ex.Message;
            }
        }

        public bool ConnectionState { get {
                bool value = client.Connected;
                return value;
            } }

    }
}
