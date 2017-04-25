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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Collections;
using System.Diagnostics;

namespace CT3DProgram
{
	/// <summary>
	/// _3DUserControl.xaml 的交互逻辑
	/// </summary>
	///
	public partial class _3DUserControl : Window
	{
		public MainWindow m_MainWindows { get; set; }

        protected Dictionary<string, string> m_StlNameToFile;

		public _3DUserControl(MainWindow ptrMainWindow)
        {
            InitializeComponent();
            m_StlNameToFile = new Dictionary<string, string>();
            GetAllFileByDir(System.Windows.Forms.Application.StartupPath + ("\\STLS"));
			m_MainWindows = ptrMainWindow;
		}

		void GetAllFileByDir(string DirPath)
		{
			//列举出所有文件,添加到AL
			foreach (string file in Directory.GetFiles(DirPath))
			{
                if (file.Contains(".StlCfg"))
                {
                    string[] PathStrList = file.Split('\\');
                    string NewFileName = PathStrList.Last();
                    NewFileName = NewFileName.Replace(".StlCfg", "");
                    m_StlNameToFile.Add(NewFileName, file);
                    listBoxFile.Items.Add(NewFileName);
                }
			}
			// 			//列举出所有子文件夹,并对之调用GetAllFileByDir自己;
			// 			foreach (string dir in Directory.GetDirectories(DirPath))
			// 				GetAllFileByDir(dir, ref AL);
		}

// 		void GetAllFileByDir(string DirPath, string searchPattern, List<string> LI_Files)
// 		{
// 			//列举出所有文件,添加到AL
// 			foreach (string file in Directory.GetFiles(DirPath, searchPattern))
// 				LI_Files.Add(file);
// 			//列举出所有子文件夹,并对之调用GetAllFileByDir自己;
// 			foreach (string dir in Directory.GetDirectories(DirPath))
// 				GetAllFileByDir(dir, searchPattern, LI_Files);
// 		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			string strName = listBoxFile.SelectedValue.ToString();
            if (m_StlNameToFile.ContainsKey(strName))
            {
                this.Hide();
                m_MainWindows.LoadStlModule(m_StlNameToFile[strName]);
            }
		}
	}
}
