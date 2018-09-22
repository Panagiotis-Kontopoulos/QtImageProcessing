//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImagXpr7_OCX"
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::ImagXpress7_1Scroll(TObject *Sender, short Bar,
      short Action)
{
  ImagXpress7_2->ScrollX = ImagXpress7_1->ScrollX;
  ImagXpress7_2->ScrollY = ImagXpress7_1->ScrollY;
  ImagXpress7_2->Zoom(ImagXpress7_1->IPZoomF);

}
//---------------------------------------------------------------------------

void __fastcall TForm4::ImagXpress7_2Scroll(TObject *Sender, short Bar,
      short Action)
{
  ImagXpress7_1->ScrollX = ImagXpress7_2->ScrollX;
  ImagXpress7_1->ScrollY = ImagXpress7_2->ScrollY;
  ImagXpress7_1->Zoom(ImagXpress7_2->IPZoomF);
}
//---------------------------------------------------------------------------
