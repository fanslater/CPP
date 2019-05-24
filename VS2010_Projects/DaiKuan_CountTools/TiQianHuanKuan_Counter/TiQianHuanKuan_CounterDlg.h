
// TiQianHuanKuan_CounterDlg.h : ͷ�ļ�
//

#pragma once
#include <iostream>
#include <string>
#include "afxcmn.h"
#include <cmath>

using namespace std;

struct CountInfo
{
    double dDaiKuanZongE;   //��λ-Ԫ
    int iDaiKuanQiXian;     //��λ-��
    double dDaiKuanLiLv;    //��С��
    int iDangQianQiShu;     //�����Ͻɵ��¹��ǵڼ���
    double dTiQianHuanKuanJinE;     //��λ-Ԫ
};

struct DaiKuanResult
{
    double dDaiKuanZongE;   //�����ܶ�
    double dHuanKuanZongE;  //�����ܶ�
    double dZhiFuLiXi;      //֧����Ϣ�ܶ�
    int iDaiKuanYueShu;     //����������
    double dYueGong;        //�¹�
    double dYueGondDiJianE; //�¹��ݼ���ȶ��ʱ���õ���
    double dLiLv;           //������
    friend DaiKuanResult operator + ( DaiKuanResult& st1, DaiKuanResult& st2 )
    {
        DaiKuanResult tmp;
        tmp.dDaiKuanZongE = st1.dDaiKuanZongE + st2.dDaiKuanZongE;
        tmp.dHuanKuanZongE = st1.dHuanKuanZongE + st2.dHuanKuanZongE;
        tmp.dZhiFuLiXi = st1.dZhiFuLiXi + st2.dZhiFuLiXi;
        tmp.dYueGong = st1.dYueGong + st2.dYueGong;
        tmp.iDaiKuanYueShu = 0;
        tmp.dLiLv = 0.00;
        return tmp;
    }
    double ps_dYiHuanBenJinLiXi; //�ѻ�������Ϣ�ܶ�
    double ps_dYiHuanLiXi; //�ѻ���Ϣ�ܶ�
    double ps_dYiHuanBenJin; //�ѻ������ܶ�
    double ps_dShengYuBenJin;  //ʣ�౾���ܶ�
    double ps_dYuanDaiKuanZongE;   //ԭ-�����ܶ�
    double ps_dYuanHuanKuanZongE;  //ԭ-�����ܶ�
    double ps_dYuanZhiFuLiXi;      //ԭ-֧����Ϣ�ܶ�
    double ps_dYuanYueGong;        //ԭ-�¹�
    double ps_dYuanYueGondDiJianE; //ԭ-�¹��ݼ���ȶ��ʱ���õ���
    void print()
    {
        CString show;
        show.Format( "����=[%lf] ����=[%lf] ��Ϣ=[%lf] ����=[%d] ����=[%lf] �¹�=[%lf]\n \
                     �ѻ�������Ϣ�ܶ�=[%lf] �ѻ���Ϣ�ܶ�=[%lf] �ѻ������ܶ�=[%lf] ʣ�౾���ܶ�=[%lf] ԭ-�����ܶ�=[%lf] ԭ-�����ܶ�=[%lf] ԭ-֧����Ϣ�ܶ�=[%lf]",
                     dDaiKuanZongE, dHuanKuanZongE, dZhiFuLiXi, iDaiKuanYueShu, dLiLv, dYueGong,
                     ps_dYiHuanBenJinLiXi, ps_dYiHuanLiXi, ps_dYiHuanBenJin, ps_dShengYuBenJin, ps_dYuanDaiKuanZongE, ps_dYuanHuanKuanZongE, ps_dYuanZhiFuLiXi );
        AfxMessageBox( show );
    }
};

// CTiQianHuanKuan_CounterDlg �Ի���
class CTiQianHuanKuan_CounterDlg : public CDialogEx
{
    // ����
public:
    CTiQianHuanKuan_CounterDlg( CWnd* pParent = NULL ); // ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_TIQIANHUANKUAN_COUNTER_DIALOG };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
public:
    void InitAll();
    void UninitAll();
    void CleanInput();
    void CleanOutput();
private:
    int JiSuanDangQianQiShu( const SYSTEMTIME stFirst, const SYSTEMTIME stSecond );    
    int FindApproximateQiXian_BenXi( double Y, double B, double A );
    int FindApproximateQiXian_BenJin( double Y, double B, double A );
    void ShowResults( const DaiKuanResult& stResult );
    void DaiKuanCounter_BenXi( const CountInfo stInfo, DaiKuanResult& stResult ) ;
    void DaiKuanCounter_BenJin( const CountInfo stInfo, DaiKuanResult& stResult ) ;
    void TiQianHuanKuanCounter_BenXi_JianYueGong( const CountInfo stInfo, DaiKuanResult& stResult ) ;
    void TiQianHuanKuanCounter_BenXi_JianQiXian( const CountInfo stInfo, DaiKuanResult& stResult ) ;
    void TiQianHuanKuanCounter_BenJin_JianYueGong( const CountInfo stInfo, DaiKuanResult& stResult ) ;
    void TiQianHuanKuanCounter_BenJin_JianQiXian( const CountInfo stInfo, DaiKuanResult& stResult ) ;
public:
    afx_msg void OnBnClickedButtonStartwork();
    afx_msg void OnBnClickedButtonClean();
private:
    CListCtrl m_clsListCtrl_YueGongJinE;
};
