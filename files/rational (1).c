//  在_AUTO_RAT_TYPE_模式下，为了让函数同时支持不同精度的rat，本库使用\
    了大量的糖衣宏，展开后，单个rat变成多个，如果它是函数的返回值，就造成\
    不必要的重复调用，故不推荐在 自动类型模式 嵌套函数（尤其是get(l)rat）。
//  默认模式，即 手动类型 模式下，您可随意做函数的嵌套。
#ifdef _AUTO_RAT_TYPE_
    /*过渡为 rat*/
rat _rat_(int up,unsigned down,char splfd) {rat q={up,down,splfd}; return q;}
#define _RAT_(q) _rat_((q).up,(q).down,(q).splfd)
    /*过渡为 longrat*/
lr _lr_(ll up,ull down,char splfd) {lr q={up,down,splfd}; return q;}
#define _LR_(q) _lr_((q).up,(q).down,(q).splfd)
//  以上是自动模式下的 隐藏函数 和 糖衣宏。
#endif


    /*longrat 转为 rat*/
rat torat(lr qq) {rat q={qq.up,qq.down,qq.splfd}; return q;}
    /*rat 转为 longrat*/
lr tolr(rat q) {lr qq={q.up,q.down,q.splfd}; return qq;}


//  返回值非负，且有gcd(0,n)==gcd(n,0)==n。
    /*最大公约数*/
ll gcd(ll a,ll b){
    ll temp;
    while(b)
        temp=a%b,a=b,b=temp;
    return a<0 ? -a:a;
}
//  返回值非负，认为0与任何数的最小公倍数为0。
    /*最小公倍数*/
ull lcm(ll a,ll b){
    if(!a||!b) return 0;
    if(a<0) a=-a;
    if(b<0) b=-b;
    ull aa=a,bb=b,temp;
    while(bb)
        temp=aa%bb,aa=bb,bb=temp;
    return a/aa*b;
}


//  做运算的有理数必须经过splfy(simplify)\
    检查，它使用形参，并有权更改rat.splfd。
    /*longrat 约分*/
lr lrsplfy(lr q){
//  无需化简，立即返回。
    if(q.splfd==1) return q;
    if(q.down==1) return q.splfd=1,q;
//  单独处理uncertain的情况。
    if(!q.up&&!q.down)
    return lrUNCERTAIN;
//  正无穷和负无穷能够被归纳。
    ll temp=gcd(q.up,q.down);
        q.up/=temp;
        q.down/=temp;
        q.splfd=1;
    return q;
}
    /*rat 约分*/
rat rsplfy(rat q){
    if(q.splfd==1) return q;
    if(q.down==1) return q.splfd=1,q;
    if(!q.up&&!q.down)
    return rUNCERTAIN;
    int temp=gcd(q.up,q.down);
        q.up/=temp;
        q.down/=temp;
        q.splfd=1;
    return q;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 约分*/
#define lrsplfy(q) lrsplfy(_LR_(q))
    /*rat 约分*/
#define rsplfy(q) rsplfy(_RAT_(q))
#endif


    /*longrat 加法*/
lr lrplus(lr x,lr y){
    if(x.splfd!=1) x=lrsplfy(x);
    if(y.splfd!=1) y=lrsplfy(y);
//  分母为零的无值分数。
    if(!x.down||!y.down){
        //只有一个无值分数则返回它。
        if(x.down) return y;
        if(y.down) return x;
        //认为同种无值分数之和不变。
        if(x.up==y.up) return x;
        //不同的无值分数和为不定态。
        else return lrUNCERTAIN;
    }
//  单独处理同分母。
    if(x.down==y.down){
        x.up+=y.up;
//  各处强制化简前必将splfd置零。
        if(x.down!=1){
            x.splfd=0;
            return lrsplfy(x);
        }else return x;
    }
//  其中一方为零，立即返回另一方。
    if(!x.up)   return y;
    if(!y.up)   return x;
    unsigned temp=lcm(x.down,y.down);
    x.up=(int)(temp/x.down)*x.up
        +(int)(temp/y.down)*y.up;
    x.down=temp;
    return x.splfd=0,lrsplfy(x);
}
    /*rat 加法*/
rat rplus(rat x,rat y){
    lr  xx={x.up,x.down,x.splfd},
        yy={y.up,y.down,y.splfd};
    xx=lrplus(xx,yy);
    rat res={xx.up,xx.down,xx.splfd};
    return res;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 加法*/
#define lrplus(x,y) lrplus(_LR_(x),_LR_(y))
    /*rat 加法*/
#define rplus(x,y) rplus(_RAT_(x),_RAT_(y))
#endif


    /*longrat 求负*/
lr lrneg(lr q) {return q.up=-q.up,q;}
    /*rat 求负*/
rat rneg(rat q) {return q.up=-q.up,q;}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 求负*/
#define lrneg(q) lrneg(_LR_(q))
    /*rat 求负*/
#define rneg(q) rneg(_RAT_(q))
#endif


    /*long rat减法*/
#define lrminus(x,y) lrplus(x,lrneg(y))
    /*rat 减法*/
#define rminus(x,y) rplus(x,rneg(y))


    /*longrat 乘法*/
lr lrtime(lr x,lr y){
    if(x.splfd!=1) x=lrsplfy(x);
    if(y.splfd!=1) y=lrsplfy(y);
//  分母为零的无值分数。
    if(!x.down||!y.down){
//  这三行涉及复杂的分类讨论。
        if(!x.up||!y.up) return lrUNCERTAIN;
        if(x.up>0 ^ y.up>0) return lrNEGINF;
        else return lrINF; //同号得正，异号得负。
    }
//  单独处理整数乘法。
    if(x.down==1==y.down) return x.up*=y.up,x;
    ll temp;
    if(temp=gcd(x.up,y.down),temp!=1)   x.up/=temp,y.down/=temp;
    if(temp=gcd(y.up,x.down),temp!=1)   y.up/=temp,x.down/=temp;
    x.up*=y.up;     x.down*=y.down;     return x;
}
    /*rat 乘法*/
rat rtime(rat x,rat y){
    lr  xx={x.up,x.down,x.splfd},
        yy={y.up,y.down,y.splfd};
    xx=lrtime(xx,yy);
    rat res={xx.up,xx.down,xx.splfd};
    return res;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 乘法*/
#define lrtime(x,y) lrtime(_LR_(x),_LR_(y))
    /*rat 乘法*/
#define rtime(x,y) rtime(_RAT_(x),_RAT_(y))
#endif


//  认为无穷的倒数为0，0的倒数为正无穷，不定态的倒数为不定态。
    /*longrat 倒数*/
lr lrflip(lr q){
    if(q.splfd!=1) lrsplfy(q);
    char minus=0; ll temp;
//  鉴于分母类型为ull，引入minus记录正负性。
    if(q.up<0)
        q.up=-q.up,minus=1;
    temp=q.up;
    q.up=q.down;
    q.down=temp;
    if(minus) q.up=-q.up;
    return q;
}
    /*rat 倒数*/
rat rflip(rat q){
    if(q.splfd!=1) rsplfy(q);
    char minus=0; int temp;
    if(q.up<0)
        q.up=-q.up,minus=1;
    temp=q.up;
    q.up=q.down;
    q.down=temp;
    if(minus) q.up=-q.up;
    return q;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 倒数*/
#define lrflip(q) lrflip(_LR_(q))
    /*rat 倒数*/
#define rflip(q) rflip(_RAT_(q))
#endif


    /*longrat 除法*/
lr lrdivi(lr x,lr y){
    char minus=0; ll temp;
    if(y.up<0)
        y.up=-y.up,minus=1;
    temp=y.up;
    y.up=y.down;
    y.down=temp;
    if(minus) y.up=-y.up;
    return lrtime(x,y);
}
    /*rat 除法*/
rat rdivi(rat x,rat y){
    char minus=0; int temp;
    if(y.up<0)
        y.up=-y.up,minus=1;
    temp=y.up;
    y.up=y.down;
    y.down=temp;
    if(minus) y.up=-y.up;
    return rtime(x,y);
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 除法*/
#define lrdivi(x,y) lrdivi(_LR_(x),_LR_(y))
    /*rat 除法*/
#define rdivi(x,y) rdivi(_RAT_(x),_RAT_(y))
#endif


//  (char)ch可以取加减乘除号。
    /*calc of 2 longrats 运算包*/
lr co2lr(lr x,char ch,lr y){
    switch(ch){
        case '+':return lrplus(x,y);
        case '-':return lrminus(x,y);
        case '*':return lrtime(x,y);
        case '/':return lrdivi(x,y);
        default :return lrUNCERTAIN;
    }
}
    /*calc of 2 rats 运算包*/
rat co2r(rat x,char ch,rat y){
    switch(ch){
        case '+':return rplus(x,y);
        case '-':return rminus(x,y);
        case '*':return rtime(x,y);
        case '/':return rdivi(x,y);
        default :return rUNCERTAIN;
    }
}
#ifdef _AUTO_RAT_TYPE_
    /*calc of 2 longrats 运算包*/
#define co2lr(x,ch,y) co2lr(_LR_(x),ch,_LR_(y))
    /*calc of 2 rats 运算包*/
#define co2r(x,ch,y) co2r(_RAT_(x),ch,_RAT_(y))
#endif


    /*rat 绝对值*/
rat rabs(rat q)
    {return q.up>=0 ? q:(q.up=-q.up,q);}
    /*longrat 绝对值*/
lr lrabs(lr q)
    {return q.up>=0 ? q:(q.up=-q.up,q);}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 绝对值*/
#define lrabs(q) lrabs(_LR_(q))
    /*rat 绝对值*/
#define rabs(q) rabs(_RAT_(q))
#endif


//  ">"->1, "=="->0, "<"->-1.
//  认为无穷和不定态等于自身，且不定态等于任何值。
    /*longrat 比较大小*/
signed char lrcmp(lr x,lr y){
    if(x.up>0 ^ y.up>0)
        return x.up>y.up ? 1:-1;
//  以上防止出现不必要的计算和溢出。
    ll res=lrminus(x,y).up;
    if(res>0)   return 1;
    if(0==res)  return 0;
    if(res<0)   return -1;
}
    /*rat 比较大小*/
signed char rcmp(rat x,rat y){
    if(x.up>0 ^ y.up>0)
        return x.up>y.up ? 1:-1;
//  以上防止出现不必要的计算和溢出。
    int res=rminus(x,y).up;
    if(res>0)   return 1;
    if(0==res)  return 0;
    if(res<0)   return -1;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 比较大小*/
#define lrcmp(x,y) lrcmp(_LR_(x),_LR_(y))
    /*rat 比较大小*/
#define rcmp(x,y) rcmp(_RAT_(x),_RAT_(y))
#endif


//  longrat print，返回输出字符个数。
    /*longrat 分数输出*/
char lrap(lr q){
    _rapCHECK1_(q)
//  rapCHECK宏包含化简检查、无值分数输出。
    char i=0;
    if(q.up<0)
        q.up=-q.up,
        putchar('-'),
        ++i;
    _putu_ppi_(q.up,i)
    if(q.down==1) return i;
//  分数线前后。
    putchar('/'); ++i;
    _putu_ppi_(q.down,i)
    return i;
}
//  rat print，返回输出字符个数。
    /*rat 分数输出*/
char rap(rat q){
    lr qq={q.up,q.down,q.splfd};
    return lrap(qq);
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 分数输出*/
#define lrap(q) lrap(_LR_(q))
    /*rat 分数输出*/
#define rap(q) rap(_RAT_(q))
#endif


//  float longrat print，返回输出字符个数。
    /*longrat 指定小数位数输出*/
int fltlrap(lr q,int n){
    _rapCHECK1_(q)
    int i=0; ll temp;
    if(q.up<0)
        q.up=-q.up,
        putchar('-'),
        ++i;
    if(q.up>=q.down){
        temp=q.up/q.down;
        _putu_ppi_(temp,i);
        q.up%=q.down;
    }else putchar('0'),++i;
//  与下方的--n呼应，提速并剔除负精度。
    if(++n==1) return i;
    if(n<1) n=7; //遇负精度，输出六位。
    putchar('.');++i;
    while(--n)
        q.up*=10,++i,
        putchar(q.up/q.down+'0'),
        q.up%=q.down;
    return i;
}
    /*rat 指定小数位数输出*/
int fltrap(rat q,int n){
    lr qq={q.up,q.down,q.splfd};
    return fltlrap(qq,n);
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 指定小数位数输出*/
#define fltlrap(q,n) fltlrap(_LR_(q),n)
    /*rat 指定小数位数输出*/
#define fltrap(q,n) fltrap(_RAT_(q),n)
#endif


//  返回循环节长度，支持输出long long级长度的循环节。
    /*longrat 小数输出*/
ll lrdeci(lr q){
    _rapCHECK2_(q)
    if(q.up<0)
        putchar('-'),
        q.up=-q.up;
    ull temp;
    if(q.up>=q.down){
        temp=q.up/q.down;
        _putu_(temp);
        q.up%=q.down;
    }else putchar('0');
    if(!q.up) return 0;

    putchar('.');
    temp=q.down;
    ll n1=0,n2=0;
//  根据数论的结论，分母中包含因数5或2的个数，\
    决定了混循环小数的小数点与循环节间“废位”数。
    while(! (temp%5))   ++n1,temp/=5;
    while(! (temp%2))   ++n2,temp/=2;
    if(n2>n1) n1=n2;    //n1存放废位长度。

    for(char i=0;i<n1;++i){
        q.up*=10;
        putchar(q.up/q.down+'0');
        q.up%=q.down;
    }   n1=q.up*10;
//  发现是有限小数，截停。
    if(!(n1%q.down))return 0;

    putchar('(');   putchar(n1/q.down+'0');
    n1%=q.down;     n2=1;
//  q.up存放循环节首余数，n2记录循环节长度。
    while(n1!=q.up){
        putchar(n1*10/q.down+'0');
        ++n2;
        n1=n1*10%q.down;
    }   putchar(')');
    return n2;
}
    /*rat 小数输出*/
int rdeci(rat q){
    lr qq={q.up,q.down,q.splfd};
    return lrdeci(qq);
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 小数输出*/
#define lrdeci(q) lrdeci(_LR_(q))
    /*rat 小数输出*/
#define rdeci(q) rdeci(_RAT_(q))
#endif


//  支持分数形式和（循环）小数形式，用 *英文* 括号标出循环节。
//  getlr丢弃前导空白符，开头若连续出现多个负号，则视为一个，\
    遇到 无关字符 或右括号')'停止输入，不吸收无关字符，吸收')'。
//  getlr支持小数点前后、分数线前后缺失。当分子或分母缺失时，\
    getlr默认它（们）为 *1*。遇到无效输入，getlr返回 不定态。\
    另外，getlr不支持"inf""+inf""-inf""NaN"之类输入，请用\
    "1/0""-1/0""0/0"的分数形式代表它们。
//  为了提高效率，getlr()基于getchar()书写，不支持反斜线，不\
    支持全角数字，您也可以做修改。
    /*longrat 综合输入*/
lr getlr(){
    lr q=lrZERO; char temp;
//  pozneg记录正负性，赋初值为1。
    signed char pozneg=1;
//  丢弃开头的空白字符，包括换行。
    do temp=getchar();
    while(_isBLANK_(temp));
    _reset_:switch(temp){
        case '-': pozneg=-1; temp=getchar(); goto _reset_;
        case '.': goto _afterpoint_;
        case '/': q.up=1; goto _afterslash_;
        default : if(!_isDIGIT_(temp)){
            ungetc(temp,stdin);
            return lrUNCERTAIN;
    }}
//  记录分子或整数部分，直到出现非数字。
    _ADD_digit_(q.up,temp)
//  遇到无关字符，整理正负性并输出。
    _SHOULD_END_1_(q,temp)
//  如果遇到分数线……
    if(temp=='/'){ _afterslash_:;
        signed char pzng=1;
        _reget_: temp=getchar();
        if(temp=='-')
            {pzng=-1;goto _reget_;}
        if(_isDIGIT_(temp))
            q.down=0;
        _ADD_digit_(q.down,temp)
        ungetc(temp,stdin);
//  整理正负性。
        if(pozneg!=pzng)
            q.up=-q.up;
//  强制化简。
        q.splfd=0;
        return lrsplfy(q);
    }
//  如果遇到小数点……
    if(temp=='.'){ _afterpoint_:;
        lr qq=lrZERO;
        ull down=1;
        temp=getchar();
//  记录非循环小数部分。
        _point_ADD_digit_(qq.up,down,temp)
        qq.down=down;
        qq.splfd=0; //须强制化简。
        q=lrplus(q,qq);
//  遇到无关字符，整理正负性并输出。
        _SHOULD_END_2_(q,temp)
//  遇到了左括号，还需要处理循环节。
        qq=lrZERO;
        temp=getchar();
        _point_ADD_digit_(qq.up,qq.down,temp)
        if(temp!=')')
            ungetc(temp,stdin);
//  防止括号间无内容。
        if(qq.down!=1)
//  为了衔接循环部分，down储存了数量级。
            qq.down=(qq.down-1)*down,
            qq.splfd=0,
            q=lrplus(q,qq);
//  整理正负性，q已化简过。
        q.up*=pozneg;
        return q;
    }
}
    /*rat 综合输入*/
rat getrat(){
    lr qq=getlr();
    rat q={qq.up,qq.down,qq.splfd};
    return q;
}
//  以下四个函数为功能分裂产物。
    /*longrat 分数输入*/
lr getlfrac(){
    lr q=lrZERO; char temp;
    signed char pozneg=1;
    do temp=getchar();
    while(_isBLANK_(temp));
    _reset_:switch(temp){
        case '-': pozneg=-1; temp=getchar(); goto _reset_;
        case '/': q.up=1; goto _afterslash_;
        default : if(!_isDIGIT_(temp)){
            ungetc(temp,stdin);
            return lrUNCERTAIN;
    }}
    _ADD_digit_(q.up,temp)
    _SHOULD_END_3_(q,temp)

    _afterslash_:;
    signed char pzng=1;
    _reget_: temp=getchar();
    if(temp=='-')
        {pzng=-1;goto _reget_;}
    if(_isDIGIT_(temp))
        q.down=0;
    _ADD_digit_(q.down,temp)
    ungetc(temp,stdin);
    if(pozneg!=pzng)
        q.up=-q.up;
    q.splfd=0;
    return lrsplfy(q);
}
    /*rat 分数输入*/
rat getfrac(){
    lr qq=getlfrac();
    rat q={qq.up,qq.down,qq.splfd};
    return q;
}
    /*longrat 小数输入*/
lr getldeci(){
    lr q=lrZERO; char temp;
    signed char pozneg=1;
    do temp=getchar();
    while(_isBLANK_(temp));
    _reset_:switch(temp){
        case '-': pozneg=-1; temp=getchar(); goto _reset_;
        case '.': goto _afterpoint_;
        default : if(!_isDIGIT_(temp)){
            ungetc(temp,stdin);
            return lrUNCERTAIN;
    }}
    _ADD_digit_(q.up,temp)
    _SHOULD_END_4_(q,temp)

    _afterpoint_:;
    lr qq=lrZERO;
    ull down=1;
    temp=getchar();
    _point_ADD_digit_(qq.up,down,temp)
    qq.down=down;
    qq.splfd=0;
    q=lrplus(q,qq);
    _SHOULD_END_2_(q,temp)
    qq=lrZERO;
    temp=getchar();
    _point_ADD_digit_(qq.up,qq.down,temp)
    if(temp!=')')
    ungetc(temp,stdin);
    if(qq.down!=1)
    qq.down=(qq.down-1)*down,
    qq.splfd=0,
    q=lrplus(q,qq);
    q.up*=pozneg;
    return q;
}
    /*rat 小数输入*/
rat getdeci(){
    lr qq=getldeci();
    rat q={qq.up,qq.down,qq.splfd};
    return q;
}


    /*longrat 化为 double*/
double lrtodb(lr q)
    {return q.up/(double)q.down;}
    /*rat 化为 double*/
double rattodb(rat q)
    {return q.up/(double)q.down;}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 化为 double*/
#define lrtodb(q) lrtodb(_LR_(q))
    /*rat 化为 double*/
#define rattodb(q) rattodb(_RAT_(q))
#endif


//  以int边界代替所谓的无穷，认为rUNCERTAIN对应0。
    /*rat 化为整数*/
int rattoint(rat q){
    if(!q.down){
        if(!q.up)   return 0;
        if(q.up>0)  return INT_MAX;
        if(q.up<0)  return INT_MIN;
    }
    return q.up/(int)q.down;
}
#ifdef _AUTO_RAT_TYPE_
    /*rat 化为整数*/
#define rattoint(q) rattoint(_RAT_(q))
#endif


//  以边界代替所谓的无穷，认为lrUNCERTAIN对应0。
#if INT_MAX < LONG_MAX
    /*longrat 化为整数*/
long lrtolong(lr q){
    if(!q.down){
        if(!q.up)   return 0;
        if(q.up>0)  return LONG_MAX;
        if(q.up<0)  return LONG_MIN;
    }
    return q.up/(long)q.down;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 化为整数*/
#define lrtolong(q) lrtolong(_LR_(q))
#endif
#else
    /*longrat 化为整数*/
ll lrtoll(lr q){
    if(!q.down){
        if(!q.up)   return 0;
        if(q.up>0)  return LONG_LONG_MAX;
        if(q.up<0)  return LONG_LONG_MIN;
    }
    return q.up/(ll)q.down;
}
#ifdef _AUTO_RAT_TYPE_
    /*longrat 化为整数*/
#define lrtoll(q) lrtoll(_LR_(q))
#endif
#endif