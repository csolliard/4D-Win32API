/*
	File:		Dialogs.h

	Copyright:	� 1984-1993 by Apple Computer, Inc., all rights reserved.

	WARNING
	This file was auto generated by the interfacer tool. Modifications
	must be made to the master file.

*/

#ifndef __DIALOGS__
#define __DIALOGS__

#ifndef __WINDOWS__
#include <WindowM.H>
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.H>
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif

enum  {
	ctrlItem			= 4,
	btnCtrl			= 0,
	chkCtrl			= 1,
	radCtrl			= 2,
	resCtrl			= 3,
	statText			= 8,
	editText			= 16,
	iconItem			= 32,
	picItem			= 64,
	userItem			= 0,
	itemDisable			= 128,
	ok			= 1,
	cancel			= 2,
	stopIcon			= 0,
	noteIcon			= 1,
	cautionIcon			= 2
};


/* new, more standard names for dialog item constants */
enum {
	kControlDialogItem			= ctrlItem,
	kButtonDialogItem			= ctrlItem + btnCtrl,
	kCheckBoxDialogItem			= ctrlItem + chkCtrl,
	kRadioButtonDialogItem		= ctrlItem + radCtrl,
	kResourceControlDialogItem	= ctrlItem + resCtrl,
	kStaticTextDialogItem		= statText,
	kEditTextDialogItem			= editText,
	kIconDialogItem				= iconItem,
	kPictureDialogItem			= picItem,
	kUserDialogItem				= userItem,
	kItemDisableBit				= itemDisable,
	kStdOkItemIndex				= ok,
	kStdCancelItemIndex			= cancel,
	kStopIcon					= stopIcon,
	kNoteIcon					= noteIcon,
	kCautionIcon				= cautionIcon
};

/*	Dialog Item List Manipulation Constants	*/

typedef short DITLMethod;

enum  {
	overlayDITL			= 0,
	appendDITLRight			= 1,
	appendDITLBottom			= 2
};

typedef short StageList;

typedef WindowPtr DialogPtr;

// EHC
#ifndef DialogRef
typedef DialogPtr DialogRef;
#endif

typedef void (pascal *SoundProcPtr)(short soundNumber);

enum {
	uppSoundProcInfo = kPascalStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
};

#if USESROUTINEDESCRIPTORS
typedef UniversalProcPtr SoundUPP;

#define CallSoundProc(userRoutine, soundNumber)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppSoundProcInfo, (soundNumber))
#define NewSoundProc(userRoutine)		\
		(SoundUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppSoundProcInfo, GetCurrentISA())
#else
typedef SoundProcPtr SoundUPP;

#define CallSoundProc(userRoutine, soundNumber)		\
		(*(userRoutine))((soundNumber))
#define NewSoundProc(userRoutine)		\
		(SoundUPP)(userRoutine)
#endif

typedef Boolean (pascal *ModalFilterProcPtr)(DialogPtr theDialog, EventRecord *theEvent, short *itemHit);

enum {
	uppModalFilterProcInfo = kPascalStackBased
		| RESULT_SIZE(SIZE_CODE(sizeof(Boolean)))
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(DialogPtr)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EventRecord*)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(short*)))
};

#if USESROUTINEDESCRIPTORS
typedef UniversalProcPtr ModalFilterUPP;

#define CallModalFilterProc(userRoutine, theDialog, theEvent, itemHit)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppModalFilterProcInfo, (theDialog), (theEvent), (itemHit))
#define NewModalFilterProc(userRoutine)		\
		(ModalFilterUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppModalFilterProcInfo, GetCurrentISA())
#else
typedef ModalFilterProcPtr ModalFilterUPP;

#define CallModalFilterProc(userRoutine, theDialog, theEvent, itemHit)		\
		(*(userRoutine))((theDialog), (theEvent), (itemHit))
#define NewModalFilterProc(userRoutine)		\
		(ModalFilterUPP)(userRoutine)
#endif

typedef void (pascal *UserItemProcPtr)(WindowPtr theWindow, short itemNo);

enum {
	uppUserItemProcInfo = kPascalStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(WindowPtr)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
};

#if USESROUTINEDESCRIPTORS
typedef UniversalProcPtr UserItemUPP;

#define CallUserItemProc(userRoutine, theWindow, itemNo)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppUserItemProcInfo, (theWindow), (itemNo))
#define NewUserItemProc(userRoutine)		\
		(UserItemUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppUserItemProcInfo, GetCurrentISA())
#else
typedef UserItemProcPtr UserItemUPP;

#define CallUserItemProc(userRoutine, theWindow, itemNo)		\
		(*(userRoutine))((theWindow), (itemNo))
#define NewUserItemProc(userRoutine)		\
		(UserItemUPP)(userRoutine)
#endif

#pragma PRAGMA_PACK_1

struct DialogRecord {
	WindowRecord			window;
	Handle			items;
	TEHandle			textH;
	short			editField;
	short			editOpen;
	short			aDefItem;
#if WINVER
	Handle itemsColors; // ictb for this dialog
#endif
};
typedef struct DialogRecord DialogRecord;

typedef DialogRecord *DialogPeek;

struct DialogTemplate {
	Rect			boundsRect;
	short			procID;
	Boolean			visible;
	Boolean			filler1;
	Boolean			goAwayFlag;
	Boolean			filler2;
	long			refCon;
	short			itemsID;
	Str255			title;
};
typedef struct DialogTemplate DialogTemplate;

typedef DialogTemplate *DialogTPtr, **DialogTHndl;

struct AlertTemplate {
	Rect		boundsRect;
	short		itemsID;
#if MACVER
    StageList	stages;
#else
	//lve otherwise AlertTemplate is 14 bytes rather than 12 
	// because of the implicit int base type on Win32
	// we were lucky with the VC++ layout, it worked, but it breaks the
	// new system 7 style template def which has an extra short after the bitfields
	BYTE	boldItm4	: 1;
	BYTE	boxDrwn4	: 1;
	BYTE	sound4		: 2;	
	BYTE	boldItm3	: 1;
	BYTE	boxDrwn3	: 1;
	BYTE	sound3		: 2;	
	BYTE	boldItm2	: 1;
	BYTE	boxDrwn2	: 1;
	BYTE	sound2		: 2;	
	BYTE	boldItm1	: 1;
	BYTE	boxDrwn1	: 1;
	BYTE	sound1		: 2;
#endif
};
typedef struct AlertTemplate AlertTemplate;

#if WINVER
// lve system 7 style ALRT
struct ExtendedAlertTemplate {
	AlertTemplate oldTemplate;
	short positionning;
};
typedef struct ExtendedAlertTemplate ExtendedAlertTemplate;

#endif

typedef AlertTemplate *AlertTPtr, **AlertTHndl;

#pragma PRAGMA_PACK_RESET

#ifdef __cplusplus
extern "C" {
#endif

/*
	NOTE: Code running under MultiFinder or System 7.0 or newer
	should always pass NULL to InitDialogs.
*/

extern void pascal InitDialogs(long ignored);
extern void pascal ErrorSound(SoundUPP soundProc);
extern DialogPtr pascal NewDialog(void *wStorage, const Rect *boundsRect, ConstStr255Param title, Boolean visible, short procID, WindowPtr behind, Boolean goAwayFlag, long refCon, Handle itmLstHndl);
extern DialogPtr newdialog(void *wStorage, const Rect *boundsRect, char *title, Boolean visible, short procID, WindowPtr behind, Boolean goAwayFlag, long refCon, Handle itmLstHndl);
extern DialogPtr pascal GetNewDialog(short dialogID, void *dStorage, WindowPtr behind);
extern void pascal CloseDialog(DialogPtr theDialog);
extern void pascal DisposeDialog(DialogPtr theDialog);
extern void pascal ParamText(ConstStr255Param param0, ConstStr255Param param1, ConstStr255Param param2, ConstStr255Param param3);
extern void pascal ModalDialog(ModalFilterUPP modalFilter, short *itemHit);
extern Boolean pascal IsDialogEvent(const EventRecord *theEvent);
extern Boolean pascal DialogSelect(const EventRecord *theEvent, DialogPtr *theDialog, short *itemHit);
extern void pascal DrawDialog(DialogPtr theDialog);
extern void pascal UpdateDialog(DialogPtr theDialog, RgnHandle updateRgn);
extern short pascal Alert(short alertID, ModalFilterUPP modalFilter);
extern short pascal StopAlert(short alertID, ModalFilterUPP modalFilter);
extern short pascal NoteAlert(short alertID, ModalFilterUPP modalFilter);
extern short pascal CautionAlert(short alertID, ModalFilterUPP modalFilter);
extern void pascal GetDialogItem(DialogPtr theDialog, short itemNo, short *itemType, Handle *item, Rect *box);
extern void pascal SetDialogItem(DialogPtr theDialog, short itemNo, short itemType, Handle item, const Rect *box);
extern void pascal HideDialogItem(DialogPtr theDialog, short itemNo);
extern void pascal ShowDialogItem(DialogPtr theDialog, short itemNo);
extern void pascal SelectDialogItemText(DialogPtr theDialog, short itemNo, short strtSel, short endSel);
extern void pascal GetDialogItemText(Handle item, Str255 text);
extern void pascal SetDialogItemText(Handle item, ConstStr255Param text);
extern short pascal FindDialogItem(DialogPtr theDialog, Point thePt);
extern DialogPtr pascal NewColorDialog(void *dStorage, const Rect *boundsRect, ConstStr255Param title, Boolean visible, short procID, WindowPtr behind, Boolean goAwayFlag, long refCon, Handle items);
extern DialogPtr newcolordialog(void *dStorage, const Rect *boundsRect, char *title, Boolean visible, short procID, WindowPtr behind, Boolean goAwayFlag, long refCon, Handle items);
#if USESCODEFRAGMENTS
extern short pascal GetAlertStage(void);
#else
#endif

extern void pascal ResetAlertStage(void);
extern void pascal DialogCut(DialogPtr theDialog);
extern void pascal DialogPaste(DialogPtr theDialog);
extern void pascal DialogCopy(DialogPtr theDialog);
extern void pascal DialogDelete(DialogPtr theDialog);
extern void pascal SetDialogFont(short fontNum);
extern void paramtext(char *param0, char *param1, char *param2, char *param3);
extern void getdialogitemtext(Handle item, char *text);
extern void setdialogitemtext(Handle item, char *text);
extern short finddialogitem(DialogPtr theDialog, Point *thePt);
extern void pascal AppendDITL(DialogPtr theDialog, Handle theHandle, DITLMethod method);
extern short pascal CountDITL(DialogPtr theDialog);
extern void pascal ShortenDITL(DialogPtr theDialog, short numberItems);
extern Boolean pascal StdFilterProc(DialogPtr theDialog, EventRecord *event, short *itemHit);
extern OSErr pascal GetStdFilterProc(ModalFilterUPP *theProc);
extern OSErr pascal SetDialogDefaultItem(DialogPtr theDialog, short newItem);
extern OSErr pascal SetDialogCancelItem(DialogPtr theDialog, short newItem);
extern OSErr pascal SetDialogTracksCursor(DialogPtr theDialog, Boolean tracks);
#if OLDROUTINENAMES
#define DisposDialog(theDialog) DisposeDialog(theDialog)

#define UpdtDialog(theDialog, updateRgn) UpdateDialog(theDialog, updateRgn)

#define GetDItem(theDialog, itemNo, itemType, item, box)  \
	GetDialogItem(theDialog, itemNo, itemType, item, box)

#define SetDItem(theDialog, itemNo, itemType, item, box)  \
	SetDialogItem(theDialog, itemNo, itemType, item, box)

#define HideDItem(theDialog, itemNo) HideDialogItem(theDialog, itemNo)

#define ShowDItem(theDialog, itemNo) ShowDialogItem(theDialog, itemNo)

#define SelIText(theDialog, itemNo, strtSel, endSel)  \
	SelectDialogItemText(theDialog, itemNo, strtSel, endSel)

#define GetIText(item, text) GetDialogItemText(item, text)

#define SetIText(item, text) SetDialogItemText(item, text)

#define FindDItem(theDialog, thePt) FindDialogItem(theDialog, thePt)

#define NewCDialog(dStorage, boundsRect, title, visible, procID, behind, goAwayFlag, refCon, items)  \
	NewColorDialog(dStorage, boundsRect, title, visible, procID, behind,  \
	goAwayFlag, refCon, items)

// loic
#define GetAlrtStage GetAlertStage

#define ResetAlrtStage ResetAlertStage

#define DlgCut(theDialog) DialogCut(theDialog)

#define DlgPaste(theDialog) DialogPaste(theDialog)

#define DlgCopy(theDialog) DialogCopy(theDialog)

#define DlgDelete(theDialog) DialogDelete(theDialog)

#define SetDAFont(fontNum) SetDialogFont(fontNum)

#define newcdialog(dStorage, boundsRect, title, visible, procID, behind, goAwayFlag, refCon, items)  \
	newcolordialog(dStorage, boundsRect, title, visible, procID, behind,  \
	goAwayFlag, refCon, items)

#define getitext(item, text) getdialogitemtext(item, text)

#define setitext(item, text) setdialogitemtext(item, text)

#define findditem(theDialog, thePt) finddialogitem(theDialog, thePt)

#endif

//--------------------

short pascal GetDialogDefaultItem(DialogPtr theDialog);
short pascal GetDialogCancelItem(DialogPtr theDialog);

void  pascal SetGrafPortOfDialog(DialogRef dialog);

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif
#ifdef __cplusplus
}
#endif

#endif


