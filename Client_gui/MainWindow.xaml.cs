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
using System.Text.RegularExpressions;

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
            string port = "";
            int flag;
            this.Dispatcher.Invoke(() =>
            {
                add = TextBox_IP.Text;
                port = TextBox_Port.Text;
            });
            Regex rx = new Regex(@"((?:(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d))))");
            if (!rx.IsMatch(add))
            {
                MessageBoxResult result = System.Windows.MessageBox.Show("IP 地址非法。", "提示");
                return;
            }
            System.Text.RegularExpressions.Regex rex =new System.Text.RegularExpressions.Regex(@"^\d+$");
            if (!rex.IsMatch(port))
            {
                MessageBoxResult result = System.Windows.MessageBox.Show("端口非法。", "提示");
                return;
            }
            
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(add);
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                flag = client.Connect( Convert.ToInt32(port),sbyteStr);
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
                //    ErrMsgRefresh();
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
                int len=System.Text.Encoding.Default.GetBytes(msg).Length;
                client.SendMsg(sbyteStr,len);
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

                    
                label_AllInfo.Content = client.GetAllInfoCount();
                label_CurInfo.Content = client.GetCurrentInfoCount();
                });
                    Thread.Sleep(100);
                }

        }


    }
}

