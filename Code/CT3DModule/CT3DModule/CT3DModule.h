// CT3DModule.h : CT3DModule DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCT3DModuleApp
// �йش���ʵ�ֵ���Ϣ������� CT3DModule.cpp
//

class CCT3DModuleApp : public CWinApp
{
public:
	CCT3DModuleApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
