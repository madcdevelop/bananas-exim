using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace BananasEditor
{
    public class Scene : INotifyPropertyChanged
    {
        #region PInvoke

        // PInvoke declarations
        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void CreateScene();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneSaveScene(string fileName);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneLoadScene(string fileName);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr GetScene();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneImportModels(string fileName);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneExportModels(string fileName);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneShutdown();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern string SceneGetMeshName();

        #endregion

        IntPtr m_renderScene = IntPtr.Zero;
        private string m_meshName = "Placeholder Name";
        
        public event PropertyChangedEventHandler PropertyChanged;

        // This method is called by the Set accessor of each property.  
        // The CallerMemberName attribute that is applied to the optional propertyName  
        // parameter causes the property name of the caller to be substituted as an argument.  
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")  
        {  
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }  

        public string MeshName 
        { 
            get
            { 
                return m_meshName; 
            } 
            set
            { 
                if (value != this.m_meshName)
                {
                    m_meshName = value; 
                    NotifyPropertyChanged();
                }
            } 
        }

        public Scene()
        {
        }

        public void NewScene()
        {
            CreateScene();
            m_renderScene = GetScene();
        }

        public void SaveScene(string fileName)
        {
            SceneSaveScene(fileName);
        }

        public void LoadScene(string fileName)
        {
            SceneLoadScene(fileName);
        }

        public void ImportModels(string fileName)
        {
            SceneImportModels(fileName);
        }

        public void ExportModels(string fileName)
        {
            SceneExportModels(fileName);
        }

        public static void Shutdown()
        {
            SceneShutdown();
        }

        public string GetMeshName()
        {
            // TODO(neil): Crashes when trying to retrieve name.
            // Figure out how to convert unmanaged C string to managed C# string
            string result = new string(SceneGetMeshName());
            return result;
        }
    }
}
