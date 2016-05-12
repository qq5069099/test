// DlgLogon.cpp : 实现文件
//

#include "stdafx.h"
#include "Sdup.h"
#include "DlgLogon.h"
#include "afxdialogex.h"


// CDlgLogon 对话框

IMPLEMENT_DYNAMIC(CDlgLogon, CDialogEx)

CDlgLogon::CDlgLogon(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogon::IDD, pParent)
#ifdef _DEBUG

//#define SERVER
#ifdef SERVER
	, m_strServerIP(_T("server"))
	, m_strDataBaseName(_T("ZCL"))
	, m_strUserName(_T("sa"))
	, m_strPassword(_T("139139"))
#else
	, m_strServerIP(_T("192.168.1.112"))
	, m_strDataBaseName(_T("jj"))
	, m_strUserName(_T("sa"))
	, m_strPassword(_T("139139"))
#endif
#else
	, m_strServerIP(_T(""))
	, m_strDataBaseName(_T(""))
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
#endif
{

}

CDlgLogon::~CDlgLogon()
	
{
}

void CDlgLogon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUserName);
	DDX_Text(pDX, IDC_EDIT2, m_strPassword);
	DDX_Text(pDX, IDC_EDIT3, m_strServerIP);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT4, m_strDataBaseName);
}


BEGIN_MESSAGE_MAP(CDlgLogon, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLogon::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLogon 消息处理程序


void CDlgLogon::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_strServerIP.IsEmpty() || m_strUserName.IsEmpty() || m_strPassword.IsEmpty()) exit(0);

	CDialogEx::OnOK();
}


BOOL CDlgLogon::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{ 
		if(pMsg->wParam == VK_RETURN && GetFocus() && m_btnOK.GetSafeHwnd() != GetFocus()->GetSafeHwnd())
			pMsg->wParam = VK_TAB;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgLogon::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRet = CDialogEx::OnWndMsg(message, wParam, lParam, pResult);

	if (message = WM_KEYDOWN && wParam == VK_TAB && GetFocus() && m_btnOK.GetSafeHwnd() == GetFocus()->GetSafeHwnd())
		PostMessage(WM_KEYDOWN, VK_RETURN, wParam);

	return bRet;
}
