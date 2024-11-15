#include <dsound.h>
#include <vcl.h>

#pragma hdrstop

#include "main.h"
#include "dsufmod.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

/* Setup actual sampling rate in Hz. (48KHz is the default value.) */
#define uFMOD_MixRate 48000
TForm1 *Form1;

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner){
}

void __fastcall TForm1::Button1Click(TObject *Sender){
	if(!Edit1->Text.IsEmpty()){
		uFMOD_DSPlaySong(Edit1->Text.c_str(),0,XM_FILE,pidsb);
	}
}

bool __fastcall TForm1::InitDx(){
	IDirectSound *pids=NULL;
	WAVEFORMATEX pcm={0};
	DSBUFFERDESC dsbd={0};

	if(DirectSoundCreate(NULL,&pids,NULL)<0) return false;
	if(pids->SetCooperativeLevel(this->Handle,DSSCL_PRIORITY)<0) return false;

	pcm.wFormatTag=WAVE_FORMAT_PCM;
	pcm.nChannels=2;
	pcm.nSamplesPerSec=uFMOD_MixRate;
	pcm.nAvgBytesPerSec= uFMOD_MixRate * 4;
	pcm.nBlockAlign=4;
	pcm.wBitsPerSample=16;
	pcm.cbSize=0;

	dsbd.dwSize=20;
	dsbd.dwFlags=DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STICKYFOCUS;
	dsbd.dwBufferBytes=uFMOD_BUFFER_SIZE;
	dsbd.lpwfxFormat=&pcm;
	dsbd.guid3DAlgorithm=GUID_NULL;
	dsbd.dwReserved=0;

	if(pids->CreateSoundBuffer(&dsbd,&pidsb,NULL)<0) return false;
	return true;
}

void __fastcall TForm1::FormCreate(TObject *Sender){
	if(!InitDx()){
		MessageBox(this->Handle,"DirectSound error",0,MB_ICONSTOP | MB_SYSTEMMODAL);
		Application->Terminate();
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
