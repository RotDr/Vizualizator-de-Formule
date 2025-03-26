#include <iostream>
#include<queue>
#include<cstring>
#include <stack>
#include<vector>
#include<graphics.h>
#include<windows.h>
using namespace std;
HDC BGI__GetWinbgiDC(HWND hWnd=NULL);
void BGI__ReleaseWinbgiDC(HWND hWnd=NULL);
int n,i,j,k;
bool o,calc;
vector<char*>v;
char s[2001];
stack<char*>st;
double font;
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
    return ch=='+' or ch=='-' or ch=='*' or ch=='/' or ch=='^' or ch==',';
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
    while(j < strlen(l) && (isdigit(l[j]) or l[j]=='.'))
        j++;
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
} rad;
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

nod* arbbuild()
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
    return stc.top();
}

void dreptunghi (nod * p)
{
    line (p->xmin,p->ymin,p->xmin,p->ymax);
    line (p->xmin,p->ymin,p->xmax,p->ymin);
    line (p->xmax,p->ymax,p->xmax,p->ymin);
    line (p->xmax,p->ymax,p->xmin,p->ymax);
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
    p->L+=2*font*p->marime;
}
void parantezare (nod *&p)
{
    HFONT hFont=CreateFont(p->H+font*p->marime,25*p->marime,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    HDC hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
    p->xmin+=font*p->marime,p->xmax-=font*p->marime;
    p->L-=2*font*p->marime;
    int centerx=(p->xmin*2-font)/2;
    int centery=(p->ymin+p->ymax)/2;
    outtextxy(centerx-textwidth("(")/2,centery-textheight("(")/2,"(");
    centerx=(p->xmax*2+font)/2;
    outtextxy(centerx-textwidth(")")/2,centery-textheight(")")/2,")");
    //dreptunghi(p);
    hFont=CreateFont(font*p->marime,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
};
void bazadim(nod*& b)
{
    b->H=textheight(b->info)*b->marime;
    b->L=textwidth(b->info)*b->marime;
    b->xmin=0;
    b->xmax=b->xmin+b->L;
    b->ymin=0;
    b->ymax=b->ymin+b->H;
    b->centery=(b->ymin+b->ymax)/2;
}
void drawPiSymbol(int x, int y, int w,int h) {
    // Draw two vertical lines and one horizontal line
    line(x+w/5, y+h/3, x+w/5, y +h*4/5); // Left vertical line
    line(x + w*4/5, y+h/3, x + w *4/5, y + h*4/5); // Right vertical line
    line(x, y+h/3, x + w, y+h/3); // Connecting horizontal line
}
void drawInfinitySymbol(int x, int y, int width, int height) {
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
    p->L=a->L+font*p->marime;
    p->xmin=0;
    p->xmax=p->xmin+p->L;
    a->xmin=p->xmin+font*p->marime;
    a->xmax=a->xmin+a->L;
    p->ymax=a->ymax;
    p->ymin=a->ymin;
    p->centery=a->centery;
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
    p->L=a->L+b->L+font*p->marime/3;
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
    f->H=b->centery-b->ymin+max(b->ymax-b->centery,int(b->centery+textheight("log")*f->marime/2+font*f->marime/10+a->H));
    f->L=b->L+textwidth("log")*f->marime+a->L+font*f->marime/2;
    a->xmin=0+textwidth("log")*f->marime;
    int aux=a->ymin;
    a->xmax=a->xmin+a->L;
    b->xmin=a->xmax+font*f->marime/2;
    b->xmax=b->xmin+b->L;
    b->ymin=0;
    b->ymax=b->ymin+b->H;
    a->ymin=b->centery+textheight("log")*f->marime/2+font*f->marime/10;
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
    font=font;
    paradim(a);
    f->L=a->L+textwidth(f->info)*f->marime;
    a->xmin=0+textwidth(f->info)*f->marime;
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
    int centerx,centery;
    int w=textwidth(f->info);
    int h=textheight(f->info);
    centerx=(f->xmin+f->st->xmin)/2;
    centery=f->centery=f->st->centery;
    change_font(f->marime,font);
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
            else if(r->tip==1)
                functions(r);
            else if(strcmp(r->info,"log")==0)
                log(r);

            preordine(r->st);
            preordine(r->dr);
        }
    }
}
/*void marime(nod*r){
    if(r->tip){
        cout<<r->info<<" ";
       if(strlen(r->info)>2 && r->info[2]=='g'){
        r->st->marime=r->marime/2;
        r->dr->marime=r->marime;
        }
       else if(r->info[0]=='^'){
        r->dr->marime=r->marime/2;
        r->st->marime=r->marime;
        }
        else{
            r->st->marime=r->marime;
            if(r->dr!=NULL)
            r->dr->marime=r->marime;
        }
        marime(r->st);
        if(r->dr!=NULL)
        marime(r->dr);
    }
}*/
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
        else if(r->tip==1)
            functionsdim(r);
        else if(strcmp(r->info,"log")==0)
            logdim(r);
        else cout<<" eugen ";
        cout<<"Termenul "<<r->info<<" "<<r->xmin<<" "<<r->ymin<<" "<<r->xmax<<" "<<r->ymax<<" "<<r->L<<" "<<r->H<<'\n';
    }
}
int main()
{
    //cout<<"Fontul dorit este: "<<'\n';
    //cin>>font;
    //cin.get();
    cin.getline(s,2001);
    for(int i=0; i<strlen(s); i++)
    {
        o=0;
        char * m = new char[10];
        if(isdigit(s[i]))
        {
            v.push_back(numar(s+i));
            i+=strlen(numar(s+i))-1;
            if(!(i+1==strlen(s) or isoperator(s[i+1]) or s[i+1]==')'))
            {
                cout<<"ERROR";
                return 0;
            }
        }
        else if(s[i]=='-' && (i==0 or s[i-1]=='('))
        {
            st.push("ops");
        }
        else if(isoperator(s[i]))
        {
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
        else if(s[i]=='i' && s[i+1]=='n' && s[i+2]=='f')
        {
            m[0]='i',m[1]='n',m[2]='f',m[3]='\0';
            i+=2;
            v.push_back(m);
        }
        else if(s[i]=='p' && s[i+1]=='i')
        {
            m[0]='p',m[1]='i',m[2]='\0';
            i++;
            v.push_back(m);
        }
        else
        {
            cout<<"ERROR";
            return 0;
        }
    }
    while(st.empty()==0)
    {
        v.push_back(st.top());
        st.pop();
    }
    initwindow(600,600);
    font=30;
    HFONT hFont=CreateFont(font,0,0,0,0,0,0,0,1,0,0,0,0,TEXT("Segoe UI Semibold"));
    HDC hDC=BGI__GetWinbgiDC();
    DeleteObject(SelectObject(hDC,hFont));
    BGI__ReleaseWinbgiDC(NULL);
    //for(int i=0; i<v.size(); i++)
    //cout<<v[i]<<" ";
    nod* rad=arbbuild();
    rad->marime=1.0;
    //cout<<rad->tip<<" ";
    postordine(rad);
    rad->xmin=300-rad->L/2;
    rad->ymin=300-rad->H/2;
    rad->xmax=rad->xmin+rad->L;
    rad->ymax=rad->ymin+rad->H;
    rad->centery+=rad->ymin;
    //line(rad->xmax/2,rad->ymax/2,300,300);
    //cout<<rad->xmin<<" "<<rad->ymin<<" "<<rad->xmax<<" "<<rad->ymax<<'\n';
    //rectangle(rad->xmin,rad->ymin,rad->xmax,rad->ymax);
    //parantezare(rad);
    setcolor(10);
    setbkcolor(7);
    preordine(rad);
    getch();


}
