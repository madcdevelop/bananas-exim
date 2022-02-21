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
    /// Interaction logic for PropertyGrid.xaml
    /// </summary>
    public partial class PropertyGrid : UserControl
    {
        public PropertyGrid()
        {
            InitializeComponent();
        }

        public void LoadPropertyGridUI(EntityViewModel entityViewModel)
        {
            List<EntityViewModel> items = new List<EntityViewModel>();
            items.Add(entityViewModel);
            m_entity.ItemsSource = items;
            m_meshes.ItemsSource = items[0].Meshes;
        }

        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape || e.Key == Key.Enter)
            {
                UIElement elementWithFocus = Keyboard.FocusedElement as UIElement;
                if (elementWithFocus is System.Windows.Controls.TextBox tb)
                {
                    if (Keyboard.FocusedElement != null)
                    {
                        Keyboard.FocusedElement.RaiseEvent(new RoutedEventArgs(UIElement.LostFocusEvent));
                        // Clear logical focus
                        FocusManager.SetFocusedElement(FocusManager.GetFocusScope(elementWithFocus), null);
                        // Clear keyboard focus
                        Keyboard.ClearFocus();
                        var mainWindow = Application.Current.MainWindow;
                        Keyboard.Focus(mainWindow);
                    }
                }
            }
        }
    }
}
