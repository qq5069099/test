#pragma once


// CDlgPrinterSetup �Ի���

class CDlgPrinterSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPrinterSetup)
public:
	int								m_nBeginIndex;				//��ʼ����
	int								m_nEndIndex;				//��������

public:
	CDlgPrinterSetup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPrinterSetup();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRINTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
