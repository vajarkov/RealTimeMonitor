using System.Web.Http;
using Microsoft.AspNet.SignalR;
using RealTimeCPU.Hubs;
using RealTimeCPU.Models;

namespace RealTimeCPU.Api
{
    public class CpuInfoController : ApiController
    {
        public void Post(CpuInfoPostData cpuInfo)
        {
            var context = GlobalHost.ConnectionManager.GetHubContext<CpuInfo>();
            context.Clients.All.cpuInfoMessage(cpuInfo.MachineName, cpuInfo.Processor, cpuInfo.MemUsage, cpuInfo.TotalMemory);
        }
        
    }
}