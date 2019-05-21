// ListCtrlCombo.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlCombo.h"

// CListCtrlCombo

IMPLEMENT_DYNAMIC(CListCtrlCombo, CComboBox)

CListCtrlCombo::CListCtrlCombo()
{
}

CListCtrlCombo::~CListCtrlCombo()
{
}


BEGIN_MESSAGE_MAP(CListCtrlCombo, CComboBox)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CListCtrlCombo message handlers

void CListCtrlCombo::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	CWnd *pParent = this->GetParent();
	::PostMessage(pParent->GetSafeHwnd(), WM_USER_COMBO_END, 0, 0);
}
