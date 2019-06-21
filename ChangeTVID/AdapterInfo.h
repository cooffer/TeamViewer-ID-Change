#ifndef _ADAPTERINFO_H_
#define _ADAPTERINFO_H_

#define MAX_IP_ADDR_LEN 16     //���IP����
#define MAX_MAC_ADDR_LEN 13    //MAC��󳤶�
#define MAX_NAME_LEN 256       //����������Ƴ���

#define CONNECTNAME "CONNECTNAME"
#define IPDHCP "IPDHCP"
#define DNSDHCP "DNSDHCP"
#define IPADDRESS "IPADDRESS"
#define MASK "MASK"
#define GATEWAY "GATEWAY"
#define PRIMARYDNS "PRIMARYDNS"
#define BACKUPDNS "BACKUPDNS"
#define MACADDR   "MACADDR"


#define WM_MSG_MODIFY_COMPLETE WM_USER + 100  //���������Ϣ

typedef struct _AdapterInfo
{
	PIP_ADAPTER_INFO pIpAdapterInfo;      //ͨ��iphlpapi���ȡ
	int iCount;                           //����������
	char (*csConnectName)[MAX_NAME_LEN];  //��������
}AdapterInfo;

class PlanInfo
{
public:
	char m_PlanName[MAX_NAME_LEN];          //��������
	char m_ConnectName[MAX_NAME_LEN];	    //��������
	BOOL m_IsIpDHCP;                        //IP�Ƿ��Զ���ȡ
	BOOL m_IsDNSDHCP;                       //DNS�Ƿ��Զ���ȡ   
	char m_IpAddress[MAX_IP_ADDR_LEN];      //IP��ַ
	char m_Mask[MAX_IP_ADDR_LEN];           //��������
	char m_GateWay[MAX_IP_ADDR_LEN];        //����
	char m_PrimaryDns[MAX_IP_ADDR_LEN];     //��DNS
	char m_BackupDns[MAX_IP_ADDR_LEN];      //����DNS
	char m_MacAddr[MAX_MAC_ADDR_LEN];       //MAC��ַ
	PlanInfo();
	~PlanInfo();
	BOOL SetConnectName(char *pName);
	BOOL SetDHCPFlag(BOOL flag);
	BOOL SetIpInfo(char *pIpAddr, char *pMask, char *pGateWay);
	BOOL SetDns(char *pPrimaryDns, char *pBackupDns);
	BOOL SetMacAddr(char *pMac);
};
#endif