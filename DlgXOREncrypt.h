#pragma once


// CDlgXOREncrypt �Ի���

class CDlgXOREncrypt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgXOREncrypt)

public:
	CDlgXOREncrypt(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgXOREncrypt();

// �Ի�������
	enum { IDD = IDD_DIALOG_XOR_ENCRYPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
