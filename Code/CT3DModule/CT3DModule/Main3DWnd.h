#pragma once
#include "DrawSTLMesh.h"
#include "CommShareMemoryMgr.h"
#include "XmlParse.h"

struct STLInfo
{
	CDrawSTLMesh*	pStlInterface;
	Ogre::SceneNode* pStlNode;
};

typedef std::map<CString, STLInfo> STLInfoMap;
class CMain3DWnd :
	public CWnd, public ShareMemoryNotify
{
public:
	CMain3DWnd();
	~CMain3DWnd();

public:
	//初始化
	bool Init( );

	//设置回调
	void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback);

	//设置窗口大小
	int SetEnginWndPos(int x, int y, int w, int h);

	//接收消息
	void SendCmdMsg(char* szMsg);

	virtual void NotifyShareMemory(const char *szText, long lSize);

protected:

	//初始化Ogre
	void InitOgre(HWND hWnd);

	//初始化资源
	void InitRes();

	//初始化地形
	void InitTerr();

	//加载模型组
	void LoadSTLS();

	//更新变形
	void UpdateChape();

protected:

	Ogre::Root* root;

	Ogre::RenderWindow* MainRenderWnd;

	Ogre::SceneManager* CurSceneMgr;

	Ogre::Camera* MainCamera;

	Ogre::Viewport* MainViewPort;

	Ogre::Light* MainLight;

	bool m_isInitOgre;

	bool m_isExit;

	Ogre::SceneNode* MainNode;

	//鼠标状态
	std::pair<bool, bool>	m_LAndRButtonDownState;

	//鼠标按下时的坐标
	Ogre::Vector2	m_ButtonDownPos;

	//模型对象
	CDrawSTLMesh*	m_STLModel;

	//变形的中心位置
	Ogre::Vector4	m_ChangeShapeCenter;
	//变形的位移
	Ogre::Vector4	m_ChangeShapePower;

protected:

	CmdMsgCallback	m_GuiCallBack;

	//当前STL文件路径
	CString		m_CurSTLFilePath;

	/** 共享内存.*/
	CommShareMemoryMgr *m_pShareMemoryMgr;

	IXMLParse*	m_XmlInterface;

	//当前STL组的模型列表
	STLInfoMap	m_StlInfoList;

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

