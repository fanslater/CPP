#pragma once

#include "ListCtrlEdit.h"
#include "ListCtrlCombo.h"
#include <string>
#include <list>
#include <map>

using std::string;
using std::list;
using std::map;

// CEditableListCtrl

enum ColumnCtrlType
{
	CCT_NONE = 0,
	CCT_COMBOBOX,
	CCT_EDITBOX
};

class CEditableListCtrl;
typedef void (*EditableListCtrlItemValueUpdateAftFunc)(
	CWnd* pParent,
	CEditableListCtrl *pEditableListCtrl, 
	int iItem, int iSubItem);

class CEditableListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditableListCtrl)

public:
	CEditableListCtrl();
	virtual ~CEditableListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	afx_msg LRESULT OnEditEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnComboEnd(WPARAM wParam,LPARAM lParam);
	void ShowEdit(bool bShow, int nItem, int nSubItem, CRect rcCtrl);
	void ShowCombo(bool bShow, int nItem, int nSubItem, CRect rcCtrl);

public:
	// 设置列控件的类型
	void SetColumnCtrlType(int nColumn, ColumnCtrlType ctrlType);
	// 设置列控件Combox的值
	void SetColumnComboValue(int nColumn, list<string>& lstValue);
	// 设置项更新后回调函数
	void SetItemUpdateAftCallBackFunc(EditableListCtrlItemValueUpdateAftFunc func);
private:
	CListCtrlEdit m_edit;
	CListCtrlCombo m_combo;
	int nItem;		         // 当前点击项
	int nSubItem;            // 当前点击子项
	bool m_bEditVisible;     // Edit是否可见
	bool m_bComboVisible;    // ComboBox是否可见

	map<int, ColumnCtrlType> m_Col2CtrlType;
	map<int, list<string> > m_Col2ComboValue;

	EditableListCtrlItemValueUpdateAftFunc m_ItemValueUpdateAftFunc;
};


