using System;
using System.Net.Sockets;
using System.Text;
using RTKFunctions;


namespace TCPStream
{
    public class TCP
    {
        private const int portTCP = 5018;
        private const string serverTCP = "192.168.0.186";

        private TcpClient client = new TcpClient();
        private byte[] data = new byte[1024];
        private DecodeRTCM RTCM = new DecodeRTCM();

        public void Connect(string server, int port)
        {
            //Пока что отладочный код
            
            try
            {
                client.Connect(serverTCP, portTCP);
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


        public unsafe byte[] GetBytes()
        {
            
            NetworkStream stream = client.GetStream();
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
            stream.Close();
            return data;
        } 


        public string GetData()
        {
            
            StringBuilder response = new StringBuilder();
            NetworkStream stream = client.GetStream();
            do
            {
                int bytes = stream.Read(data, 0, data.Length);
                response.Append(Encoding.ASCII.GetString(data, 0, bytes));
            }
            while (stream.DataAvailable);
            stream.Close();
            return response.ToString();
        }


        public void CloseConnection()
        {
            client.Close();
        }

        public bool ConnectionState { get {
                bool value = client.Connected;
                return value;
            } }

    }
}
