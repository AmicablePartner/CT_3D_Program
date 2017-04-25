using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace CTShareMemory
{
	public delegate void NotifyShareMemory(String szText, long lSize);

	[StructLayoutAttribute(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 1)]
	public class _ShareMemoryInfo
    {
		[MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 4)]
		public Byte[] lWriteTime ;      //* 写入的时间.
		[MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 4)]
		public Byte[] lReadTime;       //* 读取的时间.
		[MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 4)]
		public Byte[] lCurSize;         //* 当前数据长度.
		[MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 1024)]
		public Byte[] ptrData;        //* 写入的内容.

		public _ShareMemoryInfo()
		{
			lWriteTime = new Byte[4];
			lReadTime = new Byte[4];
			lCurSize = new Byte[4];
			ptrData = new Byte[1024];
		}
	}

	class ShareMemory
    {
		public int m_ShareMemorySize = 1036;
		public int m_ShareMemoryContextSize = 1024;
		public IntPtr m_pShareMemory;

		[DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SendMessage(IntPtr hWnd, int Msg, int wParam, IntPtr lParam);

        [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr CreateFileMapping(int hFile, IntPtr lpAttributes, uint flProtect, uint dwMaxSizeHi, uint dwMaxSizeLow, string lpName);

        [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr OpenFileMapping(int dwDesiredAccess, [MarshalAs(UnmanagedType.Bool)] bool bInheritHandle, string lpName);

        [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr MapViewOfFile(IntPtr hFileMapping, uint dwDesiredAccess, uint dwFileOffsetHigh, uint dwFileOffsetLow, uint dwNumberOfBytesToMap);

        [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
        public static extern bool UnmapViewOfFile(IntPtr pvBaseAddress);

        [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
        public static extern bool CloseHandle(IntPtr handle);

        [DllImport("kernel32", EntryPoint = "GetLastError")]
        public static extern int GetLastError();

		[DllImport("kernel32.dll")]
		static extern uint GetTickCount();

		const int ERROR_ALREADY_EXISTS = 183;

        const int FILE_MAP_COPY = 0x0001;
        const int FILE_MAP_WRITE = 0x0002;
        const int FILE_MAP_READ = 0x0004;
        const int FILE_MAP_ALL_ACCESS = 0x0002 | 0x0004;

        const int PAGE_READONLY = 0x02;
        const int PAGE_READWRITE = 0x04;
        const int PAGE_WRITECOPY = 0x08;
        const int PAGE_EXECUTE = 0x10;
        const int PAGE_EXECUTE_READ = 0x20;
        const int PAGE_EXECUTE_READWRITE = 0x40;

        const int SEC_COMMIT = 0x8000000;
        const int SEC_IMAGE = 0x1000000;
        const int SEC_NOCACHE = 0x10000000;
        const int SEC_RESERVE = 0x4000000;

        const int INVALID_HANDLE_VALUE = -1;

        IntPtr	m_hSharedMemoryFile = IntPtr.Zero;
		string	m_strMemoryName;

		Thread m_DataThead = null;
		bool m_bStop = false;
		bool m_bPause = false;
        Semaphore semDealData;
		AutoResetEvent m_EventCtrl = new AutoResetEvent(false);

		NotifyShareMemory m_pCallBackFunc = null;

		_ShareMemoryInfo m_pTmp = new _ShareMemoryInfo();

		public ShareMemory()
        {
        }
        ~ShareMemory()
        {
            Close();
        }

        /// 
        /// 初始化共享内存
        /// 
        /// 共享内存名称
        /// 共享内存大小
        /// 
        public int Init(bool bCreate, string strName)
		{
			SetSemaphore();
			if (bCreate)
            {
                return CreateShareMemory(strName);
            }
            else
            {
                return OpenShareMemory(strName);
            }
        }

        public int OpenShareMemory(string strName)
        {
            m_hSharedMemoryFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, strName);
			if (IntPtr.Zero != m_hSharedMemoryFile)
			{
				//创建内存映射
				m_pShareMemory = MapViewOfFile(m_hSharedMemoryFile, FILE_MAP_ALL_ACCESS, 0, 0, (uint)m_ShareMemorySize);
			}
			else
			{
				if (m_strMemoryName == null)
				{
					m_strMemoryName = strName;
					Thread thread = new Thread(new ThreadStart(OnOpenMemoryThread));
					thread.Start();
				}
			}
            return 0;     //创建成功
        }

		public void OnOpenMemoryThread()
		{
			while (IntPtr.Zero == m_hSharedMemoryFile)
			{
				OpenShareMemory(m_strMemoryName);
				Thread.Sleep(500);
			}
		}

		public void SetCallBackFunc(NotifyShareMemory pFunc)
		{
			m_pCallBackFunc = pFunc;
		}

		public int CreateShareMemory(string strName)
        {
			if (strName.Length > 0)
			{
				//创建内存共享体(INVALID_HANDLE_VALUE)
				m_hSharedMemoryFile = CreateFileMapping(INVALID_HANDLE_VALUE, IntPtr.Zero, (uint)PAGE_READWRITE, 0, (uint)m_ShareMemorySize, strName);
				if (IntPtr.Zero != m_hSharedMemoryFile)
				{
					//创建内存映射
					m_pShareMemory = MapViewOfFile(m_hSharedMemoryFile, FILE_MAP_ALL_ACCESS, 0, 0, (uint)m_ShareMemorySize);
				}
			}
			return 0;
        }

        /// 
        /// 关闭共享内存
        /// 
        public void Close()
        {
			if (m_DataThead.IsAlive)
			{
				semDealData.WaitOne();
				m_bStop = true;
				m_DataThead.Abort();
				semDealData.Release();
			}
			UnmapViewOfFile(m_pShareMemory);
            CloseHandle(m_hSharedMemoryFile);
			
        }

        public bool SetSemaphore()
        {
            try
            {
                semDealData = Semaphore.OpenExisting("DealDataShareMemory");
            }
            catch (Exception)
            {
                semDealData = new Semaphore(1, 1, "DealDataShareMemory");
            }
            return true;
        }

		public void OnRun()
		{
			if (m_DataThead == null)
			{
				m_DataThead = new Thread(new ThreadStart(OnDealDataThread));
				m_DataThead.Start();
			}
			if (!m_DataThead.IsAlive)
			{
				if (m_bPause)
				{
					m_EventCtrl.Set();
				}
				m_DataThead.Start();
			}
		}

		public void OnPause()
		{
			if (m_DataThead.IsAlive)
			{
				m_bPause = true;
			}
		}

		public void OnDealDataThread()
		{
			while (!m_bStop)
			{
				if (m_bPause)
				{
					m_EventCtrl.WaitOne();
					m_bPause = false;
				}
				if (m_pShareMemory != IntPtr.Zero)
				{
					semDealData.WaitOne();

					Marshal.PtrToStructure(m_pShareMemory, m_pTmp);

					if (CharArrayToLong(ref m_pTmp.lReadTime) < CharArrayToLong(ref m_pTmp.lWriteTime))
					{
						LongToCharArray(ref m_pTmp.lReadTime, GetTickCount());
						if (m_pCallBackFunc != null)
						{
							m_pCallBackFunc(System.Text.Encoding.Default.GetString(m_pTmp.ptrData), CharArrayToLong(ref m_pTmp.lCurSize));
						}
						Marshal.StructureToPtr(m_pTmp, m_pShareMemory, false);
					}
					semDealData.Release();
				}
				Thread.Sleep(30);
			}
		}

		public long CharArrayToLong(ref Byte[] ayInfo)
		{
			long lNum = 0;
			lNum += ayInfo[0];
			lNum += ayInfo[1] << 8;
			lNum += ayInfo[2] << 16;
			lNum += ayInfo[3] << 24;

			return lNum;
		}

		public void LongToCharArray(ref Byte[] ayInfo, long lNum)
		{
			ayInfo[0] = (Byte)lNum;
			ayInfo[1] = (Byte)(lNum >> 8);
			ayInfo[2] = (Byte)(lNum >> 16);
			ayInfo[3] = (Byte)(lNum >> 24);
		}

		public void WriteShareMemory(string strText, int lngSize)
		{
			semDealData.WaitOne();
			if (m_pTmp != null && m_pShareMemory != IntPtr.Zero)
			{
				LongToCharArray(ref m_pTmp.lWriteTime, GetTickCount());
				LongToCharArray(ref m_pTmp.lCurSize, lngSize);
				
				//m_pTmp.ptrData = System.Text.Encoding.ASCII.GetBytes(strText);

				IntPtr bufferHandler = Marshal.AllocHGlobal(m_pTmp.ptrData.Length);
				for (int index = 0; index < lngSize; index++)
				{
					Marshal.WriteByte(bufferHandler, index, (Byte)strText[index]);
				}
				Marshal.Copy(bufferHandler, m_pTmp.ptrData, 0, lngSize);
				Marshal.FreeHGlobal(bufferHandler);

				//StructType structObject = (StructType)Marshal.PtrToStructure(bufferHandler, typeof(StructType));
				Marshal.StructureToPtr(m_pTmp, m_pShareMemory, false);
			}
            semDealData.Release();
        }
    }
}

