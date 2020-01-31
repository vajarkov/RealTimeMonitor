using System;
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
    }
}
