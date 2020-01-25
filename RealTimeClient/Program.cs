using System;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR.Client;

namespace RealTimeClient
{
    class Program
    {
        static HubConnection connection;

        static void Main(string[] args)
        {
            connection = new HubConnectionBuilder()
                .WithUrl("https://localhost:44384/chat/")
                .Build();
            connection.Closed += async (error) =>
            {
                await Task.Delay(new Random().Next(0, 5) * 1000);
                await connection.StartAsync();
            };


            connection.On<string, string>("Send", (user, message) =>
            {
                this.Dispatcher.Invoke(() =>
                {
                    var newMessage = $"{user}: {message}";
                    messagesList.Items.Add(newMessage);
                });
            });

            try
            {

            }

        }

        private void Connect() { 
        }
    }
}
