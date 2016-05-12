
// SdupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sdup.h"
#include "SdupDlg.h"
#include "afxdialogex.h"
#include "DlgLogon.h"
#include "CrashRpt.h"
#include "DlgPrinterSetup.h"
#include "Encrypt.h"
#include "DlgXOREncrypt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSdupDlg 对话框

CSdupDlg::CSdupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSdupDlg::IDD, pParent)
	, m_PrintDialog(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//清空类型
	ZeroMemory(m_KindArray, sizeof(m_KindArray));
	m_nKindCount = 0;

	m_CurrentLoadType = lTypeAll;
}

void CSdupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtEnd);
}

BEGIN_MESSAGE_MAP(CSdupDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSdupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSdupDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CSdupDlg::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CSdupDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CSdupDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSdupDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON4, &CSdupDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSdupDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSdupDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON8, &CSdupDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CSdupDlg::OnBnClickedButton9)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CSdupDlg::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CSdupDlg::OnNMClickList1)
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CSdupDlg 消息处理程序

BOOL CSdupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//灾难恢复
	CCrashRpt *pCCrashRpt = new CCrashRpt;
	try
	{
		CDlgLogon dlgLogon;
		if (IDOK != dlgLogon.DoModal()) exit(0);

		//连接数据库
		m_DataBase.SetConnectionInfo(dlgLogon.m_strServerIP, 1433, dlgLogon.m_strDataBaseName, dlgLogon.m_strUserName, dlgLogon.m_strPassword);
		m_DataBase.OpenConnection();
	}
#ifdef _DEBUG
	catch (CDataBaseException * e)
	{
		CString strError;
		strError.Format(_T("打开数据库失败 : %s"), e->GetErrorDescribe());

		AfxMessageBox(strError);
	}
#else
	catch (...)
	{
		exit(0);
	}
#endif

	LOGFONT lf; 
	memset(&lf, 0, sizeof(LOGFONT));                     
	lf.lfHeight = 16;                                                   
	sprintf_s(lf.lfFaceName, sizeof(lf.lfFaceName), "宋体");                       
	HFONT hfont = ::CreateFontIndirect(&lf);     
	CFont * font = CFont::FromHandle(hfont); 
	m_ListCtrl.SetFont(font);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//选择整行

	//本月
	OnBnClickedButton5();

	//流水查询
	OnBnClickedButton2();

	ShowWindow(SW_MAXIMIZE);

	::RegisterHotKey(this->GetSafeHwnd(), 1001, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 'E');

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSdupDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSdupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSdupDlg::OnBnClickedOk()
{
}

void CSdupDlg::OnBnClickedCancel()
{
}

//退出程序
void CSdupDlg::OnClose()
{
	//获得路径
	TCHAR szModuleFile[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(), szModuleFile, MAX_PATH);
	PathRemoveFileSpec(szModuleFile);

	if (m_DataBase.IsConnected())
	{
		m_DataBase.ResetParameter();
		m_DataBase.ExecuteProcess("GR_ClearJunkData", true);
	}

	EndDialog(0);
}

void CSdupDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//分条控件
	HDWP hDwp = BeginDeferWindowPos(32);
	const UINT uFlags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS;

	DeferWindowPos(hDwp, m_ListCtrl, NULL, 3, 70, cx - 3 - 3, cy - 70 - 3, uFlags);

	EndDeferWindowPos(hDwp);
}

//流水查询
void CSdupDlg::OnBnClickedButton2()
{
	//加载类型
	LoadKind(lTypeAll);
	//加载数据
	LoadSdupItem(lTypeAll);
	//刷新数据
	FreshListCtrl(true);
}

//收入查询
void CSdupDlg::OnBnClickedButton6()
{
	//加载类型
	LoadKind(lTypeIn);
	//加载数据
	LoadSdupItem(lTypeIn);
	//刷新数据
	FreshListCtrl(true);
}

//支出查询
void CSdupDlg::OnBnClickedButton7()
{
	//加载类型
	LoadKind(lTypeOut);
	//加载数据
	LoadSdupItem(lTypeOut);
	//刷新数据
	FreshListCtrl(true);
}

//今年
void CSdupDlg::OnBnClickedButton4()
{
	CTime timeNow = CTime::GetTickCount();
	m_dtBegin.SetTime(&CTime(timeNow.GetYear(), 1, 1, 0, 0, 0));
	m_dtEnd.SetTime(&CTime(timeNow.GetYear(), 12, 31, 23, 59, 59));
}

//本月
void CSdupDlg::OnBnClickedButton5()
{
	CTime timeNow = CTime::GetTickCount();
	m_dtBegin.SetTime(&CTime(timeNow.GetYear(), timeNow.GetMonth(), 1, 0, 0, 0));
	m_dtEnd.SetTime(&CTime(timeNow.GetYear(), timeNow.GetMonth(), GetMonthDayCount(timeNow), 23, 59, 59));

	//加载类型
	LoadKind(m_CurrentLoadType);
	//加载数据
	LoadSdupItem(m_CurrentLoadType);
	//刷新数据
	FreshListCtrl(false);
}

//今天
void CSdupDlg::OnBnClickedButton1()
{
	CTime timeNow = CTime::GetTickCount();
	m_dtBegin.SetTime(&CTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0));
	m_dtEnd.SetTime(&CTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 23, 59, 59));

	//加载类型
	LoadKind(m_CurrentLoadType);
	//加载数据
	LoadSdupItem(m_CurrentLoadType);
	//刷新数据
	FreshListCtrl(false);
}

//修改选择
void CSdupDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem == -1 ||
		(tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem) == &m_SdupItemTotal ||
		(tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem) == &m_SdupItemTotal2) return;

	if (pNMItemActivate->iSubItem <= 1 || pNMItemActivate->iSubItem == 3) return;

	tagSdupItem * pSdupItem = new tagSdupItem;
	if ((tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem) != NULL)
	{
		if (((tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem))->pKindValue != NULL)
			*pSdupItem = *(tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem);

		//帐目超时
		if (pSdupItem->AtTime != 0 && pNMItemActivate->iSubItem > 3)
		{
			CTimeSpan span = CTime::GetTickCount() - CTime(pSdupItem->AtTime);
			LONGLONG lspan = span.GetTotalHours();
			if (lspan < 0) lspan *= -1;

			if (lspan > 0)
			{
				AfxMessageBox("此帐目超过一小时不再允许修改!");
				delete pSdupItem;
				m_ListCtrl.SetFocus();
				return;
			}
		}
	}
	else
	{
		pSdupItem->SetKindValueCount(m_nKindCount);
		for (int i = 0; i < m_nKindCount; i++)
			pSdupItem->pKindValue[i].KindID = m_KindArray[i].KindID;
	}

	if (pSdupItem->pKindValue == NULL)
	{
		delete pSdupItem;
		return;
	}
	int nSelIndex = pNMItemActivate->iItem;
	

	tagKind Kind;
	ZeroMemory(&Kind, sizeof(Kind));
	if (pNMItemActivate->iSubItem > 3)
	{
		Kind = m_KindArray[pNMItemActivate->iSubItem - 4];
	}

	CDlgAddItem dlg(*pSdupItem, Kind, this);

	if (IDOK == dlg.DoModal())
	{
		char selcom[1024] = {0};
		ZeroMemory(selcom, sizeof(selcom));
		try
		{
			if (dlg.m_bDeleteSdupItem)
			{
				sprintf_s(selcom, "DELETE FROM Sdup WHERE (SdupID=%d)", pSdupItem->SdupID);
				m_DataBase.ExecuteSentence(selcom, true);
				sprintf_s(selcom, "DELETE FROM KindValue WHERE (SdupID=%d)", pSdupItem->SdupID);
				m_DataBase.ExecuteSentence(selcom, true);

				m_SdupItemList.remove((tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem));
				nSelIndex--;
			}
			else if (pSdupItem->SdupID == 0)	//新加帐目
			{
				//加密摘要
				char SdupValue[512] = {0};
				ZeroMemory(SdupValue, sizeof(SdupValue));
				CXOREncrypt::EncryptData(pSdupItem->SdupValue, SdupValue, sizeof(SdupValue));

				m_DataBase.ResetParameter();
				m_DataBase.AddParameter("@SdupValue", SdupValue);
				m_DataBase.ExecuteProcess("GR_InsertSdup", true);
				pSdupItem->SdupID = m_DataBase.GetValue_INT("SdupID");
				pSdupItem->AtTime = m_DataBase.GetValue_TIME("AtTime");

				int iIndex = 0;
				for (; iIndex < pSdupItem->KindValueCount; iIndex++)
				{
					if (pSdupItem->pKindValue[iIndex].KindID == Kind.KindID)
					{
						break;
					}
				}

				if (iIndex < pSdupItem->KindValueCount)
				{
					m_DataBase.ResetParameter();
					m_DataBase.AddParameter("@ValueID", pSdupItem->pKindValue[iIndex].ValueID);
					m_DataBase.AddParameter("@SdupID", pSdupItem->SdupID);
					m_DataBase.AddParameter("@KindID", Kind.KindID);
					m_DataBase.AddParameter("@Value", pSdupItem->pKindValue[iIndex].Value/100.0f);
					m_DataBase.ExecuteProcess("GR_InsertKindValue", true);
					pSdupItem->pKindValue[iIndex].ValueID = m_DataBase.GetValue_INT("ValueID");
					pSdupItem->pKindValue[iIndex].KindID = Kind.KindID;
				}

				m_SdupItemList.push_back(pSdupItem);
				nSelIndex = m_SdupItemList.size() - 1;
				pSdupItem = NULL;
			}
			else		//修改帐目
			{
				char SdupValue[512] = {0};
				ZeroMemory(SdupValue, sizeof(SdupValue));
				CXOREncrypt::EncryptData(pSdupItem->SdupValue, SdupValue, sizeof(SdupValue));

				sprintf_s(selcom, "UPDATE Sdup SET [SdupValue]=\'%s\' WHERE (SdupID=%d)", SdupValue, pSdupItem->SdupID);
				m_DataBase.ExecuteSentence(selcom, true);

				int iIndex = 0;
				for (; iIndex < pSdupItem->KindValueCount; iIndex++)
				{
					ASSERT(pSdupItem->pKindValue);
					if (pSdupItem->pKindValue[iIndex].KindID == Kind.KindID)
					{
						break;
					}
				}

				if (Kind.KindID != 0)
				{
					m_DataBase.ResetParameter();
					m_DataBase.AddParameter("@ValueID", pSdupItem->pKindValue[iIndex].ValueID);
					m_DataBase.AddParameter("@SdupID", pSdupItem->SdupID);
					m_DataBase.AddParameter("@KindID", Kind.KindID);
					m_DataBase.AddParameter("@Value", pSdupItem->pKindValue[iIndex].Value/100.0f);
					m_DataBase.ExecuteProcess("GR_InsertKindValue", true);
					if (pSdupItem->pKindValue[iIndex].Value == 0.0f)
					{
						pSdupItem->pKindValue[iIndex].ValueID = 0;
					}
					else
					{
						if (pSdupItem->pKindValue[iIndex].ValueID == 0)
							pSdupItem->pKindValue[iIndex].ValueID = m_DataBase.GetValue_INT("ValueID");
						if (pSdupItem->pKindValue[iIndex].KindID == 0)
							pSdupItem->pKindValue[iIndex].KindID = Kind.KindID;
					}
				}

				*(tagSdupItem *)m_ListCtrl.GetItemData(pNMItemActivate->iItem) = *pSdupItem;
				pSdupItem = NULL;
			}

			CalcSdupItem();
			FreshListCtrl(false, nSelIndex);
		}
		catch (CDataBaseException * e)
		{
			CString strError;
			strError.Format(_T("错误 : %s"), e->GetErrorDescribe());

			AfxMessageBox(strError);
		}
	}

	if (pSdupItem) delete pSdupItem;
	*pResult = 0;
	m_ListCtrl.SetFocus();
}

//上月
void CSdupDlg::OnBnClickedButton8()
{
	CTime timeNow;
	m_dtEnd.GetTime(timeNow);
	timeNow -= CTimeSpan(GetMonthDayCount(timeNow), 0, 0, 0);
	m_dtBegin.SetTime(&CTime(timeNow.GetYear(), timeNow.GetMonth(), 1, 0, 0, 0));
	m_dtEnd.SetTime(&CTime(timeNow.GetYear(), timeNow.GetMonth(), GetMonthDayCount(timeNow), 23, 59, 59));
}

//取月天数
int CSdupDlg::GetMonthDayCount(CTime time)
{
	switch (time.GetMonth())
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
		break;
	case 2:
		if (0 == time.GetYear() % 4)
			return 29;
		else
			return 28;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	default:
		return 30;
	}
}

//加载类型
void CSdupDlg::LoadKind(eLoadType LoadType)
{
	//清空类型
	ZeroMemory(m_KindArray, sizeof(m_KindArray));
	m_nKindCount = 0;
	m_CurrentLoadType = LoadType;
	m_ListCtrl.m_nSelColIndex = -1;

	//先查表头
	char selcom[1024] = {0};
	ZeroMemory(selcom, sizeof(selcom));

	switch (LoadType)
	{
	case lTypeIn:
		{
			sprintf_s(selcom, "SELECT * FROM Kind WHERE (IsOut=0) ORDER BY SortID ASC");
			break;
		}
	case lTypeOut:
		{
			sprintf_s(selcom, "SELECT * FROM Kind WHERE (IsOut=1) ORDER BY SortID ASC");
			break;
		}
	case lTypeAll:
		{
			sprintf_s(selcom, "SELECT * FROM Kind ORDER BY SortID ASC");
			break;
		}
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	try
	{
		m_DataBase.ExecuteSentence(selcom, true);

		while(!m_DataBase.IsRecordsetEnd())//获取具体的数据
		{
			m_KindArray[m_nKindCount].KindID = m_DataBase.GetValue_INT("KindID");
			char szBuf[512] = {0};
			m_DataBase.GetValue_String("KindName", szBuf, sizeof(szBuf));
			CXOREncrypt::CrevasseData(szBuf, m_KindArray[m_nKindCount].KindName, sizeof(m_KindArray[m_nKindCount].KindName));
			m_KindArray[m_nKindCount].IsOut = m_DataBase.GetValue_BOOL("IsOut");
			m_KindArray[m_nKindCount].AddTotal = m_DataBase.GetValue_BOOL("AddTotal");
			m_KindArray[m_nKindCount].Width = m_DataBase.GetValue_INT("Width");

			m_nKindCount++;
			m_DataBase.MoveToNext();
		}

		//设置数量
		for (int i = 0; i < CountArray(m_SdupMonthTotal); i++)
		{
			m_SdupMonthTotal[i].SetKindValueCount(m_nKindCount);
			m_SdupMonthTotal[i].SdupID = 0xFFFE;
		}

		//设置统计
		m_SdupItemTotal.SetKindValueCount(m_nKindCount);
		m_SdupItemTotal.SdupID = 0xFFFF;
		for (int i = 0; i < m_nKindCount; i++)
		{
			m_SdupItemTotal.pKindValue[i].KindID = m_KindArray[i].KindID;
		}

		m_SdupItemTotal2.SetKindValueCount(m_nKindCount);
		m_SdupItemTotal2.SdupID = 0xFFFF;
		for (int i = 0; i < m_nKindCount; i++)
		{
			m_SdupItemTotal2.pKindValue[i].KindID = m_KindArray[i].KindID;
		}
	}
	catch (CDataBaseException * e)
	{
		CString strError;
		strError.Format(_T("错误 : %s"), e->GetErrorDescribe());

		AfxMessageBox(strError);
		return;
	}
}

//加载帐目
void CSdupDlg::LoadSdupItem(eLoadType LoadType)
{
	char selcom[1024] = {0};
	ZeroMemory(selcom, sizeof(selcom));
	CTime timeBegin;
	CTime timeEnd;
	m_dtBegin.GetTime(timeBegin);
	m_dtEnd.GetTime(timeEnd);

	for (list<tagSdupItem*>::iterator i = m_SdupItemList.begin(); i != m_SdupItemList.end(); i++)
	{
		delete (*i);
	}
	//清空数据
	m_SdupItemList.clear();

	switch (LoadType)
	{
	case lTypeIn:
		{
			sprintf_s(selcom, "SELECT * FROM Sdup,KindValue,Kind WHERE (Sdup.SdupID=KindValue.SdupID AND Kind.KindID=KindValue.KindID AND Kind.IsOut=0 AND AtTime >= \'%s\' AND AtTime <= \'%s\') ORDER BY Sdup.SdupID ASC, KindValue.KindID ASC",
				timeBegin.Format("%Y/%m/%d 0:0:0"), timeEnd.Format("%Y/%m/%d 23:59:59"));

			break;
		}
	case lTypeOut:
		{
			sprintf_s(selcom, "SELECT * FROM Sdup,KindValue,Kind WHERE (Sdup.SdupID=KindValue.SdupID AND Kind.KindID=KindValue.KindID AND Kind.IsOut=1 AND AtTime >= \'%s\' AND AtTime <= \'%s\') ORDER BY Sdup.SdupID ASC, KindValue.KindID ASC",
				timeBegin.Format("%Y/%m/%d 0:0:0"), timeEnd.Format("%Y/%m/%d 23:59:59"));

			break;
		}
	case lTypeAll:
		{
			sprintf_s(selcom, "SELECT * FROM Sdup,KindValue WHERE (Sdup.SdupID=KindValue.SdupID AND AtTime >= \'%s\' AND AtTime <= \'%s\') ORDER BY Sdup.SdupID ASC, KindValue.KindID ASC",
				timeBegin.Format("%Y/%m/%d 0:0:0"), timeEnd.Format("%Y/%m/%d 23:59:59"));

			break;
		}
	default:
		{
			ASSERT(FALSE);
			return;
		}
	}

	try
	{
		m_DataBase.ExecuteSentence(selcom, true);

		tagSdupItem * pItem = NULL;
		while(!m_DataBase.IsRecordsetEnd())//获取具体的数据
		{
			//准备数据
			int nSdupID = m_DataBase.GetValue_INT("SdupID");

			if (pItem == NULL || pItem->SdupID != nSdupID)
			{
				pItem = new tagSdupItem;
				pItem->SetKindValueCount(m_nKindCount);
				m_SdupItemList.push_back(pItem);

				pItem->SdupID = m_DataBase.GetValue_INT("SdupID");

				char szBuf[512] = {0};
				m_DataBase.GetValue_String("SdupValue", szBuf, sizeof(szBuf));
				CXOREncrypt::CrevasseData(szBuf, pItem->SdupValue, sizeof(pItem->SdupValue));

				pItem->AtTime = m_DataBase.GetValue_TIME("AtTime");
				pItem->LeftValue = 0.0f;										//结余
			}

			for (int i = 0; i < m_nKindCount; i++)
			{
				if (pItem->pKindValue[i].ValueID == 0)
				{
					pItem->pKindValue[i].ValueID = m_DataBase.GetValue_INT("ValueID");
					pItem->pKindValue[i].KindID = m_DataBase.GetValue_INT("KindID");
					pItem->pKindValue[i].Value = m_DataBase.GetValue_FLOAT("Value") * 100.0f;
					break;
				}
			}

			m_DataBase.MoveToNext();
		}
	}
	catch (CDataBaseException * e)
	{
		CString strError;
		strError.Format(_T("错误 : %s"), e->GetErrorDescribe());

		AfxMessageBox(strError);
	}

	//计算帐务
	CalcSdupItem();
}

//获取类型
tagKind * CSdupDlg::GetKind(int nKineID)
{
	for (int i = 0; i < m_nKindCount; i++)
	{
		if (m_KindArray[i].KindID == nKineID)
			return &m_KindArray[i];
	}

	ASSERT(FALSE);
	return NULL;
}

//统计数据+
float CSdupDlg::Total2(int nPosKindID, int nTotalKindArray[], int nSize)
{
	float fMenoy = 0.0f;
	for (int v = 0; v < nSize; v++)
	{
		for (int i = 0; i < m_nKindCount; i++)
		{
			if (m_SdupItemTotal.pKindValue[i].KindID == nTotalKindArray[v])
			{
				tagKind * pKind = GetKind(nTotalKindArray[v]);
				if (pKind)
				{
					fMenoy += (pKind->IsOut) ? (m_SdupItemTotal.pKindValue[i].Value * -1) : m_SdupItemTotal.pKindValue[i].Value;
				}
				else
				{
					fMenoy += m_SdupItemTotal.pKindValue[i].Value;
				}
			}
		}
	}
	
	//放好位置
	for (int i = 0; i < m_nKindCount; i++)
	{
		if (m_SdupItemTotal2.pKindValue[i].KindID == nPosKindID)
		{
			m_SdupItemTotal2.pKindValue[i].Value = fMenoy;
		}
	}

	return fMenoy;
}

//计算帐务
void CSdupDlg::CalcSdupItem()
{
	for (list<tagSdupItem*>::iterator i = m_SdupItemList.begin(); i != m_SdupItemList.end(); i++)
	{
		(*i)->LeftValue = 0.0f;
	}
	m_SdupItemTotal.LeftValue = 0.0f;

	if (m_SdupItemTotal.pKindValue)
	for (int i = 0; i < m_SdupItemTotal.KindValueCount; i++)
		m_SdupItemTotal.pKindValue[i].Value = 0.0f;

	m_SdupItemTotal2.LeftValue = 0.0f;

	if (m_SdupItemTotal2.pKindValue)
	for (int i = 0; i < m_SdupItemTotal2.KindValueCount; i++)
		m_SdupItemTotal2.pKindValue[i].Value = 0.0f;

	float fLeftValue = 0.0f;
	for (list<tagSdupItem*>::iterator i = m_SdupItemList.begin(); i != m_SdupItemList.end(); i++)
	{
		for (int j = 0; j < m_nKindCount; j++)
		{
			if ((*i)->pKindValue == NULL) break;
			if ((*i)->pKindValue[j].KindID == 0) break;

			tagKind * pKined = GetKind((*i)->pKindValue[j].KindID);
			float fKindValue = (*i)->pKindValue[j].Value;

			if (pKined->AddTotal)
			{
				(*i)->LeftValue += fKindValue * (pKined->IsOut ? -1 : 1);
			}

			//设置统计
			for (int v = 0; v < m_nKindCount; v++)
			{
				if (m_SdupItemTotal.pKindValue[v].KindID == pKined->KindID)
				{
					//最后一行
					m_SdupItemTotal.pKindValue[v].Value += fKindValue;
					break;
				}
			}
		}

		(*i)->LeftValue += fLeftValue;
		fLeftValue = (*i)->LeftValue;

		//最后一行
		m_SdupItemTotal.LeftValue = fLeftValue;
		m_SdupItemTotal2.LeftValue = fLeftValue;
	}

	strcpy(m_SdupItemTotal.SdupValue, "各列统计");
	//最后一行+
	strcpy(m_SdupItemTotal2.SdupValue, "收支统计");

	//挣钱
	int nKindIDArray2[] = { 3, 4 };
	float fMenoyCLIn = Total2(4, nKindIDArray2, CountArray(nKindIDArray2));

	int nKindIDArray4[] = { 5, 6, 7, 8, 9 };
	float fMenoyCLOut2 = Total2(9, nKindIDArray4, CountArray(nKindIDArray4));

	int nKindIDArray8[] = { 10, 11, 12 };
	float fMenoyCLOut3 = Total2(10, nKindIDArray8, CountArray(nKindIDArray8));

	int nKindIDArray3[] = { 5, 6, 7, 8, 9, 10, 11, 12 };
	float fMenoyCLOut = Total2(12, nKindIDArray3, CountArray(nKindIDArray3));
}

//刷新界面
void CSdupDlg::FreshListCtrl(bool bResetColHead, int nSelIndex/* = -1*/)
{
	m_ListCtrl.SetRedraw(FALSE);

	try
	{
		m_ListCtrl.DeleteAllItems();

		if (bResetColHead)
		{
			//删除所有列
			CHeaderCtrl *pHeaderCtrl = m_ListCtrl.GetHeaderCtrl();
			for (int i = pHeaderCtrl->GetItemCount() - 1; i >= 0; i--)
				m_ListCtrl.DeleteColumn(i);

			int nColIndex = 0;
			m_ListCtrl.InsertColumn(nColIndex++, _T("序"), LVCFMT_CENTER, 40);
			m_ListCtrl.InsertColumn(nColIndex++, _T("日期"), LVCFMT_CENTER, 50);
			m_ListCtrl.InsertColumn(nColIndex++, _T("摘要"), LVCFMT_LEFT, 180);
			m_ListCtrl.InsertColumn(nColIndex++, _T("结余"), LVCFMT_RIGHT, 80);

			for (int i = 0; i < m_nKindCount; i++)
			{
				m_ListCtrl.InsertColumn(nColIndex++, m_KindArray[i].KindName, LVCFMT_RIGHT, m_KindArray[i].Width);
			}
		}

		//刷新数据
		int nItemNum = 1;
		for (auto i = m_SdupItemList.begin(); i != m_SdupItemList.end(); i++)
		{
			tagSdupItem * pItem = *i;
			if (pItem == NULL) continue;
			
			CString strTemp;
			strTemp.Format(_T("%0.4d"), pItem->SdupID);
			strTemp.Format(_T("%0.4d"), nItemNum++);
			int nIndex = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strTemp);
			m_ListCtrl.SetItemData(nIndex, (DWORD_PTR)pItem);

			CTime timeAt(pItem->AtTime);
			m_ListCtrl.SetItemText(nIndex, 1, timeAt.Format("%m-%d"));
			m_ListCtrl.SetItemText(nIndex, 2, CString(pItem->SdupValue));
			strTemp.Format("%0.2f", pItem->LeftValue);
			m_ListCtrl.SetItemText(nIndex, 3, strTemp);

			for (int j = 0; j < CountArray(m_KindArray); j++)
			{
				if (m_KindArray[j].KindID == 0) break;

				for (int u = 0; u < m_nKindCount; u++)
				{
					if (pItem->pKindValue == NULL) break;
					if (pItem->pKindValue[u].KindID == 0) break;

					if (pItem->pKindValue[u].KindID == m_KindArray[j].KindID)
					{
						if (pItem->pKindValue[u].Value != 0.0f)
						{
							if (pItem->pKindValue[u].Value - (int)pItem->pKindValue[u].Value >= 0.1)
								strTemp.Format(_T("%0.1f"), pItem->pKindValue[u].Value);
							else
								strTemp.Format(_T("%0.0f"), pItem->pKindValue[u].Value);

							m_ListCtrl.SetItemText(nIndex, 4 + j, strTemp);
						}
					}
				}
			}
		}

		CString strTemp;
		strTemp.Format(_T("%0.4d"), nItemNum++);
		//空排?
		m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strTemp);

		//最后一行
		strTemp.Format(_T("%0.4d"), nItemNum++);
		int nIndex = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strTemp);
		m_ListCtrl.SetItemData(nIndex, (DWORD_PTR)(&m_SdupItemTotal));

		strTemp.Format("%0.2f", m_SdupItemTotal.LeftValue);
		m_ListCtrl.SetItemText(nIndex, 3, strTemp);

		m_ListCtrl.SetItemText(nIndex, 2, CString(m_SdupItemTotal.SdupValue));

		for (int j = 0; j < CountArray(m_KindArray); j++)
		{
			if (m_KindArray[j].KindID == 0) break;

			for (int u = 0; u < m_nKindCount; u++)
			{
				if (m_SdupItemTotal.pKindValue[u].KindID == 0) break;

				if (m_SdupItemTotal.pKindValue[u].KindID == m_KindArray[j].KindID)
				{
					if (m_SdupItemTotal.pKindValue[u].Value != 0.0f)
					{
						if (m_SdupItemTotal.pKindValue[u].Value - (int)m_SdupItemTotal.pKindValue[u].Value >= 0.1)
							strTemp.Format(_T("%0.1f"), m_SdupItemTotal.pKindValue[u].Value);
						else
							strTemp.Format(_T("%0.0f"), m_SdupItemTotal.pKindValue[u].Value);

						m_ListCtrl.SetItemText(nIndex, 4 + j, strTemp);
					}
				}
			}
		}

		//最后一行+
		strTemp.Format(_T("%0.4d"), nItemNum++);
		nIndex = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strTemp);
		m_ListCtrl.SetItemData(nIndex, (DWORD_PTR)(&m_SdupItemTotal2));

		strTemp.Format("%0.2f", m_SdupItemTotal2.LeftValue);
		m_ListCtrl.SetItemText(nIndex, 3, strTemp);

		m_ListCtrl.SetItemText(nIndex, 2, CString(m_SdupItemTotal2.SdupValue));

		for (int j = 0; j < CountArray(m_KindArray); j++)
		{
			if (m_KindArray[j].KindID == 0) break;

			for (int u = 0; u < m_nKindCount; u++)
			{
				if (m_SdupItemTotal2.pKindValue[u].KindID == 0) break;

				if (m_SdupItemTotal2.pKindValue[u].KindID == m_KindArray[j].KindID)
				{
					if (m_SdupItemTotal2.pKindValue[u].Value != 0.0f)
					{
						if (m_SdupItemTotal2.pKindValue[u].Value - (int)m_SdupItemTotal2.pKindValue[u].Value >= 0.1)
							strTemp.Format(_T("%0.1f"), m_SdupItemTotal2.pKindValue[u].Value);
						else
							strTemp.Format(_T("%0.0f"), m_SdupItemTotal2.pKindValue[u].Value);

						m_ListCtrl.SetItemText(nIndex, 4 + j, strTemp);
					}
				}
			}
		}

		if (nSelIndex != -1)
			m_ListCtrl.SetItemState(nSelIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	catch (...) {}

	m_ListCtrl.EnsureVisible(m_ListCtrl.GetItemCount() - 1, FALSE);

	m_ListCtrl.SetRedraw(TRUE);
}

//打印帐目
void CSdupDlg::PrinterSdup(int nBeginIndex, int nEndIndex)
{
	if (IDOK != m_PrintDialog.DoModal()) return;

	CDC PrinterDC;
	PrinterDC.Attach(m_PrintDialog.GetPrinterDC());

	// 页面尺寸大小
	int nPageWidth = PrinterDC.GetDeviceCaps(PHYSICALWIDTH);
	int nPageheigh = PrinterDC.GetDeviceCaps(PHYSICALHEIGHT);

	//获得打印机分辨率 : 每英寸象素数量
	int nPrinterX = PrinterDC.GetDeviceCaps(LOGPIXELSX);
	int nPrinterY = PrinterDC.GetDeviceCaps(LOGPIXELSY);

	//获得打印机分辨率 : 每英寸象素数量
	int nPrinterSpaceX = PrinterDC.GetDeviceCaps(PHYSICALOFFSETX);
	int nPrinterSpaceY = PrinterDC.GetDeviceCaps(PHYSICALOFFSETY);

	//获得屏幕分辨率 : 每英寸象素数量
	CDC* pListCtrlDC = m_ListCtrl.GetDC();
	int nScreenX = pListCtrlDC->GetDeviceCaps(LOGPIXELSX);
	int nScreenY = pListCtrlDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pListCtrlDC);

	//屏幕与打印机比例
	double rateX = (double)nPrinterX / (double)nScreenX;
	double rateY = (double)nPrinterY / (double)nScreenY;

	//行距列距
	int nSpaceWidth = (int)(2 * rateX);
	int nSpaceheigh = (int)(2 * rateY);

	//字体大小改变
	CFont *pFont = m_ListCtrl.GetFont();
	PrinterDC.SelectObject(pFont);
	LOGFONT lf; 
	memset(&lf, 0, sizeof(LOGFONT));
	m_ListCtrl.GetFont()->GetLogFont(&lf);
	lf.lfHeight *= rateY;
	lf.lfWidth *= rateX;
	HFONT hfont = ::CreateFontIndirect(&lf);     
	CFont * font = CFont::FromHandle(hfont); 
	PrinterDC.SelectObject(font);

	int nMaxLineCountOfPage = (nPageheigh - nPrinterSpaceY * 2) / (lf.lfHeight + nSpaceheigh) - 2;

	PrinterDC.StartDoc("");

	//打印内容
	int nPrinterTopPixel = 0;
	if (nEndIndex > m_ListCtrl.GetItemCount())
		nEndIndex = m_ListCtrl.GetItemCount();
	if (nBeginIndex < 0)
		nBeginIndex = 0;

	//页脚
	CTime timeNow = CTime::GetTickCount();
	for (int row = nBeginIndex; row < nEndIndex; row++)
	{
		//开始新的一页
		if ((row - nBeginIndex) % nMaxLineCountOfPage == 0)
		{
			if (row - nBeginIndex != 0)
			{
				CRect rcButtom(0, nPrinterTopPixel, nPageWidth, nPrinterTopPixel + lf.lfHeight + nSpaceheigh);

				CString strText;
				strText.Format("第 %d 页 共 %d 页 打印时间:%s", (row - nBeginIndex) / nMaxLineCountOfPage, (row - nBeginIndex) / nMaxLineCountOfPage + 1, timeNow.Format("%Y-%m-%d"));

				PrinterDC.SetTextColor(RGB(20, 20, 20));
				PrinterDC.DrawText(strText, strText.GetLength(), &rcButtom, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
				EndPage(PrinterDC);
			}
			StartPage(PrinterDC);
			nPrinterTopPixel = 0;
			//打印列头
			LV_COLUMN lvc;
			ZeroMemory(&lvc, sizeof(LV_COLUMN));
			lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
			TCHAR lpBuffer[256];
			lvc.pszText = lpBuffer;
			lvc.cchTextMax = sizeof(lpBuffer);

			int nPrinterLeftPixel = nSpaceWidth;
			for (int nCol = 0; m_ListCtrl.GetColumn(nCol, &lvc); nCol++)
			{
				CString strText(lpBuffer);

				CRect rcSub(nPrinterLeftPixel, nPrinterTopPixel, nPrinterLeftPixel + lvc.cx * rateX, nPrinterTopPixel + lf.lfHeight + nSpaceheigh);
				nPrinterLeftPixel += lvc.cx * rateX;

//				PrinterDC.Rectangle(rcSub);
//				PrinterDC.FillSolidRect(rcSub, RGB(200, 200, 200));
//				PrinterDC.SetBkMode(TRANSPARENT);

				PrinterDC.DrawText(strText, strText.GetLength(), &rcSub, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}


			nPrinterTopPixel = lf.lfHeight + nSpaceheigh;
		}

		tagSdupItem * pSdupItem = (tagSdupItem *)m_ListCtrl.GetItemData(row);
		if (pSdupItem != NULL && pSdupItem->SdupID == 0xFFFF)
			PrinterDC.SetTextColor(RGB(20, 20, 255));
		else
			PrinterDC.SetTextColor(RGB(20, 20, 20));

		LV_COLUMN lvc;
		ZeroMemory(&lvc, sizeof(LV_COLUMN));
		lvc.mask = LVCF_WIDTH | LVCF_FMT;
		int nPrinterLeftPixel = nSpaceWidth;
		for (int nCol = 0; m_ListCtrl.GetColumn(nCol, &lvc); nCol++)
		{
			TCHAR lpBuffer[256];
			LV_ITEM lvi;
			ZeroMemory(&lvi, sizeof(LV_ITEM));
			lvi.iItem = row;
			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			lvi.iSubItem = nCol;
			lvi.pszText = lpBuffer;
			lvi.cchTextMax = sizeof(lpBuffer);
			VERIFY(m_ListCtrl.GetItem(&lvi));
			CString strText(lpBuffer);

			CRect rcSub(nPrinterLeftPixel, nPrinterTopPixel, nPrinterLeftPixel + lvc.cx * rateX, nPrinterTopPixel + lf.lfHeight + nSpaceheigh);
			nPrinterLeftPixel += lvc.cx * rateX;

			PrinterDC.Rectangle(rcSub);

			if (lvc.fmt & LVCFMT_CENTER)
			{
				PrinterDC.DrawText(strText, strText.GetLength(), &rcSub, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}
			else if (lvc.fmt & LVCFMT_RIGHT)
			{
				rcSub.right -= 3;
				PrinterDC.DrawText(strText, strText.GetLength(), &rcSub, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
				rcSub.right += 3;
			}
			else
			{
				rcSub.left += 3;
				PrinterDC.DrawText(strText, strText.GetLength(), &rcSub, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
				rcSub.left -= 3;
			}
		}

		nPrinterTopPixel += lf.lfHeight + nSpaceheigh;

		//最后一行打印完成
		if (row == nEndIndex - 1)
		{
			CRect rcButtom(0, nPrinterTopPixel, nPageWidth, nPrinterTopPixel + lf.lfHeight + nSpaceheigh);

			CString strText;
			strText.Format("第 %d 页 共 %d 页 打印时间:%s", (row - nBeginIndex) / nMaxLineCountOfPage + 1, (row - nBeginIndex) / nMaxLineCountOfPage + 1, timeNow.Format("%Y-%m-%d"));

			PrinterDC.SetTextColor(RGB(20, 20, 20));
			PrinterDC.DrawText(strText, strText.GetLength(), &rcButtom, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			EndPage(PrinterDC);
		}
	}

	PrinterDC.EndDoc();
}

//打印
void CSdupDlg::OnBnClickedButton9()
{
	if (m_ListCtrl.GetItemCount() == 0) return;

	CDlgPrinterSetup dlg;
	dlg.m_nBeginIndex = 1;
	dlg.m_nEndIndex = m_ListCtrl.GetItemCount();

	if (IDOK == dlg.DoModal())
		PrinterSdup(dlg.m_nBeginIndex - 1, dlg.m_nEndIndex);
}

//点击列头
void CSdupDlg::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;
}

//单击事件
void CSdupDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_ListCtrl.SetRedraw(FALSE);
	try
	{
		m_ListCtrl.m_nSelColIndex = pNMItemActivate->iSubItem;
		if (m_ListCtrl.m_nSelColIndex <= 3)
			m_ListCtrl.m_nSelColIndex = -1;
		m_ListCtrl.Invalidate(FALSE);
		m_ListCtrl.SetRedraw(TRUE);
	}
	catch (...)
	{

	}

	*pResult = 0;
}


void CSdupDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	static int nIndex = 0;
	nIndex++;
	if (nIndex >= 128 && nHotKeyId == 1001 && nKey2 == 'E')
	{
		CDlgXOREncrypt dlg;
		dlg.DoModal();
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
