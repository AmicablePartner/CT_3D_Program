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
using WindowsFormsControlLibrary;
using System.Windows.Forms;

namespace CT3DProgram
{
    /// <summary>
    /// Window_Graphic.xaml 的交互逻辑
    /// </summary>
    public partial class Window_Graphic : Window
    {
        Form_ZedGraph_UserControl m_ZedGraph = null;
        public Window_Graphic()
        {
            InitializeComponent();
            Loaded += Window_Loaded;

            m_ZedGraph = new Form_ZedGraph_UserControl();
            if (m_ZedGraph != null)
            {
                m_ZedGraph.TopLevel = false;
                m_ZedGraph.FormBorderStyle = FormBorderStyle.None;
                windowsFormsHost_Graphic.Child = m_ZedGraph;
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_ZedGraph != null)
            {
                m_ZedGraph.SetGraphicsTitile(1, "X坐标数据", "时间（ms）", "压力值(KG)");
                m_ZedGraph.SetGraphicsTitile(2, "Y坐标数据", "时间（ms）", "压力值(KG)");
                m_ZedGraph.SetGraphicsTitile(3, "Z坐标数据", "时间（ms）", "压力值(KG)");
                m_ZedGraph.Show();
            }
            this.Width = 835;
            this.Height = 675;
        }

        public void ClearGraphics()
        {
            m_ZedGraph.ClearAllCurve();
        }

        public void SetMaxGrapihcCount(int nCount)
        {
            m_ZedGraph.SetGraphicsMax(nCount);
        }

        public void AddCurveData(int nType, String strName, System.Collections.ArrayList ayData, int nIndex, int nGraphicType)
        {
            m_ZedGraph.AddCurveData(nType, strName, ayData, nIndex, nGraphicType);
        }

        public void AddGraphicsData(int nType, String strName, double dData, int nGraphicsType)
        {
            m_ZedGraph.AddGraphicsData(nType, strName, dData, nGraphicsType);
        }

        private void WindowSize_chang(object sender, SizeChangedEventArgs e)
        {
            if (m_ZedGraph != null)
            {
                m_ZedGraph.Width = (int)this.ActualWidth;
                m_ZedGraph.Height = (int)this.ActualHeight;
                windowsFormsHost_Graphic.Width = (int)this.ActualWidth;
                windowsFormsHost_Graphic.Height = (int)this.ActualHeight;
            }
        }
    }
}
