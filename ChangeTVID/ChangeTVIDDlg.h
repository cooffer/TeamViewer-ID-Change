
// ChangeTVIDDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "AdapterInfo.h"
#include <winsvc.h>

// CChangeTVIDDlg 对话框
class CChangeTVIDDlg : public CDialogEx
{
// 构造
public:
	CChangeTVIDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHANGETVID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	AdapterInfo m_AdapterInfo;        //网卡信息
	void GuiShowByAdaterInfo(PIP_ADAPTER_INFO pIpAdapterInfo);
	// 生成的消息映射函数
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
