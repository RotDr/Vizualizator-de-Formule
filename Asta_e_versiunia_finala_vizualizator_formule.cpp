#include <iostream>
#include<stack>
#include<vector>
#include <fstream>
#include <winbgim.h>
#include <graphics.h>
#include <windows.h>
#include<mmsystem.h>
#include<stdlib.h>
#pragma comment(lib,"winmm.lib")
HDC BGI__GetWinbgiDC(HWND hWnd=NULL);
void BGI__ReleaseWinbgiDC(HWND hWnd=NULL);
using namespace std;
ifstream fin("listaputeri.in");
ifstream din("texting.in");
ifstream bin("meniu.in");
ifstream gin("setari.in");
ifstream him("numere.in");
ifstream nothim("credit.in");
ifstream finna("font.in");
ifstream operatori("operatori.in");
ifstream trig("functiiTrig.in");
char formul[400]="";
char ch[400],ch1[200],ch2[200];
char c;
int H,b,i;
int fontsize[6]={10,20,30,40,50,60};
char musicname[6][15]={"Menu.wav","Starting.wav","Informatii.wav","setari.wav","Credits.wav"};
bool muzica=1;
bool muzic[6]={0},nomu=0;
int color1=15,color2=0;
int eymin,eymax=800,exmax=800,exmin;
bool ok=1;
struct buton
{
    int xmin,ymin,xmax,ymax;
};
struct buton_info
{
    int xmin,ymin,xmax,ymax;
    char ch[100];
};
bool atingebuton (int x, int y, buton b)
{
    return x>=b.xmin && x<=b.xmax && y>=b.ymin && y<=b.ymax;
}
bool atingebuton (int x, int y, buton_info b)
{
    return x>=b.xmin && x<=b.xmax && y>=b.ymin && y<=b.ymax;
}
void dreptunghi (buton b)
{
    rectangle(b.xmin,b.ymin,b.xmax,b.ymax);
}
void dreptunghi (buton_info b)
{
    rectangle(b.xmin,b.ymin,b.xmax,b.ymax);
}
void formula(char* formul);
void Inceput();
void informati();
void settings();
void starting();
void goodbye();
void failure();
void credits();
void info_operatori();
void info_functii();
int n,j,k;
bool o,calc;
vector<char*>v;
stack<char*>st;
double font=30;
char mat[20][100]= {"abs","lg","ln","sqrt","sin","cos","tg","ctg","arcsin","arccos","arctg","arcctg","ops","rad","log","lim","sum","integrala","integraladef"};
int prioritate(char *l)
{
    //if(l[0]=='')
    if(l[0]=='(') return 0;
    if(l[0]==',') return 1;
    if(l[0]=='+' or l[0]=='-')
        return 2;
    if(l[0]=='*' or l[0]=='/' or l[0]==':')
        return 3;
    if(l[0]=='^')
        return 4;
    return 5;
}
bool isoperator(char ch)
{
    return ch=='+' or ch=='-' or ch=='*' or ch=='/' or ch=='^' or ch==',' or ch==':';
}
int aritate(char *l)
{
    if(isoperator(l[0]))
        return 2;
    if(l[0]=='l' && l[1]=='o')
        return 2;
    if(l[0]=='r')
        return 2;
    if(strlen(l)>=12 && l[0]=='i' && l[1]=='n' && l[2]=='t')
        return 4;
    if(strlen(l)>=9 && l[0]=='i' && l[1]=='n' && l[2]=='t')
        return 2;
    if(strlen(l)>=3 && l[0]=='s' && l[1]=='u')
        return 4;
    if(strlen(l)>=3 && l[0]=='l' && l[1]=='i' && l[2]=='m')
        return 3;
    return 1;
}
char *numar(char *l)
{
    char*p=new char[1001];
    int j=0;
    int cnt=0;
    while(j < strlen(l))
    {
        if(l[j]=='.')
        {
            cnt++;
            if(cnt>1) return NULL;
        }
        else if(isdigit(l[j])==0)
            break;
        j++;
    }
    strncpy(p, l,j);
    p[j] = '\0';
    return p;
}
char* isfunction(char *l)
{
    for(int i=0; i<19; i++)
    {
        bool ok=1;
        for(int j=0; j<strlen(mat[i]); j++)
            if(l[j]!=mat[i][j])
            {
                ok=0;
                break;
            }
        if(ok==1 && (l[strlen(mat[i])]=='(' or isdigit(l[strlen(mat[i])]) or isfunction(l+strlen(mat[i])) or ((l[strlen(mat[i])]>='a' &&  l[strlen(mat[i])]<='z') && ((l[strlen(mat[i])+1]=='\0') or isoperator(l[strlen(mat[i])+1])))))
            return mat[i];
    }
    return NULL;
}
char* isvariable(char *l)
{
    char*p=new char[1001];
    int j=0;
    cout<<l<<" ";
    cout<<"caca";
    while(j < strlen(l))
    {
        if(!(l[j]>='a' && l[j]<='z'))
            break;
        j++;
    }
    cout<<"Pipi"<<'\n';
    strncpy(p, l,j);
    p[j] = '\0';
    if(j==0) return NULL;
    return p;
}
char* issymbol(char *s)
{
    char p[10][10]= {"pi","inf"};
    for(int j=0; j<2; j++)
        if(strcmp(s,p[j]))
            return p[j];
    return NULL;
}
struct nod
{
    char* info;
    nod* st;
    nod* dr;
    int xmin,xmax,ymin,ymax;
    int tip;
    double L,H;
    double marime;
    int centery;
};
void preamareformula(nod * p);
nod*rad;
nod* init_nod(const char* val)
{
    nod* new_nod = new nod;
    new_nod->st = NULL;
    new_nod->dr = NULL;
    new_nod->info = new char[strlen(val) + 1];
    strcpy(new_nod->info, val);
    new_nod->marime=1.0;
    return new_nod;
}
stack<nod*>stc;
void arbbuild(nod*& rad)
{
    for(int i=0; i<v.size(); i++)
    {
        if(isdigit(v[i][0]) or (v[i][0]>='a' && v[i][0]<='z' &&strlen(v[i])==1) or (strlen(v[i])==3 &&v[i][2]=='f') or (v[i][0]=='p' && v[i][1]=='i'))
        {
            nod* newnod=init_nod(v[i]);
            newnod->tip=0;
            stc.push(newnod);
        }
        else
        {
            int x=aritate(v[i]);
            nod* rad;
            if(v[i][0]=='o')
                rad=init_nod("-");
            else rad=init_nod(v[i]);
            if(x==1)
            {
                if (stc.empty())
                {
                    cout<<"I FALIN";
                    failure();
                    cleardevice();
                    Inceput();
                }
                rad->st=stc.top();
                stc.pop();
            }
            else if(x==2 && stc.top()->info[0]!=',')
            {
                if (stc.empty())
                {
                    cout<<"I FALIN";
                    failure();
                    cleardevice();
                    Inceput();
                }
                rad->dr=stc.top();
                stc.pop();
                if (stc.empty())
                {
                    cout<<"I FALIN";
                    failure();
                    cleardevice();
                    Inceput();
                }
                rad->st=stc.top();
                stc.pop();
            }
            else if(x==2 && stc.top()->info[0]==',')
            {
                if (stc.empty() || stc.top()->dr==NULL || stc.top()->st==NULL)
                {
                    cout<<"I FALIN";
                    failure();
                    cleardevice();
                    Inceput();
                }
                rad->dr=stc.top()->dr;
                rad->st=stc.top()->st;
                stc.pop();
            }
            else if(x==3)
            {
                if (stc.empty() || stc.top()->dr==NULL || stc.top()->st==NULL)
                {
                    failure();
                    cleardevice();
                    Inceput();
                }
                rad->dr=stc.top()->dr;
                rad->st=stc.top()->st;
                stc.pop();
            }
            else if(x==4)
            {
                if (stc.empty() || stc.top()->dr==NULL || stc.top()->st==NULL)
                {
                    failure();
                    cleardevice();
                    Inceput();
                }
                rad->dr=stc.top()->dr;
                rad->st=stc.top()->st;
                stc.pop();
                swap(rad->dr,rad->st->st);
                swap(rad->st,rad->dr);
            }
            rad->tip=x;
            stc.push(rad);
        }
    }
    if (stc.empty())
    {
        failure();
        cleardevice();
        Inceput();
    }
    rad=stc.top();
    rad->marime=1.0;
}
void dreptunghi (nod * p)
{
    line (p->xmin,p->ymin,p->xmin,p->ymax);
    line (p->xmin,p->ymin,p->xmax,p->ymin);
    line (p->xmax,p->ymax,p->xmax,p->ymin);
    line (p->xmax,p->ymax,p->xmin,p->ymax);
}
void drawLeftParenthesis(int x, int y, int height, int thickness)
{
    int radius = height/15;
    int verticalLineHeight = height-radius*2;
    for (int i = 0; i < thickness; i++)
    {
        arc(x+radius, y + radius, 90,180, radius);
        line(x, y + radius, x, y + radius + verticalLineHeight);
        arc(x+radius, y + radius + verticalLineHeight, 180,270, radius);
        x++;
        y++;
    }
}
void drawRightParenthesis(int x, int y, int height, int thickness)
{
    int radius = height/15;
    int verticalLineHeight = height-radius*2;
    for (int i = 0; i < thickness; i++)
    {
        arc(x-radius, y + radius, 0, 90, radius);
        line(x, y + radius, x, y + radius + verticalLineHeight);
        arc(x-radius, y + radius + verticalLineHeight, 270, 0, radius);
        x--;
        y++;
    }
}
void change_font(double marime,double font)
{
    HFONT hFont=CreateFont(font* marime,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    HDC hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
}
void paradim (nod *&p)
{
    HFONT hFont=CreateFont(p->H+font*p->marime,20*p->marime,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    HDC hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
    p->L+=2*textwidth("(");
    hFont=CreateFont(font*p->marime,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
}
void parantezare (nod *&p)
{
    HFONT hFont=CreateFont(p->H,20*p->marime,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    HDC hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
    p->xmin+=textwidth("("),p->xmax-=textwidth("(");
    p->L-=2*textwidth("(");
    if(p->H<=font)
    {
        int centerx=(p->xmin*2-textwidth("("))/2;
        int centery=(p->ymin+p->ymax)/2;
        outtextxy(centerx-textwidth("(")/2,centery-textheight("(")/2,"(");
        centerx=(p->xmax*2+textwidth(")"))/2;
        outtextxy(centerx-textwidth(")")/2,centery-textheight(")")/2,")");
    }
    else
    {
        int centerx=(p->xmin*2-textwidth("("))/2;
        drawLeftParenthesis(centerx,p->ymin,p->H,3);
        centerx=(p->xmax*2+textwidth(")"))/2;
        drawRightParenthesis(centerx,p->ymin,p->H,3);
    }
    hFont=CreateFont(font*p->marime,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
}
void bazadim(nod*& b)
{
    change_font(b->marime,font);
    b->H=textheight(b->info);
    b->L=textwidth(b->info);
    b->xmin=0;
    b->xmax=b->xmin+b->L;
    b->ymin=0;
    b->ymax=b->ymin+b->H;
    b->centery=(b->ymin+b->ymax)/2;
}
void drawPiSymbol(int x, int y, int w,int h)
{
    // Draw two vertical lines and one horizontal line
    line(x+w/5, y+h/3, x+w/5, y +h*4/5); // Left vertical line
    line(x + w*4/5, y+h/3, x + w *4/5, y + h*4/5); // Right vertical line
    line(x, y+h/3, x + w, y+h/3); // Connecting horizontal line
}
void drawInfinitySymbol(int x, int y, int width, int height)
{
    int center1=x-width/4;
    int center2=x+width/4;
    circle(center1,y,width/4);
    circle(center2,y,width/4);
}
void baza(nod* b)
{
    cout<<b->info<<" "<<b->marime<<'\n';
    change_font(b->marime,font);
    double w=textwidth(b->info);
    double h=textheight(b->info);
    double centerx=(b->xmin+b->xmax)/2;
    if(b->info[0]=='p' && b->info[1]=='i')
        drawPiSymbol(centerx-w/2,b->centery-h/2,w,h);
    else if(b->info[0]=='i' && b->info[1]=='n')
        drawInfinitySymbol(centerx,b->centery,w,h);
    else outtextxy(centerx-w/2,b->centery-h/2,b->info);
}
void negadim (nod *&p)
{
    nod *a=p->st;
    p->H=a->H;
    if(a->info[0]=='-' or a->info[0]=='+')
        paradim(a);
    change_font(p->marime,font);
    p->L=a->L+font*p->marime;
    p->xmin=0;
    p->xmax=p->xmin+p->L;
    a->xmin=p->xmin+font*p->marime;
    a->xmax=a->xmin+a->L;
    p->ymax=a->ymax;
    p->ymin=a->ymin;
    p->centery=a->centery;
}
void limdim(nod *&p)
{
    change_font(font,p->marime);
    nod *a=p->st->st;
    nod*b=p->st->dr;
    nod*c=p->dr;
    p->L=max(textwidth("lim"),int(a->L+font*2*p->marime+b->L))+c->L+font*p->marime;
    p->H=c->centery-c->ymin+max(textheight("lim")/2+max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery),c->ymax-c->centery);
    a->xmin=0;
    a->xmax=a->L;
    c->ymin=0;
    c->ymax=c->ymin+c->H;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymin+=c->centery+textheight("lim")/2;
    b->ymin+=c->centery+textheight("lim")/2;
    a->centery=b->centery=b->centery+c->centery+textheight("lim")/2;
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    b->xmin=a->xmax+2*font*p->marime;
    b->xmax=b->xmin+b->L;
    c->xmax=p->L;
    c->xmin=c->xmax-c->L;
    p->xmin=0;
    p->xmax=p->L;
    p->ymin=0;
    p->ymax=p->H;
    p->centery=c->centery;
    /*p->st->xmin=a->xmin;
    p->st->xmax=b->xmax;
    p->st->L=p->st->xmax-p->st->xmin;
    p->st->ymin=min(a->ymin,b->ymin);
    p->st->ymax=max(b->ymax,a->ymax);
    p->st->H=p->st->ymax-p->st->ymin;
    p->st->centery=a->centery;*/
}
void limita(nod *p)
{
    change_font(p->marime,font);
    nod*a=p->st->st;
    nod*b=p->st->dr;
    cout<<"DESENEZ LIMITA"<<a->info<<": "<<a->xmin<<" "<<a->ymin<<" "<<a->xmax<<" "<<a->ymax<<'\n';
    cout<<b->info<<" "<<b->xmin<<" "<<b->ymin<<" "<<b->xmax<<" "<<b->ymax<<'\n';
    cout<<"TATA"<<p->st->info<<'\n';
    outtextxy((a->xmax+b->xmin)/2-textwidth("lim")/2,p->centery-textheight("lim")/2,"lim");
    line(a->xmax+font*p->marime/2,a->centery,b->xmin-font*p->marime/2,a->centery);
    line(a->xmax+font*p->marime/2+font*p->marime*7/8,a->centery-font*p->marime/8,b->xmin-font*p->marime/2,a->centery);
    line(a->xmax+font*p->marime/2+font*p->marime*7/8,a->centery+font*p->marime/8,b->xmin-font/2*p->marime,a->centery);
}
void negativ(nod*p)
{
    nod*b=p->st;

    for(int i=-1; i<=1; i++)
    {
        if(p->marime!=1)
            i++;
        line (p->xmin+font*p->marime/4,p->centery+i,b->xmin-font*p->marime/4,p->centery+i);
        if(p->marime!=1)
            break;
    }
    if(b->info[0]=='-' or b->info[0]=='+')
        parantezare(b);
}
void dividim (nod* &divi)
{
    change_font(divi->marime,font);
    nod *a=divi->st;
    nod *b=divi->dr;
    divi->L=max(a->L,b->L)+font*divi->marime;
    divi->H=a->H+b->H+font*divi->marime;
    a->xmin=(divi->L-a->L)/2;
    a->xmax=a->xmin+a->L;
    a->ymin=0;
    a->ymax=a->ymin+a->H;
    b->xmin=(divi->L-b->L)/2;
    b->xmax=b->xmin+b->L;
    b->ymin=a->ymax+font*divi->marime;
    b->ymax=b->ymin+b->H;
    b->centery+=a->H+font*divi->marime;
    divi->xmin=0;
    divi->xmax=divi->xmin+divi->L;
    divi->ymin=0;
    divi->ymax=divi->ymin+divi->H;
    double ceny=divi->ymin+a->H+font*divi->marime/2;
    divi->centery=ceny;
}
void divi(nod* divi)
{
    for(int i=-1; i<=1; i++)
    {
        if(divi->marime!=1)
            i++;
        line (divi->xmin,divi->centery+i,divi->xmax,divi->centery+i);
        if(divi->marime!=1)
            break;
    }
}
void putdim(nod *&p)
{
    nod *a=p->st;
    nod *b=p->dr;
    if(a->info[0]!='^' && a->tip)
        paradim(a);
    change_font(p->marime,font);
    p->L=a->L+b->L+font*p->marime/3;
    cout<<a->info<<" "<<a->H<<" "<<b->info<<" "<<b->H<<" "<<p->marime<<'\n';
    p->H=a->H-font*p->marime/4+b->H;
    int aux=a->ymin;
    a->xmin=0;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*p->marime/3;
    b->xmax=b->xmin+b->L;
    b->ymin=0;
    b->ymax=b->ymin+b->H;
    a->ymin=b->ymax-font*p->marime/4;
    a->ymax=a->ymin+a->H;
    p->ymin=0;
    p->ymax=p->ymin+p->H;
    p->xmin=0;
    p->xmax=p->xmin+p->L;
    a->centery+=a->ymin-aux;
    p->centery=a->centery;
}
void putere(nod*&p)
{
    nod*a=p->st;
    if(a->info[0]!='^' && a->tip)
        parantezare(a);
}
void logdim(nod*&f)
{
    nod*a=f->st;
    nod*b=f->dr;
    paradim(b);
    change_font(f->marime,font);
    f->H=b->centery-b->ymin+max(b->ymax-b->centery,int(b->centery+textheight("log")/2+font*f->marime/10+a->H));
    f->L=b->L+textwidth("log")+a->L+font*f->marime/2;
    a->xmin=0+textwidth("log");
    int aux=a->ymin;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*f->marime/2;
    b->xmax=b->xmin+b->L;
    b->ymin=0;
    b->ymax=b->ymin+b->H;
    a->ymin=b->centery+textheight("log")/2+font*f->marime/10;
    a->ymax=a->ymin+a->H;
    a->centery+=a->ymin-aux;
    f->xmin=0;
    f->xmax+=f->L;
    f->centery=b->centery;
}
void log(nod*&f)
{
    change_font(f->marime,font);
    outtextxy(f->xmin,f->centery-textheight("log")/2,"log");
    parantezare(f->dr);
}
void functionsdim(nod*& f)
{
    nod*a=f->st;
    f->H=a->H;
    paradim(a);
    change_font(f->marime,font);
    f->L=a->L+textwidth(f->info);
    a->xmin=0+textwidth(f->info);
    a->xmax=a->xmin+a->L;
    a->ymin=0;
    a->ymax=a->ymin+a->H;
    f->xmin=0;
    f->xmax=f->xmin+f->L;
    f->ymin=0;
    f->ymax=f->ymin+f->H;
    f->centery=a->centery;
}
void functions(nod* f)
{
    change_font(f->marime,font);
    int centerx,centery;
    int w=textwidth(f->info);
    int h=textheight(f->info);
    centerx=(f->xmin+f->st->xmin)/2;
    centery=f->centery=f->st->centery;
    outtextxy(centerx-w/2,centery-h/2,f->info);
    parantezare(f->st);
}
void adundim (nod *&suma)
{
    nod*a=suma->st;
    nod*b=suma->dr;
    if(b->info[0]=='-' && b->tip==1)
        paradim(b);
    suma->H=max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery);
    suma->L=a->L+b->L+font*suma->marime;
    a->xmin=0;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*suma->marime;
    b->xmax=b->xmin+b->L;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    suma->centery=a->centery;
    suma->ymin=0;
    suma->ymax=suma->ymin+suma->H;
}
void adunare (nod* suma)
{
    nod*a=suma->st;
    nod*b=suma->dr;
    change_font(font,suma->marime);
    outtextxy((a->xmax+b->xmin)/2-textwidth("+")/2,suma->centery-textheight("+")/2,"+");
    if(b->info[0]=='-' && b->tip==1)
        parantezare(b);
}
void scadim (nod *&dif)
{
    nod*a=dif->st;
    nod*b=dif->dr;
    if(b->info[0]=='-' or b->info[0]=='+')
        paradim(b);
    dif->H=max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery);
    dif->L=a->L+b->L+font*dif->marime;
    a->xmin=0;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*dif->marime;
    b->xmax=b->xmin+b->L;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    dif->centery=a->centery;
    dif->ymin=0;
    dif->ymax=dif->ymin+dif->H;
}
void scadere (nod* &dif)
{
    nod*a=dif->st;
    nod*b=dif->dr;
    change_font(font,dif->marime);
    outtextxy((a->xmax+b->xmin)/2-textwidth("-")/2,dif->centery-textheight("-")/2,"-");
    if(b->info[0]=='-' or b->info[0]=='+')
        parantezare(b);
}
void muldim (nod *&mul)
{
    nod*a=mul->st;
    nod*b=mul->dr;
    if(b->info[0]=='-' or b->info[0]=='+')
        paradim(b);
    if(a->info[0]=='-' or a->info[0]=='+')
        paradim(a);
    mul->H=max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery);
    mul->L=a->L+b->L+font*mul->marime;
    a->xmin=0;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*mul->marime;
    b->xmax=b->xmin+b->L;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    mul->centery=a->centery;
    mul->ymin=0;
    mul->ymax=mul->ymin+mul->H;
}
void inmultire (nod *& mul)
{
    nod*a=mul->st;
    nod*b=mul->dr;
    double cenx=a->xmax+font*mul->marime/2;
    fillellipse(cenx,mul->centery,min(5,int(font*mul->marime/8)),min(5,int(font*mul->marime/8)));
    if(a->info[0]=='-' or a->info[0]=='+')
        parantezare(a);
    if(b->info[0]=='-' or b->info[0]=='+')
        parantezare(b);
}
void impartiredim(nod *&div)
{
    nod*a=div->st;
    nod*b=div->dr;
    if(b->info[0]=='-' or b->info[0]=='+')
        paradim(b);
    if(a->info[0]=='-' or a->info[0]=='+')
        paradim(a);
    div->H=max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery);
    div->L=a->L+b->L+font*div->marime;
    a->xmin=0;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*div->marime;
    b->xmax=b->xmin+b->L;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    div->centery=a->centery;
    div->ymin=0;
    div->ymax=div->ymin+div->H;
}
void impartire (nod *& div)
{
    nod*a=div->st;
    nod*b=div->dr;
    double cenx=a->xmax+font*div->marime/2;
    fillellipse(cenx,div->centery+font*div->marime/4,max(1,int(font*div->marime/12)),max(1,int(font*div->marime/12)));
    fillellipse(cenx,div->centery-font*div->marime/4,max(1,int(font*div->marime/12)),max(1,int(font*div->marime/12)));
    if(a->info[0]=='-' or a->info[0]=='+')
        parantezare(a);
    if(b->info[0]=='-' or b->info[0]=='+')
        parantezare(b);
}
void sqdim(nod *&p)
{
    nod *a=p->st;
    p->H=a->H+font*p->marime/5;
    p->L=a->L+font*7/10;
    a->ymin=font*p->marime/5;
    a->ymax=a->ymin+a->H;
    a->xmin=font*p->marime*7/10;
    a->xmax=a->xmin+a->L;
    p->ymin=0;
    p->xmin=0;
    p->ymax=p->H;
    p->xmax=p->L;
    a->centery+=font*p->marime/5;
    p->centery=a->centery;
}
void sqrtafis(nod *&p)
{
    nod *a=p->st;
    line(a->xmin-font*p->marime/7,p->ymin,a->xmax+font*p->marime/10,p->ymin);
    line(a->xmin-font*p->marime/3,a->ymax-font*p->marime/8,a->xmin-font*p->marime/7,p->ymin);
    line(a->xmin-font*p->marime/3,a->ymax-font*p->marime/8,a->xmin-font*p->marime/2,p->centery);
    line(a->xmin-font*p->marime/2,p->centery,a->xmin-font*p->marime/2-font*p->marime/10,p->centery+font*p->marime/10);
}
void radim(nod *&p)
{
    nod *b=p->dr;
    nod*a=p->st;
    b->centery+=font*p->marime/5;
    p->centery=b->centery;
    p->H=max(b->H+font*p->marime/5,a->H+min(font*p->marime,b->ymax-b->centery*1.0));
    p->L=a->L+b->L+font*p->marime*7/10+font*p->marime/20;
    b->ymin=font*p->marime/5;
    b->ymax=b->ymin+b->H;
    b->xmin=font*p->marime*7/10+a->L+font*p->marime/20;
    b->xmax=b->xmin+b->L;
    p->ymin=0;
    p->xmin=0;
    p->ymax=p->H;
    p->xmax=p->L;
    if(b->H+font*p->marime/5<=a->H+min(font*p->marime,b->ymax-b->centery*1.0))
    {
        int aux=b->ymin;
        b->ymin+=a->H+min(font*p->marime,b->ymax-b->centery*1.0)-b->H-font*p->marime/5;
        b->centery+=b->ymin-aux;
        b->ymax=b->ymin+b->H;
        p->ymax=b->ymax;
        p->centery=b->centery;
        p->ymin=p->ymax-p->H;
    }
    else
    {
        a->ymin=-(a->H+min(font*p->marime,b->ymax-b->centery*1.0)-b->H-font*p->marime/5);
        a->centery+=a->ymin;
        a->ymax=a->ymin+a->H;
    }
}
void radafis(nod *&p)
{
    change_font(font,p->marime);
    nod *b=p->dr;
    line(b->xmin-font*p->marime/7,b->ymin-font*p->marime/5,b->xmax+font*p->marime/10,b->ymin-font*p->marime/5);
    line(b->xmin-font*p->marime/3,b->ymax-font*p->marime/8,b->xmin-font*p->marime/7,b->ymin-font*p->marime/5);
    line(b->xmin-font*p->marime/3,b->ymax-font*p->marime/8,b->xmin-font*p->marime/2,max(p->centery*1.0,p->ymax-font*p->marime));
    line(b->xmin-font*p->marime/2,max(p->centery*1.0,p->ymax-font*p->marime),b->xmin-font*p->marime/2-font*p->marime/10-font*p->marime/10,max(p->centery*1.0,p->ymax-font*p->marime)+font*p->marime/10);
    // dreptunghi(p);
}
void sumdim(nod*p)
{
    nod*a=p->st->st;
    nod*b=p->st->dr;
    nod*c=p->dr->st;
    nod*d=p->dr->dr;
    cout<<a->info<<" "<<b->info<<" "<<c->info<<" "<<d->info<<'\n';
    p->L=max(a->L+font/4,c->L/2)+max(b->L+font/4,c->L/2)+d->L;
    p->H=max(int(font*p->marime+c->H),d->centery-d->ymin)+max(int(font*p->marime)+max(a->centery-a->ymin,b->centery-b->ymin)+max(b->ymax-b->centery,a->ymax-a->centery),d->ymax-d->centery);
    p->xmin=0;
    p->xmax=p->L;
    p->ymin=0;
    p->ymax=p->H;
    if(a->L+font/4>=c->L/2)
    {
        a->xmin=0;
        a->xmax=a->L;
        b->xmin=a->xmax+font/2;
        b->xmax=b->xmin+b->L;
        c->xmin=(a->xmax+b->xmin)/2-c->L/2;
        c->xmax=c->xmin+c->L;
        d->xmin=max(b->xmax,c->xmax);
        d->xmax=d->xmin+d->L;
    }
    else
    {
        c->xmin=0;
        c->xmax=c->L;
        a->xmax=(c->xmin+c->xmax)/2-font/4;
        a->xmin=a->xmax-a->L;
        b->xmin=a->xmax+font/2;
        b->xmax=b->xmin+b->L;
        d->xmin=max(b->xmax,c->xmax);
        d->xmax=d->xmin+d->L;
    }
    if(int(font*p->marime+c->H)>d->centery-d->ymin)
    {
        c->ymin=0;
        c->ymax=c->H;
        int aux=d->centery;
        d->centery=c->ymax+font*p->marime;
        d->ymin+=d->centery-aux;
        d->ymax=d->ymin+d->H;
        if(a->centery-a->ymin>=b->centery-b->ymin)
        {
            a->ymin=0;
            b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
            b->centery=a->centery;
        }
        else
        {
            b->ymin=0;
            a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
            a->centery=b->centery;
        }
        a->ymin+=2*font*p->marime+c->H;
        a->ymax=a->ymin+a->H;
        a->centery+=2*font*p->marime+c->H;
        b->ymin+=2*font*p->marime+c->H;
        b->ymax=b->ymin+b->H;
        b->centery+=2*font*p->marime+c->H;
    }
    else
    {
        d->ymin=0;
        d->ymax=d->ymin+d->H;
        c->ymax=d->centery-font*p->marime;
        c->ymin=c->ymax-c->H;
        c->centery+=c->ymin;
        if(a->centery-a->ymin>=b->centery-b->ymin)
        {
            a->ymin=0;
            b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
            b->centery=a->centery;
        }
        else
        {
            b->ymin=0;
            a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
            a->centery=b->centery;
        }
        a->ymin+=d->centery+font*p->marime;
        a->ymax=a->ymin+a->H;
        a->centery+=a->ymin;
        b->ymin+=d->centery+font*p->marime;
        b->ymax=b->ymin+a->H;
        b->centery+=b->ymin;
    }
    p->centery=d->centery;
}
void suma(nod* p)
{
    nod*a=p->st->st;
    nod*b=p->st->dr;
    nod*c=p->dr->st;
    nod*d=p->dr->dr;
    line(((c->xmin+c->xmax)-font*p->marime)/2,c->ymax+font*p->marime/10,((c->xmin+c->xmax)+font*p->marime)/2,c->ymax+font*p->marime/10);//partea de sus a sumei
    line(((c->xmin+c->xmax)-font*p->marime)/2,min(a->ymin,b->ymin)-font*p->marime/10,((c->xmin+c->xmax)+font*p->marime)/2,min(a->ymin,b->ymin)-font*p->marime/10);//partea de jos a sumei
    line(((c->xmin+c->xmax)-font*p->marime)/2,c->ymax+font*p->marime/10,((c->xmin+c->xmax))/2,c->ymax+font*p->marime);
    line(((c->xmin+c->xmax)-font*p->marime)/2,min(a->ymin,b->ymin)-font*p->marime/10,((c->xmin+c->xmax))/2,min(a->ymin,b->ymin)-font*p->marime);
    line(a->xmax+font/10,a->centery+font/20,b->xmin-font/10,a->centery+font/20);
    line(a->xmax+font/10,a->centery-font/20,b->xmin-font/10,a->centery-font/20);
    // dreptunghi(a);
}
void integraladefdim(nod*p)
{
    change_font(font,p->marime);
    nod*a=p->st->st;
    nod*b=p->st->dr;
    nod*c=p->dr->st;
    nod*d=p->dr->dr;
    p->L=font*p->marime/2+a->L+font*p->marime/2+b->L+textwidth("d");
    p->H=max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery)+font*p->marime;
    a->xmin=font*p->marime;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*p->marime/2+textwidth("d");
    b->xmax=b->xmin+b->L;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymin+=font*p->marime/2;
    b->ymin+=font*p->marime/2;
    a->centery+=font*p->marime/2;
    b->centery+=font*p->marime/2;
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    p->centery=a->centery;
    p->ymin=0;
    p->ymax=p->ymin+p->H;

    //
    p->L+=max(c->L,d->L);
    p->H+=c->H+d->H;
    p->ymax=p->H;
    a->ymin+=c->H;
    a->ymax=a->ymin+a->H;
    a->centery+=c->H;
    p->centery=a->centery;
    b->ymin+=c->H;
    b->ymax=b->ymin+b->H;
    b->centery+=c->H;
    c->ymin=0;
    c->ymax=c->H;
    d->ymax=p->H;
    d->ymin=p->H-d->H;
    d->centery+=d->ymin;
    c->xmin+=font*p->marime/6;
    c->xmax=c->xmin+c->L;
    d->xmin-=font*p->marime/4;
    d->xmax=d->xmin+d->L;
    a->xmin+=max(c->L,d->L);
    b->xmin+=max(c->L,d->L);
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+textwidth("d")+font*p->marime/2;
    b->xmax=b->xmin+b->L;
}
void integraladef(nod*p){
    //dreptunghi(p);
    change_font(font,p->marime);
    nod*a=p->st->st;
    nod*b=p->st->dr;
    nod*c=p->dr->st;
    nod*d=p->dr->dr;
    line(p->xmin,c->ymax+font*p->marime/4,p->xmin,d->ymin-font*p->marime/4);
    arc(p->xmin+font*p->marime/4,c->ymax+font*p->marime/4,0,180,font*p->marime/4);
    arc(p->xmin-font*p->marime/4,d->ymin-font*p->marime/4,180,0,font*p->marime/4);
    outtextxy(b->xmin-textwidth("d"),b->centery-textheight("d")/2,"d");
}
void integraladim(nod*p)
{
    change_font(font,p->marime);
    nod*a=p->st;
    nod*b=p->dr;
    p->L=font*p->marime/2+a->L+font*p->marime/2+b->L+textwidth("d");
    p->H=max(a->centery-a->ymin,b->centery-b->ymin)+max(a->ymax-a->centery,b->ymax-b->centery)+font*p->marime;
    a->xmin=font*p->marime;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*p->marime/2+textwidth("d");
    b->xmax=b->xmin+b->L;
    if(a->centery-a->ymin>=b->centery-b->ymin)
    {
        a->ymin=0;
        b->ymin=(a->centery-a->ymin)-(b->centery-b->ymin);
        b->centery=a->centery;
    }
    else
    {
        b->ymin=0;
        a->ymin=(b->centery-b->ymin)-(a->centery-a->ymin);
        a->centery=b->centery;
    }
    a->ymin+=font*p->marime/2;
    b->ymin+=font*p->marime/2;
    a->centery+=font*p->marime/2;
    b->centery+=font*p->marime/2;
    a->ymax=a->ymin+a->H;
    b->ymax=b->ymin+b->H;
    p->centery=a->centery;
    p->ymin=0;
    p->ymax=p->ymin+p->H;

}
void integrala(nod*p){
    //dreptunghi(p);
    change_font(font,p->marime);
    line(p->xmin+font*p->marime/4,p->ymin,p->xmin,p->ymax);
    arc(p->xmin+font*p->marime/2,p->ymin,0,180,font*p->marime/4);
    arc(p->xmin-font*p->marime/4,p->ymax,180,0,font*p->marime/4);
    nod*d=p->dr;
    outtextxy(d->xmin-textwidth("d"),d->centery-textheight("d")/2,"d");
}
void update(nod* &r)
{
    cout<<"Centru lui"<< r->info<<" : "<<r->centery<<'\n';
    cout<<" Coordonatele : "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
    cout<<"Centru lui"<< r->st->info<<" : "<<r->st->centery<<'\n';
    cout<<" Coordonatele : "<<r->st->xmin<<" "<<r->st->ymin<<" "<<r->st->xmax<<" "<<r->st->ymax<<" "<<r->st->L<<" "<<r->st->H<<'\n';
    if(r->dr)
    {
        cout<<"Centru lui"<< r->dr->info<<" : "<<r->dr->centery<<'\n';
        cout<<" Coordonatele : "<<r->dr->xmin<<" "<<r->dr->ymin<<" "<<r->dr->xmax<<" "<<r->dr->ymax<<" "<<r->dr->L<<" "<<r->dr->H<<'\n';
    }
    r->st->xmin+=r->xmin;
    r->st->xmax+=r->xmin;
    r->st->ymin+=r->ymin;
    r->st->ymax+=r->ymin;
    r->st->centery+=r->ymin;
    if(r->dr!=NULL)
    {
        r->dr->xmin+=r->xmin;
        r->dr->xmax+=r->xmin;
        r->dr->ymin+=r->ymin;
        r->dr->ymax+=r->ymin;
        r->dr->centery+=r->ymin;
    }
    cout<<"Centru lui"<< r->info<<" : "<<r->centery<<'\n';
    cout<<" Coordonatele : "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
    cout<<"Centru lui"<< r->st->info<<" : "<<r->st->centery<<'\n';
    cout<<" Coordonatele : "<<r->st->xmin<<" "<<r->st->ymin<<" "<<r->st->xmax<<" "<<r->st->ymax<<" "<<r->st->L<<" "<<r->st->H<<'\n';
    if(r->dr)
    {
        cout<<"Centru lui"<< r->dr->info<<" : "<<r->dr->centery<<'\n';
        cout<<" Coordonatele : "<<r->dr->xmin<<" "<<r->dr->ymin<<" "<<r->dr->xmax<<" "<<r->dr->ymax<<" "<<r->dr->L<<" "<<r->dr->H<<'\n';
    }
    if(r->st->info[0]==',')
        update(r->st);
    if(r->dr!=NULL)
        if(r->dr->info[0]==',')
        {
            update(r->dr);
        }

}
void absdim(nod* p){
    nod*a=p->st;
    p->L=a->L+font*p->marime;
    p->H=a->H;
    a->xmin=font*p->marime/2;
    a->xmax=a->xmin+a->L;
    p->centery=a->centery;
}
void absafis(nod*p){
    line(p->xmin,p->ymin,p->xmin,p->ymax);
    line(p->xmin+1,p->ymin,p->xmin+1,p->ymax);
    line(p->xmax,p->ymin,p->xmax,p->ymax);
    line(p->xmax-1,p->ymin,p->xmax-1,p->ymax);
}
void preordine(nod*& r)
{
    if(r!=NULL)
    {
        if(r->tip==0)
        {
            baza(r);
            //cout<<"Centru lui"<< r->info<<" : "<<r->centery<<'\n';
            //cout<<" Coordonatele : "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
            return;
        }
        else
        {
            if(r->info[0]!=',')
                update(r);
            if(strcmp(r->info,"+")==0)
                adunare(r);
            else if(strcmp(r->info,"-")==0 && r->tip==2)
                scadere(r);
            else if(strcmp(r->info,"-")==0 && r->tip==1)
                negativ(r);
            else if(strcmp(r->info,"^")==0)
                putere(r);
            else if(strcmp(r->info,"/")==0)
                divi(r);
            else if(strcmp(r->info,"*")==0)
                inmultire(r);
            else if(strcmp(r->info,":")==0)
                impartire(r);
            else if(strcmp(r->info,"abs")==0)
                absafis(r);
            else if(strcmp(r->info,"log")==0)
                log(r);
            else if(strcmp(r->info,"lim")==0)
                limita(r);
            else if(strcmp(r->info,"sum")==0)
                suma(r);
            else if(strcmp(r->info,"sqrt")==0)
                sqrtafis(r);
            else if(strcmp(r->info,"rad")==0)
                radafis(r);
            else if(strcmp(r->info,"integraladef")==0)
                integraladef(r);
            else if(strcmp(r->info,"integrala")==0)
                integrala(r);
            else if(r->tip==1)
                functions(r);

            preordine(r->st);
            preordine(r->dr);
        }
    }
}
void postordine(nod*& r)
{
    if(r!=NULL)
    {
        if(r->tip==0)
        {
            bazadim(r);
            cout<<"Termenul "<<r->info<<" "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
            return;
        }
        if(strcmp(r->info,"log")==0)
        {
            //cout<<r->info<<" "<<r->st->info<<" "<<r->dr->info<<" "<<r->marime<<'\n';
            r->st->marime=max(0.5,r->marime/2);
            r->dr->marime=r->marime;
        }
        else if(r->info[0]=='^')
        {
            r->dr->marime=max(0.5,r->marime/2);
            r->st->marime=r->marime;
        }
        else if(strcmp(r->info,"lim")==0)
        {
            r->st->marime=0.5;

            r->st->st->marime=max(0.5,r->marime/2);
            r->st->dr->marime=max(0.5,r->marime/2);
            r->dr->marime=r->marime;
            cout<<r->st->st->info<<" "<<r->st->st->marime<<" ";
        }
        else if(strcmp(r->info,"sum")==0)
        {
            cout<<"SCHIMB MARIMEA"<<" "<<'\n';
            r->st->st->marime=0.5;
            r->st->dr->marime=0.5;
            r->dr->st->marime=0.5;
            r->dr->dr->marime=r->marime;
        }
        else if(strcmp(r->info,"integraladef")==0){
            r->st->st->marime=r->marime;
            r->st->dr->marime=r->marime;
            r->dr->st->marime=0.5;
            r->dr->dr->marime=0.5;
        }
        else if((strcmp(r->info,"rad")==0))
        {
            r->st->marime=max(0.25,r->marime*3/4);
            r->dr->marime=r->marime;
        }
        else if(r->info[0]!=',')
        {
            r->st->marime=r->marime;
            if(r->dr!=NULL)
                r->dr->marime=r->marime;
        }
        postordine(r->st);
        postordine(r->dr);
        if(strcmp(r->info,"+")==0)
            adundim(r);
        else if(strcmp(r->info,"-")==0 && r->tip==2)
            scadim(r);
        else if(strcmp(r->info,"-")==0 && r->tip==1)
            negadim(r);
        else if(strcmp(r->info,"^")==0)
            putdim(r);
        else if(strcmp(r->info,"/")==0)
            dividim(r);
        else if(strcmp(r->info,"*")==0)
            muldim(r);
        else if(strcmp(r->info,":")==0)
            impartiredim(r);
        else if(strcmp(r->info,"abs")==0)
            absdim(r);
        else if(strcmp(r->info,"lim")==0)
            limdim(r);
        else if(strcmp(r->info,"log")==0)
            logdim(r);
        else if(strcmp(r->info,"sum")==0)
            sumdim(r);
        else if(strcmp(r->info,"sqrt")==0)
            sqdim(r);
        else if(strcmp(r->info,"rad")==0)
            radim(r);
        else if(strcmp(r->info,"integraladef")==0)
            integraladefdim(r);
        else if(strcmp(r->info,"integrala")==0)
            integraladim(r);
        else if(r->tip==1)
            functionsdim(r);
        else cout<<" eugen ";
        cout<<"Termenul "<<r->info<<" "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
    }
}
bool parantezare(char* sir)
{
    stack<char> stiva;
    for (int i=0; i<strlen(sir); i++)
    {
        char ch=sir[i];
        if (ch == '(')
        {
            stiva.push(ch);
        }
        else if (ch == ')')
        {
            if (stiva.empty() || stiva.top() != '(')
            {
                return false;
            }
            stiva.pop();
        }
    }
    return stiva.empty();
}
void corectitudine(char*s)
{
    if(parantezare(s)==0)
    {
        cout<<"VERIFICA PARANTEZELE";
        failure();
        cleardevice();
        Inceput();
    }
}
void postfix(char *s)
{
    for(int i=0; i<strlen(s); i++)
    {
        cout<<i<<" ";
        o=0;
        char * m = new char[10];
        if(isdigit(s[i]))
        {
            v.push_back(numar(s+i));
            i+=strlen(numar(s+i))-1;
        }
        else if(s[i]=='-' && (i==0 or s[i-1]=='('))
        {
            st.push("ops");
        }
        else if(isoperator(s[i]))
        {
            if(strlen(s+i+1)==0 or i==0 or isoperator(s[i+1]) or isoperator(s[i-1]))
            {
                cout<<"ERROR";
                failure();
                cleardevice();
                Inceput();
            }
            m[0]=s[i];
            m[1]='\0';
            while(!st.empty() &&prioritate(st.top())>=prioritate(m))
            {
                v.push_back(st.top());
                st.pop();
            }
            st.push(m);
        }
        else if(s[i]=='(')
        {
            m[0]='(';
            m[1]='\0';
            st.push(m);
        }
        else if(s[i]==')')
        {
            while(!st.empty() && st.top()[0]!='(')
            {
                v.push_back(st.top());
                st.pop();
            }
            if(!st.empty())
                st.pop();
        }
        else if(isfunction(s+i))
        {
            st.push(isfunction(s+i));
            i+=strlen(isfunction(s+i))-1;
        }
        else if(s[i]>='a' && s[i]<='z' && (i+1==strlen(s) or s[i+1]==')' or isoperator(s[i+1])))
        {
            m[0]=s[i];
            m[1]='\0';
            v.push_back(m);
        }
        else if(s[i]=='i' && s[i+1]=='n' && s[i+2]=='f'&& (i+3==strlen(s) or s[i+3]==')' or isoperator(s[i+3])))
        {
            m[0]='i',m[1]='n',m[2]='f',m[3]='\0';
            i+=2;
            v.push_back(m);
        }
        else if(s[i]=='p' && s[i+1]=='i' && (i+2==strlen(s) or s[i+2]==')' or isoperator(s[i+2])))
        {
            m[0]='p',m[1]='i',m[2]='\0';
            i++;
            v.push_back(m);
        }
        else
        {
            cout<<"ERROR";
            failure();
            cleardevice();
            Inceput();
        }
    }
    while(st.empty()==0)
    {
        v.push_back(st.top());
        st.pop();
    }
}
void openwindow()
{
    int x=eymax;
    int y=getmaxheight();
    //initwindow(600,600);
    setbkcolor(0);
    cleardevice();
    font=30;
}
bool initrad(nod* rad)
{
    rad->xmin=400-rad->L/2;
    rad->ymin=400-rad->H/2;
    rad->xmax=rad->xmin+rad->L;
    rad->ymax=rad->ymin+rad->H;
    rad->centery+=rad->ymin;
    return rad->xmin>10 && rad->xmax<exmax-10 && rad->ymax<eymax-20 && rad->ymin>10 && rad->H<eymax-30 && rad->L<exmax-20;
}
void Inceput()
{
    setcolor(color1);
    setbkcolor(color2);
    bin.clear();
    bin.seekg(0, bin.beg);
    int xmij=exmax/2;
    buton b[5];
    bin.getline(ch,255);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    outtextxy(xmij-textwidth(ch)/2,30+textheight(ch)/2,ch);
    if (muzica)
        PlaySound(TEXT(musicname[0]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    else
        PlaySound(0,0,1);
    H=eymax/5;
    setusercharsize(1,1,1,1);
    for (int i=0; i<=4; i++)
    {
        bin.getline(ch,255);
        outtextxy(xmij-textwidth(ch)/2,H-textheight(ch)/2,ch);
        b[i].ymin=H-textheight(ch)/2-10;
        b[i].ymax=H+textheight(ch)/2+10;
        b[i].xmin=xmij-textwidth(ch)/2-10;
        b[i].xmax=xmij+textwidth(ch)/2+10;
        dreptunghi(b[i]);
        H+=eymax/12+textheight(ch);
    }
    bin.getline(ch,255);
    setusercharsize(1,2,1,2);
    outtextxy(exmax-30-textwidth(ch),eymax-30-textheight(ch),ch);
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))

        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if (atingebuton(x,y,b[0]))
            {
                cleardevice();
                starting();
            }
            if (atingebuton(x,y,b[1]))
            {
                cleardevice();
                informati();

            }
            if (atingebuton(x,y,b[2]))
            {
                cleardevice();
                settings();
            }
            if (atingebuton(x,y,b[3]))
            {
                cleardevice();
                if (muzica)
                    muzic[4]=1;
                credits();
            }
            if (atingebuton(x,y,b[4]))
            {
                cleardevice();
                ok=0;
                goodbye();
                exit(0);
            }
        }
    }
}
void informati ()
{
    fin.clear();
    fin.seekg(0,fin.beg);   //pentru a putea reciti files
    int ymij=eymax/2,xmij=exmax/2;
    if (muzica)
        PlaySound(TEXT(musicname[2]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);    //initializere fontsize
    setusercharsize(2,1,2,1);
    fin.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,30-textheight(ch)/2,ch);
    H=30+textheight(ch);
    setusercharsize(1,2,1,2);
    buton_info b[12];
    buton next[3];
    fin.getline(ch,200);
    next[0].xmin=50;
    next[0].xmax=60+textwidth(ch);
    next[0].ymin=H;
    next[0].ymax=H+eymax/80+textheight(ch);
    H+=eymax/40+textheight(ch);
    outtextxy(next[0].xmin+10,next[0].ymin+eymax/80,ch);
    dreptunghi(next[0]);
    for (int i=0;i<4;i++)
    {
        fin.getline(ch,200);
        b[i].xmin=50;
        b[i].xmax=70+textwidth(ch);
        b[i].ymin=H;
        b[i].ymax=H+eymax/80+textheight(ch);
        H+=eymax/30+textheight(ch);
        int cnt=0;
        while (ch[cnt]!=' ')
        {
            b[i].ch[cnt]=ch[cnt];
            cnt++;
        }
        b[i].ch[cnt]=NULL;
        outtextxy(b[i].xmin+10,b[i].ymin+eymax/80,ch);
        dreptunghi(b[i]);
    }
    fin.getline(ch,200);
    next[1].xmin=50;
    next[1].xmax=60+textwidth(ch);
    next[1].ymin=H;
    next[1].ymax=H+eymax/80+textheight(ch);
    outtextxy(next[1].xmin+10,next[1].ymin+eymax/80,ch);
    dreptunghi(next[1]);
    H+=eymax/30+textheight(ch);
    for (int i=4;i<11;i++)
    {
        fin.getline(ch,200);
        b[i].xmin=50;
        b[i].xmax=70+textwidth(ch);
        b[i].ymin=H;
        b[i].ymax=H+eymax/80+textheight(ch);
        H+=eymax/30+textheight(ch);
        int cnt=0;
        while (ch[cnt]!=' ')
        {
            b[i].ch[cnt]=ch[cnt];
            cnt++;
        }
        b[i].ch[cnt]=NULL;
        outtextxy(b[i].xmin+10,b[i].ymin+eymax/80,ch);
        dreptunghi(b[i]);
    }
    strcpy(b[9].ch,"integraladef(a,b,c,d)");
    cout<<b[9].ch<<'\n';
    setusercharsize(2,3,2,3);   //schimbare font size
    fin.getline(ch,200);
    outtextxy(20,eymax-20-textheight(ch),ch);
    fin.getline(ch,200);
    next[2].xmin=exmax-25-textwidth(ch);
    next[2].xmax=exmax-15;
    next[2].ymin=eymax-25-textheight(ch);
    next[2].ymax=eymax-15;
    outtextxy(exmax-20-textwidth(ch),eymax-20-textheight(ch),ch);
    dreptunghi(next[2]);
    ok=1;

    buton_info exp[3];
    H=eymax/4;
    for (int i=0;i<=2;i++)
    {
        char nume[20]="exemplu ";
        nume[8]=i+49;
        exp[i].xmin=eymax*3/4-10-textwidth(nume)/2;
        exp[i].xmax=eymax*3/4+10+textwidth(nume)/2;
        exp[i].ymin=H-textheight(nume)/2-10;
        exp[i].ymax=H+textheight(nume)/2+10;
        H+=eymax/6;
        dreptunghi(exp[i]);
        outtextxy(exp[i].xmin+10,exp[i].ymin+10,nume);
    }
    strcpy(exp[1].ch,"sum(x,0,lim(x,inf,log(log(log(2/3/4,3/4/5),6/(7/8)),sin((1/x/y):(pi/9)))),inf)");
    strcpy(exp[0].ch,"integraladef((arccos(ln(x)+(pi/4)^2))/(sin(2*k*pi+x)),x,rad(pi,cos(x)/sin(y)),2^((1/x+1/y)^2))");
    strcpy(exp[2].ch,"integrala((sin(x)^2-cos(x)^2)/(sin(x)^2cos(x)^2)ln((e^x+2/x)/(e^(4x)+x^3+2x^2+1/x^7)),x)");
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0;i<11;i++)
            {
                if (atingebuton(x,y,b[i]))
                {
                    cleardevice();
                    formula(b[i].ch);
                    setusercharsize(2,3,2,3);   //schimbare font size
                    strcpy(ch1,"Apasa orice tasta pentru revenii la Info");
                    outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
                    rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
                    getch();
                    cleardevice();
                    ok=0;
                    i=12;
                    informati();
                }

            }
            if (atingebuton(x,y,next[0]))
                info_functii();
            if (atingebuton(x,y,next[1]))
                info_operatori();
            if (atingebuton(x,y,next[2]))
            {
                    cleardevice();
                    ok=0;
                    Inceput();
            }
            for (int i=0;i<=2;i++)
                if (atingebuton(x,y,exp[i]))
                {
                    cleardevice();
                    formula(exp[i].ch);
                    setusercharsize(2,3,2,3);   //schimbare font size
                    strcpy(ch1,"Apasa orice tasta pentru revenii la Info");
                    outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
                    rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
                    getch();
                    cleardevice();
                    ok=0;
                    i=12;
                    informati();
                }
        }
    }
}
void info_functii()
{
    cleardevice();
    trig.clear();
    trig.seekg(0,trig.beg);
    settextstyle(1,0,0);    //initializere fontsize
    setusercharsize(1,1,1,1);
    trig.getline(ch,255);
    outtextxy(exmax/2-textwidth(ch)/2,30-textheight(ch)/2,ch);
    H=80+textheight(ch);
    setusercharsize(1,1,1,1);
    buton_info b[9];
    for (int i=0;i<=3;i++)
    {
        trig.getline(b[2*i].ch,200);
        b[2*i].xmin=exmax/4-textwidth(b[2*i].ch)/2-10;
        b[2*i].xmax=exmax/4+textwidth(b[2*i].ch)/2+10;
        b[2*i].ymin=H;
        b[2*i].ymax=H+20+textheight(b[i].ch);
        trig.getline(b[2*i+1].ch,200);
        b[2*i+1].xmin=exmax*3/4-textwidth(b[2*i+1].ch)/2-10;
        b[2*i+1].xmax=exmax*3/4+textwidth(b[2*i+1].ch)/2+10;
        b[2*i+1].ymin=H;
        b[2*i+1].ymax=H+20+textheight(b[2*i+1].ch);
        outtextxy(exmax/4-textwidth(b[2*i].ch)/2,H+10,b[2*i].ch);
        outtextxy(exmax*3/4-textwidth(b[2*i+1].ch)/2,H,b[2*i+1].ch);
        dreptunghi(b[2*i]);
        dreptunghi(b[2*i+1]);
        H+=eymax/8;
    }
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0;i<8;i++)
                if (atingebuton(x,y,b[i]))
                {
                    formula(b[i].ch);
                    setusercharsize(2,3,2,3);   //schimbare font size
                    strcpy(ch1,"Apasa orice tasta pentru revenii la Info");
                    outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
                    rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
                    getch();
                    cleardevice();
                    ok=0;
                    informati();
                }
        }
    }
}
void info_operatori()
{
    operatori.clear();
    operatori.seekg(0,operatori.beg);
    cleardevice();
    settextstyle(1,0,0);    //initializere fontsize
    setusercharsize(1,1,1,1);
    operatori.getline(ch,255);
    outtextxy(eymax/2-textwidth(ch)/2,30-textheight(ch)/2,ch);
    H=80+textheight(ch);
    setusercharsize(1,1,1,1);
    buton_info b[5];
    for (int i=0;i<=4;i++)
    {
        operatori.getline(b[i].ch,200);
        b[i].xmin=exmax/2-textwidth(b[i].ch)/2-10;
        b[i].xmax=exmax/2+textwidth(b[i].ch)/2+10;
        b[i].ymin=H;
        b[i].ymax=H+20+textheight(b[i].ch);
        outtextxy(exmax/2-textwidth(b[i].ch)/2,H+10,b[i].ch);
        dreptunghi(b[i]);
        H+=eymax/7;
    }
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0;i<5;i++)
                if (atingebuton(x,y,b[i]))
                {
                    formula(b[i].ch);
                    setusercharsize(2,3,2,3);   //schimbare font size
                    strcpy(ch1,"Apasa orice tasta pentru revenii la Info");
                    outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
                    rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
                    getch();
                    cleardevice();
                    ok=0;
                    informati();
                }
        }
    }

}
void turnondamusic(int cnt)
{
    muzic[cnt]=1;
    cleardevice();
    credits();
}
void credits()
{
    nothim.clear();
    nothim.seekg(0,nothim.beg);   //pentru a putea reciti files
    int ymij=eymax/2,xmij=exmax/2;
    settextstyle(1,0,0);    //initializere fontsize
    setusercharsize(2,1,2,1);
    nothim.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,30-textheight(ch)/2,ch);
    H=30+textheight(ch)/2+eymax/12;
    setusercharsize(1,2,1,2);
    int wig[14]={0},hig[14]={0};
    int cnt=0;
    int neko=0;
    buton b[10];
    while (nothim.getline(ch,255))
    {
        int h=textheight(ch);
        int l=textwidth(ch);
        setcolor(color1);
        setbkcolor(color2);
        b[cnt].xmin=xmij-l/2-5;
        b[cnt].xmax=xmij+l/2+5;
        b[cnt].ymin=H-h/2-5;
        b[cnt].ymax=H+5+h/2;
        if (muzic[cnt])
        {
            setcolor(color2);
            setbkcolor(color1);
            PlaySound(TEXT(musicname[cnt]),NULL,SND_FILENAME| SND_ASYNC | SND_LOOP);
            outtextxy(xmij-l/2,H-h/2,ch);
            dreptunghi(b[cnt]);
            neko=cnt;
            muzic[cnt]=0;
        }
        else
        {
            wig[2*cnt]=xmij-l/2-5,wig[2*cnt+1]=xmij+l/2+5,hig[2*cnt]=H-h/2-5,hig[2*cnt+1]=H+5+h/2;
            outtextxy(xmij-l/2,H-h/2,ch);
            dreptunghi(b[cnt]);
        }
        cnt++;
        H+=h+eymax/12;
    }
    strcpy(ch,"Inapoi la Meniu");
    buton bacc;
    bacc.xmin=exmax-25-textwidth(ch),bacc.xmax=exmax-15,bacc.ymin=eymax-25-textheight(ch),bacc.ymax=eymax-15;
    rectangle(exmax-25-textwidth(ch),eymax-25-textheight(ch),exmax-15,eymax-15);
    outtextxy(exmax-20-textwidth(ch),eymax-20-textheight(ch),ch);
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0;i<=4;i++)
            {
                if (atingebuton(x,y,b[i]))
                    turnondamusic(i);
            }
            if (atingebuton(x,y,b[5]))
            {
                setcolor(color2);
                setbkcolor(color1);
                outtextxy(b[5].xmin,b[5].ymin+5,"??????? - from Melty Blood (a video game)");
                dreptunghi(b[5]);
                PlaySound(TEXT("Secret.wav"),NULL,SND_FILENAME| SND_SYNC);
                setcolor(color1);
                setbkcolor(color2);

                outtextxy(b[5].xmin,b[5].ymin+5,"??????? - from Melty Blood (a video game)");
                dreptunghi(b[5]);
                PlaySound(TEXT(musicname[neko]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
            }
            if (atingebuton(x,y,bacc))
            {
                setcolor(color1);
                setbkcolor(color2);
                cleardevice();
                Inceput();
            }
        }
    }
}
void schimbareculoribk()
{
    him.clear();
    him.seekg(0, him.beg);
    int xmij=exmax/2;
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    strcpy(ch,"Alege o culoare");
    outtextxy(xmij-textwidth(ch)/2,30+textheight(ch),ch);
    H=100+textheight(ch);
    setusercharsize(1,2,1,2);
    int len=exmax/4;
    int cnt=-1;
    buton b[20];
    for (int i=1; i<=4; i++)
    {
        for (int j=1; j<=4; j++)
        {
            him.getline(ch,10);
            cnt++;
            setfillstyle(1,cnt);
            b[cnt].xmin=len*(j-1)+len/4;
            b[cnt].xmax=len*j-len/4;
            b[cnt].ymin=H;
            b[cnt].ymax=H+eymax/16;
            dreptunghi(b[cnt]);
            floodfill(len*(j-1)+len/2+1,H+1,color1);
            setbkcolor(cnt);
            outtextxy((b[cnt].xmin+b[cnt].xmax)/2,(b[cnt].ymin+b[cnt].ymax)/2-6,ch);
        }
        H+=eymax/8;
    }
    setbkcolor(color2);
    strcpy(ch,"Pentru background(nu alege la fel ca font)");
    outtextxy(xmij-textwidth(ch)/2,eymax-20,ch);
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0; i<=15 && ok; i++)
            {
                if (atingebuton(x,y,b[i]))
                    color2=i,ok=0;
            }
        }
    }
    cleardevice();
    if (color1==color2)
        failure();
    settings();
}
void schimbareculori()
{
    him.clear();
    him.seekg(0, him.beg);
    int xmij=exmax/2;
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    strcpy(ch,"Alege o culoare");
    outtextxy(xmij-textwidth(ch)/2,30+textheight(ch),ch);
    H=100+textheight(ch);
    setusercharsize(1,2,1,2);
    int len=exmax/4;
    int cnt=-1;
    buton b[20];
    for (int i=1; i<=4; i++)
    {
        for (int j=1; j<=4; j++)
        {
            him.getline(ch,10);
            cnt++;
            b[cnt].xmin=len*(j-1)+len/4;
            b[cnt].xmax=len*j-len/4;
            b[cnt].ymin=H;
            b[cnt].ymax=H+eymax/16;
            dreptunghi(b[cnt]);
            setcolor(cnt);
            outtextxy((b[cnt].xmin+b[cnt].xmax)/2,(b[cnt].ymin+b[cnt].ymax)/2-6,ch);
            setcolor(color1);
        }

        H+=eymax/8;
    }
    strcpy(ch,"Pentru font (nu alege la fel ca background)");
    outtextxy(xmij-textwidth(ch)/2,eymax-20,ch);
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0; i<=15 && ok; i++)
            {
                if (atingebuton(x,y,b[i]))
                    color1=i,ok=0;
            }
        }
    }
    cleardevice();
    if (color1==color2)
        failure();

    settings();
}
void baccToBasic()
{
    color1=15;
    color2=0;
    font=30;
    muzica=1;
    cleardevice();
    settings();
}
void alegerefont()
{
    setcolor(color1);
    setbkcolor(color2);
    cleardevice();
    finna.clear();
    finna.seekg(0,finna.beg);
    buton b[6];
    int H=10;
    int xmij=exmax/2;
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    finna.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,H+textheight(ch),ch);
    H+=eymax/10;
    for (int i=0;i<=5;i++)
    {
        setcolor(color1);
        setbkcolor(color2);
        finna.getline(ch,255);
        if (font==fontsize[i])
        {
             setcolor(color2);
             setbkcolor(color1);
        }
        b[i].xmin=xmij-textwidth(ch)/2-10;
        b[i].xmax=xmij+textwidth(ch)/2+10;
        b[i].ymin=H;
        b[i].ymax=H+textheight(ch)+10;
        dreptunghi(b[i]);
        outtextxy(b[i].xmin+10,b[i].ymin+10,ch);
        H+=textheight(ch)+eymax/14;
    }
    setcolor(color1);
    setbkcolor(color2);
    setusercharsize(1,2,1,2);
    finna.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,eymax-20,ch);
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for (int i=0;i<=5;i++)
            {
                if (atingebuton(x,y,b[i]))
                {
                    font=fontsize[i];
                    cleardevice();
                    settings();
                    ok=0;
                }
            }
        }
    }
    ok=1;
}
void settings()
{
    setcolor(color1);
    setbkcolor(color2);
    cleardevice();
    gin.clear();
    gin.seekg(0,gin.beg);
    int xmij=exmax/2;
    if (muzica && ok)
        PlaySound(TEXT(musicname[3]),NULL,SND_FILENAME| SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    gin.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,20,ch);
    H=eymax/16+textheight(ch);
    buton b[7];
    for (int i=0; i<=5; i++)
    {
        gin.getline(ch,255);
        b[i].xmin=xmij-textwidth(ch)/2-10;
        b[i].xmax=xmij+textwidth(ch)/2+10;
        b[i].ymin=H;
        b[i].ymax=H+textheight(ch)+10;
        H+=textheight(ch)+eymax/14;
        outtextxy(b[i].xmin+10,b[i].ymin+10,ch);
        dreptunghi(b[i]);
    }
    if (!muzica)
    {
        line(b[2].xmin,b[2].ymin,b[2].xmax,b[2].ymax);
        line (b[2].xmin,b[2].ymax,b[2].xmax,b[2].ymin);
    }
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if (atingebuton(x,y,b[0]))
            {
                cleardevice();
                schimbareculoribk();
            }
            if (atingebuton(x,y,b[1]))
            {
                cleardevice();
                schimbareculori();
            }
            if (atingebuton(x,y,b[2]))
            {
                muzica=1-muzica;
                if (!muzica)
                    PlaySound(0,0,0);
                cleardevice();
                settings();
            }
            if (atingebuton(x,y,b[3]))
            {
                alegerefont();
                settings();
            }
            if (atingebuton(x,y,b[4]))
            {
                baccToBasic();
            }
            if (atingebuton(x,y,b[5]))
            {
                cleardevice();
                Inceput();
            }
        }
    }
}
void formula(char* formul){
    HFONT hFont=CreateFont(font,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
                HDC hDC=BGI__GetWinbgiDC();
                DeleteObject(SelectObject(hDC,hFont));
                BGI__ReleaseWinbgiDC(NULL);
                cleardevice();
                hFont=CreateFont(font,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
                hDC=BGI__GetWinbgiDC();
                DeleteObject(SelectObject(hDC,hFont));
                BGI__ReleaseWinbgiDC(NULL);
                corectitudine(formul);
                postfix(formul);
                //openwindow();
                arbbuild(rad);
                postordine(rad);
                if (initrad(rad))
                {
                    preordine(rad);
                    ok=0;
                }
                else
                    preamareformula(rad);
}
void preamareformula(nod *p)
{
    cleardevice();
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    if (p->H/font*10>eymax-30 || p->L/font*10>exmax-20)
    {
        ok=0;
        cleardevice();
        failure();
    }
    else
    {
        int fontposibil=min((exmax-20)*font/p->L,(eymax-30)*font/p->H);
        int cnt=0;
        finna.clear();
        finna.seekg(0,finna.beg);
        finna.getline(ch,255);
        int H=10;
        int xmij=exmax/2;
        strcpy(ch,"Font prea mare, poti schimba");
        outtextxy(xmij-textwidth(ch)/2,H+textheight(ch),ch);
        buton b[6];
        H+=eymax/16+textheight(ch);
        setusercharsize(1,1,1,1);
        while (fontposibil>=fontsize[cnt] && fontposibil<font)
        {
                finna.getline(ch,255);
                b[cnt].xmin=xmij-textwidth(ch)/2-10;
                b[cnt].xmax=xmij+textwidth(ch)/2+10;
                b[cnt].ymin=H;
                b[cnt].ymax=H+textheight(ch)+10;
                dreptunghi(b[cnt]);
                outtextxy(b[cnt].xmin+10,b[cnt].ymin+10,ch);
                H+=textheight(ch)+eymax/14;
                cnt++;
        }
        cnt--;
        ok=1;
        while (ok)
        {
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                int x=0,y=0;
                getmouseclick(WM_LBUTTONDOWN,x,y);
                for (int i=0;i<=cnt;i++)
                {
                    if (atingebuton(x,y,b[i]))
                    {
                        font=fontsize[i];
                        cleardevice();
                        formula(formul);
                        ok=0;
                    }
                }
            }
        }
        ok=1;
        setusercharsize(2,3,2,3);   //schimbare font size
        strcpy(ch1,"Apasa orice tasta pentru revenii la Meniu");
        outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
        rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
        getch();
        cleardevice();
        Inceput();
    }
}
void starting()
{
    setcolor(color1);
    setbkcolor(color2);
    din.clear();
    din.seekg(0,din.beg);
    int xmij=exmax/2;
    if (muzica)
        PlaySound(TEXT(musicname[1]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    din.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,300,ch);
    H=310+textheight(ch);
    setusercharsize(1,2,1,2);
    char lit=0;
    int cnt=0;
    for (int i=0;i<=200;i++)
        formul[i]=NULL;
    do
    {
        lit=getch();
        if (lit!=8 && lit!=13 && textwidth(formul)<=exmax-10)
        {
            setfillstyle(1,color2);
            bar(xmij-textwidth(formul)/2,H-textheight(formul)/2,xmij+textwidth(formul)/2,H+textheight(formul)/2);
            setcolor(color1);
            formul[cnt++]=lit;
            outtextxy(xmij-textwidth(formul)/2,H-textheight(formul)/2,formul);
        }
        if (lit==8 && cnt>=0)
        {
            setfillstyle(1,color2);
            bar(0,H-textheight(formul)/2,exmax,H+textheight(formul)/2);
            setcolor(color1);
            if (cnt>0)
            {
                formul[cnt-1]=NULL;
                cnt--;
                outtextxy(xmij-textwidth(formul)/2,H-textheight(formul)/2,formul);
            }

        }
    }
    while (lit!=13);
    H+=textheight(formul)+10;
    setusercharsize(1,1,1,1);
    din.getline(ch2,200);
    outtextxy(xmij-textwidth(ch2)/2,H,ch2);
    H+=textheight(ch2)+10;
    char a[3]="Da",b[3]="Nu";
    int wig[4]= {0};
    wig[0]=xmij/2-10-textwidth(a)/2;
    wig[1]=xmij/2+10+textwidth(a);
    wig[2]=3*xmij/2-10-textwidth(b);
    wig[3]=3*xmij/2+10+textwidth(b);
    outtextxy(wig[0]+10,H,a),outtextxy(wig[2]+10,H,b);
    rectangle(wig[0],H,wig[1],H+textheight(a)+10),rectangle(wig[2],H,wig[3],H+textheight(b)+10);
    while (ok)
    {
        ismouseclick(WM_LBUTTONDOWN);
        int x=0,y=0;
        getmouseclick(WM_LBUTTONDOWN,x,y);
        if (y>=H)
        {
            if (x>=wig[0] && x<=wig[1] && y<+H+textheight(a))
            {
                formula(formul);
            }
            if (x>=wig[2] && x<=wig[3] && y<+H+textheight(b))
            {
                cleardevice();
                failure();
            }
        }
    }
    ok=1;
    setusercharsize(2,3,2,3);   //schimbare font size
    strcpy(ch1,"Apasa orice tasta pentru revenii la Meniu");
    outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
    rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
    getch();
    cleardevice();
    Inceput();
}
void goodbye()
{
    int xmij=exmax/2,ymij=eymax/2;
    char bye[15]="SEE YOU LATER";
    settextstyle(1,0,0);
    setusercharsize(3,2,3,2);
    outtextxy(xmij-textwidth(bye)/2,ymij-textheight(bye)/2,bye);
    rectangle(xmij-10-textwidth(bye)/2,ymij-10-textheight(bye)/2,xmij+10+textwidth(bye)/2,ymij+10+textheight(bye)/2);
    if (muzica)
    {
        bool playa=PlaySound(TEXT("Bye.wav"),NULL,SND_FILENAME | SND_SYNC);
        return;
    }
    else
    {
        delay(2000);
        return;
    }
}
void failure()
{
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    outtextxy(exmax/2-textwidth("EROARE, ai facut ceva gresit")/2,100-textheight("EROARE, ai facut ceva gresit"),"EROARE, ai facut ceva gresit");
    ok=0;
    setusercharsize(2,3,2,3);   //schimbare font size
    strcpy(ch1,"Apasa orice tasta pentru revenii la Meniu");
    outtextxy(exmax-20-textwidth(ch1),eymax-20-textheight(ch1),ch1);
    rectangle(exmax-25-textwidth(ch1),eymax-25-textheight(ch1),exmax-15,eymax-15);
    getch();
    if (color1==color2)
    {
        readimagefile("Secret.jpg",300,300,600,600);
        color1=0,color2=15;
        if (muzica)
            bool playa=PlaySound(TEXT("Secret.wav"),NULL,SND_FILENAME | SND_SYNC);
        else
            delay(2000);
    }
    else
    {
        delay(1000);
    }

}
int main()
{
    initwindow(exmax,eymax,"Vizualizator de formule");
    Inceput();
    closegraph();
    bin.close(),fin.close(),din.close();
    gin.close();nothim.close();trig.close();him.close();operatori.close();finna.close();

}

