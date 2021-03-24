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

using CLI;

namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            // Subscribe to an event. Don't need to define in xaml
            MainGrid.MouseUp   += new MouseButtonEventHandler(MainGrid_MouseUp);
            MainGrid.MouseDown += new MouseButtonEventHandler(MainGrid_MouseDown);

            // test run
            Entity e = new Entity("The Wallman", 20, 35);
            e.Move(5, -10);
            Console.WriteLine(e.XPosition + " " + e.YPosition);
            Console.Read();

            // Open win32 API Window (opengl context)
        }

        private void MainGrid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            MessageBox.Show("You clicked me at " + e.GetPosition(this).ToString());
        }

        private void MainGrid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            MessageBox.Show("Mouse button down was pressed");
        }
    }
}
