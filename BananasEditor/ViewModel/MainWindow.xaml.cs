using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
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

        private Application mainApp;
        private Window parentWindow;
        private ControlHost windowHost;
        private Scene renderScene;

        public Scene RenderScene
        {
            get { return renderScene; } set { renderScene = value; }
        }

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

        private void menuNewScene_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("New Scene!");
        }

        private void menuOpenScene_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openFileDlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Bananas Scene|*.bxml",
                Title = "Open Bananas Scene",
            };

            Nullable<bool> result = openFileDlg.ShowDialog();

            if(result == true)
            {
                string fileName = openFileDlg.FileName;
                renderScene.OpenScene(fileName);
            }
        }

        private void menuSaveScene_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog saveFileDlg = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Bananas Scene|*.bxml",
                Title = "Save Bananas Scene",
            };

            Nullable<bool> result = saveFileDlg.ShowDialog();

            if(result == true)
            {
                string fileName = saveFileDlg.FileName;
                renderScene.SaveScene(fileName);
            }
        }

        private void menuImportWavefrontOBJ_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openFileDlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Wavefront|*.obj",
                Title = "Import Wavefront(.obj) File",
            };

            Nullable<bool> result = openFileDlg.ShowDialog();

            if(result == true)
            {
                string filename = openFileDlg.FileName;
                renderScene.LoadModels(filename);
            }
        }

        private void menuExportWavefrontOBJ_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog saveFileDlg = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Wavefront|*.obj",
                Title = "Export Wavefront(.obj) File",
            };

            Nullable<bool> result = saveFileDlg.ShowDialog();

            if(result == true)
            {
                string filename = saveFileDlg.FileName;
                renderScene.ExportModels(filename);
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
