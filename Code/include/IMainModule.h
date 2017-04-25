#pragma once
typedef void(__stdcall *CmdMsgCallback)(char *);
namespace MainModule
{
	class _declspec(novtable) IMainModule
	{
	public:
		/// <summary>
		/// 创建SimViz窗口，显示3D视图
		/// </summary>
		/// <param name="hwnd"></param>
		virtual HWND CreateEnginWnd( int nHwd) = 0 ;
		/// <summary>
		/// 释放SimViz框架相关资源
		/// </summary>
		virtual void DestroyEnginWnd() = 0 ; 
		
		/// <summary>
		/// 发送命令到SimViz框架
		/// </summary>
		virtual void SendCmdMsg(char* szMsg) = 0 ;
		/// <summary>
		/// 设置窗口大小位置
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		/// <returns></returns>
		virtual int SetEnginWndPos(int x, int y, int w, int h) = 0 ;
		/// <summary>
		/// 设置接收命令消息回调函数
		/// </summary>
		/// <param name="pCmdMsgCallback">void func(string cmd)</param>
		virtual void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback) = 0 ;
	};
	
}

EXTERN_C _declspec(dllexport) MainModule::IMainModule* GetMainModule() ;