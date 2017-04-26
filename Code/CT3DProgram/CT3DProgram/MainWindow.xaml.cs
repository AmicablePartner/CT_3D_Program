using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WPFInterface;
using CTShareMemory;
using CT3DProgram;
using System.Diagnostics;
using System.Runtime.InteropServices;
namespace CT3DProgram
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        _3DUserControl m_3DUserControl;
        CPlusDLLLoader m_3DInterface = null;
		ShareMemory m_ShareMemory = null;
        _3DRenderWindow m_3DRenderWnd = null;
        Window_CoorData m_WndData = null;

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool TerminateProcess(IntPtr hProcess, uint uExitCode);

        public MainWindow()
        {
            InitializeComponent();

            Loaded += MainWindow_Loaded;
        }

        void WindowPositionChange(object sender, EventArgs e)
        {
            Point temPoint = BackWnd.PointToScreen(new Point(0, 0));
            m_3DRenderWnd.Owner = this;

            m_3DRenderWnd.Left = temPoint.X;
            m_3DRenderWnd.Top = temPoint.Y; 
        }

        public void ShareMemoryNotifyInfo(String strText, long lSize)
		{
			//System.Diagnostics.Debug.WriteLine(strText);
		}

		private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            m_ShareMemory = new ShareMemory();
            if (m_ShareMemory != null)
            {
                m_ShareMemory.Init(true, "Test_Memory");
                m_ShareMemory.SetCallBackFunc(ShareMemoryNotifyInfo);
                //m_ShareMemory.WriteShareMemory("Test_Infdo", 10);
                m_ShareMemory.OnRun();
            }
            m_3DUserControl = new _3DUserControl(this);
            m_3DInterface = new CPlusDLLLoader();
            m_WndData = new Window_CoorData(this);
			IntPtr hwnd = new WindowInteropHelper(this).Handle;

            m_3DRenderWnd = new _3DRenderWindow(m_3DInterface, this);

            m_3DRenderWnd.Visibility = Visibility.Visible;
            m_3DRenderWnd.Owner = this;
            ChangWindowSize();
            CompositionTarget.Rendering += new EventHandler(WindowPositionChange);

            //FileLoadProgressBar.Visibility = Visibility.Visible;
            // FileLoadProgressBar.Value = 100;
        }

        private void Grid_SizeChanged(object sender, SizeChangedEventArgs e)
        {
			ChangWindowSize();
        }

		private void ChangWindowSize()
		{
			int CurWidth = (int)this.ActualWidth;
			int CurHeight = (int)this.ActualHeight;

            // Menus.Width = CurWidth;
            //BackWnd.Width = CurWidth;
            //BackWnd.Height = CurHeight - Menus.Height;
            if (m_3DRenderWnd != null)
            {
                //m_3DRenderWnd.SetWndPos(0, 0, CurWidth, CurHeight);
            }
            if (m_3DRenderWnd != null)
            {
                m_3DRenderWnd.Height = BackWnd.ActualHeight;
                m_3DRenderWnd.Width = BackWnd.ActualWidth;
                Point CurPos = BackWnd.PointToScreen(new Point(0, 0));
                m_3DRenderWnd.Left = CurPos.X;
                m_3DRenderWnd.Top = CurPos.Y;
            }
            
		}

        private void Grid_Unloaded(object sender, RoutedEventArgs e)
        {
			if (m_ShareMemory != null)
			{
				m_ShareMemory.Close();
			}

            if (m_WndData != null)
            {
                m_WndData.Close();
            }

 			if (m_3DInterface != null)
             {
                 m_3DInterface.DestroyEnginWnd();
             }
            this.Close();

            TerminateProcess(Process.GetCurrentProcess().Handle, 1);
            //Process.GetCurrentProcess().CloseMainWindow();
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            if (m_3DUserControl == null)
            {
                m_3DUserControl = new _3DUserControl(this);
            }
			if (m_3DUserControl != null)
			{
                m_3DUserControl.Show();
			}
		}

		public void LoadStlModule(string strPath)
		{
			string strMsg = ("LOAD STLS ");
			strMsg += strPath;
			if (m_3DInterface != null)
			{
				m_3DInterface.SendCmdMsg(strMsg);
			}
		}
        public void On3DModuleMsg(string strMsg)
        {
            System.Console.WriteLine("有消息传进来!\n" + strMsg);
            //FileLoadProgressBar.Value++;
        }

        private void MenuItem_Click_Data(object sender, RoutedEventArgs e)
        {
            if (m_WndData == null)
            {
                m_WndData = new Window_CoorData(this);
            }
            if (m_WndData != null)
            {
                m_WndData.Show();
            }
        }
        public void WriteDataToShareMemory(string strData, int nSize)
        {
            m_ShareMemory.WriteShareMemory(strData, nSize);
        }

        public void SendWndMsg(String strMsg)
        {
            if (m_3DInterface != null)
			{
				m_3DInterface.SendCmdMsg(strMsg);
			}
        }
    }
}
