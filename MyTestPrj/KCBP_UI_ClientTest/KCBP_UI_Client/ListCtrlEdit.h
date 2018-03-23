#pragma once

#define WM_USER_EDIT_END  WM_USER+101

// CListCtrlEdit

class CListCtrlEdit : public CEdit
{
	DECLARE_DYNAMIC(CListCtrlEdit)

public:
	CListCtrlEdit();
	virtual ~CListCtrlEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


