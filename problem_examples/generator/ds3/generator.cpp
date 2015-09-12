#include <bits/stdc++.h>

#ifdef putchar
#undef putchar
#endif
#define putchar(x) (*_pout++=(x), (_pout==_eout?(fwrite(_pout=_out, 1, 4096, stdout)):0))
#define flush() fwrite(_out, 1, _pout-_out, stdout)
char _out[4097], *_eout=_out+4096, *_pout=_out;
#define print(x) do{if(!x)putchar('0');else{for(;x;x/=10)*_p++='0'+x%10;do putchar(*--_p);while(_p!=_buf);}}while(0)
char _buf[20], *_p=_buf;
#define putchare(x) (*_poute++=(x), (_poute==_eoute?(fwrite(_poute=_oute, 1, 4096, stderr)):0))
#define flushe() fwrite(_oute, 1, _poute-_oute, stderr)
char _oute[4097], *_eoute=_oute+4096, *_poute=_oute;
#define printe(x) do{if(!x)putchare('0');else{for(;x;x/=10)*_p++='0'+x%10;do putchare(*--_p);while(_p!=_buf);}}while(0)

#include <limits>
#include <random>
std::mt19937 random_engine(0xdeadbeef);
template<class T>
T randint(T L, T R)
{
    return L+(unsigned long long)(R-L+1)*random_engine()/(std::numeric_limits<unsigned int>::max()+1ULL);
}
#include <utility>
std::pair<int, int> randpair(int L, int R, int min_dist)
{
    int a=randint(L, R-min_dist+1);
    int b=randint(a+min_dist-1, R);
    return std::make_pair(a, b);
}

using namespace std;

int A[100001];

struct node
{
    int mini;
    int gcd;
    int cnt;
} seg[262144];

int _gcd(int a, int b)
{
    if(!b)
        return a;
    return _gcd(b, a%b);
}

node combine(node a, node b)
{
    node ret;
    ret.mini=min(a.mini, b.mini);
    ret.gcd=_gcd(a.gcd, b.gcd);
    ret.cnt=0;
    if(ret.gcd==a.gcd)
        ret.cnt+=a.cnt;
    if(ret.gcd==b.gcd)
        ret.cnt+=b.cnt;
    return ret;
}

void init(int idx, int begin, int end)
{
    if(begin==end)
    {
        seg[idx].mini=A[begin];
        seg[idx].gcd=A[begin];
        seg[idx].cnt=1;
    }
    else
    {
        int mid=(begin+end)/2;
        init(idx*2, begin, mid);
        init(idx*2+1, mid+1, end);
        seg[idx]=combine(seg[idx*2], seg[idx*2+1]);
    }
}

void update(int idx, int begin, int end, int x, int v)
{
    if(x<begin || end<x)
        return;
    if(begin==end)
    {
        seg[idx].mini=v;
        seg[idx].gcd=v;
        seg[idx].cnt=1;
    }
    else
    {
        int mid=(begin+end)/2;
        update(idx*2, begin, mid, x, v);
        update(idx*2+1, mid+1, end, x, v);
        seg[idx]=combine(seg[idx*2], seg[idx*2+1]);
    }
}

node query(int idx, int begin, int end, int i, int j)
{
    if(j<begin || end<i)
        return (node){0x3f3f3f3f, 0, 0};
    if(i<=begin && end<=j)
        return seg[idx];
    int mid=(begin+end)/2;
    return combine(query(idx*2, begin, mid, i, j),
                   query(idx*2+1, mid+1, end, i, j));
}

void gen(int N, int M, int unique_base, int max_gap, int p_c, int p_m, int p_g, int p_q, int min_dist)
{
    random_engine.seed(0xdeadbeef^N^M^unique_base^max_gap^p_c^p_m^p_g^p_q^min_dist);
    int tmp;
    tmp=N;
    print(tmp);
    putchar(' ');
    tmp=M;
    print(tmp);
    putchar('\n');
    int max_mul=1000000000/unique_base;
    int last=0;
    int mul=1;
    for(int i=1; i<=N; i++)
    {
        A[i]=randint(1, unique_base)*mul;
        if(randint(0, max_gap-(i-last))==0)
        {
            mul=randint(1, max_mul);
            last=i;
        }
        tmp=A[i];
        print(tmp);
        putchar(" \n"[i==N]);
    }
    int p_all=p_c+p_m+p_g+p_q;
    init(1, 1, N);
    char c;
    int a, b;
    pair<int, int> p;
    for(int i=0; i<M; i++)
    {
        c=randint(0, p_all-1);
        if(c<p_c)
        {
            putchar('C');
            a=randint(1, N);
            b=randint(1, 1000000000);
            update(1, 1, N, a, b);
        }
        else
        {
            p=randpair(1, N, min_dist);
            a=p.first;
            b=p.second;
            if(c<p_c+p_m)
            {
                putchar('M');
                tmp=query(1, 1, N, a, b).mini;
            }
            else if(c<p_c+p_m+p_g)
            {
                putchar('G');
                tmp=query(1, 1, N, a, b).gcd;
            }
            else
            {
                putchar('Q');
                tmp=query(1, 1, N, a, b).cnt;
            }
            printe(tmp);
            putchare('\n');
        }
        putchar(' ');
        print(a);
        putchar(' ');
        print(b);
        putchar('\n');
    }
    flush();
    flushe();
    fflush(stdout);
    fflush(stderr);
}

int main()
{
    int T;
    scanf("%d", &T);
    switch(T)
    {
    case 1:
        gen(5, 5, 2, 3, 1, 1, 1, 1, 1);
        break;
    case 2:
        gen(10, 20, 10, 5, 0, 1, 1, 1, 1);
        break;
    case 3:
        gen(100, 500, 100, 20, 0, 1, 0, 0, 1);
        break;
    case 4:
        gen(100, 500, 10, 20, 0, 0, 1, 0, 1);
        break;
    case 5:
        gen(100, 500, 10, 20, 0, 0, 0, 1, 1);
        break;
    case 6:
        gen(100, 500, 30, 10, 1, 1, 0, 0, 1);
        break;
    case 7:
        gen(100, 500, 30, 10, 1, 0, 1, 0, 1);
        break;
    case 8:
        gen(100, 500, 3, 100, 1, 0, 0, 1, 1);
        break;
    case 9:
        gen(1000, 10000, 10, 100, 1, 2, 2, 2, 10);
        break;
    case 10:
        gen(10000, 50000, 1, 200, 1, 5, 5, 5, 100);
        break;
    case 11:
        gen(10000, 50000, 50, 1000, 1, 25, 25, 25, 1);
        break;
    case 12:
        gen(100000, 500000, 50, 1000, 0, 0, 0, 1, 1);
        break;
    case 13:
        gen(100000, 500000, 100, 1000, 0, 1, 1, 1, 50000);
        break;
    case 14:
        gen(100000, 500000, 100, 1000, 1, 1, 1, 1, 50000);
        break;
    case 15:
        gen(100000, 500000, 100, 1000, 100, 1, 1, 1, 1000);
        break;
    case 16:
        gen(100000, 500000, 100, 1000, 1, 100, 1, 1, 1000);
        break;
    case 17:
        gen(100000, 500000, 100, 1000, 1, 1, 100, 1, 1000);
        break;
    case 18:
        gen(100000, 500000, 100, 1000, 1, 1, 1, 100, 1000);
        break;
    case 19:
        gen(100000, 500000, 10000, 10000, 1, 100, 10, 10, 1000);
        break;
    case 20:
        gen(100000, 500000, 100000, 50000, 1, 1000, 1000, 1000, 10000);
        break;
    }
    return 0;
}
