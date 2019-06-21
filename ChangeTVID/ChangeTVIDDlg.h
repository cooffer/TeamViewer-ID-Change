
// ChangeTVIDDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "AdapterInfo.h"
#include <winsvc.h>

// CChangeTVIDDlg �Ի���
class CChangeTVIDDlg : public CDialogEx
{
// ����
public:
	CChangeTVIDDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHANGETVID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	AdapterInfo m_AdapterInfo;        //������Ϣ
	void GuiShowByAdaterInfo(PIP_ADAPTER_INFO pIpAdapterInfo);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	void GetIpAddrsInfo();
	void GetConnectNames();
	BOOL InitSock();
	void InitGuiInfo();
	void InitIpAddrsInfo();
	void UpdateIpAddrsInfo();
	void InsertConnectName(ULONG index, char *pName);
	BOOL GuiShowByPlanInfo(PlanInfo *pPlan);
	BOOL SetMacByRegist();

	void ExcuteCommand(char* pCommand);
	static UINT ModifyAdress(LPVOID lParam);
	afx_msg LRESULT OnModifyComplete(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_tMacCtl;
	CEdit m_tNewMacCtl;
	CComboBox m_cNetWorkDeviceInfo;
	BOOL m_bIsModifySuccess = FALSE;
	afx_msg void OnCbnSelchangeAdapterinfocombo();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCreatemac();
	afx_msg void stopTvService();
	afx_msg void startTvService();
	afx_msg void restartTvService();
};
