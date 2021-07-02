using System;
using System.Diagnostics;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace BananasEditor
{
    public class ControlHost : HwndHost
    {
        #region PInvoke
        //PInvoke declarations
        [DllImport("user32.dll")]
        internal static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("user32.dll")]
        internal static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
        #endregion

        internal const int
            WS_CHILD = 0x40000000,
            WS_VISIBLE = 0x10000000,
            GWL_STYLE = (-16);

        private readonly int m_hostHeight;
        private readonly int m_hostWidth;
        IntPtr m_hwndChild = IntPtr.Zero;

        BananasEngineDll.Window hostWindow;

        public ControlHost(double height, double width)
        {
            m_hostHeight = (int)height;
            m_hostWidth  = (int)width;
        }

        public int Run()
        {
            return hostWindow.Run();
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            HandleRef href = new HandleRef();

            if(m_hwndChild == IntPtr.Zero)
            {
                hostWindow = new BananasEngineDll.Window(IntPtr.Zero, hwndParent.Handle);
                if(!hostWindow.Init())
                    return href;
                m_hwndChild = hostWindow.WindowHandle;
                Debug.Assert(m_hwndChild != IntPtr.Zero);

                SetWindowLong(m_hwndChild, GWL_STYLE, WS_CHILD | WS_VISIBLE);
                SetParent(m_hwndChild, hwndParent.Handle);

                href = new HandleRef(this, m_hwndChild);
            }

            return href;
        }
        
        protected override void DestroyWindowCore(HandleRef hwnd) 
        {
            DestroyWindow(hwnd.Handle);
        }
    }
}