// ---------------------------------------------------------------
//
// 4D Plugin API
//
// File : Flags.h
// Description : 
// define compiler directive for the two supported platforms : 
// Windows and Mac PPC (68k Mac are not supported)
//
// rev : 6.8.1
//
// ---------------------------------------------------------------

#ifndef __FLAGS__
#define __FLAGS__


// WIN32 is defined when compiling for Windows target
#ifdef WIN32
#define VERSIONWIN 1
#define VERSIONMAC 0
#define VERSIONDEBUG _DEBUG
#endif

// __POWERPC__ is defined when compiling for Macintosh target
#ifdef __POWERPC__
#define VERSIONWIN 0
#define VERSIONMAC 1
#define VERSIONDEBUG __option(sym)
#endif

#endif
