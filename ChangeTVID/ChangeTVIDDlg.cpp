
// ChangeTVIDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChangeTVID.h"
#include "ChangeTVIDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeTVIDDlg 对话框



CChangeTVIDDlg::CChangeTVIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeTVIDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangeTVIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NETWORKCARDLIST, m_cNetWorkDeviceInfo);
	DDX_Control(pDX, IDC_OLDMACADDREDIT, m_tMacCtl);
	DDX_Control(pDX, IDC_NEWMACADDREDIT, m_tNewMacCtl);
}

BEGIN_MESSAGE_MAP(CChangeTVIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_NETWORKCARDLIST, &CChangeTVIDDlg::OnCbnSelchangeAdapterinfocombo)
	ON_BN_CLICKED(ID_SAVE, &CChangeTVIDDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CChangeTVIDDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CREATEMAC, &CChangeTVIDDlg::OnBnClickedCreatemac)
	ON_MESSAGE(WM_MSG_MODIFY_COMPLETE, OnModifyComplete)
END_MESSAGE_MAP()


// CChangeTVIDDlg 消息处理程序

BOOL CChangeTVIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MINIMIZE);

	// TODO:  在此添加额外的初始化代码
	CWnd WndHandle;//定义句柄类型变量 

	WndHandle.FindWindowA(_T("Shell_TrayWnd"), NULL);
	WndHandle.SetLayeredWindowAttributes(LWA_ALPHA, 100, 0x1 | 0x2);
	//应用程序初始化
	InitSock();                 //初始化Socket
	InitIpAddrsInfo();			//初始化网络适配器信息
	InitGuiInfo();              //初始化控件信息
	OnBnClickedCreatemac();		//生成新的MAC地址

	MessageBoxA("在使用本工具前，请做好MAC地址的备份，\n\
以防修改MAC地址后无法连接网络时以便恢复。\n\n\
此工具通过修改网卡MAC地址的方式来达到修改TV ID的目的，\n\
修改完成后无需重启电脑，并且程序会自动重启TV，\n\
如果修改MAC地址后长时间(大于5分钟)无法连接网络，\n\
请重新生成MAC地址，再次修改即可。\n\n\
本程序仅供学习交流，严禁用于商业用途，违者后果自负。\n\n\
		     51开源时代By一杯咖啡不加糖","帮助",MB_OK);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

LRESULT CChangeTVIDDlg::OnModifyComplete(WPARAM wParam, LPARAM lParam)
{
	this->SetWindowText("TVID更换工具By51开源时代&一杯咖啡不加糖 V1.0");
	this->EnableWindow(TRUE);
	if (m_bIsModifySuccess)
	{
		int msgCode = MessageBox("MAC地址修改完成！重启TV后生效，是否立即重启TV？", "提示", MB_YESNO);
		if (msgCode == IDYES)
			restartTvService();
	}
	else
	{
		MessageBox("MAC地址修改失败，如果安全软件提示是否允许修改注册表，请选择允许", "提示", MB_OK);
	}
	UpdateIpAddrsInfo();//更新地址信息
	return 0;
}

void CChangeTVIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChangeTVIDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChangeTVIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//插入和网卡信息相关的连接名称
void CChangeTVIDDlg::InsertConnectName(ULONG index, char *pName)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	for (int i = 0; i < m_AdapterInfo.iCount; i++)
	{
		if (pIpAdapterInfo->Index == index)
		{
			strcpy_s(m_AdapterInfo.csConnectName[i], pName);
		}
		pIpAdapterInfo = pIpAdapterInfo->Next;
	}
}

//初始化控件信息
void CChangeTVIDDlg::InitGuiInfo()
{
	//初始化适配器Combo box信息,显示第一个适配器的信息
	for (int i = 0; i < m_AdapterInfo.iCount; i++)
	{
		//AfxMessageBox(m_AdapterInfo.csConnectName[i], MB_OK);
		m_cNetWorkDeviceInfo.InsertString(i, m_AdapterInfo.csConnectName[i]);
	}
	m_cNetWorkDeviceInfo.SetCurSel(0);

	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;

	GuiShowByAdaterInfo(pIpAdapterInfo);
}

//初始化Socket环境
BOOL CChangeTVIDDlg::InitSock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

void CChangeTVIDDlg::GetConnectNames()
{
	/*******************************************
	*通过mprapi库获取连接名称
	*并通过index将网卡信息和连接名称相关联
	********************************************/
	HANDLE   hMprConfig;                    //连接信息的句柄
	DWORD   dwRet = 0;                        //返回值
	PIP_INTERFACE_INFO   plfTable = NULL;   //接口信息表
	DWORD   dwBufferSize = 0;                 //接口信息表空间大小

	m_AdapterInfo.csConnectName = new char[m_AdapterInfo.iCount][256];  //申请空间存储连接名

	dwRet = MprConfigServerConnect(NULL, &hMprConfig);  //获得句柄
	dwRet = GetInterfaceInfo(NULL, &dwBufferSize);      //获得接口信息表大小

	if (dwRet == ERROR_INSUFFICIENT_BUFFER)              //获得接口信息
	{
		plfTable = (PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
		GetInterfaceInfo(plfTable, &dwBufferSize);
	}


	TCHAR   szFriendName[256];                   //接口名称
	DWORD   tchSize = sizeof(TCHAR)* 256;
	ZeroMemory(&szFriendName, tchSize);

	for (UINT i = 0; i < plfTable->NumAdapters; i++)
	{
		IP_ADAPTER_INDEX_MAP   AdaptMap;         //接口信息
		AdaptMap = plfTable->Adapter[i];

		dwRet = MprConfigGetFriendlyName(hMprConfig, AdaptMap.Name,
			(PWCHAR)szFriendName, tchSize);      //获得连接名称unicode 
		USES_CONVERSION;
		char* pName = W2A((PWCHAR)szFriendName);                           //转换为ansi

		InsertConnectName(AdaptMap.Index, pName);                          //根据Index存储名字信息                                         
	}
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, plfTable);
}


//获取IP地址信息
void CChangeTVIDDlg::GetIpAddrsInfo()
{
	/*******************************************
	*通过Iphlpapi库获取网卡信息和个数
	********************************************/
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ULONG stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //获得其大小

	if (ERROR_BUFFER_OVERFLOW == nRel)                      //重新申请所需要的空间
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}

	if (ERROR_SUCCESS == nRel)                              //获取信息成功
	{
		m_AdapterInfo.pIpAdapterInfo = pIpAdapterInfo;
		m_AdapterInfo.iCount = 0;
		while (pIpAdapterInfo)                          //获取网卡个数
		{
			m_AdapterInfo.iCount++;
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
}

//初始化网络适配器信息
void CChangeTVIDDlg::InitIpAddrsInfo()
{
	GetIpAddrsInfo();
	GetConnectNames();
}

//更新网络适配器信息
void CChangeTVIDDlg::UpdateIpAddrsInfo()
{
	delete m_AdapterInfo.pIpAdapterInfo;
	m_AdapterInfo.pIpAdapterInfo = NULL;
	m_AdapterInfo.iCount = 0;
	GetIpAddrsInfo();
}


//MAC地址显示
void CChangeTVIDDlg::GuiShowByAdaterInfo(PIP_ADAPTER_INFO pIpAdapterInfo)
{
	if (pIpAdapterInfo->DhcpEnabled)
	{}
	else
	{}
	//MAC地址
	CString csMac;
	csMac.Format("%02X-%02X-%02X-%02X-%02X-%02X",
		pIpAdapterInfo->Address[0],
		pIpAdapterInfo->Address[1],
		pIpAdapterInfo->Address[2],
		pIpAdapterInfo->Address[3],
		pIpAdapterInfo->Address[4],
		pIpAdapterInfo->Address[5]);
	m_tMacCtl.SetWindowText(csMac);
}

//选择不同的适配器操作
void CChangeTVIDDlg::OnCbnSelchangeAdapterinfocombo()
{
	int iPos = m_cNetWorkDeviceInfo.GetCurSel();
	//初始化Combox信息,显示第一个适配器的信息
	int i = 0;
	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	while (i++ < iPos)
	{
		pIpAdapterInfo = pIpAdapterInfo->Next;
	}
	m_cNetWorkDeviceInfo.SetCurSel(iPos);

	GuiShowByAdaterInfo(pIpAdapterInfo);

	//UpdateData(FALSE);
}

BOOL CChangeTVIDDlg::GuiShowByPlanInfo(PlanInfo *pPlan)
{
	//设置Gui信息
	int index = m_cNetWorkDeviceInfo.FindString(0, pPlan->m_ConnectName);
	if (index == CB_ERR)
	{
		AfxMessageBox("没有找到该方案的网络连接！", MB_OK);
		return FALSE;
	}
	m_cNetWorkDeviceInfo.SetCurSel(index);

	//MAC地址
	//m_tMacAddr.Format("%s", pPlan->m_MacAddr);
	m_tMacCtl.SetWindowText(pPlan->m_MacAddr);

	//UpdateData(FALSE);
	return TRUE;
}


UINT CChangeTVIDDlg::ModifyAdress(LPVOID lParam)
{
	CChangeTVIDDlg* pDlg = (CChangeTVIDDlg*)lParam;
	pDlg->SetMacByRegist();     //修改MAC
	pDlg->SendMessage(WM_MSG_MODIFY_COMPLETE, NULL, NULL);
	return 0;
}

//修改注册表信息来修改MAc,通过命令行重启网卡
BOOL CChangeTVIDDlg::SetMacByRegist()
{
	//获得适配器instance
	//获得当前适配器连接名称
	int iPos = m_cNetWorkDeviceInfo.GetCurSel();
	CString csAdapterName;
	m_cNetWorkDeviceInfo.GetLBText(iPos, csAdapterName);

	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	//找到相应的适配器信息
	for (int i = 0; i < m_AdapterInfo.iCount; i++)
	{
		if (strcmp(m_AdapterInfo.csConnectName[i], csAdapterName.GetBuffer(0)) == 0)
			break;
		pIpAdapterInfo = pIpAdapterInfo->Next;
	}
	//获得适配器instance
	char *strNetCfgInstanceId = pIpAdapterInfo->AdapterName;
	//获得MAC
	CString csMac;
	m_tNewMacCtl.GetWindowText(csMac);
	if (csMac == "")
		return TRUE;
	char *lpszMacID = csMac.GetBuffer(0);


	//修改注册表修改mAC
	BOOL bRet = FALSE;
	HKEY hKey = NULL;
	int error_code = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002bE10318}"), 0, KEY_WOW64_64KEY | KEY_ALL_ACCESS, &hKey);
	if (error_code == ERROR_SUCCESS)
	{
		DWORD dwIndex = 0;
		TCHAR Name[1024];
		DWORD cName = 1024;

		while (RegEnumKeyEx(hKey, dwIndex, Name, &cName,
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{

			HKEY hSubKey = NULL;
			if (RegOpenKeyEx(hKey, Name, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)
			{

				BYTE Data[1204];
				DWORD cbData = 1024;
				if (RegQueryValueEx(hSubKey, _T("NetCfgInstanceId"), NULL, NULL, Data, &cbData) == ERROR_SUCCESS)
				{
					if (_tcscmp((TCHAR*)Data, strNetCfgInstanceId) == 0)
					{
						if (lpszMacID == NULL)
						{
							//Delete the NetCfgInstanceId entry
							LONG nErr = RegDeleteValue(hSubKey, _T("NetworkAddress"));
							if ((nErr == ERROR_SUCCESS) || (nErr == ERROR_FILE_NOT_FOUND))
							{
								bRet = TRUE;
							}
						}
						else
						{
							//Add the NetCfgInstanceId entry
							if (RegSetValueEx(hSubKey, _T("NetworkAddress"), 0, REG_SZ, (const BYTE*)lpszMacID, sizeof(TCHAR)* ((DWORD)_tcslen(lpszMacID) + 1)) == ERROR_SUCCESS)
							{
								bRet = TRUE;
							}
						}
					}
				}
				RegCloseKey(hSubKey);
			}
			cName = 1024;
			dwIndex++;
		}
		RegCloseKey(hKey);
		m_bIsModifySuccess = TRUE;
	}
	else
	{
		m_bIsModifySuccess = FALSE;
		AfxMessageBox("打开注册表失败", MB_OK);
	}
	if (m_bIsModifySuccess)
	{	//因为修改MAC后需要重新启动来使用新的MAC
		//禁用网卡
		CString csValue;
		//连接名称
		m_cNetWorkDeviceInfo.GetWindowText(csValue);
		CString CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" DISABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));

		//启动网卡
		CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" ENABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));
	}
	return bRet;
}

//启动cmd执行 netsh命令，并等待命令结束
void CChangeTVIDDlg::ExcuteCommand(char* pCommandParam)
{
	//初始化shellexe信息
	SHELLEXECUTEINFO   ExeInfo;
	ZeroMemory(&ExeInfo, sizeof(SHELLEXECUTEINFO));
	ExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ExeInfo.lpFile = "cmd.exe";
	ExeInfo.lpParameters = pCommandParam;
	ExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ExeInfo.nShow = SW_HIDE;
	ExeInfo.hwnd = NULL;
	ExeInfo.lpVerb = NULL;
	ExeInfo.lpDirectory = NULL;
	ExeInfo.hInstApp = NULL;

	//执行命令
	ShellExecuteEx(&ExeInfo);

	//等待进程结束
	WaitForSingleObject(ExeInfo.hProcess, INFINITE);
}

void CChangeTVIDDlg::OnBnClickedSave()
{
	// TODO:  在此添加控件通知处理程序代码
	this->SetWindowText("正在修改MAC地址....");
	this->EnableWindow(FALSE);

	CWinThread * pThread = AfxBeginThread(ModifyAdress, (LPVOID)this);
}


void CChangeTVIDDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CChangeTVIDDlg::OnBnClickedCreatemac()
{
	// TODO:  在此添加控件通知处理程序代码
	char buffer[2][9];
	srand((unsigned)time(NULL));
	for (int i = 0; i < 2; i++)
	{
		sprintf(buffer[i], "%08X", (rand() << 16) + rand());
	}
	CString csTempMac, csNewMac;
	csTempMac.Format("%s%s", buffer[0], buffer[1]);
	csNewMac = csTempMac.Mid(1, 12);
	m_tNewMacCtl.SetWindowText(csNewMac);
}


//停止服务
void CChangeTVIDDlg::stopTvService()
{
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// 打开服务。
	SC_HANDLE hSvc = ::OpenService(hSC, "TeamViewer",
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open TeamViewer error。");
		::CloseServiceHandle(hSC);
		return;
	}
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		TRACE("Get Service state error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			TRACE("stop service error。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				//AfxMessageBox("stop success。");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
}

//启动服务
void CChangeTVIDDlg::startTvService()
{
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// 打开服务。
	SC_HANDLE hSvc = ::OpenService(hSC, "TeamViewer", SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open TeamViewer error。");
		::CloseServiceHandle(hSC);
		return;
	}
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		TRACE("Get Service state error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			TRACE("start service error。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务启动
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox("TeamViewer 重启成功","提示", MB_OK);
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
}

void CChangeTVIDDlg::restartTvService()
{
	stopTvService();
	::Sleep(1000);
	startTvService();
}
