//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TEdit *Size;
        TLabel *Label1;
        TLabel *Threshold;
        TEdit *Value;
        TButton *Button1;
        void __fastcall SizeChange(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ValueChange(TObject *Sender);
private:	// User declarations
        int size,threshold;
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
 