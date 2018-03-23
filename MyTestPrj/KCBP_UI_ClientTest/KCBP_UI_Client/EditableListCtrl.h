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
	// �����пؼ�������
	void SetColumnCtrlType(int nColumn, ColumnCtrlType ctrlType);
	// �����пؼ�Combox��ֵ
	void SetColumnComboValue(int nColumn, list<string>& lstValue);
	// ��������º�ص�����
	void SetItemUpdateAftCallBackFunc(EditableListCtrlItemValueUpdateAftFunc func);
private:
	CListCtrlEdit m_edit;
	CListCtrlCombo m_combo;
	int nItem;		         // ��ǰ�����
	int nSubItem;            // ��ǰ�������
	bool m_bEditVisible;     // Edit�Ƿ�ɼ�
	bool m_bComboVisible;    // ComboBox�Ƿ�ɼ�

	map<int, ColumnCtrlType> m_Col2CtrlType;
	map<int, list<string> > m_Col2ComboValue;

	EditableListCtrlItemValueUpdateAftFunc m_ItemValueUpdateAftFunc;
};


