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
        internal static extern void EntityEngineMaterialSetAmbient(int mIndex, IntPtr ambient);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetDiffuse(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EntityEngineMaterialSetDiffuse(int mIndex, IntPtr diffuse);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetSpecular(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EntityEngineMaterialSetSpecular(int mIndex, IntPtr specular);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EntityEngineMaterialGetEmissive(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EntityEngineMaterialSetEmissive(int mIndex, IntPtr emissive);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern float EntityEngineMaterialGetShininess(int mIndex);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern float EntityEngineMaterialSetShininess(int mIndex, float shininess);

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
        private string m_meshName = string.Empty;
        private int m_meshVertexCount = 0;
        private int m_meshIndexCount = 0;
        private string m_materialName = string.Empty;

        private Vec3 m_ambient;
        private float m_ambientX;
        private float m_ambientY;
        private float m_ambientZ;

        private Vec3 m_diffuse;
        private float m_diffuseX;
        private float m_diffuseY;
        private float m_diffuseZ;

        private Vec3 m_specular;
        private float m_specularX;
        private float m_specularY;
        private float m_specularZ;

        private Vec3 m_emissive;
        private float m_emissiveX;
        private float m_emissiveY;
        private float m_emissiveZ;

        private float m_shininess;

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
            set
            {
                if (value.X != this.m_ambient.X &&
                    value.Y != this.m_ambient.Y &&
                    value.Z != this.m_ambient.Z)
                {
                    m_ambient = value;
                    EntityEngineMaterialSetAmbient(m_meshIndex, EngineMaterialSetAmbient());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialAmbientX
        {
            get{ return m_ambientX; }
            set
            {
                if (value != this.m_ambientX) 
                {
                    m_ambientX = value;
                    m_ambient.X = value;
                    EntityEngineMaterialSetAmbient(m_meshIndex, EngineMaterialSetAmbient());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialAmbientY
        {
            get{ return m_ambientY; }
            set
            {
                if (value != this.m_ambientY) 
                {
                    m_ambientY = value;
                    m_ambient.Y = value;
                    EntityEngineMaterialSetAmbient(m_meshIndex, EngineMaterialSetAmbient());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialAmbientZ
        {
            get{ return m_ambientZ; }
            set
            {
                if (value != this.m_ambientZ) 
                {
                    m_ambientZ = value;
                    m_ambient.Z = value;
                    EntityEngineMaterialSetAmbient(m_meshIndex, EngineMaterialSetAmbient());
                    NotifyPropertyChanged();
                }
            }
        }

        public Vec3 MaterialDiffuse
        {
            get{ return m_diffuse; }
            set
            {
                if (value.X != this.m_diffuse.X &&
                    value.Y != this.m_diffuse.Y &&
                    value.Z != this.m_diffuse.Z)
                {
                    m_diffuse = value;
                    EntityEngineMaterialSetDiffuse(m_meshIndex, EngineMaterialSetDiffuse());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialDiffuseX
        {
            get{ return m_diffuseX; }
            set
            {
                if (value != this.m_diffuseX) 
                {
                    m_diffuseX = value;
                    m_diffuse.X = value;
                    EntityEngineMaterialSetDiffuse(m_meshIndex, EngineMaterialSetDiffuse());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialDiffuseY
        {
            get{ return m_diffuseY; }
            set
            {
                if (value != this.m_diffuseY) 
                {
                    m_diffuseY = value;
                    m_diffuse.Y = value;
                    EntityEngineMaterialSetDiffuse(m_meshIndex, EngineMaterialSetDiffuse());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialDiffuseZ
        {
            get{ return m_diffuseZ; }
            set
            {
                if (value != this.m_diffuseZ) 
                {
                    m_diffuseZ = value;
                    m_diffuse.Z = value;
                    EntityEngineMaterialSetDiffuse(m_meshIndex, EngineMaterialSetDiffuse());
                    NotifyPropertyChanged();
                }
            }
        }

        public Vec3 MaterialSpecular
        {
            get{ return m_specular; }
            set
            {
                if (value.X != this.m_specular.X &&
                    value.Y != this.m_specular.Y &&
                    value.Z != this.m_specular.Z)
                {
                    m_specular = value;
                    EntityEngineMaterialSetSpecular(m_meshIndex, EngineMaterialSetSpecular());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialSpecularX
        {
            get{ return m_specularX; }
            set
            {
                if (value != this.m_specularX) 
                {
                    m_specularX = value;
                    m_specular.X = value;
                    EntityEngineMaterialSetSpecular(m_meshIndex, EngineMaterialSetSpecular());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialSpecularY
        {
            get{ return m_specularY; }
            set
            {
                if (value != this.m_specularY) 
                {
                    m_specularY = value;
                    m_specular.Y = value;
                    EntityEngineMaterialSetSpecular(m_meshIndex, EngineMaterialSetSpecular());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialSpecularZ
        {
            get{ return m_specularZ; }
            set
            {
                if (value != this.m_specularZ) 
                {
                    m_specularZ = value;
                    m_specular.Z = value;
                    EntityEngineMaterialSetSpecular(m_meshIndex, EngineMaterialSetSpecular());
                    NotifyPropertyChanged();
                }
            }
        }

        public Vec3 MaterialEmissive
        {
            get{ return m_emissive; }
            set
            {
                if (value.X != this.m_emissive.X &&
                    value.Y != this.m_emissive.Y &&
                    value.Z != this.m_emissive.Z)
                {
                    m_emissive = value;
                    EntityEngineMaterialSetEmissive(m_meshIndex, EngineMaterialSetEmissive());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialEmissiveX
        {
            get{ return m_emissiveX; }
            set
            {
                if (value != this.m_emissiveX) 
                {
                    m_emissiveX = value;
                    m_emissive.X = value;
                    EntityEngineMaterialSetEmissive(m_meshIndex, EngineMaterialSetEmissive());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialEmissiveY
        {
            get{ return m_emissiveY; }
            set
            {
                if (value != this.m_emissiveY) 
                {
                    m_emissiveY = value;
                    m_emissive.Y = value;
                    EntityEngineMaterialSetEmissive(m_meshIndex, EngineMaterialSetEmissive());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialEmissiveZ
        {
            get{ return m_emissiveZ; }
            set
            {
                if (value != this.m_emissiveZ) 
                {
                    m_emissiveZ = value;
                    m_emissive.Z = value;
                    EntityEngineMaterialSetEmissive(m_meshIndex, EngineMaterialSetEmissive());
                    NotifyPropertyChanged();
                }
            }
        }

        public float MaterialShininess
        {
            get{ return m_shininess; }
            set
            { 
                if (value != this.m_shininess)
                {
                    m_shininess = value;
                    EntityEngineMaterialSetShininess(m_meshIndex, m_shininess);
                    NotifyPropertyChanged();
                }
            }
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

        public IntPtr EngineMaterialSetAmbient()
        {
            IntPtr result = Marshal.AllocHGlobal(Marshal.SizeOf(m_ambient));
            Marshal.StructureToPtr(m_ambient, result, false);
            return result;
        }

        public Vec3 EngineMaterialGetDiffuse()
        {
            IntPtr diffuse = EntityEngineMaterialGetDiffuse(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(diffuse, typeof(Vec3));
            return result;
        }

        public IntPtr EngineMaterialSetDiffuse()
        {
            IntPtr result = Marshal.AllocHGlobal(Marshal.SizeOf(m_diffuse));
            Marshal.StructureToPtr(m_diffuse, result, false);
            return result;
        }

        public Vec3 EngineMaterialGetSpecular()
        {
            IntPtr specular = EntityEngineMaterialGetSpecular(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(specular, typeof(Vec3));
            return result;
        }

        public IntPtr EngineMaterialSetSpecular()
        {
            IntPtr result = Marshal.AllocHGlobal(Marshal.SizeOf(m_specular));
            Marshal.StructureToPtr(m_specular, result, false);
            return result;
        }

        public Vec3 EngineMaterialGetEmissive()
        {
            IntPtr emissive = EntityEngineMaterialGetEmissive(m_meshIndex);
            Vec3 result= (Vec3)Marshal.PtrToStructure(emissive, typeof(Vec3));
            return result;
        }

        public IntPtr EngineMaterialSetEmissive()
        {
            IntPtr result = Marshal.AllocHGlobal(Marshal.SizeOf(m_emissive));
            Marshal.StructureToPtr(m_emissive, result, false);
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
            MaterialAmbientX = MaterialAmbient.X;
            MaterialAmbientY = MaterialAmbient.Y;
            MaterialAmbientZ = MaterialAmbient.Z;

            MaterialDiffuse = EngineMaterialGetDiffuse();
            MaterialDiffuseX = MaterialDiffuse.X;
            MaterialDiffuseY = MaterialDiffuse.Y;
            MaterialDiffuseZ = MaterialDiffuse.Z;

            MaterialSpecular = EngineMaterialGetSpecular();
            MaterialSpecularX = MaterialSpecular.X;
            MaterialSpecularY = MaterialSpecular.Y;
            MaterialSpecularZ = MaterialSpecular.Z;

            MaterialEmissive = EngineMaterialGetEmissive();
            MaterialEmissiveX = MaterialEmissive.X;
            MaterialEmissiveY = MaterialEmissive.Y;
            MaterialEmissiveZ = MaterialEmissive.Z;

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
