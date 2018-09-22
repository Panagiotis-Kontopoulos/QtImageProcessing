//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "ImagXpr7_OCX.h"
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
#include <iostream>
#include <fstream>
#include <math.h>       /* sqrt */
#include <ctime>        /* time */
#include <sstream>
//#include <random>
using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *N1;
        TMenuItem *N2;
        TOpenDialog *OpenDialog1;
        TImagXpress7_ *ImagXpress7_1;
        TMenuItem *N4;
        TPanel *Panel1;
        TLabel *Label1;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N8;
        TMenuItem *N10;
        TMenuItem *N12;
        TMenuItem *N8x821;
        TMenuItem *Dilation1;
        TMenuItem *Erosion1;
        TMenuItem *ShrinkFilter1;
        TMenuItem *SwellFilter1;
        TMenuItem *N13;
        TMenuItem *N15;
        TMenuItem *N16;
        TMenuItem *N17;
        TMenuItem *N18;
        TMenuItem *N19;
        TMenuItem *Dilation2;
        TMenuItem *N4x41;
        TMenuItem *N8x81;
        TMenuItem *N4x42;
        TMenuItem *N8x82;
        TMenuItem *N3x32;
        TMenuItem *N3x33;
        TMenuItem *N3x35;
        TMenuItem *N7;
        TMenuItem *N9;
        TMenuItem *N5x51;
        TMenuItem *N5x53;
        TMenuItem *N5x54;
        TMenuItem *N5x56;
        TMenuItem *N11;
        TMenuItem *N20;
        TMenuItem *N21;
        TMenuItem *N22;
        TMenuItem *N23;
        TMenuItem *N25;
        TMenuItem *N24;
        TMenuItem *FMeasure1;
        TMenuItem *N14;
        TMenuItem *N3;
        TMenuItem *N26;
        TOpenDialog *OpenDialog2;
        TMenuItem *FMeasureAuto1;
        TImagXpress7_ *ImagXpress7_2;
        TMenuItem *FMeasure2;
        void __fastcall N2Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);
        void __fastcall ImagXpress7_1MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall N8Click(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall N12Click(TObject *Sender);
        void __fastcall N8x821Click(TObject *Sender);
        void __fastcall Dilation1Click(TObject *Sender);
        void __fastcall N4x42Click(TObject *Sender);
        void __fastcall N8x82Click(TObject *Sender);
        void __fastcall N3x31Click(TObject *Sender);
        void __fastcall N3x32Click(TObject *Sender);
        void __fastcall N3x33Click(TObject *Sender);
        void __fastcall N3x34Click(TObject *Sender);
        void __fastcall N3x35Click(TObject *Sender);
        void __fastcall N5x51Click(TObject *Sender);
        void __fastcall N5x52Click(TObject *Sender);
        void __fastcall N5x53Click(TObject *Sender);
        void __fastcall N5x56Click(TObject *Sender);
        void __fastcall N5x55Click(TObject *Sender);
        void __fastcall N5x54Click(TObject *Sender);
        void __fastcall SwellFilter1Click(TObject *Sender);
        void __fastcall ShrinkFilter1Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
        void __fastcall FMeasure1Click(TObject *Sender);
        void __fastcall N24Click(TObject *Sender);
        void __fastcall N14Click(TObject *Sender);
        void __fastcall N26Click(TObject *Sender);
        void __fastcall N15Click(TObject *Sender);
        void __fastcall N21Click(TObject *Sender);
        void __fastcall N22Click(TObject *Sender);
        void __fastcall FMeasureAuto1Click(TObject *Sender);
        void __fastcall N20Click(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall N23Click(TObject *Sender);
        void __fastcall N17Click(TObject *Sender);
        void __fastcall N4x41Click(TObject *Sender);
        void __fastcall N8x81Click(TObject *Sender);
        void __fastcall N18Click(TObject *Sender);
        void __fastcall Dilation2Click(TObject *Sender);
        void __fastcall Erosion1Click(TObject *Sender);
        void __fastcall N19Click(TObject *Sender);

private:	// User declarations
        int Ix,Iy,ksw,ksh,windowsize,Ix1,Iy1,Ix2,Iy2;
        AnsiString A, B;
        int thr,savestages,countstages,fromfile;
        float mean, deviation;
        void Rec(int x,int y, unsigned char *IMAGE,long offs);
        int x1,x2,y1,y2;

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        int getFilterSize() const { return windowsize; }
        int getThresholdW() const { return ksw; }
        int getThresholdH() const { return ksh; }
        void setFilterSize(int s) { windowsize = s; }
        void setThreshold(int t) { ksw = t; ksh = t; }
        void setMean(float m) { mean = m; }
        void setDeviation(float d) { deviation = d; }
        void stagesSave();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
