﻿using System;
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
        internal static extern IntPtr GetScene();
        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void SceneLoadModels(string fileName);
        #endregion

        IntPtr RenderScene = IntPtr.Zero;

        public Scene()
        {
            CreateScene();
            RenderScene = GetScene();
        }

        public void LoadModels(string fileName)
        {
            SceneLoadModels(fileName);
        }
    }
}
