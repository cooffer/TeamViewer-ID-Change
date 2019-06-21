#include "stdafx.h"
#include "AdapterInfo.h"

PlanInfo::PlanInfo()
{
	m_IsDNSDHCP = TRUE;
	m_IsDNSDHCP = TRUE;
	strcpy(m_IpAddress, "0.0.0.0");
	strcpy(m_Mask, "0.0.0.0");
	strcpy(m_GateWay, "0.0.0.0");
	strcpy(m_MacAddr, "");
}

PlanInfo::~PlanInfo()
{
	;
}

BOOL PlanInfo::SetConnectName(char *pName)
{
	strcpy(m_ConnectName, pName);
	return TRUE;
}

BOOL PlanInfo::SetDHCPFlag(BOOL flag)
{
	m_IsIpDHCP = flag;
	return TRUE;
}

BOOL PlanInfo::SetIpInfo(char *pIpAddr, char *pMask, char *pGateWay)
{
	strcpy(m_IpAddress, pIpAddr);
	strcpy(m_Mask, pMask);
	strcpy(m_GateWay, pGateWay);
	return TRUE;
}

BOOL PlanInfo::SetDns(char *pPrimaryDns, char *pBackupDns)
{
	strcpy(m_PrimaryDns, pPrimaryDns);
	strcpy(m_BackupDns, pBackupDns);
	return TRUE;
}

BOOL PlanInfo::SetMacAddr(char *pMac)
{
	strcpy(m_MacAddr, pMac);
	return TRUE;
}