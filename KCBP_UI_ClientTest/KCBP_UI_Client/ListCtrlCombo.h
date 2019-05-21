#pragma once

#define WM_USER_COMBO_END WM_USER+102

// CListCtrlCombo

class CListCtrlCombo : public CComboBox
{
	DECLARE_DYNAMIC(CListCtrlCombo)

public:
	CListCtrlCombo();
	virtual ~CListCtrlCombo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


