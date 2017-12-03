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
using System.Windows.Forms;
using System.ComponentModel;

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
        ThreadStart UIRefreshRef;
        Thread UIRefreshThread;

        public MainWindow()
        {
            InitializeComponent();
            client = new Client_managed();
        }

        public void StartClient()
        {
            string add="";
            int port = 0;
            int flag;
            this.Dispatcher.Invoke(() =>
            {
                add = TextBox_IP.Text;
                port = Convert.ToInt32(TextBox_Port.Text);
            });
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(add);
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                flag = client.Connect( port,sbyteStr);
            }
             this.Dispatcher.Invoke(() =>
            {
                if (flag == 0)
                {

                Status.Content = "Connected";
                }
                else
                {
                    Status.Content = "Error";
                    ErrMsgRefresh();
                }
            });
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (client.GetStatus() == 0)
            {

            ClientThreadRef = new ThreadStart(StartClient);
            ClientThread = new Thread(ClientThreadRef);
            ClientThread.Start();
                UIRefreshRef = new ThreadStart(ErrMsgRefresh);
                UIRefreshThread = new Thread(UIRefreshRef);
                UIRefreshThread.Start();

            }
            else
            {
                client.Close();
            }
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

        private void ErrMsgRefresh()
        {
            BindingList<string> ErrMsgList = new BindingList<string>();
            BindingSource bs = new BindingSource();
            bs.DataSource = ErrMsgList;



            App.Current.Dispatcher.Invoke((Action)delegate // <--- HERE
            {
                //  _matchObsCollection.Add(match);
                ListBox_ErrMsg.ItemsSource = bs;

            });
                while (true)
                {

                App.Current.Dispatcher.Invoke((Action)delegate // <--- HERE
                {
                    //  _matchObsCollection.Add(match);
                    

                for (int i = ErrMsgList.Count; i < client.errMsgCount(); i++)
                    {
                        ErrMsgList.Add(client.errMsg(i));
                    }
                    if (client.GetStatus() == 1)
                    {
                        Status.Content = "已连接至" + client.GetIP() + ":" + client.GetPort().ToString();
                        Button_S.Content = "断开";
                        Button_Sent.IsEnabled = true;
                    }
                    else
                    {
                        Status.Content = "连接已关闭";
                        Button_S.Content = "连接";
                        Button_Sent.IsEnabled = false;
                        return;
                    }

                    
                });
                    Thread.Sleep(100);
                }

        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            ErrMsgRefresh();
        }
    }
}
