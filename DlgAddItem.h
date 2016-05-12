#pragma once
#include "afxwin.h"
#include "DataBaseService.h"
#include "resource.h"
#include "GlobalDef.h"

// CDlgAddItem �Ի���

class CDlgAddItem : public CDialogEx
{
protected:
	friend class CSdupDlg;
	CSdupDlg								* m_pSdupDlg;

public:
	CButton									m_btnOK;
	CString									m_strSdupValue;
	const tagKind							&m_Kind;
	tagSdupItem								&m_SdupItem;
	bool									m_bDeleteSdupItem;			//ɾ����Ŀ

public:
	DECLARE_DYNAMIC(CDlgAddItem)

public:
	CDlgAddItem(tagSdupItem &SdupItem, const tagKind &Kind, CSdupDlg * pSdupDlg);   // ��׼���캯��
	virtual ~CDlgAddItem();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//�����Ŀ
	afx_msg void OnBnClickedOk();
	//��ʼ��
	virtual BOOL OnInitDialog();
	//ɾ����Ŀ
	afx_msg void OnBnClickedButton1();
	
	afx_msg void OnStnClickedStaticKindname();
};
