
// TiQianHuanKuan_CounterDlg.h : 头文件
//

#pragma once
#include <iostream>
#include <string>
#include "afxcmn.h"
#include <cmath>

using namespace std;

struct CountInfo
{
    double dDaiKuanZongE;   //单位-元
    int iDaiKuanQiXian;     //单位-月
    double dDaiKuanLiLv;    //纯小数
    int iDangQianQiShu;     //即将上缴的月供是第几期
    double dTiQianHuanKuanJinE;     //单位-元
};

struct DaiKuanResult
{
    double dDaiKuanZongE;   //贷款总额
    double dHuanKuanZongE;  //还款总额
    double dZhiFuLiXi;      //支付利息总额
    int iDaiKuanYueShu;     //贷款月总数
    double dYueGong;        //月供
    double dYueGondDiJianE; //月供递减额（等额本金时会用到）
    double dLiLv;           //年利率
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
    double ps_dYiHuanBenJinLiXi; //已还本金利息总额
    double ps_dYiHuanLiXi; //已还利息总额
    double ps_dYiHuanBenJin; //已还本金总额
    double ps_dShengYuBenJin;  //剩余本金总额
    double ps_dYuanDaiKuanZongE;   //原-贷款总额
    double ps_dYuanHuanKuanZongE;  //原-还款总额
    double ps_dYuanZhiFuLiXi;      //原-支付利息总额
    double ps_dYuanYueGong;        //原-月供
    double ps_dYuanYueGondDiJianE; //原-月供递减额（等额本金时会用到）
    void print()
    {
        CString show;
        show.Format( "贷款=[%lf] 还款=[%lf] 利息=[%lf] 期数=[%d] 利率=[%lf] 月供=[%lf]\n \
                     已还本金利息总额=[%lf] 已还利息总额=[%lf] 已还本金总额=[%lf] 剩余本金总额=[%lf] 原-贷款总额=[%lf] 原-还款总额=[%lf] 原-支付利息总额=[%lf]",
                     dDaiKuanZongE, dHuanKuanZongE, dZhiFuLiXi, iDaiKuanYueShu, dLiLv, dYueGong,
                     ps_dYiHuanBenJinLiXi, ps_dYiHuanLiXi, ps_dYiHuanBenJin, ps_dShengYuBenJin, ps_dYuanDaiKuanZongE, ps_dYuanHuanKuanZongE, ps_dYuanZhiFuLiXi );
        AfxMessageBox( show );
    }
};

// CTiQianHuanKuan_CounterDlg 对话框
class CTiQianHuanKuan_CounterDlg : public CDialogEx
{
    // 构造
public:
    CTiQianHuanKuan_CounterDlg( CWnd* pParent = NULL ); // 标准构造函数

    // 对话框数据
    enum { IDD = IDD_TIQIANHUANKUAN_COUNTER_DIALOG };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
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
