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
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using System.ComponentModel;

namespace Server_gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public server_managed server;
      //  public BindingList<string> msglist=new BindingList<string>();
        public BindingList<ClientInfo> clientlist = new BindingList<ClientInfo>();
      //  public BindingList<string> msglist = new BindingList<string>();
        ThreadStart UIRefreshRef;
        Thread UIRefresh;
        BindingSource msglistbs;

        public class ClientInfo : INotifyPropertyChanged
        {
            public ClientInfo()
            {
                id = 0;
                msglist = new BindingList<string>();
                _connected = true;
            }
            public BindingList<string> msglist;
            public int id;
            public void setid(int i) { id = i;  }
            public string ip;
            public int port;
            public bool _connected;
            public string name
            {
                set
                {
                    if (value == "1") _connected = true;
                    else _connected = false;
                    PropertyChanged(this, new PropertyChangedEventArgs("name"));
                }
                get
                {
                    if (_connected)
                        return ip + ":" + port.ToString() + " (在线)";
                    else
                        return ip + ":" + port.ToString() + " (离线)";
                }
            }
            public event PropertyChangedEventHandler PropertyChanged = delegate { };
        }

        public MainWindow()
        {
            InitializeComponent();
            server = new server_managed();
            BindingSource bs = new BindingSource();
            bs.DataSource = clientlist;
            
            listBox.ItemsSource = bs;
            listBox.DisplayMemberPath = "name";
            //  listBox.
            msglistbs = new BindingSource();

            listBox1.ItemsSource = msglistbs;
            UIRefreshRef = new ThreadStart(UpdateData);
            UIRefresh = new Thread(UIRefreshRef);
            UIRefresh.Start();
        }

        ThreadStart serverThreadRef ;
        Thread serverThread ;

        private void button_Click(object sender, RoutedEventArgs e)
        {
            string port = textBox.Text;
            System.Text.RegularExpressions.Regex rex = new System.Text.RegularExpressions.Regex(@"^\d+$");
            if (!rex.IsMatch(port))
            {
                MessageBoxResult result = System.Windows.MessageBox.Show("端口非法。", "提示");
                return;
            }
            else
            {
                if (Convert.ToInt32(port) > 65535 || Convert.ToInt32(port) < 0)
                {
                    MessageBoxResult result = System.Windows.MessageBox.Show("端口范围错误。", "提示");
                    return;
                }
            }
            if (server.GetServerStatus() == 0)
            {

            serverThreadRef = new ThreadStart(StartSever);
            serverThread = new Thread(serverThreadRef);
                serverThread.Start();
                this.Closing += Window_Closing;
            label1.Content = "正在运行...";
                button.Content = "停止";
            }
            else
            {
                serverThread.Abort();
                button.Content = "运行";
                server.close();
                label1.Content = "未运行";
            }
        }

        public void StartSever()
        {
            string add = "127.0.0.1";
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(add);
            int port=0;
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                this.Dispatcher.Invoke(() =>
                {
                port = port = Convert.ToInt32(textBox.Text);
                    
                });
                int flag = server.Init(sbyteStr, port);
                if(flag == 0)
                {
                    server.Run();
                    label1.Content = "Running";
                }
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            serverThread.Abort();
            label1.Content = "Closed";
            
        }

        private void UpdateData()
        {
            while (true)
            {
                for (int i = 0; i < server.clientCount(); i++)
                {
                    App.Current.Dispatcher.Invoke((Action)delegate // <--- HERE
                    {

                        //  _matchObsCollection.Add(match);
                        if (i >= clientlist.Count)
                        {

                            clientlist.Add(new ClientInfo());
                            clientlist[i].setid(server.clientID(i));
                            clientlist[i].ip = server.GetIP(i);
                            clientlist[i].port = server.GetPort(i);
                        }
                        for (int j = clientlist[i].msglist.Count; j < server.clientMsgCount(i); j++)
                        {
                                    clientlist[i].msglist.Add(server.msg(i, j));
                        }
                        if (server.GetStatus(i) != 0|| server.GetIP(i) == "204.204.204.204") clientlist[i].name = "0";
                        if (listBox.SelectedIndex==-1) {
                            label_status.Content = "未选中";
                            button_close.IsEnabled = false;
                            button_send.IsEnabled = false;
                        }
                        else if (server.GetStatus(listBox.SelectedIndex) == 0&&server.GetIP(listBox.SelectedIndex)!="204.204.204.204")
                        {

                            label_status.Content = "连接正常";
                            button_close.IsEnabled = true;
                            button_send.IsEnabled = true;
                            label_infoCount.Content = server.GetClientInfoCount(listBox.SelectedIndex);

                        }
                        else
                        {
                            label_status.Content = "连接已关闭";
                            button_close.IsEnabled = false;
                            button_send.IsEnabled = false;
                        label_infoCount.Content = server.GetClientInfoCount(listBox.SelectedIndex);
                        }
                    });
                }
                Thread.Sleep(100);
            }
            
        }


        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            BindingList<string> ErrMsgList = new BindingList<string>();
            for (int i = 0; i < server.errMsgCount(); i++)
            {
                ErrMsgList.Add(server.errMsg(i));
            }
            BindingSource bs = new BindingSource();
            bs.DataSource = ErrMsgList;
            //listBox3.ItemsSource = bs;
            for (int i = 0; i < server.clientCount(); i++)
            {
                if (i >=clientlist.Count)
                {

                    clientlist.Add(new ClientInfo());
                    clientlist[i].setid(i);
                }
                    for (int j = clientlist[i].msglist.Count; j < server.clientMsgCount(i); j++)
                    {
                        clientlist[i].msglist.Add(server.msg(i, j));
                    }
            }
        }



        private void listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            msglistbs.DataSource = clientlist[listBox.SelectedIndex].msglist;
            listBox_SendMsg.Text = "";
            //label_IP.Content = server.GetIP(listBox.SelectedIndex);
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            string s = listBox_SendMsg.Text;
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(s);
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                int len = System.Text.Encoding.Default.GetBytes(s).Length;
                server.SendMsg(listBox.SelectedIndex, sbyteStr,len);
            }
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            server.CloseClient(listBox.SelectedIndex);
            
        }

        private void textBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            server.close();
            serverThread.Abort();
            UIRefresh.Abort();

        }
    }
}
