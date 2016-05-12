#pragma once
#include "afxwin.h"
#include "DataBaseService.h"
#include "resource.h"
#include "GlobalDef.h"

// CDlgAddItem 对话框

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
	bool									m_bDeleteSdupItem;			//删除帐目

public:
	DECLARE_DYNAMIC(CDlgAddItem)

public:
	CDlgAddItem(tagSdupItem &SdupItem, const tagKind &Kind, CSdupDlg * pSdupDlg);   // 标准构造函数
	virtual ~CDlgAddItem();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//添加帐目
	afx_msg void OnBnClickedOk();
	//初始化
	virtual BOOL OnInitDialog();
	//删除帐目
	afx_msg void OnBnClickedButton1();
	
	afx_msg void OnStnClickedStaticKindname();
};
