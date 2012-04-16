// ---------------------------------------------------------------
//
// 4D Plugin API
//
// File : PrivateTypes.h
// Description : This file stores all the internal structures
//				 used by 4D Plugin API and useless for API user.
//
// rev : 6.8.1
//
// ---------------------------------------------------------------

#ifndef __PRIVATETYPES__
#define __PRIVATETYPES__


#ifdef __cplusplus
extern "C" {
#endif


// all the 4th Dimension structures use 2 bytes alignment
#if VERSIONWIN
	#pragma pack(push,2)
#elif VERSIONMAC
	#pragma options align = mac68k
#endif

// used for query operations.
typedef struct LineBlock
{
	char				fOperator;
	short				fField;
	short				fTable;
	char				fComparison;
	union
	{
		char				fString[41];
		double				fReal;			
		PA_Date				fDate;
		long				fLongint;
		short				fInteger;
		char				fBoolean;
	} uValue;
} LineBlock;

	
// This structure is always sent when calling back 4th Dimension.
// the different fields are used depending the kind of
// the action required.
typedef struct EngineBlock
{
	short				fTable;
	short				fField;
	long				fRecord;
	char				fManyToOne;
	char				fOneToMany;
	char				fName[256];
	PA_Handle			fHandle;
	short				fError;
	long				fParam1;
	long				fParam2;
	long				fParam3;
	long				fParam4;
	double				fReal;	
	short				fFiller;
	PA_Date				fDate;
	long				fLongint;			
	short				fShort;		
	char				fString[82];		
	PA_Text				fText;		
	char				fClearOldVariable;
	char				fNativeReal;
	short				fNbSearchLines;
} EngineBlock;

// facility to call back 4D more easily using the proc pointer 
#define Call4D(s,p) (*gCall4D)(s,p)

#if VERSIONMAC
	typedef pascal void (*Call4DProcPtr)( short, EngineBlock* );
	pascal void Main( long selector, void* params, void** data, void* result );
#elif VERSIONWIN
	typedef void (__stdcall *Call4DProcPtr)( short, EngineBlock* );
	void __stdcall FourDPack( long selector, void* params, void** data, void* result );
#endif

extern Call4DProcPtr gCall4D;

// this structure is sent to Plugin at init.
typedef struct PackInitBlock
{
	long			fVersion;
	long			fLength;
	long			fCPUType;
	Call4DProcPtr	fCall4D;
 	long			fSupportedVersion;
} PackInitBlock;


// reset struct alignment
#if VERSIONWIN
	#pragma pack(pop)
#elif VERSIONMAC
	#pragma options align = reset
#endif

#ifdef __cplusplus
}
#endif

#endif
