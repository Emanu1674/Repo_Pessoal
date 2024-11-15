#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "ufmod.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
	
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner){
}

void __fastcall TForm1::Button1Click(TObject *Sender){
	if(!Edit1->Text.IsEmpty()){
		uFMOD_PlaySong(Edit1->Text.c_str(),0,XM_FILE);
	}
}

void __fastcall TForm1::Button2Click(TObject *Sender){
	uFMOD_StopSong();
}

void __fastcall TForm1::Button3Click(TObject *Sender){
	TOpenDialog *od=new TOpenDialog(this);
	od->Execute();
	Edit1->Text=od->FileName;
	delete od;
}

void __fastcall TForm1::Edit1Click(TObject *Sender){
	Edit1->Text="";
}
