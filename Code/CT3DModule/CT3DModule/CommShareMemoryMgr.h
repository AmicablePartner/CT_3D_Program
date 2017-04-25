/** Header Of CommShareMemoryMgr
* Author	: HanYang
* Time		: 2016/02/27
* History	: 
*/
#pragma once
#include <string>

class  _declspec(novtable) ShareMemoryNotify
{
public:
	/** 数据通知接口.*/
	virtual void NotifyShareMemory(const char *szText, long lSize) = 0;
};


#pragma pack(push, 1)
/** 数据结构，可以自己继承然后自定义结构体.*/
#define SHAREMEMORYLEN 1024
struct _CommShareMemory
{
	long		m_lWriteTime;			/** 写入的时间.*/
	long		m_lReadTime;			/** 读取的时间.*/
	long		m_lCurSize;				/** 当前数据长度.*/
	char		m_pVoid[SHAREMEMORYLEN];				/** 写入的内容.*/

	_CommShareMemory()
	{
		m_lWriteTime = 0;
		m_lReadTime = 0;
		m_lCurSize = 0;
		ZeroMemory(m_pVoid, SHAREMEMORYLEN);
	}

	/** 清理数据.*/
// 	void Clear()
// 	{
// 		if (m_pVoid)
// 		{
// 			delete []m_pVoid;
// 			m_pVoid = NULL;
// 		}
// 	}
};

#pragma pack(pop)
class CommShareMemoryMgr
{
public:
	CommShareMemoryMgr();
	~CommShareMemoryMgr();

public:
	/** 初始化当前的共享内存管理类.*/
	void InitShareMemoryMgr(bool bCreate, const char *szMemoryName);

	/** 关闭共享内存.*/
	void ReleaseShareMemoryMgr();

	/** 开始共享内存数据交换.*/
	void OnRun();

	/** 暂停数据交换.*/
	void OnPause();

	/** 设置数据接口.*/
	void SetShareMemoryNotify(ShareMemoryNotify *pNotify);

	/** 写入数据.*/
	void WriteShareMemory(const char *szText, long lSize);

protected:
	/** 创建.*/
	void CreateShareMemory(const char *szMemoryName);

	/** 打开.*/
	void OpenShareMemory(const char *szMemoryName);

	/** 数据处理线程.*/
	static unsigned _stdcall OnDataDealThread(LPVOID lPvoid);
	void OnDataDeal();

	/** 打开线程.*/
	static unsigned _stdcall OnOpenMemoryThread(LPVOID lPvoid);
	void OnOpenShareMemory();

	/** 清理.*/
	void Clear();

private:
	void				*m_pShareMemory;		/** 共享内存数据.*/
	std::string			m_strMemoryName;		/** 共享内存名称.*/
	HANDLE				m_hShareMemoryFile;		/** 共享内存的句柄.*/
	HANDLE				m_hEventControl;		/** 控制事件.*/
	bool				m_bStop;				/** 停止使用.*/
	bool				m_bPause;				/** 暂停使用.*/
	CRITICAL_SECTION	m_csSection;

	HANDLE				m_hDataThread;			/** 数据线程句柄.*/
	ShareMemoryNotify	*m_pMemoryNotify;		/** 数据通知接口指针.*/

};

