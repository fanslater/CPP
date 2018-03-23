// ListCtrlEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEdit.h"

// CListCtrlEdit

IMPLEMENT_DYNAMIC(CListCtrlEdit, CEdit)

CListCtrlEdit::CListCtrlEdit()
{
}

CListCtrlEdit::~CListCtrlEdit()
{
}

BEGIN_MESSAGE_MAP(CListCtrlEdit, CEdit)
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CListCtrlEdit message handlers
void CListCtrlEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	CWnd *pParent = this->GetParent();
	::PostMessage(pParent->GetSafeHwnd(), WM_USER_EDIT_END, 0, 0);
}

BOOL CListCtrlEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CWnd *pParent = this->GetParent();
	::PostMessage(pParent->GetSafeHwnd(), WM_USER_EDIT_END, 0, 0);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
