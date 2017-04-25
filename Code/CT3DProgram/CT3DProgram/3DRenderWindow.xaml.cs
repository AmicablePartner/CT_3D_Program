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
using System.Windows.Shapes;
using WPFInterface;

namespace CT3DProgram
{
    /// <summary>
    /// _3DRenderWindow.xaml 的交互逻辑
    /// </summary>
    public partial class _3DRenderWindow : Window
    {
        CPlusDLLLoader m_3DInterface = null;
        public IntPtr m_WndHandle;
        MainWindow m_pMainWnd = null;
        public _3DRenderWindow(CPlusDLLLoader p3DInterface, MainWindow pMainWnd)
        {
            InitializeComponent();
            m_3DInterface = p3DInterface;
            m_pMainWnd = pMainWnd;
            this.Loaded += _3DRenderWindow_Loaded;
        }

        private void _3DRenderWindow_Loaded(object sender, RoutedEventArgs e)
        {
            m_WndHandle = new WindowInteropHelper(this).Handle;

            m_3DInterface.CreateEnginWnd(m_WndHandle.ToInt32());
            m_3DInterface.SetCmdMsgCallback(new CSharpCallback(m_pMainWnd.On3DModuleMsg));
        }

        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (m_WndHandle.ToInt32() != 0)
            {
                m_3DInterface.SetEnginWndPos(0, 0, (int)e.NewSize.Width, (int)e.NewSize.Height);
            }
        }
    }
}
