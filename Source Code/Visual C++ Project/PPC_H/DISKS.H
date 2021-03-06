/*
	File:		Disks.h

	Copyright:	� 1984-1993 by Apple Computer, Inc., all rights reserved.

	WARNING
	This file was auto generated by the interfacer tool. Modifications
	must be made to the master file.

*/

#ifndef __DISKS__
#define __DISKS__

#ifndef __TYPES__
#include <MacTypes.H>
#endif

#ifndef __OSUTILS__
#include <OSUtils.H>
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
struct DrvSts {
	short			track;			/*current track*/
	char			writeProt;			/*bit 7 = 1 if volume is locked*/
	char			diskInPlace;			/*disk in drive*/
	char			installed;			/*drive installed*/
	char			sides;			/*-1 for 2-sided, 0 for 1-sided*/
	QElemPtr			qLink;			/*next queue entry*/
	short			qType;			/*1 for HD20*/
	short			dQDrive;			/*drive number*/
	short			dQRefNum;			/*driver reference number*/
	short			dQFSID;			/*file system ID*/
	char			twoSideFmt;			/*after 1st rd/wrt: 0=1 side, -1=2 side*/
	char			needsFlush;			/*-1 for MacPlus drive*/
	short			diskErrs;			/*soft error count*/
};
typedef struct DrvSts DrvSts;

struct DrvSts2 {
	short			track;
	char			writeProt;
	char			diskInPlace;
	char			installed;
	char			sides;
	QElemPtr			qLink;
	short			qType;
	short			dQDrive;
	short			dQRefNum;
	short			dQFSID;
	short			driveSize;
	short			driveS1;
	short			driveType;
	short			driveManf;
	short			driveChar;
	char			driveMisc;
};
typedef struct DrvSts2 DrvSts2;

#ifdef __cplusplus
extern "C" {
#endif

extern OSErr pascal DiskEject(short drvNum);
extern OSErr pascal SetTagBuffer(void *buffPtr);
extern OSErr pascal DriveStatus(short drvNum, DrvSts *status);
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


