using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace BananasEditor
{
    public class Scene
    {
        #region PInvoke

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
        private EntityViewModel m_entityViewModel;

        public Scene(EntityViewModel entityViewModel)
        {
            m_entityViewModel = entityViewModel;
        }

        public void NewScene()
        {
            if (m_entityViewModel.Meshes.Count > 0)
            {
                m_entityViewModel.ClearProperties();
                m_entityViewModel.Meshes.Clear();
            }
            CreateScene();
            m_renderScene = GetScene();
        }

        public void SaveScene(string fileName)
        {
            SceneSaveScene(fileName);
        }

        public void LoadScene(string fileName)
        {
            m_entityViewModel.Meshes.Clear();
            SceneLoadScene(fileName);
            // NOTE(neil): This is fine as no new thread is created for opening a scene.
            m_entityViewModel.GetModelProperties();
        }

        public void ImportModels(string fileName)
        {
            m_entityViewModel.Meshes.Clear();
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
                    // Required for ObservableCollection, as it is instantiated on the UI Thread initially
                    // https://stackoverflow.com/questions/18331723/this-type-of-collectionview-does-not-support-changes-to-its-sourcecollection-fro
                    App.Current.Dispatcher.Invoke(() => m_entityViewModel.GetModelProperties());
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
    }
}
