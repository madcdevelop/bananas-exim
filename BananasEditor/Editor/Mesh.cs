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
        internal static extern int EntityEngineMeshGetVerticesCount(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EntityEngineMeshGetIndicesCount(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMeshGetMaterialName(int mIndex);

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

        #endregion

        #region Functions

        public Mesh()
        {
            Textures = new ObservableCollection<Texture>();
        }

        public string EngineGetMeshName(int mIndex)
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineGetMeshName(mIndex));
            return result;
        }

        public int EngineMeshGetVerticesCount(int mIndex)
        {
            return EntityEngineMeshGetVerticesCount(mIndex);
        }

        public int EngineMeshGetIndicesCount(int mIndex)
        {
            return EntityEngineMeshGetIndicesCount(mIndex);
        }

        public string EngineMeshGetMaterialName(int mIndex)
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMeshGetMaterialName(mIndex));
            return result;
        }

        public Vec3 EngineMaterialGetAmbient(int mIndex)
        {
            IntPtr ambient = EntityEngineMaterialGetAmbient(mIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(ambient, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetDiffuse(int mIndex)
        {
            IntPtr diffuse = EntityEngineMaterialGetDiffuse(mIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(diffuse, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetSpecular(int mIndex)
        {
            IntPtr specular = EntityEngineMaterialGetSpecular(mIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(specular, typeof(Vec3));
            return result;
        }

        public Vec3 EngineMaterialGetEmissive(int mIndex)
        {
            IntPtr emissive = EntityEngineMaterialGetEmissive(mIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(emissive, typeof(Vec3));
            return result;
        }

        public float EngineMaterialGetShininess(int mIndex)
        {
            return EntityEngineMaterialGetShininess(mIndex);
        }
        
        public int EngineMaterialGetTextureSize(int mIndex)
        {
            int result = EntityEngineMaterialGetTextureSize(mIndex); 
            return result;
        }

        public string EngineMaterialGetTextureType(int mIndex, int textureIndex)
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMaterialTextureType(mIndex, textureIndex));
            return result;
        }

        public string EngineMaterialGetTextureFilePath(int mIndex, int textureIndex)
        {
            string result = Marshal.PtrToStringAnsi(EntityEngineMaterialTextureFilePath(mIndex, textureIndex));
            return result;
        }

        public void GetMeshProperties(int mIndex)
        {
            MeshName = EngineGetMeshName(mIndex);
            MeshVertexCount = EngineMeshGetVerticesCount(mIndex);
            MeshIndexCount = EngineMeshGetIndicesCount(mIndex);
            MaterialName = EngineMeshGetMaterialName(mIndex);
            MaterialAmbient = EngineMaterialGetAmbient(mIndex);
            MaterialDiffuse = EngineMaterialGetDiffuse(mIndex);
            MaterialSpecular = EngineMaterialGetSpecular(mIndex);
            MaterialEmissive = EngineMaterialGetEmissive(mIndex);
            MaterialShininess = EngineMaterialGetShininess(mIndex);

            int textureSize = EngineMaterialGetTextureSize(mIndex);
            for (int textureIndex = 0; textureIndex < textureSize; textureIndex++)
            {
                Texture texture = new Texture();
                texture.Type = EngineMaterialGetTextureType(mIndex, textureIndex);
                texture.FilePath = EngineMaterialGetTextureFilePath(mIndex, textureIndex);
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
