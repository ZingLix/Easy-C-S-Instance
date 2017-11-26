using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using System.Runtime.InteropServices;

namespace Client_gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Client_managed client;
        ThreadStart ClientThreadRef;
        Thread ClientThread;

        public MainWindow()
        {
            InitializeComponent();
            client = new Client_managed();
        }

        public void StartClient()
        {
            string add="";
            int port = 0;
            this.Dispatcher.Invoke(() =>
            {
                add = TextBox_IP.Text;
                port = Convert.ToInt32(TextBox_Port.Text);
            });
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(add);
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                client.Connect( port,sbyteStr);
            }
            this.Dispatcher.Invoke(() =>
            {
                Status.Content = "Connected";
            });
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ClientThreadRef = new ThreadStart(StartClient);
            ClientThread = new Thread(ClientThreadRef);
            ClientThread.Start();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            string msg = TextBox_Msg.Text;
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(msg);
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                client.SendMsg(sbyteStr,msg.Length);
            }
        }
    }
}
