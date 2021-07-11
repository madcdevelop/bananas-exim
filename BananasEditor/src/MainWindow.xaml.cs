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


namespace BananasEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Application mainApp;
        Window parentWindow;
        ControlHost windowHost;
        Scene renderScene;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void On_UIReady(object sender, EventArgs e)
        {
            // Init Window
            mainApp = System.Windows.Application.Current;
            parentWindow = mainApp.MainWindow;
            parentWindow.SizeToContent = SizeToContent.WidthAndHeight;

            // Host win32 Window inside WPF
            windowHost = new ControlHost(ControlHostElement.ActualHeight, ControlHostElement.ActualWidth);
            ControlHostElement.Child = windowHost;
            renderScene = new Scene();
            CompositionTarget.Rendering += new EventHandler(Render);
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
                    MessageBox.Show("File select is not an .obj file.");
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
                if(tokens.Length > 1 || tokens[1] == "obj")
                {
                    renderScene.ExportModels(filename);
                }
                else
                {
                    MessageBox.Show("File type is not an .obj file.");
                }
            }
        }

        private void menuExit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

    }
}
