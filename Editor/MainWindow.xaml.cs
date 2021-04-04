﻿using System;
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


namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Application mainApp;
        Window parentWindow;
        ControlHost windowHost;

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
            // @TODO: Bug - Closing window when using native Run() function does not close application
            windowHost.Run();
        }

    }
}
