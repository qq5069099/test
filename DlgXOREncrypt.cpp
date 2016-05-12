// DlgXOREncrypt.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sdup.h"
#include "DlgXOREncrypt.h"
#include "afxdialogex.h"
#include "Encrypt.h"


// CDlgXOREncrypt �Ի���

IMPLEMENT_DYNAMIC(CDlgXOREncrypt, CDialogEx)

CDlgXOREncrypt::CDlgXOREncrypt(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgXOREncrypt::IDD, pParent)
{

}

CDlgXOREncrypt::~CDlgXOREncrypt()
{
}

void CDlgXOREncrypt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgXOREncrypt, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgXOREncrypt::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgXOREncrypt ��Ϣ�������


void CDlgXOREncrypt::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	if (str.IsEmpty()) return;

	char szBuf[1024] = { 0 };
	CXOREncrypt::EncryptData(str.GetBuffer(), szBuf, sizeof(szBuf));

	SetDlgItemText(IDC_EDIT2, szBuf);
}
