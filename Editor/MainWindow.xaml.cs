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

            // test run
            Entity e = new Entity("The Wallman", 20, 35);
            entityText1.Text = "First Position: " + e.XPosition + " " + e.YPosition;
            entityTextMove.Text = "Entity Moved!";
            e.Move(5, -10);
            entityText2.Text = "Next Position: " + e.XPosition + " " + e.YPosition;

            // Open win32 API Window (opengl context)
        }

    }
}
