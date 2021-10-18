using System;
using System.Windows;
using System.Windows.Input;


namespace BananasEditor
{
    /// <summary>
    /// Interaction logic for StartupWindow.xaml
    /// </summary>
    public partial class StartupWindow : Window
    {
        public StartupWindow()
        {
            InitializeComponent();
        }

        private void window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
                DragMove();
        }

        private void buttonMinimize_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.MainWindow.WindowState = WindowState.Minimized;
        }

        private void buttonMaximize_Click(object sender, RoutedEventArgs e)
        {
            if(WindowState == System.Windows.WindowState.Normal)
            {
                Application.Current.MainWindow.WindowState = WindowState.Maximized;
            }
            else
            {
                Application.Current.MainWindow.WindowState = WindowState.Normal;
            }
        }

        private void buttonExit_Click(object sender, RoutedEventArgs e)
        { 
            Application.Current.Shutdown();
        }

        private void buttonNewScene_Click(object sender, RoutedEventArgs e)
        {
            MainWindow mainWindow = new MainWindow();
            mainWindow.Show();
            this.Close();
        }

        private void buttonOpenScene_Click(object sender, RoutedEventArgs e)
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
                MainWindow mainWindow = new MainWindow();
                mainWindow.Show();
                mainWindow.Title = "Bananas Import/Export " + "[" + fileName + "]";
                mainWindow.RenderScene.OpenScene(fileName);
                this.Close();
            }
        }
    }
}
