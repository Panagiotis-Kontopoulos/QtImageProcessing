//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ImagXpr7_OCX.h"
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TForm6 : public TForm
{
__published:	// IDE-managed Components
        TImagXpress7_ *ImagXpress7_1;
        TImagXpress7_ *ImagXpress7_2;
        TScrollBar *ScrollBar1;
        TLabel *Label1;
        TLabel *Label2;
        void __fastcall ScrollBar1Change(TObject *Sender);
private:	// User declarations
        void CalcProj();
        int Ix,Iy;

public:		// User declarations
        __fastcall TForm6(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm6 *Form6;
//---------------------------------------------------------------------------
#endif
