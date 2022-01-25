using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Threading;

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
        internal static extern int GetModelLoadState(); 

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneImportModels(string fileName);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneExportModels(string fileName);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneShutdown();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineGetModelName();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneEngineSetModelName(string name);

        #endregion

        private enum ModelLoaded
        {
            NOT_LOADED  = 0,
            FILE_LOADED = 1,
            DATA_LOADING = 2,
            DATA_LOADED = 3
        }

        private Thread importThread;
        private IntPtr m_renderScene = IntPtr.Zero;
        private string m_ModelName = String.Empty;

        public event PropertyChangedEventHandler PropertyChanged;

        public string ModelName 
        { 
            get
            {
                return m_ModelName;
            }
            set
            {
                if (value != this.m_ModelName)
                {
                    m_ModelName = value;
                    EngineSetModelName(m_ModelName);
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
            ModelName = String.Empty;
        }

        public void SaveScene(string fileName)
        {
            SceneSaveScene(fileName);
        }

        public void LoadScene(string fileName)
        {
            SceneLoadScene(fileName);
            // NOTE(neil): This is fine as no new thread is created for 
            // opening a scene.
            GetModelProperties();
        }

        public void ImportModels(string fileName)
        {
            SceneImportModels(fileName);
            importThread = new Thread(new ThreadStart(this.CreateImportThread));
            importThread.IsBackground = true;
            importThread.Start();
        }

        private void CreateImportThread()
        {
            while (true)
            {
                int result = GetModelLoadState();
                if (result >= (int)ModelLoaded.FILE_LOADED)
                {
                    GetModelProperties();
                    break;
                }
                Thread.Sleep(100);
            }
        }

        public void ExportModels(string fileName)
        {
            SceneExportModels(fileName);
        }

        public static void Shutdown()
        {
            SceneShutdown();
        }

        public void EngineGetModelName()
        {
            string result = Marshal.PtrToStringAnsi(SceneEngineGetModelName());
            ModelName = result;
        }

        public void EngineSetModelName(string name)
        {
            SceneEngineSetModelName(name);
        }

        private void GetModelProperties()
        {
            EngineGetModelName();
        }

        // This method is called by the Set accessor of each property.  
        // The CallerMemberName attribute that is applied to the optional propertyName  
        // parameter causes the property name of the caller to be substituted as an argument.  
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
