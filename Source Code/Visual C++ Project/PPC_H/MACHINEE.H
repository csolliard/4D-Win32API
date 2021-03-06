/*
	File:		MachineExceptions.h

	Copyright:	� 1984-1993 by Apple Computer, Inc., all rights reserved.

	WARNING
	This file was auto generated by the interfacer tool. Modifications
	must be made to the master file.

*/

#ifndef		__MACHINEEXCEPTIONS__
#define __MACHINEEXCEPTIONS__

#ifndef		__TYPES__
#include <MacTypes.H>
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
/*
	Some basic declarations used throughout the kernel
*/

typedef void *Ref;

typedef long OSStatus;

typedef Ref AreaID;

typedef Ref LogicalAddress;


/*
	Machine Dependent types for PowerPC
*/

struct MachineInformation {
	UnsignedWide			CTR;
	UnsignedWide			LR;
	UnsignedWide			PC;
	unsigned long			CR;
	unsigned long			XER;
	unsigned long			MSR;
};
typedef struct MachineInformation MachineInformation;

struct RegisterInformation {
	UnsignedWide			R0;
	UnsignedWide			R1;
	UnsignedWide			R2;
	UnsignedWide			R3;
	UnsignedWide			R4;
	UnsignedWide			R5;
	UnsignedWide			R6;
	UnsignedWide			R7;
	UnsignedWide			R8;
	UnsignedWide			R9;
	UnsignedWide			R10;
	UnsignedWide			R11;
	UnsignedWide			R12;
	UnsignedWide			R13;
	UnsignedWide			R14;
	UnsignedWide			R15;
	UnsignedWide			R16;
	UnsignedWide			R17;
	UnsignedWide			R18;
	UnsignedWide			R19;
	UnsignedWide			R20;
	UnsignedWide			R21;
	UnsignedWide			R22;
	UnsignedWide			R23;
	UnsignedWide			R24;
	UnsignedWide			R25;
	UnsignedWide			R26;
	UnsignedWide			R27;
	UnsignedWide			R28;
	UnsignedWide			R29;
	UnsignedWide			R30;
	UnsignedWide			R31;
};
typedef struct RegisterInformation RegisterInformation;

struct FPUInformation {
	UnsignedWide			Registers[32];
	unsigned long			FPSCR;
};
typedef struct FPUInformation FPUInformation;


/*
	Exception related declarations
*/


//	Memory exceptions

enum  {
	writeReference			= 0,
	readReference			= 1,
	fetchReference			= 2
};

typedef unsigned long MemoryReferenceKind;

struct MemoryExceptionInformation {
	AreaID			theArea;
	LogicalAddress			theAddress;
	OSStatus			theError;
	MemoryReferenceKind			theReference;
};
typedef struct MemoryExceptionInformation MemoryExceptionInformation;

enum  {
	unknownException			= 0,
	illegalInstructionException	= 1,
	trapException			= 2,
	accessException			= 3,
	unmappedMemoryException		= 4,
	excludedMemoryException		= 5,
	readOnlyMemoryException		= 6,
	unresolvablePageFaultException = 7,
	privilegeViolationException	= 8,
	traceException			= 9,
	instructionBreakpointException = 10,
	dataBreakpointException		= 11,
	integerException			= 12,
	floatingPointException		= 13,
	stackOverflowException		= 14,
	terminationException		= 15
};

typedef unsigned long ExceptionKind;

struct ExceptionInformation {
	ExceptionKind			theKind;
	MachineInformation			*machineState;
	RegisterInformation			*registerImage;
	FPUInformation			*FPUImage;
	union {
		MemoryExceptionInformation	*memoryInfo;
	}			info;
};
typedef struct ExceptionInformation ExceptionInformation;


/*
	Note: An ExceptionHandler is NOT a UniversalProcPtr�s. It must be
	a native PowerPC transition vector with NO routine descriptor.
*/

typedef OSStatus (*ExceptionHandler)(ExceptionInformation *theException);


/*
	Routines for installing per-process exception handlers
*/

#ifdef __cplusplus
extern "C" {
#endif

extern ExceptionHandler InstallExceptionHandler(ExceptionHandler theHandler);
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


