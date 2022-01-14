using System;
using System.Runtime.InteropServices;

namespace BananasEditor
{
    public class Scene
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
        private string m_meshName = "";

        public string MeshName { get{ return m_meshName; } set{ this.m_meshName = value; } }

        public Scene()
        {
            CreateScene();
            m_renderScene = GetScene();
        }

        public void NewScene()
        {
            SceneShutdown();
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

        public void SetMeshName()
        {
            MeshName = SceneGetMeshName();
        }

    }
}
