//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::SizeChange(TObject *Sender)
{
   AnsiString s = Size->Text;
   size = atoi(s.c_str());
   if (size%2==0)size++;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
   if (size==0 || threshold==0){
     size=3;
     threshold=4;
   }
   ShowMessage("Filter Size : "+IntToStr(size)+"\n\nThreshold : "+IntToStr(threshold));
   Form1->setFilterSize(size);
   Form1->setThreshold(threshold);
   Form2->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ValueChange(TObject *Sender)
{
   AnsiString v = Value->Text;
   threshold = atoi(v.c_str());
}
//---------------------------------------------------------------------------
