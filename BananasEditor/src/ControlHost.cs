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
        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool Win32CreateWindow(IntPtr hInstance, IntPtr hwnd);

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr Win32GetWindowHandle();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern int Win32Run();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void Win32Resize();

        [DllImport("BananasEngineDll.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void Win32KeyboardCameraMove();

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

        public ControlHost(double height, double width)
        {
            m_hostHeight = (int)height;
            m_hostWidth  = (int)width;
        }

        public int Run()
        {
            return Win32Run();
        }

        public void Resize()
        {
            Win32Resize();
        }

        public void KeyboardCameraMove()
        {
            Win32KeyboardCameraMove();
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            HandleRef href = new HandleRef();

            if(m_hwndChild == IntPtr.Zero)
            {
                if(!Win32CreateWindow(IntPtr.Zero, hwndParent.Handle))
                    return href;
                m_hwndChild = Win32GetWindowHandle();
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