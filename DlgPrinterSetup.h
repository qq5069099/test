#pragma once


// CDlgPrinterSetup 对话框

class CDlgPrinterSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPrinterSetup)
public:
	int								m_nBeginIndex;				//开始索引
	int								m_nEndIndex;				//结束索引

public:
	CDlgPrinterSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPrinterSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRINTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
