using System;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.AspNetCore.SignalR.Client;


namespace WPFClientRealTimeMonitor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //private readonly object messagesList;
        HubConnection connection;
        public MainWindow()
        {
            
            InitializeComponent();

            connection = new HubConnectionBuilder()
                .WithUrl("http://localhost:44384/chat")
                .Build();

            connection.Closed += async (error) =>
            {
                await Task.Delay(new Random().Next(0, 5) * 1000);
                await connection.StartAsync();
            };
        }

        private async void btnConnect_Click(object sender, RoutedEventArgs e)
        {
            /*connection.On<string, string>("Send", (message, user) =>
            {
                this.Dispatcher.Invoke(() =>
                {
                    var newMessage = $"{user}: {message}";
                    //messagesList.Items.Add(newMessage);
                });
            });
            */
            try
            {
                await connection.StartAsync();
                //messagesList.Items.Add("Connection started");
                btnConnect.IsEnabled = false;
                btnSend.IsEnabled = true;
                MessageBox.Show("Соединение сервером");
            }
            catch (Exception ex)
            {Message);
                throw;
            }
        }

        private async void btnSend_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                await connection.InvokeAsync("Send",
                    tbMessage.Text, tbUserName.Text);
                MessageBox.Show("Сообщение: " + tbMessage.Text + " от пользователя " + tbUserName.Text);
            }
            catch (Exception ex)
            {
                throw;
                //messagesList.Items.Add(ex.Message);
            }
        }
    }
}
