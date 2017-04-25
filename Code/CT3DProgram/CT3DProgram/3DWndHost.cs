using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Interop;
using WPFInterface;

namespace CT3DProgram
{
    class _3DWndHost : HwndHost
    {
        CPlusDLLLoader m_3DInterface = null;
        IntPtr m_Wnd;
        int m_WndWidth = 525;
        int m_WndHeight = 350;
        public IntPtr hWnd
        {
            get { return m_Wnd; }
        }
        public _3DWndHost(CPlusDLLLoader p3DInterface, int nWidth, int nHeight)
        {
            m_3DInterface = p3DInterface;
            m_WndWidth = nWidth;
            m_WndHeight = nHeight;
        }
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            m_Wnd = IntPtr.Zero;

            m_Wnd = CreateWindowEx(0, "static", "",
                                      WS_CHILD | WS_VISIBLE,
                                      0, 0,
                                      m_WndWidth, m_WndHeight,
                                      hwndParent.Handle,
                                      (IntPtr)HOST_ID,
                                      IntPtr.Zero,
                                      0);

            return new HandleRef(this, m_Wnd);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            DestroyWindow(hwnd.Handle);
        }

        public void SetWndPos(int x, int y, int cx, int cy)
        {
            SetWindowPos(m_Wnd, (IntPtr)HWND_TOP, x, y, cx, cy, 0);
            SendMessage(m_Wnd, WM_SIZE, cx, cy);
            //MoveWindow(new IntPtr(0), x, y, cx, cy, false);
        }
        //</SnippetIntPtrProperty>
        //<SnippetBuildWindowCoreHelper>
        //PInvoke declarations
        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Auto)]
        internal static extern IntPtr CreateWindowEx(int dwExStyle,
                                                      string lpszClassName,
                                                      string lpszWindowName,
                                                      int style,
                                                      int x, int y,
                                                      int width, int height,
                                                      IntPtr hwndParent,
                                                      IntPtr hMenu,
                                                      IntPtr hInst,
                                                      [MarshalAs(UnmanagedType.AsAny)] object pvParam);
        //</SnippetBuildWindowCoreHelper>
        //<SnippetWndProcDestroyHelper>
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Auto)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
        //</SnippetWndProcDestroyHelper>
        //<SnippetControlHostConstants>
        internal const int
          WS_CHILD = 0x40000000,
          WS_VISIBLE = 0x10000000,
          LBS_NOTIFY = 0x00000001,
          HOST_ID = 0x00000002,
          SWP_NOSIZE = 0x0001,
          SWP_NOMOVE = 0x1000,
          SWP_FRAMECHANGED = 0x20,
          HWND_TOP = (0),
          WM_SIZE = 0x0005,
          WS_THICKFRAME = 0x00040000,
          GWL_STYLE = (-16);
        //</SnippetControlHostConstants>

        [DllImport("user32.dll", EntryPoint = "GetWindowLong", CharSet = CharSet.Auto)]
        public static extern int GetWindowLong(IntPtr hWnd, int nIndex);
        [DllImport("user32.dll", EntryPoint = "SetWindowLong", CharSet = CharSet.Auto)]
        public static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);
        [DllImport("user32.dll", EntryPoint = "SetParent", CharSet = CharSet.Auto)]
        public static extern void SetParent(IntPtr pChild, IntPtr pParent);

        [DllImport("user32.dll")]
        internal static extern bool SetWindowPos(IntPtr hWnd,
                                                IntPtr hWndInsertAfter,
                                                int X, 
                                                int Y, 
                                                int cx,
                                                int cy,
                                                UInt32 uFlags);

        [DllImport("user32.dll")]
        internal static extern bool MoveWindow(IntPtr hWnd,
                                        int X,
                                        int Y,
                                        int cx,
                                        int cy,
                                        bool bRepaint);

        [DllImport("user32.dll", EntryPoint = "SendMessage", CharSet = CharSet.Auto)]
        internal static extern int SendMessage(IntPtr hwnd,
                                       int msg,
                                       int wParam,
                                       int lParam);
    }
}
