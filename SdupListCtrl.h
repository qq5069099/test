#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"

// CRightListCtrl

class CSdupListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSdupListCtrl)

public:
	int					m_nSelColIndex;

public:
	CSdupListCtrl();
	virtual ~CSdupListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	//×Ô»­º¯Êý
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	LRESULT CSdupListCtrl::OnSetFont(WPARAM wParam, LPARAM);
	void CSdupListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


