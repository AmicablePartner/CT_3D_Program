#include "stdafx.h"
#include "CommShareMemoryMgr.h"


CommShareMemoryMgr::CommShareMemoryMgr()
{
	m_pShareMemory = NULL;
	m_hShareMemoryFile = INVALID_HANDLE_VALUE;
	m_hEventControl = INVALID_HANDLE_VALUE;
	m_bStop = false;
	m_bPause = false;
	m_hDataThread = INVALID_HANDLE_VALUE;
	m_pMemoryNotify = NULL;
	InitializeCriticalSection(&m_csSection);
}


CommShareMemoryMgr::~CommShareMemoryMgr()
{
	Clear();
}

void CommShareMemoryMgr::InitShareMemoryMgr(bool bCreate, const char *szMemoryName)
{
	if (bCreate)
	{
		CreateShareMemory(szMemoryName);
	}
	else
	{
		OpenShareMemory(szMemoryName);
	}
}

void CommShareMemoryMgr::ReleaseShareMemoryMgr()
{
	if (m_bPause && m_hEventControl)
	{
		SetEvent(m_hEventControl);
	}
	m_bStop = true;
	Clear();
}

void CommShareMemoryMgr::OnRun()
{
	if (INVALID_HANDLE_VALUE == m_hEventControl)
	{
		m_hEventControl = CreateEvent(NULL, false, false, NULL);
	}

	if (m_hEventControl)
	{
		if (INVALID_HANDLE_VALUE == m_hDataThread)
		{
			m_bStop = false;
			m_bPause = false;
			unsigned int uID = 0;
			_beginthreadex(NULL, 0, CommShareMemoryMgr::OnDataDealThread, this, 0, &uID);
		}
		else
		{
			m_bPause = false;
			SetEvent(m_hEventControl);
		}
	}
}

void CommShareMemoryMgr::OnPause()
{
	m_bPause = true;
}

void CommShareMemoryMgr::SetShareMemoryNotify(ShareMemoryNotify *pNotify)
{
	m_pMemoryNotify = pNotify;
}

void CommShareMemoryMgr::WriteShareMemory(const char *szText, long lSize)
{
	if (m_pShareMemory)
	{
		((_CommShareMemory*)m_pShareMemory)->m_lWriteTime = GetTickCount();
		((_CommShareMemory*)m_pShareMemory)->m_lCurSize = lSize > SHAREMEMORYLEN ? SHAREMEMORYLEN - 1 : lSize;
		memcpy_s(((_CommShareMemory*)m_pShareMemory)->m_pVoid, SHAREMEMORYLEN, szText, ((_CommShareMemory*)m_pShareMemory)->m_lCurSize);
	}
}

void CommShareMemoryMgr::CreateShareMemory(const char *szMemoryName)
{
	if (INVALID_HANDLE_VALUE == m_hShareMemoryFile)
	{
		m_hShareMemoryFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(_CommShareMemory), (CString)szMemoryName);
		if (m_hShareMemoryFile )
		{
			m_pShareMemory = (_CommShareMemory*)MapViewOfFile(m_hShareMemoryFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(_CommShareMemory));
		}
	}
}

void CommShareMemoryMgr::OpenShareMemory(const char *szMemoryName)
{
	if (INVALID_HANDLE_VALUE == m_hShareMemoryFile)
	{
		m_hShareMemoryFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, (CString)szMemoryName);
		if (m_hShareMemoryFile)
		{
			m_pShareMemory = (_CommShareMemory*)MapViewOfFile(m_hShareMemoryFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(_CommShareMemory));
		}
		else
		{
			if (m_strMemoryName.empty())
			{
				m_strMemoryName = szMemoryName;
				unsigned int uID = 0;
				_beginthreadex(NULL, 0, CommShareMemoryMgr::OnOpenMemoryThread, this, 0, &uID);
			}
		}
	}
}

unsigned _stdcall CommShareMemoryMgr::OnDataDealThread(LPVOID lPvoid)
{
	CommShareMemoryMgr *pThis = (CommShareMemoryMgr*)lPvoid;
	if (pThis)
	{
		pThis->OnDataDeal();
	}
	return 1;
}

void CommShareMemoryMgr::OnDataDeal()
{
	while (!m_bStop)
	{
		if (m_bPause)
		{
			WaitForSingleObject(m_hEventControl, INFINITE);
		}

		if (m_bStop)
		{
			break;
		}

		EnterCriticalSection(&m_csSection);
		if (m_pShareMemory)
		{
			if (((_CommShareMemory*)m_pShareMemory)->m_lReadTime < ((_CommShareMemory*)m_pShareMemory)->m_lWriteTime)
			{
				((_CommShareMemory*)m_pShareMemory)->m_lReadTime = GetTickCount();
				if (m_pMemoryNotify)
				{
					m_pMemoryNotify->NotifyShareMemory((char*)((_CommShareMemory*)m_pShareMemory)->m_pVoid, ((_CommShareMemory*)m_pShareMemory)->m_lCurSize);
				}
			}
		}
		LeaveCriticalSection(&m_csSection);

		Sleep(30);
	}
	m_hDataThread = INVALID_HANDLE_VALUE;
}

unsigned _stdcall CommShareMemoryMgr::OnOpenMemoryThread(LPVOID lPvoid)
{
	CommShareMemoryMgr *pThis = (CommShareMemoryMgr*)lPvoid;
	if (pThis)
	{
		pThis->OnOpenShareMemory();
	}
	return 1;
}

void CommShareMemoryMgr::OnOpenShareMemory()
{
	while (INVALID_HANDLE_VALUE == m_hShareMemoryFile)
	{
		OpenShareMemory(m_strMemoryName.c_str());
		Sleep(200);
	}
	m_strMemoryName.clear();
}

void CommShareMemoryMgr::Clear()
{
	EnterCriticalSection(&m_csSection);
	if (m_hDataThread)
	{
		m_bStop = true;
		m_bPause = false;
		WaitForSingleObject(m_hDataThread, INFINITE);
		m_hDataThread = NULL;
	}
	LeaveCriticalSection(&m_csSection);

	if (m_pShareMemory)
	{
		UnmapViewOfFile(m_hShareMemoryFile);
		//m_pShareMemory->Clear();
		delete m_pShareMemory;
		m_pShareMemory = NULL;
	}

	if (m_hShareMemoryFile)
	{
		CloseHandle(m_hShareMemoryFile);
		m_hShareMemoryFile = NULL;
	}

	if (m_hEventControl)
	{
		CloseHandle(m_hEventControl);
		m_hEventControl = NULL;
	}
	DeleteCriticalSection(&m_csSection);
}
