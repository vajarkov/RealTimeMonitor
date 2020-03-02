using System;
using System.Net.Sockets;
using System.Text;


namespace TCPStream
{
    public class TCP
    {
        private const int portTCP = 5018;
        private const string serverTCP = "192.168.0.186";

        private TcpClient client = new TcpClient();
        

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
                
            }
            catch (Exception e)
            {
                
            }
        }

        public byte[] GetBytes()
        {
            byte[] data = new byte[1024];
            NetworkStream stream = client.GetStream();
            do
            {
                int bytes = stream.Read(data, 0, data.Length);

            }
            while (stream.DataAvailable);
            stream.Close();
            return data;
        }


        public string GetData()
        {
            byte[] data = new byte[1024];
            StringBuilder response = new StringBuilder();
            NetworkStream stream = client.GetStream();
            do
            {
                int bytes = stream.Read(data, 0, data.Length);
                response.Append(data.ToString());
            }
            while (stream.DataAvailable);
            stream.Close();
            return response.ToString();
        }


        public void CloseConnection()
        {
            client.Close();
        }

    }
}
