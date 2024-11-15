unit System;

interface

procedure _HandleFinally;

type
	TGUID = record
		D1: LongWord;
		D2: Word;
		D3: Word;
		D4: array [0..7] of Byte;
	end;

	PInitContext = ^TInitContext;
	TInitContext = record
		OuterContext:   PInitContext;
		ExcFrame:       Pointer;
		InitTable:      pointer;
		InitCount:      LongInt;
		Module:         pointer;
		DLLSaveEBP:     Pointer;
		DLLSaveEBX:     Pointer;
		DLLSaveESI:     Pointer;
		DLLSaveEDI:     Pointer;
		ExitProcessTLS: procedure;
		DLLInitState:   Byte;
	end;

	IUnknown = interface
		['{00000000-0000-0000-C000-000000000046}']
		function QueryInterface(const IID: TGUID; out Obj): Integer; stdcall;
		function _AddRef: Integer; stdcall;
		function _Release: Integer; stdcall;
	end;

procedure _IntfClear(var Dest: IUnknown);

implementation

procedure _HandleFinally;
asm
end;

procedure _IntfClear(var Dest: IUnknown);
asm
end;

end.