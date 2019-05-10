// test_counter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
using namespace std;

struct DaiKuanResult
{
    double dDaiKuanZongE;
    double dHuanKuanZongE;
    double dZhiFuLiXi;
    int iDaiKuanYueShu;
    double dYueGong;
    double dLiLv;
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
    void print()
    {
        printf( "����=[%.02lf] ����=[%.02lf] ��Ϣ=[%.02lf] ����=[%d] ����=[%.03lf] �¹�=[%.02lf]\n", dDaiKuanZongE, dHuanKuanZongE, dZhiFuLiXi, iDaiKuanYueShu, dLiLv, dYueGong );
    }
};

void BenXinCounter( double dBenJin, double dLiLv, int iMonthNum, DaiKuanResult& stResult )
{
    double dMonthRate = dLiLv / 1200;
    //�������������ʡ���1�������ʣ��޻����������¡���1�������ʣ��޻���������1��
    stResult.dYueGong = ( dBenJin * dMonthRate * pow( dMonthRate + 1, iMonthNum ) ) / ( pow( dMonthRate + 1, iMonthNum ) - 1 ) ;
    stResult.dDaiKuanZongE = dBenJin;
    stResult.dHuanKuanZongE = stResult.dYueGong * iMonthNum;
    stResult.dZhiFuLiXi = stResult.dHuanKuanZongE - stResult.dDaiKuanZongE;
    stResult.iDaiKuanYueShu = iMonthNum;
    stResult.dLiLv = dLiLv;
}

void TiQianHuanKuanCounter( double dOldBenJin, double dOldLiLv, int iOldMonthNum, int iDangQianQiShu, double dTiQianHuanKuan, DaiKuanResult& stResult )
{
    DaiKuanResult stOldResult;
    BenXinCounter( dOldBenJin, dOldLiLv, iOldMonthNum, stOldResult );
    double dMonthRate = dOldLiLv / 1200;
    double dYiHuanBenXin = stOldResult.dYueGong * iDangQianQiShu;
    double dYiHuanLiXi = ( dOldBenJin * dMonthRate - stOldResult.dYueGong ) * ( pow( dMonthRate + 1, iDangQianQiShu ) - 1 ) / dMonthRate + iDangQianQiShu * stOldResult.dYueGong;
    double dYiHuanBenJin = dYiHuanBenXin - dYiHuanLiXi;
    double dShengYuBenJin = dOldBenJin - dYiHuanBenJin;
    BenXinCounter( dShengYuBenJin - dTiQianHuanKuan, dOldLiLv, iOldMonthNum - iDangQianQiShu, stResult );
}

//ϣ����֤һ����ǰ��������´�����������ӣ��Ƿ�����´��������¾������йء����Լ�����ٽ�㡣
int _tmain( int argc, _TCHAR* argv[] )
{
    double dOldDaiKuan = 800000.0;
    int iOldQiXian = 360;
    double dHuanKuan = 200000.0;
    double dNewDaiKuan = 1200000.0;
    double dGJJDaiKuan = 900000.0;
    int iQiShu = 36;
    double dOldRate = 4.9 * 0.95;
    double dGJJRate = 3.25 * 1.0;
    double dNewRate = 4.9 * 1.1;
    if( 0 )
    {
        DaiKuanResult st1;
        BenXinCounter( dOldDaiKuan, dOldRate, iOldQiXian, st1 );
        st1.print();
        printf( "\n" );
        DaiKuanResult st2;
        TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, dHuanKuan, st2 );
        st2.print();
        return 0;
    }
    if( 1 )
    {
        DaiKuanResult st0;
        BenXinCounter( dGJJDaiKuan, dGJJRate, iOldQiXian, st0 );        
        DaiKuanResult st1;
        TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, dHuanKuan, st1 );
        DaiKuanResult st2;
        BenXinCounter( dNewDaiKuan - dGJJDaiKuan, dNewRate, iOldQiXian, st2 );
        DaiKuanResult stSum1 = st1 + st2 + st0;
        DaiKuanResult st3;
        TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, 0, st3 );
        DaiKuanResult st4;
        BenXinCounter( dNewDaiKuan - dHuanKuan - dGJJDaiKuan, dNewRate, iOldQiXian, st4 );
        DaiKuanResult stSum2 = st3 + st4 + st0;
        printf( "\n����ǰ����20W��\n" );
        st1.print();
        printf( "\n����ǰ����0W��\n" );
        st3.print();
        printf( "\n�����¹��������90W��\n" );
        st0.print();
        printf( "\n��������ҵ����30W��\n" );
        st2.print();
        printf( "\n��������ҵ����10W��\n" );
        st4.print();
        printf( "\n������1=��ǰ����20W������120W(������90W)���ϼơ�\n" );
        stSum1.print();
        printf( "\n������2=��ǰ����0W������100W(������90W)���ϼơ�\n" );
        stSum2.print();
        return 0;
    }
    if( 1 )
    {
        //����1����ǰ��X�����´���Y
        DaiKuanResult stInfo1;
        TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, dHuanKuan, stInfo1 );
        DaiKuanResult stInfo2;
        BenXinCounter( dNewDaiKuan, dNewRate, iOldQiXian, stInfo2 );
        DaiKuanResult stSum1 = stInfo1 + stInfo2;
        //����2������ǰ��X�����´���Y-X
        DaiKuanResult stInfo3;
        TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, 0, stInfo3 );
        DaiKuanResult stInfo4;
        BenXinCounter( dNewDaiKuan - dHuanKuan, dNewRate, iOldQiXian, stInfo4 );
        DaiKuanResult stSum2 = stInfo3 + stInfo4;
        printf( "\n����ǰ����20W��\n" );
        stInfo1.print();
        printf( "\n����ǰ����0W��\n" );
        stInfo3.print();
        printf( "\n�����´���120W��\n" );
        stInfo2.print();
        printf( "\n�����´���100W��\n" );
        stInfo4.print();
        printf( "\n������1=��ǰ����20W���׸�180W������120W���ϼơ�\n" );
        stSum1.print();
        printf( "\n������2=��ǰ����0W���׸�200W������100W���ϼơ�\n" );
        stSum2.print();
        return 0;
    }
    {
        for( double i = 5.050 ; i <= 5.1 ; i += 0.0001 )
        {
            dNewRate = i;
            //����1����ǰ��X�����´���Y
            DaiKuanResult stInfo1;
            TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, dHuanKuan, stInfo1 );
            DaiKuanResult stInfo2;
            BenXinCounter( dNewDaiKuan, dNewRate, iOldQiXian, stInfo2 );
            DaiKuanResult stSum1 = stInfo1 + stInfo2;
            //����2������ǰ��X�����´���Y-X
            DaiKuanResult stInfo3;
            TiQianHuanKuanCounter( dOldDaiKuan, dOldRate, iOldQiXian, iQiShu, 0, stInfo3 );
            DaiKuanResult stInfo4;
            BenXinCounter( dNewDaiKuan - dHuanKuan, dNewRate, iOldQiXian, stInfo4 );
            DaiKuanResult stSum2 = stInfo3 + stInfo4;
            printf( "[����1-����2]����=[%.04f]�������=[%.03f] ��Ϣ��=[%.03f] �¹���=[%.03f] \n", dNewRate,
                    stSum1.dDaiKuanZongE - stSum2.dDaiKuanZongE,
                    stSum1.dZhiFuLiXi - stSum2.dZhiFuLiXi,
                    stSum1.dYueGong - stSum2.dYueGong );
        }
    }
    return 0;
}

