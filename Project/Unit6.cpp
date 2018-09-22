//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImagXpr7_OCX"
#pragma resource "*.dfm"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm6::ScrollBar1Change(TObject *Sender)
{

  float r = (float)ScrollBar1->Position/10;
  ImagXpress7_1->hDIB = Form1->ImagXpress7_1->CopyDIB();
  Ix = ImagXpress7_1->IWidth;
  Iy = ImagXpress7_1->IHeight;
  ImagXpress7_1->Resize(Ix/2,Iy/2);

  ImagXpress7_1->Rotate(r);
  ImagXpress7_1->ZoomToFit(2);

  Ix = ImagXpress7_1->IWidth;
  Iy = ImagXpress7_1->IHeight;

  Label1->Caption = r;

  CalcProj();
}
//---------------------------------------------------------------------------

void TForm6::CalcProj()
{
   ImagXpress7_2->CreateDIB(Ix,Iy,24,clWhite);


  //Save ImagXpress1 to pointer
  ImagXpress7_1->ColorDepth(8,2,0);

  ImagXpress7_1->SaveToBuffer = true;
  ImagXpress7_1->SaveFileType =  FT_TIFF;
  ImagXpress7_1->SaveFile ();

  unsigned char *IMAGE;
  HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
  IMAGE = (unsigned char *)GlobalLock(hIM);
  long ln = GlobalSize(hIM);
  long offs=ln-(long)Ix*Iy;
  GlobalUnlock(hIM);

  //Save ImagXpress2 to pointer
  ImagXpress7_2->ColorDepth(8,2,0);

  ImagXpress7_2->SaveToBuffer = true;
  ImagXpress7_2->SaveFileType =  FT_TIFF;
  ImagXpress7_2->SaveFile ();

  unsigned char *IMAGE2;
  HANDLE hIM2 = (HANDLE)ImagXpress7_2->SaveBufferHandle;
  IMAGE2 = (unsigned char *)GlobalLock(hIM2);
  long ln2 = GlobalSize(hIM2);
  long offs2=ln2-(long)Ix*Iy;
  GlobalUnlock(hIM2);

   float E=0;

   for (int y=0;y<Iy;y++)
   {
     int c=0;

     for (int x=0;x<Ix;x++)
     if (*(IMAGE+y*Ix+x+offs)==0) c++;

     E=E+c*c;

     for (int x=0;x<=c;x++)
     *(IMAGE2+y*Ix+x+offs2)=0;
   }

   Label2->Caption = E/100000000;

   ImagXpress7_2->ZoomToFit(2);
   ImagXpress7_2->DIBUpdate();

   ImagXpress7_2->LoadBuffer((long)IMAGE2);

   GlobalFree(IMAGE);
   GlobalFree(IMAGE2);




}
