using System;
using System.Collections.ObjectModel;
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

        #endregion

        // Property Grid
        private string m_modelName = String.Empty;
        private int m_meshCount = 0;
        private int m_verticesCount = 0;
        private int m_indicesCount = 0;

        public event PropertyChangedEventHandler PropertyChanged;

        public ObservableCollection<Mesh> Meshes { get; set; }

        #region Properties

        public string ModelName 
        { 
            get { return m_modelName; }
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

        #endregion

        #region Functions

        public EntityViewModel()
        {
            Meshes = new ObservableCollection<Mesh>();
        }

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

        public void GetModelProperties()
        {
            ModelName = EngineGetModelName();
            MeshCount = EngineGetMeshCount();
            VerticesCount = EngineGetVerticesCount();
            IndicesCount = EngineGetIndicesCount();

            for (int i = 0; i < MeshCount; i++)
            {
                Meshes.Add(new Mesh());
            }

            for (int i = 0; i < Meshes.Count; i++)
            {
                Meshes[i].GetMeshProperties(i);
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
