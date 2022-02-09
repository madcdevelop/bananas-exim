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

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineGetMeshName();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int SceneEngineMeshGetVerticesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int SceneEngineMeshGetIndicesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineMeshGetMaterialName();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineMaterialGetAmbient();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineMaterialGetDiffuse();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineMaterialGetSpecular();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr SceneEngineMaterialGetEmissive();

        #endregion

        private enum ModelLoaded
        {
            NOT_LOADED  = 0,
            FILE_LOADED = 1,
            DATA_LOADING = 2,
            DATA_LOADED = 3
        }

        public struct Vec3
        {
            public float X { get; set; }
            public float Y { get; set; }
            public float Z { get; set; }
        }

        private Thread importThread;
        private IntPtr m_renderScene = IntPtr.Zero;

        // Property Grid
        private string m_modelName = String.Empty;
        private int m_meshCount = 0;
        private int m_verticesCount = 0;
        private int m_indicesCount = 0;

        private string m_meshName = String.Empty;
        private int m_meshVertexCount = 0;
        private int m_meshIndexCount = 0;
        private string m_materialName = String.Empty;

        private Vec3 m_ambient;
        private Vec3 m_diffuse;
        private Vec3 m_specular;
        private Vec3 m_emissive;

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

        public string MeshName
        {
            get{ return m_meshName; }
            set
            {
                if (value != this.m_meshName)
                {
                    m_meshName = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public int MeshVertexCount
        {
            get{ return m_meshVertexCount; }
            set
            {
                if (value != this.m_meshVertexCount)
                {
                    m_meshVertexCount = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public int MeshIndexCount 
        {
            get{ return m_meshIndexCount; }
            set
            {
                if (value != this.m_meshIndexCount)
                {
                    m_meshIndexCount = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public string MaterialName
        {
            get{ return m_materialName; }
            set
            {
                if (value != this.m_materialName)
                {
                    m_materialName = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public Vec3 MaterialAmbient 
        {
            get{ return m_ambient; }
            set{ m_ambient = value; NotifyPropertyChanged(); }
        }

        public Vec3 MaterialDiffuse 
        {
            get{ return m_diffuse; }
            set{ m_diffuse = value; NotifyPropertyChanged(); }
        }

        public Vec3 MaterialSpecular 
        {
            get{ return m_specular; }
            set{ m_specular = value; NotifyPropertyChanged(); }
        }

        public Vec3 MaterialEmissive 
        {
            get{ return m_emissive; }
            set{ m_emissive = value; NotifyPropertyChanged(); }
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

        public string EngineGetMeshName()
        {
            string result = Marshal.PtrToStringAnsi(SceneEngineGetMeshName());
            return result;
        }

        public int EngineMeshGetVerticesCount()
        {
            return SceneEngineMeshGetVerticesCount();
        }

        public int EngineMeshGetIndicesCount()
        {
            return SceneEngineMeshGetIndicesCount();
        }

        public string EngineMeshGetMaterialName()
        {
            string result = Marshal.PtrToStringAnsi(SceneEngineMeshGetMaterialName());
            return result;
        }

        public Vec3 EngineMaterialGetAmbient()
        {
            IntPtr ambient = SceneEngineMaterialGetAmbient();
            Vec3 result= (Vec3)Marshal.PtrToStructure(ambient, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetDiffuse()
        {
            IntPtr diffuse = SceneEngineMaterialGetDiffuse();
            Vec3 result= (Vec3)Marshal.PtrToStructure(diffuse, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetSpecular()
        {
            IntPtr specular = SceneEngineMaterialGetSpecular();
            Vec3 result= (Vec3)Marshal.PtrToStructure(specular, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetEmissive()
        {
            IntPtr emissive = SceneEngineMaterialGetEmissive();
            Vec3 result= (Vec3)Marshal.PtrToStructure(emissive, typeof(Vec3));
            return result;
        }

        private void GetModelProperties()
        {
            ModelName = EngineGetModelName();
            MeshCount = EngineGetMeshCount();
            VerticesCount = EngineGetVerticesCount();
            IndicesCount = EngineGetIndicesCount();
            MeshName = EngineGetMeshName();
            MeshVertexCount = EngineMeshGetVerticesCount();
            MeshIndexCount = EngineMeshGetIndicesCount();
            MaterialName = EngineMeshGetMaterialName();
            MaterialAmbient = EngineMaterialGetAmbient();
            MaterialDiffuse = EngineMaterialGetDiffuse();
            MaterialSpecular = EngineMaterialGetSpecular();
            MaterialEmissive = EngineMaterialGetEmissive();
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
