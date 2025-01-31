#include <stdio.h>
#include "rational.h"
#define g getrat()
//这程序测试rational中rat族函数。
int main(){
    rat q;int opt;
    while(opt=0,1){
        puts("测试项目：\n\
1.加法  2.减法  3.乘法  4.除法\n\
5.化简  6.负数  7.倒数  8.输入\n\
9.分数输出   10.浮点输出   11.小数输出\n\
12.Q转Z     13.Q转flt     14.最大公约数\n\
15.最小公倍数 16.比较大小 17.二元运算包");
        scanf("%d",&opt);
        switch(opt){
            case 1: rap(rplus(g,g)); break;
            case 2: q=g; rap(rminus(q,g)); break;
            case 3: rap(rtime(g,g)); break;
            case 4: q=g; rap(rdivi(q,g)); break;
            case 5: rap(rsplfy(g)); break;
            case 6: rap(rneg(g)); break;
            case 7: rap(rflip(g)); break;
            case 8:
            case 9: puts("输入提示：用英文括号标出循环节。"); printf("\n输出%d个字符",rap(g)); break;
            case 10: q=g; printf("\n输出%d个字符",fltrap(q,g.up)); break;
            case 11: printf("\n循环节长%d位",rdeci(g)); break;
            case 12: printf("%d",rattoint(g)); break;
            case 13: printf("%f",rattodb(g)); break;
            case 14: printf("%d",gcd(g.up,g.up)); break;
            case 15: printf("%u",lcm(g.up,g.up)); break;
            case 16: q=g; printf("%d",rcmp(q,g)); break;
            case 17: puts("输入示范：\"-2.4+-7.9\" \"3/2/6/5\" \"0.(428571)*2.(3)\" \".-/\"");
                q=g; opt=getchar(); rap(co2r(q,opt,g)); break;
            default: ;
        }   fflush(stdin); putchar('\n');
    }
}