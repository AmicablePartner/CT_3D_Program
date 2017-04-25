// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� MODULEWRAPPER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// MODULEWRAPPER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef MODULEWRAPPER_EXPORTS
#define MODULEWRAPPER_API __declspec(dllexport)
#else
#define MODULEWRAPPER_API __declspec(dllimport)
#endif

//// �����Ǵ� ModuleWrapper.dll ������
//class MODULEWRAPPER_API CModuleWrapper {
//public:
//	CModuleWrapper(void);
//	// TODO: �ڴ�������ķ�����
//};
//
//extern MODULEWRAPPER_API int nModuleWrapper;
//
//MODULEWRAPPER_API int fnModuleWrapper(void);
#include "IMainModule.h"
#define WND_NAME_LENGTH 128
#pragma pack(push, 1)
struct _WndInfo
{
	int nHwnd;
	int nMainWnd;
	char szName[WND_NAME_LENGTH];
};
#pragma pack(pop)

//typedef void(__stdcall *CmdMsgCallback)(char *);
/// <summary>
/// ����SimViz���ڣ���ʾ3D��ͼ
/// </summary>
/// <param name="hwnd"></param>
extern "C" _declspec(dllexport) HWND CreateEnginWnd(int *pAddr, int nCount);

/// <summary>
/// �ͷ�SimViz��������Դ
/// </summary>
extern "C" _declspec(dllexport) void DestroyEnginWnd();

/// <summary>
/// �������SimViz���
/// </summary>
extern "C" _declspec(dllexport) void SendCmdMsg(char* szMsg);

/// <summary>
/// ���ô��ڴ�Сλ��
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="w"></param>
/// <param name="h"></param>
/// <returns></returns>
extern "C" _declspec(dllexport) int SetEnginWndPos(int x, int y, int w, int h, const char *szName);


/// <summary>
/// ���ý���������Ϣ�ص�����
/// </summary>
/// <param name="pCmdMsgCallback">void func(string cmd)</param>
extern "C" _declspec(dllexport) void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback);