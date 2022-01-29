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

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int SceneEngineGetMeshCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int SceneEngineGetVerticesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int SceneEngineGetIndicesCount();

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

        // Property Grid
        private string m_modelName = String.Empty;
        private int m_meshCount = 0;
        private int m_verticesCount = 0;
        private int m_indicesCount = 0;

        public event PropertyChangedEventHandler PropertyChanged;

        public string ModelName 
        { 
            get
            {
                return m_modelName;
            }
            set
            {
                if (value != this.m_modelName)
                {
                    m_modelName = value;
                    EngineSetModelName(m_modelName);
                    NotifyPropertyChanged();
                }
            }
        }

        public int MeshCount
        {
            get{ return m_meshCount; }
            set
            {
                if (value != this.m_meshCount)
                {
                    m_meshCount = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public int VerticesCount
        {
            get{ return m_verticesCount; }
            set
            {
                if (value != this.m_verticesCount)
                {
                    m_verticesCount = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public int IndicesCount
        {
            get{ return m_indicesCount; }
            set
            {
                if (value != this.m_indicesCount)
                {
                    m_indicesCount = value;
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
            // NOTE(neil): This is fine as no new thread is created for opening a scene.
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

        public string EngineGetModelName()
        {
            string result = Marshal.PtrToStringAnsi(SceneEngineGetModelName());
            return result;
        }

        public void EngineSetModelName(string name)
        {
            SceneEngineSetModelName(name);
        }

        public int EngineGetMeshCount()
        {
            return SceneEngineGetMeshCount();
        }

        public int EngineGetVerticesCount()
        {
            return SceneEngineGetVerticesCount();
        }

        public int EngineGetIndicesCount()
        {
            return SceneEngineGetIndicesCount();
        }

        private void GetModelProperties()
        {
            ModelName = EngineGetModelName();
            MeshCount = EngineGetMeshCount();
            VerticesCount = EngineGetVerticesCount();
            IndicesCount = EngineGetIndicesCount();
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
