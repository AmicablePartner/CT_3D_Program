#pragma once
#include "stdafx.h"
#include "CT3DProgram.h"

_declspec(dllexport) IMainModule* GetMainModule()
{
	IMainModule* pObj = new CCT3DProgram();
	return pObj;
}

CCT3DProgram::CCT3DProgram()
{
}


CCT3DProgram::~CCT3DProgram()
{
}

HWND CCT3DProgram::CreateEnginWnd(int nHwd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//////////////////////////////////////////////////
	CWnd *pWnd = NULL;
	pWnd = CWnd::FromHandle((HWND)nHwd);

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	m_pMainWnd = new CMain3DWnd;
	{
		WNDCLASS wndcls;
		HINSTANCE hInst = AfxGetInstanceHandle();
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		//wndcls.style            =  CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = _T("主窗模块");
		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
		CRect rc;
		pWnd->GetClientRect(&rc);
		if (m_pMainWnd->Create(_T("主窗模块"), _T(""), WS_CHILD | WS_VISIBLE, rc, pWnd, 1))
		{
			//设置显示风格
			m_pMainWnd->ShowWindow(SW_NORMAL);

			m_pMainWnd->Init();

			return m_pMainWnd->GetSafeHwnd();
		}
	}
	return HWND();
}

void CCT3DProgram::DestroyEnginWnd()
{
	if (NULL != m_pMainWnd)
	{
		delete m_pMainWnd;
	}
}

void CCT3DProgram::SendCmdMsg(char * szMsg)
{
	if (NULL != m_pMainWnd)
	{
		m_pMainWnd->SendCmdMsg(szMsg);
	}
}

int CCT3DProgram::SetEnginWndPos(int x, int y, int w, int h)
{
	if(NULL != m_pMainWnd)
	{
		m_pMainWnd->SetEnginWndPos(x, y, w, h);
	}
	return 0;
}

void CCT3DProgram::SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback)
{
	if (NULL != m_pMainWnd)
	{
		m_pMainWnd->SetCmdMsgCallback(pCmdMsgCallback);
	}
}
