#pragma once
#include "afxwin.h"


// CDlgLogon �Ի���

class CDlgLogon : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogon)

public:
	CButton m_btnOK;
	CString m_strServerIP;
	CString m_strDataBaseName;
	CString m_strUserName;
	CString m_strPassword;

public:
	CDlgLogon(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogon();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
