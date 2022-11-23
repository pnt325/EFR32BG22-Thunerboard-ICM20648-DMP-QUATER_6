using System;
using System.Collections.Immutable;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;



namespace DemoCube
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        const string STR_BTN_CONNECT = "Connect";
        const string STR_BTN_DISCONNECT = "DisConnect";

        SerialPort? serialPort;
        public MainWindow()
        {
            InitializeComponent();

            cbbPort.DropDownOpened += CbbPort_DropDownOpened;
            btnConnect.Click += BtnConnect_Click;
        }

        private void BtnConnect_Click(object sender, RoutedEventArgs e)
        {
            string btnText = (string)btnConnect.Content;
            if(btnText == STR_BTN_CONNECT)
            {
                serialPort = new SerialPort(cbbPort.Text, 115200);
                try
                {
                    serialPort.Open();
                    if(serialPort.IsOpen)
                    {
                        Thread th = new Thread(() =>
                        {
                            while(true)
                            {
                                try
                                {
                                    string msg = serialPort.ReadLine();
                                    ParseMessage(msg);
                                }
                                catch (Exception ex)
                                {
                                    Debug.WriteLine(ex.Message);
                                    break;
                                }
                            }
                        });
                        th.IsBackground = true;
                        th.Start();
                    }
                    btnConnect.Content = STR_BTN_DISCONNECT;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }

            if(btnText == STR_BTN_DISCONNECT)
            {
                if (serialPort == null)
                {
                    return;
                }

                if(serialPort.IsOpen)
                {
                    serialPort.Close();
                }

                btnConnect.Content = STR_BTN_CONNECT;
            }
        }

        private void ParseMessage(string msg)
        {
            // Message format
            // $,<value1>,<value2>,<value3><newline>
            string[] values = msg.Split(',');
            if(values.Length < 4 && values[0] != "$")
            {
                return;
            }

            double x, y, z;
            values[3] = values[3].Trim('\r');
            try
            {
                x = double.Parse(values[1]);
                y = double.Parse(values[2]);
                z = double.Parse(values[3]);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return; 
            }

            vmMainWindow.AngleX = x;
            vmMainWindow.AngleY = y;
            vmMainWindow.AngleZ = z;
        }

        private void CbbPort_DropDownOpened(object? sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();

            cbbPort.Items.Clear();
            foreach(var port in ports)
            {
                cbbPort.Items.Add(port);
            }
        }
    }
}
