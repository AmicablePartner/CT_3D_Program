// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MODULEWRAPPER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MODULEWRAPPER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef MODULEWRAPPER_EXPORTS
#define MODULEWRAPPER_API __declspec(dllexport)
#else
#define MODULEWRAPPER_API __declspec(dllimport)
#endif

//// 此类是从 ModuleWrapper.dll 导出的
//class MODULEWRAPPER_API CModuleWrapper {
//public:
//	CModuleWrapper(void);
//	// TODO: 在此添加您的方法。
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
/// 创建SimViz窗口，显示3D视图
/// </summary>
/// <param name="hwnd"></param>
extern "C" _declspec(dllexport) HWND CreateEnginWnd(int *pAddr, int nCount);

/// <summary>
/// 释放SimViz框架相关资源
/// </summary>
extern "C" _declspec(dllexport) void DestroyEnginWnd();

/// <summary>
/// 发送命令到SimViz框架
/// </summary>
extern "C" _declspec(dllexport) void SendCmdMsg(char* szMsg);

/// <summary>
/// 设置窗口大小位置
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="w"></param>
/// <param name="h"></param>
/// <returns></returns>
extern "C" _declspec(dllexport) int SetEnginWndPos(int x, int y, int w, int h, const char *szName);


/// <summary>
/// 设置接收命令消息回调函数
/// </summary>
/// <param name="pCmdMsgCallback">void func(string cmd)</param>
extern "C" _declspec(dllexport) void SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback);