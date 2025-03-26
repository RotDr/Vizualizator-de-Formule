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
char ch[400],ch1[200],ch2[200];
char c;
int H,b,i;
char music[6][15]={"Menu.wav","Starting.wav","Informatii.wav","setari.wav","Credits.wav"};
bool muzica=1;
bool muzic[6]={0},nomu=0;
int color1=15,color2=0;
int eymin,eymax=800,exmax=800,exmin;
bool ok=1;
void informati();
void settings();
void starting();
void goodbye();
void failure();
void credits();
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
        if(l[j]=='.') cnt++;
        if(cnt>1) return NULL;
        if(isdigit(l[j])==0)
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
                rad->st=stc.top();
                stc.pop();
            }
            else if(x==2 && stc.top()->info[0]!=',')
            {
                rad->dr=stc.top();
                stc.pop();
                rad->st=stc.top();
                stc.pop();
            }
            else if(x==2 && stc.top()->info[0]==',')
            {
                rad->dr=stc.top()->dr;
                rad->st=stc.top()->st;
                stc.pop();
            }
            rad->tip=x;
            stc.push(rad);
        }
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
    // Set color
    //setcolor(WHITE);

    // Calculate dimensions
    int radius = height/15;
    int verticalLineHeight = height-radius*2;

    // Left parenthesis
    for (int i = 0; i < thickness; i++)
    {
        // Upper curve
        arc(x+radius, y + radius, 90,180, radius);
        // Vertical line (adjusted to start at the end of the upper curve)
        line(x, y + radius, x, y + radius + verticalLineHeight);
        // Lower curve (adjusted to start at the end of the vertical line)
        arc(x+radius, y + radius + verticalLineHeight, 180,270, radius);
        x++;
        y++;
    }

    // Space between the parentheses

    // Right parenthesis
}
void drawRightParenthesis(int x, int y, int height, int thickness)
{
    int radius = height/15;
    int verticalLineHeight = height-radius*2;
    for (int i = 0; i < thickness; i++)
    {
        // Upper curve
        arc(x-radius, y + radius, 0, 90, radius);
        // Vertical line (adjusted to start at the end of the upper curve)
        line(x, y + radius, x, y + radius + verticalLineHeight);
        // Lower curve (adjusted to start at the end of the vertical line)
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
    HFONT hFont=CreateFont(p->H+font*p->marime,25*p->marime,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
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
    HFONT hFont=CreateFont(p->H,25*p->marime,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
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
    circle(center1,y*100/99,width/4);
    circle(center2,y*100/99,width/4);
}
void baza(nod* b)
{
    change_font(b->marime,font);
    double w=textwidth(b->info);
    double h=textheight(b->info);
    double centerx=(b->xmin+b->xmax)/2;
    if(b->info[0]=='p')
        drawPiSymbol(centerx-w/2,b->centery-h/2,w,h);
    else if(b->info[0]=='i')
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
    //p->L=max(textwidth("lim"),a->L+font*p->marime;
    //p->H=a->H+max(b->H,textheight("lim"))+font*p->marime/4;
    a->xmin=0;
    a->xmax=a->L;
    a->ymin=0;
    a->ymax=a->H;
    a->marime=p->marime/2;  // daca nu e corect asa, te rog sa modifici

    b->xmin=textwidth("lim")+font*p->marime;
    b->xmax=b->xmin+b->L;   // vezi ca aici este cazul de parantezare
    b->ymin=a->H+font*p->marime/4;
    b->ymax=b->ymin+b->H;

    p->xmin=0;
    p->xmax=p->L;
    p->ymin=0;
    p->ymax=p->H;
    p->centery=b->centery;
}
void limita(nod *p)
{
    outtextxy(p->xmin,p->dr->ymin,"lim");   // nu e testat inca
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
    a->ymin=b->centery+textheight("log")+font*f->marime/10;
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
            else if(r->tip==1)
                functions(r);
            else if(strcmp(r->info,"log")==0)
                log(r);

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
            //cout<<"Termenul "<<r->info<<" "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
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
        else
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
        else if(r->tip==1)
            functionsdim(r);
        else if(strcmp(r->info,"log")==0)
            logdim(r);
        else cout<<" eugen ";
        //cout<<"Termenul "<<r->info<<" "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
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
        exit(0);
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
                exit(0);
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
            exit (0);
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
    int x=getmaxwidth();
    int y=getmaxheight();
    initwindow(600,600);
    setbkcolor(7);
    cleardevice();
    font=30;
}
void initrad(nod* rad)
{
    rad->xmin=300-rad->L/2;
    rad->ymin=300-rad->H/2;
    rad->xmax=rad->xmin+rad->L;
    rad->ymax=rad->ymin+rad->H;
    rad->centery+=rad->ymin;
}
void Inceput()
{
    setcolor(color1);
    setbkcolor(color2);
    bin.clear();
    bin.seekg(0, bin.beg);
    int xmij=exmax/2;
    int hig[10]= {0};
    int wig[10]= {0};
    bin.getline(ch,255);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    outtextxy(xmij-textwidth(ch)/2,30+textheight(ch)/2,ch);
    if (muzica)
        PlaySound(TEXT(music[0]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    else
        PlaySound(0,0,1);
    H=eymax/5;
    setusercharsize(1,1,1,1);
    for (int i=0; i<=4; i++)
    {
        bin.getline(ch,255);
        outtextxy(xmij-textwidth(ch)/2,H-textheight(ch)/2,ch);
        rectangle(xmij-textwidth(ch)/2-10,H-textheight(ch)/2-10,xmij+textwidth(ch)/2+10,H+textheight(ch)/2+10);
        hig[2*i]=H-textheight(ch)/2-10;
        hig[2*i+1]=H+textheight(ch)/2+10;
        wig[2*i]=xmij-textwidth(ch)/2-10;
        wig[2*i+1]=xmij+textwidth(ch)/2+10;
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
            if (y>=hig[0] && y<=hig[1] && x>=wig[0] && x<=wig[1])
            {
                cleardevice();
                starting();
            }
            if (y>=hig[2] && y<=hig[3] && x>=wig[2] && x<=wig[3])
            {
                cleardevice();
                informati();

            }
            if (y>=hig[4] && y<=hig[5] && x>=wig[4] && x<=wig[5])
            {
                cleardevice();
                settings();
            }
            if (y>=hig[6] && y<=hig[7] && x>=wig[6] && x<=wig[7])
            {
                cleardevice();
                if (muzica)
                    muzic[4]=1;
                credits();
            }
            if (y>=hig[8] && y<=hig[9] && x>=wig[8] && x<=wig[9])
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
        PlaySound(TEXT(music[2]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);    //initializere fontsize
    setusercharsize(2,1,2,1);
    fin.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,30-textheight(ch)/2,ch);
    H=30+textheight(ch)/2;
    setusercharsize(1,2,1,2);
    while (fin.getline(ch,255))
    {
        int h=textheight(ch);
        int l=textwidth(ch);
        outtextxy(50,H,ch);
        H+=h+4;

    }
    setusercharsize(2,3,2,3);   //schimbare font size
    strcpy(ch,"Apasa orice tasta pentru revenii la Meniu");
    rectangle(exmax-25-textwidth(ch),eymax-25-textheight(ch),exmax-15,eymax-15);
    outtextxy(exmax-20-textwidth(ch),eymax-20-textheight(ch),ch);
    getch();
    cleardevice();
    Inceput();

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
    while (nothim.getline(ch,255))
    {
        int h=textheight(ch);
        int l=textwidth(ch);
        setcolor(color1);
        setbkcolor(color2);
        if (muzic[cnt])
        {
            setcolor(color2);
            setbkcolor(color1);
            PlaySound(TEXT(music[cnt]),NULL,SND_FILENAME| SND_ASYNC | SND_LOOP);
            wig[2*cnt]=xmij-l/2-5,wig[2*cnt+1]=xmij+l/2+5,hig[2*cnt]=H-h/2-5,hig[2*cnt+1]=H+5+h/2;
            outtextxy(xmij-l/2,H-h/2,ch);
            rectangle(wig[2*cnt],hig[2*cnt],wig[2*cnt+1],hig[2*cnt+1]);
            neko=cnt;
            muzic[cnt]=0;
        }
        else
        {
            wig[2*cnt]=xmij-l/2-5,wig[2*cnt+1]=xmij+l/2+5,hig[2*cnt]=H-h/2-5,hig[2*cnt+1]=H+5+h/2;
            outtextxy(xmij-l/2,H-h/2,ch);
            rectangle(wig[2*cnt],hig[2*cnt],wig[2*cnt+1],hig[2*cnt+1]);
        }
        cnt++;
        H+=h+eymax/12;
    }
    strcpy(ch,"Inapoi la Meniu");
    wig[12]=exmax-25-textwidth(ch),wig[13]=exmax-15,hig[12]=eymax-25-textheight(ch),hig[13]=eymax-15;
    rectangle(exmax-25-textwidth(ch),eymax-25-textheight(ch),exmax-15,eymax-15);
    outtextxy(exmax-20-textwidth(ch),eymax-20-textheight(ch),ch);
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if (x>=wig[0] && x<=wig[1] && y>=hig[0] && y<=hig[1])
            {
                    muzic[0]=1;
                    cleardevice();
                    credits();
            }
            if (x>=wig[2] && x<=wig[3] && y>=hig[2] && y<=hig[3])
            {
                    muzic[1]=1;
                    cleardevice();
                    credits();
            }
            if (x>=wig[4] && x<=wig[5] && y>=hig[4] && y<=hig[5])
            {
                    muzic[2]=1;
                    cleardevice();
                    credits();
            }
            if (x>=wig[6] && x<=wig[7] && y>=hig[6] && y<=hig[7])
            {
                    muzic[3]=1;
                    cleardevice();
                    credits();
            }
            if (x>=wig[8] && x<=wig[9] && y>=hig[8] && y<=hig[9])
            {
                    muzic[4]=1;
                    cleardevice();
                    credits();
            }
            if (x>=wig[10] && x<=wig[11] && y>=hig[10] && y<=hig[11])
            {
                setcolor(color2);
                setbkcolor(color1);
                outtextxy(wig[10]+5,hig[10]+5,"??????? - from Melty Blood (a video game)");
                rectangle(wig[10],hig[10],wig[11],hig[11]);
                PlaySound(TEXT("Secret.wav"),NULL,SND_FILENAME| SND_SYNC);
                setcolor(color1);
                setbkcolor(color2);

                outtextxy(wig[10]+5,hig[10]+5,"??????? - from Melty Blood (a video game)");
                rectangle(wig[10],hig[10],wig[11],hig[11]);
                PlaySound(TEXT(music[neko]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
            }
            if (x>=wig[12] && x<=wig[13] && y>=hig[12] && y<=hig[13])
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
    int hig[34]= {0},wig[34]= {0};
    for (int i=1; i<=4; i++)
    {
        for (int j=1; j<=4; j++)
        {
            him.getline(ch,10);
            cnt++;
            setfillstyle(1,cnt);
            wig[2*cnt]=len*(j-1)+len/4;
            wig[2*cnt+1]=len*j-len/4;
            hig[2*cnt]=H;
            hig[2*cnt+1]=H+60;
            rectangle(wig[2*cnt],hig[2*cnt],wig[2*cnt+1],hig[2*cnt+1]);
            floodfill(len*(j-1)+len/2+1,H+1,color1);
            setbkcolor(cnt);
            outtextxy((wig[2*cnt+1]+wig[2*cnt])/2,(hig[2*cnt+1]+hig[2*cnt])/2-6,ch);
        }
        H+=120;
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
                if (x>=wig[2*i] && x<=wig[2*i+1] && y>=hig[2*i] && y<=hig[2*i+1])
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
    int hig[34]= {0},wig[34]= {0};
    for (int i=1; i<=4; i++)
    {
        for (int j=1; j<=4; j++)
        {
            him.getline(ch,10);
            cnt++;
            wig[2*cnt]=len*(j-1)+len/4;
            wig[2*cnt+1]=len*j-len/4;
            hig[2*cnt]=H;
            hig[2*cnt+1]=H+60;
            rectangle(wig[2*cnt],hig[2*cnt],wig[2*cnt+1],hig[2*cnt+1]);
            setcolor(cnt);
            outtextxy((wig[2*cnt+1]+wig[2*cnt])/2,(hig[2*cnt+1]+hig[2*cnt])/2-6,ch);
            setcolor(color1);
        }

        H+=120;
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
                if (x>=wig[2*i] && x<=wig[2*i+1] && y>=hig[2*i] && y<=hig[2*i+1])
                    color1=i,ok=0;
            }
        }
    }
    cleardevice();
    if (color1==color2)
        failure();

    settings();
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
        PlaySound(TEXT(music[3]),NULL,SND_FILENAME| SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    gin.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,20,ch);
    H=60+textheight(ch);
    int wig[10]= {0},hig[10]= {0};
    for (int i=0; i<=3; i++)
    {
        gin.getline(ch,255);
        wig[2*i]=xmij-textwidth(ch)/2-10;
        wig[2*i+1]=xmij+textwidth(ch)/2+10;
        hig[2*i]=H;
        hig[2*i+1]=H+textheight(ch)+10;
        H+=textheight(ch)+100;
        outtextxy(wig[2*i]+10,hig[2*i]+10,ch);
        rectangle(wig[2*i],hig[2*i],wig[i*2+1],hig[2*i+1]);
    }
    if (!muzica)
    {
        line(wig[4],hig[4],wig[5],hig[5]);
        line (wig[4],hig[5],wig[5],hig[4]);
    }
    ok=1;
    while (ok)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x=0,y=0;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if (x>=wig[0] && x<=wig[1] && y>=hig[0] && y<=hig[1])
            {
                cleardevice();
                schimbareculoribk();
            }
            if (x>=wig[2] && x<=wig[3] && y>=hig[2] && y<=hig[3])
            {
                cleardevice();
                schimbareculori();
            }
            if (x>=wig[4] && x<=wig[5] && y>=hig[4] && y<=hig[5])
            {
                muzica=1-muzica;
                if (!muzica)
                    PlaySound(0,0,0);
                cleardevice();
                settings();
            }
            if (x>=wig[6] && x<=wig[7] && y>=hig[6] && y<=hig[7])
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
                initrad(rad);
                preordine(rad);
                ok=0;
}
void starting()
{
    setcolor(color1);
    setbkcolor(color2);
    din.clear();
    din.seekg(0,din.beg);
    int xmij=exmax/2;
    if (muzica)
        PlaySound(TEXT(music[1]),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    din.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,300,ch);
    H=310+textheight(ch);
    setusercharsize(1,2,1,2);
    char lit=0;
    char formul[400]="";
    int cnt=0;
    do
    {
        lit=getch();
        if (lit!=8 && lit!=13)
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
    readimagefile("Secret.jpg",300,300,600,600);
    if (muzica)
        bool playa=PlaySound(TEXT("Secret.wav"),NULL,SND_FILENAME | SND_SYNC);
    else
        delay(2000);
    ok=0;
    if (color1==color2)
        color1=0,color2=15;

}
int main()
{
    initwindow(exmax,eymax,"Vizualizator de formule");
    Inceput();
    closegraph();
    bin.close(),fin.close(),din.close();
    gin.close();

}

