//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormActivate(TObject *Sender)
{
	map <int, double>::iterator cur2;
	double math_o;
	double disp;
	math_o = mathOF();
	disp = disperF(math_o);

	Series1->Clear();
	for(cur2=Form1->probMap.begin(); cur2!=Form1->probMap.end(); cur2++)
	{
		Series1->AddXY((*cur2).first, (*cur2).second, "", 10);
	}


}
//---------------------------------------------------------------------------

