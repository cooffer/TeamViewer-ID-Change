
// ChangeTVIDDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChangeTVID.h"
#include "ChangeTVIDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CChangeTVIDDlg �Ի���



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


// CChangeTVIDDlg ��Ϣ�������

BOOL CChangeTVIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MINIMIZE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	CWnd WndHandle;//���������ͱ��� 

	WndHandle.FindWindowA(_T("Shell_TrayWnd"), NULL);
	WndHandle.SetLayeredWindowAttributes(LWA_ALPHA, 100, 0x1 | 0x2);
	//Ӧ�ó����ʼ��
	InitSock();                 //��ʼ��Socket
	InitIpAddrsInfo();			//��ʼ��������������Ϣ
	InitGuiInfo();              //��ʼ���ؼ���Ϣ
	OnBnClickedCreatemac();		//�����µ�MAC��ַ

	MessageBoxA("��ʹ�ñ�����ǰ��������MAC��ַ�ı��ݣ�\n\
�Է��޸�MAC��ַ���޷���������ʱ�Ա�ָ���\n\n\
�˹���ͨ���޸�����MAC��ַ�ķ�ʽ���ﵽ�޸�TV ID��Ŀ�ģ�\n\
�޸���ɺ������������ԣ����ҳ�����Զ�����TV��\n\
����޸�MAC��ַ��ʱ��(����5����)�޷��������磬\n\
����������MAC��ַ���ٴ��޸ļ��ɡ�\n\n\
���������ѧϰ�������Ͻ�������ҵ��;��Υ�ߺ���Ը���\n\n\
		     51��Դʱ��Byһ�����Ȳ�����","����",MB_OK);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

LRESULT CChangeTVIDDlg::OnModifyComplete(WPARAM wParam, LPARAM lParam)
{
	this->SetWindowText("TVID��������By51��Դʱ��&һ�����Ȳ����� V1.0");
	this->EnableWindow(TRUE);
	if (m_bIsModifySuccess)
	{
		int msgCode = MessageBox("MAC��ַ�޸���ɣ�����TV����Ч���Ƿ���������TV��", "��ʾ", MB_YESNO);
		if (msgCode == IDYES)
			restartTvService();
	}
	else
	{
		MessageBox("MAC��ַ�޸�ʧ�ܣ������ȫ�����ʾ�Ƿ������޸�ע�����ѡ������", "��ʾ", MB_OK);
	}
	UpdateIpAddrsInfo();//���µ�ַ��Ϣ
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChangeTVIDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChangeTVIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�����������Ϣ��ص���������
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

//��ʼ���ؼ���Ϣ
void CChangeTVIDDlg::InitGuiInfo()
{
	//��ʼ��������Combo box��Ϣ,��ʾ��һ������������Ϣ
	for (int i = 0; i < m_AdapterInfo.iCount; i++)
	{
		//AfxMessageBox(m_AdapterInfo.csConnectName[i], MB_OK);
		m_cNetWorkDeviceInfo.InsertString(i, m_AdapterInfo.csConnectName[i]);
	}
	m_cNetWorkDeviceInfo.SetCurSel(0);

	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;

	GuiShowByAdaterInfo(pIpAdapterInfo);
}

//��ʼ��Socket����
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
	*ͨ��mprapi���ȡ��������
	*��ͨ��index��������Ϣ���������������
	********************************************/
	HANDLE   hMprConfig;                    //������Ϣ�ľ��
	DWORD   dwRet = 0;                        //����ֵ
	PIP_INTERFACE_INFO   plfTable = NULL;   //�ӿ���Ϣ��
	DWORD   dwBufferSize = 0;                 //�ӿ���Ϣ��ռ��С

	m_AdapterInfo.csConnectName = new char[m_AdapterInfo.iCount][256];  //����ռ�洢������

	dwRet = MprConfigServerConnect(NULL, &hMprConfig);  //��þ��
	dwRet = GetInterfaceInfo(NULL, &dwBufferSize);      //��ýӿ���Ϣ���С

	if (dwRet == ERROR_INSUFFICIENT_BUFFER)              //��ýӿ���Ϣ
	{
		plfTable = (PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
		GetInterfaceInfo(plfTable, &dwBufferSize);
	}


	TCHAR   szFriendName[256];                   //�ӿ�����
	DWORD   tchSize = sizeof(TCHAR)* 256;
	ZeroMemory(&szFriendName, tchSize);

	for (UINT i = 0; i < plfTable->NumAdapters; i++)
	{
		IP_ADAPTER_INDEX_MAP   AdaptMap;         //�ӿ���Ϣ
		AdaptMap = plfTable->Adapter[i];

		dwRet = MprConfigGetFriendlyName(hMprConfig, AdaptMap.Name,
			(PWCHAR)szFriendName, tchSize);      //�����������unicode 
		USES_CONVERSION;
		char* pName = W2A((PWCHAR)szFriendName);                           //ת��Ϊansi

		InsertConnectName(AdaptMap.Index, pName);                          //����Index�洢������Ϣ                                         
	}
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, plfTable);
}


//��ȡIP��ַ��Ϣ
void CChangeTVIDDlg::GetIpAddrsInfo()
{
	/*******************************************
	*ͨ��Iphlpapi���ȡ������Ϣ�͸���
	********************************************/
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ULONG stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //������С

	if (ERROR_BUFFER_OVERFLOW == nRel)                      //������������Ҫ�Ŀռ�
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}

	if (ERROR_SUCCESS == nRel)                              //��ȡ��Ϣ�ɹ�
	{
		m_AdapterInfo.pIpAdapterInfo = pIpAdapterInfo;
		m_AdapterInfo.iCount = 0;
		while (pIpAdapterInfo)                          //��ȡ��������
		{
			m_AdapterInfo.iCount++;
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
}

//��ʼ��������������Ϣ
void CChangeTVIDDlg::InitIpAddrsInfo()
{
	GetIpAddrsInfo();
	GetConnectNames();
}

//����������������Ϣ
void CChangeTVIDDlg::UpdateIpAddrsInfo()
{
	delete m_AdapterInfo.pIpAdapterInfo;
	m_AdapterInfo.pIpAdapterInfo = NULL;
	m_AdapterInfo.iCount = 0;
	GetIpAddrsInfo();
}


//MAC��ַ��ʾ
void CChangeTVIDDlg::GuiShowByAdaterInfo(PIP_ADAPTER_INFO pIpAdapterInfo)
{
	if (pIpAdapterInfo->DhcpEnabled)
	{}
	else
	{}
	//MAC��ַ
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

//ѡ��ͬ������������
void CChangeTVIDDlg::OnCbnSelchangeAdapterinfocombo()
{
	int iPos = m_cNetWorkDeviceInfo.GetCurSel();
	//��ʼ��Combox��Ϣ,��ʾ��һ������������Ϣ
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
	//����Gui��Ϣ
	int index = m_cNetWorkDeviceInfo.FindString(0, pPlan->m_ConnectName);
	if (index == CB_ERR)
	{
		AfxMessageBox("û���ҵ��÷������������ӣ�", MB_OK);
		return FALSE;
	}
	m_cNetWorkDeviceInfo.SetCurSel(index);

	//MAC��ַ
	//m_tMacAddr.Format("%s", pPlan->m_MacAddr);
	m_tMacCtl.SetWindowText(pPlan->m_MacAddr);

	//UpdateData(FALSE);
	return TRUE;
}


UINT CChangeTVIDDlg::ModifyAdress(LPVOID lParam)
{
	CChangeTVIDDlg* pDlg = (CChangeTVIDDlg*)lParam;
	pDlg->SetMacByRegist();     //�޸�MAC
	pDlg->SendMessage(WM_MSG_MODIFY_COMPLETE, NULL, NULL);
	return 0;
}

//�޸�ע�����Ϣ���޸�MAc,ͨ����������������
BOOL CChangeTVIDDlg::SetMacByRegist()
{
	//���������instance
	//��õ�ǰ��������������
	int iPos = m_cNetWorkDeviceInfo.GetCurSel();
	CString csAdapterName;
	m_cNetWorkDeviceInfo.GetLBText(iPos, csAdapterName);

	PIP_ADAPTER_INFO pIpAdapterInfo = m_AdapterInfo.pIpAdapterInfo;
	//�ҵ���Ӧ����������Ϣ
	for (int i = 0; i < m_AdapterInfo.iCount; i++)
	{
		if (strcmp(m_AdapterInfo.csConnectName[i], csAdapterName.GetBuffer(0)) == 0)
			break;
		pIpAdapterInfo = pIpAdapterInfo->Next;
	}
	//���������instance
	char *strNetCfgInstanceId = pIpAdapterInfo->AdapterName;
	//���MAC
	CString csMac;
	m_tNewMacCtl.GetWindowText(csMac);
	if (csMac == "")
		return TRUE;
	char *lpszMacID = csMac.GetBuffer(0);


	//�޸�ע����޸�mAC
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
		AfxMessageBox("��ע���ʧ��", MB_OK);
	}
	if (m_bIsModifySuccess)
	{	//��Ϊ�޸�MAC����Ҫ����������ʹ���µ�MAC
		//��������
		CString csValue;
		//��������
		m_cNetWorkDeviceInfo.GetWindowText(csValue);
		CString CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" DISABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));

		//��������
		CommandLine = "";
		CommandLine += "/c netsh interface set interface \"" + csValue + "\" ENABLED";
		ExcuteCommand(CommandLine.GetBuffer(0));
	}
	return bRet;
}

//����cmdִ�� netsh������ȴ��������
void CChangeTVIDDlg::ExcuteCommand(char* pCommandParam)
{
	//��ʼ��shellexe��Ϣ
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

	//ִ������
	ShellExecuteEx(&ExeInfo);

	//�ȴ����̽���
	WaitForSingleObject(ExeInfo.hProcess, INFINITE);
}

void CChangeTVIDDlg::OnBnClickedSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->SetWindowText("�����޸�MAC��ַ....");
	this->EnableWindow(FALSE);

	CWinThread * pThread = AfxBeginThread(ModifyAdress, (LPVOID)this);
}


void CChangeTVIDDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CChangeTVIDDlg::OnBnClickedCreatemac()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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


//ֹͣ����
void CChangeTVIDDlg::stopTvService()
{
	// �򿪷���������
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// �򿪷���
	SC_HANDLE hSvc = ::OpenService(hSC, "TeamViewer",
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open TeamViewer error��");
		::CloseServiceHandle(hSC);
		return;
	}
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		TRACE("Get Service state error��");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	//�������ֹͣ״̬���������񣬷���ֹͣ����
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// ֹͣ����
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			TRACE("stop service error��");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// �ȴ�����ֹͣ
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				//AfxMessageBox("stop success��");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
}

//��������
void CChangeTVIDDlg::startTvService()
{
	// �򿪷���������
	SC_HANDLE hSC = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// �򿪷���
	SC_HANDLE hSvc = ::OpenService(hSC, "TeamViewer", SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open TeamViewer error��");
		::CloseServiceHandle(hSC);
		return;
	}
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		TRACE("Get Service state error��");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// ��������
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			TRACE("start service error��");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// �ȴ���������
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox("TeamViewer �����ɹ�","��ʾ", MB_OK);
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
