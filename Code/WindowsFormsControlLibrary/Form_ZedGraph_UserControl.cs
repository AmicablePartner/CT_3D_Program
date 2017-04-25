using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ZedGraph;

namespace WindowsFormsControlLibrary
{
    public partial class Form_ZedGraph_UserControl : Form
    {
        private int m_nListCount;
        Dictionary<String, PointPairList> m_dicXData = new Dictionary<String, PointPairList>();
        Dictionary<String, PointPairList> m_dicYData = new Dictionary<String, PointPairList>();
        Dictionary<String, PointPairList> m_dicZData = new Dictionary<String, PointPairList>();

        private List<Color> m_listColor = new List<Color>();
        private Dictionary<String, int> m_nXDataIndex = new Dictionary<string,int>();
        private Dictionary<String, int> m_nYDataIndex = new Dictionary<string, int>();
        private Dictionary<String, int> m_nZDataIndex = new Dictionary<string, int>();
        public Form_ZedGraph_UserControl()
        {
            InitializeComponent();
            m_listColor.Add(Color.Red);
            m_listColor.Add(Color.Blue);
            m_listColor.Add(Color.Green);
            m_listColor.Add(Color.Yellow);
            m_nListCount = 500;
        }

        public void SetGraphicsMax(int nCount)
        {
            m_nListCount = nCount;
        }

        public void ClearAllCurve()
        {
            m_dicXData.Clear();
            m_dicYData.Clear();
            m_dicZData.Clear();
            m_nXDataIndex.Clear();
            m_nYDataIndex.Clear();
            m_nZDataIndex.Clear();

            zedGraphControl_X.GraphPane.CurveList.Clear(); 
            zedGraphControl_X.Refresh();
            zedGraphControl_Y.GraphPane.CurveList.Clear(); 
            zedGraphControl_Y.Refresh();
            zedGraphControl_Z.GraphPane.CurveList.Clear(); 
            zedGraphControl_Z.Refresh();
        }

        public void SetGraphicsTitile(int nType, String strTitle, String strHorAxes, String strVerAxes)
        {
            GraphPane zPane = null;
            switch (nType)
            {
                case 1:
                    {
                        zPane = zedGraphControl_X.GraphPane;
                        break;
                    }
                case 2:
                    {
                        zPane = zedGraphControl_Y.GraphPane;
                        break;
                    }
                case 3:
                    {
                        zPane = zedGraphControl_Z.GraphPane;
                        break;
                    }
            }
            if (zPane != null)
            {
                zPane.Title.Text = strTitle;
                zPane.XAxis.Title.Text = strHorAxes;
                zPane.YAxis.Title.Text = strVerAxes;
            }
        }

        public void AddGraphicsData(int nType, String strName, double dData, int nGraphicsType)
        {
            switch (nType)
            {
                case 1:
                    {
                        String strCurveName = "X_" + strName;
                        if (!m_dicXData.ContainsKey(strCurveName))
                        {
                            PointPairList ptList = new PointPairList();
                            ptList.Add(ptList.Count, dData);
                            int nIndex = (m_dicXData.Count > m_listColor.Count) ? (m_dicXData.Count % m_listColor.Count) : m_dicXData.Count;
                            zedGraphControl_X.GraphPane.AddCurve(strCurveName, ptList, m_listColor[nIndex], (SymbolType)nGraphicsType);
                            m_dicXData.Add(strCurveName, ptList);
                            m_nXDataIndex.Add(strCurveName, 1);
                        }
                        else
                        {
                            if (m_dicXData[strCurveName].Count > m_nListCount)
                            {
                                m_dicXData[strCurveName].RemoveAt(0);
                            }
                            m_dicXData[strCurveName].Add(m_nXDataIndex[strCurveName]++, dData);
                        }
                        zedGraphControl_X.AxisChange();
                        zedGraphControl_X.Refresh();
                        break;
                    }
                case 2:
                    {
                        String strCurveName = "Y_" + strName;
                        if (!m_dicYData.ContainsKey(strCurveName))
                        {
                            PointPairList ptList = new PointPairList();
                            ptList.Add(ptList.Count, dData);
                            int nIndex = (m_dicYData.Count > m_listColor.Count) ? (m_dicYData.Count % m_listColor.Count) : m_dicYData.Count;
                            zedGraphControl_Y.GraphPane.AddCurve(strCurveName, ptList, m_listColor[nIndex], (SymbolType)nGraphicsType);
                            m_dicYData.Add(strCurveName, ptList);
                            m_nYDataIndex.Add(strCurveName, 1);
                        }
                        else
                        {
                            if (m_dicYData[strCurveName].Count > m_nListCount)
                            {
                                m_dicYData[strCurveName].RemoveAt(0);
                            }
                            m_dicYData[strCurveName].Add(m_nYDataIndex[strCurveName]++, dData);
                        }
                        zedGraphControl_Y.AxisChange();
                        zedGraphControl_Y.Refresh();
                        break;
                    }
                case 3:
                    {
                        String strCurveName = "Z_" + strName;
                        if (!m_dicZData.ContainsKey(strCurveName))
                        {
                            PointPairList ptList = new PointPairList();
                            ptList.Add(ptList.Count, dData);
                            int nIndex = (m_dicZData.Count > m_listColor.Count) ? (m_dicZData.Count % m_listColor.Count) : m_dicZData.Count;
                            zedGraphControl_Z.GraphPane.AddCurve(strCurveName, ptList, m_listColor[nIndex], (SymbolType)nGraphicsType);
                            m_dicZData.Add(strCurveName, ptList);
                            m_nZDataIndex.Add(strCurveName, 1);
                        }
                        else
                        {
                            if (m_dicZData[strCurveName].Count > m_nListCount)
                            {
                                m_dicZData[strCurveName].RemoveAt(0);
                            }
                            m_dicZData[strCurveName].Add(m_nZDataIndex[strCurveName]++, dData);
                        }
                        zedGraphControl_Z.AxisChange();
                        zedGraphControl_Z.Refresh();
                        break;
                    }
            }
        }


        public void AddCurveData(int nType, String strName, System.Collections.ArrayList ayData, int nIndex, int nGraphicType)
        {
            switch (nType)
            {
                case 1:
                    {
                        PointPairList ptList = new PointPairList();
                        foreach (double dValue in ayData)
                        {
                            ptList.Add(ptList.Count, dValue);
                        }
                        zedGraphControl_X.GraphPane.AddCurve(strName, ptList, m_listColor[nIndex], (ZedGraph.SymbolType)nGraphicType);
                        zedGraphControl_X.AxisChange();
                        zedGraphControl_X.Refresh();
                        break;
                    }
                case 2:
                    {
                        PointPairList ptList = new PointPairList();
                        foreach (double dValue in ayData)
                        {
                            ptList.Add(ptList.Count, dValue);
                        }
                        zedGraphControl_Y.GraphPane.AddCurve(strName, ptList, m_listColor[nIndex], (ZedGraph.SymbolType)nGraphicType);
                        zedGraphControl_Y.AxisChange();
                        zedGraphControl_Y.Refresh();
                        break;
                    }
                case 3:
                    {
                        PointPairList ptList = new PointPairList();
                        foreach (double dValue in ayData)
                        {
                            ptList.Add(ptList.Count, dValue);
                        }
                        zedGraphControl_Z.GraphPane.AddCurve(strName, ptList, m_listColor[nIndex], (ZedGraph.SymbolType)nGraphicType);
                        zedGraphControl_Z.AxisChange();
                        zedGraphControl_Z.Refresh();
                        break;
                    }
            }
        }
    }
}
