// ---------------------------------------------------------------
//
// 4D Plugin API
//
// File : 4DPluginAPI.c
//
// rev : 6.8.1
//
// ---------------------------------------------------------------

#include "4DPluginAPI.h"
#include "PrivateTypes.h"
#include "EntryPoints.h"
#include <string.h> // dcc code patch
#include <Windows.h> // dcc code patch
#include "..\ppc_h\asiport.h" // dcc code patch


// gCall4D stores the address of a callback routine in 4D.
// this address is given by 4D when it calls the plugin for the first time.
Call4DProcPtr gCall4D = 0;
long				sIsPriorTo67; //dcc for patch code

// 4D always calls to the plugin by calling a "Main" function on Macintosh
// and a "FourDPack" function on Windows.
// 4DPluginAPI implements this call in order to store the callback address
// then call a "PluginMain" function you must provide.

#if VERSIONMAC
	pascal void Main( long selector, void *parameters, void **data, void *result )
#elif VERSIONWIN
	void __stdcall FourDPack( long selector, void *parameters, void **data, void *result )
#endif
{
	PluginBlock params;

	params.fParameters = parameters;
	params.fResult = result;
	params.fData = data;

	// Package is initialized. It's time to read the callback address
	if ( ( selector == kServerInitPlugin ) || ( selector ==  kInitPlugin ) )
	{
		// get the callback address
		gCall4D = ( (PackInitBlock*)parameters )->fCall4D;
		( (PackInitBlock*)parameters )->fSupportedVersion = 0x00000660;
		sIsPriorTo67 = ((PA_Get4DVersion() & 0x0000FFFF) < 0x00000660); //dcc for patch code
	}

	// Call "PluginMain" provided by the user of the 4DPluginAPI
	PluginMain( selector, &params );
}


// -----------------------------------------
//
// Error codes
//
// -----------------------------------------


static short sErrorCode = 0;

PA_ErrorCode PA_GetLastError()
{
	return (PA_ErrorCode) sErrorCode;
}

// -----------------------------------------
//
// 4th Dimensions memory manager
//
// -----------------------------------------

PA_Handle PA_NewHandle( long len )
{
	EngineBlock eb;
	//+++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		eb.fHandle = NewHandle(len);
		sErrorCode = MemError();
	} else {
	//+++++++++++++++++++++++++++++++++++++++++
		eb.fParam1 = 1;
		eb.fParam2 = len;
		eb.fError  = 0;

		Call4D( EX_HANDLE_MANAGER, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;
	} // code patch
	return eb.fHandle;
}

void PA_DisposeHandle( PA_Handle handle )
{
	EngineBlock eb;
	//++++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		if (handle) {
			DisposeHandle((Handle) handle);
			sErrorCode = MemError();
		}
	} else {
	//++++++++++++++++++++++++++++++++++++++++++
		eb.fError  = 0;

		if ( handle )
		{
			eb.fParam1 = 2;
			eb.fHandle = handle;
			Call4D( EX_HANDLE_MANAGER, &eb );
		}

		sErrorCode = (PA_ErrorCode) eb.fError;
	} // code patch
}


// return true if successfull
char PA_SetHandleSize( PA_Handle handle, long newlen )
{
	EngineBlock eb;
	char				ok = 0;

	//+++++++++++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		if (handle) {
			SetHandleSize((Handle) handle, newlen);
			sErrorCode = MemError();
			ok = (sErrorCode ? 0 : 1);
		}
	} else {
	//+++++++++++++++++++++++++++++++++++++++++++++++++
		eb.fParam1 = 3;
		eb.fParam2 = newlen;
		eb.fHandle = handle;
		eb.fError  = 0;

		Call4D( EX_HANDLE_MANAGER, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;

		ok = (char) (eb.fError ? 0 : 1);
	} //code patch
	return ok;
	//return (char) ( eb.fError ? 0 : 1 );
}


long PA_GetHandleSize( PA_Handle handle )
{
	EngineBlock eb;

	//++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		eb.fParam2 = GetHandleSize(handle);
		sErrorCode = MemError();
	} else {
	//++++++++++++++++++++++++++++++++++++++++
		eb.fParam1 = 4;
		eb.fParam2 = 0;
		eb.fHandle = handle;
		eb.fError  = 0;

		Call4D( EX_HANDLE_MANAGER, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;
	} //code patch
	return eb.fParam2;
}


char* PA_LockHandle( PA_Handle handle )
{
	EngineBlock eb;
	//++++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		HLock(handle);
		sErrorCode = MemError();
		return *handle;
	} else {
	//++++++++++++++++++++++++++++++++++++++++++
		eb.fParam1 = 5;
		eb.fHandle = handle;
		eb.fError  = 0;

		Call4D( EX_HANDLE_MANAGER, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;

		return (char*) eb.fParam3;
	}
}


void PA_UnlockHandle( PA_Handle handle )
{
	EngineBlock eb;

	//+++++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		HUnlock(handle);
		sErrorCode = MemError();
	} else {
	//+++++++++++++++++++++++++++++++++++++++++++
		eb.fParam1 = 6;
		eb.fHandle = handle;
		eb.fError  = 0;

		Call4D( EX_HANDLE_MANAGER, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;
	} //code patch
}


void PA_MoveBlock( void *source, void *dest, long len )
{
	char* ptsource;
	char* ptdest;

	if ( source == 0 || dest == 0 )
		return;

	ptsource = (char*) source;
	ptdest   = (char*) dest;

	while (--len >= 0)
		*ptdest++ = *ptsource++;
}

// -----------------------------------------
//
// Strings
//
// -----------------------------------------

#if VERSIONMAC
static PA_CharSet sUserCharSet = eCS_Macintosh;
#endif

#if VERSIONWIN
static PA_CharSet sUserCharSet = eCS_Ansi;
#endif

static PA_StringKind sUserStringKind = eSK_CString;


void PA_UseMacCharacters()
{
	sUserCharSet = eCS_Macintosh;
}


void PA_UseAnsiCharacters()
{
	sUserCharSet = eCS_Ansi;
}


PA_CharSet PA_GetUserCharSet()
{
	return sUserCharSet;
}


void PA_UsePStrings()
{
	sUserStringKind = eSK_PString;
}


void PA_UseCStrings()
{
	sUserStringKind = eSK_CString;
}


PA_StringKind PA_GetUserStringKind()
{
	return sUserStringKind;
}


static void ToDialVarName( char* source, char* dest )
{
	PA_ConvertStrings( source, sUserStringKind, sUserCharSet, dest, eSK_CString, eCS_Macintosh );
}


static void ToUserString( char* source, char* dest )
{
	PA_ConvertStrings( source, eSK_PString, eCS_Macintosh, dest, sUserStringKind, sUserCharSet );
}


static void FromUserString( char* source, char* dest )
{
	PA_ConvertStrings( source, sUserStringKind, sUserCharSet, dest, eSK_PString, eCS_Macintosh );
}


static long ToUserText( PA_Handle macHandle, char* dest )
{
	EngineBlock eb;
	long				size;
	char				state;
	if ( macHandle == 0 )
		return 0;

	//+++++++++++++++++++++++++++++++++
	if(sIsPriorTo67) {
		size = GetHandleSize(macHandle);
		if(size > 32000)
			size = 32000;
		if(dest) {
			state = HGetState(macHandle);
			HLock(macHandle);
			PA_MoveBlock(*macHandle, dest, size);
			HSetState(macHandle, state);
		}
		return size;
	}else {
	//+++++++++++++++++++++++++++++++++
		if ( sUserCharSet == eCS_Macintosh )
			eb.fParam1 = 4;
		else
			eb.fParam1 = 2;

		eb.fParam2 = 0;
		eb.fParam3 = (long) dest;
		eb.fParam4 = 0;
		eb.fHandle = macHandle;

		Call4D( EX_CONVERT_STRING, &eb );

		return eb.fParam4;
	}//dcc code patch
}


static PA_Handle FromUserText( char* text, long* len )
{
	EngineBlock		eb;
	Handle				h;

	//+++++++++++++++++++++++++++++++++++++++++++++++
	if(!text || !len) return 0L;
	if(sIsPriorTo67) {
		if (*len > 32000)	*len = 32000;
		h = NewHandle(*len);
		if (h) {
			PA_LockHandle(h);
			PA_MoveBlock(text, *h, *len);
			PA_UnlockHandle(h);
		}
		return (PA_Handle)h;

	} else {
	//+++++++++++++++++++++++++++++++++++++++++++++++
		if ( text == 0 )
		return 0;

		if ( sUserCharSet == eCS_Macintosh )
		{
			if ( *len > 32000 )
			*len = 32000;

			eb.fParam1 = 5;
		}
		else
		eb.fParam1 = 3;

		eb.fParam2 = 0;
		eb.fParam3 = (long) text;
		eb.fParam4 = *len;
		eb.fHandle = 0;

		Call4D( EX_CONVERT_STRING, &eb );

		*len = eb.fParam4;

		return eb.fHandle;
	}
}


static long ToUserData( PA_Handle handle, char* dest )
{
	EngineBlock eb;
	long				size;
	char				state;

	if ( handle == 0 )
		return 0;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		size = GetHandleSize(handle);
		if (dest) {
			state = HGetState(handle);
			HLock(handle);
			PA_MoveBlock(*handle, dest, size);
			HSetState(handle, state);
		}
		return size;

	} else {
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
		eb.fParam1 = 4;
		eb.fParam2 = 0;
		eb.fParam3 = (long) dest;
		eb.fParam4 = 0;
		eb.fHandle = handle;

		Call4D( EX_CONVERT_STRING, &eb );

		return eb.fParam4;
	} //dcc code patch
}

// reads the picture informations at the end of picture handle
static void GetPictureInfo( PA_Handle handle, PA_PictureInfo* info )
{
	long len;
	char* pt;

	if ( info )
	{
		info->fMode = 0;
		info->fHOffset = 0;
		info->fVOffset = 0;
	}
	else
		return;

	if ( ( handle == 0 ) || ( info == 0 ) )
		return;

	len = PA_GetHandleSize( handle ) - (long)sizeof( PA_PictureInfo );
	if ( len >= 0)
	{
		pt = PA_LockHandle( handle );
		*info = *(PA_PictureInfo*) ( pt + len );
		PA_UnlockHandle( handle );
	}
}


static long ToUserPicture( PA_Handle handle, void* dest, PA_PictureInfo* info )
{
	long len;
	char* pt;

	if ( handle == 0 )
		return 0;

	len = PA_GetHandleSize( handle ) - (long)sizeof( PA_PictureInfo );
	if ( len >= 0)
	{
		pt = PA_LockHandle( handle );

		if ( dest )
			PA_MoveBlock( pt,dest, len );

		if ( info )
			*info = *(PA_PictureInfo*) ( pt + len );

		PA_UnlockHandle( handle );
	}

	return len;
}

static PA_Handle FromUserData( void* data, long len )
{
	EngineBlock eb;

	if ( data == 0 )
		return 0;

	eb.fParam1 = 5;
	eb.fParam2 = 0;
	eb.fParam3 = (long) data;
	eb.fParam4 = len;
	eb.fHandle = 0;

	Call4D( EX_CONVERT_STRING, &eb );

	return eb.fHandle;
}


// adds picture informations at the end of the picture handle
static PA_Handle FromUserPictureHandle( PA_Handle hpicture, PA_PictureInfo info )
{
	long len;
	char* pt;

	if ( hpicture == 0 )
		return 0;

	len = PA_GetHandleSize( hpicture );
	if ( PA_SetHandleSize( hpicture, len + (long) sizeof( PA_PictureInfo ) ) )
	{
		pt = PA_LockHandle( hpicture );
		*(PA_PictureInfo*) ( pt + len ) = info;
		PA_UnlockHandle( hpicture );
	}

	return hpicture;
}


static PA_Handle FromUserPicture( void* picture, long len, PA_PictureInfo info )
{
	PA_Handle handle;
	char* pt;

	if ( picture == 0 )
		return 0;

	handle = PA_NewHandle( len + (long) sizeof( PA_PictureInfo ) );
	if ( handle )
	{
		pt = PA_LockHandle( handle );
		PA_MoveBlock( picture, pt, len );
		*(PA_PictureInfo*) ( pt + len ) = info;
		PA_UnlockHandle( handle );
	}

	return handle;
}


static char StripInterprocessVariableName( char* variableName )
{
	char interprocess = 0;
	short i;

	// check interprocess variable
	if ( ( variableName[0] > 1 ) && ( variableName[1] == 0xD7 ) )
		interprocess = 1;
	else if ( ( variableName[0] > 2 ) && ( variableName[1] == '<' ) && ( variableName[2] == '>' ) )
		interprocess = 2;

	if ( interprocess )
	{
		// remove '<>' or diamond character before variable name
		variableName[0] -= interprocess;
		for ( i = 1; i <= variableName[0]; i++ )
			variableName[i] = variableName[i + interprocess];
	}

	return interprocess;
}


// --------------------------------------------
//
// 4th Dimension application informations
//
// --------------------------------------------


unsigned long PA_Get4DVersion()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (unsigned long) eb.fParam2;
}


char PA_IsDemoVersion()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( ( (unsigned long) eb.fHandle ) & 0x20000000 ) != 0 ) ? 1 : 0 );
}


char PA_IsDatabaseLocked()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( ( (unsigned long) eb.fHandle ) & 0x40000000 ) != 0 ) ? 1 : 0 );
}


char PA_IsCompiled()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( eb.fParam1 & 0x08000000 ) != 0 ) ? 1 : 0 );
}


char PA_Is4DClient()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( eb.fParam1 & 0x04000000 ) != 0 ) ? 1 : 0 );
}


char PA_Is4DServer()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( eb.fParam1 & 0x02000000 ) != 0 ) ? 1 : 0 );
}


char PA_IsWebProcess()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( eb.fParam1 & 0x01000000 ) != 0 ) ? 1 : 0 );
}


char PA_GetMessagesStatus()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( eb.fParam1 & 0x10000000 ) != 0 ) ? 1 : 0 );
}


void PA_SetMessagesStatus( char showMessages )
{
	EngineBlock eb;
	eb.fParam1 = showMessages;
	Call4D( EX_MESSAGES, &eb );
	sErrorCode = eER_NoErr;
}


void PA_SetThermometerRect( PA_Rect rect )
{
	EngineBlock eb;
	eb.fParam1 = rect.fLeft;
	eb.fParam2 = rect.fTop;
	eb.fParam3 = rect.fRight;
	eb.fParam4 = rect.fBottom;
	eb.fError  = 0;
	Call4D( EX_THERMOMETER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_Is4DMono()
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( ( ( eb.fParam1 & 0x06000000 ) == 0 ) ? 1 : 0 );
}


void PA_GetStructureName( char* structName )
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, structName );
}


void PA_GetDataName( char* dataName )
{
	EngineBlock eb;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fString, dataName );
}


void PA_GetLogName( char* logName )
{
	EngineBlock eb;
	Call4D( EX_LOG_INFO, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, logName );
}


void PA_GetApplicationFullPath( char* applicationPath )
{
	EngineBlock eb;
	Call4D( EX_GET_APPLICATION_FULLPATH, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, applicationPath );
}


void PA_GetStructureFullPath( char* structurePath )
{
	EngineBlock eb;
	Call4D( EX_GET_STRUCTURE_FULLPATH, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, structurePath );
}


// warning : two packages can share the same ID,
// exept for modules with ID under 15000 like 4D Write
short PA_FindPackage( short packageID )
{
	EngineBlock eb;

	eb.fParam1 = packageID;
	eb.fParam2 = 0;
	Call4D( EX_FIND_PACKAGE, &eb );
	sErrorCode = eER_NoErr;

	return (short) eb.fParam2;
}


void PA_PackageInfo( short packageNumber, short* packageID, void** procPtr, void** data, short* resfile, char* name )
{
	EngineBlock eb;

	eb.fParam1 = packageNumber;
	Call4D( EX_PACKAGE_INFO, &eb );
	sErrorCode = eER_NoErr;

	if ( packageID )
		*packageID = (short) eb.fParam1;

	if ( procPtr )
		*procPtr = (void*) eb.fParam2;

	if ( resfile )
		*resfile = (short) eb.fParam4;

	if ( data )
		*data = (void*) eb.fHandle;

	ToUserString( eb.fName, name );
}


void PA_Quit4D()
{
	EngineBlock eb;
	Call4D( EX_QUIT4D, &eb );
	sErrorCode = eER_NoErr;
}


void PA_Get4DPreferencesFilename( char* fileName )
{
	EngineBlock eb;
	Call4D( EX_GET_PREF_FILE, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, fileName );
}

// return full pathname of 4D preferences folder.
// you can use this folder to store your own preference file
void PA_Get4DPreferencesFolder( char* folderPath )
{
	EngineBlock eb;
	char path[255];

	eb.fParam2 = 'PATH';
	eb.fParam3 = (long)path;
	eb.fParam4 = 255;	// string buffer size

	Call4D( EX_GET_PREF_FILE, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( path, folderPath );
}


// 4D preferences is a Macintosh Resource File, you can add your own
// resources on Macintosh or using Altura's Mac2Win on Windows.
// It is not recommended to modify 4D preferences resources, or to store 
// your preferences in 4D preferences file.
// Better create you own preferences file
short PA_Open4DPreferencesMacResFile()
{
	EngineBlock eb;
	
	eb.fParam1 = 0;
	eb.fManyToOne = 1;
	Call4D( EX_OPEN_CLOSE_PREFS_FILE, &eb );
	sErrorCode = eER_NoErr;

	return (short) eb.fParam1;
}


// each call to PA_OpenPreferencesMacResFile needs to be balanced
// with a call to PA_Close4DPreferences
void PA_Close4DPreferences()
{
	EngineBlock eb;

	eb.fManyToOne = 0;
	Call4D( EX_OPEN_CLOSE_PREFS_FILE, &eb );
	sErrorCode = eER_NoErr;
}


void PA_Get4Dfolder( char* folderPath )
{
	EngineBlock eb;
	Call4D( EX_GET_4D_FOLDER, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, folderPath );
}


PA_PlatformInterface PA_GetPlatformInterface()
{
	EngineBlock eb;
	eb.fParam1 = 0;
	Call4D( EX_GET_PLATFORM_INTERFACE, &eb );
	sErrorCode = eER_NoErr;
	return (PA_PlatformInterface) eb.fParam1;
}


void PA_GetToolBarInfo( char* displayed, short* toolbarHeight )
{
	EngineBlock eb;

	Call4D( EX_GET_TOOLBAR_INFO, &eb );
	sErrorCode = eER_NoErr;

	if ( displayed )
		*displayed = (char) eb.fParam1;

	if ( toolbarHeight )
		*toolbarHeight = (short) eb.fParam2;
}


// 0 : hide, 1: show, 2 : Switch
void PA_ShowHideToolBar( char displayed )
{
	EngineBlock eb;

	eb.fParam1 = displayed;
	Call4D( EX_SHOW_HIDE_TOOLBAR, &eb );
	sErrorCode = eER_NoErr;
}


char PA_GetTipsEnabled()
{
	EngineBlock eb;

	eb.fParam1 = 0;
	Call4D( EX_GET_TIPS_ENABLED, &eb ); // fgr 12/04/2000
	sErrorCode = eER_NoErr;

	return (char) eb.fParam1;
}


void PA_SetTipsEnabled( char enabled )
{
	EngineBlock eb;

	eb.fParam1 = enabled;
	Call4D( EX_SET_TIPS_ENABLED, &eb );
	sErrorCode = eER_NoErr;
}


// when 4D will be short in memory, it will call
// the growZoneHandler procedure you pass to this command.
// The address returned by this function is the address of
// the 4D original growZoneHandler.
// You have to call this address in your growZoneHandler
// procedure after finishing disposing all the memory you can.
void* PA_SetGrowZone( void* growZoneHandler )
{
	void** currentHandlerPtr;
	void* currentHandler;
	EngineBlock eb;

	Call4D( EX_GET_4D_GROW_ZONE, &eb );
	sErrorCode = eER_NoErr;

	currentHandlerPtr = (void**) eb.fHandle;
	currentHandler = *currentHandlerPtr;
	*currentHandlerPtr = growZoneHandler;

	return currentHandler;
}


// this function checks if the stack can hold the amount of stack passed in parameter
// if it can't 4D displays an alert. The amount of stack available is returned.
// if you just want to know the amount of available stack, pass 0 as requested stack
// error code
long PA_CheckFreeStack( long requestedStack )
{
	EngineBlock eb;

	eb.fParam1 = requestedStack;
	eb.fParam2 = 0;
	eb.fError  = 0;
	Call4D( EX_CHECK_FREE_STACK, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam2;
}


void* PA_Get4DWinMacOSGlobals()
{
	EngineBlock eb;
	
	eb.fHandle = 0;
	eb.fError = 0;
	Call4D( EX_GET_4DWIN_MACOS_GLOBALS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (void*) eb.fHandle;
}


// warning : colors components are returned in the range 0 -> 65535
void PA_Get4DSysColors( short index, unsigned short* red, unsigned short* green, unsigned short* blue )
{
	unsigned short* colorPtr;

	EngineBlock eb;
	Call4D( EX_GET_4D_SYS_COLORS, &eb );
	sErrorCode = eER_NoErr;

	if ( index >= 0 && index <= 4 )
	{
		colorPtr = ( (unsigned short**) eb.fHandle )[index];

		if ( red )
			*red = colorPtr[0];

		if ( green )
			*green = colorPtr[1];

		if ( blue )
			*blue = colorPtr[2];
	}
}

void PA_GetCenturyInfo( long* pivotYear, long* defaultCentury )
{
	EngineBlock eb;
	Call4D( EX_GET_CENTURY_INFO, &eb );
	sErrorCode = eER_NoErr;

	*pivotYear      = eb.fParam1;
	*defaultCentury = eb.fParam2;
}


// ---------------------------------------------------------------
// these commands are specific to MacOS plugins.
// PA_Remove4DTraps allow to remove 4D traps (used for multitasking)
// in order to call specific MacOS routines that does 
// not support these traps. Call PA_Use4DTraps immediately
// after your MacOS call, and never give the hand to 4D
// with the traps removed.
// ---------------------------------------------------------------

void PA_Remove4DTraps()
{
	EngineBlock eb;
	Call4D( EX_RESTORE_MACOS_ENV, &eb );
	sErrorCode = eER_NoErr;
}


void PA_Restore4DTraps()
{
	EngineBlock eb;
	Call4D( EX_RESTORE_MAC4D_ENV, &eb );
	sErrorCode = eER_NoErr;
}


void PA_DetrapWaitNextEvent()
{
	EngineBlock eb;
	Call4D( EX_DETRAP_WAIT_NEXT_EVENT, &eb );
	sErrorCode = eER_NoErr;
}


void PA_RetrapWaitNextEvent()
{
	EngineBlock eb;
	Call4D( EX_RETRAP_WAIT_NEXT_EVENT, &eb );
	sErrorCode = eER_NoErr;
}

// on Windows only, returns 4D application HINSTANCE
long PA_Get4DHInstance()
{
	EngineBlock eb;
	Call4D( EX_GET_4D_HINSTANCE, &eb );
	sErrorCode = eER_NoErr;
	return eb.fParam1;
}

long PA_MethodNames( PA_Handle* hnames, PA_Handle* hids )
{
	EngineBlock eb;
	Call4D( EX_METHOD_NAMES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( hnames )
		*hnames = eb.fHandle;
	else if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	if ( hids )
		*hids = (PA_Handle) eb.fParam1;
	else if ( eb.fParam1 )
		PA_DisposeHandle( (PA_Handle) eb.fParam1 );

	return eb.fParam2;
}

// -----------------------------------------
//
// Virtual structure
//
// -----------------------------------------


static short sVirtualStructureMode = 0;


void PA_UseVirtualStructure()
{
	sVirtualStructureMode = 911;
}


void PA_UseRealStructure()
{
	sVirtualStructureMode = 0;
}


char PA_VirtualStructureDefined()
{
	EngineBlock eb;

	Call4D( EX_VIRTUAL_STRUCTURE_DEFINED, &eb );
	sErrorCode = eER_NoErr;

	return (char) eb.fParam1;
}


short PA_GetTrueTableNumber( short virtualTable )
{
	EngineBlock eb;

	eb.fTable = virtualTable;
	Call4D( EX_GET_TRUE_TABLE_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


void PA_GetTrueFieldNumber( short virtualTable, short virtualField, short* trueTable, short* trueField )
{
	EngineBlock eb;

	eb.fTable = virtualTable;
	eb.fField = virtualField;

	Call4D( EX_GET_TRUE_FIELD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( trueTable )
		*trueTable = eb.fTable;

	if ( trueField )
		*trueField = eb.fField;
}


short PA_GetVirtualTableNumber( short trueTable )
{
	EngineBlock eb;

	eb.fTable = trueTable;
	Call4D( EX_GET_VIRTUAL_TABLE_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


void PA_GetVirtualFieldNumber( short trueTable, short trueField, short* virtualTable, short* virtualField )
{
	EngineBlock eb;

	eb.fTable = trueTable;
	eb.fField = trueField;

	Call4D( EX_GET_VIRTUAL_FIELD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( virtualTable )
		*virtualTable = eb.fTable;

	if ( virtualField )
		*virtualField = eb.fField;
}




// -----------------------------------------
//
// automatic relations
//
// -----------------------------------------


static char sManyToOne = 1;
static char sOneToMany = 1;

void PA_UseAutomaticRelations( char manyToOne, char oneToMany )
{
	sManyToOne = manyToOne;
	sOneToMany = oneToMany;
}


// -----------------------------------------
//
// Selection
//
// -----------------------------------------


void PA_CreateRecord( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = 0;

	Call4D( EX_CREATE_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SaveRecord( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = 0;

	Call4D( EX_SAVE_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_NextRecord( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = 0;

	Call4D( EX_NEXT_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_PreviousRecord( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = 0;

	Call4D( EX_PREVIOUS_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

// record is from 1 to number of records in selection
void PA_GotoSelectedRecord( short table, long record )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fRecord    = record - 1;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = 0;

	Call4D( EX_GOTO_SELECTED_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_SelectedRecordNumber( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fRecord    = 0;
	eb.fError     = 0;

	Call4D( EX_SELECTED_RECORD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam1;
}


long PA_RecordsInSelection( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = 0;

	Call4D( EX_RECORDS_IN_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam1;
}


void PA_DeleteSelection( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = 0;

	Call4D( EX_DELETE_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_FirstRecord( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = 0;

	Call4D( EX_FIRST_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_BeforeSelection( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = 0;

	Call4D( EX_BEFORE_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;	
}


char PA_EndSelection( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = 0;

	Call4D( EX_END_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


void PA_GotoRecord( short table, long record )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fRecord = record;
	eb.fError  = 0;

	Call4D( EX_GOTO_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_RecordNumber( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fRecord = 0;
	eb.fError  = 0;

	Call4D( EX_RECORD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fRecord;
}


char PA_Locked( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = 0;

	Call4D( EX_LOCKED, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


void PA_ReadWrite( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = 0;

	Call4D( EX_READ_WRITE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_ReadOnly( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fParam1 = 1;
	eb.fError  = 0;

	Call4D( EX_READ_WRITE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_LoadRecord( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = 0;

	Call4D( EX_LOAD_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RelateOne( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = 0;

	Call4D( EX_RELATE_ONE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RelateMany( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = 0;

	Call4D( EX_RELATE_MANY, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RelateOneSelection( short manyTable, short oneTable )
{
	EngineBlock eb;

	eb.fTable = manyTable;
	eb.fField = oneTable;

	Call4D( EX_RELATE_ONE_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eER_NoErr;
}


void PA_RelateManySelection( short manyTable, short manyField )
{
	EngineBlock eb;

	eb.fTable = manyTable;
	eb.fField = manyField;
	eb.fError = 0;

	Call4D( EX_RELATE_MANY_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// Tables
//
// -----------------------------------------


short PA_CountTables()
{
	EngineBlock	eb;
	
	eb.fParam1 = 0;
	eb.fParam4 = sVirtualStructureMode;

	Call4D( EX_COUNT_TABLES, &eb );
	sErrorCode = (PA_ErrorCode) eER_NoErr;

	return (short) eb.fParam1;
}


void PA_GetTableAndFieldNumbers( char* tableAndFieldNames, short* table, short* field )
{
	EngineBlock	eb;
	
	eb.fTable = 0;
	eb.fField = 0;
	FromUserString( tableAndFieldNames, eb.fName );
	eb.fParam4 = sVirtualStructureMode;
	eb.fError = 0;

	Call4D( EX_GET_TABLE_FIELD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( table )
		*table = eb.fTable;

	if ( field )
		*field = eb.fField;
}


void PA_GetTableName( short table, char* tableName )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError  = 0;

	Call4D( EX_GET_TABLE_NAME, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( tableName )
	{
		if ( sErrorCode == eER_NoErr )
			ToUserString( eb.fName, tableName );
		else
			*tableName = 0;
	}
}


char PA_IsTableVisible( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = 0;

	Call4D( EX_GET_TABLE_NAME, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


void PA_UpdateInternalCache( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fError  = 0;

	Call4D( EX_UPDATE_INTERNAL_CACHE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	
}


// -----------------------------------------
//
// Fields
//
// -----------------------------------------

short PA_CountFields( short table )
{
	EngineBlock	eb;
	
	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = 0;

	Call4D( EX_COUNT_FIELDS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (short) eb.fParam1;
}


void PA_GetFieldName( short table, short field, char* fieldName )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = 0;

	Call4D( EX_FIELD_ATTRIBUTES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( fieldName )
	{
		if ( sErrorCode == eER_NoErr )
			ToUserString( eb.fName, fieldName );
		else
			*fieldName = 0;
	}
}


void PA_GetFieldProperties( short table, short field, PA_FieldKind* kind, short* stringlength, char* indexed, long* attributes )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam1 = eFK_InvalidFieldKind;
	eb.fParam2 = 0;
	eb.fParam3 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = 0;

	Call4D( EX_FIELD_ATTRIBUTES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( kind )
		*kind = (PA_FieldKind) (short) eb.fParam1;

	if ( stringlength )
		*stringlength = (short) eb.fParam2;

	if ( indexed )
		*indexed = (char) eb.fParam3;

	if ( attributes )
		*attributes = eb.fParam4;
}


void PA_GetFieldRelation( short table, short field, short* relatedTable, short* relatedField )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam1 = 0;
	eb.fParam2 = 0;
	eb.fHandle = 0;		// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD_RELATIONS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( relatedTable )
		*relatedTable = (short) eb.fParam1;

	if ( relatedField )
		*relatedField = (short) eb.fParam2;
}


void PA_GetFieldList( short table, short field, char* listName )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;		// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD_RELATIONS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( listName )
	{
		if ( sErrorCode == eER_NoErr )
			ToUserString( eb.fName, listName );
		else
			*listName = 0;
	}
}


void PA_TableAndFieldPopup( short x, short y, short* table, short* field )
{
	EngineBlock eb;

	eb.fTable = 0;
	eb.fField = 0;
	eb.fParam1 = x;
	eb.fParam2 = y;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError  = 0;

	Call4D( EX_POPUP_TABLE_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( table )
		*table = eb.fTable;

	if ( field )
		*field = eb.fField;

}


void PA_FieldPopup( short x, short y, short table, short* field )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fField  = 0;
	eb.fParam1 = x;
	eb.fParam2 = y;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError  = 0;

	Call4D( EX_POPUP_FIELD_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( field )
		*field = eb.fField;
}



// -----------------------------------------
//
//  Order by
//
// -----------------------------------------


void PA_OrderBy( short table, short field, char ascending )
{
	EngineBlock* eb;
	LineBlock* lineblock;
	PA_OrderByRef h;
	
	h = PA_NewHandle( sizeof( EngineBlock ) + sizeof( LineBlock ) );
	if ( h )
	{
		eb = (EngineBlock*) PA_LockHandle( h );
		eb->fTable = table;
		eb->fField = field;
		eb->fNbSearchLines = 1;
		eb->fError  = 0;

		lineblock = (LineBlock*) ( sizeof( EngineBlock ) + (char*) eb );

		lineblock->fTable = table;
		lineblock->fField = field;

		if ( ascending == 0 )
			lineblock->fComparison = (char) eQC_IsLess;
		else
			lineblock->fComparison = (char) eQC_IsGreater;

		Call4D( EX_ORDER_BY, eb );
		sErrorCode = (PA_ErrorCode) eb->fError;

		PA_UnlockHandle( h );
		PA_DisposeHandle( h );
	}
	else
		sErrorCode = eER_NotEnoughMemory;
}


// create an empty PA_OrderByRef
PA_OrderByRef PA_OpenOrderBy( short table )
{
	PA_OrderByRef ref = 0;
	EngineBlock* pt;

	ref = PA_NewHandle( sizeof( EngineBlock ) );
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		pt->fTable  = table;
		pt->fError  = 0;
		pt->fNbSearchLines = 0;
		PA_UnlockHandle( ref );
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_NotEnoughMemory;

	return ref;
}


// add a specific order criteria
void PA_AddFieldToOrderBy( PA_OrderByRef ref, short table, short field, char ascending )
{
	EngineBlock* pt;
	LineBlock* lineblock;
	short nb;
	
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		nb = pt->fNbSearchLines;
		PA_UnlockHandle( ref );

		if ( nb < 32000 )
		{
			if ( PA_SetHandleSize( ref, (long)( PA_GetHandleSize( ref ) + sizeof( LineBlock ) ) ) )
			{
				pt = (EngineBlock*) PA_LockHandle( ref );
				pt->fNbSearchLines++;

				lineblock = (LineBlock*) ( ( nb * (long) sizeof( LineBlock ) ) + sizeof( EngineBlock ) + (char*) pt );

				lineblock->fTable = table;
				lineblock->fField = field;

				if ( ascending == 0 )
					lineblock->fComparison = (char) eQC_IsLess;
				else
					lineblock->fComparison = (char) eQC_IsGreater;
			
				PA_UnlockHandle( ref );
				sErrorCode = eER_NoErr;
			}
			else
				sErrorCode = eER_NotEnoughMemory;
		}
		else
			sErrorCode = eER_TooManyOrderBy;
	}
	else
		sErrorCode = eER_InvalidOrderByRef;
}


// perform the order by, and dispose the PA_OrderBy ref
void PA_CloseOrderBy( PA_OrderByRef ref )
{
	EngineBlock* pt;

	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		Call4D( EX_ORDER_BY, pt );
		sErrorCode = (PA_ErrorCode) pt->fError;

		PA_UnlockHandle( ref );
		PA_DisposeHandle( ref );
	}
	else
		sErrorCode = eER_InvalidOrderByRef;
}


// -----------------------------------------
//
// Query
//
// -----------------------------------------


PA_QueryRef PA_OpenQuery( short table )
{
	PA_QueryRef ref = 0;
	EngineBlock* pt;

	ref = PA_NewHandle( sizeof( EngineBlock ) );
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		pt->fTable = table;
		pt->fError = 0;
		pt->fNbSearchLines = 0;
		PA_UnlockHandle( ref );
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_NotEnoughMemory;

	return ref;
}


static LineBlock* AddQuery( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc )
{
	EngineBlock* pt;
	LineBlock* lineblock = 0;
	short nb;
	
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		nb = pt->fNbSearchLines;
		PA_UnlockHandle( ref );

		if ( nb < 32000 )
		{
			if ( PA_SetHandleSize( ref, (long)( PA_GetHandleSize( ref ) + sizeof( LineBlock ) ) ) )
			{
				pt = (EngineBlock*) PA_LockHandle( ref );
				pt->fNbSearchLines++;

				lineblock = (LineBlock*) ( ( nb * (long) sizeof( LineBlock ) ) + sizeof( EngineBlock ) + (char*) pt );

				lineblock->fTable = table;
				lineblock->fField = field;

				// first query operator should always be eQO_NoOperator
				if ( nb == 0 )
					lineblock->fOperator = (char) eQO_NoOperator;
				else
					lineblock->fOperator = (char) qo;

				lineblock->fComparison = (char) qc;
			}
			else
				sErrorCode = eER_NotEnoughMemory;
		}
		else
			sErrorCode = eER_TooManyQueries;
	}
	else
		sErrorCode = eER_InvalidQueryRef;

	return lineblock;
}


// add a query for a string value
void PA_QueryString( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, char* string )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		FromUserString( string, lineblock->uValue.fString );
		PA_UnlockHandle( ref );
	}
}

// add a query for a real value
void PA_QueryReal( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, double value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fReal = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for a time value
void PA_QueryTime( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, long value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fLongint = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for a date value
void PA_QueryDate( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short day, short month, short year )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fDate.fDay   = day;
		lineblock->uValue.fDate.fMonth = month;
		lineblock->uValue.fDate.fYear  = year;
		PA_UnlockHandle( ref );
	}
}


// add a query for a longint value
void PA_QueryLongint( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, long value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fLongint = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for an integer value
void PA_QueryInteger( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fInteger = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for a Boolean value
void PA_QueryBoolean( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, char value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fBoolean = value;
		PA_UnlockHandle( ref );
	}
}


// stop recording, and perform the query.
void PA_CloseQuery( PA_QueryRef ref )
{
	EngineBlock* pt;

	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		Call4D( EX_QUERY, pt );
		sErrorCode = (PA_ErrorCode) pt->fError;

		PA_UnlockHandle( ref );
		PA_DisposeHandle( ref );
	}
	else
		sErrorCode = eER_InvalidQueryRef;
}



// --------------------------------------------------------------
//
// Get Parameters from a plugin commmand (start at index 1)
//
// --------------------------------------------------------------


short PA_GetShortParameter( PA_PluginParameters params, short index )
{
	return *( ( (short**) params->fParameters ) [ index - 1 ] );
}


long PA_GetLongParameter( PA_PluginParameters params, short index )
{
	return *( ( (long**) params->fParameters ) [ index - 1 ] );
}


double PA_GetDoubleParameter( PA_PluginParameters params, short index )
{
	return *( ( (double**) params->fParameters ) [ index - 1 ] );
}


void PA_GetStringParameter( PA_PluginParameters params, short index, char* string )
{
	ToUserString( ( (char**) params->fParameters ) [ index - 1 ], string );
}


long PA_GetTextParameter( PA_PluginParameters params, short index, char* text )
{
	return ToUserText( ( ( (PA_Text**) params->fParameters ) [ index - 1 ] )->fHandle, text );
}


long PA_GetPictureParameter( PA_PluginParameters params, short index, void* picture, PA_PictureInfo* info )
{
	return ToUserPicture( *( ( (PA_Handle**) params->fParameters ) [ index - 1 ] ), picture, info );
}


PA_Handle PA_GetPictureHandleParameter( PA_PluginParameters params, short index, PA_PictureInfo* info )
{
	PA_Handle hpicture;
	
	hpicture = *( ( (PA_Handle**) params->fParameters ) [ index - 1 ] );
	GetPictureInfo( hpicture, info );

	return hpicture;
}


long PA_GetBlobParameter( PA_PluginParameters params, short index, void* blob )
{
	return ToUserData( *( ( (PA_Handle**) params->fParameters ) [ index - 1 ] ), blob );
}


PA_Handle PA_GetBlobHandleParameter( PA_PluginParameters params, short index )
{
	return *( ( (PA_Handle**) params->fParameters ) [ index - 1 ] );
}


void PA_GetDateParameter( PA_PluginParameters params, short index, short* day, short* month, short* year )
{
	PA_Date* ptdate;

	ptdate = ( (PA_Date**) params->fParameters ) [ index - 1 ];

	if ( day )
		*day   = ptdate->fDay;

	if ( month )
		*month = ptdate->fMonth;

	if ( year )
		*year  = ptdate->fYear;
}


long PA_GetTimeParameter( PA_PluginParameters params, short index )
{
	return *( ( (long**) params->fParameters ) [ index - 1 ] );
}


PA_Variable PA_GetVariableParameter( PA_PluginParameters params, short index )
{
	PA_Variable var;

	var = *( ( (PA_Variable**) params->fParameters ) [ index - 1 ] );
	
	return var;
}


PA_Pointer PA_GetPointerParameter( PA_PluginParameters params, short index )
{
	if ( PA_IsCompiled() )
	{
		PA_Variable* ptvar;

		ptvar = ( (PA_Variable**) params->fParameters ) [ index - 1 ];

		if ( ptvar->fType == eVK_Pointer )
		{
			if ( ptvar->uValue.fPointer )
				return * ptvar->uValue.fPointer;
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return ( (PA_Pointer*) params->fParameters ) [ index - 1 ];
}


PA_PointerKind PA_GetPointerKind( PA_Pointer pointer )
{
	if ( pointer == 0 )
		return ePK_InvalidPointer;

	if ( pointer->fClass == 1 )
		return ePK_PointerToVariable;
	else if ( pointer->uValue.fTableField.fField == 0 )
		return ePK_PointerToTable;
	else
		return ePK_PointerToField;
}


void PA_GetPointerTableField( PA_Pointer pointer, short* table, short* field )
{
	short t, f;

	if ( pointer && pointer->fClass == 0 )
	{
		t = pointer->uValue.fTableField.fTable;
		f = pointer->uValue.fTableField.fField;
	}
	else
		t = f = 0;

	if ( table )
		*table = t;

	if ( field )
		*field = f;
}


PA_Variable PA_GetPointerVariable( PA_Pointer pointer, long* indice )
{
	PA_Variable variable;
	EngineBlock eb;
	char* pt;

	sErrorCode = eER_NoErr;
	
	if ( indice )
		*indice = 0;

	if ( pointer && pointer->fClass == 1 )
	{
		eb.fHandle = (PA_Handle) &variable;
		eb.fError = 0;
		pt = pointer->uValue.fVariable.fName;
		PA_MoveBlock( pt, eb.fName, pt[0] + 1 );

		if ( pointer->fScope == 2 )
			Call4D( EX_GET_INTERPROCESS_VARIABLE, &eb );
		else
			Call4D( EX_GET_VARIABLE, &eb );

		if ( indice )
			*indice = pointer->uValue.fVariable.fIndice;
	
		sErrorCode = (PA_ErrorCode) eb.fError;
	}
	else
		variable.fType = eVK_Undefined;

	return variable;
}



// ---------------------------------------------------------------
//
// Set Parameters from a plugin command (start at index 1)
//
// ---------------------------------------------------------------


void PA_SetShortParameter( PA_PluginParameters params, short index, short value )
{
	*( ( (short**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetLongParameter( PA_PluginParameters params, short index, long value )
{
	*( ( (long**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetDoubleParameter( PA_PluginParameters params, short index, double value )
{
	*( ( (double**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetStringParameter( PA_PluginParameters params, short index, char* string )
{
	FromUserString( string, ( (char**) params->fParameters ) [ index - 1 ] );
}


void PA_SetTextParameter( PA_PluginParameters params, short index, char* text, long len )
{
	PA_Text* pttext;
	
	pttext = ( (PA_Text**) params->fParameters ) [ index - 1 ];
	
	if ( pttext->fHandle )
		PA_DisposeHandle( pttext->fHandle );
		
	pttext->fHandle = FromUserText( text, &len );
	pttext->fSize = (short) len;
}


void PA_SetPictureParameter( PA_PluginParameters params, short index, void* picture, long len, PA_PictureInfo info )
{
	PA_Handle* pth;
	
	pth = ( (PA_Handle**) params->fParameters ) [ index - 1 ];

	if ( *pth )
		PA_DisposeHandle( *pth );

	*pth = FromUserPicture( picture, len, info );
}


void PA_SetPictureHandleParameter( PA_PluginParameters params, short index, PA_Handle hpicture, PA_PictureInfo info ) 
{
	PA_Handle* pth;
	
	pth = ( (PA_Handle**) params->fParameters ) [ index - 1 ];

	if ( *pth && ( *pth != hpicture ) )
		PA_DisposeHandle( *pth );

	*pth = FromUserPictureHandle( hpicture, info );
}


void PA_SetBlobParameter( PA_PluginParameters params, short index, void* blob, long len )
{
	PA_Handle* pth;
	
	pth = ( (PA_Handle**) params->fParameters ) [ index - 1 ];

	if ( *pth )
		PA_DisposeHandle( *pth );

	*pth = FromUserData( blob, len );
}


void PA_SetBlobHandleParameter( PA_PluginParameters params, short index, PA_Handle hblob )
{
	PA_Handle* pth;
	
	pth = ( (PA_Handle**) params->fParameters ) [ index - 1 ];

	if ( *pth && ( *pth != hblob ) )
		PA_DisposeHandle( *pth );

	*pth = hblob;
}


void PA_SetDateParameter( PA_PluginParameters params, short index, short day, short month, short year )
{
	PA_Date* ptdate;

	ptdate = ( (PA_Date**) params->fParameters ) [ index - 1 ];

	ptdate->fDay   = day;
	ptdate->fMonth = month;
	ptdate->fYear  = year;
}


void PA_SetTimeParameter( PA_PluginParameters params, short index, long value )
{
	*( ( (long**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetVariableParameter( PA_PluginParameters params, short index, PA_Variable variable, char clearOldValue )
{
	// set clearOldValue when you create the new 4D variable using PA_CreateVariable.
	// in this case, you need to dispose the previous content of the 4D variable
	if ( clearOldValue )
		PA_ClearVariable( ( (PA_Variable**) params->fParameters) [ index - 1 ] );

	*( ( (PA_Variable**) params->fParameters ) [ index - 1 ] ) = variable;
}


// -----------------------------------------
//
// Return a value from a plugin command
//
// -----------------------------------------


void PA_ReturnShort( PA_PluginParameters params, short value )
{
	*(long*) params->fResult = value;
}


void PA_ReturnLong( PA_PluginParameters params, long value )
{
	*(long*) params->fResult = value;
}


void PA_ReturnDouble( PA_PluginParameters params, double value )
{
	**(double**) params->fResult = value;
}


void PA_ReturnString( PA_PluginParameters params, char* string )
{
	FromUserString( string, *(char**) params->fResult );
}


void PA_ReturnText( PA_PluginParameters params, char* text, long len )
{
	PA_Text* pttext;
	
	pttext = *(PA_Text**) params->fResult;
	
	if ( pttext->fHandle )
		PA_DisposeHandle( pttext->fHandle );
		
	pttext->fHandle = FromUserText( text, &len );
	pttext->fSize = (short) len;
}


void PA_ReturnPicture( PA_PluginParameters params, void* picture, long len, PA_PictureInfo info )
{
	*(PA_Handle*) params->fResult = FromUserPicture( picture, len, info );
}


void PA_ReturnPictureHandle( PA_PluginParameters params, PA_Handle hpicture, PA_PictureInfo info )
{
	*(PA_Handle*) params->fResult = FromUserPictureHandle( hpicture, info );
}


void PA_ReturnBlob( PA_PluginParameters params, void* blob, long len )
{
	*(PA_Handle*) params->fResult = FromUserData( blob, len );
}


void PA_ReturnBlobHandle( PA_PluginParameters params, PA_Handle hblob )
{
	*(PA_Handle*) params->fResult = hblob;
}


void PA_ReturnDate( PA_PluginParameters params, short day, short month, short year )
{
	PA_Date* ptdate;

	ptdate = *(PA_Date**) params->fResult;

	ptdate->fDay   = day;
	ptdate->fMonth = month;
	ptdate->fYear  = year;
}


void PA_ReturnTime( PA_PluginParameters params, long value )
{
	*(long*) params->fResult = value;
}



// -----------------------------------------
//
// Get events in a plugin area
//
// -----------------------------------------

PA_AreaEvent PA_GetAreaEvent( PA_PluginParameters params )
{
	return (PA_AreaEvent) ( ( (PA_Event**) params->fParameters )[ 0 ] )->fWhat;
}


PA_Rect PA_GetAreaRect( PA_PluginParameters params )
{
	return *(PA_Rect*) ( ( (PA_Rect**) params->fParameters )[ 1 ] );
}


void PA_GetAreaName( PA_PluginParameters params, char* name )
{
	ToUserString( (char*) ( ( (char**) params->fParameters )[ 2 ] ), name );
}


void PA_GetPluginProperties( PA_PluginParameters params, PA_PluginProperties* properties )
{
	PA_Event* event;
	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_InitArea || event->fWhat == eAE_InitAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		if ( properties && event->fMessage )
			*properties = *(PA_PluginProperties*) ( event->fMessage );

		event->fModifiers = 0x600;	// tell 4D that message is handled
	}
	else if ( event->fWhat == eAE_DesignUpdate )
	{
		sErrorCode = eER_NoErr;
		if ( properties && event->fWhen )
			*properties = *(PA_PluginProperties*)( event->fWhen );
	}
	else if ( event->fWhat == eAE_EditAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		if ( properties && event->fMessage )
			*properties = *(PA_PluginProperties*) ( event->fMessage );
	}
	else
		sErrorCode = eER_BadEventCall;
}


// need to be called when area event == eAE_EditAdvancedProperties to give 4D the private data
// you want to keep. Area event eAE_EditAdvancedProperties will be followed by the 
// eAE_DisposeAdvancedProperties to tell you to release the buffer given to 4D.
void PA_SetAdvancedProperties( PA_PluginParameters params, void* data, long datasize )
{
	PA_PluginProperties* properties;
	PA_Event* event;
	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_EditAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		properties = (PA_PluginProperties*) ( event->fMessage );
		if ( properties )
		{
			properties->fAdvancedProperties = data;
			properties->fAdvancedPropertiesSize = datasize;
		
			event->fModifiers = 0x0600;
		}
		else
			event->fModifiers = 0;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called after a eAE_DisposeAdvancedProperties event. Gives you back the
// pointer passed in PA_SetPluginParameters, so you can dispose the memory.
void* PA_GetAdvancedPropertiesToDispose( PA_PluginParameters params )
{
	void* data;
	PA_Event* event;

	data = 0;
	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_DisposeAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		data = (void*)(event->fMessage);	
	}
	else
		sErrorCode = eER_BadEventCall;

	return data;
}


void* PA_GetAdvancedProperties( PA_PluginProperties* properties, long* datasize )
{
	if ( properties )
	{
		if ( datasize )
			*datasize = properties->fAdvancedPropertiesSize;

		return properties->fAdvancedProperties;
	}
	else
	{
		if ( datasize )
			*datasize = 0;
	
		return 0;
	}
}


// to be called when AreaEvent == eAE_InitArea
void PA_SetAreaReference( PA_PluginParameters params, void* ref )
{	
	*( ( (void***) params->fParameters )[ 3 ] ) = ref;
}


// to get back the reference passed at init
void* PA_GetAreaReference( PA_PluginParameters params )
{
	return *( ( (void***) params->fParameters )[ 3 ] );
}

// to be called when AreaEvent == eAE_IsFocusable
void PA_SetAreaFocusable( PA_PluginParameters params, char focusable )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_IsFocusable )
	{
		sErrorCode = eER_NoErr;
		event->fMessage = focusable ? 101 : 0;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called when AreaEvent == eAE_Select to know if the window is also activated
char PA_IsActivated( PA_PluginParameters params )
{
	PA_Event* event;
	char activated = 0;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_Select )
	{
		sErrorCode = eER_NoErr;
		activated = (char)( ( event->fWhen == 1 ) ? 1 : 0 );
	}
	else
		sErrorCode = eER_BadEventCall;

	return activated;
}


// to be called when AreaEvent == eAE_Deselect to know if the window is also deactivated
char PA_IsDeActivated( PA_PluginParameters params )
{
	PA_Event* event;
	char deactivated = 0;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_Deselect )
	{
		sErrorCode = eER_NoErr;
		deactivated = (char)( ( event->fWhen == 1 ) ? 1 : 0 );
	}
	else
		sErrorCode = eER_BadEventCall;

	return deactivated;
}


// can be called when AreaEvent == eAE_Select
void PA_AcceptSelect( PA_PluginParameters params, char accept )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_Select )
	{
		sErrorCode = eER_NoErr;
		event->fMessage = accept ? 0 : 103;
	}
	else
		sErrorCode = eER_BadEventCall;
}



// can be called when AreaEvent == eAE_Deselect
void PA_AcceptDeselect( PA_PluginParameters params, char accept )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_Deselect )
	{
		sErrorCode = eER_NoErr;
		event->fMessage = accept ? 0 : 103;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called when AreaEvent == eAE_Drop or eAE_AllowDrop
PA_DragAndDropInfo PA_GetDragAndDropInfo( PA_PluginParameters params )
{
	PA_Event* event;
	PA_DragAndDropInfo dropinfo;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_Drop || event->fWhat == eAE_AllowDrop )
	{
		sErrorCode = eER_NoErr;
		dropinfo = *(PA_DragAndDropInfo*) event->fMessage;
	}
	else
		sErrorCode = eER_BadEventCall;

	return dropinfo;
}

// to be called when AreaEvent == eAE_Drag to have information on dragging position of object
// you received informations on object itself when you receive eAE_AllowDrop
void PA_GetDragPositions( PA_PluginParameters params, PA_Rect* rect, short* x, short* y )
{
	PA_Event* event;
	PA_Rect r = {0,0,0,0};
	short h = 0;
	short v = 0;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_Drag )
	{
		sErrorCode = eER_NoErr;
		r = *(PA_Rect*) ( event->fMessage );
		h = event->fWhereH;
		v = event->fWhereV;
	}
	else
		sErrorCode = eER_BadEventCall;
	
	if ( x )
		*x = h;

	if ( y )
		*y = v;

	if ( rect )
		*rect = r;
}


void PA_DragAndDrop( short startX, short startY, char useCustomRect, PA_Rect customRect )
{
	EngineBlock eb;
	eb.fParam1 = (long) &customRect;
	eb.fParam2 = useCustomRect ? 0 : -1;
	eb.fParam3 = startX;
	eb.fParam4 = startY;

	Call4D( EX_DRAG_AND_DROP, &eb );
	sErrorCode = eER_NoErr;
}

// to be called when AreaEvent == eAE_Drag to tell 4D you will handle yourself
// the way your area will show that something is dragging over.
// Otherwise, 4D will highlight a 2 pixels frame around your area if you tell that it allows drop.
void PA_CustomizeDragOver( PA_PluginParameters params )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_Drag )
	{
		sErrorCode = eER_NoErr;
		event->fModifiers = 1;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called when AreaEvent == eAE_AllowDrop to accept or refuse drop
void PA_AllowDrop( PA_PluginParameters params, char allow )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_AllowDrop )
	{
		sErrorCode = eER_NoErr;
		event->fModifiers = (short)( allow ? 0 : 1 );
	}
	else
		sErrorCode = eER_BadEventCall;
}


// Getting informations on a Drag And Drop Info structure
PA_DragKind PA_GetDragAndDropKind( PA_DragAndDropInfo info )
{
	if ( info.fVariableName[0] )
		return eDK_DragVariable;
	else if ( info.fField )
		return eDK_DragField;
	else if ( info.fTable )
		return eDK_DragTable;
	else
		return eDK_InvalidDrag;
}


PA_Variable PA_GetDragAndDropVariable( PA_DragAndDropInfo info, long* indice )
{
	PA_Variable variable;
	EngineBlock eb;

	sErrorCode = eER_NoErr;
	
	if ( indice )
		*indice = 0;

	if ( info.fVariableName[0] )
	{
		eb.fHandle = (PA_Handle) &variable;
		eb.fError = 0;
		PA_MoveBlock( info.fVariableName, eb.fName, info.fVariableName[0] + 1 );

		if ( info.fInterProcess )
			Call4D( EX_GET_INTERPROCESS_VARIABLE, &eb );
		else
			Call4D( EX_GET_VARIABLE, &eb );

		if ( indice )
			*indice = info.fFromArrayIndice;
	
		sErrorCode = (PA_ErrorCode) eb.fError;
	}
	else
		variable.fType = eVK_Undefined;

	return variable;
}


void PA_GetDragAndDropTableField( PA_DragAndDropInfo info, short* table, short* field )
{
	if ( table )
		*table = info.fTable;

	if ( field )
		*field = info.fField;
}


// to be called on Windows when AreaEvent == eAE_Update
long PA_GetUpdateHDC()
{
	EngineBlock eb;
	eb.fHandle = 0;
	eb.fParam1 = 0;
	eb.fError  = 0;
	Call4D( EX_GET_HWND, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	return (long) eb.fParam1;
}


// to be called when AreaEvent == eAE_MouseDown, eAE_MouseUp or eAE_WebClick
void PA_GetClick( PA_PluginParameters params, short* x, short* y )
{
	short h, v;
	PA_Event* event;

	h = v = 0;
	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_MouseDown || event->fWhat == eAE_MouseUp || event->fWhat == eAE_WebClick )
	{
		sErrorCode = eER_NoErr;
		h = event->fWhereH;
		v = event->fWhereV;
	}
	else
		sErrorCode = eER_BadEventCall;

	if ( x )
		*x = h;

	if ( y )
		*y = v;
}

// to be called when AreaEvent == eAE_KeyDown
char PA_GetKey( PA_PluginParameters params )
{
	char c;
	PA_Event* event;

	c = 0;
	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_KeyDown || event->fWhat == eAE_AutoKey )
	{
		sErrorCode = eER_NoErr;
		c = (char) ( 0x000000FF & event->fMessage );
		if ( sUserCharSet == eCS_Ansi )
		{
			char macstring[2];
			char ansistring[4];

			macstring[0] = c;
			macstring[1] = 0;

			ansistring[0] = 0;

			PA_ConvertStrings( macstring,  eSK_CString, eCS_Macintosh,
							   ansistring, eSK_CString, eCS_Ansi );

			c = ansistring[0];
		}
	}
	else
		sErrorCode = eER_BadEventCall;

	return c;
}

// to be called if AreaEvent == eAE_DesignUpdate 
// if you want to draw yourself your plugin in design mode
void PA_CustomizeDesignMode( PA_PluginParameters params )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_DesignUpdate )
	{
		sErrorCode = eER_NoErr;
		event->fMessage = 102;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called if AreaEvent == eAE_GetMenuIcon 
// to give the id of the 'cicn' resource that will appears
// beside the area name in plugin area popup of the the tools palette.
void PA_SetMenuIcon( PA_PluginParameters params, short id )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_GetMenuIcon )
	{
		sErrorCode = eER_NoErr;
		event->fMessage = 604;
		event->fWhen = (long) id;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called when AreaEvent == eAE_ArePropertiesEditable
// to tell 4D you want to use the "Advanced Properties" Button
void PA_SetAdvancedPropertiesEditable( PA_PluginParameters params, char editable )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( event->fWhat == eAE_AreAdvancedPropertiesEditable )
	{
		sErrorCode = eER_NoErr;
		if ( editable )
			event->fMessage = 603;
		else
			event->fMessage = 0;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// if you don't take an area event, call this routine to tell 4D to handle it.
void PA_DontTakeEvent( PA_PluginParameters params )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];
	event->fModifiers = 112;
}


void PA_CallPluginAreaMethod( PA_PluginParameters params )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_MouseDown || event->fWhat == eAE_AutoKey || event->fWhat == eAE_KeyDown || event->fWhat == eAE_Idle )
	{
		event->fMessage = 301;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

void PA_GotoNextField( PA_PluginParameters params )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_MouseDown || event->fWhat == eAE_AutoKey || event->fWhat == eAE_KeyDown || event->fWhat == eAE_Idle )
	{
		event->fMessage = 110;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

void PA_GotoPreviousField( PA_PluginParameters params )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_MouseDown || event->fWhat == eAE_AutoKey || event->fWhat == eAE_KeyDown || event->fWhat == eAE_Idle )
	{
		event->fMessage = 111;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_WebPublish
void PA_PublishWebPicture( PA_PluginParameters params, char pictureMap )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_WebPublish )
	{
		event->fModifiers = (short)( ( pictureMap == 0 ) ? 2 : 3 );
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called at eAE_WebPublish
void PA_SendHTML( PA_PluginParameters params, void* webData, char* HTMLbuffer, long len )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_WebPublish )
	{
		event->fMessage = (long) HTMLbuffer;
		event->fWhen = len;
		*(void**)( & event->fWhereV ) = webData;
		event->fModifiers = 1;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_WebPublishPicture
void PA_SendWebPicture( PA_PluginParameters params, void* webData, void* picture, long len, PA_WebPictureKind kind )
{
	PA_Event* event;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_WebPublish )
	{
		event->fMessage = (long) picture;
		event->fWhen = len;
		*(void**)( & event->fWhereV ) = webData;
		event->fModifiers = (short) kind;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_WebDisposeData
void* PA_GetWebDataToDispose( PA_PluginParameters params )
{
	PA_Event* event;
	void* data = 0;

	event = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( event->fWhat == eAE_WebDisposeData )
	{
		data = * (void**) ( & event->fWhereH );
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;

	return data;
}


// -----------------------------------------
//
// Get fields from database
//
// -----------------------------------------

void PA_GetStringField( short table, short field, char* string )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fString[0] = 0;
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( string )
	{
		if ( sErrorCode == eER_NoErr )
			ToUserString( eb.fString, string );
		else
			*string = 0;
	}
}

// warning, be sure to have a large enough buffer to receive the text
long PA_GetTextField( short table, short field, char* text )
{
	EngineBlock	eb;
	long len = 0;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( text )
	{
		if ( sErrorCode == eER_NoErr )
			len = ToUserText( eb.fHandle, text );
	}
	return len;
}

// if blob size is unknow, call the function a first time with buffer equal to zero,
// the value returned will be the size of the needed buffer.
// Allocate the buffer and call again the function with the correct buffer
long PA_GetBlobField( short table, short field, void* blob )
{
	EngineBlock	eb;
	long len = 0;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		len = ToUserData( eb.fHandle, blob );

	return len;
}


PA_Handle PA_GetBlobHandleField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		return eb.fHandle;
	else
		return 0;
}


long PA_GetPictureField ( short table, short field, void* picture, PA_PictureInfo* info )
{
	EngineBlock	eb;
	long len = 0;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		len = ToUserPicture( eb.fHandle, picture, info );

	return len;
}


PA_Handle PA_GetPictureHandleField ( short table, short field, PA_PictureInfo* info )
{
	EngineBlock	eb;
	long len = 0;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	GetPictureInfo( eb.fHandle, info );

	if ( sErrorCode == eER_NoErr )
		return eb.fHandle;
	else
		return 0;
}


double PA_GetRealField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fReal = 0;
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fReal;
}


long PA_GetLongintField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable   = table;
	eb.fField   = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint = 0;
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fLongint;
}


long PA_GetTimeField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint  = 0;
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fLongint;
}


short PA_GetIntegerField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fShort = 0;
	eb.fError  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fShort;
}


void PA_GetDateField( short table, short field, short* day, short* month, short* year )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	eb.fDate.fDay   = 0;
	eb.fDate.fMonth = 0;
	eb.fDate.fYear  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( day )
		*day = eb.fDate.fDay;

	if ( month )
		*month = eb.fDate.fMonth;

	if ( year )
		*year = eb.fDate.fYear;
}

char PA_GetBooleanField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;
	eb.fShort = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fShort;
}


// -----------------------------------------
//
// Set fields in database
//
// -----------------------------------------


void PA_SetStringField( short table, short field, char* string )
{
	EngineBlock	eb;
	
  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;
	FromUserString( string, eb.fString );

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetTextField( short table, short field, char* text, long len )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	eb.fText.fHandle = FromUserText( text, &len );
	eb.fText.fSize = (short) len;
	
	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetBlobField( short table, short field, void* blob, long len )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;
	eb.fText.fSize = -1;
	eb.fText.fHandle = FromUserData( blob, len );

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetBlobHandleField( short table, short field, PA_Handle hblob )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;
	eb.fText.fSize = -1;
	eb.fText.fHandle = hblob;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetPictureField ( short table, short field, void* picture, long len, PA_PictureInfo info )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;
	eb.fText.fSize = -1;
	eb.fText.fHandle = FromUserPicture( picture, len, info );

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetPictureHandleField ( short table, short field, PA_Handle hpicture, PA_PictureInfo info )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;
	eb.fText.fSize = -1;
	eb.fText.fHandle = FromUserPictureHandle( hpicture, info );

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetRealField( short table, short field, double value )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fReal  = value;
	eb.fError  = 0;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetLongintField( short table, short field, long value )
{
	EngineBlock	eb;

  	eb.fTable   = table;
	eb.fField   = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint = value;
	eb.fError  = 0;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetIntegerField( short table, short field, short value )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fShort = value;
	eb.fError  = 0;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetTimeField( short table, short field, long value )
{
	EngineBlock	eb;

  	eb.fTable   = table;
	eb.fField   = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint = value;
	eb.fError  = 0;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetDateField( short table, short field, short day, short month, short year )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetBooleanField( short table, short field, char value )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fShort = value;
	eb.fHandle = 0;	// subtable
	eb.fError  = 0;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4th Dimension Variables
//
// -----------------------------------------


PA_Variable PA_CreateVariable( PA_VariableKind kind, short stringSize )
{
	PA_Variable variable;
	char* pt;

	variable.fType = (char) kind;
	switch ( kind )
	{
		case eVK_Real :
			variable.uValue.fReal = 0.0;
			break;

		case eVK_Text :
			variable.uValue.fText.fHandle = PA_NewHandle( 0 );
			variable.uValue.fText.fSize = 0;
			break;
		
		case eVK_Date:
			variable.uValue.fDate.fDay   = 0;
			variable.uValue.fDate.fMonth = 0;
			variable.uValue.fDate.fYear  = 0;
			break;
		
		case eVK_Boolean :
		case eVK_Longint :
			variable.uValue.fLongint = 0;
			break;

		case eVK_Time :
			variable.uValue.fTime = 0;
			break;
			
		case eVK_Picture :
			variable.uValue.fPicture.fHandle = PA_NewHandle( 0 );
			variable.uValue.fPicture.fSize = 0;
			break;

		case eVK_Blob :
			variable.uValue.fBlob.fHandle = PA_NewHandle( 0 );
			variable.uValue.fBlob.fSize = 0;
			break;

		case eVK_ArrayReal :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( double ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(double*) pt = 0.0;
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;
		
		case eVK_ArrayBoolean :
		case eVK_ArrayInteger :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( short ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(short*) pt = 0;
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayLongint :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( long ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(long*) pt = 0;
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayDate :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Date ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
			{
				( (PA_Date*) pt )->fDay = 0;
				( (PA_Date*) pt )->fMonth = 0;
				( (PA_Date*) pt )->fYear = 0;
			}
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayText :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Text ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
			{
				( (PA_Text*) pt )->fHandle = PA_NewHandle( 0 );
				( (PA_Text*) pt )->fSize = 0;
			}
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayPicture :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Handle ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(PA_Handle*) pt  = PA_NewHandle( 0 );
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayString :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( (long) ( sizeof( short ) + stringSize ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
			{
				*(short*) pt = stringSize;
				*(char*) ( pt + (long) sizeof( short ) ) = 0;
			}
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_String :
			variable.uValue.fString.fSize = stringSize;
			variable.uValue.fString.fString[ 0 ] = 0;
			break;
	}
	return variable;
}


PA_Variable PA_GetVariable( char* variableName )
{
	PA_Variable vb;
	EngineBlock eb;
	char interprocess;

	eb.fHandle = (PA_Handle) &vb;
	eb.fError = 0;

	FromUserString( variableName, eb.fName );
	interprocess = StripInterprocessVariableName( eb.fName );

	if ( interprocess )
		Call4D( EX_GET_INTERPROCESS_VARIABLE, &eb );
	else
		Call4D( EX_GET_VARIABLE, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return vb;
}



PA_VariableKind PA_GetVariableKind( PA_Variable variable )
{
	return (PA_VariableKind) variable.fType;
}


void PA_GetStringVariable( PA_Variable variable, char* string )
{
	if ( string && variable.fType == eVK_String )
		ToUserString( variable.uValue.fString.fString, string );
}


// warning, be sure to have a large enough buffer to receive the text
long PA_GetTextVariable( PA_Variable variable, char* text )
{
	long len = 0;

	if ( variable.fType == eVK_Text )
		len = ToUserText( variable.uValue.fText.fHandle, text );

	return len;
}


// if blob size is unknow, call the function a first time with buffer equal to zero,
// the value returned will be the size of the needed buffer.
// Allocate the buffer and call again the function with the correct buffer
long PA_GetBlobVariable( PA_Variable variable, void* blob )
{
	long len = 0;

	if ( variable.fType == eVK_Blob )
		len = ToUserData( variable.uValue.fBlob.fHandle, blob );

	return len;
}


// the handle still belongs to the variable, you cannot dispose it.
PA_Handle PA_GetBlobHandleVariable( PA_Variable variable )
{
	if ( variable.fType == eVK_Blob )
		return variable.uValue.fBlob.fHandle;
	else
		return 0;
}


// blob and pictures are basically the same
long PA_GetPictureVariable( PA_Variable variable, void* picture, PA_PictureInfo* info )
{
	long len = 0;

	if ( variable.fType == eVK_Picture )
		len = ToUserPicture( variable.uValue.fPicture.fHandle, picture, info );

	return len;
}


// the handle still belongs to the variable, you cannot dispose it.
PA_Handle PA_GetPictureHandleVariable( PA_Variable variable, PA_PictureInfo* info )
{
	PA_Handle hpicture = 0;	
	
	if ( variable.fType == eVK_Picture )
		hpicture = variable.uValue.fPicture.fHandle;
		
	GetPictureInfo( hpicture, info );

	return hpicture;
}


double PA_GetRealVariable( PA_Variable variable )
{
	double value = 0.0;

	if ( variable.fType == eVK_Real )
		value = variable.uValue.fReal;

	return value;
}


long PA_GetLongintVariable( PA_Variable variable )
{
	long value = 0;

	if ( variable.fType == eVK_Longint )
		value = variable.uValue.fLongint;

	return value;
}


long PA_GetTimeVariable( PA_Variable variable )
{
	long value = 0;

	if ( variable.fType == eVK_Time )
		value = variable.uValue.fTime;

	return value;
}


void PA_GetDateVariable( PA_Variable variable, short* day, short* month, short* year )
{
	if ( variable.fType == eVK_Date )
	{
		if ( day )
			*day = variable.uValue.fDate.fDay;

		if ( month )
			*month = variable.uValue.fDate.fMonth;

		if ( year )
			*year = variable.uValue.fDate.fYear;
	}
}


char PA_GetBooleanVariable( PA_Variable variable )
{
	char value = 0;

	if ( variable.fType == eVK_Boolean )
		value = variable.uValue.fBoolean;

	return value;
}


// -----------------------------------------
//
// Set 4th Dimension variables
//
// -----------------------------------------


void PA_SetVariable( char* variableName, PA_Variable variable, char clearOldValue )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) &variable;
	eb.fError  = 0;

	eb.fClearOldVariable = clearOldValue;

	FromUserString( variableName, eb.fName );
	
	if ( StripInterprocessVariableName( eb.fName ) )
		Call4D( EX_SET_INTERPROCESS_VARIABLE, &eb );
	else
		Call4D( EX_SET_VARIABLE, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetStringVariable( PA_Variable* variable, char* string )
{
	variable->fType = eVK_String;
	variable->uValue.fString.fSize = (short) 256;
	FromUserString( string, variable->uValue.fString.fString );
}


void PA_SetTextVariable( PA_Variable* variable, char* text, long len )
{
	variable->fType = eVK_Text;
	variable->uValue.fText.fHandle = FromUserText( text, &len );
	variable->uValue.fText.fSize = (short) len;
}


void PA_SetBlobVariable( PA_Variable* variable, void* blob, long len )
{
	variable->fType = eVK_Blob;
	variable->uValue.fBlob.fHandle = FromUserData( blob, len );
	variable->uValue.fBlob.fSize = -1;
}


void PA_SetBlobHandleVariable( PA_Variable* variable, PA_Handle hblob )
{
	variable->fType = eVK_Blob;
	variable->uValue.fBlob.fHandle = hblob;
	variable->uValue.fBlob.fSize = -1;
}


void PA_SetPictureVariable( PA_Variable* variable, void* picture, long len, PA_PictureInfo info )
{
	variable->fType = eVK_Picture;
	variable->uValue.fPicture.fHandle = FromUserPicture( picture, len, info );
	variable->uValue.fPicture.fSize = -1;
}


void PA_SetPictureHandleVariable( PA_Variable* variable, PA_Handle hpicture, PA_PictureInfo info )
{
	variable->fType = eVK_Picture;
	variable->uValue.fPicture.fHandle = FromUserPictureHandle( hpicture, info );
	variable->uValue.fPicture.fSize = -1;
}
 

void PA_SetRealVariable( PA_Variable* variable, double value )
{
	variable->fType = eVK_Real;
	variable->uValue.fReal = value;
}


void PA_SetLongintVariable( PA_Variable* variable, long value )
{
	variable->fType = eVK_Longint;
	variable->uValue.fLongint = value;
}


void PA_SetTimeVariable( PA_Variable* variable, long value )
{
	variable->fType = eVK_Time;
	variable->uValue.fTime = value;
}


void PA_SetDateVariable( PA_Variable* variable, short day, short month, short year )
{
	variable->fType = eVK_Date;
	variable->uValue.fDate.fDay   = day;
	variable->uValue.fDate.fMonth = month;
	variable->uValue.fDate.fYear  = year;
}


void PA_SetBooleanVariable( PA_Variable* variable, char value )
{
	variable->fType = eVK_Boolean;
	variable->uValue.fBoolean = value;
}


void PA_ClearVariable( PA_Variable* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) variable;
	Call4D( EX_CLEAR_VARIABLE, &eb );
	sErrorCode = eER_NoErr;
}


// -----------------------------------------
//
// 4th Dimension arrays
//
// -----------------------------------------


long PA_GetArrayNbElements( PA_Variable array )
{
	if ( array.fType >= eVK_ArrayOfArray && array.fType <= eVK_ArrayBoolean )
		return array.uValue.fArray.fNbElements;
	else
		return -1;
}


void PA_ResizeArray( PA_Variable *array, long nb )
{
	long		size = 0;
	long		nb1 = nb + 1;	// we have to count element number zero
	long		i;
	PA_Text		*ptText;
	PA_Handle	*ptPictureHandle;
	char		*pt;
	char		*ptmax;
	long		oldSize;

	if (    nb >= 0 
		 && array->fType >= eVK_ArrayOfArray
		 && array->fType <= eVK_ArrayBoolean
		 && array->uValue.fArray.fNbElements != nb
		 && array->uValue.fArray.fData
	   )
	{
		switch ( array->fType )
		{
			case eVK_ArrayReal :
				size = nb1 * (long) sizeof( double );
				break;

			case eVK_ArrayInteger :
				size = nb1 * (long) sizeof( short );
				break;

			case eVK_ArrayLongint :
				size = nb1 * (long) sizeof( long );
				break;
			
			case eVK_ArrayDate :
				size = nb1 * (long) sizeof( PA_Date );
				break;

			case eVK_ArrayText :
				// dispose in memory text handles that will be removed
				// if array become smaller
				if ( nb < array->uValue.fArray.fNbElements )
				{
					ptText = (PA_Text *)PA_LockHandle( array->uValue.fArray.fData );

					for ( i = nb + 1; i <= array->uValue.fArray.fNbElements; i++ )
						PA_DisposeHandle( ptText[ i ].fHandle );
						
					PA_UnlockHandle( array->uValue.fArray.fData );
				}
				size = nb1 * (long) sizeof( PA_Text );
				break;

			case eVK_ArrayPicture :
				// dispose in memory picture handles that will be removed
				// if array become smaller
				if ( nb < array->uValue.fArray.fNbElements )
				{
					ptPictureHandle = (PA_Handle *)PA_LockHandle( array->uValue.fArray.fData );

					for ( i = nb + 1; i <= array->uValue.fArray.fNbElements; i++ )
						PA_DisposeHandle( ptPictureHandle[ i ] );
						
					PA_UnlockHandle( array->uValue.fArray.fData );
				}
				size = nb1 * (long) sizeof( PA_Handle );
				break;

			case eVK_ArrayString :
				size = ( nb1 * (long) PA_GetArrayStringSize( *array ) ) + (long) sizeof( short );
				break;

			case eVK_ArrayBoolean :
				size = ( nb1 + 7 ) / 8;
				break;
		}

		oldSize = PA_GetHandleSize( array->uValue.fArray.fData );

		if ( PA_SetHandleSize( array->uValue.fArray.fData, size ) )
		{
			// fill with zero the new allocated bytes
			if ( oldSize < size )
			{
				pt = PA_LockHandle( array->uValue.fArray.fData );

				ptmax = pt + size;
				pt += oldSize;
				
				while ( pt < ptmax )
					*pt++ = 0;

				PA_UnlockHandle( array->uValue.fArray.fData );
			}
			array->uValue.fArray.fNbElements = nb;
		}
	}
}


long PA_GetArrayCurrent( PA_Variable array )
{
	if ( array.fType >= eVK_ArrayOfArray && array.fType <= eVK_ArrayBoolean )
		return array.uValue.fArray.fCurrent;
	else
		return -1;
}


void PA_SetArrayCurrent( PA_Variable* array, long current )
{
	if ( array->fType >= eVK_ArrayOfArray && array->fType <= eVK_ArrayBoolean )
		array->uValue.fArray.fCurrent = (short) current;
}


short PA_GetArrayStringSize( PA_Variable array )
{
	short size = 0;
	
	if ( array.fType == eVK_ArrayString && array.uValue.fArray.fData  )
		size = ** (short**) array.uValue.fArray.fData;

	return size;
}


short PA_GetIntegerInArray( PA_Variable array, long i )
{
	short value;
	
	if (	array.fType == eVK_ArrayInteger 
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		value = ( * (short**) (array.uValue.fArray.fData) )[i];

	return value;
}


long PA_GetLongintInArray( PA_Variable array, long i )
{
	long value = 0;
	
	if (	array.fType == eVK_ArrayLongint
		 && array.uValue.fArray.fData  
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		value = ( * (long**) (array.uValue.fArray.fData) )[i];
	
	return value;
}


double PA_GetRealInArray( PA_Variable array, long i )
{
	double value = 0.0;

	if (	array.fType == eVK_ArrayReal
		 && array.uValue.fArray.fData  
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		value = ( * (double**) (array.uValue.fArray.fData) )[i];

	return value;
}


void PA_GetDateInArray( PA_Variable array, long i, short* day, short* month, short* year )
{
	PA_Date date;

	date.fDay   = 0;
	date.fMonth = 0;
	date.fYear  = 0;

	if (	array.fType == eVK_ArrayDate
		 && array.uValue.fArray.fData  
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		date = ( * (PA_Date**) (array.uValue.fArray.fData) )[i];

	if ( day )
		*day   = date.fDay;

	if ( month )
		*month = date.fMonth;

	if ( year )
		*year  = date.fYear;
}


void PA_GetStringInArray( PA_Variable array, long i, char* string )
{
	short size;
	char* pt;

	if (    array.fType == eVK_ArrayString	
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
	{
		size = ** (short**) array.uValue.fArray.fData;
		pt = ( *(char**) array.uValue.fArray.fData ) + ( i * (long) size ) + (long) sizeof( short );
		ToUserString( pt, string );
	}
}


long PA_GetTextInArray( PA_Variable array, long i, char* text )
{
	long len = 0;
	PA_Text textblock;
	
	if (	array.fType == eVK_ArrayText 
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
	{
		textblock = ( * (PA_Text**) array.uValue.fArray.fData ) [ i ];
		if ( textblock.fHandle )
			len = ToUserText( textblock.fHandle, text );
	}
	return len;
}


long PA_GetPictureInArray( PA_Variable array, long i, void* picture, PA_PictureInfo* info )
{
	long len = 0;
	PA_Handle hpicture;
	
	if (	array.fType == eVK_ArrayPicture 
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
	{
		hpicture = ( * (PA_Handle**) array.uValue.fArray.fData ) [ i ];
		if ( hpicture )
			len = ToUserPicture( hpicture, picture, info );
	}
	return len;
}


PA_Handle PA_GetPictureHandleInArray( PA_Variable array, long i, PA_PictureInfo* info )
{
	PA_Handle hpicture = 0;
	
	if (	array.fType == eVK_ArrayPicture 
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		hpicture = ( * (PA_Handle**) array.uValue.fArray.fData ) [ i ];

	GetPictureInfo( hpicture, info );

	return hpicture;
}


static unsigned char tabTrue [ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
static unsigned char tabFalse[ 8 ] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };

static char GetBitInTable( unsigned char *bitTable, long index)
{
	return (char) ( ( *( bitTable + ( index >> 3 ) ) & tabTrue[ index & 7 ] ) ? 1 : 0 );
}


static void SetBitInTable( unsigned char *bitTable, long index, char value)
{
	long bytePos;

	bytePos = index >> 3;
	index &= 7;
	if ( value )
		*( bitTable + bytePos ) |= tabTrue[ index ];
	else
		*( bitTable + bytePos ) &= tabFalse[ index ];
}


char PA_GetBooleanInArray( PA_Variable array, long i )
{
	char value = 0;

	if (	array.fType == eVK_ArrayBoolean 
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		value = GetBitInTable( *(unsigned char**) (array.uValue.fArray.fData), i );

	return value;
}


void PA_SetIntegerInArray( PA_Variable array, long i, short value )
{
	if (	array.fType == eVK_ArrayInteger
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		( * (short**) (array.uValue.fArray.fData) )[i] = value;
}


void PA_SetLongintInArray( PA_Variable array, long i, long value )
{
	if (	array.fType == eVK_ArrayLongint 
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		( * (long**) (array.uValue.fArray.fData) )[i] = value;
}


void PA_SetRealInArray( PA_Variable array, long i, double value )
{
	if (	array.fType == eVK_ArrayReal  
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		( * (double**) (array.uValue.fArray.fData) )[i] = value;
}


void PA_SetDateInArray( PA_Variable array, long i, short day, short month, short year )
{
	PA_Date date;

	date.fDay   = day;
	date.fMonth = month;
	date.fYear  = year;

	if (	array.fType == eVK_ArrayDate 
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		( * (PA_Date**) (array.uValue.fArray.fData) )[i] = date;
}


void PA_SetStringInArray( PA_Variable array, long i, char* string )
{
	short size;
	char* pt;

	if (	array.fType == eVK_ArrayString
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
		 && string
	   )
	{
		size = ** (short**) array.uValue.fArray.fData;
		pt = ( *(char**) array.uValue.fArray.fData ) + ( i * (long) size ) + (long) sizeof( short );
		FromUserString( string, pt );
	}
}


void PA_SetTextInArray( PA_Variable array, long i, char* text, long len )
{
	PA_Text* textblock;
	
	if (	array.fType == eVK_ArrayText  
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
		 && text
	   )
	{
		// lock the array handle and get a pointer on the element
		textblock = (PA_Text*) PA_LockHandle( array.uValue.fArray.fData );
		textblock += i;

		if ( textblock->fHandle )
			PA_DisposeHandle( textblock->fHandle );
		
		textblock->fHandle = FromUserText( text, &len );
		textblock->fSize = (short) len;

		PA_UnlockHandle( array.uValue.fArray.fData );
	}
}


void PA_SetPictureInArray( PA_Variable array, long i, void* picture, long len, PA_PictureInfo info )
{
	PA_Handle* pth;
	
	if (	array.fType == eVK_ArrayPicture 
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
		 && picture
	   )
	{
		// lock the array handle and get a pointer on the element
		pth = (PA_Handle*) PA_LockHandle( array.uValue.fArray.fData );
		pth += i;

		// remove existing picture
		if ( *pth )
			PA_DisposeHandle( *pth );

		// allocate new picture and store its handle in array
		*pth = FromUserPicture( picture, len, info );

		PA_UnlockHandle( array.uValue.fArray.fData );
	}
}


void PA_SetPictureHandleInArray( PA_Variable array, long i, PA_Handle hpicture, PA_PictureInfo info )
{
	PA_Handle* pth;
	
	if (	array.fType == eVK_ArrayPicture 
		 && array.uValue.fArray.fData
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
	{
		// lock the array handle and get a pointer on the element
		pth = (PA_Handle*) PA_LockHandle( array.uValue.fArray.fData );
		pth += i;

		if ( *pth && ( *pth != hpicture ) )
			PA_DisposeHandle( *pth );

		*pth = FromUserPictureHandle( hpicture, info );

		PA_UnlockHandle( array.uValue.fArray.fData );
	}
}


void PA_SetBooleanInArray( PA_Variable array, long i, char value )
{
	if (	array.fType == eVK_ArrayBoolean
		 && array.uValue.fArray.fData 
		 && i >= 0
		 && i <= array.uValue.fArray.fNbElements
	   )
		SetBitInTable( *(unsigned char**) array.uValue.fArray.fData, i, value );
}


// -----------------------------------------
//
// formating strings, values, date or time
//
// -----------------------------------------


void PA_FormatString( char* string, char* format, char* result )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	FromUserString( format, eb.fName );
	FromUserString( string, eb.fString );

	Call4D( EX_STRING, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fString, result );
}


void PA_FormatReal( double value, char* format, char* result )
{
	EngineBlock eb;

	eb.fParam1 = eVK_Real;
	eb.fReal   = value;
	FromUserString( format, eb.fString );

	Call4D( EX_STRING, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fString, result );
}


void PA_FormatLongint( long value, char* format, char* result )
{
	EngineBlock eb;

	eb.fParam1  = eVK_Longint;
	eb.fLongint = value;
	FromUserString( format, eb.fString );

	Call4D( EX_STRING, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fString, result );
}


void PA_FormatDate( short day, short month, short year, short formatNumber, char* result )
{
	EngineBlock eb;

	eb.fParam1      = eVK_Date;
	eb.fParam2      = formatNumber;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;

	Call4D( EX_STRING, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fString, result );
}


void PA_FormatTime( long time, short formatNumber, char* result )
{
	EngineBlock eb;

	eb.fParam1  = eVK_Time;
	eb.fParam2  = formatNumber;
	eb.fLongint = time;

	Call4D( EX_STRING, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fString, result );
}


double PA_EvalReal( char* string )
{
	EngineBlock eb;

	eb.fParam1 = eVK_Real;
	eb.fReal   = 0.0;
	FromUserString( string, eb.fString );

	Call4D( EX_EVAL_NUMBER, &eb );
	sErrorCode = eER_NoErr;

	return eb.fReal;
}


long PA_EvalLongint( char* string )
{
	EngineBlock eb;

	eb.fParam1  = eVK_Longint;
	eb.fLongint = 0;
	FromUserString( string, eb.fString );

	Call4D( EX_EVAL_NUMBER, &eb );
	sErrorCode = eER_NoErr;

	return eb.fLongint;
}


void PA_EvalDate( char* string, short* day, short* month, short* year )
{
	EngineBlock eb;

	eb.fParam1      = eVK_Date;
	eb.fDate.fDay   = 0;
	eb.fDate.fMonth = 0;
	eb.fDate.fYear  = 0;
	FromUserString( string, eb.fString );

	Call4D( EX_EVAL_NUMBER, &eb );
	sErrorCode = eER_NoErr;

	if ( day )
		*day = eb.fDate.fDay;

	if ( month )
		*month = eb.fDate.fMonth;

	if ( year )
		*year = eb.fDate.fYear;
}


long PA_EvalTime( char* string )
{
	EngineBlock eb;

	eb.fParam1  = eVK_Time;
	eb.fLongint = 0;
	FromUserString( string, eb.fString );

	Call4D( EX_EVAL_NUMBER, &eb );
	sErrorCode = eER_NoErr;

	return eb.fLongint;
}

// return values:
// 0 if the strings are the same,
// 1 if text1 is greater than text2,
// 2 if text1 is less than text2
char PA_CompareStrings( char* text1, long len1, char* text2, long len2, char diacritic )
{
	EngineBlock eb;

	char result = 0;
	PA_Handle htext1 = 0;
	PA_Handle htext2 = 0;

	eb.fParam1 = 0;
	sErrorCode = eER_NoErr;

	if ( sUserCharSet == eCS_Macintosh )
	{
		eb.fParam1 = (long) text1;
		eb.fParam2 = (long) text2;
	}
	else
	{
		htext1 = FromUserText( text1, &len1 );
		htext2 = FromUserText( text2, &len2 );
		if ( htext1 && htext2 )
		{
			eb.fParam1 = (long) PA_LockHandle( htext1 );
			eb.fParam2 = (long) PA_LockHandle( htext2 );
		}
		else
			sErrorCode = eER_NotEnoughMemory;
	}

	eb.fParam3 = len1;
	eb.fParam4 = len2;

	if ( diacritic )
		eb.fParam3 = -eb.fParam3;

	if ( sErrorCode == eER_NoErr )
		Call4D( EX_COMPARE_STRINGS, &eb );

	if ( htext1 && htext2 )
	{
		PA_UnlockHandle( htext1 );
		PA_UnlockHandle( htext2 );
	}

	if ( htext1 )
		PA_DisposeHandle( htext1 );

	if ( htext2 )
		PA_DisposeHandle( htext2 );

	return (char) eb.fParam1;
}
	

void PA_ConvertStrings( char* string1, PA_StringKind kind1, PA_CharSet charset1, 
					    char* string2, PA_StringKind kind2, PA_CharSet charset2 )
{
	EngineBlock eb;
	long				l1;

	//+++++++++++++++++++++++++++++++++++++++++++
	if (sIsPriorTo67) {
		if (kind1 == eSK_CString) {
			l1 = strlen(string1);
			switch (kind2)
			{
				case eSK_CString :
					strcpy(string2, string1);
					break;
				case eSK_PString :
					PA_MoveBlock(string1, &string2[1], l1);
					string2[0] = (char)l1;
					break;
			}
		} else if (kind1 == eSK_PString) {
			switch (kind2)
			{
			case eSK_CString :
				PA_MoveBlock(&string1[1], string2, string1[0]);
				string2[ string1[0] ] = (char) 0;
				break;
			case eSK_PString :
				PA_MoveBlock(string1, string2, string1[0] + 1);
				break;
			}
		}
	} else {
	//+++++++++++++++++++++++++++++++++++++++++++


		eb.fParam1 = 4;	// no conversion;

		if (    ( charset1 == eCS_Macintosh )
			 && ( charset2 == eCS_Ansi ) )
			eb.fParam1 = 2;

		if (    ( charset1 == eCS_Ansi )
			 && ( charset2 == eCS_Macintosh ) )
			eb.fParam1 = 3;

		eb.fParam2 = -1;
		if ( kind1 == eSK_CString )
		{
			if ( kind2 == eSK_CString )
				eb.fParam2 = 4;
			else if ( kind2 == eSK_PString )
				eb.fParam2 = 3;
		}
		else if ( kind1 == eSK_PString )
		{
			if ( kind2 == eSK_CString )
				eb.fParam2 = 2;
			else if ( kind2 == eSK_PString )
				eb.fParam2 = 1;
		}

		eb.fParam3 = (long) string1;
		eb.fParam4 = (long) string2;
		eb.fError = 0;

		Call4D( EX_CONVERT_STRING, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;
	} //dcc code patch
}



// -----------------------------------------
//
// 4th Dimension import/export filters
//
// -----------------------------------------

char* PA_GetExportFilter()
{
	EngineBlock eb;

	eb.fHandle = 0;
	
	Call4D( EX_GET_EXPORT_FILTER, &eb );
	sErrorCode = eER_NoErr;
	
	return (char*) eb.fHandle;
}


char* PA_GetImportFilter()
{
	EngineBlock eb;

	eb.fHandle = 0;
	
	Call4D( EX_GET_IMPORT_FILTER, &eb );
	sErrorCode = eER_NoErr;
	
	return (char*) eb.fHandle;
}


// -----------------------------------------
//
// 4th Dimension Packed Records
//
// -----------------------------------------


long PA_GetPackedRecord( short table, void* buffer )
{
	long len = 0;
	EngineBlock eb;

	eb.fTable = table;
	eb.fHandle = 0;

	Call4D( EX_GET_PACKED_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
	{
		len = ToUserData( eb.fHandle, buffer );
		PA_DisposeHandle( eb.fHandle );
	}
	return len;
}


void PA_SetPackedRecord( short table, void* buffer, long len )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fHandle = FromUserData( buffer, len );

	Call4D( EX_SET_PACKED_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


// -----------------------------------------
//
// 4th Dimension selections
//
// -----------------------------------------


void PA_CopyNamedSelection( short table, char* name )
{
	EngineBlock eb;

	eb.fManyToOne = 1;
	eb.fTable = table;
	eb.fError = 0;
	FromUserString( name, eb.fName );

	Call4D( EX_COPY_CUT_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_CutNamedSelection( short table, char* name )
{
	EngineBlock eb;

	eb.fManyToOne = 0;
	eb.fTable = table;
	eb.fError = 0;
	FromUserString( name, eb.fName );

	Call4D( EX_COPY_CUT_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UseNamedSelection( char* name )
{
	EngineBlock eb;

	FromUserString( name, eb.fName );
	eb.fError = 0;

	Call4D( EX_USE_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_ClearNamedSelection( char* name )
{
	EngineBlock eb;

	FromUserString( name, eb.fName );
	eb.fError = 0;

	Call4D( EX_CLEAR_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4th Dimension sets
//
// -----------------------------------------


void PA_CreateEmptySet( short table, char* name )
{
	EngineBlock eb;

	eb.fTable = table;
	FromUserString( name, eb.fName );

	Call4D( EX_CREATE_EMPTY_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_CreateSet( short table, char* name )
{
	EngineBlock eb;

	eb.fTable = table;
	FromUserString( name, eb.fName );

	Call4D( EX_CREATE_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UseSet( char* name )
{
	EngineBlock eb;

	eb.fTable = 1;
	FromUserString( name, eb.fName );

	Call4D( EX_USE_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_DeleteSet( char* name )
{
	EngineBlock eb;

	eb.fTable = 1;
	FromUserString( name, eb.fName );

	Call4D( EX_DELETE_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_AddToSet( short table, char* name )
{
	EngineBlock eb;

	eb.fHandle = 0;
	eb.fTable = table;
	FromUserString( name, eb.fName );

	Call4D( EX_ADD_TO_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4th Dimension methods
//
// -----------------------------------------


void PA_ExecuteMethod( char* text, long len )
{
	EngineBlock eb;

	eb.fError = 0;
	eb.fHandle = FromUserText( text, &len );

	Call4D( EX_EXECUTE_METHOD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


PA_Variable PA_ExecuteFunction( char* text, long len )
{
	EngineBlock eb;
	PA_Variable vb;

	vb.fType = eVK_Undefined;

	eb.fError = 0;
	eb.fHandle = FromUserText( text, &len );
	eb.fParam1 = (long) &vb;

	Call4D( EX_EXECUTE_FUNCTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return vb;
}


long PA_Tokenize( char* text, long len, void* tokens )
{
	EngineBlock eb;
	long tokenslen = 0;

	eb.fHandle = FromUserText( text, &len );
	eb.fError = 0;
	eb.fParam1 = 0;

	Call4D( EX_TOKENIZE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	if ( eb.fParam1 && sErrorCode == eER_NoErr )
		tokenslen = ToUserData( (PA_Handle) eb.fParam1, tokens );

	if ( eb.fParam1 )
		PA_DisposeHandle( (PA_Handle) eb.fParam1 );

	return tokenslen;
}


long PA_Detokenize( void* tokens, long len, char* text )
{
	EngineBlock eb;
	long textlen = 0;

	eb.fParam1 = (long) FromUserData( tokens, len );
	eb.fError  = 0;
	eb.fHandle = 0;

	Call4D( EX_DETOKENIZE_IN_TEXT, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle && sErrorCode == eER_NoErr )
		textlen = ToUserText( eb.fHandle, text );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	if ( eb.fParam1 )
		PA_DisposeHandle( (PA_Handle) eb.fParam1 );

	return textlen;
}


// handle returned belongs to user
PA_Handle PA_DetokenizeInTEHandle( void* tokens, long len )
{
	EngineBlock eb;

	eb.fParam1 = (long) FromUserData( tokens, len );
	eb.fError  = 0;
	eb.fHandle = 0;

	Call4D( EX_DETOKENIZE_IN_TEHANDLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return eb.fHandle;
}


void PA_ByteSwapTokens( void* tokens, long len )
{
	EngineBlock eb;
	eb.fHandle = FromUserData( tokens, len );
	eb.fError  = 0;
	Call4D( EX_BYTE_SWAP_TOKENS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
	{
		ToUserData( eb.fHandle, tokens );
		PA_DisposeHandle( eb.fHandle );
	}
}


void PA_ExecuteTokens ( void* tokens, long len )
{
	EngineBlock eb;

	eb.fHandle = FromUserData( tokens, len );
	eb.fError = 0;

	Call4D( EX_EXEC_TOKEN_PROC, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


PA_Variable PA_ExecuteTokensAsFunction( void* tokens, long len )
{
	EngineBlock eb;
	PA_Variable vb;

	vb.fType = eVK_Undefined;

	eb.fError = 0;
	eb.fHandle = FromUserData( tokens, len );
	eb.fParam1 = (long) &vb;

	Call4D( EX_EXEC_TOKEN_FUNC, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return vb;
}


long PA_GetMethodID( char* methodName )
{
	EngineBlock eb;

	eb.fParam1 = -1;
	eb.fError  = 0;
	FromUserString( methodName, eb.fName );

	Call4D( EX_FIND_PROCID, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam1;
}


PA_Variable PA_ExecuteMethodByID( long id, PA_Variable* parameters, short nbParameters )
{
	EngineBlock eb;
	PA_Variable* pt;
	PA_Variable returned;
	long i;

	returned.fType = eVK_Undefined;
	returned.uValue.fReal = 0.0;

	eb.fHandle = PA_NewHandle( ( 1 + nbParameters ) * (long) sizeof( PA_Variable ) );
	if ( eb.fHandle )
	{
		pt = (PA_Variable*) PA_LockHandle( eb.fHandle );
		
		// set the PA_Variable used for function return
		*pt++ = returned;

		// fill the handle with the user parameters
		for ( i = 0; i < nbParameters; i++ )
			*pt++ = *parameters++;

		PA_UnlockHandle( eb.fHandle );

		eb.fError  = 0;
		eb.fParam1 = id;
		eb.fParam2 = nbParameters;
		Call4D( EX_CALL_BY_PROCID, &eb );
		sErrorCode = (PA_ErrorCode) eb.fError;

		pt = (PA_Variable*) PA_LockHandle( eb.fHandle );
		returned = *pt;
		PA_UnlockHandle( eb.fHandle );

		PA_DisposeHandle( eb.fHandle );
	}

	return returned;
}


// ---------------------------------------------------------------
//
// Log and Backup operations
//
// ---------------------------------------------------------------

void PA_LockDatabase()
{
	EngineBlock eb;
	Call4D( EX_LOCK_DATABASE, &eb );
	sErrorCode = eER_NoErr;
}


void PA_UnlockDatabase()
{
	EngineBlock eb;
	Call4D( EX_UNLOCK_DATABASE, &eb );
	sErrorCode = eER_NoErr;
}


void PA_NewLog( char* logName )
{
	EngineBlock eb;

	eb.fParam1 = -1;
	eb.fError  = 0;
	FromUserString( logName, eb.fName );
	Call4D( EX_NEW_LOG, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_OpenLog( char* logName )
{
	EngineBlock eb;

	eb.fParam1 = -1;
	eb.fError  = 0;
	FromUserString( logName, eb.fName );
	Call4D( EX_OPEN_LOG, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_GetBackupStatus()
{
	EngineBlock eb;
	Call4D( EX_GET_BACKUP_STATUS, &eb );
	sErrorCode = eER_NoErr;
	return (char) eb.fParam1;
}


void PA_GetLogInfo( PA_LogHeader* logInfo )
{
	EngineBlock eb;
	Call4D( EX_LOG_INFO, &eb );
	sErrorCode = eER_NoErr;
	if ( logInfo )
		*logInfo = **(PA_LogHeader**) eb.fHandle;
}


PA_FieldKind PA_GetLogFieldType( PA_LogTag* logtag, short field, char* logdata, long len )
{
	EngineBlock eb;

	eb.fError  = 0;
	eb.fParam1 = (long) logtag;
	eb.fParam2 = field;
	eb.fParam3 = eFK_InvalidFieldKind;
	eb.fHandle = FromUserData( logdata, len );

	Call4D( EX_GET_LOG_FIELD_TYPE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
	
	return (PA_FieldKind) eb.fParam3;
}


// this one works for all types of fields, exept text and pictures
void PA_GetLogField( PA_LogTag* logtag, short field, char* logdata, long len, char* string )
{
	EngineBlock eb;

	eb.fError  = 0;
	eb.fParam1 = (long) logtag;
	eb.fParam2 = field;
	eb.fHandle = FromUserData( logdata, len );
	eb.fString[0] = 0;

	Call4D( EX_GET_LOG_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	ToUserString( eb.fString, string );
	
	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


void PA_GetTextLogField( PA_LogTag* logtag, short field, char* logdata, long logdatalen, char* text )
{
	EngineBlock eb;

	eb.fError  = 0;
	eb.fParam1 = (long) logtag;
	eb.fParam2 = field;
	eb.fHandle = FromUserData( logdata, logdatalen );
	eb.fText.fSize   = 0;
	eb.fText.fHandle = 0;

	Call4D( EX_GET_LOG_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	ToUserText( eb.fText.fHandle, text );
	
	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
	
	if ( eb.fText.fHandle )
		PA_DisposeHandle( eb.fText.fHandle );
}


long PA_GetPictureLogField( PA_LogTag* logtag, short field, char* logdata, long logdatalen, void* picture, PA_PictureInfo* info )
{
	EngineBlock eb;
	long len = 0;

	eb.fError  = 0;
	eb.fParam1 = (long) logtag;
	eb.fParam2 = field;
	eb.fHandle = FromUserData( logdata, logdatalen );
	eb.fText.fSize   = 0;
	eb.fText.fHandle = 0;

	Call4D( EX_GET_LOG_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	len = ToUserPicture( eb.fText.fHandle, picture, info );
	
	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
	
	if ( eb.fText.fHandle )
		PA_DisposeHandle( eb.fText.fHandle );

	return len;
}


void PA_PerformLogAction( PA_LogTag* logtag, char* logdata, long logdatalen )
{
	EngineBlock eb;

	eb.fError  = 0;
	eb.fParam1 = (long) logtag;
	eb.fHandle = FromUserData( logdata, logdatalen );

	Call4D( EX_PERFORM_LOG_ACTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// ---------------------------------------------------------------
//
// Multiprocess Printing Management
//
// ---------------------------------------------------------------

char PA_TryToTakePrinter()
{
	EngineBlock eb;

	Call4D( EX_TRY_TO_TAKE_PRINTER, &eb );
	sErrorCode = eER_NoErr;

	return (char) ( eb.fParam1 ? 0 : 1 );
}


// The printer is now unavailable for all other processes in 4D
void PA_TakePrinter()
{
	EngineBlock eb;

	Call4D( EX_OPEN_PRINTER, &eb );
	sErrorCode = eER_NoErr;
}


// other processes can now use the printer
void PA_ReleasePrinter()
{
	EngineBlock eb;

	Call4D( EX_RELEASE_PRINTER, &eb );
	sErrorCode = eER_NoErr;
}


// Warning, the handle belongs to 4D, do not dispose it
PA_Handle PA_GetMacPrintInfo()
{
	EngineBlock eb;

	eb.fHandle = 0;
	eb.fError  = 0;
	Call4D( EX_GET_MAC_PRINT_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fHandle;
}


// return the resfile of 4D current printing session
short PA_GetPrintFileResFile()
{
	EngineBlock eb;

	Call4D( EX_GET_PRINTFILE_RESFILE, &eb );
	sErrorCode = eER_NoErr;

	return (short) eb.fParam4;
}


// ---------------------------------------------------------------
//
// Processes
//
// ---------------------------------------------------------------


// return number of running process
long PA_CountActiveProcess()
{
	EngineBlock eb;

	eb.fParam2 = 0;
	Call4D( EX_NB_PROCESS, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam2;
}


// return number of process, including aborted process
long PA_CountTotalProcess()
{
	EngineBlock eb;

	eb.fParam1 = 0;
	Call4D( EX_NB_PROCESS, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam1;
}


void PA_GetProcessInfo( long process, char* name, long* state, long* time )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fString[0] = 0;
	eb.fParam2 = 0;
	eb.fParam3 = 0;
	eb.fError  = 0;

	Call4D( EX_GET_PROCESS_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
	{
		ToUserString( eb.fString, name );

		if ( state )
			*state = eb.fParam2;

		if ( time )
			*time = eb.fParam3;
	}
}


void PA_FreezeProcess( long process )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fError  = 0;
	Call4D( EX_FREEZE_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UnfreezeProcess( long process )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fError  = 0;
	Call4D( EX_UNFREEZE_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_IsProcessDying()
{
	EngineBlock eb;
	eb.fParam1 = 0;	// or process number
	Call4D( EX_IS_PROCESS_DYING, &eb );
	sErrorCode = eER_NoErr;

	return eb.fManyToOne;
}


void PA_KillProcess()
{
	EngineBlock eb;
	Call4D( EX_KILL_PROCESS, &eb );
	sErrorCode = eER_NoErr;
}


void PA_PutProcessToSleep( long process, long time )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fParam2 = time;
	eb.fError  = 0;
	Call4D( EX_PUT_PROCESS_TO_SLEEP, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_GetCurrentProcessNumber()
{
	EngineBlock eb;
	Call4D( EX_CURRENT_PROCESS_NUMBER, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam1;
}


long PA_GetWindowProcess( PA_WindowRef windowRef )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) windowRef;
	eb.fParam2 = -1;
	Call4D( EX_GET_WINDOW_PROCESS, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam1;
}


long PA_GetFrontWindowProcess( char withPalette )
{
	EngineBlock eb;
	eb.fHandle = 0;
	eb.fParam2 = withPalette ? 1 : 0;
	Call4D( EX_GET_WINDOW_PROCESS, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam1;
}


void PA_SetWindowProcess( PA_WindowRef windowRef, long process )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) windowRef;
	eb.fParam1 = process;
	Call4D( EX_SET_WINDOW_PROCESS, &eb );
	sErrorCode = eER_NoErr;
}


void PA_Yield()
{
	EngineBlock eb;
	Call4D( EX_YIELD, &eb );
	sErrorCode = eER_NoErr;
}


void PA_YieldAbsolute()
{
	EngineBlock eb;
	Call4D( EX_YIELD_ABSOLUTE, &eb );
	sErrorCode = eER_NoErr;
}


char PA_WaitNextEvent( PA_Event* event )
{
	EngineBlock eb;
	eb.fParam1 = 0xFFFF;	// everyEvent
	eb.fParam2 = 3;
	eb.fHandle = (PA_Handle) event;
	eb.fParam3 = 0;
	Call4D( EX_WAIT_NEXT_EVENT, &eb );
	sErrorCode = eER_NoErr;
	return (char) eb.fParam4;
}


void PA_UpdateProcessVariable( long process )
{
	EngineBlock eb;
	eb.fParam1 = process;
	eb.fError = 0;
	Call4D( EX_UPDATE_PROCESS_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_BringProcessToFront( long process )
{
	EngineBlock eb;
	eb.fParam1 = process;
	Call4D( EX_BRING_PROCESS_TO_FRONT, &eb );
	sErrorCode = eER_NoErr;
}


long PA_NewProcess( void* procPtr, long stackSize, char* name )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) procPtr;
	eb.fParam3 = stackSize;
	FromUserString( name, eb.fString );
	eb.fParam1 = 0;
	eb.fError = 0;
	Call4D( EX_NEW_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam1;
}


void PA_PostMacEvent( long process, PA_Event* event )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) event;
	eb.fParam1 = process;
	Call4D( EX_POST_EVENT, &eb );
	sErrorCode = eER_NoErr;
}


// ---------------------------------------------------------------
//
// Sending/Receiving documents from 4D Server
//
// ---------------------------------------------------------------


long PA_ReceiveDocumentFromServer( char* docName, void* buffer )
{
	long len = 0;
	EngineBlock eb;

	FromUserString( docName, eb.fName );
	eb.fHandle = 0;

	Call4D( EX_READ_DOCUMENT, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
	{
		len = ToUserData( eb.fHandle, buffer );
		PA_DisposeHandle( eb.fHandle );
	}
	return len;
}


void PA_SendDocumentToServer( char* docName, void* buffer, long len, unsigned long type, unsigned long creator )
{
	EngineBlock eb;

	FromUserString( docName, eb.fName );
	eb.fHandle = (PA_Handle) buffer;
	eb.fParam2 = len;
	eb.fParam3 = (long) type;
	eb.fParam4 = (long) creator;

	Call4D( EX_SAVE_DOCUMENT, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_DocumentExistOnServer( char* docName, unsigned long type, unsigned long creator )
{
	EngineBlock eb;
	FromUserString( docName, eb.fName );
	eb.fParam1 = 0;
	eb.fParam3 = (long) type;
	eb.fParam4 = (long) creator;

	Call4D( EX_EXIST_DOCUMENT_ON_SERVER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


// ---------------------------------------------------------------
//
// Sending/Receiving data between 4D Client and 4D Server
//
// ---------------------------------------------------------------


void PA_SendDataToServer( long pluginRef, long dataRef, long dataKind, void* buffer, long len )
{
	EngineBlock eb;

	eb.fParam1 = pluginRef;
	eb.fParam2 = len;
	eb.fParam3 = dataKind;
	eb.fParam4 = dataRef;
	eb.fHandle = (PA_Handle) buffer;

	Call4D( EX_WRITE_DATA, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_ReceiveDataFromServer( long pluginRef, long dataRef, long dataKind, void* buffer )
{
	EngineBlock eb;

	eb.fParam1 = pluginRef;
	eb.fParam3 = dataKind;
	eb.fParam4 = dataRef;
	eb.fHandle = 0;

	Call4D( EX_READ_DATA, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return ToUserData( eb.fHandle, buffer );
}


// -----------------------------------------
//
// 4th Dimensions internal resources access
//
// -----------------------------------------


// if you want 4D to give you an unique id, set id to -1.
// The unique id created by 4D will be returned
short PA_CreateResource( short resfile, unsigned long kind, short id, char* name, char* data, long len )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fHandle = FromUserData( data, len );
	eb.fError = 0;
	FromUserString( name, eb.fName );

	if ( id == -1 )
	{
		eb.fParam2 = 0;
		eb.fManyToOne = 1;
	}
	else
	{
		eb.fParam2 = id;
		eb.fManyToOne = 0;
	}

	Call4D( EX_CREATE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (short) eb.fParam2;
}


short PA_CreateResourceFromHandle( short resfile, unsigned long kind, short id, char* name, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fHandle = resourceHandle;
	eb.fError = 0;
	FromUserString( name, eb.fName );

	if ( id == -1 )
	{
		eb.fParam2 = 0;
		eb.fManyToOne = 1;
	}
	else
	{
		eb.fParam2 = id;
		eb.fManyToOne = 0;
	}

	Call4D( EX_CREATE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (short) eb.fParam2;
}


void PA_RemoveResourceByID( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = 0;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fError = 0;

	Call4D( EX_REMOVE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RemoveResourceByName( short resfile, unsigned long kind, char* name )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = 0;
	eb.fParam1 = (long) kind;
	eb.fParam2 = 0;
	eb.fError = 0;
	FromUserString( name, eb.fName );

	Call4D( EX_REMOVE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_GetResource( short resfile, unsigned long kind, short id, char* data )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 0;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return ToUserData( eb.fHandle, data );
}


// warning, you are NOT the owner of the resource handle, unless you detach it
PA_Handle PA_GetResourceHandle( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 0;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fHandle;
}


void PA_WriteResourceHandle( short resfile, PA_Handle handle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = handle;
	eb.fError = 0;

	Call4D( EX_WRITE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetResource( short resfile, unsigned long kind, short id, char* data, long len )
{
	EngineBlock eb;
	char* pt;
	long i;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 0;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr && eb.fHandle && data )
	{
		if ( PA_SetHandleSize( eb.fHandle, len ) )
		{
			// lock 4D resource handle
			pt = PA_LockHandle( eb.fHandle );
			
			// copy data on it
			for ( i = 0; i < len; i++ )
				*pt++ = *data++;

			// unlock it
			PA_UnlockHandle( eb.fHandle );

			// ask 4D to write the new resource content
			Call4D( EX_WRITE_RESOURCE, &eb );
			sErrorCode = (PA_ErrorCode) eb.fError;
		}
	}
}


// on 4D Server/4D Client only
// the resource is locked for others 4D Client (they can't delete or modify it)
char PA_LockResource( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 1;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char)( ( eb.fManyToOne == 0 ) ? 1 : 0 );
}


// on 4D Server/4D Client only
// the resource is now unlocked for others 4D Client
void PA_UnlockResource( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_UNLOCK_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

// the "Lock" attribute of the resource is now set. The handle will
// stay locked, and be automatically locked in future use.
void PA_LockResourceHandle( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = resourceHandle;
	eb.fManyToOne = 1;
	eb.fError = 0;

	Call4D( EX_LOCK_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// the 'Lock' attribute of the resource is now unset. The handle will
// further be moveable.
void PA_UnlockResourceHandle( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = resourceHandle;
	eb.fManyToOne = 0;
	eb.fError = 0;

	Call4D( EX_LOCK_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// the resource and the memory it uses are purged from memory
void PA_ReleaseResourceHandle( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = resourceHandle;
	eb.fError = 0;

	Call4D( EX_RELEASE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// the resource and the memory it uses are purged from memory
void PA_ReleaseResource( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = id;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_RELEASE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_GetIndexedResource( short resfile, unsigned long kind, short index, char* data )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (long) kind;
	eb.fParam2 = index;
	eb.fHandle = 0;
	eb.fError = 0;

	Call4D( EX_GET_INDEXED_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return ToUserData( eb.fHandle, data );
}


long PA_CountResources( short resfile, unsigned long kind )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam3 = 0;
	eb.fParam1 = (long) kind;
	eb.fError = 0;

	Call4D( EX_COUNT_RESOURCES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam3;
}


long PA_CountResourceKinds( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = 0;
	eb.fParam3 = 0;

	Call4D( EX_COUNT_RESOURCE_KINDS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam3;
}


unsigned long PA_GetIndexedResourceKind( short resfile, short index )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = 0;
	eb.fParam1 = 0;
	eb.fParam3 = index;

	Call4D( EX_GET_INDEXED_RESOURCE_KIND, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (unsigned long) eb.fParam1;
}


void PA_GetResourceName( short resfile, unsigned long kind, short id, char* name )
{
	EngineBlock eb;

	eb.fTable   = resfile;
	eb.fParam1  = (long) kind;
	eb.fParam2  = id;
	eb.fError   = 0;
	eb.fName[0] = 0;
	eb.fHandle  = 0;

	Call4D( EX_GET_RESOURCE_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		ToUserString( eb.fName, name );
}


void PA_SetResourceName( short resfile, unsigned long kind, short id, char* name )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1   = (long) kind;
	eb.fParam2   = id;
	eb.fError    = 0;
	FromUserString( name, eb.fName );
	eb.fHandle   = 0;

	Call4D( EX_SET_RESOURCE_NAME, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


long PA_GetResourceSize( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable   = resfile;
	eb.fParam1  = (long) kind;
	eb.fParam2  = id;
	eb.fParam3  = 0;
	eb.fError   = 0;
	eb.fHandle  = 0;
	eb.fName[0] = 0;

	Call4D( EX_GET_RESOURCE_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam3;
}


long PA_GetResourceTimeStamp ( short resfile, unsigned long kind, short id )
{
	EngineBlock eb;

	eb.fTable   = resfile;
	eb.fLongint = (long) kind;
	eb.fShort   = id;
	eb.fHandle  = 0;
	eb.fError   = 0;

	Call4D( EX_GET_RESOURCE_TIMESTAMP, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fParam1;
}


long PA_GetResourceIDList( short resfile, unsigned long kind, short* IDlist )
{
	EngineBlock eb;
	long len = 0;
	long i;
	short* pt;

	eb.fTable  = resfile;
	eb.fParam1 = (long) kind;
	eb.fHandle = 0;
	eb.fError  = 0;

	Call4D( EX_GET_RESOURCE_ID_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
	{
		// warning : return the number of id in the list, 
		// not the size in bytes of the list buffer
		len = PA_GetHandleSize( eb.fHandle ) / (long) sizeof( short );
		
		// if user provides a buffer, copy the ID in the buffer
		if ( IDlist )
		{
			pt = (short*) PA_LockHandle( eb.fHandle );
			for ( i = 0; i < len; i++ )
				*IDlist++ = *pt++;
			PA_UnlockHandle( eb.fHandle );
		}
		PA_DisposeHandle( eb.fHandle );
	}
	return len;	
}


long PA_GetResourceKindList( short resfile, unsigned long* kindlist )
{
	EngineBlock eb;
	long len = 0;
	long i;
	unsigned long* pt;

	eb.fTable  = resfile;
	eb.fHandle = 0;
	eb.fError  = 0;

	Call4D( EX_GET_RESOURCE_KIND_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
	{
		// warning : return the number of id in the list, 
		// not the size in bytes of the list buffer
		len = PA_GetHandleSize( eb.fHandle ) / (long) sizeof( long );
		
		// if user provides a buffer, copy the ID in the buffer
		if ( kindlist )
		{
			pt = (unsigned long*) PA_LockHandle( eb.fHandle );
			for ( i = 0; i < len; i++ )
				*kindlist++ = *pt++;
			PA_UnlockHandle( eb.fHandle );
		}
		PA_DisposeHandle( eb.fHandle );
	}
	return len;	
}


long PA_GetResourceNameList( short resfile, unsigned long kind, char* namelist )
{
	EngineBlock eb;
	char* pt;
	long len = 0;
	long i;

	eb.fTable  = resfile;
	eb.fParam1 = (long) kind;
	eb.fHandle = 0;
	eb.fError  = 0;

	Call4D( EX_GET_RESOURCE_NAME_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
	{
		// warning : return the number of names in the list, 
		// not the size in bytes of the list buffer
		len = PA_GetHandleSize( eb.fHandle ) / 32;
		
		// if user provides a buffer, copy the names in the buffer
		if ( namelist )
		{
			pt = PA_LockHandle( eb.fHandle );
			for ( i = 0; i < len; i++ )
			{
				ToUserString( pt, namelist );
				namelist += 32;	// skip to next names in arrays
				pt += 32;
			}
			PA_UnlockHandle( eb.fHandle );
		}
		PA_DisposeHandle( eb.fHandle );
	}
	return len;	
}


short PA_OpenResFile( char* filename )
{
	EngineBlock eb;
	char string[ 257 ];

	FromUserString( filename, string );
	// convert to C string
	string[ string[0] + 1 ] = 0;

	eb.fHandle    = (PA_Handle) (string + 1);
	eb.fManyToOne = 0;
	eb.fError     = 0;
	eb.fTable     = -1;

	Call4D( EX_OPEN_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


short PA_CreateResFile( char* filename )
{
	EngineBlock eb;
	char string[ 257 ];

	FromUserString( filename, string );
	// convert to C string
	string[ string[0] + 1 ] = 0;

	eb.fHandle = (PA_Handle) (string + 1);
	eb.fError  = 0;
	eb.fTable  = -1;

	Call4D( EX_CREATE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


short PA_GetDatabaseResFile()
{
	EngineBlock eb;

	eb.fTable = 0;

	Call4D( EX_GET_DATABASE_RESFILE, &eb );
	sErrorCode = eER_NoErr;

	return eb.fTable;
}


void PA_UpdateResFile( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	Call4D( EX_UPDATE_RESFILE, &eb );
	sErrorCode = eER_NoErr;
}


void PA_UseResFile( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = 0;
	Call4D( EX_USE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_CloseResFile( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = 0;
	Call4D( EX_CLOSE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


short PA_GetUniqueResID( short resfile, unsigned long kind )
{
	EngineBlock eb;

	eb.fTable  = resfile;
	eb.fParam1 = (long)kind;
	eb.fParam2 = 0;
	eb.fError = 0;
	Call4D( EX_GET_UNIQUE_RESID, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (short) eb.fParam2;
}


void PA_DetachResource( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable  = resfile;
	eb.fHandle = resourceHandle;
	Call4D( EX_DETACH_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
//	4th Dimension users and groups
//
// -----------------------------------------


void PA_GetUserName( char* name )
{
	EngineBlock eb;

	eb.fName[0] = 0;
	Call4D( EX_GET_USER_NAME, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( eb.fName, name );
}


long PA_GetCurrentUserID()
{
	EngineBlock eb;

	Call4D( EX_GET_CURRENT_USER_ID, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam1;
}


static long CountUsersOrGroups( PA_UserKind kind, short entrypoint )	
{
	EngineBlock eb;

	eb.fError = 0;
	eb.fParam1 = 0;
	eb.fParam2 = 0;
	eb.fHandle = 0;

	Call4D( entrypoint, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	if ( kind == eUK_CreatedByDesigner )
		return eb.fParam1;
	else
		return eb.fParam2;
}


long PA_CountUsers( PA_UserKind kind )
{
	return CountUsersOrGroups( kind, EX_GET_USER_LIST );
}


long PA_CountUserGroups( PA_UserKind kind )
{
	return CountUsersOrGroups( kind, EX_GET_GROUP_LIST );
}


static void GetUserOrGroupName( long index, PA_UserKind kind, short entrypoint, char* userName )
{
	EngineBlock eb;
	char* pt;
	long limit, offset;

	eb.fError = 0;
	eb.fParam1 = 0;
	eb.fParam2 = 0;
	eb.fHandle = 0;

	Call4D( entrypoint, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( kind == eUK_CreatedByDesigner )
	{
		limit = eb.fParam1;
		offset = 0;
	}
	else
	{
		limit = eb.fParam2;
		offset = eb.fParam1;
	}

	if ( eb.fHandle )
	{
		if ( userName && ( index >= 1 ) && ( index <= limit ) )
		{
			pt = PA_LockHandle( eb.fHandle );
			pt += ( 32 * ( index - 1 + limit ) );

			ToUserString( pt, userName );

			PA_UnlockHandle( eb.fHandle );
		}
		PA_DisposeHandle( eb.fHandle );
	}
}


void PA_GetIndUserName( long index, PA_UserKind kind, char* userName )
{
	GetUserOrGroupName( index, kind, EX_GET_USER_LIST, userName );
}


void PA_GetIndGroupName( long index, PA_UserKind kind, char* userName )
{
	GetUserOrGroupName( index, kind, EX_GET_GROUP_LIST, userName );
}



// -----------------------------------------
//
//	4th Dimension serial key and users info
//
// -----------------------------------------


long PA_GetSerialKey()
{
	EngineBlock eb;

	Call4D( EX_GET_SERIAL_KEY, &eb );
	sErrorCode = eER_NoErr;
	
	return eb.fParam1;
}


void PA_GetRegisteredUserName( char *name )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	
	Call4D( EX_GET_REGISTRATION_INFO, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( (char*) eb.fParam1, name );
}


void PA_GetRegisteredUserCompany( char *company )
{
	EngineBlock eb;

	eb.fParam2 = 0;
	
	Call4D( EX_GET_REGISTRATION_INFO, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( (char*) eb.fParam2, company );
}


long PA_CountConnectedUsers()
{
	EngineBlock eb;

	Call4D( EX_GET_USERS_INFO, &eb );
	sErrorCode = eER_NoErr;
	
	return eb.fParam1;
}


long PA_GetMaxAllowedUsers()
{
	EngineBlock eb;

	Call4D( EX_GET_USERS_INFO, &eb );
	sErrorCode = eER_NoErr;
	
	return eb.fParam2;
}



// -----------------------------------------
//
//	4th Dimension Plugin area
//
// -----------------------------------------


// force current front window to update
void PA_RedrawWindow()
{
	EngineBlock eb;
	Call4D( EX_REDRAW_WINDOW, &eb );
	sErrorCode = eER_NoErr;
}


// force current front window to show current values of variables
void PA_UpdateVariables()
{
	EngineBlock eb;
	Call4D( EX_UPDATE_VARIABLES, &eb );
}


// on Windows, this command can convert a 4D window reference
// into a Windows regular HWND.
// if you pass 0, return current window HWND
long PA_GetHWND( PA_WindowRef windowRef )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) windowRef;
	eb.fError  = 0;
	Call4D( EX_GET_HWND, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	return (long) eb.fHandle;
}


// on Macintosh, this command can convert a 4D window reference
// into a Macintosh regular WindowPtr
long PA_GetWindowPtr( PA_WindowRef windowRef )
{
	return (long) windowRef;
}


PA_PluginRef PA_OpenPluginWindow( char* areaName, char* windowTitle, PA_Rect rect )
{
	EngineBlock eb;

	FromUserString( areaName, eb.fString );
	FromUserString( windowTitle, eb.fName );

	eb.fParam1 = rect.fLeft  + ( rect.fTop    << 16 );
	eb.fParam2 = rect.fRight + ( rect.fBottom << 16 );

	eb.fHandle = 0;

	Call4D( EX_OPEN_PLUGIN_WINDOW, &eb );
	sErrorCode = eER_NoErr;

	return (PA_PluginRef) eb.fHandle;
}


void PA_ClosePluginWindow( PA_PluginRef pluginRef )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) pluginRef;

	Call4D( EX_CLOSE_PLUGIN_WINDOW, &eb );
	sErrorCode = eER_NoErr;
}

// when a plugin area is in an external window area, you can use this function
// to change the window title, to reflect for example the name of the current document
// you plugin has opened
void PA_SetPluginWindowTitle( PA_PluginRef pluginRef, char* windowTitle )
{
	EngineBlock eb;

	FromUserString( windowTitle, eb.fName );
	eb.fHandle = (PA_Handle) pluginRef;
	eb.fError = 0;

	Call4D( EX_CHANGE_TITLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// return the current page number of the current layout
short PA_GetCurrentPage()
{
	EngineBlock eb;

	eb.fError  = 0;
	eb.fParam1 = 0;

	Call4D( EX_GET_CURRENT_PAGE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return (short) eb.fParam1;
}


PA_WindowRef PA_NewWindow( PA_Rect rect, PA_WindowLevel level, short kind, char* title, char closeBox )
{
	EngineBlock eb;
	
	eb.fParam1 = (long) level;
	Call4D( EX_NEXT_WINDOW_LEVEL, &eb );
	
	eb.fParam1 = (long) &rect;
	FromUserString( title, eb.fName );
	eb.fParam2 = (long) kind;
	eb.fParam3 = (char) closeBox;
	eb.fParam4 = 0;	// may be used to put a window refcon

	Call4D( EX_NEW_WINDOW, &eb );
	sErrorCode = eER_NoErr;

	return (PA_WindowRef) eb.fHandle;
}


void PA_CloseWindow( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;

	Call4D( EX_CLOSE_WINDOW, &eb );
	sErrorCode = eER_NoErr;
}


void PA_SetWindowFocusable( PA_WindowRef windowRef, char focusable )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fManyToOne = focusable;
	Call4D( EX_SET_WINDOW_FOCUSABLE, &eb );
	sErrorCode = eER_NoErr;
}


char PA_IsWindowFocusable( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fManyToOne = 0;
	eb.fError = 0;
	Call4D( EX_IS_WINDOW_FOCUSABLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fManyToOne;
}


PA_WindowRef PA_GetWindowFocused()
{
	EngineBlock eb;

	eb.fHandle = 0;
	Call4D( EX_GET_WINDOW_FOCUSED, &eb );
	sErrorCode = eER_NoErr;

	return (PA_WindowRef) eb.fHandle;
}


void PA_SetWindowFocused( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	Call4D( EX_SET_WINDOW_FOCUSED, &eb );
	sErrorCode = eER_NoErr;
}


char PA_IsWindowFocused( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fManyToOne = 0;
	eb.fError = 0;
	Call4D( EX_GET_WINDOW_FOCUSED, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fManyToOne;
}


// if posX and posY equal both zero, tip will be by default at 16 pixels South East from mouse position
// the rectangle is the area of the object that is described by the tip. When mouse will left this area,
// the tip window will close.
// both coordinates are in the local coordinates of the window
void PA_CreateTip( char* string, short posX, short posY, PA_Rect rect )
{
	EngineBlock eb;

	eb.fParam1 = (long) &rect;
	if ( posX == posY == 0 )
		eb.fParam2 = 0;
	else
	{
		eb.fParam2 = 'WHER';
		eb.fParam3 = posX;
		eb.fParam4 = posY;
	}

	eb.fLongint = 'TIPS';
	FromUserString( string, eb.fName );

	Call4D( EX_CREATE_TIP, &eb );
	sErrorCode = eER_NoErr;
}


void PA_GotoArea( char* variableName )
{
	EngineBlock eb;

	FromUserString( variableName, eb.fName );
	if ( StripInterprocessVariableName( eb.fName ) )
		eb.fParam1 = 1;
	else
		eb.fParam1 = 0;

	Call4D( EX_GOTO_AREA, &eb );
	sErrorCode = eER_NoErr;
}


void PA_UpdateEditMenu( char* undoString, char undo, char redo, char cut, char copy, char paste, char clear, char selectAll )
{
	EngineBlock eb;

	FromUserString( undoString, eb.fName );

	eb.fParam1 = 0;

	if ( undo )
		eb.fParam1 |= 1;

	if ( redo )
		eb.fParam1 |= 2;

	if ( cut )
		eb.fParam1 |= 4;

	if ( copy )
		eb.fParam1 |= 8;

	if ( paste )
		eb.fParam1 |= 16;

	if ( clear )
		eb.fParam1 |= 32;

	if ( selectAll )
		eb.fParam1 |= 64;

	Call4D( EX_UPDATE_EDIT_MENU, &eb );
	sErrorCode = eER_NoErr;
}



// -----------------------------------------
//
//	4th Dimension Text Editors
//
// -----------------------------------------


PA_Handle PA_GetCurrentTEHandle()
{
	EngineBlock eb;

	eb.fHandle = 0;
	Call4D( EX_GET_CURRENT_TEHANDLE, &eb );
	sErrorCode = eER_NoErr;

	return eb.fHandle;
}


// -----------------------------------------
//
//	4th Dimension Web Server
//
// -----------------------------------------

void PA_SetWebListeners( void* listenProcPtr, void* closeProcPtr, 
						 void* readProcPtr,   void* writeProcPtr,
						 void* idleProcPtr,   void* errorProcPtr )
{
	EngineBlock eb;

	eb.fParam1  = (long) listenProcPtr;
	eb.fParam2  = (long) closeProcPtr;
	eb.fParam3  = (long) readProcPtr;
	eb.fParam4  = (long) writeProcPtr;
	eb.fLongint = (long) idleProcPtr;
	eb.fRecord  = (long) errorProcPtr;

	Call4D( EX_SET_WEB_LISTENERS, &eb );
	sErrorCode = eER_NoErr;
}


void PA_SetWebFilters(   void* filterInProcPtr,   void* filterOutProcPtr, 
						 void* releasePtrProcPtr, char webFilterMacChars )
{
	EngineBlock eb;

	eb.fParam1 = (long) filterInProcPtr;
	eb.fParam2 = (long) filterOutProcPtr;
	eb.fParam3 = (long) releasePtrProcPtr;
	eb.fParam4 = (long) webFilterMacChars;

	Call4D( EX_SET_WEB_FILTERS, &eb );
	sErrorCode = eER_NoErr;
}


void PA_StartWebServer()
{
	EngineBlock eb;
	Call4D( EX_START_WEB_SERVER, &eb );
	sErrorCode = eER_NoErr;
}


void PA_StopWebServer()
{
	EngineBlock eb;
	Call4D( EX_STOP_WEB_SERVER, &eb );
	sErrorCode = eER_NoErr;
}


void PA_GetWebServerInfo( long* webServerProcess, long* TCPport )
{
	EngineBlock eb;
	Call4D( EX_GET_WEB_SERVER_INFO, &eb );
	sErrorCode = eER_NoErr;

	if ( webServerProcess )
		*webServerProcess = eb.fParam2;

	if ( TCPport )
		*TCPport = eb.fParam1;
}


void PA_SetWebTCPport( long TCPport )
{
	EngineBlock eb;
	eb.fParam1 = TCPport;
	Call4D( EX_SET_WEB_TCP_PORT, &eb );
	sErrorCode = eER_NoErr;
}


void PA_GetWebContext( long* context, long* subContext, char* name )
{
	EngineBlock eb;
	Call4D( EX_GET_WEB_CONTEXT, &eb );
	sErrorCode = eER_NoErr;
	
	if ( context )
		*context = eb.fParam1;
	
	if ( subContext )
		*subContext = eb.fParam2;

	ToUserString( eb.fName, name );
}


long PA_GetWebTimeOut()
{
	EngineBlock eb;

	eb.fParam1 = -1;
	eb.fParam2 = 0;
	Call4D( EX_WEB_TIMEOUT, &eb );
	sErrorCode = eER_NoErr;

	return eb.fParam2;
}


void PA_SetWebTimeOut( long timeOut )
{
	EngineBlock eb;

	eb.fParam1 = timeOut;
	Call4D( EX_WEB_TIMEOUT, &eb );
	sErrorCode = eER_NoErr;
}


void PA_OpenURL( char* url, long len )
{
	EngineBlock eb;

	eb.fHandle = FromUserText( url, &len );

	Call4D( EX_OPEN_URL, &eb );
	sErrorCode = eER_NoErr;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


// -----------------------------------------
//
//	4th Dimension SpellCheckers
//
// -----------------------------------------


void PA_Install4DSpeller( void* spellcheckerProcPtr )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) spellcheckerProcPtr;
	Call4D( EX_INSTALL_4D_SPELLER, &eb );
	sErrorCode = eER_NoErr;
}


void* PA_Get4DSpellerProcPtr()
{
	EngineBlock eb;
	eb.fHandle = 0;
	Call4D( EX_GET_4D_SPELLER, &eb );
	sErrorCode = eER_NoErr;
	return (void*) eb.fHandle;
}


void PA_Install4DWriteSpeller( void* spellcheckerProcPtr )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) spellcheckerProcPtr;
	Call4D( EX_INSTALL_4DWRITE_SPELLER, &eb );
	sErrorCode = eER_NoErr;
}


void* PA_Get4DWriteSpellerProcPtr()
{
	EngineBlock eb;
	eb.fHandle = 0;
	Call4D( EX_GET_4DWRITE_SPELLER, &eb );
	sErrorCode = eER_NoErr;
	return (void*) eb.fHandle;
}



// -----------------------------------------
//
//	4th Dimension Editors
//
// -----------------------------------------


char PA_FormulaEditor( short defaultTable, char* text, long* len )
{
	EngineBlock eb;

	eb.fHandle = FromUserText( text, len );
	eb.fParam1 = 0;
	eb.fTable = defaultTable;

	Call4D( EX_FORMULA_EDITOR, &eb );
	sErrorCode = eER_NoErr;
	
	if ( eb.fParam1 )
		*len = ToUserText( eb.fHandle, text );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return (char) eb.fParam1;
}


void PA_QueryDialog( short table )
{
	EngineBlock eb;
	eb.fTable = table;
	eb.fNbSearchLines = 0;

	Call4D( EX_QUERY, &eb );
	sErrorCode = eER_NoErr;
}


void PA_OrderByDialog( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fNbSearchLines = 0;

	Call4D( EX_ORDER_BY, &eb );
	sErrorCode = eER_NoErr;
}


PA_Handle PA_PictureEditor( char* windowTitle, void* picture, long len )
{
	EngineBlock eb;

	eb.fHandle = FromUserData( picture, len );
	eb.fParam1 = 0;
	FromUserString( windowTitle, eb.fName );
	eb.fParam4 = 'PICT';

	Call4D( EX_PICTURE_EDITOR, &eb );
	sErrorCode = eER_NoErr;

	return (PA_Handle) eb.fParam1;
}


void PA_MethodEditor( char* methodName, long lineNumber )
{
	EngineBlock eb;

	eb.fManyToOne = 1;
	eb.fParam1 = 0;	// or use CC4D resource ID
	FromUserString( methodName, eb.fName );
	eb.fParam2 = lineNumber;
	eb.fError = 0;

	Call4D( EX_METHOD_EDITOR, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_Alert( char* message )
{
	EngineBlock eb;
	char string[256];

	FromUserString( message, string );
	eb.fParam1 = (long) string;

	Call4D( EX_ALERT, &eb );
	sErrorCode = eER_NoErr;
}


char PA_Confirm( char* message )
{
	EngineBlock eb;
	char string[256];

	FromUserString( message, string );
	eb.fParam1 = (long) string;

	Call4D( EX_CONFIRM, &eb );
	sErrorCode = eER_NoErr;

	return (char) eb.fParam2;
}


char PA_Request( char* message, char* value, char* okButton, char* cancelButton )
{
	EngineBlock eb;

	char string1[256];
	char string2[256];
	char string3[256];
	char string4[256];

	FromUserString( message,      string1 );
	FromUserString( value,        string2 );
	FromUserString( okButton,     string3 );
	FromUserString( cancelButton, string4 );

	eb.fParam1 = (long) string1;
	eb.fParam2 = (long) string2;
	eb.fParam3 = (long) string3;
	eb.fParam4 = (long) string4;
	
	Call4D( EX_REQUEST, &eb );
	sErrorCode = eER_NoErr;

	ToUserString( string2, value );
	
	return (char) eb.fManyToOne;
}


void PA_AboutDialog()
{
	EngineBlock eb;
	Call4D( EX_ABOUT_DIALOG, &eb );
}


// -----------------------------------------
//
//	4th Dimension Pictures
//
// -----------------------------------------


PA_Handle PA_ConvertPicture( void *picture, long len, unsigned long format )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fParam2 = 'QTIM';
	eb.fParam3 = format;
	eb.fHandle = FromUserData( picture, len );

	Call4D( EX_PICTURE_CONVERSION, &eb );
	sErrorCode = eER_NoErr;

	return (PA_Handle) eb.fParam1;
}


PA_Handle PA_PictureToGIF( void* picture, long len )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fParam2 = 'GIF ';
	eb.fHandle = FromUserData( picture, len );

	Call4D( EX_PICTURE_CONVERSION, &eb );
	sErrorCode = eER_NoErr;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return (PA_Handle) eb.fParam1;
}


// returns a HEMF
long PA_PictureToEMF( void* picture, long len )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fParam2 = 'EMF ';
	eb.fHandle = FromUserData( picture, len );
	sErrorCode = eER_NoErr;

	Call4D( EX_PICTURE_CONVERSION, &eb );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return eb.fParam1;
}



// -----------------------------------------
//
//	4th Dimension Dialogs
//
// -----------------------------------------


PA_Dial4D PA_NewDialog()
{
	EngineBlock eb;

	Call4D( EX_DIAL4D_NEW_EMPTY_DIALOG, &eb );
	return eb.fParam1;
}


PA_Dial4D PA_OpenDialog( PA_Dial4D dialog, char* dialogName, char closeBox )
{
	EngineBlock eb;
	PA_Rect rect = {0,0,0,0}; // will use default dialog rect

	eb.fParam4 = dialog;
	eb.fParam1 = 0;
	eb.fLongint = '6565';
	eb.fOneToMany = 0;	// hidden
	eb.fManyToOne = closeBox;
	eb.fParam2 = (long) & rect;
	eb.fParam3 = eWL_Dialog;	// PA_WindowLevel
	eb.fShort = 1;	// modal
	eb.fString[0] = 0; // window title
	PA_ConvertStrings( dialogName, sUserStringKind, sUserCharSet, 
					   eb.fName,   eSK_CString,     eCS_Macintosh );

	Call4D( EX_DIAL4D_OPEN_DIALOG, &eb );

	return (PA_Dial4D) eb.fParam1;
}


char PA_ModalDialog( PA_Dial4D dialog, char* variableName )
{
	EngineBlock eb;
	char val = 0;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = 0;
	
	Call4D( EX_DIAL4D_MODAL_DIALOG, &eb );
	PA_ConvertStrings( eb.fString, eSK_PString, eCS_Macintosh, 
					   variableName, sUserStringKind, sUserCharSet );

	if ( eb.fManyToOne )		// cancel
		val = 2;
	else if( eb.fOneToMany )	// validate
		val = 1;

	return val;
}


void PA_CloseDialog( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_CLOSE_DIALOG, &eb );
}


void PA_Dial4DSetEnable( PA_Dial4D dialog, char* variable, char enabled )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fManyToOne = enabled;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ENABLE, &eb );
}


PA_Variable PA_Dial4DGetVariable( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;
	PA_Variable var;

	eb.fParam1 = (long) dialog;
	eb.fHandle = (PA_Handle) &var;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_VARIABLE, &eb );
	return var;
}


void PA_Dial4DSetVariable( PA_Dial4D dialog, char* variable, PA_Variable var )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fHandle = (PA_Handle) &var;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_VARIABLE, &eb );
}


long PA_Dial4DGetLong( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fLongint = 0;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_LONG, &eb );
	return eb.fLongint;
}


void PA_Dial4DSetLong( PA_Dial4D dialog, char* variable, long value )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fLongint = value;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_LONG, &eb );
}


double PA_Dial4DGetReal( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fReal = 0.0;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_REAL, &eb );
	return eb.fReal;
}


void PA_Dial4DSetReal( PA_Dial4D dialog, char* variable, double value )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fReal = value;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_REAL, &eb );
}


void PA_Dial4DGetString( PA_Dial4D dialog, char* variable, char* string )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fName[0] = 0;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_STRING, &eb );
	ToUserString( eb.fName, string );
}


void PA_Dial4DSetString( PA_Dial4D dialog, char* variable, char* string )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	FromUserString( string, eb.fName );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_STRING, &eb );
}


void PA_Dial4DSetPictureHandle( PA_Dial4D dialog, char* variable, PA_Handle picture, PA_PictureInfo info )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fHandle = FromUserPictureHandle( picture, info );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_PICTURE, &eb );
}

// dialogs use their own variable context. If you want to access
// the current process variables, you need to switch from dialog variables
// to current process variables
void* PA_Dial4DSaveVariables( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = 0;
	Call4D( EX_DIAL4D_SAVE_VARIABLES, &eb );
	return (void*) eb.fParam2;
}

// to switch back to the dialog variable, after calling PA_Dial4DSaveVariables
// You must pass the value returned by PA_Dial4DSaveVariables
void PA_Dial4DRestoreVariables( PA_Dial4D dialog, void* env )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) env;
	Call4D( EX_DIAL4D_RESTORE_VARIABLES, &eb );
}


double PA_Dial4DGetArrayReal( PA_Dial4D dialog, char* variable, long indice )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = indice;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_ARRAY_REAL, &eb );

	return eb.fReal;
}


void PA_Dial4DGetArrayString( PA_Dial4D dialog, char* variable, char* string, long indice )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = indice;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_ARRAY_STRING, &eb );
	ToUserString( eb.fName, string );
}


long PA_Dial4DGetArrayLong( PA_Dial4D dialog, char* variable, long indice )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = indice;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_ARRAY_LONG, &eb );
	return eb.fParam3;
}


void PA_Dial4DSetAreaHandler( PA_Dial4D dialog, char* variable, void* handler, void* privateData )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = 6642;
	eb.fHandle = (PA_Handle) privateData;
	eb.fLongint = (long) handler;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_AREA_HANDLER, &eb );
}


void PA_Dial4DGetVariableRect( PA_Dial4D dialog, char* variable, PA_Rect* rect )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) rect;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_VARIABLE_RECT, &eb );
}


void PA_Dial4DSetArrayTextFromResource( PA_Dial4D dialog, char* variable, short resourceID )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = 'STR#';	// also supported : 'TXT#' and 'MENU'
	eb.fParam3 = (long) resourceID;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_RESOURCE, &eb );
}


void PA_Dial4DSetArrayTextFromTableList( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam4 = sVirtualStructureMode;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_TABLE_LIST, &eb );
}


void PA_Dial4DSetArrayTextFromFieldList( PA_Dial4D dialog, char* variable, short table )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fTable = table;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_FIELD_LIST, &eb );
}


// to be called before changing variables values
void PA_Dial4DBeginUpdateVariables( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_BEGIN_UPDATE_VARIABLES, &eb );
}


// to be called once variables values are changed
// all the modified variables will be redrawn
void PA_Dial4DEndUpdateVariables( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_END_UPDATE_VARIABLES, &eb );
}


void PA_Dial4DNewArrayString( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_NEW_ARRAY_STRING, &eb );
}


void PA_Dial4DSetArrayString( PA_Dial4D dialog, char* variable, char* string, long position )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	FromUserString( string, eb.fName );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_STRING, &eb );
}


void PA_Dial4DSetDate( PA_Dial4D dialog, char* variable, short day, short month, short year )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_DATE, &eb );
}


void PA_Dial4DGetDate( PA_Dial4D dialog, char* variable, short* day, short* month, short* year )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_DATE, &eb );

	if ( day )
		*day = eb.fDate.fDay;

	if ( month )
		*month = eb.fDate.fMonth;
	
	if ( year )
		*year = eb.fDate.fYear;
}


void PA_Dial4DShowHideVariable( PA_Dial4D dialog, char* variable, char visible )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) visible;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SHOW_HIDE_VARIABLE, &eb );
}


void PA_Dial4DGotoPage( PA_Dial4D dialog, short page )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) page;
	Call4D( EX_DIAL4D_GOTO_PAGE, &eb );
}


short PA_Dial4DGetCurrentPage( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_GET_CURRENT_PAGE, &eb );

	return (short) eb.fParam2;
}

// to get all the tables, pass 0 to table
void PA_Dial4DNewTableFieldHList( PA_Dial4D dialog, char* variable, short table,
								  char showRelatedTables, char showRelatedFields, 
								  char useIcons, char sorted )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fManyToOne = showRelatedFields;	// only if table > 0
	eb.fField  = showRelatedTables;
	eb.fParam2 = 0;
	eb.fParam3 = (long) sorted;
	eb.fOneToMany = useIcons;
	eb.fParam4 = sVirtualStructureMode;
	eb.fTable  = table;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_NEW_TABLE_FIELD_HLIST, &eb );
}


void PA_Dial4DDisposeHList( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_DISPOSE_HLIST, &eb );
}


void PA_Dial4DGetDragInfo( PA_Dial4D dialog,
						    char* dropVariable, short* dropX, short* dropY,
							PA_Dial4D* dragDialog,
							char* dragVariable, short* dragX, short* dragY )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_GET_DRAG_INFO, &eb );
	
	ToUserString( eb.fString, dropVariable );

	if ( dropX )
		*dropX = ( (PA_Point*) &eb.fParam2 )->fh;

	if ( dropY )
		*dropY = ( (PA_Point*) &eb.fParam2 )->fv;

	ToUserString( eb.fName, dragVariable );

	if ( dragDialog )
		*dragDialog = (PA_Dial4D) eb.fParam3;

	if ( dragX )
		*dragX = ( (PA_Point*) &eb.fParam4 )->fh;

	if ( dragY )
		*dragY = ( (PA_Point*) &eb.fParam4 )->fv;
}


void PA_Dial4DGetTableFieldHListCurrent( PA_Dial4D dialog, char* variable, short* table, short* field )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam4 = sVirtualStructureMode;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_TABLE_FIELD_HLIST_CURRENT, &eb );

	if ( table )
		*table = eb.fTable;

	if ( field )
		*field = eb.fField;
}


void PA_Dial4DNewArrayLong( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_NEW_ARRAY_LONG, &eb );
}


void PA_Dial4DSetArrayLong( PA_Dial4D dialog, char* variable, long value, long position )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	eb.fParam3 = value;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_LONG, &eb );
}

// each element of the text array will be filled by a paragraph of the given text.
// paragraphs in source text needs to be separated by a carriage return.
void PA_Dial4DSetArrayTextFromTTR( PA_Dial4D dialog, char* variable, char* text, long len )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = 0;
	eb.fHandle = FromUserText( text, &len );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_TTR, &eb );
	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


long PA_Dial4DGetArraySize( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_ARRAY_SIZE, &eb );

	return eb.fParam2;
}


void PA_Dial4DDeleteArrayElements( PA_Dial4D dialog, char* variable, long position, long amount )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	eb.fParam3 = amount;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_DELETE_ARRAY_ELEMENTS, &eb );
}


void PA_Dial4DRedrawExternalArea( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_REDRAW_EXTERNAL_AREA, &eb );
}


void PA_Dial4DNewArrayPicture( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_NEW_ARRAY_PICTURE, &eb );
}


void PA_Dial4DSetArrayPicture( PA_Dial4D dialog, char* variable, void* picture, long len, PA_PictureInfo info, long position )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	eb.fHandle = FromUserPicture( picture, len, info );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_PICTURE, &eb );
	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


void PA_Dial4DSetArrayPictureHandle( PA_Dial4D dialog, char* variable, PA_Handle hpicture, PA_PictureInfo info, long position )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	eb.fHandle = FromUserPictureHandle( hpicture, info );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_PICTURE, &eb );
}

// idArray is an array of short that holds the ID of the PICT resources to load.
// nb is the number of ids in the array 
void PA_Dial4DSetArrayPictureFromResources( PA_Dial4D dialog, char* variable, short* idArray, long nb )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fHandle = (PA_Handle) idArray;
	eb.fParam2 = nb;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_PICT_FROM_RESOURCES, &eb );
}


void PA_Dial4DSetEnterable( PA_Dial4D dialog, char* variable, char enterable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fManyToOne = enterable;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ENTERABLE, &eb );
}


void PA_Dial4DSetMin( PA_Dial4D dialog, char* variable, double minValue )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = eVK_Real;
	eb.fReal   = minValue;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_MIN, &eb );
}


void PA_Dial4DSetMinDate( PA_Dial4D dialog, char* variable, short day, short month, short year )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = eVK_Date;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_MIN, &eb );
}


void PA_Dial4DSetMax( PA_Dial4D dialog, char* variable, double maxValue )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = eVK_Real;
	eb.fReal   = maxValue;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_MAX, &eb );
}


void PA_Dial4DSetMaxDate( PA_Dial4D dialog, char* variable, short day, short month, short year )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = eVK_Date;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_MAX, &eb );
}


void PA_Dial4DSetUserData( PA_Dial4D dialog, void* userData )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) userData;
	Call4D( EX_DIAL4D_SET_USER_DATA, &eb );
}


void* PA_Dial4DGetUserData( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_GET_USER_DATA, &eb );
	return (void*) eb.fParam2;
}


void PA_Dial4DSet3StatesCheckBox( PA_Dial4D dialog, char* variable, char state )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) state;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_3_STATES_CHECKBOX, &eb );
}


PA_Dial4D PA_Dial4DDial4DFromWindow( PA_WindowRef window )
{
	EngineBlock eb;

	eb.fParam2 = (long) window;
	Call4D( EX_DIAL4D_DIAL4D_FROM_WINDOW, &eb );
	return (long) eb.fParam1;
}


PA_WindowRef PA_Dial4DWindowFromDial4D( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_WINDOW_FROM_DIAL4D, &eb );
	return (PA_WindowRef) eb.fParam2;
}


void PA_Dial4DSetTableFieldHListCurrent( PA_Dial4D dialog, char* variable, short table, short field )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam4 = sVirtualStructureMode;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_TABLE_FIELD_HLIST_CURRENT, &eb );
}


void PA_Dial4DSetText( PA_Dial4D dialog, char* variable, char* text, long len )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fHandle = FromUserText( text, &len );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_TEXT, &eb );
}


long PA_Dial4DGetText( PA_Dial4D dialog, char* variable, char* text )
{
	EngineBlock eb;
	long len;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_TEXT, &eb );
	len = ToUserText( eb.fHandle, text );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return len;
}


void PA_Dial4DNewArrayReal( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_NEW_ARRAY_REAL, &eb );
}


void PA_Dial4DSetArrayReal( PA_Dial4D dialog, char* variable, double value, long position )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fReal = value;
	eb.fParam2 = position;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_REAL, &eb );
}


void PA_Dial4DSetEntryFilter( PA_Dial4D dialog, char* variable, char* entryFilter )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	ToUserString( entryFilter, eb.fName );
	Call4D( EX_DIAL4D_SET_ENTRY_FILTER, &eb );
}


void PA_Dial4DSetDisplayFormat( PA_Dial4D dialog, char* variable, char* displayFormat )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	ToUserString( displayFormat, eb.fName );
	Call4D( EX_DIAL4D_SET_DISPLAY_FORMAT, &eb );
}


void PA_Dial4DNewArrayText( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_NEW_ARRAY_TEXT, &eb );
}


void PA_Dial4DSetArrayText( PA_Dial4D dialog, char* variable, char* text, long len, long position )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	eb.fHandle = FromUserText( text, &len );
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT, &eb );
	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );
}


long PA_Dial4DGetArrayText( PA_Dial4D dialog, char* variable, char* text, long position )
{
	EngineBlock eb;
	long len;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = position;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_ARRAY_TEXT, &eb );

	len = ToUserText( eb.fHandle, text );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return len;
}


void PA_Dial4DDisposeEmptyDialog( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_DISPOSE_EMPTY_DIALOG, &eb );
}


void PA_Dial4DShowHideObject( PA_Dial4D dialog, char* objectName, char visible )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) visible;
	ToDialVarName( objectName, eb.fString );
	Call4D( EX_DIAL4D_SHOW_HIDE_OBJECT, &eb );
}


void PA_Dial4DSetTime( PA_Dial4D dialog, char* variable, long time )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fLongint = time;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_TIME, &eb );
}


long PA_Dial4DGetTime( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GET_TIME, &eb );

	return eb.fLongint;
}


void PA_Dial4DSetObjectTitle( PA_Dial4D dialog, char* objectName, char* title )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fManyToOne = 0;	// 1 to pass variable name
	ToUserString( title, eb.fName );
	ToDialVarName( objectName, eb.fString );
	Call4D( EX_DIAL4D_SET_OBJECT_TITLE, &eb );
}


void PA_Dial4DUpdateObjectTitle( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_UPDATE_OBJECT_TITLE, &eb );
}


void PA_Dial4DSetVariableRect( PA_Dial4D dialog, char* variable, PA_Rect rect )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) &rect;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_SET_VARIABLE_RECT, &eb );
}


void PA_Dial4DAllowXResize( PA_Dial4D dialog, char allowResize )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) allowResize;
	Call4D( EX_DIAL4D_ALLOW_X_RESIZE, &eb );
}


void PA_Dial4DAllowYResize( PA_Dial4D dialog, char allowResize )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) allowResize;
	Call4D( EX_DIAL4D_ALLOW_Y_RESIZE, &eb );
}


void PA_Dial4DGetWindowMinMaxInfo( PA_Dial4D dialog,
									long* minXresize, long* maxXresize,
									long* minYresize, long* maxYresize )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_GET_WINDOW_MIN_MAX_INFO, &eb );

	if ( minXresize )
		*minXresize = eb.fParam1;

	if ( maxXresize )
		*maxXresize = eb.fParam2;

	if ( minYresize )
		*minYresize = eb.fParam3;

	if ( maxYresize )
		*maxYresize = eb.fParam4;

}


void PA_Dial4DSetWindowSize( PA_Dial4D dialog, long width, long height )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = width;
	eb.fParam3 = height;
	Call4D( EX_DIAL4D_SET_WINDOW_SIZE, &eb );
}


long PA_Dial4DFindArrayLong( PA_Dial4D dialog, char* variable, long value, long startPosition )
{
	EngineBlock eb;

	eb.fParam1  = (long) dialog;
	eb.fLongint = value;
	eb.fParam2  = startPosition;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_FIND_ARRAY_LONG, &eb );

	return eb.fParam2;
}


long PA_Dial4DFindArrayReal( PA_Dial4D dialog, char* variable, double value, long startPosition )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fReal = value;
	eb.fParam2 = startPosition;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_FIND_ARRAY_REAL, &eb );

	return eb.fParam2;
}


void PA_Dial4DGetLastObject( PA_Dial4D dialog, char* objectName )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	Call4D( EX_DIAL4D_GET_LAST_OBJECT, &eb );
	ToUserString( eb.fName, objectName );
}


void PA_Dial4DGotoVariable( PA_Dial4D dialog, char* variable )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_GOTO_VARIABLE, &eb );
}


void PA_Dial4DCancelValidate( PA_Dial4D dialog, char cancel )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) dialog;
	eb.fParam2 = (long) cancel;
	Call4D( EX_DIAL4D_CANCEL_VALIDATE, &eb );
}


void PA_Dial4DHighlightText( PA_Dial4D dialog, char* variable, short startSelection, short endSelection )
{
	EngineBlock eb;

	eb.fParam1 = (long) dialog;
	eb.fParam2 = (long) startSelection;
	eb.fParam3 = (long) endSelection;
	ToDialVarName( variable, eb.fString );
	Call4D( EX_DIAL4D_HIGHLIGHT_TEXT, &eb );
}

