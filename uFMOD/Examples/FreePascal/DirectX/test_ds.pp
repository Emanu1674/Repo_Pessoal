program test_ds;

{$APPTYPE GUI}

uses DirectSound, DSuFMOD;

const

{ Set to actual sampling rate in Hz. (48KHz for default uFMOD build.) }
uFMOD_MixRate = 48000;

{ The following XM dump was generated using the Eff utility.
  uFMOD supports file and resource playback as well. }
xm : array[1..905] of Byte = (
	$45,$78,$74,$65,$6E,$64,$65,$64,$20,$4D,$6F,$64,$75,$6C,$65,$3A,
	$20,$73,$6F,$66,$74,$20,$6D,$61,$6E,$69,$61,$63,$2D,$6D,$69,$6E,
	$69,$6D,$61,$6C,$00,$1A,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$04,$01,$34,$00,$00,$00,
	$20,$00,$00,$00,$02,$00,$0D,$00,$01,$00,$01,$00,$0A,$00,$91,$00,
	$00,$01,$02,$03,$04,$05,$06,$07,$00,$01,$02,$03,$04,$05,$06,$07,
	$08,$09,$0A,$0B,$08,$09,$0C,$0B,$08,$09,$0A,$0B,$08,$09,$0C,$0B,
	$09,$00,$00,$00,$00,$04,$00,$01,$00,$83,$16,$01,$80,$80,$2E,$01,
	$00,$0E,$60,$80,$3A,$01,$00,$0E,$62,$81,$61,$83,$35,$01,$09,$00,
	$00,$00,$00,$04,$00,$01,$00,$83,$16,$01,$80,$80,$2E,$01,$00,$0E,
	$60,$80,$35,$01,$00,$0E,$62,$81,$61,$83,$38,$01,$09,$00,$00,$00,
	$00,$04,$00,$01,$00,$83,$16,$01,$80,$80,$2E,$01,$00,$0E,$60,$80,
	$38,$01,$00,$0E,$62,$80,$83,$33,$01,$09,$00,$00,$00,$00,$06,$00,
	$01,$00,$83,$16,$01,$80,$80,$2E,$01,$00,$0E,$60,$80,$33,$01,$00,
	$0E,$61,$81,$61,$83,$35,$01,$83,$0D,$01,$83,$36,$01,$80,$83,$36,
	$01,$09,$00,$00,$00,$00,$04,$00,$01,$00,$83,$0F,$01,$80,$80,$2E,
	$01,$00,$0E,$60,$80,$36,$01,$00,$0E,$62,$81,$61,$83,$33,$01,$09,
	$00,$00,$00,$00,$06,$00,$01,$00,$83,$0F,$01,$80,$80,$2E,$01,$00,
	$0E,$60,$80,$33,$01,$00,$0E,$61,$81,$61,$83,$2E,$01,$83,$12,$01,
	$83,$33,$01,$80,$83,$35,$01,$09,$00,$00,$00,$00,$06,$00,$01,$00,
	$83,$16,$01,$80,$80,$2E,$01,$00,$0E,$60,$80,$35,$01,$00,$0E,$61,
	$81,$61,$83,$2E,$01,$83,$0D,$01,$83,$31,$01,$80,$83,$2E,$01,$09,
	$00,$00,$00,$00,$08,$00,$01,$00,$83,$12,$01,$98,$0A,$01,$83,$19,
	$01,$88,$0A,$83,$1E,$01,$81,$61,$83,$12,$01,$80,$83,$14,$01,$80,
	$83,$1B,$01,$80,$83,$20,$01,$80,$83,$14,$01,$80,$09,$00,$00,$00,
	$00,$08,$00,$01,$00,$83,$12,$01,$81,$61,$83,$19,$01,$80,$83,$1E,
	$01,$80,$83,$12,$01,$80,$83,$19,$01,$83,$31,$01,$83,$1E,$01,$80,
	$83,$12,$01,$83,$31,$01,$83,$19,$01,$80,$09,$00,$00,$00,$00,$08,
	$00,$01,$00,$83,$14,$01,$83,$33,$01,$83,$1B,$01,$80,$83,$20,$01,
	$83,$31,$01,$83,$14,$01,$80,$83,$1B,$01,$83,$30,$01,$83,$20,$01,
	$80,$83,$14,$01,$83,$31,$01,$83,$1B,$01,$80,$09,$00,$00,$00,$00,
	$08,$00,$01,$00,$83,$16,$01,$83,$30,$01,$83,$1D,$01,$83,$31,$01,
	$83,$22,$01,$83,$35,$01,$83,$16,$01,$98,$0A,$01,$83,$1D,$01,$88,
	$0A,$83,$22,$01,$81,$61,$83,$16,$01,$80,$83,$1D,$01,$80,$09,$00,
	$00,$00,$00,$08,$00,$01,$00,$83,$16,$01,$80,$83,$1D,$01,$80,$83,
	$22,$01,$80,$83,$16,$01,$80,$83,$18,$01,$80,$83,$1D,$01,$80,$83,
	$11,$01,$80,$83,$18,$01,$80,$09,$00,$00,$00,$00,$08,$00,$01,$00,
	$83,$16,$01,$83,$30,$01,$83,$1D,$01,$83,$31,$01,$83,$19,$01,$83,
	$2E,$01,$83,$16,$01,$98,$0A,$01,$83,$1D,$01,$88,$0A,$83,$19,$01,
	$81,$61,$83,$16,$01,$80,$83,$1D,$01,$80,$F1,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$27,$01,$00,$12,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$40,$00,$08,
	$00,$2C,$00,$0E,$00,$08,$00,$18,$00,$16,$00,$20,$00,$08,$00,$2D,
	$00,$0D,$00,$32,$00,$04,$00,$3C,$00,$07,$00,$44,$00,$04,$00,$5A,
	$00,$00,$00,$64,$00,$00,$00,$6E,$00,$00,$00,$00,$00,$20,$00,$0A,
	$00,$28,$00,$1E,$00,$18,$00,$32,$00,$20,$00,$3C,$00,$20,$00,$46,
	$00,$20,$00,$50,$00,$20,$00,$5A,$00,$20,$00,$64,$00,$20,$00,$6E,
	$00,$20,$00,$78,$00,$20,$00,$82,$00,$20,$00,$09,$06,$01,$02,$04,
	$02,$03,$05,$01,$00,$00,$00,$00,$00,$80,$00,$0C,$00,$00,$00,$00,
	$00,$00,$00,$0C,$00,$00,$00,$40,$00,$01,$80,$F9,$00,$BF,$00,$C3,
	$00,$0A,$00,$57,$00,$6E,$00,$23,$00
);

procedure MessageBox(hWnd:LongInt;lpText,lpCaption:AnsiString;uType:LongWord); stdcall; external 'user32.dll' name 'MessageBoxA';
function GetForegroundWindow:LongInt; stdcall; external 'user32.dll';
function DirectSoundCreate(lpGuid:REFGUID;ppDS:PDirectSound;pUnkOuter:Pointer):LongInt; stdcall; external 'dsound.dll' index 1;

var
	{ The actual order of the following 2 interface instances is relevant!
	  Any sound buffer instances should precede the main direct-sound instance,
	  because FreePascal automatically releases all of them in the same order.
	  An attempt to release a sound buffer after the main instance whould
	  throw an access violation exception. So, make sure IDirectSound is always
	  the last one! }
	pidsb: IDirectSoundBuffer;
	pids: IDirectSound;

	pcm: TWAVEFORMATEX;  { Wave format descriptor }
	dsbd: TDSBufferDesc; { DirectSound buffer descriptor }
	noerror: Byte = 0;

begin

	{ Request an instance of IDirectSound. }
	if DirectSoundCreate(nil, @pids, nil) >= 0 then
	begin
		{ It is important to set the cooperative level to at least
		  DSSCL_PRIORITY prior to creating 16-bit stereo buffers. }
		if pids.SetCooperativeLevel(GetForegroundWindow, DSSCL_PRIORITY) >= 0 then
		begin
			with pcm do
			begin
				wFormatTag      := 1; { PCM }
				nChannels       := 2; { stereo }
				nSamplesPerSec  := uFMOD_MixRate;
				nAvgBytesPerSec := uFMOD_MixRate * 4;
				nBlockAlign     := 4;
				wBitsPerSample  := 16;
				cbSize          := 0
			end;
			with dsbd do
			begin
				dwSize        := DSBUFFERDESC1;
				dwFlags       := DSBCAPS_STICKYFOCUS Or DSBCAPS_GETCURRENTPOSITION2;
				dwBufferBytes := uFMOD_BUFFER_SIZE;
				lpwfxFormat   := @pcm
			end;
			{ Create a secondary sound buffer. }
			if pids.CreateSoundBuffer(@dsbd, @pidsb, nil) >= 0 then
			begin
				{ Start playback. }
				if uFMOD_DSPlaySong(@xm, Length(xm), XM_MEMORY, pidsb) >= 0 then
				begin
					{ Wait for user input. }
					MessageBox(0, 'uFMOD ruleZ!', 'FreePascal', 0);
					{ Stop playback. }
					uFMOD_StopSong;
					noerror := 1
				end;
			end;
		end;
	end;
	{ Signal an error condition. }
	if noerror = 0 then MessageBox(0, 'DirectX error', '', $10);
end.