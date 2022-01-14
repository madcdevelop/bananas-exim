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

namespace BananasEditor.Editor
{
    /// <summary>
    /// Interaction logic for PropertyGrid.xaml
    /// </summary>
    public partial class PropertyGrid : UserControl
    {
        public PropertyGrid()
        {
            InitializeComponent();

            Loaded += new RoutedEventHandler(ParentWindow_Loaded);
        }
        
        void ParentWindow_Loaded(object sender, RoutedEventArgs e)
        {
            CreatePropertyGrid();
        }

        private void CreatePropertyGrid()
        {
            // TODO(neil): Add in parent grid to hold title(Property Grid) and kvpGrid
            Grid grid = new Grid();

            grid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(30, 0) });
            grid.RowDefinitions.Add(new RowDefinition() { Height = GridLength.Auto });

            Grid kvpGrid = new Grid();

            CreateLabel(grid, 0, 0, "Property Grid");
            Grid.SetRow(kvpGrid, 1);

            kvpGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(2, GridUnitType.Star) });
            kvpGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });

            for (int i = 0; i < 5; i++)
            {
                kvpGrid.RowDefinitions.Add(new RowDefinition() { Height = GridLength.Auto });
            }

            for (int i = 0; i < 5; i++)
            {
                CreateLabel(kvpGrid, 0, i, "Name");
                CreateTextBox(kvpGrid, 1, i);
            }

            grid.Children.Add(kvpGrid);
            LayoutRoot.Children.Add(grid);
        }

        private void CreateLabel(Grid grid, int col, int row, string content)
        {
            var propertyKey = new Label();
            propertyKey.Content = content; 
            propertyKey.Height = 30.0;
            propertyKey.Margin = new Thickness(5);
            propertyKey.Padding = new Thickness(5);
            propertyKey.VerticalAlignment = VerticalAlignment.Top;
            propertyKey.HorizontalAlignment = HorizontalAlignment.Center;
            Grid.SetColumn(propertyKey, col);
            Grid.SetRow(propertyKey, row);
            grid.Children.Add(propertyKey);
        }

        private void CreateTextBox(Grid grid, int col, int row)
        {
            var propertyValue = new TextBox();
            propertyValue.Height = 30.0;
            propertyValue.Margin = new Thickness(5);
            propertyValue.Padding = new Thickness(5);
            propertyValue.VerticalAlignment = VerticalAlignment.Top;
            propertyValue.HorizontalAlignment = HorizontalAlignment.Center;
            propertyValue.Text = "Placeholder Text";
            Grid.SetColumn(propertyValue, col);
            Grid.SetRow(propertyValue, row);
            grid.Children.Add(propertyValue);
        }
    }
}
