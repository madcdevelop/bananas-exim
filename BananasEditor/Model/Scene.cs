using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Interop;
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

        #endregion

        IntPtr m_renderScene = IntPtr.Zero;

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
    }
}
