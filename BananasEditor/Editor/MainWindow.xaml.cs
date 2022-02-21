using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;

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

        private Application m_mainApp;
        private Window m_parentWindow;
        private SceneView m_windowHost;
        private Scene m_renderScene;
        private StartupWindow m_startupWindow;
        private EntityViewModel m_entityViewModel;

        private readonly string m_untitled = "Bananas Import/Export " + "[Untitled.bscene]";
        private string m_fileName;

        public string FileName { get { return m_fileName; } set { m_fileName = value; } }

        public Scene RenderScene
        {
            get { return m_renderScene; } set { m_renderScene = value; }
        }

        public MainWindow(StartupWindow startupWindow)
        {
            InitializeComponent();
            m_startupWindow = startupWindow;
            Loaded += On_UIReady;
        }

        private void On_UIReady(object sender, EventArgs e)
        {
            this.Title = m_untitled;
            Loaded -= On_UIReady;

            // Init Window
            m_mainApp = System.Windows.Application.Current;
            m_parentWindow = m_mainApp.MainWindow;
            m_parentWindow.SizeToContent = SizeToContent.WidthAndHeight;

            // Host win32 Window inside WPF
            m_windowHost = new SceneView(ControlHostElement.ActualHeight, ControlHostElement.ActualWidth);
            m_windowHost.MessageHook += new HwndSourceHook(MsgProc);
            ControlHostElement.Child = m_windowHost;

            m_entityViewModel = new EntityViewModel();
            DataContext = m_entityViewModel;

            m_renderScene = new Scene(m_entityViewModel);
            m_renderScene.NewScene();

            m_propertyGrid.LoadPropertyGridUI(m_entityViewModel);
            
            CompositionTarget.Rendering += new EventHandler(Render);
        }

        private IntPtr MsgProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch((Win32Msg)msg)
            {
                case Win32Msg.WM_SIZE:
                {
                    Debug.WriteLine("Resize Window");
                    m_windowHost.Resize();
                }break;
                default:
                    break;
            }
            return IntPtr.Zero;
        }

        private void Render(object sender, EventArgs e)
        {
            m_windowHost.Run();
        }

        private void menuNewScene_Click(object sender, RoutedEventArgs e)
        {
            Scene.Shutdown();
            m_renderScene.NewScene();
            this.Title = m_untitled;
        }

        private void menuOpenScene_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openFileDlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Bananas Scene|*.bscene",
                Title = "Open Bananas Scene",
            };

            Nullable<bool> result = openFileDlg.ShowDialog();
            if(result == true)
            {
                m_fileName = openFileDlg.FileName;
                string[] filePathNoExt = m_fileName.Split(".");
                m_startupWindow.AddRecentScene(filePathNoExt[0]);
                m_renderScene.LoadScene(filePathNoExt[0]);
                this.Title = "Bananas Import/Export " + "[" + m_fileName + "]";
            }
        }

        private void menuSaveScene_Click(object sender, RoutedEventArgs e)
        {
            if (this.Title == m_untitled)
            {
                Microsoft.Win32.SaveFileDialog saveFileDlg = new Microsoft.Win32.SaveFileDialog
                {
                    Filter = "Bananas Scene | *.bscene",
                    Title = "Save Bananas Scene",
                };

                Nullable<bool> result = saveFileDlg.ShowDialog();
                if (result == true)
                {
                    m_fileName = saveFileDlg.FileName;
                    string[] filePathNoExt = m_fileName.Split(".");
                    m_startupWindow.AddRecentScene(filePathNoExt[0]);
                    m_renderScene.SaveScene(filePathNoExt[0]);
                    File.Create(m_fileName);
                    this.Title = "Bananas Import/Export " + "[" + m_fileName + "]";
                }
            }
            else
            {
                string[] filePathNoExt = m_fileName.Split(".");
                m_renderScene.SaveScene(filePathNoExt[0]);
                File.Create(m_fileName);
            }
        }

        private void menuSaveAsScene_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog saveFileDlg = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Bananas Scene | *.bscene",
                Title = "Save Bananas Scene",
            };

            Nullable<bool> result = saveFileDlg.ShowDialog();
            if(result == true)
            {
                m_fileName = saveFileDlg.FileName;
                string[] filePathNoExt = m_fileName.Split(".");
                m_startupWindow.AddRecentScene(filePathNoExt[0]);
                m_renderScene.SaveScene(filePathNoExt[0]);
                File.Create(m_fileName);
                this.Title = "Bananas Import/Export " + "[" + m_fileName + "]";
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
                m_renderScene.ImportModels(filename);
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
                m_renderScene.ExportModels(filename);
            }
        }

        private void menuExit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void window_KeyDown(object sender, KeyEventArgs e)
        {
            IInputElement focusedControl = Keyboard.FocusedElement;
            if (this == focusedControl)
            {
                // Camera Movement
                if (e.Key == Key.W ||
                    e.Key == Key.S ||
                    e.Key == Key.A ||
                    e.Key == Key.D)
                {
                    e.Handled = true;
                    m_windowHost.KeyboardCameraMove();
                }
            }
        }
    }
}
