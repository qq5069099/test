// DlgPrinterSetup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sdup.h"
#include "DlgPrinterSetup.h"
#include "afxdialogex.h"


// CDlgPrinterSetup �Ի���

IMPLEMENT_DYNAMIC(CDlgPrinterSetup, CDialogEx)

CDlgPrinterSetup::CDlgPrinterSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPrinterSetup::IDD, pParent)
{

	m_nBeginIndex = 0;
	m_nEndIndex = 0;
}

CDlgPrinterSetup::~CDlgPrinterSetup()
{
}

void CDlgPrinterSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nBeginIndex);
	DDX_Text(pDX, IDC_EDIT2, m_nEndIndex);
}


BEGIN_MESSAGE_MAP(CDlgPrinterSetup, CDialogEx)
END_MESSAGE_MAP()


// CDlgPrinterSetup ��Ϣ�������
