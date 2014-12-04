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

namespace Fangtian
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        bool bAbort = false;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void _Start_Click(object sender, RoutedEventArgs e)
        {
            bAbort = false;
            Thread oThread = new Thread(new ThreadStart(ThreadProc));
            oThread.Start();

            _Start.IsEnabled = false;
            _Stop.IsEnabled = true;
        }

        private void _Stop_Click(object sender, RoutedEventArgs e)
        {
            bAbort = true;

            _Start.IsEnabled = true;
            _Stop.IsEnabled = false;
        }

        void ThreadProc()
        {
            while (!bAbort)
            {
                // check serial port 
            }
        }
    }
}
