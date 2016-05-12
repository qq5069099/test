// RightListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "SdupListCtrl.h"
#include "SdupDlg.h"

// CRightListCtrl

IMPLEMENT_DYNAMIC(CSdupListCtrl, CListCtrl)

CSdupListCtrl::CSdupListCtrl()
{
	m_nSelColIndex = -1;
}

CSdupListCtrl::~CSdupListCtrl()
{
}

BEGIN_MESSAGE_MAP(CSdupListCtrl, CListCtrl)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CRightListCtrl 消息处理程序
#include "DlgAddItem.h"


//自画函数
void CSdupListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	GetItemText(lpDrawItemStruct->itemID, 0); 

	tagSdupItem * pSdupItem = (tagSdupItem *)lpDrawItemStruct->itemData;

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		if (pSdupItem != NULL && pSdupItem->SdupID == 0xFFFF)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(210, 210, 210));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(155, 0, 0));
		}
	}
	else
	{
		if (pSdupItem != NULL && pSdupItem->SdupID == 0xFFFF)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(210, 210, 210));
		}
		else
		{
			pDC->SetTextColor(RGB(20, 20, 20));
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 255));
		}
	}

	CRect rcSub;
	rcSub = lpDrawItemStruct->rcItem;

	LV_COLUMN lvc;
	ZeroMemory(&lvc, sizeof(LV_COLUMN));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	for (int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		TCHAR lpBuffer[256];
		LV_ITEM lvi;
		ZeroMemory(&lvi, sizeof(LV_ITEM));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));

		rcSub.right = rcSub.left + lvc.cx;

		CString strText(lpBuffer);

		if (nCol == m_nSelColIndex && (lpDrawItemStruct->itemState & ODS_SELECTED) == 0)
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->FillSolidRect(&rcSub, RGB(255, 100, 100));
		}
		else if ((lpDrawItemStruct->itemState & ODS_SELECTED) == 0)
		{
			pDC->SetTextColor(RGB(20, 20, 20));
		}

		if (pSdupItem != NULL && pSdupItem->SdupID == 0xFFFF)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}

		if (lvc.fmt & LVCFMT_CENTER)
		{
			pDC->DrawText(strText, strText.GetLength(), &rcSub, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		else if (lvc.fmt & LVCFMT_RIGHT)
		{
			rcSub.right -= 1;
			pDC->DrawText(strText, strText.GetLength(), &rcSub, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
			rcSub.right += 1;
		}
		else
		{
			rcSub.left += 1;
			pDC->DrawText(strText, strText.GetLength(), &rcSub, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
			rcSub.left -= 1;
		}

		rcSub.left += lvc.cx;
	}
}

LRESULT CSdupListCtrl::OnSetFont(WPARAM wParam, LPARAM)
{
	LRESULT res = Default();
	CRect rc;
	GetWindowRect(&rc);

	WINDOWPOS wp;
	wp.hwnd  = m_hWnd;
	wp.cx    = rc.Width();
	wp.cy    = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);

	return res;
}

void CSdupListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);

	if (lf.lfHeight < 0)
		lpMeasureItemStruct->itemHeight = -(lf.lfHeight * 3 / 2); 
	else
		lpMeasureItemStruct->itemHeight = lf.lfHeight * 3 / 2; 
}

BOOL CSdupListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CListCtrl::OnEraseBkgnd(pDC);
}


void CSdupListCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CRect headerRect;
	GetDlgItem(0)->GetWindowRect(&headerRect);

	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CDC MemDC;//内存ID表
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(&rect, RGB(255, 255, 255));

	//这一句是调用默认的OnPaint(),把图形画在内存DC表上
	DefWindowProc(WM_PAINT, (WPARAM)MemDC.m_hDC, (LPARAM)0);

	//输出
	dc.BitBlt(0, headerRect.Height(), rect.Width(), rect.Height(), &MemDC, 0, headerRect.Height(), SRCCOPY);

	MemDC.DeleteDC();
	MemBitmap.DeleteObject();

// 	CPaintDC dc(this);
// 
// 	CListCtrl::OnPaint();
}
