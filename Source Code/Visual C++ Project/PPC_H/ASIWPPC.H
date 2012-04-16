// ASIWPPC.H

#ifndef _ASIWPPC_H
#define _ASIWPPC_H


#if PPC_HEADERS

#ifdef __cplusplus
extern "C" {
#endif

#define LMGetGrayRgn()		GetGrayRgn()

#define LMGetUTableBase()	UTableBase
#define LMGetCurStackBase()	CurStackBase
#define LMGetCurApRefNum()	CurApRefNum
#define LMGetKeyThresh()	KeyThresh
#define LMGetCurJTOffset()	CurJTOffset
#define LMGetWindowList()	WindowList
#define LMGetGhostWindow()  GhostWindow
#define LMGetResLoad()		ResLoad
#define LMGetHiliteMode()	HiliteMode
#define LMGetCurApName()	CurApName
#define LMGetFSFCBLen()		FSFCBLen
#define LMGetCrsrBusy()		CrsrBusy
#define LMGetHWCfgFlags()	HWCfgFlags
#define GetUnitEntryCount()	UnitNtryCnt

#define LMGetGZRootHnd()	GZRootHnd
#define LMGetGZMoveHnd()	GZMoveHnd

void LMSetROMMapInsert(unsigned char ROMMapInsertValue);
void LMSetResLoad(unsigned char ResLoadValue);
void LMSetHiliteRGB(RGBColor *HiliteRGBValue);


#ifdef __cplusplus
}
#endif

void LMSetROMMapInsert(unsigned char ROMMapInsertValue)
{
	RomMapInsert = ROMMapInsertValue;
}

void LMSetResLoad(unsigned char ResLoadValue)
{
	ResLoad = ResLoadValue;	
}

void LMSetHiliteRGB(RGBColor *HiliteRGBValue)
{
	HiliteRGB = HiliteRGBValue;	
}


#endif // PPC_HEADERS

#endif // _ASIWPPC_H

