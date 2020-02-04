using Microsoft.AspNetCore.SignalR;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Channels;
using System.Threading.Tasks;

namespace RealTimeMonitor
{
    public class StreamHub : Hub 
    {
        public ChannelReader<int> DelayCounter(int delay)
        {
            var channel = Channel.CreateUnbounded<int>();
            _ = WriteItems(channel.Writer, 20, delay);
            return channel.Reader;
        }

        private async Task WriteItems(ChannelWriter<int> writer, int count, int delay)
        {
            for(var i = 0; i < count;  i++)
            {
                
                if (i % 5 == 0)
                    delay = delay * 2;

                await writer.WriteAsync(i);
                await Task.Delay(delay);
            }

            writer.TryComplete();
        }
    }
}
