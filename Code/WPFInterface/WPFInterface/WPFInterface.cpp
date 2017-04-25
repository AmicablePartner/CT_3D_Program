// WPFInterface.cpp : ���� DLL Ӧ�ó���ĵ���������
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
		//���������б�
	public:
		/// <summary>
		/// ����SimViz���ڣ���ʾ3D��ͼ
		/// </summary>
		/// <param name="nHwd" desc="���ھ��"></param>
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
		/// �ͷ������Դ
		/// </summary>
		void DestroyEnginWnd()
		{
			if (pMainModule != NULL)
			{
				pMainModule->DestroyEnginWnd();
			}
		}

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="szMsg" desc="��Ϣ����"></param>
		void SendCmdMsg(String ^szMsg)
		{
			//Stringת��Ϊchar *
			IntPtr iPtr = Marshal::StringToHGlobalAnsi(szMsg);
			const char *pMsg = static_cast<const char *>(iPtr.ToPointer());

			//��������
			char *chMsg = new char[strlen(pMsg) + 1];
			strcpy_s(chMsg, strlen(pMsg) + 1, pMsg);
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
		int SetEnginWndPos(int x, int y, int w, int h)
		{
			int nRes = 0;
			//��������
			if (pMainModule != NULL)
			{
				nRes = pMainModule->SetEnginWndPos(x, y, w, h);
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

