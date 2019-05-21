// EditableListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EditableListCtrl.h"

// CEditableListCtrl

IMPLEMENT_DYNAMIC(CEditableListCtrl, CListCtrl)

CEditableListCtrl::CEditableListCtrl() 
: m_bEditVisible(false), m_bComboVisible(false), nItem(-1), nSubItem(-1), m_ItemValueUpdateAftFunc(NULL)
{
}

CEditableListCtrl::~CEditableListCtrl()
{
}

void CEditableListCtrl::SetColumnCtrlType(int nColumn, ColumnCtrlType ctrlType)
{
	m_Col2CtrlType[nColumn] = ctrlType;
}

void CEditableListCtrl::SetColumnComboValue(int nColumn, list<string>& lstValue)
{
	m_Col2ComboValue[nColumn].clear();

	list<string>::iterator iterValue = lstValue.begin();
	for (; iterValue != lstValue.end(); ++iterValue)
	{
		m_Col2ComboValue[nColumn].push_back(*iterValue);
	}
}

void CEditableListCtrl::SetItemUpdateAftCallBackFunc(EditableListCtrlItemValueUpdateAftFunc func)
{
	m_ItemValueUpdateAftFunc = func;
}

void CEditableListCtrl::ShowEdit(bool bShow, int nItem, int nSubItem, CRect rcCtrl)
{
	if (m_edit.m_hWnd == NULL)
	{
		m_edit.Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN | WS_BORDER,
			CRect(0,0,0,0), this, 1001);
		m_edit.ShowWindow(SW_HIDE);
		m_bEditVisible = false;
		m_edit.SetFont(this->GetFont());
	}

	if (bShow == TRUE)
	{
		CString cstrItem = GetItemText(nItem, nSubItem);
		m_edit.MoveWindow(&rcCtrl);
		m_edit.ShowWindow(SW_SHOW);
		m_bEditVisible = true;
		m_edit.SetWindowText(cstrItem);
		m_edit.SetFocus();
		m_edit.SetSel(-1); 
	}
	else
	{
		m_edit.ShowWindow(SW_HIDE);
		m_bEditVisible = false;
	}
}

void CEditableListCtrl::ShowCombo(bool bShow, int nItem, int nSubItem, CRect rcCtrl)
{
	if (m_combo.m_hWnd == NULL)
	{
		m_combo.Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD | ES_LEFT | CBS_DROPDOWNLIST,
			CRect(0,0,0,0), this, 1002);
		m_combo.ShowWindow(SW_HIDE);
		m_bComboVisible = false;
		m_combo.SetFont(this->GetFont());
	}

	if (bShow == TRUE && !m_Col2ComboValue[nSubItem].empty())
	{
		CString cstrItem = GetItemText(nItem, nSubItem);
		rcCtrl.bottom += 100;
		m_combo.MoveWindow(&rcCtrl);
		m_combo.ShowWindow(SW_SHOW);
		m_bComboVisible = true;
		m_combo.SetWindowText(cstrItem);
		m_combo.SetFocus();
		
		m_combo.ResetContent();
		list<string>::iterator iterValue = m_Col2ComboValue[nSubItem].begin();
		for (; iterValue != m_Col2ComboValue[nSubItem].end(); ++iterValue)
		{
			m_combo.AddString(iterValue->c_str());
		}

		m_combo.SetCurSel(-1);
		int nItems = m_combo.GetCount();
		for (int i=0; i<nItems; ++i)
		{
			CString cstrItemText;
			m_combo.GetLBText(i, cstrItemText);
			if (cstrItem == cstrItemText)
			{
				m_combo.SetCurSel(i);
				break;
			}
		}
	}
	else
	{
		m_combo.ShowWindow(SW_HIDE);
		m_bComboVisible = false;
	}
}

BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CEditableListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CEditableListCtrl::OnHdnBegintrack)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_MESSAGE(WM_USER_EDIT_END, OnEditEnd)
	ON_MESSAGE(WM_USER_COMBO_END, OnComboEnd)
END_MESSAGE_MAP()

// CEditableListCtrl message handlers

LRESULT CEditableListCtrl::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	m_edit.GetWindowText(strText);
	SetItemText(nItem,nSubItem,strText);
	if (m_ItemValueUpdateAftFunc != NULL)
	{
		(*m_ItemValueUpdateAftFunc)(this->GetParent(), this, nItem, nSubItem);
	}

	ShowEdit(false, -1, -1, CRect());

	return 0;
}

LRESULT CEditableListCtrl::OnComboEnd(WPARAM wParam, LPARAM lParam)
{
	int nSel = m_combo.GetCurSel();
	if (nSel != -1)
	{
		CString cstrTextSel;
		m_combo.GetLBText(nSel, cstrTextSel);
		SetItemText(nItem, nSubItem, cstrTextSel);
		if (m_ItemValueUpdateAftFunc != NULL)
		{
			(*m_ItemValueUpdateAftFunc)(this->GetParent(), this, nItem, nSubItem);
		}
	}
	
	ShowCombo(false, -1, -1, CRect());

	return 0;
}

void CEditableListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	nItem = SubItemHitTest(&lvhti);
	if (nItem == -1)
	{
		return;
	}
	nSubItem = lvhti.iSubItem;

	ColumnCtrlType colCtrlType = CCT_NONE;
	if (m_Col2CtrlType.find(nSubItem) == m_Col2CtrlType.end())
	{
		return;
	}
	else
	{
		colCtrlType = m_Col2CtrlType[nSubItem];
	}

	CRect rcCtrl;
	GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);

	switch (colCtrlType)
	{
	case CCT_NONE:
		break;

	case CCT_EDITBOX:
		ShowEdit(TRUE, nItem, nSubItem, rcCtrl);
		break;

	case CCT_COMBOBOX:
		ShowCombo(TRUE, nItem, nSubItem, rcCtrl);
		break;

	default:
		break;
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CEditableListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (m_edit.m_hWnd != NULL && m_bEditVisible)
		OnEditEnd(0, 0);
	if (m_combo.m_hWnd != NULL && m_bComboVisible)
		OnComboEnd(0, 0);
	*pResult = 0;
}

void CEditableListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (m_edit.m_hWnd != NULL && m_bEditVisible)
		OnEditEnd(0, 0);
	if (m_combo.m_hWnd != NULL && m_bComboVisible)
		OnComboEnd(0, 0);
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditableListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (m_edit.m_hWnd != NULL && m_bEditVisible)
		OnEditEnd(0, 0);
	if (m_combo.m_hWnd != NULL && m_bComboVisible)
		OnComboEnd(0, 0);
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}