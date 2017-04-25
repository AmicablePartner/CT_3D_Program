// WPFInterface.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "..\..\include\IMainModule.h"
using namespace System;
using namespace System::Runtime::InteropServices;


namespace WPFInterface {

	public delegate void CSharpCallback(String ^msg);
	public ref class CPlusDLLLoader
	{
		[DllImport("CT3DModule.dll")]
		static MainModule::IMainModule *GetMainModule();
		static MainModule::IMainModule *pMainModule = GetMainModule();
		//导出函数列表
	public:
		/// <summary>
		/// 创建SimViz窗口，显示3D视图
		/// </summary>
		/// <param name="nHwd" desc="窗口句柄"></param>
		int CreateEnginWnd(int nHwd)
		{

			if (pMainModule == NULL)
			{
				return 0;
			}
			else
			{
				return (int)(pMainModule->CreateEnginWnd(nHwd));
			}
		}

		/// <summary>
		/// 释放相关资源
		/// </summary>
		void DestroyEnginWnd()
		{
			if (pMainModule != NULL)
			{
				pMainModule->DestroyEnginWnd();
			}
		}

		/// <summary>
		/// 发送命令
		/// </summary>
		/// <param name="szMsg" desc="消息内容"></param>
		void SendCmdMsg(String ^szMsg)
		{
			//String转换为char *
			IntPtr iPtr = Marshal::StringToHGlobalAnsi(szMsg);
			const char *pMsg = static_cast<const char *>(iPtr.ToPointer());

			//保存内容
			char *chMsg = new char[strlen(pMsg) + 1];
			strcpy_s(chMsg, strlen(pMsg) + 1, pMsg);
			Marshal::FreeHGlobal(iPtr);

			//调用c++接口
			if (pMainModule != NULL)
			{
				pMainModule->SendCmdMsg(chMsg);

				delete[] chMsg;
			}
		}

		/// <summary>
		/// 设置窗口大小位置
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		/// <returns></returns>
		int SetEnginWndPos(int x, int y, int w, int h)
		{
			int nRes = 0;
			//处理数据
			if (pMainModule != NULL)
			{
				nRes = pMainModule->SetEnginWndPos(x, y, w, h);
			}
			return nRes;
		}
		/// <summary>
		/// 设置接收命令消息回调函数
		/// </summary>
		/// <param name="pCmdMsgCallback">void func(string cmd)</param>
		void SetCmdMsgCallback(CSharpCallback ^MsgCallback)
		{
			IntPtr pFunc = Marshal::GetFunctionPointerForDelegate(MsgCallback);
			pMainModule->SetCmdMsgCallback((CmdMsgCallback)(void *)pFunc);
		}
	};
}

