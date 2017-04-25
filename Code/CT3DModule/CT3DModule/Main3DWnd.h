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
	//��ʼ��
	bool Init( );

	//���ûص�
	void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback);

	//���ô��ڴ�С
	int SetEnginWndPos(int x, int y, int w, int h);

	//������Ϣ
	void SendCmdMsg(char* szMsg);

	virtual void NotifyShareMemory(const char *szText, long lSize);

protected:

	//��ʼ��Ogre
	void InitOgre(HWND hWnd);

	//��ʼ����Դ
	void InitRes();

	//��ʼ������
	void InitTerr();

	//����ģ����
	void LoadSTLS();

	//���±���
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

	//���״̬
	std::pair<bool, bool>	m_LAndRButtonDownState;

	//��갴��ʱ������
	Ogre::Vector2	m_ButtonDownPos;

	//ģ�Ͷ���
	CDrawSTLMesh*	m_STLModel;

	//���ε�����λ��
	Ogre::Vector4	m_ChangeShapeCenter;
	//���ε�λ��
	Ogre::Vector4	m_ChangeShapePower;

protected:

	CmdMsgCallback	m_GuiCallBack;

	//��ǰSTL�ļ�·��
	CString		m_CurSTLFilePath;

	/** �����ڴ�.*/
	CommShareMemoryMgr *m_pShareMemoryMgr;

	IXMLParse*	m_XmlInterface;

	//��ǰSTL���ģ���б�
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

