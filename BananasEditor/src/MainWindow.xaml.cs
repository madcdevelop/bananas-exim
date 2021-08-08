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
using System.Windows.Interop;
using System.Diagnostics;


namespace BananasEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private enum Win32Msg
        {
            WM_SIZE = 0x0005
        }

        Application mainApp;
        Window parentWindow;
        ControlHost windowHost;
        Scene renderScene;

        public MainWindow()
        {
            InitializeComponent();
            Loaded += On_UIReady;
        }

        private void On_UIReady(object sender, EventArgs e)
        {
            Loaded -= On_UIReady;

            // Init Window
            mainApp = System.Windows.Application.Current;
            parentWindow = mainApp.MainWindow;
            parentWindow.SizeToContent = SizeToContent.WidthAndHeight;

            // Host win32 Window inside WPF
            windowHost = new ControlHost(ControlHostElement.ActualHeight, ControlHostElement.ActualWidth);
            windowHost.MessageHook += new HwndSourceHook(MsgProc);
            ControlHostElement.Child = windowHost;
            renderScene = new Scene();
            CompositionTarget.Rendering += new EventHandler(Render);
        }

        private IntPtr MsgProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch((Win32Msg)msg)
            {
                case Win32Msg.WM_SIZE:
                {
                    Debug.WriteLine("Resize Window");
                    windowHost.Resize();
                }break;
                default:
                    break;
            }
            return IntPtr.Zero;
        }

        private void Render(object sender, EventArgs e)
        {
            windowHost.Run();
        }

        private void menuImportWavefrontOBJ_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openFileDlg = new Microsoft.Win32.OpenFileDialog();
            Nullable<bool> result = openFileDlg.ShowDialog();

            if(result == true)
            {
                string filename = openFileDlg.FileName;
                string[] tokens = filename.Split('.');
                if(tokens[1] == "obj")
                {
                    renderScene.LoadModels(filename);
                }
                else
                {
                    MessageBox.Show("File selected is not a Wavefront (.obj) file.");
                }
            }
        }

        private void menuExportWavefrontOBJ_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog saveFileDlg = new Microsoft.Win32.SaveFileDialog();
            Nullable<bool> result = saveFileDlg.ShowDialog();

            if(result == true)
            {
                string filename = saveFileDlg.FileName;
                string[] tokens = filename.Split('.');
                if(tokens.Length > 1 && tokens[1] == "obj")
                {
                    renderScene.ExportModels(filename);
                }
                else
                {
                    MessageBox.Show("File type is not a Wavefront (.obj) file.");
                }
            }
        }

        private void menuExit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void window_KeyDown(object sender, KeyEventArgs e)
        {
            // Camera Movement
            if(e.Key == Key.W || 
               e.Key == Key.S ||
               e.Key == Key.A ||
               e.Key == Key.D)
            {
                e.Handled = true;
                windowHost.KeyboardCameraMove();
            }
        }
    }
}
