using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace BananasEditor
{
    public class EntityViewModel : INotifyPropertyChanged
    {
        #region PInvoke

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineGetModelName();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EntityEngineSetModelName(string name);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineGetMeshCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineGetVerticesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineGetIndicesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineGetMeshName();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineMeshGetVerticesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineMeshGetIndicesCount();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMeshGetMaterialName();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetAmbient();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetDiffuse();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetSpecular();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetEmissive();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern float EntityEngineMaterialGetShininess();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialTextureType();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialTextureFilePath();

        #endregion

        public struct Vec3
        {
            public float X { get; set; }
            public float Y { get; set; }
            public float Z { get; set; }
        }

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
        private float m_shininess;
        private string m_textureType;
        private string m_textureFilePath;

        public event PropertyChangedEventHandler PropertyChanged;

        #region Properties

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

        public float MaterialShininess
        {
            get{ return m_shininess; }
            set{ m_shininess = value; NotifyPropertyChanged(); }
        }

        public string MaterialTextureType
        {
            get{ return m_textureType; }
            set{ m_textureType = value; NotifyPropertyChanged(); }
        }

        public string MaterialTextureFilePath
        {
            get{ return m_textureFilePath; }
            set{ m_textureFilePath = value; NotifyPropertyChanged(); }
        }

        #endregion

        #region Functions

        public string EngineGetModelName()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineGetModelName());
            return result;
        }

        public void EngineSetModelName(string name)
        {
            EntityEngineSetModelName(name);
        }

        public int EngineGetMeshCount()
        {
            return EntityEngineGetMeshCount();
        }

        public int EngineGetVerticesCount()
        {
            return EntityEngineGetVerticesCount();
        }

        public int EngineGetIndicesCount()
        {
            return EntityEngineGetIndicesCount();
        }

        public string EngineGetMeshName()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineGetMeshName());
            return result;
        }

        public int EngineMeshGetVerticesCount()
        {
            return EntityEngineMeshGetVerticesCount();
        }

        public int EngineMeshGetIndicesCount()
        {
            return EntityEngineMeshGetIndicesCount();
        }

        public string EngineMeshGetMaterialName()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMeshGetMaterialName());
            return result;
        }

        public Vec3 EngineMaterialGetAmbient()
        {
            IntPtr ambient = EntityEngineMaterialGetAmbient();
            Vec3 result= (Vec3)Marshal.PtrToStructure(ambient, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetDiffuse()
        {
            IntPtr diffuse = EntityEngineMaterialGetDiffuse();
            Vec3 result= (Vec3)Marshal.PtrToStructure(diffuse, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetSpecular()
        {
            IntPtr specular = EntityEngineMaterialGetSpecular();
            Vec3 result= (Vec3)Marshal.PtrToStructure(specular, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetEmissive()
        {
            IntPtr emissive = EntityEngineMaterialGetEmissive();
            Vec3 result= (Vec3)Marshal.PtrToStructure(emissive, typeof(Vec3));
            return result;
        }

        public float EngineMaterialGetShininess()
        {
            return EntityEngineMaterialGetShininess();
        }

        public string EngineMaterialGetTextureType()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMaterialTextureType());
            return result;
        }

        public string EngineMaterialGetTextureFilePath()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMaterialTextureFilePath());
            return result;
        }

        public void GetModelProperties()
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
            MaterialShininess = EngineMaterialGetShininess();
            MaterialTextureType = EngineMaterialGetTextureType();
            MaterialTextureFilePath = EngineMaterialGetTextureFilePath();
        }

        // This method is called by the Set accessor of each property.  
        // The CallerMemberName attribute that is applied to the optional propertyName  
        // parameter causes the property name of the caller to be substituted as an argument.  
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion
    }
}
