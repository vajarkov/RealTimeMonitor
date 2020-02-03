using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;



namespace RealTimeMonitor
{
    public class ChatHub : Hub
    {
        //[DllImport("RTKLib.dll")]
        //static public extern IntPtr rtksvr_t rtksvr;


        public async Task Send(string message, string userName)
        {
            await this.Clients.All.SendAsync("Send", message, userName);
        }


        public async Task UploadStream(IAsyncEnumerable<string> stream)
        {
            await foreach (var item in stream)
            {
                Console.WriteLine(item);
            }
        }
    }
}
