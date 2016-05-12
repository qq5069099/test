// DlgAddItem.cpp : 实现文件
//

#include "stdafx.h"
#include "Sdup.h"
#include "DlgAddItem.h"
#include "afxdialogex.h"
#include "SdupDlg.h"

// CDlgAddItem 对话框

IMPLEMENT_DYNAMIC(CDlgAddItem, CDialogEx)

	CDlgAddItem::CDlgAddItem(tagSdupItem &SdupItem, const tagKind &Kind, CSdupDlg * pSdupDlg)
	: CDialogEx(CDlgAddItem::IDD, NULL)
	, m_pSdupDlg(pSdupDlg)
	, m_SdupItem(SdupItem)
	, m_Kind(Kind)
	, m_bDeleteSdupItem(false)
	, m_strSdupValue(_T(""))
{
}

CDlgAddItem::~CDlgAddItem()
{
}

void CDlgAddItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);

	if (!pDX->m_bSaveAndValidate && m_SdupItem.SdupID != 0)
	{
		if (m_SdupItem.SdupValue[0])
		{
			m_strSdupValue = m_SdupItem.SdupValue;
			DDX_Text(pDX, IDC_EDIT1, CString(m_strSdupValue));
			DDV_MaxChars(pDX, m_strSdupValue, 32);
		}

		if (m_Kind.KindID != 0)
		{
			CString strMoney;
			for (int i = 0; i < m_SdupItem.KindValueCount; i++)
			{
				if (m_SdupItem.pKindValue[i].KindID != 0)
				{
					if (m_SdupItem.pKindValue[i].KindID == m_Kind.KindID)
					{
						if (m_SdupItem.pKindValue[i].Value != 0.0f)
						{
							if (m_SdupItem.pKindValue[i].Value - (int)m_SdupItem.pKindValue[i].Value >= 0.1)
							{
								strMoney.Format("%0.1f", m_SdupItem.pKindValue[i].Value);
							}
							else
							{
								strMoney.Format("%0.0f", m_SdupItem.pKindValue[i].Value);
							}

							SetDlgItemText(IDC_EDIT2, strMoney);
						}

						break;
					}
				}
			}

			if (strMoney.IsEmpty() && m_Kind.AddTotal == false)
			{
				float fMoney = 0.0f;
				
				for (int j = 0; j < m_SdupItem.KindValueCount; j++)
				{
					if (m_SdupItem.pKindValue[j].Value != 0.0f &&
						m_pSdupDlg->GetKind(m_SdupItem.pKindValue[j].KindID) &&
						m_pSdupDlg->GetKind(m_SdupItem.pKindValue[j].KindID)->AddTotal == true)
					{
						fMoney += m_SdupItem.pKindValue[j].Value;
					}
				}

				if (fMoney > 0.0f)
				{
					if (fMoney - (int)fMoney >= 0.1)
					{
						strMoney.Format("%0.1f", fMoney);
					}
					else
					{
						strMoney.Format("%0.0f", fMoney);
					}

					SetDlgItemText(IDC_EDIT2, strMoney);
				}
			}
		}

		if (m_SdupItem.SdupID != 0 && GetDlgItem(IDC_BUTTON1))
		{
			//帐目超时
			ASSERT(m_SdupItem.AtTime != 0);

			CTimeSpan span = CTime::GetTickCount() - CTime(m_SdupItem.AtTime);
			LONGLONG lspan = span.GetTotalHours();
			if (lspan < 0) lspan *= -1;
			if (lspan < 1) GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		}
	}

	DDX_Text(pDX, IDC_STATIC_KINDNAME, CString(m_Kind.KindName));
}


BEGIN_MESSAGE_MAP(CDlgAddItem, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAddItem::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgAddItem::OnBnClickedButton1)
	ON_STN_CLICKED(IDC_STATIC_KINDNAME, &CDlgAddItem::OnStnClickedStaticKindname)
END_MESSAGE_MAP()


// CDlgAddItem 消息处理程序


BOOL CDlgAddItem::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{ 
		if(pMsg->wParam == VK_RETURN && GetFocus() && m_btnOK.GetSafeHwnd() != GetFocus()->GetSafeHwnd())
			pMsg->wParam = VK_TAB;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgAddItem::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRet = CDialogEx::OnWndMsg(message, wParam, lParam, pResult);

	if (message = WM_KEYDOWN && wParam == VK_TAB && GetFocus() && m_btnOK.GetSafeHwnd() == GetFocus()->GetSafeHwnd())
		PostMessage(WM_KEYDOWN, VK_RETURN, wParam);

	return bRet;
}

//添加帐目
void CDlgAddItem::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT1, m_SdupItem.SdupValue, sizeof(m_SdupItem.SdupValue));
	if (m_SdupItem.SdupValue[0] == 0)
	{
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
	}

	CString strMoney;
	GetDlgItemText(IDC_EDIT2, strMoney);
	float fMoney = (float)atof(strMoney);
	if (fMoney == 0.0f && strMoney > "0")
	{
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
		pEdit->SetFocus();
		pEdit->SetSel(0,-1);
		return;
	}

	if (m_Kind.KindID != 0)
	{
		bool bAdd = false;
		for (int i = 0; i < m_SdupItem.KindValueCount; i++)
		{
			if (NULL == m_SdupItem.pKindValue) break;

			if (m_SdupItem.pKindValue[i].KindID == m_Kind.KindID)
			{
				m_SdupItem.pKindValue[i].Value = fMoney;
				bAdd = true;
				break;
			}
		}

		if (bAdd == false)
		{
			for (int i = 0; i < m_SdupItem.KindValueCount; i++)
			{
				if (NULL == m_SdupItem.pKindValue) break;

				if (m_SdupItem.pKindValue[i].KindID == 0)
				{
					m_SdupItem.pKindValue[i].KindID = m_Kind.KindID;
					m_SdupItem.pKindValue[i].Value = fMoney;
					break;
				}
			}
		}
	}

	CDialogEx::OnOK();
}

//初始化
BOOL CDlgAddItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char szText[32] = {0};
	if (GetDlgItemText(IDC_EDIT1, szText, sizeof(szText)) > 0 && m_Kind.KindID != 0)
	{
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
		pEdit->SetFocus();
		pEdit->SetSel(0,-1);
		return FALSE;
	}
	else
	{
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
		pEdit->SetFocus();
		pEdit->SetSel(0,-1);
		return FALSE;
	}
}

//删除帐目
void CDlgAddItem::OnBnClickedButton1()
{
	if (IDYES == MessageBox("真的要删除此帐目吗?", NULL, MB_YESNO))
	{
		m_bDeleteSdupItem = true;
		CDialogEx::OnOK();
	}
}


void CDlgAddItem::OnStnClickedStaticKindname()
{
	CString strText;
	GetDlgItemText(IDC_EDIT1, strText);
	if (strText.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT1, m_Kind.KindName);

		GetDlgItem(IDC_EDIT2)->SetFocus();
	}
}
