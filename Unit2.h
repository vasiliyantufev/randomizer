//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Types.hpp>
#include <FMX.Dialogs.hpp>
#include <fstream>
#include <fcntl.h>
#include <io.h>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TEdit *Edit1;
	TLabel *Label1;
	TButton *Button1;
	TOpenDialog *OpenDialog1;
	TRadioButton *RadioButton3;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
