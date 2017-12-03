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

        public class ClientInfo
        {
            public ClientInfo()
            {
                id = 0;
                msglist = new BindingList<string>();
              
            }
            public BindingList<string> msglist;
            public int id;
            public void setid(int i) { id = i;  }
            public string name
            {
                get
                {
                    return id.ToString();
                }
            }
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
            serverThreadRef = new ThreadStart(StartSever);
            serverThread = new Thread(serverThreadRef);
            serverThread.Start();
            label1.Content = "Running";
        }

        public void StartSever()
        {
            string add = "127.0.0.1";
            IntPtr intPtrStr = (IntPtr)Marshal.StringToHGlobalAnsi(add);
            unsafe
            {
                sbyte* sbyteStr = (sbyte*)intPtrStr;
                int flag = server.Init(sbyteStr, 8888);
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
                        }
                        for (int j = clientlist[i].msglist.Count; j < server.clientMsgCount(i); j++)
                        {
                                    clientlist[i].msglist.Add(server.msg(i, j));
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
            listBox3.ItemsSource = bs;
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
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            String s = listBox_SendMsg.Text;

            server.SendMsg(listBox.SelectedIndex, listBox_SendMsg.Text);
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            server.CloseClient(listBox.SelectedIndex);
        }
    }
}
