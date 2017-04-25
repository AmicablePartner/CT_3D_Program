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
	/** ����֪ͨ�ӿ�.*/
	virtual void NotifyShareMemory(const char *szText, long lSize) = 0;
};


#pragma pack(push, 1)
/** ���ݽṹ�������Լ��̳�Ȼ���Զ���ṹ��.*/
#define SHAREMEMORYLEN 1024
struct _CommShareMemory
{
	long		m_lWriteTime;			/** д���ʱ��.*/
	long		m_lReadTime;			/** ��ȡ��ʱ��.*/
	long		m_lCurSize;				/** ��ǰ���ݳ���.*/
	char		m_pVoid[SHAREMEMORYLEN];				/** д�������.*/

	_CommShareMemory()
	{
		m_lWriteTime = 0;
		m_lReadTime = 0;
		m_lCurSize = 0;
		ZeroMemory(m_pVoid, SHAREMEMORYLEN);
	}

	/** ��������.*/
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
	/** ��ʼ����ǰ�Ĺ����ڴ������.*/
	void InitShareMemoryMgr(bool bCreate, const char *szMemoryName);

	/** �رչ����ڴ�.*/
	void ReleaseShareMemoryMgr();

	/** ��ʼ�����ڴ����ݽ���.*/
	void OnRun();

	/** ��ͣ���ݽ���.*/
	void OnPause();

	/** �������ݽӿ�.*/
	void SetShareMemoryNotify(ShareMemoryNotify *pNotify);

	/** д������.*/
	void WriteShareMemory(const char *szText, long lSize);

protected:
	/** ����.*/
	void CreateShareMemory(const char *szMemoryName);

	/** ��.*/
	void OpenShareMemory(const char *szMemoryName);

	/** ���ݴ����߳�.*/
	static unsigned _stdcall OnDataDealThread(LPVOID lPvoid);
	void OnDataDeal();

	/** ���߳�.*/
	static unsigned _stdcall OnOpenMemoryThread(LPVOID lPvoid);
	void OnOpenShareMemory();

	/** ����.*/
	void Clear();

private:
	void				*m_pShareMemory;		/** �����ڴ�����.*/
	std::string			m_strMemoryName;		/** �����ڴ�����.*/
	HANDLE				m_hShareMemoryFile;		/** �����ڴ�ľ��.*/
	HANDLE				m_hEventControl;		/** �����¼�.*/
	bool				m_bStop;				/** ֹͣʹ��.*/
	bool				m_bPause;				/** ��ͣʹ��.*/
	CRITICAL_SECTION	m_csSection;

	HANDLE				m_hDataThread;			/** �����߳̾��.*/
	ShareMemoryNotify	*m_pMemoryNotify;		/** ����֪ͨ�ӿ�ָ��.*/

};

