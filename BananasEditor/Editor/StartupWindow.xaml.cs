using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows;
using System.Windows.Input;

namespace BananasEditor
{
    /// <summary>
    /// Interaction logic for StartupWindow.xaml
    /// </summary>
    public partial class StartupWindow : Window
    {
        [Serializable]
        public class Scenes
        {
            public ObservableCollection<string> RecentFiles { get; set; }
        }

        private Scenes m_scenes;
        private string m_recentFilePath = "../../../Content/Scenes/RecentScenes";

        public Scenes RecentScenes { get{ return m_scenes; } set{ m_scenes = value; } }
        public string RecentFilePath { get{ return m_recentFilePath; } set{ m_recentFilePath = value; } }

        public ObservableCollection<string> RecentFiles { get; set; }

        public IFormatter Formatter { get; set; }

        public StartupWindow()
        {
            InitializeComponent();

            m_scenes = new Scenes();
            m_scenes.RecentFiles = new ObservableCollection<string>();
            Formatter = new BinaryFormatter();
            if (File.Exists(m_recentFilePath))
            {
                // TODO(neil): (warning SYSLIB0011) Dangerious to serialize data using binary
                Stream stream = new FileStream(m_recentFilePath, FileMode.Open, FileAccess.Read);
                if (stream.Length > 0)
                    m_scenes = (Scenes)Formatter.Deserialize(stream);
                stream.Close();
            }
            else
            {
                Stream stream = new FileStream(m_recentFilePath, FileMode.Create, FileAccess.Write);
                stream.Close();
            }
            RecentFiles = m_scenes.RecentFiles;

            // NOTE(neil): same as in xaml
            // DataContext="{Binding RelativeSource={RelativeSource Self}}"
            this.DataContext = this;
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

        private void buttonExit_Click(object sender, RoutedEventArgs e)
        { 
            Application.Current.Shutdown();
        }

        private void buttonNewScene_Click(object sender, RoutedEventArgs e)
        {
            MainWindow mainWindow = new MainWindow(this);
            mainWindow.Show();
            this.Close();
        }

        private void buttonOpenScene_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openFileDlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Bananas Scene|*.bscene",
                Title = "Open Bananas Scene",
            };

            Nullable<bool> result = openFileDlg.ShowDialog();
            if(result == true)
            {
                string filePath = openFileDlg.FileName;                
                MainWindow mainWindow = new MainWindow(this);
                mainWindow.FileName = filePath;
                mainWindow.Show();
                mainWindow.Title = "Bananas Import/Export " + "[" + filePath + "]";
                string[] filePathNoExt = filePath.Split(".");

                this.AddRecentScene(filePathNoExt[0]);

                mainWindow.RenderScene.LoadScene(filePathNoExt[0]);
                mainWindow.LoadPropertyGrid();
                this.Close();
            }
        }

        public void AddRecentScene(string path)
        {
            string[] name = path.Split("\\");

            if (!m_scenes.RecentFiles.Contains(name[name.Length-1]))
            {
                if (m_scenes.RecentFiles.Count >= 5)
                    m_scenes.RecentFiles.RemoveAt(0);
                m_scenes.RecentFiles.Add(name[name.Length-1]);
                if (File.Exists(m_recentFilePath))
                {
                    // TODO(neil): (warning SYSLIB0011) Dangerious to serialize data using binary
                    Stream stream = new FileStream(m_recentFilePath, FileMode.Open, FileAccess.Write);
                    Formatter.Serialize(stream, m_scenes);
                    stream.Close();
                }
            }
        }
    }
}
