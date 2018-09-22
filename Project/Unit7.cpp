//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------






void __fastcall TForm7::Button1Click(TObject *Sender)
{
   if (mean==0 || deviation==0){
     mean=3;
     deviation=10;
   }
   ShowMessage("Mean : "+FloatToStr(mean)+"\n\nDeviation : "+FloatToStr(deviation));
   Form1->setDeviation(deviation);
   Form1->setMean(mean);
   Form7->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit1Change(TObject *Sender)
{
   AnsiString s = Edit1->Text;
   mean = atof(s.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Edit2Change(TObject *Sender)
{
   AnsiString s = Edit2->Text;
   deviation = atof(s.c_str());
}
//---------------------------------------------------------------------------
