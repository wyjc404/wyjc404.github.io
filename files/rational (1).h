#ifndef _RATIONAL_H_
#define _RATIONAL_H_
//  大冰块stupid吗 2025-1-28  //讨论：未央  //指导：袁同学  //协助测试：郦同学
//  这有理库包含分数结构、常量、运算、输入、输出和转化等基本功能。
#include <stdio.h>
#include <limits.h>

//  辅助宏：类型改名，本库认为辅助宏最后必须销毁。
#
#define ll long long
#define ul unsigned long
#define ull unsigned long long
#define lr longrat

//  定义类型rational，三个成员分别是：分子、非负的分母、化简状态。\
//  当splfd(simplified)的值为1时，认为一个rat已经被化简过，此外，\
    您也可以通过将splfd赋值为1来保护一个rat，或者搞破坏。splfd仅\
    1 bit大小，在一个rat 未 初始化时可能导致错误。\
//  rat.down为unsigned类型，请留意补码造成的隐患。
#
struct rat{
    int up; unsigned down: sizeof(int)*8-1; unsigned splfd: 1;
};  typedef struct rat rat;

//  为了保证longrat的精度比rat高，这里使用条件编译。
#
struct longrat{
#if INT_MAX < LONG_MAX
    long up; ul down: sizeof(long)*8-1; ul splfd: 1;
#else
    ll up; ull down: sizeof(ll)*8-1; ull splfd: 1;
#endif
};  typedef struct longrat longrat;

//  根据#include本库前是否#define _AUTO_RAT_TYPE_\
    本库分为 手动类型调整 模式 和 自动类型调整 模式。

//  默认模式，\
    即手动类型调整模式，本库对rat和longrat的一切函数参数与返回\
    值进行严格区分，如果想让不同精度rational变量混合传参，必须\
    手动转化，本库提供了转化函数，您当然可以自行写语句做转化。

//  自动类型调整模式，\
    本库中的大量糖衣宏被激活，自动将传入函数的rational类型强行\
    统一，使混合传参（仅rats）成为可能。由于糖衣宏的展开行为，\
    代码中某些只出现 一次 的有理数据 将变得 多次 出现，如果这有\
    理数恰是本库函数的返回值，糖衣宏的展开就会产生不必要的重复\
    调用，拖慢效率并带来隐患（例如一个输入函数因宏展开而被调用\
    多次）。若使用_AUTO_RAT_TYPE_，建议 *回避* 函数的 *嵌套*。

//  定义常量：0、1、分数式的正无穷、负无穷、不定态（即NaN）等，\
    后三个不妨统称无值分数。另外，推荐经常利用(l)rZERO做初始化。
#
const rat      rZERO={0,1,1},  rONE={1,1,1},  rINF={1,0,1},  rNEGINF={-1,0,1},  rUNCERTAIN={0,0,1};
const longrat  lrZERO={0,1,1}, lrONE={1,1,1}, lrINF={1,0,1}, lrNEGINF={-1,0,1}, lrUNCERTAIN={0,0,1};

//  函数式的长短rat转化。
#
rat torat(lr);
lr tolr(rat);

//  运算：最大公约数、最小公倍数、约分、加、负、减、乘、倒、除、二元运算包、绝对值、比较。
#
ll gcd(ll,ll);          ull lcm(ll,ll);
lr lrsplfy(lr);         rat rsplfy(rat);
lr lrplus(lr,lr);       rat rplus(rat,rat);
lr lrneg(lr);           rat rneg(rat);
lr lrminus(lr,lr);      rat rminus(rat,rat);
lr lrtime(lr,lr);       rat rtime(rat,rat);
lr lrflip(lr);          rat rflip(rat);
lr lrdivi(lr,lr);       rat rdivi(rat,rat);
lr co2lr(lr,char,lr);   rat co2r(rat,char,rat);
lr lrabs(lr);           rat rabs(rat);
signed char lrcmp(lr,lr);
signed char rcmp(rat,rat);

//  输出：分数、指定位数小数、（循环）小数。输入：综合、分数、（循环）小数。转化：到浮点、到整数。
#
char lrap(lr);          char rap(rat);
int fltlrap(lr,int);    int fltrap(rat,int);
ll lrdeci(lr);          int rdeci(rat);
lr getlr();             rat getrat();
lr getlfrac();          rat getfrac();
lr getldeci();          rat getdeci();    
double lrtodb(lr);      double rattodb(rat);
int rattoint(rat);
#if     INT_MAX < LONG_MAX
long    lrtolong(lr);
#else
ll      lrtoll(lr);
#endif

//  辅助宏：输出非负整数(++i)、输出检查、是否空白、是否数码、循环添位、小数添位、输入截停。
#define _putu_ppi_(n,i){\
    ull ten=1,lv=2,m=n;\
    while(n/=10) ten*=10,++lv;\
    while(--lv){\
        putchar(m/ten+'0');\
        m%=ten;ten/=10;++i;}\
}   //  int lv=2;与--lv呼应。
#define _putu_(n){\
    ull ten=1,lv=2,m=n;\
    while(n/=10) ten*=10,++lv;\
    while(--lv){\
        putchar(m/ten+'0');\
        m%=ten;ten/=10;}\
}
#define _rapCHECK1_(q){\
    if(q.splfd!=1) q=lrsplfy(q);\
    if(!q.down){\
        switch(q.up){\
            case -1: fputs("-inf",stdout);return 4;\
            case 0: fputs("NaN",stdout);return 3;\
            case 1: fputs("+inf",stdout);return 4;\
        }\
    }\
}
#define _rapCHECK2_(q){\
    if(q.splfd!=1) q=lrsplfy(q);\
    if(!q.down){\
        switch(q.up){\
            case -1: fputs("-inf",stdout);break;\
            case 0: fputs("NaN",stdout);break;\
            case 1: fputs("+inf",stdout);break;\
        }   return 0;\
    }\
}
#define _isBLANK_(ch) (ch==' '||ch=='\n'||ch=='\t')
#define _isDIGIT_(ch) (ch>='0'&&ch<='9')
#define _ADD_digit_(n,temp) while(_isDIGIT_(temp)) {n=n*10+temp-'0'; temp=getchar();}
#define _point_ADD_digit_(up,down,temp) while(_isDIGIT_(temp)) {up=up*10+temp-'0'; down*=10; temp=getchar();}
#define _SHOULD_END_1_(q,temp) if(temp!='/'&&temp!='.') {ungetc(temp,stdin); q.up*=pozneg; return q;}
#define _SHOULD_END_2_(q,temp) if(temp!='(') {ungetc(temp,stdin); q.up*=pozneg; return q;}
#define _SHOULD_END_3_(q,temp) if(temp!='/') {ungetc(temp,stdin); q.up*=pozneg; return q;}
#define _SHOULD_END_4_(q,temp) if(temp!='.') {ungetc(temp,stdin); q.up*=pozneg; return q;}

//  函数实现和相关解释说明：
#include "rational.c"
//  请将.h.c放在同一目录下。

#undef ll
#undef ul
#undef ull
#undef lr
#undef _putu_ppi_
#undef _putu_
#undef _rapCHECK1_
#undef _rapCHECK2_
#undef _isBLANK_
#undef _isDIGIT_
#undef _ADD_digit_
#undef _point_ADD_digit_
#undef _SHOULD_END_1_
#undef _SHOULD_END_2_
#undef _SHOULD_END_3_
#undef _SHOULD_END_4_
#endif