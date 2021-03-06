/*
	File:		Finder.h

	Copyright:	� 1984-1993 by Apple Computer, Inc., all rights reserved.

	WARNING
	This file was auto generated by the interfacer tool. Modifications
	must be made to the master file.

*/

#ifndef __FINDER__
#define __FINDER__


/* make only the following consts avaiable to resource files that include this file */

#define kCustomIconResource -16455

#define kContainerFolderAliasType OST_fdrp

#define kContainerTrashAliasType OST_trsh

#define kContainerHardDiskAliasType OST_hdsk

#define kContainerFloppyAliasType OST_flpy

#define kContainerServerAliasType OST_srvr

#define kApplicationAliasType OST_adrp

#define kContainerAliasType OST_drop


/* type for Special folder aliases */

#define kSystemFolderAliasType OST_fasy

#define kAppleMenuFolderAliasType OST_faam

#define kStartupFolderAliasType OST_fast

#define kPrintMonitorDocsFolderAliasType OST_fapn

#define kPreferencesFolderAliasType OST_fapf

#define kControlPanelFolderAliasType OST_fact

#define kExtensionFolderAliasType OST_faex


/* type for AppleShare folder aliases */

#define kExportedFolderAliasType OST_faet

#define kDropFolderAliasType OST_fadr

#define kSharedFolderAliasType OST_fash

#define kMountedFolderAliasType OST_famn


enum {
/* Finder Flags */
	kIsOnDesk					= 0x1,
	kColor						= 0xE,
	kIsShared					= 0x40,
	kHasBeenInited				= 0x100,
	kHasCustomIcon				= 0x400,
	kIsStationery				= 0x800,
	kIsStationary				= 0x800,
	kNameLocked					= 0x1000,
	kHasBundle					= 0x2000,
	kIsInvisible				= 0x4000,
	kIsAlias					= 0x8000
};


#endif


