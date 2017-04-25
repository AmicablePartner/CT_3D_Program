// CT3DModule.h : CT3DModule DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCT3DModuleApp
// 有关此类实现的信息，请参阅 CT3DModule.cpp
//

class CCT3DModuleApp : public CWinApp
{
public:
	CCT3DModuleApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
