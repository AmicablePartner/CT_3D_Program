#pragma once
/************************************************************************
 *	Copyright (c) 2012 China Graphics Technology Co.,Ltd. 
 
 *	File:ThreadEnginLoader.h
 *	Purpose: 
			ʹ��c++/CLR������װC#ʹ�ýӿ�

 *	Author: xiaoyh
 *	Resourse: hook
 *	Date:	2014.12.27

 *	Revision:	1.5
************************************************************************
 �汾��ʷ��
 *	xiaoyh		2014.12.27		create
 v1.5 201501.01 �����ӿں����ڵĴ���ʵ�ֻص�����
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
		//���������б�
	public:
		/// <summary>
		/// ����SimViz���ڣ���ʾ3D��ͼ
		/// </summary>
		/// <param name="pAddr" desc="���ھ���б�"></param>
		/// <param name="nCount" desc="�б�������"></param>
		//int CreateEnginWnd( int %pAddr, int nCount)
		int CreateEnginWnd( System::IntPtr pAddr, int nCount)
		{
			//C#������ת����ָ��
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
		/// �ͷ�SimViz��������Դ
		/// </summary>
		void DestroyEnginWnd()
		{
			if (pMainModule != NULL)
			{
				pMainModule->DestroyEnginWnd();
			}
		}

		/// <summary>
		/// �������SimViz���
		/// </summary>
		/// <param name="szMsg" desc="��Ϣ����"></param>
		void SendCmdMsg(String ^szMsg)
		{
			//Stringת��Ϊchar *
			IntPtr iPtr = Marshal::StringToHGlobalAnsi(szMsg);
			const char *pMsg = static_cast<const char *>(iPtr.ToPointer());

			//��������
			char *chMsg = new char[strlen(pMsg) + 1];
			strcpy_s(chMsg, strlen(pMsg)+1, pMsg);
			Marshal::FreeHGlobal(iPtr);

			//����c++�ӿ�
			if (pMainModule != NULL)
			{
				pMainModule->SendCmdMsg(chMsg);

				delete[] chMsg;
			}
		}

		/// <summary>
		/// ���ô��ڴ�Сλ��
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		/// <returns></returns>
		int SetEnginWndPos(int x, int y, int w, int h, String ^szName)
		{
			//Stringת��Ϊchar *
			IntPtr iPtr = Marshal::StringToHGlobalAnsi(szName);
			const char *pName = static_cast<const char *>(iPtr.ToPointer());

			//��������
			char *pMsg = new char[strlen(pName) + 1];
			strcpy_s(pMsg, strlen(pName)+1, pName);
			Marshal::FreeHGlobal(iPtr);

			int nRes = 0;
			//��������
			if (pMainModule != NULL)
			{
				nRes =  pMainModule->SetEnginWndPos(x, y, w, h, pMsg);

				delete [] pMsg;
			}
			return nRes;
		}
		/// <summary>
		/// ���ý���������Ϣ�ص�����
		/// </summary>
		/// <param name="pCmdMsgCallback">void func(string cmd)</param>
		void SetCmdMsgCallback(CSharpCallback ^MsgCallback)
		{
			IntPtr pFunc = Marshal::GetFunctionPointerForDelegate(MsgCallback);
			pMainModule->SetCmdMsgCallback((CmdMsgCallback)(void *)pFunc);
		}
	};
}
