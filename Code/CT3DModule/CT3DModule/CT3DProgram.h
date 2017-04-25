#pragma once
#include "Main3DWnd.h"

class CCT3DProgram :
	public IMainModule
{
public:
	CCT3DProgram();
	~CCT3DProgram();

	/// <summary>
	/// �������ڣ���ʾ3D��ͼ
	/// </summary>
	/// <param name="hwnd"></param>
	virtual HWND CreateEnginWnd(int nHwd);
	/// <summary>
	/// �ͷ������Դ
	/// </summary>
	virtual void DestroyEnginWnd();

	/// <summary>
	/// ��������
	/// </summary>
	virtual void SendCmdMsg(char* szMsg);
	/// <summary>
	/// ���ô��ڴ�Сλ��
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="w"></param>
	/// <param name="h"></param>
	/// <returns></returns>
	virtual int SetEnginWndPos(int x, int y, int w, int h);
	/// <summary>
	/// ���ý���������Ϣ�ص�����
	/// </summary>
	/// <param name="pCmdMsgCallback">void func(string cmd)</param>
	virtual void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback);

public:

	//������
	CMain3DWnd*		m_pMainWnd;
};

