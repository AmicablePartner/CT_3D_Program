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
using System.Collections;
using System.IO;

namespace CT3DProgram
{

    public struct Data_Info
    {
        public String m_strName;
        public ArrayList m_ayXData;
        public ArrayList m_ayYData;
        public ArrayList m_ayZData;

        public Data_Info(String strName, ArrayList listX = null, ArrayList listY = null, ArrayList listZ = null)
        {
           if (listX != null)
           {
               m_ayXData = listX;
           }
           else
           {
               m_ayXData = new ArrayList();
           }

            if (listY == null)
            {
                m_ayYData = new ArrayList();
            }
            else
            {
                m_ayYData = listY;
            }

            if (listZ == null)
            {
                m_ayZData = new ArrayList();
            }
            else
            {
                m_ayZData = listZ;
            }
            m_strName = strName;
        }
    }

    /// <summary>
    /// Window_CoorData.xaml 的交互逻辑
    /// </summary>
    public partial class Window_CoorData : Window
    {
        public MainWindow m_MainWindows { get; set; }
        private List<Data_Info> m_ayLists = new List<Data_Info>();
        private ArrayList m_ayDirs = new ArrayList();
        private Window_Analysis m_WndAnalysis = new Window_Analysis();

        int m_nMaxSize = 0;
        int m_nCurIndex = 0;
        private int m_nDataType = 0;

        private System.Windows.Forms.Timer m_Timer = null;
        private int m_nSimState = 0;
        public int m_nTimer = 100;
        public int m_nCurGraphicType = 0;
        public Window_Graphic m_wndGraphic = new Window_Graphic();

        public int m_nSimStep = 1;

        public Window_CoorData(MainWindow mainWnd)
        {
            InitializeComponent();
            button_begin.IsEnabled = false;
            button_pause.IsEnabled = false;
            button_end.IsEnabled = false;
            button_data.IsEnabled = false;
            m_MainWindows = mainWnd;

            comboBox_List.Items.Add("正方形");
            comboBox_List.Items.Add("菱形");
            comboBox_List.Items.Add("三角形");
            comboBox_List.Items.Add("圆形");
            comboBox_List.Items.Add("叉号");
            comboBox_List.Items.Add("加号");
            comboBox_List.Items.Add("星形");
            comboBox_List.Items.Add("倒三角");
            comboBox_List.Items.Add("横线");
            comboBox_List.Items.Add("竖线");
            comboBox_List.Items.Add("自定义");
            comboBox_List.Items.Add("默认");
            comboBox_List.Items.Add("无");
            textBox_step.Text = "1";

            comboBox_List.SelectedIndex = 12;
            Loaded += Window_Loaded;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            textBox_PlayNum.Text = "100";
            textBox_GraphicNum.Text = "1000";
        }       

        private void button_load_Click(object sender, RoutedEventArgs e)
        {
            if (!m_wndGraphic.IsVisible)
            {
                m_wndGraphic.Show();
            }

            if (!m_WndAnalysis.IsVisible)
            {
                m_WndAnalysis.Show();
            }
            if (listBox_Data.SelectedIndex == -1)
            {
                return;
            }
            button_begin.IsEnabled = true;

            String strPath = listBox_Data.SelectedValue.ToString();
            m_ayLists.Clear();

            if (m_nDataType == 3)
            {
                Data_Info data = new Data_Info(strPath.Substring(strPath.LastIndexOf("\\") + 1));
                int nSize = GetFileDatas(strPath + "\\X.csv", data.m_ayXData);
                m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                nSize = GetFileDatas(strPath + "\\Y.csv", data.m_ayYData);
                m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                nSize = GetFileDatas(strPath + "\\Z.csv", data.m_ayZData);
                m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 

                m_ayLists.Add(data);
            }
            else if (m_nDataType == 1)
            {
                foreach (string dir in Directory.GetDirectories(strPath))
                {
                    Data_Info data = new Data_Info(dir.Substring(dir.LastIndexOf("\\") + 1));
                    int nSize = GetFileDatas(dir + "\\X.csv", data.m_ayXData);
                    m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                    nSize = GetFileDatas(dir + "\\Y.csv", data.m_ayYData);
                    m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                    nSize = GetFileDatas(dir + "\\Z.csv", data.m_ayZData);
                    m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                    m_ayLists.Add(data);
                }
            }
            else if (m_nDataType == 2)
            {
                foreach (string dir in m_ayDirs)
                {
                    if (dir.IndexOf(strPath) != -1)
                    {
                        String strName = dir.Substring(0, dir.IndexOf(strPath) - 1);

                        Data_Info data = new Data_Info(strName.Substring(strName.LastIndexOf("\\") + 1));
                        int nSize = GetFileDatas(dir + "\\X.csv", data.m_ayXData);
                        m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                        nSize = GetFileDatas(dir + "\\Y.csv", data.m_ayYData);
                        m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                        nSize = GetFileDatas(dir + "\\Z.csv", data.m_ayZData);
                        m_nMaxSize = m_nMaxSize < nSize ? nSize : m_nMaxSize; 
                        m_ayLists.Add(data);
                    }
                }
            }


           
            ShowNotifyInfo();
            slider_process.Minimum = 0;
            slider_process.Maximum = m_nMaxSize;

            m_wndGraphic.ClearGraphics();
            m_WndAnalysis.ClearCurve();
        }

        private int GetFileDatas(String strFile, ArrayList ayList)
        {
            StreamReader sr = new StreamReader(strFile, Encoding.Default);
            String line;
            while ((line = sr.ReadLine()) != null)
            {
                ayList.Add(Convert.ToDouble(line));
            }
            return ayList.Count;
        }

        private void button_begin_Click(object sender, RoutedEventArgs e)
        {
            m_nCurIndex = 0;
            button_begin.IsEnabled = false;
            button_pause.IsEnabled = true;
            button_end.IsEnabled = true;
            textBox_GraphicNum.IsEnabled = false;
            textBox_PlayNum.IsEnabled = false;
            button_jump.IsEnabled = false;
            textBox_input.IsEnabled = false;
            m_nCurGraphicType = comboBox_List.SelectedIndex;
            slider_process.IsEnabled = false;
            m_wndGraphic.SetMaxGrapihcCount(Convert.ToInt32(textBox_GraphicNum.Text.ToString()));
            m_WndAnalysis.SetMaxCount(Convert.ToInt32(textBox_GraphicNum.Text.ToString()));
            if (m_nSimState == 0)
            {
                m_nSimState = 1;
                m_Timer = new System.Windows.Forms.Timer();
                m_Timer.Interval = 1;
                m_Timer.Tick += new EventHandler(DealTimerInfo);
                m_Timer.Enabled = true;
            }
            m_nSimStep = Convert.ToInt32(textBox_step.Text.ToString());
            m_nTimer = (int)(1000 / Convert.ToInt32(textBox_PlayNum.Text.ToString()));
            m_Timer.Start();
            m_nSimState = 1;
        }

        public void DealTimerInfo(object sender, EventArgs e)
        {
            if (m_nSimState == 1)
            {
                m_nCurIndex += m_nSimStep;
                SetCurDataByIndex();
            }
            System.Threading.Thread.Sleep(m_nTimer);

            if (m_nCurIndex > m_nMaxSize)
            {
                button_end_Click(null, null);
            }
        }

        private void button_pause_Click(object sender, RoutedEventArgs e)
        {
            button_begin.IsEnabled = true;
            button_pause.IsEnabled = false;
            m_nSimState = 2;

            textBox_GraphicNum.IsEnabled = true;
            textBox_PlayNum.IsEnabled = true;
        }

        private void button_end_Click(object sender, RoutedEventArgs e)
        {
            button_end.IsEnabled = false;
            button_pause.IsEnabled = false;
            button_begin.IsEnabled = true;
            //slider_process.IsEnabled = true;
            button_jump.IsEnabled = true;
            textBox_input.IsEnabled = true;
            m_nSimState = 0;
            m_Timer.Stop();
            m_Timer.Dispose();
            m_Timer = null;


            textBox_GraphicNum.IsEnabled = true;
            textBox_PlayNum.IsEnabled = true;
        }

        private void button_close_Click(object sender, RoutedEventArgs e)
        {
            //this.Close();
            m_wndGraphic.ClearGraphics();
            m_WndAnalysis.ClearCurve();
        }

        private void button_jump_Click(object sender, RoutedEventArgs e)
        {
            m_nCurIndex = Convert.ToInt32(textBox_input.Text.ToString());
            //SetCurDataByIndex();
            int nCurGraphicType = comboBox_List.SelectedIndex;
            int nBegin = 0;
            int nMax = Convert.ToInt32(textBox_GraphicNum.Text.ToString());
            if (m_nCurIndex - nMax > 0)
            {
                nBegin = m_nCurIndex - nMax - 1;
            } 
            
            m_wndGraphic.ClearGraphics();

            ArrayList ayXList = new ArrayList();
            ArrayList ayYList = new ArrayList();
            ArrayList ayZList = new ArrayList();
            int nIndex = 0;
            foreach (object obj in m_ayLists)
            {
                Data_Info pData = (Data_Info)obj;
                for (int i = nBegin; i <= m_nCurIndex; ++i)
                {
                    if (i < pData.m_ayXData.Count)
                    {
                        ayXList.Add(pData.m_ayXData[i]);
                    }
                    if (i < pData.m_ayYData.Count)
                    {
                        ayYList.Add(pData.m_ayYData[i]); 
                    }
                    if (i < pData.m_ayZData.Count)
                    {
                        ayZList.Add(pData.m_ayZData[i]);
                    }
                }
                m_wndGraphic.AddCurveData(1, pData.m_strName, ayXList, nIndex, nCurGraphicType);
                m_wndGraphic.AddCurveData(2, pData.m_strName, ayYList, nIndex, nCurGraphicType);
                m_wndGraphic.AddCurveData(3, pData.m_strName, ayZList, nIndex, nCurGraphicType);
                ++nIndex;

                ShowNotifyInfo();
            }
        }

        private void ShowNotifyInfo()
        {
            String str = Convert.ToString(m_nCurIndex) + "/" + Convert.ToString(m_nMaxSize);
            textBox_input.Text = Convert.ToString(m_nCurIndex);
            label_text.Content = str;
            slider_process.Value = m_nCurIndex;
        }

        private void slider_process_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            m_nCurIndex = (int)slider_process.Value;
            ShowNotifyInfo();
        }

        private void button_All_Click(object sender, RoutedEventArgs e)
        {
            m_nDataType = 3;
            listBox_Data.Items.Clear();
            m_ayDirs.Clear();
            String strPath = System.AppDomain.CurrentDomain.SetupInformation.ApplicationBase + ("手法");
            GetAllFileByDir(strPath);
            listBox_Data.SelectedIndex = 0;
        }
           
        void GetAllFileByDir(string DirPath)
        {
            //列举出所有子文件夹,并对之调用GetAllFileByDir自己;
            foreach (string dir in Directory.GetDirectories(DirPath))
            {
                GetAllFileByDir(dir);
            }

            bool bHasFiles = false;
            foreach (string files in Directory.GetFiles(DirPath))
            {
                bHasFiles = true;
            }
            if (bHasFiles && -1 == m_ayDirs.IndexOf(DirPath))
            {
                m_ayDirs.Add(DirPath);
                listBox_Data.Items.Add(DirPath); 
            }
        }

        void GetAllFileByDir(string DirPath, string searchPattern, List<string> LI_Files)
        {
        	//列举出所有文件,添加到AL
        	foreach (string file in Directory.GetFiles(DirPath, searchPattern))
        		LI_Files.Add(file);
        	//列举出所有子文件夹,并对之调用GetAllFileByDir自己;
        	foreach (string dir in Directory.GetDirectories(DirPath))
        		GetAllFileByDir(dir, searchPattern, LI_Files);
        }

        private void button_Teach_Click(object sender, RoutedEventArgs e)
        {
            m_nDataType = 1;
            listBox_Data.Items.Clear();
            String strPath = System.AppDomain.CurrentDomain.SetupInformation.ApplicationBase + ("手法");
            foreach (string dir in Directory.GetDirectories(strPath))
            {
                listBox_Data.Items.Add(dir);
            }
            listBox_Data.SelectedIndex = 0;
        }

        private void button_List_Click(object sender, RoutedEventArgs e)
        {
            m_nDataType = 2;
            m_ayDirs.Clear();
            listBox_Data.Items.Clear();
            String strPath = System.AppDomain.CurrentDomain.SetupInformation.ApplicationBase + ("手法");
            GetAllFile(strPath, true);
            listBox_Data.SelectedIndex = 0;
        }

        void GetAllFile(string DirPath, bool bReapt)
        {
            //列举出所有子文件夹,并对之调用GetAllFileByDir自己;
            foreach (string dir in Directory.GetDirectories(DirPath))
            {
                GetAllFile(dir, bReapt);
            }

            bool bHasFiles = false;
            foreach (string files in Directory.GetFiles(DirPath))
            {
                bHasFiles = true;
            }
            if (bHasFiles && -1 == m_ayDirs.IndexOf(DirPath))
            {
                String strName = DirPath.Substring(DirPath.LastIndexOf("\\") + 1);
                if (bReapt)
                {
                    foreach (object obj in m_ayDirs)
                    {
                        if (obj.ToString().IndexOf(strName) != -1)
                        {
                            m_ayDirs.Add(DirPath);
                            return;
                        }
                    }
                } 
                m_ayDirs.Add(DirPath);
                listBox_Data.Items.Add(strName);
            }
        }

        private void button_AllData_Click(object sender, RoutedEventArgs e)
        {
            String strPath = System.AppDomain.CurrentDomain.SetupInformation.ApplicationBase + ("手法");
            ListAllFileByDir(strPath);
            listBox_AllData.SelectedIndex = 0;
        }

        void ListAllFileByDir(string DirPath)
        {
            button_data.IsEnabled = true;
            //列举出所有子文件夹,并对之调用GetAllFileByDir自己;
            foreach (string dir in Directory.GetDirectories(DirPath))
            {
                ListAllFileByDir(dir);
            }

            bool bHasFiles = false;
            foreach (string files in Directory.GetFiles(DirPath))
            {
                bHasFiles = true;
            }
            if (bHasFiles)
            {
                listBox_AllData.Items.Add(DirPath);
            }
        }

        private void button_data_Click(object sender, RoutedEventArgs e)
        {
            if (listBox_AllData.SelectedIndex == -1)
            {
                return;
            }
            String strPath = listBox_AllData.SelectedValue.ToString();

            ArrayList ayXList = new ArrayList();
            ArrayList ayYList = new ArrayList();
            ArrayList ayZList = new ArrayList();

            GetFileDatas(strPath + "\\X.csv", ayXList);
            GetFileDatas(strPath + "\\Y.csv", ayYList);
            GetFileDatas(strPath + "\\Z.csv", ayZList);

            AnalysisData(1, ayXList);
            AnalysisData(2, ayYList);
            AnalysisData(3, ayZList);

            AnalysisMaxMinValue(1, ayXList);
            AnalysisMaxMinValue(2, ayYList);
            AnalysisMaxMinValue(3, ayZList);

        }

        private void AnalysisMaxMinValue(int nType, ArrayList List)
        {
            double dMaxValue = 0.0;
            double dMinValue = 0.0;

            foreach (double dValue in List)
            {
                if (dValue > dMaxValue)
                {
                    dMaxValue = dValue;
                }

                if (dValue < dMinValue)
                {
                    dMinValue = dValue;
                }
            }

            switch (nType)
            {
                case 1:
                    {
                        textBox_XMax.Text = Convert.ToString(dMaxValue);
                        textBox_XMin.Text = Convert.ToString(dMinValue);
                        break;
                    }
                case 2:
                    {
                        textBox_YMax.Text = Convert.ToString(dMaxValue);
                        textBox_YMin.Text = Convert.ToString(dMinValue);
                        break;
                    }
                case 3:
                    {
                        textBox_ZMax.Text = Convert.ToString(dMaxValue);
                        textBox_ZMin.Text = Convert.ToString(dMinValue);
                        break;
                    }
            }
        }

        private void AnalysisData(int nType, ArrayList list)
        {
            double dAvgValue = GetAvgData(list);

            ArrayList ayMax = new ArrayList();
            ArrayList ayMin = new ArrayList();
            double dMaxValue = dAvgValue;
            double dMinValue = dAvgValue;

            for (int i = 0; i < list.Count - 1; ++i )
            {
                Boolean bUp = false;
                for (int j = i + 1; j < list.Count; ++j )
                {
                    if (Math.Abs((double)list[j] - (double)list[i]) < 1e-5)
                    {
                        continue;
                    }
                    //////////////////////////////////////////////////////////////////////////上升
                    if ((double)list[i] - (double)list[j] < 1e-5)
                    {
                        dMaxValue = (double)list[i];
                        bUp = true;
                        break;
                    }
                    else
                    {
                        dMinValue = (double)list[i];
                        bUp = false;
                        break;
                    }
                }

                if (bUp)
                {
                    for (int j = i + 1; j < list.Count; ++j)
                    {
                        if (dMaxValue - (double)list[j] <= 1e-5)
                        {
                            dMaxValue = (double)list[j];
                        }
                        else
                        {
                            ayMax.Add(dMaxValue);
                            i = j;
                            break;
                        }
                    }
                }
                else
                {
                    for (int j = i + 1; j < list.Count; ++j)
                    {
                        if ((double)list[j] - dMinValue <= 1e-5)
                        {
                            dMinValue = (double)list[j];
                        }
                        else
                        {
                            ayMin.Add(dMinValue);
                            i = j;
                            break;
                        }
                    }
                }
            }

            int nMaxCount = ayMax.Count;
            int nMinCount = ayMin.Count;

            if (nMaxCount > nMinCount)
            {
                nMinCount = nMaxCount;
            }
            else
            {
                nMaxCount = nMinCount;
            }
            double dMaxAvge = GetAvgData(ayMax);
            double dMinAvge = GetAvgData(ayMin);
            double dSDeviation = GetStandardDeviation(list);
            switch (nType)
            {
                case 1:
                    {
                        textBox_X_Avg.Text = Convert.ToString(dAvgValue);
                        textBox_X_Max.Text = Convert.ToString(nMaxCount);
                        textBox_X_Max_Avg.Text = Convert.ToString(dMaxAvge);
                        textBox_X_Min.Text = Convert.ToString(nMinCount);
                        textBox_X_Min_Avg.Text = Convert.ToString(dMinAvge);
                        textBox_X_BaseOffset.Text = Convert.ToString(dSDeviation);
                        break;
                    }
                case 2:
                    {
                        textBox_Y_Avg.Text = Convert.ToString(dAvgValue);
                        textBox_Y_Max.Text = Convert.ToString(nMaxCount);
                        textBox_Y_Max_Avg.Text = Convert.ToString(dMaxAvge);
                        textBox_Y_Min.Text = Convert.ToString(nMinCount);
                        textBox_Y_Min_Avg.Text = Convert.ToString(dMinAvge);
                        textBox_Y_BaseOffset.Text = Convert.ToString(dSDeviation);
                        break;
                    }
                case 3:
                    {
                        textBox_Z_Avg.Text = Convert.ToString(dAvgValue);
                        textBox_Z_Max.Text = Convert.ToString(nMaxCount);
                        textBox_Z_Max_Avg.Text = Convert.ToString(dMaxAvge);
                        textBox_Z_Min.Text = Convert.ToString(nMinCount);
                        textBox_Z_Min_Avg.Text = Convert.ToString(dMinAvge);
                        textBox_Z_BaseOffset.Text = Convert.ToString(dSDeviation);
                        break;
                    }
            }
        }

        private double GetAvgData(ArrayList list)
        {
            double dValue = 0.0;
            foreach (double dObj in list)
            {
                dValue += dObj;
            }
            return dValue / (1.0 * list.Count);
        }

        /// <summary>
        /// 求标准差
        /// </summary>
        /// <param name="list"></param>
        /// <returns></returns>
        private double GetStandardDeviation(ArrayList list)
        {
            double dAvgValue = GetAvgData(list);

            double dTmp = 0.0;
            foreach (double dObj in list)
            {
                dTmp += Math.Pow((dObj - dAvgValue), 2.0);
            }
            dTmp = dTmp / (1.0 * list.Count);

            return Math.Sqrt(dTmp);
        }

        private void SetCurDataByIndex()
        {
            foreach (object obj in m_ayLists)
            {
                Data_Info pData = (Data_Info)obj;
                string strData_x = "0";
                string strData_y = "0";
                string strData_z = "0";
                double dX = 0.0;
                double dY = 0.0;
                double dZ = 0.0;

                int nIndex = 0;
                if (m_nCurIndex < pData.m_ayXData.Count)
                {
                    dX = (double)pData.m_ayXData[m_nCurIndex];
                    m_wndGraphic.AddGraphicsData(1, pData.m_strName, (double)pData.m_ayXData[m_nCurIndex], m_nCurGraphicType);
                    ++nIndex;
					strData_x = pData.m_ayXData[m_nCurIndex].ToString();
                }

                if (m_nCurIndex < pData.m_ayYData.Count)
                {
                    dY = (double)pData.m_ayYData[m_nCurIndex];
                    m_wndGraphic.AddGraphicsData(2, pData.m_strName, (double)pData.m_ayYData[m_nCurIndex], m_nCurGraphicType);
                    ++nIndex;
					strData_y = pData.m_ayYData[m_nCurIndex].ToString();
                }

                if (m_nCurIndex < pData.m_ayZData.Count)
                {
                    dZ = (double)pData.m_ayZData[m_nCurIndex];
                    m_wndGraphic.AddGraphicsData(3, pData.m_strName, (double)pData.m_ayZData[m_nCurIndex], m_nCurGraphicType);
                    ++nIndex;
                    strData_z = pData.m_ayZData[m_nCurIndex].ToString();
                }

                if (m_nDataType == 3 && nIndex == 3 && null != m_WndAnalysis)
                {
                    double dValue = Math.Sqrt(Math.Pow(dX, 2.0) + Math.Pow(dY, 2.0) + Math.Pow(dZ, 2.0));
                    dValue *= 2;
                    m_WndAnalysis.AddAnalysisPoint(dValue, m_nCurGraphicType);
                }
                //将数据发下去
                string CurData = strData_x + "," + strData_y + "," + strData_z;
                m_MainWindows.WriteDataToShareMemory(CurData, CurData.Length);

                ShowNotifyInfo();
            }
        }

        private void button_Send_Click(object sender, RoutedEventArgs e)
        {
            String strTmp = ("MSG PRESSURE ");
            strTmp += textBox_F.Text.ToString();
            m_MainWindows.SendWndMsg(strTmp);

            strTmp = ("MSG AREA ");
            strTmp += textBox_S.Text.ToString();
            m_MainWindows.SendWndMsg(strTmp);
        }
    }
}
