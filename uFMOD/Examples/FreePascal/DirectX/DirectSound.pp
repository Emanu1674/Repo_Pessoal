{ *** DirectX DirectSound for FreePascal *** }

{$MODE OBJFPC}

unit DirectSound;

interface

type

	D3DVALUE = Single;
	PD3DVALUE = ^Single;
	D3DCOLOR = LongWord;
	PD3DCOLOR = ^LongWord;
	PDWORD = ^LongWord;
	PLONG = ^LongInt;
	REFERENCE_TIME = Int64;
	PREFERENCE_TIME = ^Int64;
	REFGUID = ^TGUID;

	PD3DVECTOR = ^TD3DVECTOR;
	TD3DVECTOR = packed record
		x,y,z:Single
	end;

	PWAVEFORMATEX = ^TWAVEFORMATEX;
	TWAVEFORMATEX = packed record
		wFormatTag,nChannels:Word;
		nSamplesPerSec,nAvgBytesPerSec:LongWord;
		nBlockAlign,wBitsPerSample,cbSize:Word
	end;

	PWAVEFORMATEXTENSIBLE = ^TWAVEFORMATEXTENSIBLE;
	TWAVEFORMATEXTENSIBLE = packed record
		wFormatTag,nChannels:Word;
		nSamplesPerSec,nAvgBytesPerSec:LongWord;
		nBlockAlign,wBitsPerSample,cbSize,wValidBitsPerSample,wSamplesPerBlock,wReserved:Word;
		dwChannelMask,SubFormat:LongWord
	end;

	PDSCAPS = ^TDSCAPS;
	TDSCAPS = packed record
		dwSize,dwFlags,dwMinSecondarySampleRate,dwMaxSecondarySampleRate,dwPrimaryBuffers,dwMaxHwMixingAllBuffers:LongWord;
		dwMaxHwMixingStaticBuffers,dwMaxHwMixingStreamingBuffers,dwFreeHwMixingAllBuffers,dwFreeHwMixingStaticBuffers:LongWord;
		dwFreeHwMixingStreamingBuffers,dwMaxHw3DAllBuffers,dwMaxHw3DStaticBuffers,dwMaxHw3DStreamingBuffers:LongWord;
		dwFreeHw3DAllBuffers,dwFreeHw3DStaticBuffers,dwFreeHw3DStreamingBuffers,dwTotalHwMemBytes,dwFreeHwMemBytes:LongWord;
		dwMaxContigFreeHwMemBytes,dwUnlockTransferRateHwBuffers,dwPlayCpuOverheadSwBuffers,dwReserved1,dwReserved2:LongWord
	end;

	PDSBCAPS = ^TDSBCAPS;
	TDSBCAPS = packed record
		dwSize,dwFlags,dwBufferBytes,dwUnlockTransferRate,dwPlayCpuOverhead:LongWord
	end;

	PDSBUFFERDESC = ^TDSBUFFERDESC;
	TDSBUFFERDESC = packed record
		dwSize,dwFlags,dwBufferBytes,dwReserved:LongWord;
		lpwfxFormat:PWAVEFORMATEX;
		guid3DAlgorithm:TGUID
	end;

	PDS3DBUFFER = ^TDS3DBUFFER;
	TDS3DBUFFER = packed record
		dwSize:LongWord;
		vPosition,vVelocity:TD3DVECTOR;
		dwInsideConeAngle,dwOutsideConeAngle:LongWord;
		vConeOrientation:TD3DVECTOR;
		lConeOutsideVolume:LongInt;
		flMinDistance,flMaxDistance:Single;
		dwMode:LongWord
	end;

	PDS3DLISTENER = ^TDS3DLISTENER;
	TDS3DLISTENER = packed record
		dwSize:LongWord;
		vPosition,vVelocity,vOrientFront,vOrientTop:TD3DVECTOR;
		flDistanceFactor,flRolloffFactor,flDopplerFactor:Single
	end;

	PDSCCAPS = ^TDSCCAPS;
	TDSCCAPS = packed record
		dwSize,dwFlags,dwFormats,dwChannels:LongWord
	end;

	PDSCEFFECTDESC = ^TDSCEFFECTDESC;
	TDSCEFFECTDESC = packed record
		dwSize,dwFlags:LongWord;
		guidDSCFXClass,guidDSCFXInstance:TGUID;
		dwReserved1,dwReserved2:LongWord
	end;

	PDSCBUFFERDESC = ^TDSCBUFFERDESC;
	TDSCBUFFERDESC = packed record
		dwSize,dwFlags,dwBufferBytes,dwReserved:LongWord;
		lpwfxFormat:PWAVEFORMATEX;
		dwFXCount:LongWord;
		lpDSCFXDesc:PDSCEFFECTDESC
	end;

	PDSCBCAPS = ^TDSCBCAPS;
	TDSCBCAPS = packed record
		dwSize,dwFlags,dwBufferBytes,dwReserved:LongWord
	end;

	PDSBPOSITIONNOTIFY = ^TDSBPOSITIONNOTIFY;
	TDSBPOSITIONNOTIFY = packed record
		dwOffset,hEventNotify:LongWord
	end;

	PDSCFXAec = ^TDSCFXAec;
	TDSCFXAec = packed record
		fEnable,fReset:LongInt
	end;

	PDSCFXNoiseSuppress = ^TDSCFXNoiseSuppress;
	TDSCFXNoiseSuppress = packed record
		fEnable,fReset:LongInt
	end;

	PDSEFFECTDESC = ^TDSEFFECTDESC;
	TDSEFFECTDESC = packed record
		dwSize,dwFlags:LongWord;
		guidDSFXClass:TGUID;
		dwReserved1,dwReserved2:LongWord
	end;

	PDSFXChorus = ^TDSFXChorus;
	TDSFXChorus = packed record
		fWetDryMix,fDepth,fFeedback,fFrequency:Single;
		lWaveform:LongInt;
		fDelay:Single;
		lPhase:LongInt
	end;

	PDSFXCompressor = ^TDSFXCompressor;
	TDSFXCompressor = packed record
		fGain,fAttack,fRelease,fThreshold,fRatio,fPredelay:Single
	end;

	PDSFXDistortion = ^TDSFXDistortion;
	TDSFXDistortion = packed record
		fGain,fEdge,fPostEQCenterFrequency,fPostEQBandwidth,fPreLowpassCutoff:Single
	end;

	PDSFXEcho = ^TDSFXEcho;
	TDSFXEcho = packed record
		fWetDryMix,fFeedback,fLeftDelay,fRightDelay:Single;
		lPanDelay:LongInt
	end;

	PDSFXFlanger = ^TDSFXFlanger;
	TDSFXFlanger = packed record
		fWetDryMix,fDepth,fFeedback,fFrequency:Single;
		lWaveform:LongInt;
		fDelay:Single;
		lPhase:LongInt
	end;

	PDSFXGargle = ^TDSFXGargle;
	TDSFXGargle = packed record
		dwRateHz,dwWaveShape:LongWord
	end;

	PDSFXI3DL2Reverb = ^TDSFXI3DL2Reverb;
	TDSFXI3DL2Reverb = packed record
		lRoom,lRoomHF:LongInt;
		flRoomRolloffFactor,flDecayTime,flDecayHFRatio:Single;
		lReflections:LongInt;
		flReflectionsDelay:Single;
		lReverb:LongInt;
		flReverbDelay,flDiffusion,flDensity,flHFReference:Single
	end;

	PDSFXParamEq = ^TDSFXParamEq;
	TDSFXParamEq = packed record
		fCenter,fBandwidth,fGain:Single
	end;

	PDSFXWavesReverb = ^TDSFXWavesReverb;
	TDSFXWavesReverb = packed record
		fInGain,fReverbMix,fReverbTime,fHighFreqRTRatio:Single
	end;

	IReferenceClock = interface (IUnknown)
		['{56A86897-0AD4-11CE-B03A-0020AF0BA770}']
		function GetTime(pTime:PREFERENCE_TIME):LongInt; stdcall;
		function AdviseTime(rtBaseTime,rtStreamTime:REFERENCE_TIME;hEvent:LongWord;pdwAdviseCookie:PDWORD):LongInt; stdcall;
		function AdvisePeriodic(rtStartTime,rtPeriodTime:REFERENCE_TIME;hSemaphore:LongWord;pdwAdviseCookie:PDWORD):LongInt; stdcall;
		function Unadvise(dwAdviseCookie:LongWord):LongInt; stdcall;
	end;

	IDirectSoundBuffer = interface;
	PDirectSoundBuffer = ^IDirectSoundBuffer;

	IDirectSound = interface (IUnknown)
		['{279AFA83-4981-11CE-A521-0020AF0BE560}']
		function CreateSoundBuffer(lpDSBufferDesc:PDSBUFFERDESC;lpIDirectSoundBuffer:PDirectSoundBuffer;pUnkOuter:Pointer):LongInt; stdcall;
		function GetCaps(lpDSCaps:PDSCAPS):LongInt; stdcall;
		function DuplicateSoundBuffer(lpDsbOriginal:IDirectSoundBuffer;lpDsbDuplicate:PDirectSoundBuffer):LongInt; stdcall;
		function SetCooperativeLevel(hwnd,dwLevel:LongWord):LongInt; stdcall;
		function Compact:LongInt; stdcall;
		function GetSpeakerConfig(lpdwSpeakerConfig:PDWORD):LongInt; stdcall;
		function SetSpeakerConfig(dwSpeakerConfig:LongWord):LongInt; stdcall;
		function Initialize(lpGuid:REFGUID):LongInt; stdcall;
	end;
	PDirectSound = ^IDirectSound;

	IDirectSoundBuffer8 = interface;
	PDirectSoundBuffer8 = ^IDirectSoundBuffer8;

	IDirectSound8 = interface (IUnknown)
		['{C50A7E93-F395-4834-9EF6-7FA99DE50966}']
		function CreateSoundBuffer(lpDSBufferDesc:PDSBUFFERDESC;lpIDirectSoundBuffer:PDirectSoundBuffer8;pUnkOuter:Pointer):LongInt; stdcall;
		function GetCaps(lpDSCaps:PDSCAPS):LongInt; stdcall;
		function DuplicateSoundBuffer(lpDsbOriginal:IDirectSoundBuffer8;lpDsbDuplicate:PDirectSoundBuffer8):LongInt; stdcall;
		function SetCooperativeLevel(hwnd,dwLevel:LongWord):LongInt; stdcall;
		function Compact:LongInt; stdcall;
		function GetSpeakerConfig(lpdwSpeakerConfig:PDWORD):LongInt; stdcall;
		function SetSpeakerConfig(dwSpeakerConfig:LongWord):LongInt; stdcall;
		function Initialize(lpGuid:REFGUID):LongInt; stdcall;
		function VerifyCertification(pdwCertified:PDWORD):LongInt; stdcall;
	end;
	PDirectSound8 = ^IDirectSound8;

	IDirectSoundBuffer = interface (IUnknown)
		['{279AFA85-4981-11CE-A521-0020AF0BE560}']
		function GetCaps(lpDSCaps:PDSBCAPS):LongInt; stdcall;
		function GetCurrentPosition(lpdwCapturePosition,lpdwReadPosition:PDWORD):LongInt; stdcall;
		function GetFormat(lpwfxFormat:PWAVEFORMATEX;dwSizeAllocated:LongWord;lpdwSizeWritten:PDWORD):LongInt; stdcall;
		function GetVolume(lplVolume:PLONG):LongInt; stdcall;
		function GetPan(lplPan:PLONG):LongInt; stdcall;
		function GetFrequency(lpdwFrequency:PDWORD):LongInt; stdcall;
		function GetStatus(lpdwStatus:PDWORD):LongInt; stdcall;
		function Initialize(lpDirectSound:IDirectSound;lpcDSBufferDesc:PDSBufferDesc):LongInt; stdcall;
		function Lock(dwWriteCursor,dwWriteBytes:LongWord;lplpvAudioPtr1:Pointer;lpdwAudioBytes1:PDWORD;lplpvAudioPtr2:Pointer;lpdwAudioBytes2:PDWORD;dwFlags:LongWord):LongInt; stdcall;
		function Play(dwReserved1,dwReserved2,dwFlags:LongWord):LongInt; stdcall;
		function SetCurrentPosition(dwPosition:LongWord):LongInt; stdcall;
		function SetFormat(lpcfxFormat:PWAVEFORMATEX):LongInt; stdcall;
		function SetVolume(lVolume:LongInt):LongInt; stdcall;
		function SetPan(lPan:LongInt):LongInt; stdcall;
		function SetFrequency(dwFrequency:LongWord):LongInt; stdcall;
		function Stop:LongInt; stdcall;
		function Unlock(lpvAudioPtr1:PDWORD;dwAudioBytes1:LongWord;lpvAudioPtr2:PDWORD;dwAudioBytes2:LongWord):LongInt; stdcall;
		function Restore:LongInt; stdcall;
	end;

	IDirectSoundBuffer8 = interface (IUnknown)
		['{6825A449-7524-4D82-920F-50E36AB3AB1E}']
		function GetCaps(lpDSCaps:PDSBCAPS):LongInt; stdcall;
		function GetCurrentPosition(lpdwCapturePosition,lpdwReadPosition:PDWORD):LongInt; stdcall;
		function GetFormat(lpwfxFormat:PWAVEFORMATEX;dwSizeAllocated:LongWord;lpdwSizeWritten:PDWORD):LongInt; stdcall;
		function GetVolume(lplVolume:PLONG):LongInt; stdcall;
		function GetPan(lplPan:PLONG):LongInt; stdcall;
		function GetFrequency(lpdwFrequency:PDWORD):LongInt; stdcall;
		function GetStatus(lpdwStatus:PDWORD):LongInt; stdcall;
		function Initialize(lpDirectSound:IDirectSound8;lpcDSBufferDesc:PDSBufferDesc):LongInt; stdcall;
		function Lock(dwWriteCursor,dwWriteBytes:LongWord;lplpvAudioPtr1:Pointer;lpdwAudioBytes1:PDWORD;lplpvAudioPtr2:Pointer;lpdwAudioBytes2:PDWORD;dwFlags:LongWord):LongInt; stdcall;
		function Play(dwReserved1,dwReserved2,dwFlags:LongWord):LongInt; stdcall;
		function SetCurrentPosition(dwPosition:LongWord):LongInt; stdcall;
		function SetFormat(lpcfxFormat:PWAVEFORMATEX):LongInt; stdcall;
		function SetVolume(lVolume:LongInt):LongInt; stdcall;
		function SetPan(lPan:LongInt):LongInt; stdcall;
		function SetFrequency(dwFrequency:LongWord):LongInt; stdcall;
		function Stop:LongInt; stdcall;
		function Unlock(lpvAudioPtr1:Pointer;dwAudioBytes1:LongWord;lpvAudioPtr2:Pointer;dwAudioBytes2:LongWord):LongInt; stdcall;
		function Restore:LongInt; stdcall;
		function SetFX(dwEffectsCount:LongWord;pDSFXDesc:PDSEFFECTDESC;pdwResultCodes:PDWORD):LongInt; stdcall;
		function AcquireResources(dwFlags,dwEffectsCount:LongWord;pdwResultCodes:PDWORD):LongInt; stdcall;
		function GetObjectInPath(rguidObject:REFGUID;dwIndex:LongWord;rguidInterface:REFGUID;ppObject:Pointer):LongInt; stdcall;
	end;

	IDirectSound3DListener = interface (IUnknown)
		['{279AFA84-4981-11CE-A521-0020AF0BE560}']
		function GetAllParameters(pListener:PDS3DLISTENER):LongInt; stdcall;
		function GetDistanceFactor(pflDistanceFactor:PD3DVALUE):LongInt; stdcall;
		function GetDopplerFactor(pflDopplerFactor:PD3DVALUE):LongInt; stdcall;
		function GetOrientation(pvOrientFront,pvOrientTop:PD3DVECTOR):LongInt; stdcall;
		function GetPosition(pvPosition:PD3DVECTOR):LongInt; stdcall;
		function GetRolloffFactor(pflRolloffFactor:PD3DVALUE):LongInt; stdcall;
		function GetVelocity(pvVelocity:PD3DVECTOR):LongInt; stdcall;
		function SetAllParameters(pcListener:PDS3DLISTENER;dwApply:LongWord):LongInt; stdcall;
		function SetDistanceFactor(flDistanceFactor:Single;dwApply:LongWord):LongInt; stdcall;
		function SetDopplerFactor(flDopplerFactor:Single;dwApply:LongWord):LongInt; stdcall;
		function SetOrientation(xFront,yFront,zFront,xTop,yTop,zTop:Single;dwApply:LongWord):LongInt; stdcall;
		function SetPosition(x,y,z:Single;dwApply:LongWord):LongInt; stdcall;
		function SetRolloffFactor(flRolloffFactor:Single;dwApply:LongWord):LongInt; stdcall;
		function SetVelocity(x,y,z:Single;dwApply:LongWord):LongInt; stdcall;
		function CommitDeferredSettings:LongInt; stdcall;
	end;
	PDirectSound3DListener = ^IDirectSound3DListener;

	IDirectSound3DBuffer = interface (IUnknown)
		['{279AFA86-4981-11CE-A521-0020AF0BE560}']
		function GetAllParameters(pDs3dBuffer:PDS3DBUFFER):LongInt; stdcall;
		function GetConeAngles(pdwInsideConeAngle,pdwOutsideConeAngle:PDWORD):LongInt; stdcall;
		function GetConeOrientation(pvOrientation:PD3DVECTOR):LongInt; stdcall;
		function GetConeOutsideVolume(plConeOutsideVolume:PLONG):LongInt; stdcall;
		function GetMaxDistance(pflMaxDistance:PD3DVALUE):LongInt; stdcall;
		function GetMinDistance(pflMinDistance:PD3DVALUE):LongInt; stdcall;
		function GetMode(pdwMode:PDWORD):LongInt; stdcall;
		function GetPosition(pvPosition:PD3DVECTOR):LongInt; stdcall;
		function GetVelocity(pvVelocity:PD3DVECTOR):LongInt; stdcall;
		function SetAllParameters(pcDs3dBuffer:PDS3DBUFFER;dwApply:LongWord):LongInt; stdcall;
		function SetConeAngles(dwInsideConeAngle,dwOutsideConeAngle,dwApply:LongWord):LongInt; stdcall;
		function SetConeOrientation(x,y,z:Single;dwApply:LongWord):LongInt; stdcall;
		function SetConeOutsideVolume(lConeOutsideVolume:LongInt;dwApply:LongWord):LongInt; stdcall;
		function SetMaxDistance(flMaxDistance:Single;dwApply:LongWord):LongInt; stdcall;
		function SetMinDistance(flMinDistance:Single;dwApply:LongWord):LongInt; stdcall;
		function SetMode(dwMode,dwApply:LongWord):LongInt; stdcall;
		function SetPosition(x,y,z:Single;dwApply:LongWord):LongInt; stdcall;
		function SetVelocity(x,y,z:Single;dwApply:LongWord):LongInt; stdcall;
	end;
	PDirectSound3DBuffer = ^IDirectSound3DBuffer;

	IDirectSoundCaptureBuffer = interface;
	PDirectSoundCaptureBuffer = ^IDirectSoundCaptureBuffer;

	IDirectSoundCapture = interface (IUnknown)
		['{B0210781-89CD-11D0-AF08-00A0C925CD16}']
		function CreateCaptureBuffer(pcDSCBufferDesc:PDSCBUFFERDESC;ppDSCBuffer:PDirectSoundCaptureBuffer;pUnkOuter:Pointer):LongInt; stdcall;
		function GetCaps(pDSCCaps:PDSCCAPS):LongInt; stdcall;
		function Initialize(pcGuidDevice:REFGUID):LongInt; stdcall;
	end;
	PDirectSoundCapture = ^IDirectSoundCapture;

	IDirectSoundCaptureBuffer = interface (IUnknown)
		['{B0210782-89CD-11D0-AF08-00A0C925CD16}']
		function GetCaps(pDSCBCaps:PDSCBCAPS):LongInt; stdcall;
		function GetCurrentPosition(pdwCapturePosition,pdwReadPosition:PDWORD):LongInt; stdcall;
		function GetFormat(pwfxFormat:PWAVEFORMATEX;dwSizeAllocated:LongWord;pdwSizeWritten:PDWORD):LongInt; stdcall;
		function GetStatus(pdwStatus:PDWORD):LongInt; stdcall;
		function Initialize(pDirectSoundCapture:IDirectSoundCapture;pcDSCBufferDesc:PDSCBUFFERDESC):LongInt; stdcall;
		function Lock(dwOffset,dwBytes:LongWord;ppvAudioPtr1:Pointer;pdwAudioBytes1:PDWORD;ppvAudioPtr2:Pointer;pdwAudioBytes2:PDWORD;dwFlags:LongWord):LongInt; stdcall;
		function Start(dwFlags:LongWord):LongInt; stdcall;
		function Stop:LongInt; stdcall;
		function Unlock(pvAudioPtr1:Pointer;dwAudioBytes1:LongWord;pvAudioPtr2:Pointer;dwAudioBytes2:LongWord):LongInt; stdcall;
	end;

	IDirectSoundCaptureBuffer8 = interface (IDirectSoundCaptureBuffer)
		['{00990DF4-0DBB-4872-833E-6D303E80AEB6}']
		function GetObjectInPath(rguidObject:REFGUID;dwIndex:LongWord;rguidInterface:REFGUID;ppObject:Pointer):LongInt; stdcall;
		function GetFXStatus(dwFXCount:LongWord;pdwFXStatus:PDWORD):LongInt; stdcall;
	end;
	PDirectSoundCaptureBuffer8 = ^IDirectSoundCaptureBuffer8;

	IDirectSoundNotify = interface (IUnknown)
		['{B0210783-89CD-11D0-AF08-00A0C925CD16}']
		function SetNotificationPositions(dwPositionNotifies:LongWord;pcPositionNotifies:PDSBPOSITIONNOTIFY):LongInt; stdcall;
	end;
	PDirectSoundNotify = ^IDirectSoundNotify;

	IKsPropertySet = interface (IUnknown)
		['{31EFAC30-515C-11D0-A9AA-00AA0061BE93}']
		function Get(rguidPropSet:REFGUID;ulId:LongWord;pInstanceData:Pointer;ulInstanceLength:LongWord;pPropertyData:Pointer;ulDataLength:LongWord;pulBytesReturned:PDWORD):LongInt; stdcall;
		function Set_(rguidPropSet:REFGUID;ulId:LongWord;pInstanceData:Pointer;ulInstanceLength:LongWord;pPropertyData:Pointer;ulDataLength:LongWord):LongInt; stdcall;
		function QuerySupport(rguidPropSet:REFGUID;ulId:LongWord;pulTypeSupport:PDWORD):LongInt; stdcall;
	end;
	PKsPropertySet = ^IKsPropertySet;

	IDirectSoundFXGargle = interface (IUnknown)
		['{D616F352-D622-11CE-AAC5-0020AF0B99A3}']
		function SetAllParameters(pcDsFxGargle:PDSFXGargle):LongInt; stdcall;
		function GetAllParameters(pDsFxGargle:PDSFXGargle):LongInt; stdcall;
	end;
	PDirectSoundFXGargle = ^IDirectSoundFXGargle;

	IDirectSoundFXChorus = interface (IUnknown)
		['{880842E3-145F-43E6-A934-A71806E50547}']
		function SetAllParameters(pcDsFxChorus:PDSFXChorus):LongInt; stdcall;
		function GetAllParameters(pDsFxChorus:PDSFXChorus):LongInt; stdcall;
	end;
	PDirectSoundFXChorus = ^IDirectSoundFXChorus;

	IDirectSoundFXFlanger = interface (IUnknown)
		['{903E9878-2C92-4072-9B2C-EA68F5396783}']
		function SetAllParameters(pcDsFxFlanger:PDSFXFlanger):LongInt; stdcall;
		function GetAllParameters(pDsFxFlanger:PDSFXFlanger):LongInt; stdcall;
	end;
	PDirectSoundFXFlanger = ^IDirectSoundFXFlanger;

	IDirectSoundFXEcho = interface (IUnknown)
		['{8BD28EDF-50DB-4E92-A2BD-445488D1ED42}']
		function SetAllParameters(pcDsFxEcho:PDSFXEcho):LongInt; stdcall;
		function GetAllParameters(pDsFxEcho:PDSFXEcho):LongInt; stdcall;
	end;
	PDirectSoundFXEcho = ^IDirectSoundFXEcho;

	IDirectSoundFXDistortion = interface (IUnknown)
		['{8ECF4326-455F-4D8B-BDA9-8D5D3E9E3E0B}']
		function SetAllParameters(pcDsFxDistortion:PDSFXDistortion):LongInt; stdcall;
		function GetAllParameters(pDsFxDistortion:PDSFXDistortion):LongInt; stdcall;
	end;
	PDirectSoundFXDistortion = ^IDirectSoundFXDistortion;

	IDirectSoundFXCompressor = interface (IUnknown)
		['{4BBD1154-62F6-4E2C-A15C-D3B6C417F7A0}']
		function SetAllParameters(pcDsFxCompressor:PDSFXCompressor):LongInt; stdcall;
		function GetAllParameters(pDsFxCompressor:PDSFXCompressor):LongInt; stdcall;
	end;
	PDirectSoundFXCompressor = ^IDirectSoundFXCompressor;

	IDirectSoundFXParamEq = interface (IUnknown)
		['{C03CA9FE-FE90-4204-8078-82334CD177DA}']
		function SetAllParameters(pcDsFxParamEq:PDSFXParamEq):LongInt; stdcall;
		function GetAllParameters(pDsFxParamEq:PDSFXParamEq):LongInt; stdcall;
	end;
	PDirectSoundFXParamEq = ^IDirectSoundFXParamEq;

	IDirectSoundFXI3DL2Reverb = interface (IUnknown)
		['{4B166A6A-0D66-43F3-80E3-EE6280DEE1A4}']
		function SetAllParameters(pcDsFxI3DL2Reverb:PDSFXI3DL2Reverb):LongInt; stdcall;
		function GetAllParameters(pDsFxI3DL2Reverb:PDSFXI3DL2Reverb):LongInt; stdcall;
		function SetPreset(dwPreset:LongWord):LongInt; stdcall;
		function GetPreset(pdwPreset:PDWORD):LongInt; stdcall;
		function SetQuality(lQuality:LongInt):LongInt; stdcall;
		function GetQuality(plQuality:PLONG):LongInt; stdcall;
	end;
	PDirectSoundFXI3DL2Reverb = ^IDirectSoundFXI3DL2Reverb;

	IDirectSoundFXWavesReverb = interface (IUnknown)
		['{46858C3A-0DC6-45E3-B760-D4EEF16CB325}']
		function SetAllParameters(pcDsFxWavesReverb:PDSFXWavesReverb):LongInt; stdcall;
		function GetAllParameters(pDsFxWavesReverb:PDSFXWavesReverb):LongInt; stdcall;
	end;
	PDirectSoundFXWavesReverb = ^IDirectSoundFXWavesReverb;

	IDirectSoundCaptureFXAec = interface (IUnknown)
		['{174D3EB9-6696-4FAC-A46C-A0AC7BC9E20F}']
		function SetAllParameters(pcDscFxAec:PDSCFXAec):LongInt; stdcall;
		function GetAllParameters(pDscFxAec:PDSCFXAec):LongInt; stdcall;
	end;
	PDirectSoundCaptureFXAec = ^IDirectSoundCaptureFXAec;

	IDirectSoundCaptureFXNoiseSuppress = interface (IUnknown)
		['{ED311E41-FBAE-4175-9625-CD0854F693CA}']
		function SetAllParameters(pcDscFxNoiseSuppress:PDSCFXNoiseSuppress):LongInt; stdcall;
		function GetAllParameters(pDscFxNoiseSuppress:PDSCFXNoiseSuppress):LongInt; stdcall;
	end;
	PDirectSoundCaptureFXNoiseSuppress = ^IDirectSoundCaptureFXNoiseSuppress;

	IDirectSoundFullDuplex = interface (IUnknown)
		['{EDCB4C7A-DAAB-4216-A42E-6C50596DDC1D}']
		function Initialize(pCaptureGuid,pRenderGuid:REFGUID;lpDscBufferDesc:PDSCBUFFERDESC;lpDsBufferDesc:PDSBUFFERDESC;hWnd,dwLevel:LongWord;lplpDirectSoundCaptureBuffer8:PDirectSoundCaptureBuffer8;lplpDirectSoundBuffer8:PDirectSoundBuffer8):LongInt; stdcall;
	end;
	PDirectSoundFullDuplex = ^IDirectSoundFullDuplex;

const
	DSBUFFERDESC1=20;
	DSFX_LOCHARDWARE=1;
	DSFX_LOCSOFTWARE=2;
	DSFXR_PRESENT=0;
	DSFXR_LOCHARDWARE=1;
	DSFXR_LOCSOFTWARE=2;
	DSFXR_UNALLOCATED=3;
	DSFXR_FAILED=4;
	DSFXR_UNKNOWN=5;
	DSFXR_SENDLOOP=6;
	DSCFX_LOCHARDWARE=1;
	DSCFX_LOCSOFTWARE=2;
	DSCFXR_LOCHARDWARE=$10;
	DSCFXR_LOCSOFTWARE=$20;
	DSCFXR_UNALLOCATED=$40;
	DSCFXR_FAILED=$80;
	DSCFXR_UNKNOWN=$100;
	KSPROPERTY_SUPPORT_GET=1;
	KSPROPERTY_SUPPORT_SET=2;
	DSFXGARGLE_WAVE_TRIANGLE=0;
	DSFXGARGLE_WAVE_SQUARE=1;
	DSFXGARGLE_RATEHZ_MIN=1;
	DSFXGARGLE_RATEHZ_MAX=1000;
	DSFXCHORUS_WAVE_TRIANGLE=0;
	DSFXCHORUS_WAVE_SIN=1;
	DSFXCHORUS_WETDRYMIX_MIN=0.0;
	DSFXCHORUS_WETDRYMIX_MAX=100.0;
	DSFXCHORUS_DEPTH_MIN=0.0;
	DSFXCHORUS_DEPTH_MAX=100.0;
	DSFXCHORUS_FEEDBACK_MIN=-99.0;
	DSFXCHORUS_FEEDBACK_MAX=99.0;
	DSFXCHORUS_FREQUENCY_MIN=0.0;
	DSFXCHORUS_FREQUENCY_MAX=10.0;
	DSFXCHORUS_DELAY_MIN=0.0;
	DSFXCHORUS_DELAY_MAX=20.0;
	DSFXCHORUS_PHASE_MIN=0;
	DSFXCHORUS_PHASE_MAX=4;
	DSFXCHORUS_PHASE_NEG_180=0;
	DSFXCHORUS_PHASE_NEG_90=1;
	DSFXCHORUS_PHASE_ZERO=2;
	DSFXCHORUS_PHASE_90=3;
	DSFXCHORUS_PHASE_180=4;
	DSFXFLANGER_WAVE_TRIANGLE=0;
	DSFXFLANGER_WAVE_SIN=1;
	DSFXFLANGER_WETDRYMIX_MIN=0.0;
	DSFXFLANGER_WETDRYMIX_MAX=100.0;
	DSFXFLANGER_FREQUENCY_MIN=0.0;
	DSFXFLANGER_FREQUENCY_MAX=10.0;
	DSFXFLANGER_DEPTH_MIN=0.0;
	DSFXFLANGER_DEPTH_MAX=100.0;
	DSFXFLANGER_PHASE_MIN=0;
	DSFXFLANGER_PHASE_MAX=4;
	DSFXFLANGER_FEEDBACK_MIN=-99.0;
	DSFXFLANGER_FEEDBACK_MAX=99.0;
	DSFXFLANGER_DELAY_MIN=0.0;
	DSFXFLANGER_DELAY_MAX=4.0;
	DSFXFLANGER_PHASE_NEG_180=0;
	DSFXFLANGER_PHASE_NEG_90=1;
	DSFXFLANGER_PHASE_ZERO=2;
	DSFXFLANGER_PHASE_90=3;
	DSFXFLANGER_PHASE_180=4;
	DSFXECHO_WETDRYMIX_MIN=0.0;
	DSFXECHO_WETDRYMIX_MAX=100.0;
	DSFXECHO_FEEDBACK_MIN=0.0;
	DSFXECHO_FEEDBACK_MAX=100.0;
	DSFXECHO_LEFTDELAY_MIN=1.0;
	DSFXECHO_LEFTDELAY_MAX=2000.0;
	DSFXECHO_RIGHTDELAY_MIN=1.0;
	DSFXECHO_RIGHTDELAY_MAX=2000.0;
	DSFXECHO_PANDELAY_MIN=0;
	DSFXECHO_PANDELAY_MAX=1;
	DSFXDISTORTION_GAIN_MIN=-60.0;
	DSFXDISTORTION_GAIN_MAX=0.0;
	DSFXDISTORTION_EDGE_MIN=0.0;
	DSFXDISTORTION_EDGE_MAX=100.0;
	DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN=100.0;
	DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX=8000.0;
	DSFXDISTORTION_POSTEQBANDWIDTH_MIN=100.0;
	DSFXDISTORTION_POSTEQBANDWIDTH_MAX=8000.0;
	DSFXDISTORTION_PRELOWPASSCUTOFF_MIN=100.0;
	DSFXDISTORTION_PRELOWPASSCUTOFF_MAX=8000.0;
	DSFXCOMPRESSOR_GAIN_MIN=-60.0;
	DSFXCOMPRESSOR_GAIN_MAX=60.0;
	DSFXCOMPRESSOR_ATTACK_MIN=0.01;
	DSFXCOMPRESSOR_ATTACK_MAX=500.0;
	DSFXCOMPRESSOR_RELEASE_MIN=50.0;
	DSFXCOMPRESSOR_RELEASE_MAX=3000.0;
	DSFXCOMPRESSOR_THRESHOLD_MIN=-60.0;
	DSFXCOMPRESSOR_THRESHOLD_MAX=0.0;
	DSFXCOMPRESSOR_RATIO_MIN=1.0;
	DSFXCOMPRESSOR_RATIO_MAX=100.0;
	DSFXCOMPRESSOR_PREDELAY_MIN=0.0;
	DSFXCOMPRESSOR_PREDELAY_MAX=4.0;
	DSFXPARAMEQ_CENTER_MIN=80.0;
	DSFXPARAMEQ_CENTER_MAX=16000.0;
	DSFXPARAMEQ_BANDWIDTH_MIN=1.0;
	DSFXPARAMEQ_BANDWIDTH_MAX=36.0;
	DSFXPARAMEQ_GAIN_MIN=-15.0;
	DSFXPARAMEQ_GAIN_MAX=15.0;
	DSFX_I3DL2REVERB_ROOM_MIN=-10000;
	DSFX_I3DL2REVERB_ROOM_MAX=0;
	DSFX_I3DL2REVERB_ROOM_DEFAULT=-1000;
	DSFX_I3DL2REVERB_ROOMHF_MIN=-10000;
	DSFX_I3DL2REVERB_ROOMHF_MAX=0;
	DSFX_I3DL2REVERB_ROOMHF_DEFAULT=-100;
	DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN=0.0;
	DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX=10.0;
	DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_DEFAULT=0.0;
	DSFX_I3DL2REVERB_DECAYTIME_MIN=0.1;
	DSFX_I3DL2REVERB_DECAYTIME_MAX=20.0;
	DSFX_I3DL2REVERB_DECAYTIME_DEFAULT=1.49;
	DSFX_I3DL2REVERB_DECAYHFRATIO_MIN=0.1;
	DSFX_I3DL2REVERB_DECAYHFRATIO_MAX=2.0;
	DSFX_I3DL2REVERB_DECAYHFRATIO_DEFAULT=0.83;
	DSFX_I3DL2REVERB_REFLECTIONS_MIN=-10000;
	DSFX_I3DL2REVERB_REFLECTIONS_MAX=1000;
	DSFX_I3DL2REVERB_REFLECTIONS_DEFAULT=-2602;
	DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN=0.0;
	DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX=0.3;
	DSFX_I3DL2REVERB_REFLECTIONSDELAY_DEFAULT=0.007;
	DSFX_I3DL2REVERB_REVERB_MIN=-10000;
	DSFX_I3DL2REVERB_REVERB_MAX=2000;
	DSFX_I3DL2REVERB_REVERB_DEFAULT=200;
	DSFX_I3DL2REVERB_REVERBDELAY_MIN=0.0;
	DSFX_I3DL2REVERB_REVERBDELAY_MAX=0.1;
	DSFX_I3DL2REVERB_REVERBDELAY_DEFAULT=0.011;
	DSFX_I3DL2REVERB_DIFFUSION_MIN=0.0;
	DSFX_I3DL2REVERB_DIFFUSION_MAX=100.0;
	DSFX_I3DL2REVERB_DIFFUSION_DEFAULT=100.0;
	DSFX_I3DL2REVERB_DENSITY_MIN=0.0;
	DSFX_I3DL2REVERB_DENSITY_MAX=100.0;
	DSFX_I3DL2REVERB_DENSITY_DEFAULT=100.0;
	DSFX_I3DL2REVERB_HFREFERENCE_MIN=20.0;
	DSFX_I3DL2REVERB_HFREFERENCE_MAX=20000.0;
	DSFX_I3DL2REVERB_HFREFERENCE_DEFAULT=5000.0;
	DSFX_I3DL2REVERB_QUALITY_MIN=0;
	DSFX_I3DL2REVERB_QUALITY_MAX=3;
	DSFX_I3DL2REVERB_QUALITY_DEFAULT=2;
	DSFX_WAVESREVERB_INGAIN_MIN=-96.0;
	DSFX_WAVESREVERB_INGAIN_MAX=0.0;
	DSFX_WAVESREVERB_INGAIN_DEFAULT=0.0;
	DSFX_WAVESREVERB_REVERBMIX_MIN=-96.0;
	DSFX_WAVESREVERB_REVERBMIX_MAX=0.0;
	DSFX_WAVESREVERB_REVERBMIX_DEFAULT=0.0;
	DSFX_WAVESREVERB_REVERBTIME_MIN=0.001;
	DSFX_WAVESREVERB_REVERBTIME_MAX=3000.0;
	DSFX_WAVESREVERB_REVERBTIME_DEFAULT=1000.0;
	DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN=0.001;
	DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX=0.999;
	DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT=0.001;
	DS_OK=0;
	DS_NO_VIRTUALIZATION=$878000A;
	DS_INCOMPLETE=$8780014;
	DSERR_ALLOCATED=$8878000A;
	DSERR_CONTROLUNAVAIL=$8878001E;
	DSERR_INVALIDPARAM=$80070057;
	DSERR_INVALIDCALL=$88780032;
	DSERR_GENERIC=$80004005;
	DSERR_PRIOLEVELNEEDED=$88780046;
	DSERR_OUTOFMEMORY=$8007000E;
	DSERR_BADFORMAT=$88780064;
	DSERR_UNSUPPORTED=$80004001;
	DSERR_NODRIVER=$88780078;
	DSERR_ALREADYINITIALIZED=$88780082;
	DSERR_NOAGGREGATION=$80040110;
	DSERR_BUFFERLOST=$88780096;
	DSERR_OTHERAPPHASPRIO=$887800A0;
	DSERR_UNINITIALIZED=$887800AA;
	DSERR_NOINTERFACE=$80004002;
	DSERR_ACCESSDENIED=$80070005;
	DSERR_BUFFERTOOSMALL=$887800B4;
	DSERR_DS8_REQUIRED=$887800BE;
	DSERR_SENDLOOP=$887800C8;
	DSERR_BADSENDBUFFERGUID=$887800D2;
	DSERR_OBJECTNOTFOUND=$88781161;
	DSCAPS_PRIMARYMONO=1;
	DSCAPS_PRIMARYSTEREO=2;
	DSCAPS_PRIMARY8BIT=4;
	DSCAPS_PRIMARY16BIT=8;
	DSCAPS_CONTINUOUSRATE=$10;
	DSCAPS_EMULDRIVER=$20;
	DSCAPS_CERTIFIED=$40;
	DSCAPS_SECONDARYMONO=$100;
	DSCAPS_SECONDARYSTEREO=$200;
	DSCAPS_SECONDARY8BIT=$400;
	DSCAPS_SECONDARY16BIT=$800;
	DSSCL_NORMAL=1;
	DSSCL_PRIORITY=2;
	DSSCL_EXCLUSIVE=3;
	DSSCL_WRITEPRIMARY=4;
	DSSPEAKER_HEADPHONE=1;
	DSSPEAKER_MONO=2;
	DSSPEAKER_QUAD=3;
	DSSPEAKER_STEREO=4;
	DSSPEAKER_SURROUND=5;
	DSSPEAKER_5POINT1=6;
	DSSPEAKER_GEOMETRY_MIN=5;
	DSSPEAKER_GEOMETRY_NARROW=10;
	DSSPEAKER_GEOMETRY_WIDE=20;
	DSSPEAKER_GEOMETRY_MAX=$B4;
	DSBCAPS_PRIMARYBUFFER=1;
	DSBCAPS_STATIC=2;
	DSBCAPS_LOCHARDWARE=4;
	DSBCAPS_LOCSOFTWARE=8;
	DSBCAPS_CTRL3D=$10;
	DSBCAPS_CTRLFREQUENCY=$20;
	DSBCAPS_CTRLPAN=$40;
	DSBCAPS_CTRLVOLUME=$80;
	DSBCAPS_CTRLPOSITIONNOTIFY=$100;
	DSBCAPS_CTRLFX=$200;
	DSBCAPS_STICKYFOCUS=$4000;
	DSBCAPS_GLOBALFOCUS=$8000;
	DSBCAPS_GETCURRENTPOSITION2=$10000;
	DSBCAPS_MUTE3DATMAXDISTANCE=$20000;
	DSBCAPS_LOCDEFER=$40000;
	DSBPLAY_LOOPING=1;
	DSBPLAY_LOCHARDWARE=2;
	DSBPLAY_LOCSOFTWARE=4;
	DSBPLAY_TERMINATEBY_TIME=8;
	DSBPLAY_TERMINATEBY_DISTANCE=$10;
	DSBPLAY_TERMINATEBY_PRIORITY=$20;
	DSBSTATUS_PLAYING=1;
	DSBSTATUS_BUFFERLOST=2;
	DSBSTATUS_LOOPING=4;
	DSBSTATUS_LOCHARDWARE=8;
	DSBSTATUS_LOCSOFTWARE=$10;
	DSBSTATUS_TERMINATED=$20;
	DSBLOCK_FROMWRITECURSOR=1;
	DSBLOCK_ENTIREBUFFER=2;
	DSBFREQUENCY_MIN=100;
	DSBFREQUENCY_MAX=100000;
	DSBFREQUENCY_ORIGINAL=0;
	DSBPAN_LEFT=-10000;
	DSBPAN_CENTER=0;
	DSBPAN_RIGHT=10000;
	DSBVOLUME_MIN=-10000;
	DSBVOLUME_MAX=0;
	DSBSIZE_MIN=4;
	DSBSIZE_MAX=$FFFFFFF;
	DSBSIZE_FX_MIN=150;
	DS3DMODE_NORMAL=0;
	DS3DMODE_HEADRELATIVE=1;
	DS3DMODE_DISABLE=2;
	DS3D_IMMEDIATE=0;
	DS3D_DEFERRED=1;
	DS3D_MINDISTANCEFACTOR=0.00000001;
	DS3D_MAXDISTANCEFACTOR=99999999.9;
	DS3D_DEFAULTDISTANCEFACTOR=1.0;
	DS3D_MINROLLOFFFACTOR=0.0;
	DS3D_MAXROLLOFFFACTOR=10.0;
	DS3D_DEFAULTROLLOFFFACTOR=1.0;
	DS3D_MINDOPPLERFACTOR=0.0;
	DS3D_MAXDOPPLERFACTOR=10.0;
	DS3D_DEFAULTDOPPLERFACTOR=1.0;
	DS3D_DEFAULTMINDISTANCE=1.0;
	DS3D_DEFAULTMAXDISTANCE=1000000000.0;
	DS3D_MINCONEANGLE=0;
	DS3D_MAXCONEANGLE=360;
	DS3D_DEFAULTCONEANGLE=360;
	DS3D_DEFAULTCONEOUTSIDEVOLUME=0;
	DSCCAPS_EMULDRIVER=$20;
	DSCCAPS_CERTIFIED=$40;
	DSCBCAPS_WAVEMAPPED=$80000000;
	DSCBCAPS_CTRLFX=$200;
	DSCBLOCK_ENTIREBUFFER=1;
	DSCBSTATUS_CAPTURING=1;
	DSCBSTATUS_LOOPING=2;
	DSCBSTART_LOOPING=1;
	DSBPN_OFFSETSTOP=$FFFFFFFF;
	DS_CERTIFIED=0;
	DS_UNCERTIFIED=1;
	DS_SYSTEM_RESOURCES_NO_HOST_RESOURCES=0;
	DS_SYSTEM_RESOURCES_ALL_HOST_RESOURCES=$7FFFFFFF;
	DS_SYSTEM_RESOURCES_UNDEFINED=$80000000;
	DSFX_I3DL2_MATERIAL_PRESET_SINGLEWINDOW=0;
	DSFX_I3DL2_MATERIAL_PRESET_DOUBLEWINDOW=1;
	DSFX_I3DL2_MATERIAL_PRESET_THINDOOR=2;
	DSFX_I3DL2_MATERIAL_PRESET_THICKDOOR=3;
	DSFX_I3DL2_MATERIAL_PRESET_WOODWALL=4;
	DSFX_I3DL2_MATERIAL_PRESET_BRICKWALL=5;
	DSFX_I3DL2_MATERIAL_PRESET_STONEWALL=6;
	DSFX_I3DL2_MATERIAL_PRESET_CURTAIN=7;
	DSFX_I3DL2_ENVIRONMENT_PRESET_DEFAULT=0;
	DSFX_I3DL2_ENVIRONMENT_PRESET_GENERIC=1;
	DSFX_I3DL2_ENVIRONMENT_PRESET_PADDEDCELL=2;
	DSFX_I3DL2_ENVIRONMENT_PRESET_ROOM=3;
	DSFX_I3DL2_ENVIRONMENT_PRESET_BATHROOM=4;
	DSFX_I3DL2_ENVIRONMENT_PRESET_LIVINGROOM=5;
	DSFX_I3DL2_ENVIRONMENT_PRESET_STONEROOM=6;
	DSFX_I3DL2_ENVIRONMENT_PRESET_AUDITORIUM=7;
	DSFX_I3DL2_ENVIRONMENT_PRESET_CONCERTHALL=8;
	DSFX_I3DL2_ENVIRONMENT_PRESET_CAVE=9;
	DSFX_I3DL2_ENVIRONMENT_PRESET_ARENA=10;
	DSFX_I3DL2_ENVIRONMENT_PRESET_HANGAR=11;
	DSFX_I3DL2_ENVIRONMENT_PRESET_CARPETEDHALLWAY=12;
	DSFX_I3DL2_ENVIRONMENT_PRESET_HALLWAY=13;
	DSFX_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR=14;
	DSFX_I3DL2_ENVIRONMENT_PRESET_ALLEY=15;
	DSFX_I3DL2_ENVIRONMENT_PRESET_FOREST=16;
	DSFX_I3DL2_ENVIRONMENT_PRESET_CITY=17;
	DSFX_I3DL2_ENVIRONMENT_PRESET_MOUNTAINS=18;
	DSFX_I3DL2_ENVIRONMENT_PRESET_QUARRY=19;
	DSFX_I3DL2_ENVIRONMENT_PRESET_PLAIN=20;
	DSFX_I3DL2_ENVIRONMENT_PRESET_PARKINGLOT=21;
	DSFX_I3DL2_ENVIRONMENT_PRESET_SEWERPIPE=22;
	DSFX_I3DL2_ENVIRONMENT_PRESET_UNDERWATER=23;
	DSFX_I3DL2_ENVIRONMENT_PRESET_SMALLROOM=24;
	DSFX_I3DL2_ENVIRONMENT_PRESET_MEDIUMROOM=25;
	DSFX_I3DL2_ENVIRONMENT_PRESET_LARGEROOM=26;
	DSFX_I3DL2_ENVIRONMENT_PRESET_MEDIUMHALL=27;
	DSFX_I3DL2_ENVIRONMENT_PRESET_LARGEHALL=28;
	DSFX_I3DL2_ENVIRONMENT_PRESET_PLATE=29;

implementation
end.