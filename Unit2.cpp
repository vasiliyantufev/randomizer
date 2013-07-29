//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button1Click(TObject *Sender)
{

if(Form1->Buffer.size() > 0)
{
	int result = MessageBoxA(NULL, "Сохранить в файл?", "Буфер содержит данные ", MB_YESNO );
	if (result == IDYES) { Form1->MenuItem5Click(Sender);}
}


if(OpenDialog1->Execute())
{
	long int size;

	if(RadioButton1->IsChecked)
	{
		size = Edit1->Text.ToInt();
	}

	if(RadioButton2->IsChecked)
	{
		size = Edit1->Text.ToInt() * 1024;
	}

	if(RadioButton3->IsChecked)
	{
		size = Edit1->Text.ToInt() * 1048576;
	}

	AnsiString ansiB(OpenDialog1->FileName);
	char * str  = new char[ansiB.Length()+1];
	strcpy(str, ansiB.c_str());


	int handle=open(str , O_CREAT|O_RDWR);
	if(handle!=-1)
	{
		chsize(handle,size);
		close(handle);
	}

	Form1->Buffer.clear();

	fileBufferRecord(str);

	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " Создание нового файла " + OpenDialog1->FileName + " размером " + size + " байт";
	Form1->ListBox1->Items->Add(st);
	Form2->Close();
}
}
//---------------------------------------------------------------------------
