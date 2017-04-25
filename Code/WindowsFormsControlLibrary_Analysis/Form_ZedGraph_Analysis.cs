using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ZedGraph;

namespace WindowsFormsControlLibrary_Analysis
{
    public partial class Form_ZedGraph_Analysis : Form
    {
        private PointPairList m_ptList = new PointPairList();
        private int m_nMaxPoint = 500;
        private int m_nCurIndex = 0;
        public Form_ZedGraph_Analysis()
        {
            InitializeComponent();
        }

        public void SetWindowTitle(String strName, String strXName, String strYName)
        {
            zedGraphControl.GraphPane.Title.Text = strName;
            zedGraphControl.GraphPane.XAxis.Title.Text = strXName;
            zedGraphControl.GraphPane.YAxis.Title.Text = strYName;
        }

        public void SetAnalysisMaxCount(int nMax)
        {
            m_nMaxPoint = nMax;
        }

        public void ClearAnalysis()
        {
            m_ptList.Clear();
            m_nCurIndex = 0;

            zedGraphControl.GraphPane.CurveList.Clear();
            zedGraphControl.Refresh();
        }

        public void AddAnalysisPoint(double dValue, int nGraphicsType)
        {
            if (m_nCurIndex == 0)
            {
                m_ptList.Add(m_nCurIndex, dValue);
                zedGraphControl.GraphPane.AddCurve("", m_ptList, Color.Red, (SymbolType)nGraphicsType);
            }
            else
            {
                if (m_ptList.Count > m_nMaxPoint)
                {
                    m_ptList.RemoveAt(0);
                }
                m_ptList.Add(m_nCurIndex, dValue);
                zedGraphControl.AxisChange();
                zedGraphControl.Refresh();
                zedGraphControl.GraphPane.YAxis.Scale.FontSpec.Angle = 45;
                zedGraphControl.GraphPane.YAxis.MajorGrid.IsVisible = true;
                zedGraphControl.GraphPane.YAxis.Scale.Min = 0;
            }
            ++m_nCurIndex;
        }

        private void WidowSize_Changed(object sender, EventArgs e)
        {
            zedGraphControl.Width = this.Width;
            zedGraphControl.Height = this.Height;
        }
    }
}
