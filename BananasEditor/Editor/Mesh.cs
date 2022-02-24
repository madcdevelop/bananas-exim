using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace BananasEditor
{
    public class Mesh : INotifyPropertyChanged
    {
        #region PInvoke

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineGetMeshName(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EntityEngineSetMeshName(string name, int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineMeshGetVerticesCount(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineMeshGetIndicesCount(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMeshGetMaterialName(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EntityEngineMeshSetMaterialName(string name, int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetAmbient(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetDiffuse(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetSpecular(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetEmissive(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern float EntityEngineMaterialGetShininess(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineMaterialGetTextureSize(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialTextureType(int mIndex, int textureIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialTextureFilePath(int mIndex, int textureIndex);

        #endregion

        public struct Vec3
        {
            public float X { get; set; }
            public float Y { get; set; }
            public float Z { get; set; }
        }

        public struct Texture
        {
            public string Type { get; set; }
            public string FilePath { get; set; }
        }

        private int m_meshIndex;
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

        public ObservableCollection<Texture> Textures { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        #region Properties
        
        public string MeshName
        {
            get{ return m_meshName; }
            set
            {
                if (value != this.m_meshName)
                {
                    m_meshName = value;
                    EntityEngineSetMeshName(m_meshName, m_meshIndex);
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
                    EntityEngineMeshSetMaterialName(m_materialName, m_meshIndex);
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

        #endregion

        #region Functions

        public Mesh(int meshIndex)
        {
            m_meshIndex = meshIndex;
            Textures = new ObservableCollection<Texture>();
        }

        public string EngineGetMeshName()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineGetMeshName(m_meshIndex));
            return result;
        }

        public int EngineMeshGetVerticesCount()
        {
            return EntityEngineMeshGetVerticesCount(m_meshIndex);
        }

        public int EngineMeshGetIndicesCount()
        {
            return EntityEngineMeshGetIndicesCount(m_meshIndex);
        }

        public string EngineMeshGetMaterialName()
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMeshGetMaterialName(m_meshIndex));
            return result;
        }

        public Vec3 EngineMaterialGetAmbient()
        {
            IntPtr ambient = EntityEngineMaterialGetAmbient(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(ambient, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetDiffuse()
        {
            IntPtr diffuse = EntityEngineMaterialGetDiffuse(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(diffuse, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetSpecular()
        {
            IntPtr specular = EntityEngineMaterialGetSpecular(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(specular, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetEmissive()
        {
            IntPtr emissive = EntityEngineMaterialGetEmissive(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(emissive, typeof(Vec3));
            return result;
        }

        public float EngineMaterialGetShininess()
        {
            return EntityEngineMaterialGetShininess(m_meshIndex);
        }
        
        public int EngineMaterialGetTextureSize()
        {
            int result = EntityEngineMaterialGetTextureSize(m_meshIndex); 
            return result;
        }

        public string EngineMaterialGetTextureType(int textureIndex)
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMaterialTextureType(m_meshIndex, textureIndex));
            return result;
        }

        public string EngineMaterialGetTextureFilePath(int textureIndex)
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMaterialTextureFilePath(m_meshIndex, textureIndex));
            return result;
        }

        public void GetMeshProperties()
        {
            MeshName = EngineGetMeshName();
            MeshVertexCount = EngineMeshGetVerticesCount();
            MeshIndexCount = EngineMeshGetIndicesCount();
            MaterialName = EngineMeshGetMaterialName();
            MaterialAmbient = EngineMaterialGetAmbient();
            MaterialDiffuse = EngineMaterialGetDiffuse();
            MaterialSpecular = EngineMaterialGetSpecular();
            MaterialEmissive = EngineMaterialGetEmissive();
            MaterialShininess = EngineMaterialGetShininess();

            int textureSize = EngineMaterialGetTextureSize();
            for (int textureIndex = 0; textureIndex < textureSize; textureIndex++)
            {
                Texture texture = new Texture();
                texture.Type = EngineMaterialGetTextureType(textureIndex);
                texture.FilePath = EngineMaterialGetTextureFilePath(textureIndex);
                Textures.Add(texture);
            }
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
