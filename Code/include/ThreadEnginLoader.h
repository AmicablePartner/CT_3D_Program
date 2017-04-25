#pragma once
/************************************************************************
 *	Copyright (c) 2012 China Graphics Technology Co.,Ltd. 
 
 *	File:ThreadEnginLoader.h
 *	Purpose: 
			使用c++/CLR技术封装C#使用接口

 *	Author: xiaoyh
 *	Resourse: hook
 *	Date:	2014.12.27

 *	Revision:	1.5
************************************************************************
 版本历史：
 *	xiaoyh		2014.12.27		create
 v1.5 201501.01 修正接口函数内的错误，实现回调函数
************************************************************************/

#include "..\include\IMainModule.h"

using namespace System;
using namespace System::Runtime::InteropServices;


namespace ThreadEnginLoader {

	public delegate void CSharpCallback(String ^msg);
	public ref class CPlusDLLLoader
	{
		[DllImport("MainModule.dll")]
		static MainModule::IMainModule *GetMainModule();
		static MainModule::IMainModule *pMainModule = GetMainModule();
		//导出函数列表
	public:
		/// <summary>
		/// 创建SimViz窗口，显示3D视图
		/// </summary>
		/// <param name="pAddr" desc="窗口句柄列表"></param>
		/// <param name="nCount" desc="列表对象个数"></param>
		//int CreateEnginWnd( int %pAddr, int nCount)
		int CreateEnginWnd( System::IntPtr pAddr, int nCount)
		{
			//C#的引用转换成指针
			//pin_ptr<int> pPtr = &pAddr;
			int *pPtr = static_cast<int *>(pAddr.ToPointer());

			if (pMainModule == NULL)
			{
				return 0;
			}
			else
			{
				return (int)(pMainModule->CreateEnginWnd(pPtr, nCount));
			}
		} 

		/// <summary>
		/// 释放SimViz框架相关资源
		/// </summary>
		void DestroyEnginWnd()
		{
			if (pMainModule != NULL)
			{
				pMainModule->DestroyEnginWnd();
			}
		}

		/// <summary>
		/// 发送命令到SimViz框架
		/// </summary>
		/// <param name="szMsg" desc="消息内容"></param>
		void SendCmdMsg(String ^szMsg)
		{
			//String转换为char *
			IntPtr iPtr = Marshal::StringToHGlobalAnsi(szMsg);
			const char *pMsg = static_cast<const char *>(iPtr.ToPointer());

			//保存内容
			char *chMsg = new char[strlen(pMsg) + 1];
			strcpy_s(chMsg, strlen(pMsg)+1, pMsg);
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
		int SetEnginWndPos(int x, int y, int w, int h, String ^szName)
		{
			//String转化为char *
			IntPtr iPtr = Marshal::StringToHGlobalAnsi(szName);
			const char *pName = static_cast<const char *>(iPtr.ToPointer());

			//保存内容
			char *pMsg = new char[strlen(pName) + 1];
			strcpy_s(pMsg, strlen(pName)+1, pName);
			Marshal::FreeHGlobal(iPtr);

			int nRes = 0;
			//处理数据
			if (pMainModule != NULL)
			{
				nRes =  pMainModule->SetEnginWndPos(x, y, w, h, pMsg);

				delete [] pMsg;
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
