using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Net;
using Newtonsoft.Json;
using System.Configuration;
using System.Management;

namespace CpuInfoClient
{
    class Program
    {
        static bool _running = true;
        static PerformanceCounter _cpuCounter, _memUsageCounter;

        static void Main(string[] args)
        {
            Thread pollingThread = null;
            Console.WriteLine("CPU Info Client: Reporting your CPU usage today...");

            try
            {
                _cpuCounter = new PerformanceCounter();
                _cpuCounter.CategoryName = "Processor";
                _cpuCounter.CounterName = "% Processor Time";
                _cpuCounter.InstanceName = "_Total";
                _memUsageCounter = new PerformanceCounter("Memory", "Available Kbytes");
                pollingThread = new Thread(new ParameterizedThreadStart(RunPollingThread));
                pollingThread.Start();

                Console.WriteLine("Press any key to exit.");
                Console.ReadKey();

                _running = false;
                pollingThread.Join(5000);
            }
            catch (Exception ex)
            {
                pollingThread.Abort();
                throw;
            }

        }

        static void RunPollingThread(object data)
        {
            DateTime lastPollTime = DateTime.MinValue;
            Console.WriteLine("Start polling...");

            while (_running)
            {
                if ((DateTime.Now - lastPollTime).TotalMilliseconds >= 1000)
                {
                    double cpuTime;
                    ulong memUsage, totalMemory;

                    GetMetrics(out cpuTime, out memUsage, out totalMemory);

                    var postData = new
                    {
                        MachineName = System.Environment.MachineName,
                        Processor = cpuTime,
                        MemUsage = memUsage,
                        TotalMemory = totalMemory
                    };

                    var json = JsonConvert.SerializeObject(postData);
                    var serverUrl = new Uri(ConfigurationManager.AppSettings["ServerUrl"]);
                    var client = new WebClient();

                    client.Headers.Add("Content-Type", "application/json");
                    client.UploadString(serverUrl, json);
                    lastPollTime = DateTime.Now;
                }
                else
                {
                    Thread.Sleep(10);
                }
            }
        }

        static void GetMetrics(out double processorTime, out ulong memUsage, out ulong totalMemory)
        {
            processorTime = (double)_cpuCounter.NextValue();
            memUsage = (ulong)_memUsageCounter.NextValue();
            totalMemory = 0; // Get total memory from WMI 

            ObjectQuery memQuery = new ObjectQuery("SELECT * FROM CIM_OperatingSystem");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(memQuery);
            foreach (ManagementObject item in searcher.Get())
            {
                totalMemory = (ulong)item["TotalVisibleMemorySize"];
            }
        }
    }
}
