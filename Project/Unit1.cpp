//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImagXpr7_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  ImagXpress7_1->ScrollBars=3;
  A = "";
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N2Click(TObject *Sender)
{
  //Open Image
  savestages=0;
  if (OpenDialog1->Execute())
  {
    A = OpenDialog1->FileName;

    if (FileExists(A))
    {
      ImagXpress7_1->FileName = A;
      Ix = ImagXpress7_1->IWidth;
      Iy = ImagXpress7_1->IHeight;
      ImagXpress7_2->FileName = A;
      Ix2 = ImagXpress7_2->IWidth;
      Iy2 = ImagXpress7_2->IHeight;
    }
    else return;
  }

  ImagXpress7_1->ColorDepth(1,0,0);
  ImagXpress7_1->ColorDepth(8,1,0);
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);
  ImagXpress7_1->LoadBuffer((long)IMAGE);

  ImagXpress7_2->ColorDepth(1,0,0);
  ImagXpress7_2->ColorDepth(8,1,0);
  ImagXpress7_2->SaveToBuffer = true;
  ImagXpress7_2->SaveFileType = FT_TIFF;
  ImagXpress7_2->SaveFile ();

  unsigned char *IMAGE2;
  HANDLE hIM2 = (HANDLE)ImagXpress7_2->SaveBufferHandle;
  IMAGE2 = (unsigned char *)GlobalLock(hIM2);
  long ln2 = GlobalSize(hIM2);
  long offs2=ln-(long)Ix2*Iy2;
  GlobalUnlock(hIM2);
  ImagXpress7_2->LoadBuffer((long)IMAGE2);

  Form1->N11->Enabled=true;
  Form1->N23->Enabled=true;
  Form1->N24->Enabled=true;

}
//---------------------------------------------------------------------------
//--Mask
void __fastcall TForm1::N4Click(TObject *Sender)
{
  //Mask 4x4 all white
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();

  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (*(IMAGE+(y+1)*Ix+x+offs)==255 && *(IMAGE+y*Ix+(x-1)+offs)==255 && *(IMAGE+y*Ix+x+offs)==0 && *(IMAGE+y*Ix+(x+1)+offs)==255 && *(IMAGE+(y-1)*Ix+x+offs)==255)
        *(IMAGE+y*Ix+x+offs)=255;
     }
  }
  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImagXpress7_1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

  if (!FileExists(A)) return;

  int x = ImagXpress7_1->DIBXPos;
  int y = ImagXpress7_1->DIBYPos;
  int c = ImagXpress7_1->DIBGetPixel(x,y);

  AnsiString S = "["; S=S+x;S=S+",";S=S+y;
  S=S+"] c:";S=S+c;

  Label1->Caption=S;
  //Application->ProcessMessages();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{
  Screen->Cursor =  crHourGlass;

  int Hist[256];
  for (int i=0;i<256;i++) Hist[i]=0;


  unsigned char *I;

  if ((I = (unsigned char *)malloc(Ix*Iy))==NULL)
  {
    ShowMessage("memory problem");return;
  }

  for (int x=0;x<Ix;x++)
  for (int y=0;y<Iy;y++)
  *(I+y*Ix+x) = ImagXpress7_1->DIBGetPixel(x,y);

  for (int x=1;x<Ix-1;x++)
  for (int y=1;y<Iy-1;y++)
  {
    int L =  *(I+y*Ix+x-1)+*(I+y*Ix+x+1)+*(I+(y+1)*Ix+x)+*(I+(y-1)*Ix+x)
             -4*(*(I+y*Ix+x));
    if (L>=10) Hist[*(I+y*Ix+x)]++;
  }
  ImagXpress7_1->DIBUpdate();

  free(I);

 /*
  for (int x=0;x<Ix;x++)
  for (int y=0;y<Iy;y++)
  {
    Hist[ImagXpress7_1->DIBGetPixel(x,y)]++;
  }
   */
  Form3->Series1->Clear();
  for (int i=0;i<256;i++)
  Form3->Series1->AddY(Hist[i]);


  int i1=0,Hmax=0;
  for (int i=0;i<256;i++)
  if (Hist[i]>Hmax) {i1=i;Hmax=Hist[i];}

  int i2=0;
  Hmax=0;
  for (int i=0;i<256;i++)
  if (Hist[i]*(i-i1)*(i-i1)>Hmax) {i2=i;Hmax=Hist[i]*(i-i1)*(i-i1);}

  if (i1>i2) {int t=i1;i1=i2;i2=t;}
  thr=0;
  int Hmin=9999;
  for (int i=i1;i<=i2;i++)
  if (Hist[i]<Hmin) {thr=i;Hmin=Hist[i];}


  AnsiString S = "i1: ";S=S+i1;
  S=S+" i2: ";S=S+i2;
  S=S+" thr: ";S=S+thr;

  Form3->Caption=S;
   Screen->Cursor =  crDefault;

  Form3->Show();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::N6Click(TObject *Sender)
{
  //Add Noise Menu
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
  //Add Random Noise (example in class)
  Form4->ImagXpress7_1->hDIB = ImagXpress7_1->CopyDIB();
  Form4->ImagXpress7_1->ScrollBars=3;
  Form4->ImagXpress7_2->ScrollBars=3;



  for (int i=0;i<100000;i++)
  {
    int x = ((long)rand()*(Ix-2))/32767+1;
    int y = ((long)rand()*(Iy-2))/32767+1;
    Form4->ImagXpress7_1->DIBSetPixel(x,y,0);
  }

  Form4->ImagXpress7_1->DIBUpdate();


  unsigned char *I;

  if ((I = (unsigned char *)malloc(Ix*Iy))==NULL)
  {
    ShowMessage("memory problem");return;
  }

  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();

  for (int x=0;x<Ix;x++)
  {
    Form5->ProgressBar1->Position = x;
    for (int y=0;y<Iy;y++)
    *(I+y*Ix+x) = Form4->ImagXpress7_1->DIBGetPixel(x,y);
  }
  Form4->ImagXpress7_2->hDIB = Form4->ImagXpress7_1->CopyDIB();

  Form5->ProgressBar1->Max = Ix-1;

  for (int x=0;x<Ix;x++)
  {
    Form5->ProgressBar1->Position = x;
    for (int y=0;y<Iy;y++)
    {
       if (*(I+y*Ix+x)==0)
       {
         int c=0;
         for (int ix=x-3;ix<=x+3;ix++)
         for (int iy=y-3;iy<=y+3;iy++)
         {
           if ((ix>=0) && (ix<Ix) && (iy>=0) && (iy<Iy))
           if (*(I+iy*Ix+ix)==255)
             c++;
         }
         if (c>34)
         {
           Form4->ImagXpress7_2->DIBSetPixel(x,y,255);
         }
       }
    }
  }
  Form5->Close();
  Form4->ImagXpress7_2->DIBUpdate();
  free(I);


  Form4->Show();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N10Click(TObject *Sender)
{
  Form6->ImagXpress7_1->hDIB = ImagXpress7_1->CopyDIB();


  Form6->ImagXpress7_1->ScrollBars=3;
  Form6->ImagXpress7_2->ScrollBars=3;
  Form6->ImagXpress7_1->ZoomToFit(2);
  Form6->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
  Form6->Show();
  Form6->Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N12Click(TObject *Sender)
{
  ImagXpress7_1->ColorDepth(1,0,0);

   //Save ImagXpress1 to pointer
  ImagXpress7_1->ColorDepth(8,1,0);

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE1;
  HANDLE hIM1 = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE1 = (unsigned char *)GlobalLock(hIM1);
  long ln1 = GlobalSize(hIM1);
  long offs1=ln1-(long)Ix*Iy;
  GlobalUnlock(hIM1);



  //RLSA

  for (int y=0;y<Iy;y++)
  {
    int x=0;
    while (x<Ix-1)
    {
      while ((*(IMAGE+y*Ix+x+offs)==0) && (x<Ix-1)) x++;
      int X=x;
      while ((*(IMAGE+y*Ix+x+offs)!=0) && (x<Ix-1)) x++;

      if (x-X<13)
      for (int ix=X;ix<x;ix++) *(IMAGE+y*Ix+ix+offs)=4;

    }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);

  for (int y=0;y<Iy;y++)
  for (int x=0;x<Ix;x++)
  if (*(IMAGE+y*Ix+x+offs)<5)
  {
    x1=x;x2=x;y1=y;y2=y;
    *(IMAGE+y*Ix+x+offs)=9;
    Rec(x,y,IMAGE,offs);

    for (int ix=x1;ix<=x2;ix++)
    {
      *(IMAGE1+y1*Ix+ix+offs)=128;
      *(IMAGE1+y2*Ix+ix+offs)=128;
    }
    for (int iy=y1;iy<=y2;iy++)
    {
      *(IMAGE1+iy*Ix+x1+offs)=128;
      *(IMAGE1+iy*Ix+x2+offs)=128;
    }
  }


  ImagXpress7_1->LoadBuffer((long)IMAGE1);



  GlobalFree(IMAGE);
  GlobalFree(IMAGE1);



}
//---------------------------------------------------------------------------

void TForm1::Rec(int x,int y, unsigned char *IMAGE,long offs)
{
    for (int ix=x-1;ix<=x+1;ix++)
    for (int iy=y-1;iy<=y+1;iy++)
    if (*(IMAGE+iy*Ix+ix+offs)<5)
    {
      *(IMAGE+iy*Ix+ix+offs)=9;
      if (x<x1) x1=x;
      if (x>x2) x2=x;
      if (y<y1) y1=y;
      if (y>y2) y2=y;

      Rec(ix,iy,IMAGE,offs);
    }
}

void __fastcall TForm1::N8x821Click(TObject *Sender)
{
  //Mask 8x8 all white
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();

  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (*(IMAGE+(y+1)*Ix+(x-1)+offs)==255 && *(IMAGE+(y+1)*Ix+x+offs)==255 && *(IMAGE+(y+1)*Ix+(x+1)+offs)==255)
        if (*(IMAGE+y*Ix+(x-1)+offs)==255 && *(IMAGE+y*Ix+x+offs)==0 && *(IMAGE+y*Ix+(x+1)+offs)==255)
        if (*(IMAGE+(y-1)*Ix+(x-1)+offs)==255 && *(IMAGE+(y-1)*Ix+x+offs)==255 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==255)
        *(IMAGE+y*Ix+x+offs)=255;
        //if (ImagXpress7_1->DIBGetPixel(x-1,y+1)==0 && ImagXpress7_1->DIBGetPixel(x,y+1)==0 && ImagXpress7_1->DIBGetPixel(x+1,y+1)==0)
        //if (ImagXpress7_1->DIBGetPixel(x-1,y)==0 && ImagXpress7_1->DIBGetPixel(x,y)==1 && ImagXpress7_1->DIBGetPixel(x+1,y)==0)
        //if (ImagXpress7_1->DIBGetPixel(x-1,y-1)==0 && ImagXpress7_1->DIBGetPixel(x,y-1)==0 && ImagXpress7_1->DIBGetPixel(x+1,y-1)==0)
        //ImagXpress7_1->DIBSetPixel(x,y,0);
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Dilation1Click(TObject *Sender)
{
  //Cube 3x3 dilation
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);
  
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (*(IMAGE+y*Ix+x+offs)==0)
        {  //x-1
           if (x>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-1)+offs)==255)temp[x-1][y-1]=2;
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-1)+offs)==255)temp[x-1][y+1]=2;
           //x
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==255)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==255)temp[x][y+1]=2;
           //x+1
           if (x+1<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+1)+offs)==255)temp[x+1][y+1]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=0;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4x42Click(TObject *Sender)
{
  //Mask 4x4 all black
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();

  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (*(IMAGE+(y+1)*Ix+x+offs)==0 && *(IMAGE+y*Ix+(x-1)+offs)==0 && *(IMAGE+y*Ix+x+offs)==255 && *(IMAGE+y*Ix+(x+1)+offs)==0 && *(IMAGE+(y-1)*Ix+x+offs)==0)
        *(IMAGE+y*Ix+x+offs)=0;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8x82Click(TObject *Sender)
{
  //Mask 8x8 all black
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();

  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (*(IMAGE+(y+1)*Ix+(x-1)+offs)==0 && *(IMAGE+(y+1)*Ix+x+offs)==0 && *(IMAGE+(y+1)*Ix+(x+1)+offs)==0)
        if (*(IMAGE+y*Ix+(x-1)+offs)==0 && *(IMAGE+y*Ix+x+offs)==255 && *(IMAGE+y*Ix+(x+1)+offs)==0)
        if (*(IMAGE+(y-1)*Ix+(x-1)+offs)==0 && *(IMAGE+(y-1)*Ix+x+offs)==0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==0)
        *(IMAGE+y*Ix+x+offs)=0;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N3x31Click(TObject *Sender)
{
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==1)
        {  //x-1
           if (x>0 && y>0 && ImagXpress7_1->DIBGetPixel(x-1,y-1)==255)temp[x-1][y-1]=2;
           if (x>0 && ImagXpress7_1->DIBGetPixel(x-1,y)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && ImagXpress7_1->DIBGetPixel(x-1,y+1)==255)temp[x-1][y+1]=2;
           //x
           if (y>0 && ImagXpress7_1->DIBGetPixel(x,y-1)==255)temp[x][y-1]=2;
           if (y+1<Iy && ImagXpress7_1->DIBGetPixel(x,y+1)==255)temp[x][y+1]=2;
           //x+1
           if (x+1<Ix && y>0 && ImagXpress7_1->DIBGetPixel(x+1,y-1)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && ImagXpress7_1->DIBGetPixel(x+1,y)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && ImagXpress7_1->DIBGetPixel(x+1,y+1)==255)temp[x+1][y+1]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)ImagXpress7_1->DIBSetPixel(x,y,255);
     }
  }

  ImagXpress7_1->DIBUpdate();
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3x32Click(TObject *Sender)
{
  //Cross 3x3 dilation
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (*(IMAGE+y*Ix+x+offs)==0)
        {
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==255)temp[x-1][y]=2;
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==255)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==255)temp[x][y+1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==255)temp[x+1][y]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=0;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);;
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3x33Click(TObject *Sender)
{
  //Cube 3x3 erosion
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==255)
        {  //x-1
           if (x>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-1)+offs)==0)temp[x-1][y-1]=2;
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==0)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-1)+offs)==0)temp[x-1][y+1]=2;
           //x
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==0)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==0)temp[x][y+1]=2;
           //x+1
           if (x+1<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==0)temp[x+1][y-1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==0)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+1)+offs)==0)temp[x+1][y+1]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=255;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3x34Click(TObject *Sender)
{
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==0)
        {
           if (x>0 && y>0 && ImagXpress7_1->DIBGetPixel(x-1,y-1)==255)temp[x-1][y]=2;
           if (x>0 && ImagXpress7_1->DIBGetPixel(x-1,y)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && ImagXpress7_1->DIBGetPixel(x-1,y+1)==255)temp[x-1][y+1]=2;
           if (y>0 && ImagXpress7_1->DIBGetPixel(x,y-1)==255)temp[x][y-1]=2;
           if (y+1<Iy && ImagXpress7_1->DIBGetPixel(x,y+1)==255)temp[x][y+1]=2;
           if (x+1<Ix && y>0 && ImagXpress7_1->DIBGetPixel(x+1,y-1)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && ImagXpress7_1->DIBGetPixel(x+1,y)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && ImagXpress7_1->DIBGetPixel(x+1,y+1)==255)temp[x+1][y+1]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)ImagXpress7_1->DIBSetPixel(x,y,255);
     }
  }

  ImagXpress7_1->DIBUpdate();
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3x35Click(TObject *Sender)
{
  //Cross 3x3 erosion
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-2;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=1;x<Ix-2;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-2;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==255)
        {
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==0)temp[x-1][y]=2;
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==0)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==0)temp[x][y+1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==0)temp[x+1][y]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=255;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5x51Click(TObject *Sender)
{
  //Cube 5x5 dilation
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-3;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=2;x<Ix-3;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=2;y<Iy-3;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==0)
        {  //x-2
           if (x-1>0 && y-1>0 && *(IMAGE+(y-2)*Ix+(x-2)+offs)==255)temp[x-2][y-2]=2;
           if (x-1>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-2)+offs)==255)temp[x-2][y-1]=2;
           if (x-1>0 && *(IMAGE+y*Ix+(x-2)+offs)==255)temp[x-2][y]=2;
           if (x-1>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-2)+offs)==255)temp[x-2][y+1]=2;
           if (x-1>0 && y+2<Iy && *(IMAGE+(y+2)*Ix+(x-2)+offs)==255)temp[x-2][y+2]=2;
           //x-1
           if (x>0 && y-1>0 && *(IMAGE+(y-2)*Ix+(x-1)+offs)==255)temp[x-1][y-2]=2;
           if (x>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-1)+offs)==255)temp[x-1][y-1]=2;
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-1)+offs)==255)temp[x-1][y+1]=2;
           if (x>0 && y+2<Iy && *(IMAGE+(y+2)*Ix+(x-1)+offs)==255)temp[x-1][y+2]=2;
           //x
           if (y-1>0 && *(IMAGE+(y-2)*Ix+x+offs)==255)temp[x][y-2]=2;
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==255)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==255)temp[x][y+1]=2;
           if (y+2<Iy && *(IMAGE+(y+2)*Ix+x+offs)==255)temp[x][y+2]=2;
           //x+1
           if (x+1<Ix && y-1>0 && *(IMAGE+(y-2)*Ix+(x+1)+offs)==255)temp[x+1][y-2]=2;
           if (x+1<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+1)+offs)==255)temp[x+1][y+1]=2;
           if (x+1<Ix && y+2<Iy && *(IMAGE+(y+2)*Ix+(x+1)+offs)==255)temp[x+1][y+2]=2;
           //x+2
           if (x+2<Ix && y-1>0 && *(IMAGE+(y-2)*Ix+(x+2)+offs)==255)temp[x+2][y-2]=2;
           if (x+2<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+2)+offs)==255)temp[x+2][y-1]=2;
           if (x+2<Ix && *(IMAGE+y*Ix+(x+2)+offs)==255)temp[x+2][y]=2;
           if (x+2<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+2)+offs)==255)temp[x+2][y+1]=2;
           if (x+2<Ix && y+2<Iy && *(IMAGE+(y+2)*Ix+(x+2)+offs)==255)temp[x+2][y+2]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)ImagXpress7_1->DIBSetPixel(x,y,0);
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5x52Click(TObject *Sender)
{
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==1)
        {  //x-2
           if (x-1>0 && y-1>0 && ImagXpress7_1->DIBGetPixel(x-2,y-2)==255)temp[x-2][y-2]=2;
           if (x-1>0 && y>0 && ImagXpress7_1->DIBGetPixel(x-2,y-1)==255)temp[x-2][y-1]=2;
           if (x-1>0 && ImagXpress7_1->DIBGetPixel(x-2,y)==255)temp[x-2][y]=2;
           if (x-1>0 && y+1<Iy && ImagXpress7_1->DIBGetPixel(x-2,y+1)==255)temp[x-2][y+1]=2;
           if (x-1>0 && y+2<Iy && ImagXpress7_1->DIBGetPixel(x-2,y+2)==255)temp[x-2][y+2]=2;
           //x-1
           if (x>0 && y-1>0 && ImagXpress7_1->DIBGetPixel(x-1,y-2)==255)temp[x-1][y-2]=2;
           if (x>0 && y>0 && ImagXpress7_1->DIBGetPixel(x-1,y-1)==255)temp[x-1][y-1]=2;
           if (x>0 && ImagXpress7_1->DIBGetPixel(x-1,y)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && ImagXpress7_1->DIBGetPixel(x-1,y+1)==255)temp[x-1][y+1]=2;
           if (x>0 && y+2<Iy && ImagXpress7_1->DIBGetPixel(x-1,y+2)==255)temp[x-1][y+2]=2;
           //x
           if (y-1>0 && ImagXpress7_1->DIBGetPixel(x,y-2)==255)temp[x][y-2]=2;
           if (y>0 && ImagXpress7_1->DIBGetPixel(x,y-1)==255)temp[x][y-1]=2;
           if (y+1<Iy && ImagXpress7_1->DIBGetPixel(x,y+1)==255)temp[x][y+1]=2;
           if (y+2<Iy && ImagXpress7_1->DIBGetPixel(x,y+2)==255)temp[x][y+2]=2;
           //x+1
           if (x+1<Ix && y-1>0 && ImagXpress7_1->DIBGetPixel(x+1,y-2)==255)temp[x+1][y-2]=2;
           if (x+1<Ix && y>0 && ImagXpress7_1->DIBGetPixel(x+1,y-1)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && ImagXpress7_1->DIBGetPixel(x+1,y)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && ImagXpress7_1->DIBGetPixel(x+1,y+1)==255)temp[x+1][y+1]=2;
           if (x+1<Ix && y+2<Iy && ImagXpress7_1->DIBGetPixel(x+1,y+2)==255)temp[x+1][y+2]=2;
           //x+2
           if (x+2<Ix && y-1>0 && ImagXpress7_1->DIBGetPixel(x+2,y-2)==255)temp[x+2][y-2]=2;
           if (x+2<Ix && y>0 && ImagXpress7_1->DIBGetPixel(x+2,y-1)==255)temp[x+2][y-1]=2;
           if (x+2<Ix && ImagXpress7_1->DIBGetPixel(x+2,y)==255)temp[x+2][y]=2;
           if (x+2<Ix && y+1<Iy && ImagXpress7_1->DIBGetPixel(x+2,y+1)==255)temp[x+2][y+1]=2;
           if (x+2<Ix && y+2<Iy && ImagXpress7_1->DIBGetPixel(x+2,y+2)==255)temp[x+2][y+2]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)ImagXpress7_1->DIBSetPixel(x,y,255);
     }
  }

  ImagXpress7_1->DIBUpdate();
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5x53Click(TObject *Sender)
{
  //Rhombus 5x5 dilation
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-3;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=2;x<Ix-3;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=2;y<Iy-3;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==0)
        {  //x-2
           if (x-1>0 && *(IMAGE+y*Ix+(x-2)+offs)==255)temp[x-2][y]=2;
           //x-1
           if (x>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-1)+offs)==255)temp[x-1][y-1]=2;
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-1)+offs)==255)temp[x-1][y+1]=2;
           //x
           if (y-1>0 && *(IMAGE+(y-2)*Ix+x+offs)==255)temp[x][y-2]=2;
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==255)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==255)temp[x][y+1]=2;
           if (y+2<Iy && *(IMAGE+(y+2)*Ix+x+offs)==255)temp[x][y+2]=2;
           //x+1
           if (x+1<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+1)+offs)==255)temp[x+1][y+1]=2;
           //x+2
           if (x+2<Ix && *(IMAGE+y*Ix+(x+2)+offs)==255)temp[x+2][y]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=0;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5x56Click(TObject *Sender)
{
  //Cube 5x5 erosion
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-3;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=2;x<Ix-3;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=2;y<Iy-3;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==255)
        {  //x-2
           if (x-1>0 && y-1>0 && *(IMAGE+(y-2)*Ix+(x-2)+offs)==0)temp[x-2][y-2]=2;
           if (x-1>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-2)+offs)==0)temp[x-2][y-1]=2;
           if (x-1>0 && *(IMAGE+y*Ix+(x-2)+offs)==0)temp[x-2][y]=2;
           if (x-1>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-2)+offs)==0)temp[x-2][y+1]=2;
           if (x-1>0 && y+2<Iy && *(IMAGE+(y+2)*Ix+(x-2)+offs)==0)temp[x-2][y+2]=2;
           //x-1
           if (x>0 && y-1>0 && *(IMAGE+(y-2)*Ix+(x-1)+offs)==0)temp[x-1][y-2]=2;
           if (x>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-1)+offs)==0)temp[x-1][y-1]=2;
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==0)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-1)+offs)==0)temp[x-1][y+1]=2;
           if (x>0 && y+2<Iy && *(IMAGE+(y+2)*Ix+(x-1)+offs)==0)temp[x-1][y+2]=2;
           //x
           if (y-1>0 && *(IMAGE+(y-2)*Ix+x+offs)==0)temp[x][y-2]=2;
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==0)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==0)temp[x][y+1]=2;
           if (y+2<Iy && *(IMAGE+(y+2)*Ix+x+offs)==0)temp[x][y+2]=2;
           //x+1
           if (x+1<Ix && y-1>0 && *(IMAGE+(y-2)*Ix+(x+1)+offs)==0)temp[x+1][y-2]=2;
           if (x+1<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==0)temp[x+1][y-1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==0)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+1)+offs)==0)temp[x+1][y+1]=2;
           if (x+1<Ix && y+2<Iy && *(IMAGE+(y+2)*Ix+(x+1)+offs)==0)temp[x+1][y+2]=2;
           //x+2
           if (x+2<Ix && y-1>0 && *(IMAGE+(y-2)*Ix+(x+2)+offs)==0)temp[x+2][y-2]=2;
           if (x+2<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+2)+offs)==0)temp[x+2][y-1]=2;
           if (x+2<Ix && *(IMAGE+y*Ix+(x+2)+offs)==0)temp[x+2][y]=2;
           if (x+2<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+2)+offs)==0)temp[x+2][y+1]=2;
           if (x+2<Ix && y+2<Iy && *(IMAGE+(y+2)*Ix+(x+2)+offs)==0)temp[x+2][y+2]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=255;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5x55Click(TObject *Sender)
{
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==0)
        {  //x-2
           if (x-1>0 && y-1>0 && ImagXpress7_1->DIBGetPixel(x-2,y-2)==255)temp[x-2][y-2]=2;
           if (x-1>0 && y>0 && ImagXpress7_1->DIBGetPixel(x-2,y-1)==255)temp[x-2][y-1]=2;
           if (x-1>0 && ImagXpress7_1->DIBGetPixel(x-2,y)==255)temp[x-2][y]=2;
           if (x-1>0 && y+1<Iy && ImagXpress7_1->DIBGetPixel(x-2,y+1)==255)temp[x-2][y+1]=2;
           if (x-1>0 && y+2<Iy && ImagXpress7_1->DIBGetPixel(x-2,y+2)==255)temp[x-2][y+2]=2;
           //x-1
           if (x>0 && y-1>0 && ImagXpress7_1->DIBGetPixel(x-1,y-2)==255)temp[x-1][y-2]=2;
           if (x>0 && y>0 && ImagXpress7_1->DIBGetPixel(x-1,y-1)==255)temp[x-1][y-1]=2;
           if (x>0 && ImagXpress7_1->DIBGetPixel(x-1,y)==255)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && ImagXpress7_1->DIBGetPixel(x-1,y+1)==255)temp[x-1][y+1]=2;
           if (x>0 && y+2<Iy && ImagXpress7_1->DIBGetPixel(x-1,y+2)==255)temp[x-1][y+2]=2;
           //x
           if (y-1>0 && ImagXpress7_1->DIBGetPixel(x,y-2)==255)temp[x][y-2]=2;
           if (y>0 && ImagXpress7_1->DIBGetPixel(x,y-1)==255)temp[x][y-1]=2;
           if (y+1<Iy && ImagXpress7_1->DIBGetPixel(x,y+1)==255)temp[x][y+1]=2;
           if (y+2<Iy && ImagXpress7_1->DIBGetPixel(x,y+2)==255)temp[x][y+2]=2;
           //x+1
           if (x+1<Ix && y-1>0 && ImagXpress7_1->DIBGetPixel(x+1,y-2)==255)temp[x+1][y-2]=2;
           if (x+1<Ix && y>0 && ImagXpress7_1->DIBGetPixel(x+1,y-1)==255)temp[x+1][y-1]=2;
           if (x+1<Ix && ImagXpress7_1->DIBGetPixel(x+1,y)==255)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && ImagXpress7_1->DIBGetPixel(x+1,y+1)==255)temp[x+1][y+1]=2;
           if (x+1<Ix && y+2<Iy && ImagXpress7_1->DIBGetPixel(x+1,y+2)==255)temp[x+1][y+2]=2;
           //x+2
           if (x+2<Ix && y-1>0 && ImagXpress7_1->DIBGetPixel(x+2,y-2)==255)temp[x+2][y-2]=2;
           if (x+2<Ix && y>0 && ImagXpress7_1->DIBGetPixel(x+2,y-1)==255)temp[x+2][y-1]=2;
           if (x+2<Ix && ImagXpress7_1->DIBGetPixel(x+2,y)==255)temp[x+2][y]=2;
           if (x+2<Ix && y+1<Iy && ImagXpress7_1->DIBGetPixel(x+2,y+1)==255)temp[x+2][y+1]=2;
           if (x+2<Ix && y+2<Iy && ImagXpress7_1->DIBGetPixel(x+2,y+2)==255)temp[x+2][y+2]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)ImagXpress7_1->DIBSetPixel(x,y,255);
     }
  }

  ImagXpress7_1->DIBUpdate();
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5x54Click(TObject *Sender)
{
  //Rhombus 5x5 erosion
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-3;
  Form5->Show();
  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];
  for (int x=2;x<Ix-3;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=2;y<Iy-3;y++)
     {
        if (ImagXpress7_1->DIBGetPixel(x,y)==255)
        {  //x-2
           if (x-1>0 && *(IMAGE+y*Ix+(x-2)+offs)==0)temp[x-2][y]=2;
           //x-1
           if (x>0 && y>0 && *(IMAGE+(y-1)*Ix+(x-1)+offs)==0)temp[x-1][y-1]=2;
           if (x>0 && *(IMAGE+y*Ix+(x-1)+offs)==0)temp[x-1][y]=2;
           if (x>0 && y+1<Iy && *(IMAGE+(y+1)*Ix+(x-1)+offs)==0)temp[x-1][y+1]=2;
           //x
           if (y-1>0 && *(IMAGE+(y-2)*Ix+x+offs)==0)temp[x][y-2]=2;
           if (y>0 && *(IMAGE+(y-1)*Ix+x+offs)==0)temp[x][y-1]=2;
           if (y+1<Iy && *(IMAGE+(y+1)*Ix+x+offs)==0)temp[x][y+1]=2;
           if (y+2<Iy && *(IMAGE+(y+2)*Ix+x+offs)==0)temp[x][y+2]=2;
           //x+1
           if (x+1<Ix && y>0 && *(IMAGE+(y-1)*Ix+(x+1)+offs)==0)temp[x+1][y-1]=2;
           if (x+1<Ix && *(IMAGE+y*Ix+(x+1)+offs)==0)temp[x+1][y]=2;
           if (x+1<Ix && y+1<Iy && *(IMAGE+(y+1)*Ix+(x+1)+offs)==0)temp[x+1][y+1]=2;
           //x+2
           if (x+2<Ix && *(IMAGE+y*Ix+(x+2)+offs)==0)temp[x+2][y]=2;
        }
     }
  }
  for (int x=0;x<Ix;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=255;
     }
  }

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::SwellFilter1Click(TObject *Sender)
{

  //Swell Filter
  if (!fromfile)Form2->ShowModal();

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  int maxnumofpixels=windowsize*windowsize;
  int black=0;
  for (int y=windowsize;y<Iy-windowsize;y++)
  {
     Form5->ProgressBar1->Position = y;
     for (int x=windowsize;x<Ix-windowsize;x++)
     {
        int y1=y-((windowsize-1)/2);
        if (y1<0)y1=0;
        int maxy=y+((windowsize-1)/2);
        if (maxy>Iy)maxy=Iy;
        black=0;
        while (y1<maxy)
        {
           int x1=x-((windowsize-1)/2);
           if (x1<0)x1=0;
           int maxx=x+((windowsize-1)/2);
           if (maxx>Ix)maxx=Ix;
           while (x1<maxx)
           {
              if (*(IMAGE+y1*Ix+x1+offs)==0 && x1!=x && y1!=y)
              {
                 black++;
              }
              x1++;
           }
           y1++;
        }
        if (black>ksw)temp[x][y]=2;
     }
  }

  for (int x=0;x<Ix;x++)
  {
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=0;
     }
  }
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShrinkFilter1Click(TObject *Sender)
{

  //Shrink Filter
  if (!fromfile)Form2->ShowModal();

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  int** temp=new int*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new int[Iy];

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  int maxnumofpixels=windowsize*windowsize;
  int white=0;
  for (int y=windowsize;y<Iy-windowsize;y++)
  {
     Form5->ProgressBar1->Position = y;
     for (int x=windowsize;x<Ix-windowsize;x++)
     {
        int y1=y-((windowsize-1)/2);
        if (y1<0)y1=0;
        int maxy=y+((windowsize-1)/2);
        if (maxy>Iy)maxy=Iy;
        white=0;
        while (y1<maxy)
        {
           int x1=x-((windowsize-1)/2);
           if (x1<0)x1=0;
           int maxx=x+((windowsize-1)/2);
           if (maxx>Ix)maxx=Ix;
           while (x1<maxx)
           {
              if (*(IMAGE+y1*Ix+x1+offs)==255 && x1!=x && y1!=y)
              {
                 white++;
              }
              x1++;
           }
           y1++;
        }
        if (white>ksh)temp[x][y]=2;
     }
  }

  for (int x=0;x<Ix;x++)
  {
     for (int y=0;y<Iy;y++)
     {
        if (temp[x][y]==2)*(IMAGE+y*Ix+x+offs)=255;
     }
  }
  for (int x=0;x<Ix;x++)free(temp[x]);
  free(temp);

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  IMAGE=NULL;
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N11Click(TObject *Sender)
{
   //Save Image
   ImagXpress7_1->SaveFileType =  FT_TIFF;
   ImagXpress7_1->SaveTIFFCompression = TIFF_CCITTFAX4;
   ImagXpress7_1->SaveToBuffer = false;
   ImagXpress7_1->SaveMultiPage = false;
   ImagXpress7_1->SaveFileName = ExtractFilePath(OpenDialog1->FileName)+"savedimage.tiff";
   ImagXpress7_1->SaveFile();

   A = ExtractFilePath(OpenDialog1->FileName)+"savedimage.tiff";
   ShowMessage(A);
   ImagXpress7_1->FileName = A;
   Ix = ImagXpress7_1->IWidth;
   Iy = ImagXpress7_1->IHeight;
   ImagXpress7_1->ColorDepth(1,0,0);
   ImagXpress7_1->ColorDepth(8,1,0);
   ImagXpress7_1->SaveToBuffer = true;
   ImagXpress7_1->SaveFileType =  FT_TIFF;
   ImagXpress7_1->SaveFile ();

   unsigned char *IMAGE;
   HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
   IMAGE = (unsigned char *)GlobalLock(hIM);
   long ln = GlobalSize(hIM);
   long offs=ln-(long)Ix*Iy;
   GlobalUnlock(hIM);
   ImagXpress7_1->LoadBuffer((long)IMAGE);

}
//---------------------------------------------------------------------------


void __fastcall TForm1::FMeasure1Click(TObject *Sender)
{
  //F-Measure Comparison
  ShowMessage("Επέλεξε εικόνα για σύγκριση.");
  if (OpenDialog1->Execute())
  {
    A = OpenDialog1->FileName;

    if (FileExists(A))
    {
      Form4->ImagXpress7_2->FileName = A;
      Ix1 = Form4->ImagXpress7_2->IWidth;
      Iy1 = Form4->ImagXpress7_2->IHeight;
    }
    else return;
  }
  Form4->ImagXpress7_2->ColorDepth(8,1,0);
  Form4->ImagXpress7_2->SaveToBuffer = true;
  Form4->ImagXpress7_2->SaveFileType =  FT_TIFF;
  Form4->ImagXpress7_2->SaveFile ();

  Form1->ImagXpress7_2->hDIB=Form4->ImagXpress7_2->CopyDIB();

  unsigned char *IMAGE1;
  HANDLE hIM1 = (HANDLE)Form4->ImagXpress7_2->SaveBufferHandle;
  IMAGE1 = (unsigned char *)GlobalLock(hIM1);
  long ln1 = GlobalSize(hIM1);
  long offs1=ln1-(long)Ix1*Iy1;
  GlobalUnlock(hIM1);
  Form4->ImagXpress7_2->LoadBuffer((long)IMAGE1);

  Form4->ImagXpress7_1->hDIB = ImagXpress7_1->CopyDIB();
  Form4->ImagXpress7_1->SaveToBuffer = true;
  Form4->ImagXpress7_1->SaveFileType =  FT_TIFF;
  Form4->ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)Form4->ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);


  Form4->ImagXpress7_1->ScrollBars=3;
  Form4->ImagXpress7_2->ScrollBars=3;

  //F-Measure Calculation Start
  //IMAGE = image after proccessing
  //IMAGE1 = original image
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  float CTP=0,CFP=0,CFN=0;
  int error=0;
  float RC=0,PR=0,FM=0; //Anakthsh, Akribeia, F-Measure
  for (int x=1;x<Ix-1;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-1;y++)
     {
        if (x>Ix1 || y>Iy1)
        {
           ShowMessage("Οι εικόνες δεν έχουν το ίδιο μέγεθος...");
           error=1;
           break;
        }
        if (*(IMAGE+y*Ix+x+offs)==0 && *(IMAGE1+y*Ix1+x+offs1)==0)CTP=CTP+1;
        if (*(IMAGE+y*Ix+x+offs)==0 && *(IMAGE1+y*Ix1+x+offs1)==255)CFP=CFP+1;
        if (*(IMAGE+y*Ix+x+offs)==255 && *(IMAGE1+y*Ix1+x+offs1)==0)CFN=CFN+1;
     }
     if (error)break;
  }
  AnsiString str;
  if (!error)
  {
    Form5->Close();
    RC=CTP/(CFN+CTP);
    PR=CTP/(CFP+CTP);
    FM=((2*RC*PR/(RC+PR))*100); // FM=(((2*RC*PR)/(RC+PR))*100)/100;
    str = FM;
    ShowMessage("Ομοιότητα : "+str+" %");
  }

  GlobalFree(IMAGE);
  GlobalFree(IMAGE1);
  Screen->Cursor =  crDefault;
  Form4->Caption="Ομοιότητα : "+str+" %";
  Form4->Show();
  Form1->FMeasureAuto1->Enabled = true;
  
}
//---------------------------------------------------------------------------



void __fastcall TForm1::N24Click(TObject *Sender)
{
   //Activate Stages save
   savestages=1;
   Form1->N24->Enabled=false;
   Form1->N14->Enabled=true;
   Form1->N20->Enabled=true;
   ImagXpress7_1->SaveFileType =  FT_TIFF;
   ImagXpress7_1->SaveTIFFCompression = TIFF_CCITTFAX4;
   ImagXpress7_1->SaveToBuffer = false;
   ImagXpress7_1->SaveMultiPage = false;
   ImagXpress7_1->SaveFileName = ExtractFilePath(OpenDialog1->FileName)+"stage0.tiff";
   ImagXpress7_1->SaveFile();

   A = ExtractFilePath(OpenDialog1->FileName)+"stage0.tiff";
   ShowMessage(A);
   ImagXpress7_1->FileName = A;
   Ix = ImagXpress7_1->IWidth;
   Iy = ImagXpress7_1->IHeight;
   ImagXpress7_1->ColorDepth(1,0,0);
   ImagXpress7_1->ColorDepth(8,1,0);
   ImagXpress7_1->SaveToBuffer = true;
   ImagXpress7_1->SaveFileType =  FT_TIFF;
   ImagXpress7_1->SaveFile ();

   unsigned char *IMAGE;
   HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
   IMAGE = (unsigned char *)GlobalLock(hIM);
   long ln = GlobalSize(hIM);
   long offs=ln-(long)Ix*Iy;
   GlobalUnlock(hIM);
   ImagXpress7_1->LoadBuffer((long)IMAGE);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N14Click(TObject *Sender)
{
   //Deactivate Stages save
   savestages=0;
   Form1->N24->Enabled=true;
   Form1->N14->Enabled=false;
   Form1->N20->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N26Click(TObject *Sender)
{
  //Input methods to be applied from file
  if (OpenDialog2->Execute())
  {
    B = OpenDialog2->FileName;
    string line;

    if (FileExists(B))
    {
      ifstream myfile (B.c_str());
      if (myfile.is_open())
      {
         while ( getline (myfile,line) )
         {
           if (!line.compare("Mask 4x4 white"))
           {
              ShowMessage("Mask 4x4 white");
              Form1->N4Click(Sender);
           }
           else if (!line.compare("Mask 4x4 black"))
           {
              ShowMessage("Mask 4x4 black");
              Form1->N4x42Click(Sender);
           }
           else if (!line.compare("Mask 8x8 white"))
           {
              ShowMessage("Mask 8x8 white");
              Form1->N8x821Click(Sender);
           }
           else if (!line.compare("Mask 8x8 black"))
           {
              ShowMessage("Mask 8x8 black");
              Form1->N8x82Click(Sender);
           }
           else if (!line.compare("Dilation All 3x3"))
           {
              ShowMessage("Dilation All 3x3");
              Form1->Dilation1Click(Sender);
           }
           else if (!line.compare("Dilation Cross 3x3"))
           {
              ShowMessage("Dilation Cross 3x3");
              Form1->N3x32Click(Sender);
           }
           else if (!line.compare("Dilation All 5x5"))
           {
              ShowMessage("Dilation All 5x5");
              Form1->N5x51Click(Sender);
           }
           else if (!line.compare("Dilation Rhombus 5x5"))
           {
              ShowMessage("Dilation Rhombus 5x5");
              Form1->N5x53Click(Sender);
           }
           else if (!line.compare("Erosion All 3x3"))
           {
              ShowMessage("Erosion All 3x3");
              Form1->N3x33Click(Sender);
           }
           else if (!line.compare("Erosion Cross 3x3"))
           {
              ShowMessage("Erosion Cross 3x3");
              Form1->N3x35Click(Sender);
           }
           else if (!line.compare("Erosion All 5x5"))
           {
              ShowMessage("Erosion All 5x5");
              Form1->N5x56Click(Sender);
           }
           else if (!line.compare("Erosion Rhombus 5x5"))
           {
              ShowMessage("Erosion Rhombus 5x5");
              Form1->N5x54Click(Sender);
           }
           else if (!line.compare("Swell Filter"))
           {
              ShowMessage("Swell Filter");
              Form1->SwellFilter1Click(Sender);
           }
           else if (!line.compare("Shrink Filter"))
           {
              ShowMessage("Shrink Filter");
              Form1->ShrinkFilter1Click(Sender);
           }
           else
           {
              fromfile = 1;
              int pos = 0;
              string parts[3];
              stringstream ssin(line);
              while (ssin.good() && pos < 3){
                 ssin >> parts[pos];
                 ++pos;
              }
              /*for(int i = 0; i < 3; i++){
                 ShowMessage(parts[i].c_str());
              }*/
              if (atoi(parts[1].c_str())%2==0)
              {
                 Form1->setFilterSize(atoi(parts[1].c_str())+1);
              }
              else
              {
                 Form1->setFilterSize(atoi(parts[1].c_str()));
              }
              Form1->setThreshold(atoi(parts[2].c_str()));
              if (!parts[0].compare("Swell-Filter"))
              {
                 ShowMessage("Swell-Filter\n\nSize : "+IntToStr(Form1->getFilterSize())+"\n\nThreshold : "+IntToStr(Form1->getThresholdW()));
                 Form1->SwellFilter1Click(Sender);
              }
              else if (!parts[0].compare("Shrink-Filter"))
              {
                 ShowMessage("Shrink-Filter\n\nSize : "+IntToStr(Form1->getFilterSize())+"\n\nThreshold : "+IntToStr(Form1->getThresholdH()));
                 Form1->ShrinkFilter1Click(Sender);
              }
              else
              {
                 ShowMessage("Unknown Command...");
              }
              fromfile = 0;
           }
         }
         myfile.close();
      }
      else
      {
        cout << "Unable to open file";
      }
    }
    else return;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N15Click(TObject *Sender)
{

  //Add Gaussian Noise to the Image
  float pi = 3.14159265359;
  float e = 2.718;
  float prob = 0.0, min=1000,max=0;
  float** temp=new float*[Ix];
  for (int x=0;x<Ix;x++)
  temp[x]=new float[Iy];

  Form7->ShowModal();

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();

  int bl=0,wh=0;
  for (int x=0;x<Ix;x++)
  {
    Form5->ProgressBar1->Position = x;
    for (int y=0;y<Iy;y++)
    {
      prob = (1/deviation*sqrt(2*pi))*pow(e,(0-1)*(pow(((long)rand()*(255-0)/32767+1)-mean,2)/(2*pow(deviation,2))));
      temp[x][y]=prob;
      if (prob > max)max=prob;
      if (prob < min)min=prob;
    }
  }
  //ShowMessage("Minimum Probability : "+FloatToStr(min)+" %\nMaximum Probability : "+FloatToStr(max)+" %\nAverage Probability : "+FloatToStr((max+min)/2)+" %");
  for (int x=0;x<Ix;x++)
  {
    Form5->ProgressBar1->Position = x;
    for (int y=0;y<Iy;y++)
    {
      if (temp[x][y] >= (min+max)/2)
      {
        *(IMAGE+y*Ix+x+offs) = 0;
        bl++;
      }
      else wh++;
    }
  }
  ShowMessage("Noise changed : "+IntToStr(bl)+" pixels\nUnchanged : "+IntToStr(wh)+" pixels");

  ImagXpress7_1->LoadBuffer((long)IMAGE);
  GlobalFree(IMAGE);
  Form5->Close();
  if (savestages==1)
  {
     Form1->stagesSave();
  }
  Screen->Cursor =  crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N21Click(TObject *Sender)
{
  //Undo last action
  countstages--;
  if (countstages<0)
  {
    ShowMessage("No more steps to undo.");
    countstages++;
  }
  AnsiString nameofimage = "stage"+IntToStr(countstages)+".tiff";
  A = ExtractFilePath(OpenDialog1->FileName)+nameofimage;
  ShowMessage(A);
  if (FileExists(A))
    {
      ImagXpress7_1->FileName = A;
      Ix = ImagXpress7_1->IWidth;
      Iy = ImagXpress7_1->IHeight;
      ImagXpress7_1->ColorDepth(1,0,0);
      ImagXpress7_1->ColorDepth(8,1,0);
      ImagXpress7_1->SaveToBuffer = true;
      ImagXpress7_1->SaveFileType =  FT_TIFF;
      ImagXpress7_1->SaveFile();

      unsigned char *IMAGE;
      HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
      IMAGE = (unsigned char *)GlobalLock(hIM);
      long ln = GlobalSize(hIM);
      long offs=ln-(long)Ix*Iy;
      GlobalUnlock(hIM);
      ImagXpress7_1->LoadBuffer((long)IMAGE);
    }
  else
    {
      ShowMessage("File Missing.");
      countstages++;
      return;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N22Click(TObject *Sender)
{
  //Redo last action
  countstages++;
  AnsiString nameofimage = "stage"+IntToStr(countstages)+".tiff";
  A = ExtractFilePath(OpenDialog1->FileName)+nameofimage;
  ShowMessage(B);
  if (FileExists(A))
    {
      ImagXpress7_1->FileName = A;
      Ix = ImagXpress7_1->IWidth;
      Iy = ImagXpress7_1->IHeight;
      ImagXpress7_1->ColorDepth(1,0,0);
      ImagXpress7_1->ColorDepth(8,1,0);
      ImagXpress7_1->SaveToBuffer = true;
      ImagXpress7_1->SaveFileType =  FT_TIFF;
      ImagXpress7_1->SaveFile();

      unsigned char *IMAGE;
      HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
      IMAGE = (unsigned char *)GlobalLock(hIM);
      long ln = GlobalSize(hIM);
      long offs=ln-(long)Ix*Iy;
      GlobalUnlock(hIM);
      ImagXpress7_1->LoadBuffer((long)IMAGE);
    }
  else
    {
      ShowMessage("No more steps to redo.");
      countstages--;
      return;
    }
}
//---------------------------------------------------------------------------
void TForm1::stagesSave()
{
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveTIFFCompression = TIFF_CCITTFAX4;
  ImagXpress7_1->SaveToBuffer = false;
  ImagXpress7_1->SaveMultiPage = false;
  countstages++;
  AnsiString nameofimage = "stage"+IntToStr(countstages)+".tiff";
  ImagXpress7_1->SaveFileName = ExtractFilePath(OpenDialog1->FileName)+nameofimage;
  ImagXpress7_1->SaveFile();

  B = ExtractFilePath(OpenDialog1->FileName)+nameofimage;
  ShowMessage(B);
  ImagXpress7_1->FileName = B;
  Ix = ImagXpress7_1->IWidth;
  Iy = ImagXpress7_1->IHeight;
  ImagXpress7_1->ColorDepth(1,0,0);
  ImagXpress7_1->ColorDepth(8,1,0);
  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);
  ImagXpress7_1->LoadBuffer((long)IMAGE);
}

void __fastcall TForm1::FMeasureAuto1Click(TObject *Sender)
{
  //F-Measure Valuation
  ImagXpress7_2->SaveToBuffer = true;
  ImagXpress7_2->SaveFileType =  FT_TIFF;
  ImagXpress7_2->SaveFile ();
  unsigned char *IMAGE1;
  HANDLE hIM1 = (HANDLE)ImagXpress7_2->SaveBufferHandle;
  IMAGE1 = (unsigned char *)GlobalLock(hIM1);
  long ln1 = GlobalSize(hIM1);
  long offs1=ln1-(long)Ix2*Iy2;
  GlobalUnlock(hIM1);

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();
  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  //F-Measure Calculation Start
  //IMAGE = image after proccessing
  //IMAGE1 = original image
  Screen->Cursor =  crHourGlass;
  Form5->ProgressBar1->Max = Ix-1;
  Form5->Show();
  float CTP=0,CFP=0,CFN=0; //Counter True Positive, Counter False Positive, Counter False Negative
  int error=0;
  float RC=0,PR=0,FM=0; //Anakthsh, Akribeia, F-Measure
  for (int x=1;x<Ix-1;x++)
  {
     Form5->ProgressBar1->Position = x;
     for (int y=1;y<Iy-1;y++)
     {
        if (x>Ix2 || y>Iy2)
        {
           ShowMessage("Οι εικόνες δεν έχουν το ίδιο μέγεθος...");
           error=1;
           Form5->Close();
           break;
        }
        if (*(IMAGE+y*Ix+x+offs)==0 && *(IMAGE1+y*Ix1+x+offs1)==0)CTP=CTP+1;
        if (*(IMAGE+y*Ix+x+offs)==0 && *(IMAGE1+y*Ix1+x+offs1)==255)CFP=CFP+1;
        if (*(IMAGE+y*Ix+x+offs)==255 && *(IMAGE1+y*Ix1+x+offs1)==0)CFN=CFN+1;
     }
     if (error)break;
  }
  if (!error)
  {
    Form5->Close();
    RC=CTP/(CFN+CTP);
    PR=CTP/(CFP+CTP);
    FM=((2*RC*PR/(RC+PR))*100); // FM=(((2*RC*PR)/(RC+PR))*100)/100;
    AnsiString str;
    str = FM;
    ShowMessage("Ομοιότητα : "+str+" %");
    Form1->Caption = "Βελτιώση Ποιότητας Εγγράφου : "+str+" %";
  }

  GlobalFree(IMAGE);
  GlobalFree(IMAGE1);
  Screen->Cursor =  crDefault;
          
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N20Click(TObject *Sender)
{
        //Processing Menu
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
        //File Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N23Click(TObject *Sender)
{
        //Tools Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N17Click(TObject *Sender)
{
        //Masks Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4x41Click(TObject *Sender)
{
        //Masks 4x4 Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8x81Click(TObject *Sender)
{
        //Mask 8x8 Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N18Click(TObject *Sender)
{
        //Mathematic Morphology Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Dilation2Click(TObject *Sender)
{
        //Dilation Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Erosion1Click(TObject *Sender)
{
        //Erosion Menu        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N19Click(TObject *Sender)
{
        //Filters Menu        
}
//---------------------------------------------------------------------------




