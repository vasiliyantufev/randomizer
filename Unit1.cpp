//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MenuItem2Click(TObject *Sender)		//create file
{
	Form2->ShowModal();
	if(Buffer.size() > 0)
	{
		MenuItem5->Visible = true;
		MenuItem6->Visible = true;
		MenuItem11->Visible = true;
		MenuItem15->Visible = true;
		MenuItem17->Visible = true;
		MenuItem20->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem3Click(TObject *Sender)		//open file
{

if(Buffer.size() > 0)
{
	int result = MessageBoxA(NULL, "��������� � ����?", "����� �������� ������ ", MB_YESNO );
	if (result == IDYES) { MenuItem5Click(Sender);}
}

if(OpenDialog1->Execute())
{
	AnsiString ansiB(OpenDialog1->FileName);
	char * str  = new char[ansiB.Length()+1];
	strcpy(str, ansiB.c_str());

	fileBufferRecord(str);

char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " �������� ������ ����� " + OpenDialog1->FileName;
ListBox1->Items->Add(st);
}

if(Buffer.size() > 0)
{
	MenuItem5->Visible = true;
	MenuItem6->Visible = true;
	MenuItem11->Visible = true;
	MenuItem15->Visible = true;
	MenuItem17->Visible = true;
	MenuItem20->Visible = true;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem5Click(TObject *Sender)		//save file
{
if(SaveDialog1->Execute())
{
	ofstream fileSave(SaveDialog1->FileName.c_str(), std::ios::out);
	for (int i=0; i<Buffer.size(); i++)
	{
		fileSave<<Buffer[i];
	}
char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " ���������� ����� " + SaveDialog1->FileName;
ListBox1->Items->Add(st);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem12Click(TObject *Sender)	//record audio
{
	WaveFormat.wFormatTag=WAVE_FORMAT_PCM;
	WaveFormat.nChannels=Channels;
	WaveFormat.nSamplesPerSec=FreqDis;
	WaveFormat.nBlockAlign=Channels;
	WaveFormat.nAvgBytesPerSec=FreqDis;
	WaveFormat.wBitsPerSample=BitPesSempl;
	WaveFormat.cbSize=0;

	audioBuffer=new char[Buffer.size()];
	audioBuffer[Buffer.size()-1] = 0;

	MMRESULT mmRes =waveInOpen(&hWaveIn, WAVE_MAPPER, &WaveFormat, 0L, 0L, CALLBACK_FUNCTION);
	if(mmRes != MMSYSERR_NOERROR)
	{
		wchar_t text[MAX_PATH];
		waveInGetErrorText( mmRes, text, MAX_PATH);
		ShowMessage(text);
	}
	DWORD audioBufSize;
	audioBufSize=Buffer.size();
	WaveHdrIn1.lpData=audioBuffer;
	WaveHdrIn1.dwBufferLength=audioBufSize;
	waveInPrepareHeader(hWaveIn,&WaveHdrIn1,sizeof(WAVEHDR));
	waveInAddBuffer(hWaveIn,&WaveHdrIn1,sizeof(WAVEHDR));
	waveInStart(hWaveIn);

	while(audioBuffer[Buffer.size()-1] == 0)
		Sleep(1000);
	waveInStop(hWaveIn);


for (int i=0; i<Buffer.size(); i++)
{
	Buffer[i] = Buffer[i] ^ audioBuffer[i];
}

char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " ��������� ��������� ����";
ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void fileBufferRecord(char * fileOpen)
{
	Form1->Buffer.clear();
	ifstream file(fileOpen, std::ios::in | std::ios::binary);

	if( !file )
	{
		ShowMessage("File open error!");
	}

	ifstream::pos_type size = 0;

	if( file.seekg(0, ios::end) )
	{
	   size = file.tellg();
	}

	if( size && file.seekg(0, ios::beg) )
	{
	   Form1->Buffer.resize(size);
	   file.read(&Form1->Buffer[0], size);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem14Click(TObject *Sender)	//�������� �������
{
double x,z;
int y;

CPBuffer=new char[Buffer.size()];
memset(CPBuffer,0,Buffer.size());
CPBuffer[100] = 0;
DWORD dwTimerHi, dwTimerLo;

for(int i=0; i<100; i++)
{
	asm
	{
		DW 0x310F
		mov dwTimerLo, EAX
		mov dwTimerHi, EDX
	}
	Sleep(50);
	asm
	{
		DW 0x310F
		sub EAX, dwTimerLo
		sub EDX, dwTimerHi
		mov dwTimerLo, EAX
		mov dwTimerHi, EDX
	}
	x = dwTimerLo/(1000.0*500);
	y = modf(x,&z) * 1000;
	CPBuffer[i] = y;
}

for (int i=0, j=0; i<Buffer.size(); i++,j++)
{
	if(j == sizeof(CPBuffer)-1) j=0;
	Buffer[i] = Buffer[i] ^ CPBuffer[j];
}

char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " ��������� �������� ������� ��";
ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem7Click(TObject *Sender)		//save log
{
if(SaveDialog2->Execute())
{
	ListBox1->Items->SaveToFile(SaveDialog2->FileName, TEncoding::UTF8);
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::MenuItem16Click(TObject *Sender)	//compression
{
char timeStr [9];
int size;
_strtime( timeStr );
String st;
st = (const char*)timeStr;
size = Buffer.size();
st += " ������ ������ " + SaveDialog1->FileName + " �� ������ " + size;
ListBox1->Items->Add(st);

for (int i=0; i<Buffer.size()-1; i++)
{
	if (Buffer[i] == Buffer[i+1])
	{
		Buffer.erase(Buffer.begin() + i);
	}
}
size = Buffer.size();
st = (const char*)timeStr;
st += " ������ ������ " + SaveDialog1->FileName + " ����� ������ " + size;
ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::MenuItem8Click(TObject *Sender)		//������� �� c++
{
if(SaveDialog3->Execute())
{
	ofstream fileSave(SaveDialog3->FileName.c_str(), std::ios::out);
	fileSave<<"#define RandomSize "<<Buffer.size()<<endl;
	fileSave<<"unsigned char RandomData[RandomSize] = {"<<endl;
	int count=0;
	for (int i=0; i<Buffer.size(); i++)
	{
		count++;
		char * return_value = new char[4];
		sprintf(return_value, "%x", Buffer[i]);

		if(i != Buffer.size()-1)
			fileSave<<"0x"<<return_value<<",";

		else
			fileSave<<"0x"<<return_value;

		if(count == 16)
		{
			fileSave<<" "<<endl;
			count=0;
		}

		if(i == Buffer.size()-1)
			fileSave<<endl<<"};";
	}
}

char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " ����� ������������� � ���� " + SaveDialog3->FileName + " ��� �������� �� C++";
ListBox1->Items->Add(st);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem10Click(TObject *Sender)		//������� �� ���������
{
if(SaveDialog5->Execute())
{

	ofstream fileSave(SaveDialog5->FileName.c_str(), std::ios::out);
	fileSave<<"RandomSize dd "<<Buffer.size()<<endl;
	fileSave<<"RandomData db ";
	int count=0;
	for (int i=0; i<Buffer.size(); i++)
	{
		count++;
		char * return_value = new char[4];
		sprintf(return_value, "%x", Buffer[i]);

		fileSave<<return_value<<" ";

		if(count == 16)
		{
			fileSave<<" "<<endl<<"           "<<"db ";
			count=0;
		}

		if(i == Buffer.size()-1)
			fileSave<<endl<<"           "<<"db ";
	}
}

char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " ����� ������������� � ���� " + SaveDialog5->FileName + " ��� �������� �� ���������";
ListBox1->Items->Add(st);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem9Click(TObject *Sender)		//������� �� python
{
if(SaveDialog4->Execute())
{
	ofstream fileSave(SaveDialog4->FileName.c_str(), std::ios::out);
	fileSave<<"RandomData = ["<<endl;
	int count=0;
	for (int i=0; i<Buffer.size(); i++)
	{
		count++;
		char * return_value = new char[4];
		sprintf(return_value, "%x", Buffer[i]);

		if(i != Buffer.size()-1)
			fileSave<<"0x"<<return_value<<",";

		else
			fileSave<<"0x"<<return_value;

		if(count == 16)
		{
			fileSave<<" "<<endl;
			count=0;
		}

		if(i == Buffer.size()-1)
			fileSave<<endl<<"]";
	}
}

char timeStr [9];
_strtime( timeStr );
String st;
st = (const char*)timeStr;
st += " ����� ������������� � ���� " + SaveDialog4->FileName + " ��� �������� �� Python";
ListBox1->Items->Add(st);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::MenuItem4Click(TObject *Sender)
{
	Form3->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem13Click(TObject *Sender)	//������ �����
{

if(OpenDialog1->Execute())
{
	AnsiString ansiB(OpenDialog1->FileName);
	char * str  = new char[ansiB.Length()+1];
	strcpy(str, ansiB.c_str());

	ifstream file(str, std::ios::in | std::ios::binary);

	if( !file )
	{
		ShowMessage("File open error!");
	}

	ifstream::pos_type size = 0;

	if( file.seekg(0, ios::end) )
	{
	   size = file.tellg();
	}

	if( size && file.seekg(0, ios::beg) )
	{
	   Form1->fileBuffer.resize(size);
	   file.read(&Form1->fileBuffer[0], size);
	}

	for (int i=0, j=0; i<Buffer.size(); i++,j++)
	{
		if(j == fileBuffer.size()-1) j=0;
		Buffer[i] = Buffer[i] ^ fileBuffer[j];
	}


	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " ������ ����� " + OpenDialog1->FileName;
	ListBox1->Items->Add(st);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " ������ ��������� ";
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::MenuItem21Click(TObject *Sender)   //����� ���������� ���������
{
	string str;
	string substr;
	string nstr;

	int num;
	int start = 0;
	int pos = 0;
	nstr = "00";

	for(int i=0; i<Buffer.size(); i++)
	{
		stringstream ss;
		num = pow((int)Buffer[i],2);

		ss << num;
		ss >> str;

		while(str.length()<4)
			str += "0";
		substr = str.substr(1,2);
		pos = substr.find(nstr, start);

		if(pos == 0)
			substr = str.substr(0,2);

		num = atoi (substr.c_str());
		Buffer[i]=num;
	}
	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " ���������. ����� ���������� ��������� ";
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem22Click(TObject *Sender)	//����� ������� ������������
{
	string str;
	string substr;
	string nstr;

	int num;
	int start = 0;
	int pos = 0;
	nstr = "00";

	for(int i=0; i<Buffer.size()-1; i++)
	{
		stringstream ss;
		num = (int)Buffer[i] * (int)Buffer[i+1];

		ss << num;
		ss >> str;

		while(str.length()<4)
			str += "0";

		substr = str.substr(1,2);
		pos = substr.find(nstr, start);

		if(pos == 0)
			substr = str.substr(0,2);

		num = atoi (substr.c_str());
		Buffer[i]=num;
	}
	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " ���������. ����� ���������� ������������ ";
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem23Click(TObject *Sender)	//����� �������������
{
	int R1, R2;
	R1 = R2 = 0;

	for(int i=0; i<Buffer.size(); i++)
	{
		R1 = (int)Buffer[i] << 4;
		R2 = (int)Buffer[i] >> 4;
		Buffer[i] = R1 + R2;
	}
	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " ���������. ����� ������������� ";
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem24Click(TObject *Sender)	//����� ���������
{
	int a, b;

	if(Buffer.size() <= 10)
		a = 2; b = 1;
	if(Buffer.size() <= 50 && Buffer.size() <= 10)
		a = 17; b = 5;
	if(Buffer.size() >= 50 && Buffer.size() <= 500)
		a = 55; b = 24;
	if(Buffer.size() >= 500)
		a = 97; b = 33;
  //

	for(int i=0; i<Buffer.size(); i++)
	{
		if(i-a < 0)
		{
			if(Buffer[i+a] >= Buffer[i+b])
				Buffer[i] = Buffer[i+a] - Buffer[i+b];

			if(Buffer[i+a] < Buffer[i+b])
				Buffer[i] = Buffer[i+a] - Buffer[i+b] + 1;
		}

		if(i-a > 0)
		{
			if(Buffer[i-a] >= Buffer[i-b])
				Buffer[i] = Buffer[i-a] - Buffer[i-b];

			if(Buffer[i-a] < Buffer[i-b])
				Buffer[i] = Buffer[i-a] - Buffer[i-b] + 1;
		}
	}
	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st += " ���������. ����� ��������� ";
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem18Click(TObject *Sender)	//���. ��������
{
	double math_o;
	math_o = mathOF();


	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st = st + " ���. �������� " + math_o;
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem19Click(TObject *Sender)	//���������
{
	double math_o;
	double disp;
	math_o = mathOF();
	disp = disperF(math_o);

	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st = st + " ��������� " + disp;
	ListBox1->Items->Add(st);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem25Click(TObject *Sender)	//c������������������� ����������
{
	double stDev;
	double math_o;
	double disp;
	math_o = mathOF();
	disp = disperF(math_o);
	stDev = sqrt(disp);

	char timeStr [9];
	_strtime( timeStr );
	String st;
	st = (const char*)timeStr;
	st = st + " �������������������� ���������� " + stDev;
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

double mathOF()		//���. ��������
{
	map <int, int>::iterator cur;
	map <int, double>::iterator cur2;
	int sizeBuf;
	double math_o;

	math_o = 0;
	sizeBuf = Form1->Buffer.size();
	Form1->raspMap.clear();
	Form1->probMap.clear();


	for(int i=0; i<sizeBuf; i++)	//������� ��������� ��������
	{
		Form1->raspMap[(int)Form1->Buffer[i]]++;
	}

	for(cur=Form1->raspMap.begin(); cur!=Form1->raspMap.end(); cur++)		//����������� ��������� ��������
	{
		Form1->probMap[(*cur).first] = (float)(*cur).second / sizeBuf;
	}

	for (cur2=Form1->probMap.begin(); cur2!=Form1->probMap.end(); cur2++)
	{
		math_o = math_o + (*cur2).first * (*cur2).second;
	}

	return math_o;
}
//---------------------------------------------------------------------------

double disperF(double math)		//���������
{
	int index;
	double disp;
	double math_o_pow;
	map <int, double>::iterator cur2;

	math_o_pow = 0;
	disp = 0;

	for (cur2=Form1->probMap.begin(); cur2!=Form1->probMap.end(); cur2++)
	{
		index = pow((*cur2).first,2);
		Form1->probMapPow[index] = (*cur2).second;
	}

	for (cur2=Form1->probMapPow.begin(); cur2!=Form1->probMapPow.end(); cur2++)	//���. �������� x(2)
	{
		math_o_pow = math_o_pow + (*cur2).first * (*cur2).second;
	}

	disp = math_o_pow - pow(math, 2);

	return disp;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem26Click(TObject *Sender)	//��������� ����
{
	map <int, double>::iterator cur2;
	char timeStr [9];
	String st;
	String st2;

	double math_o;
	math_o = mathOF();

	for (cur2=probMap.begin(); cur2!=probMap.end(); cur2++)
	{
		if((double)(*cur2).second > 0.1)
		{
			st2 = " ��������� ����: ���� �� ������� ";
			break;
		}
		else
			st2 = " ��������� ����: ���� ������� ";
	}

	_strtime( timeStr );
	st = (const char*)timeStr;
	st = st + st2;
	ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem27Click(TObject *Sender)	//�������� �����
{

int Size = Buffer.size();
int count = 3;
int countSeries = 0;
int Series = 0;

for(; count>1; count--)
{
	for(int i=0; i<Size; i++)
	{
		for(int j=i; j<i+count; ++j)
		{
			if(i+count-1>Size)          //�������� ������
			break;

			if(Buffer[i] == Buffer[j])	//�������� �������
			countSeries++;
		}
		if(countSeries==count)
		Series++;

		countSeries=0;
	 }
}

double test;
test = (double)Series/Size;

char timeStr [9];
String st;
String st2;

if(test > 0.1)
	st2 = " �������� �����: ���� �� ������� ";
else
	st2 = " �������� �����: ���� ������� ";

_strtime( timeStr );
st = (const char*)timeStr;
st = st + " " +  st2 + " " + Series;
ListBox1->Items->Add(st);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem28Click(TObject *Sender)
{
map <int, int>::iterator cur;
map <int, int> raspHee;
short max, min, deltay, n_rasp, count;
int Size;
int p;               	//��������� ����������� ���. ��������� �������� � ��������
double chiSquared;		//��-�������

n_rasp = 50;			//���. ����������
chiSquared = 0;
max=Buffer[0];
min=Buffer[0];
Size = Buffer.size();
chiSquared = 0;

//p = (double)1/Size;
p = Size/n_rasp;

for(int i=0; i<Size; i++)
{
	if (Buffer[i]>max) max=Buffer[i];
	if (Buffer[i]<min) min=Buffer[i];
}

deltay=(unsigned short)((max-min)/n_rasp+1);

for (int i=0; i<Size; i++)
{
	count=min;
	for (int j=0;j<n_rasp;j++)
	{
		if ((Buffer[i] >=count) && (Buffer[i]<=count+deltay))
		{
			raspHee[j]++;
			break;
		}
	count=count+deltay;
	}
}


for(cur=raspHee.begin(); cur!=raspHee.end(); cur++)
{
	if((*cur).second > p)
		chiSquared += (*cur).second - p;

	if((*cur).second < p)
		chiSquared += p - (*cur).second;
}
// 	chiSquared += (*cur).second;// - p * Size;

//chiSquared += (pow((*cur).second, 2) / p) - Size;
//chiSquared = chiSquared / Size;

double test;
test = (double)chiSquared/Size;

char timeStr [9];
String st;
String st2;

if(test > 0.2)
	st2 = " ��������  ������������ �������������: ���� �� ������� ";
else
	st2 = " ��������  ������������ �������������: ���� ������� ";

_strtime( timeStr );
st = (const char*)timeStr;
st = st + " " +  st2 + " " + test;
ListBox1->Items->Add(st);

/*
	char timeStr [9];
	String st;
	_strtime( timeStr );
	st = (const char*)timeStr;
	st = st + " ��-������� = "+chiSquared+" "+Size + " " + chiSquared / Size;
	ListBox1->Items->Add(st);
*/
}
//---------------------------------------------------------------------------

