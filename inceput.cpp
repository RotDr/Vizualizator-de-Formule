#include <iostream>
#include <fstream>
#include <winbgim.h>
#include <graphics.h>
#include <windows.h>
#include<mmsystem.h>
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
int H;
bool muzica=1;
int color1=15,color2=0;
int eymin,eymax=800,exmax=1500,exmin;
bool ok=1;
void informati();
void settings();
void starting();
void goodbye();
void failure();
void credits();
void Inceput()
{
    setcolor(color1);
    setbkcolor(color2);
    bin.clear();
    bin.seekg(0, bin.beg);
    int xmij=exmax/2;
    int hig[10]={0};
    int wig[10]={0};
    bin.getline(ch,255);
    settextstyle(1,0,0);
    setusercharsize(4,2,4,2);
    outtextxy(xmij-textwidth(ch)/2,30+textheight(ch)/2,ch);
    if (muzica)
        PlaySound(TEXT("Menu.wav"),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    H=250;
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
        H+=60+textheight(ch);
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
        PlaySound(TEXT("Informatii.wav"),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
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
    if (muzica)
        PlaySound(TEXT("Credits.wav"),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);    //initializere fontsize
    setusercharsize(1,1,1,1);
    nothim.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,30-textheight(ch)/2,ch);
    H=90+textheight(ch)/2;
    setusercharsize(2,3,2,3);
    while (nothim.getline(ch,255))
    {
        int h=textheight(ch);
        int l=textwidth(ch);
        outtextxy(xmij-l/2,H,ch);
        H+=h+15;

    }
    strcpy(ch,"Apasa orice tasta pentru revenii la Meniu");
    rectangle(exmax-25-textwidth(ch),eymax-25-textheight(ch),exmax-15,eymax-15);
    outtextxy(exmax-20-textwidth(ch),eymax-20-textheight(ch),ch);
    getch();
    cleardevice();
    Inceput();
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
    int hig[34]={0},wig[34]={0};
    for (int i=1;i<=4;i++)
    {
        for (int j=1;j<=4;j++)
        {
            him.getline(ch,10);
            cnt++;
            setfillstyle(1,cnt);
            wig[2*cnt]=len*(j-1)+len/4; wig[2*cnt+1]=len*j-len/4;
            hig[2*cnt]=H;hig[2*cnt+1]=H+60;
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
            for (int i=0;i<=15 && ok;i++)
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
    int hig[34]={0},wig[34]={0};
    for (int i=1;i<=4;i++)
    {
        for (int j=1;j<=4;j++)
        {
            him.getline(ch,10);
            cnt++;
            wig[2*cnt]=len*(j-1)+len/4; wig[2*cnt+1]=len*j-len/4;
            hig[2*cnt]=H;hig[2*cnt+1]=H+60;
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
            for (int i=0;i<=15 && ok;i++)
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
        PlaySound(TEXT("setari.wav"),NULL,SND_FILENAME| SND_ASYNC | SND_LOOP);
    settextstyle(1,0,0);
    setusercharsize(1,1,1,1);
    gin.getline(ch,255);
    outtextxy(xmij-textwidth(ch)/2,20,ch);
    H=60+textheight(ch);
    int wig[10]={0},hig[10]={0};
    for (int i=0;i<=3;i++)
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
void starting()
{
        setcolor(color1);
        setbkcolor(color2);
        din.clear();
        din.seekg(0,din.beg);
        int xmij=exmax/2;
        if (muzica)
            PlaySound(TEXT("Starting.wav"),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
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
                formul[cnt]=NULL;
                if (cnt>0)
                {
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
        int wig[4]={0};
        wig[0]=xmij/2-10-textwidth(a)/2;wig[1]=xmij/2+10+textwidth(a);wig[2]=3*xmij/2-10-textwidth(b);wig[3]=3*xmij/2+10+textwidth(b);
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
                    cleardevice();

                    ok=0;
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
    initwindow(exmax,eymax,"Editor de formule");
    Inceput();
    closegraph();
    bin.close(),fin.close(),din.close();gin.close();

}

