#pragma once


// CDlgXOREncrypt 对话框

class CDlgXOREncrypt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgXOREncrypt)

public:
	CDlgXOREncrypt(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgXOREncrypt();

// 对话框数据
	enum { IDD = IDD_DIALOG_XOR_ENCRYPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
