/*
*       ASIEXTRN.H
*       Copyright (c) 1991 Altura Software, Inc. All Rights Reserved.
*       Created by:     LJL     9/30/92
*       Modified by:    jg     12/23/93
*
*	mod 07 26 95 lve - add ControlToHwnd and two new ASISV_gl values 
*	mod 07 05 95 shm - add ASI_SeparateFile 
*	mod 06 07 95 shm - add ASI_WinTabMessage
*	mod 06 07 95 shm - add WM_ASI_WTBASE for base of 16 WinTab messages
*			(Windows)
*	mod 03 20 95 shm - add ASI_MoveWindow
*	mod 03 17 95 shm - add ASI_GetItemEnabled
*       mod 02/16/95 RSE - add ACTIVECLIKLOOP flags for DeltaPoint
*	mod 02/02/95 csk - ASI_GetCurrentDC() ASI_PolyPolygon()
*	mod 12/29/94 BP  - Added M2W.m2w_lpAppSetupDlg 
*	mod 10/31/94 csk - expose menu mapping code
*	mod 10/06/94 dlb - prevent multi def warning of BOOLEAN
*	mod 10/14/94 DRR - added new system values ASISV_gl_WindowProcID
*			and ASISV_gl_WindowMenuHandle
*	mod 10/10/94 DRR - added new system value ASISV_gl_WindowCreateFlags
*			to replace other ways of modifying the behavior of
*			myNewWindow(); added WCF_* #defines to go with this
*	mod 09/13/94 DRR - added a new system value ASISV_gl_NoCloseCommand
*	mod 09/27/94 mes - add WING support 
*	mod 07/20/94 llb - Obsolete MapUpperTEOutput,
*			rename MapUpperInput to MapUpperChars.
*	mod 07/08/94 RSE - added LSETSEL flags for no kbd input, no selections
*	mod 07/06/94 RSE - changed name and effect of DlgKBHookProcPtr's last
*			parameter
*	mod 06/27/94 rb  - add UNFOLD_ReleaseHBM flag
*	mod 06/27/94 LLB - add ASISV_gl_ExternalMouseEventsInModalDialog.
*	mod 06/25/94 dac - add ASISV_gl_ModalDialogActive (read-only)
*	mod 05/19/94 llb - Added FOLD_4BitAs4Bit
*	mod 05/10/94 rb  - Added palette to ASI_NewDDBPixmap
*	mod 05/02/94 rb  - Added ASISV_gl_DoesPartialGetSetDIBits
*			and ASISV_gl_CurrentHPALETTE
*	mod 04/08/94 DRR - Added ASISV_gl_OnePixelPatternLines
*	mod 04/08/94 RSE - move HMENU stuff so it will compile
*	mod 03/15/94 RHK  - Add SysFontFam & Time
*	mod 03/15/94 llb - Added ASISV_gl_NoSetSysColors
*	mod 03/10/94 rb  - Add ASI_MemCheck()
*	mod 02/10/94 ms  - make ASI_AllowPictStatePush return previous state
*	mod 02/08/94 rb  - Added M2W.m2w_glDibDrvName
*			Added ASI_SystemValue ASISV_gl_MemSubMax
*	mod 02/07/94 llb - added ASI_ForcePalette
*	mod 02/01/94 rb  - ASI_NewDDBPixMap
*	mod 01/26/94 RSE - Add ASI_SetBigIText()
*	mod 01/23/94 jg  - change binding of ASI_DSSimple
*	mod 01/17/94 jg  - add UNFOLD_UsePalColors and UNFOLD_InvertPalColors
*	mod 01/06/94 jg  - add UNFOLD_NoBaseAddrReAlloc
*	mod 12/23/93 jg  - ASI_DSSimple, ASI_DSNotSimple, and
*			ASI_CopyBitsMacToDIB
*	mod 11/18/93 jg  - ASI_NewDIBPixMap
*	mod 10/28/93 jg  - ASI_GetDIBFromPixMap
*	mod 10/21/93 jg  - ASI_BeginPaletteAnimation / ASI_EndPaletteAnimation
*	mod 8 7 94 tsh - add ASI_DialogFontMode
*	mod 9 1 93 tsh - add ASI_QDInitTestScreen
*	mod 08 18 83 shm - add ASI_BandInfo
*	mod 08/20/93 LJL - add class name and resource name strings to
*			M2W struct, recompile ASIWINIT.C
*	mod 08/13/93 LJL - add freeHBM parameter to ASI_UnfoldPixMap
*	mod 07 09 93 shm - fix if _INC_WINDOWS already defined but _WINDOWS_
*		  is not
*   	mod 07 08 93 shm - make it compile in Borland C
*       mod 03/10/93  jg - #2349: use printer text metrics
*       mod  1/17/93  jg - #487: allow use of rectangle for line drawing
*       mod 12 19 92 tsh - add ASI_SetComboBoxSelect
*       mod 12 17 92 tsh - fix for Borland C
*       mod 12/10/92 DRR - add #defines for ASI_WinIniChange() return codes
*/

#ifndef __ASIEXTRN__
#define	__ASIEXTRN__
#ifndef	_ASIPORT_
#include "asiport.h"
#endif

#ifdef WINVER
#define WM_ASI_WTBASE 0x7ff0
#endif

#if !defined(BOOLEAN)
typedef unsigned int	BOOLEANUI;
#define BOOLEAN BOOLEANUI
#endif

//typedef void VOID;

#ifndef VOID
#define VOID void
#endif

#ifdef	WinPixMapPtr
#define	wPixMapPtr	WinPixMapPtr
#else
#define	wPixMapPtr	void *
#endif

#ifdef	WinRgnHandle
#define	wRgnHandle	WinRgnHandle
#else
#define	wRgnHandle	void **
#endif

#if defined(_WINDOWS_) || defined(__WINDOWS_H)
#define _INC_WINDOWS
#endif

#if !defined(_INC_WINDOWS)
#ifdef STRICT
typedef const void NEAR*        HANDLE;
#define DECLARE_HANDLE(name)    struct name##__ { int unused; }; \
                                typedef const struct name##__ NEAR* name
#define DECLARE_HANDLE32(name)  struct name##__ { int unused; }; \
                                typedef const struct name##__ FAR* name
#else   /* STRICT */
typedef UINT                    HANDLE;
#define DECLARE_HANDLE(name)    typedef UINT name
#define DECLARE_HANDLE32(name)  typedef DWORD name
#endif  /* !STRICT */

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HPALETTE);

#define LPBITMAPINFO	void *
#endif

#ifndef __TYPES__
#include <MacTypes.H>
#endif

#ifndef __MEMORY__
#include <MemoryM.H>
#endif

#ifndef __QUICKDRAW__
#if LONG_FILENAMES
#include <Quickdraw.H>
#else
#include <Quickdra.H>
#endif
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.H>
#endif

#ifndef __WINDOWS__
#include <WindowM.H>
#endif

#ifndef __DIALOGS__
#include <Dialogs.H>
#endif

#ifndef __STANDARDFILE__
#if LONG_FILENAMES
#include <StandardFile.h>
#else
#include <Standard.h>
#endif
#endif

#ifndef __LISTS__
#include <Lists.H>
#endif

#ifndef __MENUS__
#include <Menus.H>
#endif

#ifndef __SCRAP__
#include <Scrap.H>
#endif

#if PPC_HEADERS
  #ifndef __PRINTING__
    #include <Printing.H>
  #endif
#else
  #ifndef __PRINTTRAPS__
    #if LONG_FILENAMES
      #include <PrintTraps.H>
    #else
      #include <PrintTra.H>
    #endif
  #endif
#endif

#ifndef __FONTS__
#include <Fonts.H>
#endif

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

					/* structure for passing Mac2Win*/
					/*   session information in     */
					/*   ASI_InitSession()          */
#define M2W_VERSION 0x0102

#if MACVER
#define _huge
#endif

#ifdef _INC_WINDOWS
typedef struct
{
	SWORD           m2w_size;
	SWORD           m2w_version;
	LONG            m2w_SessionID;
	HINSTANCE       m2w_hInst;
	HWND            m2w_hMainWnd;
	HWND            m2w_hDeskWnd;
	LONG            m2w_ASIApp;
	HACCEL          m2w_hAccTable;
	FARPROC         m2w_lpMakeProcInst;
	FARPROC         m2w_lpFreeProcInst;
	FARPROC         m2w_lpAppScrap;
	FARPROC         m2w_lpAppByteSwap;
	ResType         *m2w_lpAppResType;
	LPSTR           m2w_lpCmdLine;
	FARPROC         m2w_lpAppPrintDlg;
	LPSTR           m2w_lpCurDir;
	LPSTR		m2w_lpMainWndClassName;
	LPSTR		m2w_lpSubWndClassName;
	LPSTR		m2w_lpSubMDIWndClassName;
	LPSTR		m2w_lpASIPortRSRName;
	LPSTR		m2w_lpDibDrvName;
	FARPROC		m2w_lpAppSetupDlg;
	LPSTR		m2w_lpDialogWndClassName;
} M2W, *pM2W, **hM2W;
#endif

					/* ptr to Mac Globals structure */
					/*   used by ASI_InitGlobals()  */
#define MG_VERSION 0x0100

#if MACVER
#define WidthTableHandle Handle
#endif

typedef struct
{
	SWORD           mg_size;
	SWORD           mg_version;
	short           *mg_SFSaveDisk;
	long            *mg_CurDirStore;
	short           *mg_SFVRefNum;
	GDHandle        *mg_MainDevice;
	GDHandle        *mg_DeviceList;
	GrafPtr         *mg_WMgrPort;
	struct QHdr     *mg_EventQueue;
	SWORD           *mg_MBarHeight;
	SWORD           *mg_MemErr;
	RgnHandle       *mg_GrayRgn;
	WindowPeek      *mg_WindowList;
	GrafPtr         *mg_thePort;
	Pattern         *mg_white;
	Pattern         *mg_black;
	Pattern         *mg_gray;
	Pattern         *mg_ltGray;
	Pattern         *mg_dkGray;
	Cursor          *mg_arrow;
	BitMap          *mg_screenBits;
	long            *mg_randSeed;
	ScrapStuff      *mg_ScrapInfo;
	short           *mg_PrintErr;
	long            *mg_Ticks;
	Ptr             *mg_ApplLimit;
	THz             *mg_ApplZone;
	Ptr             *mg_CurrentA5;
	Ptr             *mg_CurStackBase;
	long            *mg_CaretTime;
	WindowPtr       *mg_CurActivate;
	char            *mg_HiliteMode;
	GDHandle        *mg_TheGDevice;
	SWORD           *mg_ResErr;
	char            *mg_CrsrNew;
	RGBColor        *mg_HiliteRGB;
	char            *mg_MBState;
	Point           *mg_MTemp;
	Point           *mg_Mouse;
	Point           *mg_RawMouse;
	char            *mg_FractEnable;
	WidthTableHandle *mg_WidthTabHandle;
	SWORD           *mg_TheMenu;
	ProcPtr         *mg_ResumeProc;
	short           *mg_SysEvtMask;
	long            *mg_DoubleTime;
	THz             *mg_TheZone;
	Handle          *mg_MenuList;
	short           *mg_MenuFlash;
	short           *mg_TEScrpLength;
	Handle          *mg_TEScrpHandle;
	Boolean         *mg_ResLoad;
	short           *mg_PaintWhite; 
	long            *mg_MenuDisable;
	WindowPtr       *mg_CurDeactive;
	short           *mg_KeyThresh;
	short	        *mg_TEScrpHasStyle;
	Handle          *mg_TEScrpStyleHandle;
	Handle		*mg_AppParmHandle;
	short		*mg_AtMenuBottom;
	short		*mg_TopMenuItem;
#if 0
	short		*mg_SysFontFam;
	unsigned long		*mg_Time;
#endif
} MG, *pMG, **hMG;


//ASIMAIN.c
EXTERN F_ASIBND( LONG ) ASI_SetFlags( LONG ASI_carebits, LONG newASIApp, 
		 	UWORD qd_carebits, UWORD qdflags );
#if WINVER
EXTERN F_ASIBND( VOID ) ASI_Set_MacWin(rsrc_map *mm, menu_override *mo);
EXTERN F_ASIBND( VOID ) ASI_InitColors(void);
#endif

#ifdef _INC_WINDOWS
EXTERN F_ASIBND( UWORD )        ASI_InitGlobals( pMG pAppMG );
EXTERN F_ASIBND( VOID ) ASI_InitSession( pM2W pAppM2W );
EXTERN F_ASIBND( VOID ) ASI_ExitSession(void);
#endif

EXTERN F_ASIBND( BOOLEAN )      ASI_EnumWnd(HWND hWnd, LONG lvalue);
EXTERN F_ASIBND( VOID ) ASI_MakeAppPathName(char * szFileName, char * szAddName);

//CONTROLM.c
EXTERN F_ASIBND( VOID ) ASI_SetScrollBarWH( SWORD w, SWORD h );
EXTERN F_ASIBND( VOID ) ASI_SetWindowDefaultButton( ControlHandle ch, 
						Boolean SetDefault );
EXTERN F_PASCAL( void )	ASI_SetCtlFocus(ControlHandle aCMgrControl, 
						Boolean on);
EXTERN F_PASCAL( void ) ASI_ContrlOwnerChange( ControlHandle aCMgrControl, WindowPtr newowner);
EXTERN	F_ASIBND( HWND ) ASI_ControlToHWND( ControlHandle ch );

//EVENTMGR.c
EXTERN F_ASIBND( BOOLEAN )      ASI_PlayBack( BOOLEAN playbackon );
EXTERN F_ASIBND( VOID ) ASI_ActivateApp( BOOLEAN activateIt );
EXTERN F_ASIBND( VOID ) ASI_VBL_Interrupt(VOID);
EXTERN F_ASIBND( BOOLEAN )	ASI_TestUpdate( WindowPeek wPtrTemp );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_EventKey( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( LONG ) ASI_EventMouse( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( VOID ) ASI_EventCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
#endif

EXTERN F_ASIBND( SWORD )        ASI_ForceCapture( SWORD newValue );
EXTERN F_ASIBND( VOID ) ASI_SetCapture( BOOLEAN set_flag );

EXTERN F_ASIBND( void ) ASI_SetWinCapture(WindowPtr wptr, BOOLEAN set_flag);
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_SetMessageHook( FARPROC lpfnMessageHook );
EXTERN F_ASIBND( VOID ) ASI_SetPeekMessageProc( FARPROC lpfnPeekMessageProc );
#endif

//MENUMGR.c
EXTERN F_ASIBND( VOID ) ASI_AddResFontMenu( MenuHandle theMenu, 
						BOOLEAN trueTypeOnly );
EXTERN F_ASIBND( MenuHandle )	ASI_SearchHierMenuList( BOOLEAN searchFirst );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_InitMenuMsg( HMENU hSubMenu );
EXTERN F_ASIBND( VOID ) ASI_EventMenuSel( UWORD menuID );
EXTERN F_ASIBND( BOOLEAN )      ASI_MenuSelect( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( VOID ) ASI_EnterIdle( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( BOOLEAN )      ASI_MenuUserItem( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( VOID ) ASI_ParentNotify( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( SWORD ) ASI_MenuIDFromCMD(HMENU theMenu, UWORD cmd  );
EXTERN F_ASIBND( SWORD ) ASI_MenuIDFromHMENU(HMENU theMenu  );
#endif
#if !PPC_HEADERS
typedef void (pascal *MenuHookProcPtr)();
#endif
EXTERN F_ASIBND( void )	ASI_SetMenuHook( MenuHookProcPtr menuHookProc );
EXTERN F_ASIBND( MenuHookProcPtr )	ASI_GetMenuHook( void );
EXTERN F_ASIBND( BOOLEAN ) ASI_SetMenuResFile( short index,
				MenuHandle theMenu, short resFH );
EXTERN F_ASIBND( long ) ASI_PopUpMenuSelectUsingDiffFont( MenuHandle theMenu, 
					short top, short left, 
					short popUpItem, LPSTR lpFontName,
					short fontHeight );
EXTERN F_ASIBND( VOID ) ASI_CopyMacPtoWinC( BYTE *pstr, BYTE *pcstr,
				BYTE *palpha );


//PRINTTRA.c
EXTERN F_ASIBND( SWORD )        ASI_GetCollateCopies(void);
EXTERN F_ASIBND( void )		ASI_GetWinPrintInfo(THPrint hPrint, 
					HDC * pPrDC, HANDLE * pPrDlg);
EXTERN F_ASIBND( VOID ) ASI_SetPrintFileName(LPSTR lp);
	/*  Defines for return code from ASI_WinIniChange()     */
#define INI_CHANGE_WINDOWS      (1 << 0)
#define INI_CHANGE_DEVICES      (1 << 1)
#define INI_CHANGE_INTL         (1 << 2)
EXTERN F_ASIBND( UWORD )        ASI_WinIniChange(LPSTR lp);
EXTERN F_ASIBND( VOID )		ASI_AbortToTop(void);
EXTERN F_ASIBND( BOOLEAN )	ASI_BandInfo( BOOLEAN *pfGraphics, 
							BOOLEAN *pfText );
EXTERN F_ASIBND( BOOLEAN )      ASI_PrOpenPageBand(TPPrPort pPrPort, 
					TPRect pPageFrame, Rect *pBandRect );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) 	ASI_SetupPrintDlgHook(FARPROC apPrintDlg);
EXTERN F_ASIBND( VOID ) 	ASI_SetupPrSetupDlgHook(FARPROC apSetupDlg);
#endif
EXTERN F_ASIBND( VOID )		ASI_ReleasePrinter(THPrint hPrint);
EXTERN F_ASIBND( VOID )		ASI_Print72DPI(THPrint hPrint, short set);
EXTERN F_ASIBND( VOID )		ASI_FreeTHPrintRef (THPrint hPrint);
//DEVICEM.C
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( LRESULT ) ASI_WinTabMessage( HWND hWnd, UINT message,
					WPARAM wParam, LPARAM lParam );
#endif

//QUICKDR.c
EXTERN F_ASIBND( VOID ) ASI_StdProcFix( BOOLEAN makeThem );
EXTERN F_ASIBND( HDC )  ASI_SwaphDC( HDC hNewDC, BOOLEAN SetIt );
EXTERN F_ASIBND( VOID ) ASI_SetNextOpenhDC( HDC hNextDC );
EXTERN F_ASIBND( VOID ) ASI_PortRectChange( VOID );
EXTERN F_ASIBND( VOID ) ASI_UseRectForLine( BOOLEAN bUseRect );
EXTERN F_ASIBND( HWND ) ASI_SetWMgrPortDC( BOOLEAN setIt, HWND hNewWnd, 
					HDC hNewScrnDC, Rect *pNewPortRect );
EXTERN F_ASIBND( VOID )	OpenPortForHDC( CGrafPtr pCPort, HDC hDC );
EXTERN F_ASIBND( VOID ) ASI_MoveFixedAmount( BOOLEAN bRelative, Fixed horz,
	Fixed vert );
EXTERN F_ASIBND( VOID ) ASI_ChangeThePortAttr( VOID );
EXTERN F_ASIBND( VOID ) ASI_DialogFontMode( WindowPtr wind, BOOLEAN dlgmode );
EXTERN F_ASIBND( BOOLEAN ) ASI_GetItemEnabled( MenuHandle, short );
EXTERN F_ASIBND( VOID ) ASI_MoveWindow( WindowPtr, short, short, Boolean);
EXTERN F_ASIBND( void )	ASI_ScreenToLocal( Point *pt );
EXTERN F_ASIBND( void )	ASI_LocalToScreen( Point *pt );

EXTERN F_ASIBND( void ) ASI_XFormToQDRect( Rect *r );
EXTERN F_ASIBND( VOID )	ASI_ResetDC();
EXTERN F_ASIBND( HDC ) 	ASI_GetCurrentDC(void);

// Defines for ASI_GetSysColor
#define ASI_COLOR_SCROLLBAR		0
#define ASI_COLOR_BACKGROUND		1
#define ASI_COLOR_ACTIVECAPTION		2
#define ASI_COLOR_INACTIVECAPTION	3
#define ASI_COLOR_MENU			4
#define ASI_COLOR_WINDOW		5
#define ASI_COLOR_WINDOWFRAME		6
#define ASI_COLOR_MENUTEXT		7
#define ASI_COLOR_WINDOWTEXT		8
#define ASI_COLOR_CAPTIONTEXT		9
#define ASI_COLOR_ACTIVEBORDER		10
#define ASI_COLOR_INACTIVEBORDER	11
#define ASI_COLOR_APPWORKSPACE		12
#define ASI_COLOR_HIGHLIGHT		13
#define ASI_COLOR_HIGHLIGHTTEXT		14
#define ASI_COLOR_BTNFACE		15
#define ASI_COLOR_BTNSHADOW		16
#define ASI_COLOR_GRAYTEXT		17
#define ASI_COLOR_BTNTEXT		18
#define ASI_COLOR_INACTIVECAPTIONTEXT	19
#define ASI_COLOR_BTNHIGHLIGHT		20
EXTERN 	F_ASIBND( VOID ) ASI_GetSysColor( int DisplayElement,
	RGBColor *MacColor );

//QDFONTIN.C
EXTERN F_ASIBND( SWORD ) ASI_GetIntlSystemFont( VOID );

//QDINIT.C
EXTERN F_ASIBND( VOID ) ASI_QDInitTestScreen( VOID );

//SANE.c
// included in SANE.H
//EXTERN F_ASIBND( extended )     ASI_Cos( extended );
//EXTERN F_ASIBND( extended )     ASI_Sin( extended );
//EXTERN F_ASIBND( extended )     ASI_Tan( extended );
//EXTERN F_ASIBND( extended )     ASI_Exp( extended );

//SCRAPMGR.c
EXTERN F_ASIBND( VOID ) ASI_DrawClipboard(void);
EXTERN F_ASIBND( short )        ASI_AnsiToMacBuf( unsigned char const *AnsiStr,
				unsigned char *MacStr, short ansiLen);
#define MTA_ISTRUETYPE 0x0001
#define MTA_FOROUTPUT 0x0002
#define MTA_FOREXTTEXTOUT 0x0004
#define MTA_USESHIFTJIS (0x0008)		/* RHKj 12/29/94 ShiftJis support  */

EXTERN F_ASIBND( short )        ASI_MacToAnsiBuf(unsigned char const *MacStr,
				unsigned char *AnsiStr, short macLen, 
				short convFlag );

//WINDOWM.c
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( BOOLEAN )      ASI_GetMinMaxInfo( HWND hWnd, 
		MINMAXINFO *lpWinPos );
EXTERN F_ASIBND( VOID )         ASI_GetMinMaxInfo2( HWND hWnd, 
		MINMAXINFO *lpWinPos, SWORD minClientX, SWORD minClientY);
EXTERN F_ASIBND( BOOLEAN )      ASI_SysCommand( BOOLEAN isMain, HWND hWnd, UWORD SysCmd );
EXTERN F_ASIBND( BOOLEAN )      ASI_EventUpdate( HWND hSubWnd );
EXTERN F_ASIBND( LONG ) ASI_EventActivate( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN	F_ASIBND( VOID ) ASI_FixupSystemMenu( HWND hWnd, HMENU hMenu );
#endif

EXTERN F_ASIBND( VOID ) ASI_SetMinWinSize( short Width, short Height );
EXTERN F_ASIBND( VOID )	ASI_SetwPtrMinSize( WindowPeek wPtr, short Width, short Height );
EXTERN F_ASIBND( VOID ) ASI_ClipRectChange( GrafPtr pPort );
EXTERN F_ASIBND( VOID ) ASI_ClipRgnChange( GrafPtr pPort );
EXTERN F_ASIBND( VOID ) ASI_VisRgnChange( GrafPtr pPort );
EXTERN F_ASIBND( BOOLEAN )      ASI_CheckUpdateRgn( WindowPeek wPtr );
EXTERN F_ASIBND( LONG ) ASI_WindDiffSize( WindowPeek wPtr, BOOLEAN *pIsIconic );
EXTERN F_ASIBND( VOID ) ASI_WindowLocation( BOOLEAN saveIt, WindowPeek wPtr, 
							Rect *pWinLocation );
EXTERN F_ASIBND( VOID ) ASI_CheckMaxWindowRect( Rect *pwr, BOOLEAN adjustWH, 
						BOOLEAN screenRelative );
EXTERN  F_ASIBND( VOID ) ASI_TopMainWnd(VOID);

//BYTESWAP.c
EXTERN F_ASIBND( VOID ) ASI_ByteSwapWORD( UWORD *ptheWd );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapLONG( LONG *ptheLg );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapRect( Rect *ptheRect );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapLONGS( LONG *pData, LONG lSize );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapWORDS( UWORD *pData, LONG lSize );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapFLOAT( float * pFloat );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapFLOATS( float * pFloat, short size );
EXTERN F_ASIBND( VOID ) ASI_ByteSwap( BOOLEAN Mac2Win, ResType theRT, 
				SWORD theID, LONG lres_size, LPSTR lpdata );
EXTERN F_ASIBND( void )	ASI_ByteSwapDOUBLE( double * pDouble );
EXTERN F_ASIBND( void )	ASI_ByteSwapEXTENDED( long double * pDouble );
EXTERN F_ASIBND( void ) ASI_ByteSwap881( double881 *pDouble );
EXTERN F_ASIBND( void ) ASI_ByteSwapCLUT( BOOLEAN Mac2Win, CTabPtr pCTab );
EXTERN F_ASIBND( VOID )	ASI_Mac2WinCursor( BOOLEAN createCursor, 
				BOOLEAN isColorCursor, LPSTR lpData,
				BOOLEAN Mac2Win, BOOLEAN byteswapit );

#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_ByteSwap_Code( BOOLEAN Mac2Win, ResType theRT,
				SWORD theID, LONG lres_size, LPSTR lpdata,
				FARPROC optByteSwapProc);
#endif

//QDRGN.c
EXTERN F_ASIBND( BOOLEAN )      ASI_IsRectRgn( RgnHandle rgn );

#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_RgnGetBBoxRECTFromRgn( RgnHandle rgn, RECT *pr );
#endif

EXTERN F_ASIBND( HRGN ) ASI_RgnSetToRealRectRgn( RgnHandle rgn );
EXTERN F_ASIBND( HRGN ) ASI_RgnSetToFakeRectRgn( RgnHandle rgn );
EXTERN F_ASIBND( UWORD )        ASI_SelectClipRgn( HDC hDC, RgnHandle rgn );

//HFS.c
EXTERN  F_ASIBND( SWORD )       ASI_FHCreate( LPSTR lpName, UWORD attributes );
EXTERN  F_ASIBND( BOOLEAN )     ASI_FHReOpen( BOOLEAN doReOpen, SWORD MacRefNum );
EXTERN  F_ASIBND( VOID )        ASI_FHReOpenAll( BOOLEAN doReOpen );
EXTERN  F_ASIBND( SWORD )       ASI_FHOpen( LPSTR lpName, UWORD mode, SWORD *pError );
EXTERN  F_ASIBND( SWORD )       ASI_FHMap( SWORD MacRefNum );
EXTERN  F_ASIBND( SWORD )       ASI_FHUnMap( SWORD DosFH );
EXTERN  F_ASIBND( SWORD )       ASI_FHClose( SWORD MacRefNum ); 
EXTERN  F_ASIBND( LONG )        ASI_FHRead( SWORD MacRefNum, void _huge* buffptr, LONG lcnt);
EXTERN  F_ASIBND( LONG )        ASI_FHWrite( SWORD MacRefNum, void _huge* buffptr, LONG lcnt);
EXTERN  F_ASIBND( LONG )        ASI_FHLSeek( SWORD MacRefNum, LONG seekPtr, UWORD position );
EXTERN  F_ASIBND( OSErr )       ASI_FHSetEOF0( SWORD MacRefNum );
EXTERN  F_ASIBND( UWORD )       ASI_FHGetAttr( SWORD MacRefNum );

//FILEMGR.C
EXTERN  F_ASIBND( short )       ASI_DOSVolToVRefNum( LPSTR pPath );
EXTERN  F_ASIBND( BOOLEAN )     ASI_VRefNumToDOSVol( short VRefNum, LPSTR pPath );
EXTERN F_ASIBND(OSErr)		ASI_HFSpec( long ioDirID, short VRefnum,
					Str255 fileName, LPSTR cfilename );
EXTERN F_ASIBND(OSErr)		ASI_FSpec( SWORD VRefNum, Str255 fileName, 
					LPSTR cfilename );
EXTERN F_ASIBND( long )         ASI_DirIDFromPath( LPSTR pPath,
                                        short * pVRefNum, short * pWDRefNum );

//STDFILEP.c
EXTERN F_ASIBND( VOID ) ASI_SF_String(SWORD numTypes, SWORD defTypeIndex, SFTypeList typeList);
EXTERN F_ASIBND( VOID ) ASI_DosFSpec2MacSFReply( BOOLEAN appendExt, 
			BOOLEAN overrideExt, SWORD SFExtIndex, 
			BOOLEAN updateSFCurDir,
			BOOLEAN updateLastGetName,
			LPSTR pFileSpec, SFReply *reply );
EXTERN F_ASIBND( VOID ) ASI_SeparateFile(char *lpDestPath, char *lpDestFileName, char *lpSrcFileName);

//DRAWPIC.c
EXTERN F_ASIBND( PicHandle )    ASI_OpenScrapPict( Rect *picFrame );
EXTERN F_ASIBND( BOOLEAN )	ASI_AllowPictStatePush( BOOLEAN bAllow );
EXTERN F_ASIBND( void )		ASI_SetFixXORMode( short state );
EXTERN F_ASIBND( void *) ASI_PictToEmf(PicHandle srcpict,char* filename); // ev 18/12/97

				/* defines for sv_cmd		*/
#define ASISV_CMD_GET 0
#define ASISV_CMD_SET 1
				/* defines for sv_field		*/
#define ASISV_gl_DrawPicHadCopyBits 1
#define ASISV_gl_LayerNextDialogAsDocument 2
#define ASISV_SM_CXVSCROLL 3
#define ASISV_SM_CYHSCROLL 4
#define ASISV_gl_EventUpdateCheckCnt 5
#define ASISV_gl_PeekMessageYieldCnt 6
#define ASISV_gl_NoScrollRectEraseRgn 7
#define ASISV_gl_MacFHForFontMapAndRsrcInEXE 8
#define ASISV_gl_LSeekForFontMapInEXE 9
#define ASISV_gl_LSeekForSystemRsrcInEXE 10
#define ASISV_gl_LSeekForAppRsrcInEXE 11
#define ASISV_gl_AlertProcID 12
#define ASISV_gl_HasAppleMenu 13
#define ASISV_gl_256ColorPaletteStatus 14
#define ASISV_gl_MemSubMax 15
#define ASISV_gl_EraseDuringTextRedraw 16
#define ASISV_gl_NoSetSysColors 17
#define ASISV_gl_UseASIDForInitialFocus 18
#define	ASISV_gl_OnePixelPatternLines 19
#define	ASISV_gl_OpenResFilesReadOnly 20
#define ASISV_gl_DoesPartialGetSetDIBits 21		/* read-only */
#define ASISV_gl_CurrentHPALETTE 22			/* read-only */
#define ASISV_gl_DisableClipping 23
#define ASISV_gl_MapUpperChars 24
//#define ASISV_gl_MapUpperTEOutput 25
#define ASISV_gl_AccurateColor2Index 26
#define ASISV_gl_ModalDialogActive 27          /* read-only */
#define ASISV_gl_ExternalMouseEventsInModalDialog 28
#define ASISV_gl_EnableColorUse 29
#define ASISV_gl_OrigCntlValue 30
//#define ASISV_gl_NoCloseCommand 31	// replaced by below
#define	ASISV_gl_WindowCreateFlags 31

		// Does the window act like a palette (layers above doc windows)
	#define	WCF_ACTSLIKEPALETTE_NO		0x0001
	#define	WCF_ACTSLIKEPALETTE_YES		0x0002
		// Does the window look like a palette (small title bar)
	#define	WCF_LOOKSLIKEPALETTE_NO		0x0004
	#define	WCF_LOOKSLIKEPALETTE_YES	0x0008
		// Layer the next dialog as a regular document window
	#define	WCF_LAYERASDOCUMENT		0x0010
		// Layer the next window as if it were a dialog
	#define	WCF_LAYERASDIALOG 		0x0020
		// Use the bounds rect as App-relative (the default)
	#define	WCF_APPMAINRELATIVE		0x0040
		// Use the bounds rect as truly screen relative regardless
		//   of the position of the App's main window
	#define	WCF_SCREENRELATIVE		0x0080
		// Remove the Close command from the system menu
	#define	WCF_NOCLOSECOMMAND		0x0100
		// Create the window in maximized state
	#define	WCF_MAXIMIZE			0x0200
#define ASISV_gl_LayerNextDocumentAsDialog 32
#define	ASISV_gl_WindowProcID 33
#define	ASISV_gl_WindowMenuHandle 34
#define ASISV_gl_AddStarDotStarToFilters 35
#define	ASISV_gl_DefaultType 36
#define ASISV_gl_DoSimpleCopyMask 37
#define ASISV_gl_MacLike_StdState 38
#define	ASISV_gl_CreateWindowHandler 39

#define ASISV_gl_UseComboboxControl 40
#ifdef WING_CAPABLE
#define ASISV_gl_CreateWingDib 41
#endif
#define ASISV_GetIMEStringSize 42
#define ASISV_FlushIME 43
#define ASISV_gl_KeepDCInPrintDefault 44
#define ASISV_gl_MapExtendedToStrikeOut 45
#define ASISV_gl_ScaleScreenFonts 46
#define	ASISV_gl_CalcAscentLeadingFlags 47
	// how do we build ascent and descent
	#define CALF_ASCENT_EQUALS_PLUS_TMINTERNALLEADING 0x0001
	#define CALF_ASCENT_EQUALS_PLUS_TMEXTERNALLEADING 0x0002
	#define CALF_ASCENT_EQUALS_MINUS_TMINTERNALLEADING 0x0004
	#define CALF_ASCENT_EQUALS_MINUS_TMEXTERNALLEADING 0x0008

	#define CALF_LEADING_EQUALS_PLUS_TMINTERNALLEADING 0x0010
	#define CALF_LEADING_EQUALS_PLUS_TMEXTERNALLEADING 0x0020
	#define CALF_LEADING_EQUALS_MINUS_TMINTERNALLEADING 0x0040
	#define CALF_LEADING_EQUALS_MINUS_TMEXTERNALLEADING 0x0080

	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_PLUS_TMINTERNALLEADING 0x0100
	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_PLUS_TMEXTERNALLEADING 0x0200
	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_MINUS_TMINTERNALLEADING 0x0400
	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_MINUS_TMEXTERNALLEADING 0x0800

	#define CALF_EXTTEXTOUT_LEADING_EQUALS_PLUS_TMINTERNALLEADING 0x1000
	#define CALF_EXTTEXTOUT_LEADING_EQUALS_PLUS_TMEXTERNALLEADING 0x2000
	#define CALF_EXTTEXTOUT_LEADING_EQUALS_MINUS_TMINTERNALLEADING 0x4000
	#define CALF_EXTTEXTOUT_LEADING_EQUALS_MINUS_TMEXTERNALLEADING 0x8000
#define	ASISV_gl_MDIClassNameOverride 48
#define ASISV_gl_PopUpMenuCallBack 49
#define ASISV_gl_MemFreeBias 50
#define ASISV_gl_UseEnhancedMenuShortCuts 51
#define ASISV_gl_DrawPicUseStretchText 52
#define ASISV_gl_Force3D 53
#define ASISV_gl_HiliteScrollbars 54
#define ASISV_gl_DontClipTextEdit 55
#define ASISV_gl_HasSetDIBitsToDevice 56
#define ASISV_gl_PostScript_Dont_Erase 57
#define ASISV_gl_M2W 58
#define ASISV_gl_SetNextNewWindowStyles 59

EXTERN F_ASIBND( long )		ASI_SystemValue( short sv_cmd,
					short sv_field, long sv_value );

//QDBITS.c
EXTERN F_ASIBND( BOOLEAN )      ASI_QueryNewPalette( HWND hWnd );
EXTERN F_ASIBND( VOID ) ASI_PaletteChanged( HWND hWnd, HWND chgWnd );

//QDMACDIB.c
EXTERN F_ASIBND( VOID )	    ASI_CopyBitsMacToDIB( wPixMapPtr lpWPMSrc,
				wPixMapPtr lpWPMDst,
				Rect sr, Rect dr, SWORD Mode,
				CTabHandle hCTSrc, CTabHandle hCTDst,
				RgnHandle hRgnMask );

//QDDIBOUT.c
EXTERN F_ASIBND( VOID )	    ASI_DSSimple( wPixMapPtr lpWPMSrc,
				Rect sr, Rect dr,
				CTabHandle hCTSrc );
EXTERN F_ASIBND( VOID )	    ASI_DSNotSimple( wPixMapPtr lpWPMSrc,
				wPixMapPtr lpWPMDst,
				Rect sr, Rect dr, SWORD Mode,
				CTabHandle hCTSrc, CTabHandle hCTDst,
				wRgnHandle hRgnMask );
EXTERN F_ASIBND( VOID ) ASI_NextWriteIsToScreen ();
EXTERN F_ASIBND ( VOID ) ASI_WinGBitBlt( HDC hdcDest, int nXOriginDest,
        int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc,
        int nXOriginSrc, int nYOriginSrc );
EXTERN F_ASIBND ( BOOL ) ASI_LoadWinG ();

//QDMASK.c
EXTERN F_ASIBND( Handle )   ASI_CreateMask( BitMap *bmSrc,
				BOOLEAN fFlood, BOOLEAN fRetRgn );

//QDPoly.c
#ifdef _INC_WINDOWS
F_ASIBND( BOOL ) ASI_PolyPolygon( HDC, LPPOINT, LPINT, int);
#endif

//QDBRUSH.c
	F_ASIBND( VOID )
ASI_BrushAlign( SWORD wantAlign );

// QDPIC.c
EXTERN F_ASIBND( void )	ASI_AddEmfToCurrentPict( void* emf,Rect* rr ); // from ev 24/09/97


//WINDUTIL.c
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( LONG ) ASI_NCMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( LONG ) ASI_WindChange( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( LRESULT )      ASI_MDIMessage( UINT msg, WPARAM wParam, LPARAM lParam );

EXTERN F_ASIBND( LONG ) ASI_MDIDefaultState( BOOLEAN setIt, LONG lNewMDIStyle,
			MDICREATESTRUCT *mcs );

EXTERN F_ASIBND( VOID ) ASI_GetCurHDCs( HDC *curHdc, HDC *curPrHdc, 
							SWORD *isPSPrinter  );
#endif

//RESOURCE.c
EXTERN F_ASIBND( VOID ) ASI_CopyPtoP( BYTE const *pstrsrc, BYTE *pstrdst );
EXTERN F_ASIBND( char * ) ASI_CopyPtoC( BYTE const *pstr, BYTE *pcstr );
EXTERN F_ASIBND( unsigned char * ) ASI_CopyCtoP( BYTE const *pcstr, BYTE *pstr );
#ifdef _INC_WINDOWS
EXTERN int LoadStringEx(
    HINSTANCE  hInstance,	// handle of module containing string resource 
    UINT  uID,	// resource identifier 
    LPTSTR  lpBuffer,	// address of buffer for resource 
    int  nBufferMax 	// size of buffer 
   );
#endif

#define CODE_RESOURCE_UNREGISTER 0
#define CODE_RESOURCE_REGISTER 1
#define CODE_RESOURCE_INIT 2
#define CODE_RESOURCE_FIND 3
#define CODE_RESOURCE_EXIT 4
#define CODE_RESOURCE_EXISTS 5
EXTERN F_ASIBND( ProcPtr ) ASI_CodeResource( UWORD cmd, OSType DefType, 
				UWORD resID, ProcPtr theFP );

EXTERN F_ASIBND( ProcPtr ) ASI_SetSwapResourceProc(SWORD fh,ProcPtr swapProc);

//LISTMGR.c
#define LSETSEL_MASK		0x00FF
#define LSETSEL_ACTIVECLIKLOOP	0x1000
#define LSETSEL_MULTICOL	0x0800
#define LSETSEL_REAL2DLIST	0x0400
#define LSETSEL_NOKBDINPUT	0x0200
#define LSETSEL_NOSELECTION	0x0100
EXTERN F_ASIBND( VOID ) ASI_LSetSelFlags( UWORD selFlags );

#define LTEAROFF_INIT		0x0001
#define LTEAROFF_TEAROFFITEM	0x0002
#define LTEAROFF_CMD_MASK	0x000F
#define LTEAROFF_FLAG_AUTOSCROLL 0x8000
#define LTEAROFF_FLAG_DONTDRAGRECT 0x4000
EXTERN F_ASIBND( BOOLEAN )      ASI_LTearOff( ListHandle lHandle, short cmd, Point *pPt );

#if WINVER
typedef BOOLEAN (CALLBACK *TearOffHookProcPtr)( Point *pLocMouse,
		Cell *lCell, Rect *prExclude, SWORD dragW, SWORD dragH );

EXTERN	F_ASIBND( VOID ) ASI_LSetTearOffDragHook( ListHandle lh, 
					TearOffHookProcPtr dragHook );
#endif
F_ASIBND( HWND )		ASI_GetHWNDFromListHandle( ListHandle lh );
F_ASIBND( VOID )		ASI_ListUpdateVisible( ListPtr pL );

//QDFONT.c
EXTERN  F_ASIBND( VOID )        ASI_UpdateFontList( BOOLEAN forceReEnumerate );

//QDTEXT.c
EXTERN  F_ASIBND( VOID )        ASI_ExtDrawText( UWORD bUse, WidthTable *lpWT, 
							UWORD *lpMacDX);

EXTERN  F_ASIBND( VOID )        ASI_USER_WinTextOutWidths( 
				   UINT *lpUserPositioning, short numWidths );

//WINDUPDT.c
EXTERN F_ASIBND( VOID ) ASI_UpdateWindows( SWORD doit );

//QDDEVBIT.c
EXTERN F_ASIBND( PixMapHandle ) ASI_NewDDBPixMap( long lbm, long lpal );

//QDDIB.c
EXTERN F_ASIBND( PixMapHandle ) ASI_NewDIBPixMap( void * lpdib );

//OFFSCRN.c
EXTERN F_ASIBND( BOOLEAN )      ASI_BeginOffScreen( BOOLEAN bErase, Rect *pr );
EXTERN F_ASIBND( VOID ) ASI_EndOffScreen( BOOLEAN bDraw, BOOLEAN bExcludeUpdateRgn, SWORD blitMode );
EXTERN F_ASIBND( BOOLEAN )      ASI_OffScreenRotate( PixMapHandle hPM, SWORD Angle );
EXTERN F_ASIBND( BOOLEAN )      ASI_OffScreenFlip( PixMapHandle hPM, BOOLEAN bHorzFlip );
EXTERN F_ASIBND( Handle )       ASI_PackPixMap( PixMapHandle hPM );
EXTERN F_ASIBND( PixMapHandle ) ASI_UnpackPixMap( Handle hPack );
EXTERN F_ASIBND( BOOLEAN )      ASI_ExcludeUpdateRgn( WindowPeek wPtr, 
				  BOOLEAN excludeIt, RgnHandle *pOldVis );

#define OSDC_ALLOCATE_DC 0x0000		// allocate our own compatible DC
#define OSDC_SHARE_MEMDC 0x0001		// share the global Memory DC
#define OSDC_USE_DIBDC 0x0002		// allocate DIB driver based DC
EXTERN F_ASIBND( HDC )		ASI_OffScreenDC( PixMapHandle hPixMap, 
					UWORD shareFlag );

//QDDIBBIT.c
// Bit flags for DIB to PixMap Conversion process, used to set UnfoldFlag.

#define UNFOLD_FreeHBM 0x0001
#define UNFOLD_BasedOnDestPixMap 0x0002
#define UNFOLD_BasedOnSourceBitMap 0x0004

#define UNFOLD_NoBaseAddrReAlloc 0x0008	// fail if baseAddr not big enough
#define UNFOLD_UsePalColors	   0x0010	// RB: use identity palette
#define UNFOLD_InvertPalColors     0x0020 // RB: invert palette index
#define UNFOLD_ReleaseHBM 	 0x0040	  // RB: zero out HBM, but don't free

#define UNFOLD_Force1Bit 0x0100	// GetDIBits() for monochrome
#define UNFOLD_Force4Bit 0x0200	// GetDIBits() for 16 colors
#define UNFOLD_Force8Bit 0x0400	// GetDIBits() for 256 colors
#define UNFOLD_Force32Bit 0x0800	// GetDIBits() for RGB colors

#define UNFOLD_Pack32in24 0x1000	// PackType == 2
#define UNFOLD_Pack32byRLE 0x2000	// PackType == 4, not supported yet
#define UNFOLD_Pack16byRLE 0x4000	// PackType == 3, not supported yet

EXTERN F_ASIBND( BOOLEAN )	ASI_UnfoldPixMap( BitMapPtr bmSrc, 
							BitMapPtr bmDst,
							Rect *prSrc,
							Rect *prDst,
							UWORD UnfoldFlag );


#define FOLD_PutInDIB 0x0002
#define FOLD_IgnoreMacBaseAddr 0x0004
#define FOLD_PutInDDB 0x0008
#define FOLD_UpdateDIBRGBFromCTab 0x0010
#define FOLD_4BitAs4Bit 0x0020
EXTERN F_ASIBND( BOOLEAN )	ASI_FoldPixMap( BitMapPtr bmDst, 
							Rect *prDst,
							UWORD FoldFlag );

EXTERN F_ASIBND( BOOLEAN )	ASI_GetPixMapInfo( BitMapPtr bmSrc );
   
EXTERN F_ASIBND( HBITMAP )	ASI_CopyBM( HBITMAP hBMSrc );

EXTERN F_ASIBND( LPBITMAPINFO )	ASI_GetDIBFromPixMap( PixMapHandle hPM,
						      Ptr far *lplpBits );
EXTERN F_ASIBND( VOID         )	ASI_GetHdcFromPixMap( PixMapHandle hPM,
			      HANDLE far* phDC, BOOLEAN far* pIsWing );

//QDHILITE.c
EXTERN F_ASIBND( VOID ) ASI_HiliteNotSectRect( Rect r1, Rect r2, BOOLEAN bUseHilite );

//DLGKEYHK.c
EXTERN F_ASIBND( WindowPtr )    ASI_FocusWindow(void);

EXTERN F_ASIBND( HWND )	ASI_KeyHook( HWND hHookWnd, BOOLEAN hook_it );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( SWORD )        ASI_DlgKeyHook(SWORD iCode, WPARAM wParam, LPARAM lParam);
#endif

#define WNDTYPE_UNKNOWN -1
#define WNDTYPE_EDIT 1
#define WNDTYPE_SCROLLBAR 1
#define WNDTYPE_BUTTON 2
#define WNDTYPE_USERITEM 3
#define WNDTYPE_CDEF 4
#define WNDTYPE_STATIC 5
#define WNDTYPE_LISTBOX 6
#define WNDTYPE_COMBOBOX 7
typedef F_PASCAL_PTR( VOID, *DlgKBHookProcPtr )( WindowPtr wPtr, 
				SWORD itemID, SWORD itemWndType,
				UWORD resv, EventRecord *pEvent,
      				BOOLEAN *pPassToDialog, 
				BOOLEAN *pPassToControl );
EXTERN	F_ASIBND( VOID )	ASI_SetDlgKBHook( DlgKBHookProcPtr lpfnDlgKBHook );


EXTERN F_ASIBND( VOID ) ASI_AddASIDItems( DialogPeek dPtr, SWORD ResId );
#if WINVER
EXTERN F_ASIBND( ASI_DITLHandle ) ASI_UseASIDItems( DialogPeek dPtr, 
								SWORD ResId );
#endif
EXTERN F_ASIBND( HWND ) ASI_GethWnd( WindowPeek wPtr );
EXTERN F_ASIBND( BOOLEAN )      ASI_GetwPtr( HWND hSubWnd, WindowPeek *pwPtr );
EXTERN F_ASIBND( VOID ) ASI_EndSession( BOOLEAN InEndSession );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_EnableHelpHook(FARPROC helpProc);
EXTERN F_ASIBND( VOID ) ASI_DisableHelpHook(void);
#endif
 
//TEXTEDIT.c
EXTERN F_ASIBND( BOOLEAN )     ASI_SethTEfromhWnd( TEHandle hTE, BOOLEAN ForceChange );
EXTERN F_ASIBND( VOID ) 		ASI_SetTE3D(TEHandle hTE, Boolean on);
EXTERN F_ASIBND( Boolean ) 	ASI_GetTE3D(TEHandle hTE);
#if PPC_HEADERS
#define	ClikLoopProcPtr		TEClickLoopUPP
#endif
EXTERN F_PASCAL( void )		ASI_MakePIClickLoop( ClikLoopProcPtr *clikProc );

typedef void (pascal *draw_text_proc_ptr)( TEHandle hTE, 
		const void *textBuf, short firstByte, short byteCount );
typedef short (pascal *text_width_proc_ptr)(TEHandle hTE, 
		const void *textBuf, short firstByte, short byteCount );
typedef void (pascal *qdrect_proc_ptr)(GrafVerb gv,	const Rect *r_ptr);

EXTERN	F_ASIBND( void ) ASI_TECustomHooks(TEHandle hTE, 
				draw_text_proc_ptr AppDrawText, 
				text_width_proc_ptr AppTextWidth,
				qdrect_proc_ptr AppQDRectProc );

//DIALOGM.c
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( BOOLEAN )      ASI_UserItem( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
#endif

EXTERN F_ASIBND( VOID ) ASI_MaintainTEFocus( TEHandle hTE );
EXTERN F_ASIBND( VOID ) ASI_SetDlgTextFont( DialogPeek dPtr, short itemNo );
EXTERN F_ASIBND( VOID ) ASI_AddDitlItems( DialogPeek dPtr, short OrigCnt );
EXTERN F_ASIBND( VOID ) ASI_DisposeDlgItem( DialogPeek dPtr, short itemNo );
EXTERN F_ASIBND( VOID ) ASI_BuildComboBox( DialogPeek dPtr, SWORD dItemNo, MenuHandle hMenu, SWORD mDefItemNo );
EXTERN F_ASIBND( SWORD )        ASI_GetComboBoxSelect( DialogPeek dPtr, SWORD dItemNo );
EXTERN F_ASIBND( BOOLEAN )      ASI_SetComboBoxSelect( DialogPeek dPtr, SWORD dItemNo, SWORD cItemNo );
EXTERN F_ASIBND( VOID ) ASI_ScaleDlgItemRect( BOOLEAN ScaleUp, Rect *pR );
EXTERN F_ASIBND( VOID ) ASI_OffsetDlgItemRect( BOOLEAN ScaleUp, Rect *pBounds );
EXTERN F_ASIBND( VOID ) ASI_ScaleDlgWindow( BOOLEAN ScaleUp, Rect *pBounds );
EXTERN F_ASIBND( VOID ) ASI_ScaleDlgPoint( BOOLEAN ScaleUp, Point *pPt );
EXTERN F_ASIBND( HWND ) ASI_EnableWindows( WindowPeek wPtr, BOOLEAN Enable, 
			BOOLEAN IncludeThisWin );
EXTERN F_ASIBND( HWND ) ASI_EnableWindowsEx( WindowPeek wPtr, BOOLEAN Enable, 
			BOOLEAN IncludeThisWin, BOOLEAN IncludeMainWin );
EXTERN F_ASIBND( VOID ) ASI_UpdateParamStr(DialogPtr dPtr, short paramNum);
EXTERN F_ASIBND( VOID ) ASI_SetBigIText( Handle item, Handle stringH, short stringLen );
EXTERN F_ASIBND( BOOLEAN ) ASI_IsComboBox( DialogPeek dPtr, SWORD dItemNo );
EXTERN F_ASIBND( VOID ) ASI_SetDefaultButtonAndFocus( DialogPeek dPtr, 
					SWORD item, BOOLEAN permanent );
EXTERN F_ASIBND( int ) ASI_ActivateIME( HWND hWnd, Boolean visible_flag );

//MEMORYM.c
EXTERN F_ASIBND( UWORD ) ASI_GetFreeSystemResources( UWORD fuSysResource );
EXTERN F_ASIBND( VOID )	 ASI_SetMemoryLimits( Boolean bEnabled, 
							LONG lMaxMemoryUse, LONG lMinTrackSize, SWORD sMaxSelectors );
	F_ASIBND( VOID )
ASI_ComputeMemorySize( DWORD * virtualTotal, DWORD * virtualFree,
						DWORD * physicalTotal, DWORD * physicalFree,
						DWORD * allocated );
EXTERN F_ASIBND( BOOLEAN )	ASI_MemCheck( void );
#if Compile4DLL
EXTERN VOID ASI_SetMemLimits (long heapSize, SWORD nbHeaps);
EXTERN VOID ASI_ATHCA (BOOLEAN newState);
EXTERN void ASI_CheckMacHeap (void);
#endif

//COLOR.c
EXTERN F_ASIBND( void )	ASI_SetPixBM( PixMapHandle, HBITMAP );
EXTERN F_ASIBND( void )	ASI_ForcePalette( VOID );
EXTERN F_ASIBND( DWORD ) ASI_GetPixMapSize( PixMapHandle hPix );
EXTERN F_ASIBND( void ) ASI_GetWindowsPalette( WindowPtr theWindow, 
					long* hPal );	/* DLB 941111 */

//ASIUTILC.c
EXTERN F_ASIBND( VOID ) ASI_BeginPaletteAnimation( VOID );
EXTERN F_ASIBND( VOID ) ASI_EndPaletteAnimation( VOID );
EXTERN F_ASIBND( LONG ) ASI_GetByteSwapLONG( BYTE *lpb );
EXTERN F_ASIBND( UWORD )        ASI_GetByteSwapWORD( BYTE *lpb );
EXTERN F_ASIBND( VOID ) ASI_GetByteSwapRect( BYTE *lpb, Rect *pr );

//ASIWPROC.c
EXTERN F_ASIBND( VOID )	ASI_ModalDialog( ModalFilterProcPtr FilterProc,
				short *pItemHit );

//DLGPROCS.C
typedef BOOLEAN (CALLBACK *ButtonFocusHookProcPtr)( HWND hWnd, SWORD CtrlID);
EXTERN F_ASIBND( VOID ) ASI_SetButtonFocusHookProc( ButtonFocusHookProcPtr HookProc );

#ifdef _INC_WINDOWS
//EXTERN F_ASIBND( VOID ) ASI_ConvRtoR( const Rect *pr1, RECT *pr2 );
EXTERN F_ASIBND( VOID ) ASI_ConvMRtoWR( const Rect *pr1, RECT *pr2 );
EXTERN F_ASIBND( VOID ) ASI_ConvWRtoMR( const RECT *pR1, Rect *pr2 );
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#if MACVER
#define ASI_IsRectRgn( rgn ) ((**rgn).rgnSize == 10)
#define ASI_ClipRectChange( pPort ) /**/
#define ASI_ClipRgnChange( pPort ) /**/
#define ASI_VisRgnChange(pPort)
#else
#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus
	
#ifdef	_Quickdraw_
F_ASIBND( BOOLEAN )	ASI_IsRectRgn(RgnHandle rgn);
F_ASIBND( VOID )	ASI_ClipRectChange(GrafPtr pPort);
F_ASIBND( VOID )	ASI_ClipRgnChange(GrafPtr pPort);
F_ASIBND( VOID )	ASI_VisRgnChange(GrafPtr pPort);
#endif

#if !defined(_INC_WINDOWS)
#undef LPBITMAPINFO
#endif

#undef	wPixMapPtr
#undef	wRgnHandle

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// MACVER

#endif	// #ifndef __ASIEXTRN__
