#pragma once
typedef void(__stdcall *CmdMsgCallback)(char *);
namespace MainModule
{
	class _declspec(novtable) IMainModule
	{
	public:
		/// <summary>
		/// ����SimViz���ڣ���ʾ3D��ͼ
		/// </summary>
		/// <param name="hwnd"></param>
		virtual HWND CreateEnginWnd( int nHwd) = 0 ;
		/// <summary>
		/// �ͷ�SimViz��������Դ
		/// </summary>
		virtual void DestroyEnginWnd() = 0 ; 
		
		/// <summary>
		/// �������SimViz���
		/// </summary>
		virtual void SendCmdMsg(char* szMsg) = 0 ;
		/// <summary>
		/// ���ô��ڴ�Сλ��
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		/// <returns></returns>
		virtual int SetEnginWndPos(int x, int y, int w, int h) = 0 ;
		/// <summary>
		/// ���ý���������Ϣ�ص�����
		/// </summary>
		/// <param name="pCmdMsgCallback">void func(string cmd)</param>
		virtual void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback) = 0 ;
	};
	
}

EXTERN_C _declspec(dllexport) MainModule::IMainModule* GetMainModule() ;