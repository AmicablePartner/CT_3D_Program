using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using WindowsFormsControlLibrary_Analysis;
using System.Windows.Forms;

namespace CT3DProgram
{
    /// <summary>
    /// Window_Analysis.xaml 的交互逻辑
    /// </summary>
    public partial class Window_Analysis : Window
    {
        private Form_ZedGraph_Analysis m_ZedGraphAnalys = null;
        public Window_Analysis()
        {
            InitializeComponent();
            Loaded += Window_Loaded;

            m_ZedGraphAnalys = new Form_ZedGraph_Analysis();
            if (m_ZedGraphAnalys != null)
            {
                m_ZedGraphAnalys.TopLevel = false;
                m_ZedGraphAnalys.FormBorderStyle = FormBorderStyle.None;
                windowsFormsHost_Analysis.Child = m_ZedGraphAnalys;
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.Width = 900;
            this.Height = 420;
           if (m_ZedGraphAnalys != null)
           {
               m_ZedGraphAnalys.SetWindowTitle("位移数据曲线", "时间(ms)", "位移(mm)");
               m_ZedGraphAnalys.Show();
           }
        }

        public void SetMaxCount(int nMax)
        {
            m_ZedGraphAnalys.SetAnalysisMaxCount(nMax);
        }

        public void ClearCurve()
        {
            m_ZedGraphAnalys.ClearAnalysis();
        }

        public void AddAnalysisPoint(double dValue, int nType)
        {
            m_ZedGraphAnalys.AddAnalysisPoint(dValue, nType);
        }

        private void WindowSize_Change(object sender, SizeChangedEventArgs e)
        {
            if (m_ZedGraphAnalys != null)
            {
                m_ZedGraphAnalys.Width = (int)this.ActualWidth;
                m_ZedGraphAnalys.Height = (int)this.ActualHeight;
                windowsFormsHost_Analysis.Width = (int)this.ActualWidth;
                windowsFormsHost_Analysis.Height = (int)this.ActualHeight;
            }
        }
    }
}
