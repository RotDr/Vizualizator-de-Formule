#include <iostream>
#include <fstream>
#include <winbgim.h>
#include <graphics.h>
#include <windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
ifstream fin("listaputeri.in");
ifstream din("texting.in");
char ch[300];
int H;
int main()
{
    initwindow(800,800,"Lista");
    settextstyle(1,0,0);
    fin.getline(ch,255);
    bool played=PlaySound(TEXT("Informatii.wav"),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
    outtextxy(400-textwidth(ch)/2,20-textheight(ch)/2,ch);
    H=0;
    H=20+textheight(ch)/2;
    settextstyle(1,0,0);
    setusercharsize(1,2,1,2);
    while (fin.getline(ch,255))
    {
        int h=textheight(ch);
        int l=textwidth(ch);
        outtextxy(50,H,ch);
        H+=h+4;

    }
    din.getline(ch,255);
    setusercharsize(2,3,2,3);
    outtextxy(780-textwidth(ch),780-textheight(ch),ch);
    rectangle(775-textwidth(ch),775-textheight(ch),785,785);
    getch();
    PlaySound(0,0,0);
    closegraph();
    fin.close();
    din.close();
    return 0;
}
