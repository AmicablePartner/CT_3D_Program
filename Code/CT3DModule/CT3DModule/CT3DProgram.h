#pragma once
#include "Main3DWnd.h"

class CCT3DProgram :
	public IMainModule
{
public:
	CCT3DProgram();
	~CCT3DProgram();

	/// <summary>
	/// 创建窗口，显示3D视图
	/// </summary>
	/// <param name="hwnd"></param>
	virtual HWND CreateEnginWnd(int nHwd);
	/// <summary>
	/// 释放相关资源
	/// </summary>
	virtual void DestroyEnginWnd();

	/// <summary>
	/// 发送命令
	/// </summary>
	virtual void SendCmdMsg(char* szMsg);
	/// <summary>
	/// 设置窗口大小位置
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="w"></param>
	/// <param name="h"></param>
	/// <returns></returns>
	virtual int SetEnginWndPos(int x, int y, int w, int h);
	/// <summary>
	/// 设置接收命令消息回调函数
	/// </summary>
	/// <param name="pCmdMsgCallback">void func(string cmd)</param>
	virtual void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback);

public:

	//主窗口
	CMain3DWnd*		m_pMainWnd;
};

