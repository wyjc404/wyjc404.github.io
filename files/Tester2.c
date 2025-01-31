#include <stdio.h>
#define _AUTO_RAT_TYPE_
#include "rational.h"
#define g getlr()
//这程序测试rational中longrat族函数。
int main(){
    longrat p,q;int opt;
    while(opt=0,1){
        puts("测试项目：\n\
1.加法  2.减法  3.乘法  4.除法\n\
5.化简  6.负数  7.倒数  8.输入\n\
9.分数输出   10.浮点输出   11.小数输出\n\
12.Q转Z     13.Q转flt     14.最大公约数\n\
15.最小公倍数 16.比较大小 17.二元运算包");
        scanf("%d",&opt);
        switch(opt){
            case 1: p=g,q=g; lrap(lrplus(p,q)); break;
            case 2: p=g,q=g; lrap(lrminus(p,q)); break;
            case 3: p=g,q=g; lrap(lrtime(p,q)); break;
            case 4: p=g,q=g; lrap(lrdivi(p,q)); break;
            case 5: q=g; lrap(lrsplfy(q)); break;
            case 6: q=g; lrap(lrneg(q)); break;
            case 7: q=g; lrap(lrflip(q)); break;
            case 8:
            case 9: q=g; puts("输入提示：用英文括号标出循环节。"); printf("\n输出%d个字符",lrap(q)); break;
            case 10: q=g; printf("\n输出%d个字符",fltlrap(q,g.up)); break;
            case 11: q=g; printf("\n循环节长%d位",lrdeci(q)); break;
            case 12: q=g; printf("%d",lrtoll(q)); break;
            case 13: q=g; printf("%f",lrtodb(q)); break;
            case 14: printf("%d",gcd(g.up,g.up)); break;
            case 15: printf("%u",lcm(g.up,g.up)); break;
            case 16: p=g;q=g; printf("%d",lrcmp(p,q)); break;
            case 17: puts("输入示范：\"-2.4+-7.9\" \"3/2/6/5\" \"0.(428571)*2.(3)\" \".-/\"");
                p=g; q=g; opt=getchar(); lrap(co2lr(p,opt,q)); break;
            default: ;
        }   fflush(stdin); putchar('\n');
    }
}