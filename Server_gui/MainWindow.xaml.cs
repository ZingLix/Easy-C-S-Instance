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
        public BindingList<string> msglist=new BindingList<string>();
        public MainWindow()
        {
            InitializeComponent();
            server = new server_managed();
            listBox.ItemsSource = msglist;
            BindingSource bs = new BindingSource();
            bs.DataSource = msglist;

            listBox1.ItemsSource = bs;
            listBox1.DisplayMemberPath = "ToString";


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
                server.Init(sbyteStr, 8888);
                server.Run();
                
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            serverThread.Abort();
            //server.close();
            label1.Content = "Closed";
            
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            for(int i = msglist.Count; i < server.msgCount(); i++)
            {
                msglist.Add( server.msg(i));
                
           //     listBox.Items.Add(msglist[i]);
            }
        }
    }
}
