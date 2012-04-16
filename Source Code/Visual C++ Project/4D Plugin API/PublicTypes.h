// ---------------------------------------------------------------
//
// 4D Plugin API
//
// File : PublicTypes.h
// Description : all the structures needed to use 4D Plugin API
//
// rev : 6.8.1
//
// ---------------------------------------------------------------

#ifndef __PUBLICTYPES__
#define __PUBLICTYPES__


// all the 4th Dimension structures use 2 bytes alignment
#if VERSIONWIN
	#pragma pack(push,2)
#elif VERSIONMAC
	#pragma options align = mac68k
#endif


typedef char** PA_Handle;
typedef long PA_WindowRef;
typedef long PA_PortRef;
typedef long PA_PluginRef;
typedef PA_Handle PA_QueryRef;
typedef PA_Handle PA_OrderByRef;
typedef long PA_Dial4D;

// need a crossplatform type for 64 bits integers
#if VERSIONWIN
typedef __int64 PA_long64;
#elif VERSIONMAC
typedef long long PA_long64;
#endif


// opaque structure to store Plugin call parameters
typedef struct PluginBlock
{
	void*			fParameters;
	void*			fResult;
	void**			fData;
} PluginBlock;
typedef PluginBlock* PA_PluginParameters;


// different selectors that can be sent to plugin for different event
#define  kServerDeinitPlugin		-220
#define  kServerCleanUp				-207
#define  kWriteOnServer				-206
#define  kReadOnServer				-205
#define  kClientDisconnect			-202
#define  kClientConnect				-201
#define  kServerInitPlugin			-200
#define  kPackageGiveMemory			-10
#define  kCreateProcess				-3		// called each time a process is created
#define  kCloseProcess				-4		// called each time a process is removed
#define  kClientDeinitPlugin		-2 
#define  kDeinitPlugin				kClientDeinitPlugin
#define  kClientInitPlugin			-1
#define  kInitPlugin				kClientInitPlugin




// --------------------------------------------------------------------------------
// Event Record (same as Macintosh EventRecord)
// --------------------------------------------------------------------------------

typedef struct PA_Event
{
	short			fWhat;
	long			fMessage;
	long			fWhen;
	short			fWhereV;
	short			fWhereH;
	short			fModifiers;
} PA_Event;


// --------------------------------------------------------------------------------
// Plugin area events
// --------------------------------------------------------------------------------


typedef enum
{
	eAE_Idle							= 0,
	eAE_MouseDown						= 1,
	eAE_MouseUp							= 2,
	eAE_KeyDown							= 3,
	eAE_KeyUp							= 4,
	eAE_AutoKey							= 5,
	eAE_Update							= 6,
	eAE_InitArea						= 16,	// first call to area, time to call PA_SetAreaReference
	eAE_Cursor							= 18,	// mouse has moved (even if your area is not selected)
	eAE_IsFocusable						= 20,
	eAE_Select							= 21,
	eAE_Deselect						= 22,
	eAE_Scroll							= 25,	// area rect has changed, call PA_GetAreaRect
	eAE_DesignUpdate					= 26,
	eAE_TestPrintSize					= 27,
	eAE_GetPrintSize					= 28,
	eAE_PrintBand						= 29,
	eAE_UndoCommand						= 30,
	eAE_CutCommand						= 31,
	eAE_CopyCommand						= 32,
	eAE_PasteCommand					= 33,
	eAE_SelectAllCommand				= 34,
	eAE_UpdateEditCommands				= 43,	// you need to call PA_UpdateEditMenu 
	eAE_LoadRecord						= 69,
	eAE_SaveRecord						= 70,
	eAE_AllowDrop						= 80,
	eAE_Drag							= 81,
	eAE_Drop							= 82,
	eAE_WebPublish						= 128,
	eAE_WebPublishPicture				= 129,
	eAE_WebDisposeData					= 130,
	eAE_WebClick						= 131,
	eAE_EditAdvancedProperties			= 600,	// user clicks on "Advanced Properties" button
	eAE_DisposeAdvancedProperties		= 601,	// 4D has stored the advanced plugin properties, you can now dispose your data
	eAE_InitAdvancedProperties			= 602,	// it's time to read the advanced properties
	eAE_AreAdvancedPropertiesEditable	= 603,	// 4D wants to know if you want to use "Advanced Properties" button
	eAE_GetMenuIcon						= 604,	// 4D requests the id of the 'cicn' resources used in plugin menu
	eAE_DesignInit						= 605,	// called when layout is opened in design mode
	eAE_Deinit							= 17
} PA_AreaEvent;



// --------------------------------------------------------------------------------
// Plugin area properties blocks
// --------------------------------------------------------------------------------

typedef struct PA_PluginProperties
{
	short			fVersion;
	short			fFontID;			//	Macintosh font ID
	short			fJustification;
	char			fFontSize;
	char			fFontFace;
	long			fForeColor;
	long			fBackColor;
	void*			fAdvancedProperties;
	long			fAdvancedPropertiesSize;
	char			fPageMode;		//	0: List, 1: Page, 2: Page non enterable.
	char			fPrintingMode;	//	0: Not printing, -1: Printing, -2: Print line.
	short			fPage;
	short			fTable;
	long			fUnused;
	char			fDraggable;
	char			fDroppable;
	short			fLook;
	void*			fMacWindow;
	void*			fMacPort;
	void*			fWinHWND;
	void*			fWinHDC;
	char			fInterProcessVariable;
} PA_PluginProperties;



// --------------------------------------------------------------------------------
// Rectangle Record
// --------------------------------------------------------------------------------

typedef struct PA_Rect
{
	short			fTop;
	short			fLeft;
	short			fBottom;
	short			fRight;
} PA_Rect;


// --------------------------------------------------------------------------------
// Point Record
// --------------------------------------------------------------------------------

typedef struct PA_Point
{
	short			fv;
	short			fh;
} PA_Point;


// --------------------------------------------------------------------------------
// 4D Server and 4D Client, Read/Write data structure
// --------------------------------------------------------------------------------

typedef struct PA_ReadWriteBlock
{
   unsigned long	fDataType;
   short			fDataID;
   long				fDataSize;
   long				fPackID;
   long				fProcessID;
} PA_ReadWriteBlock;



// --------------------------------------------------------------------------------
// 4th Dimension Arrays 
// --------------------------------------------------------------------------------

typedef struct PA_Array
{
	long			fNbElements;	// Number of elements
	PA_Handle		fData;			// Handle to elements
	short			fCurrent;		// Selected element number
} PA_Array;



// --------------------------------------------------------------------------------
// 4th Dimension Dates 
// --------------------------------------------------------------------------------

typedef struct PA_Date
{
	short			fDay;
	short			fMonth;
	short			fYear;
} PA_Date;

// --------------------------------------------------------------------------------
// 4th Dimension text structure. text is stored in Macintosh characters
// fSize can vary from 0 to 32000. if fSize is zero, fHandle can be also zero.
// --------------------------------------------------------------------------------
typedef struct PA_Text
{
	short			fSize;
	PA_Handle		fHandle;
} PA_Text;

// --------------------------------------------------------------------------------
// 4th Dimension Picture expression
// --------------------------------------------------------------------------------
typedef struct PA_Picture
{
   long				fSize;
	PA_Handle		fHandle;
} PA_Picture;


typedef struct PA_PictureInfo	// these informations are used when
								// the picture is displayed on background
{
	short			fVOffset;
	short			fHOffset;
	short			fMode;
} PA_PictureInfo;


// --------------------------------------------------------------------------------
// 4th Dimension Blob expression 
// --------------------------------------------------------------------------------
typedef struct PA_Blob
{
   long				fSize;
	PA_Handle		fHandle;
} PA_Blob;


// --------------------------------------------------------------------------------
// 4th Dimension fixed length string variable.
// Text is stored as a Pascal string using Macintosh characters.
// --------------------------------------------------------------------------------
typedef struct PA_String
{
   short			fSize;			// as defined in C_STRING
   char				fString[ 256 ];
} PA_String;

// --------------------------------------------------------------------------------
// 4th Dimension pointers
// --------------------------------------------------------------------------------

typedef enum
{
	ePK_InvalidPointer = -1,
	ePK_PointerToVariable = 0,
	ePK_PointerToTable,
	ePK_PointerToField
} PA_PointerKind;


typedef struct PointerToVariable
{
	char			fName[ 32 ];
	long			fIndice;		// used for pointer to array elements
} PointerToVariable;


typedef struct PointerToTableField
{
	short			fField;			// field number. 0 if pointer to table
	short			fTable;
	short			fNbSubTables;
	short			fSubTables[5];
} PointerToTableField;


typedef struct PointerBlock
{
	char			fClass;			// 0 : field,   1 : variable
	char			fScope;			// 1 : process, 2 : interprocess
	union
	{
		PointerToVariable		fVariable;
		PointerToTableField		fTableField;
	} uValue;
} PointerBlock;

typedef PointerBlock* PA_Pointer;


// --------------------------------------------------------------------------------
// 4th Dimension Drag and Drop info
// --------------------------------------------------------------------------------

typedef enum
{
	eDK_InvalidDrag = -1,
	eDK_DragVariable = 0,
	eDK_DragTable,
	eDK_DragField
} PA_DragKind;


typedef struct PA_DragAndDropInfo
{
	char	fReserved1[10];
	long	fToArrayIndice;		// indice of element when destination is an array
	long	fReserved2[2];
	long	fFromArrayIndice;	// indice of element when source is an array
	short	fFromProcess;
	short	fFromWhereV;		// where user clicks at first
	short	fFromWhereH;
	short	fToWhereV;			// where user release mouse button
	short	fToWhereH;
	long	fReserved3;
	char	fVariableName[32];	// empty string or variable name if user drags a variable
	char	fInterProcess;
	short 	fField;
	short	fTable;				// zero or table name if user drags a field from a table
} PA_DragAndDropInfo;



// --------------------------------------------------------------------------------
// 4th Dimension Variables
// --------------------------------------------------------------------------------

typedef struct PA_Variable
{
   char				fType;
   char				fFiller;
   union
   {
      double			fReal;				// C_REAL variable
      PA_Date			fDate;				// C_DATE variable
      char				fBoolean;			// C_BOOLEAN variable
      PA_Text			fText;				// C_TEXT variable
      PA_Picture		fPicture;			// C_PICTURE variable
	  PA_Blob			fBlob;				// C_BLOB variable
      long				fLongint;			// C_LONGINT variable
      long				fTime;				// C_TIME variable
      PA_String			fString;			// C_STRING variable
      PA_Array			fArray;				// Any array
	  PA_Pointer*		fPointer;			// C_POINTER variables
   } uValue;
} PA_Variable;



// --------------------------------------------------------------------------------
// 4th Dimension logfile internal structure
// --------------------------------------------------------------------------------

// a logfile starts with this structure,
// then is followed by the number of PA_LogTag
// described in fNbOperations.
typedef struct PA_LogHeader
{
	long			fNbOperations;
	long			fLastFlush;
	PA_long64		fLastAction;
	PA_long64		fFirstAction;
	long			fLogNumber;
	long			fFiller;
	long			fLastPos;
} PA_LogHeader;


#define kTagLog		0x4049999

// each PA_LogTag is followed by its specific data.
// the size of the logfile specific data is in fSize
typedef struct PA_LogTag
{
	long			fTag;		// always set to kTagLog
	long			fWhen;
	char			fAction;	// as decribed in PA_LogAction enum
	char			fFiller;
	short			fProcess;
	short			fUser;
	long			fRecord;
	short			fTable;
	long			fSize;
} PA_LogTag;



typedef enum
{
    eLA_None = 0,
    eLA_Append,
    eLA_Delete,
    eLA_Modify,
    eLA_StartTransaction,
    eLA_ValidateTransaction,
    eLA_CancelTransaction,
    eLA_OpenData,
    eLA_CloseData,
    eLA_StartFlushCache,
    eLA_EndFlushCache,
    eLA_AddField,
    eLA_AddFile,
    eLA_ChangeField,
    eLA_AddIndex,
    eLA_RemoveIndex
} PA_LogAction;


// --------------------------------------------------------------------------------
// strings
// --------------------------------------------------------------------------------

typedef enum
{
	eSK_CString = 0,
	eSK_PString
} PA_StringKind;

typedef enum
{
	eCS_Macintosh = 0,
	eCS_Ansi,
	eCS_Unicode
} PA_CharSet;


// --------------------------------------------------------------------------------
// window levels
// --------------------------------------------------------------------------------

typedef enum
{
	eWL_Window      = 1,
	eWL_Palette     = 2,
	eWL_Dialog      = 5,
	eWL_OnEvent     = 6,
	eWL_Toolbar     = 7,
	eWL_Combo       = 8,
	eWL_Tip         = 9,
	eWL_SuperDialog = 10
} PA_WindowLevel;

// --------------------------------------------------------------------------------
// user kinds
// --------------------------------------------------------------------------------

typedef enum
{
	eUK_CreatedByDesigner = 0,
	eUK_CreatedByAdministrator
} PA_UserKind;


// --------------------------------------------------------------------------------
// platform interface
// --------------------------------------------------------------------------------

typedef enum
{
	ePI_Automatic	= -1,	// Show layouts according to platform GUI
	ePI_Macintosh	= 0,	// Show layouts using Macintosh GUI	
	ePI_Win31		= 1,	// Show layouts using Windows 3.1.x GUI
	ePI_Win95		= 2,	// Show layouts using Windows 95 GUI
	ePI_MacTheme	= 3		// Show layouts using Macintosh Appearance Manager
} PA_PlatformInterface;


// --------------------------------------------------------------------------------
// Web publishing picture kinds
// --------------------------------------------------------------------------------

typedef enum
{
	eWP_MacintoshPicture = 0,
	eWP_GIF,
	eWP_JPEG
} PA_WebPictureKind;


// --------------------------------------------------------------------------------
// 4th Dimension field types
// --------------------------------------------------------------------------------

typedef enum
{
  eFK_InvalidFieldKind	= -1,
  eFK_AlphaField		= 0,	//  Alphanumeric field (from 2 to 80 characters)
  eFK_RealField			= 1,	//  Numeric field (Double or Extended value)
  eFK_TextField			= 2,	//  Text field (up to 32000 characters)
  eFK_PictureField		= 3,	//  Picture field (virtually any block of data)
  eFK_DateField			= 4,	//  Date field 
  eFK_BooleanField		= 6,	//  Boolean field
  eFK_SubfileField		= 7,	//  Subfile field
  eFK_IntegerField		= 8,	//  Integer field (-32768..32767)
  eFK_LongintField		= 9,	//  Long Integer field (-2^31..(2^31)-1)
  eFK_TimeField			= 11,	//	Time field
  eFK_BlobField			= 30	//	Blob field
} PA_FieldKind;


// --------------------------------------------------------------------------------
// 4th Dimension expression and variable types
// --------------------------------------------------------------------------------

typedef enum
{														
	eVK_Real			= 1,	// Variable declared using C_REAL
	eVK_Text			= 2,	// Variable declared using C_TEXT
	eVK_Date			= 4,	// Variable declared using C_DATE
	eVK_Undefined		= 5,	// Undefined variable
	eVK_Boolean			= 6,	// variable declared using C_BOOLEAN
	eVK_Longint			= 9,	// Variable declared using C_LONGINT
	eVK_Picture			= 10,	// Variable declared using C_PICTURE
	eVK_Time			= 11,	// Variable declared using C_TIME
	eVK_ArrayOfArray	= 13,	// Any two-dimensional array
	eVK_ArrayReal		= 14,	// One dimension array declared using ARRAY REAL
	eVK_ArrayInteger	= 15,	// One dimension array declared using ARRAY INTEGER
	eVK_ArrayLongint	= 16,	// One dimension array declared using ARRAY LONGINT
	eVK_ArrayDate		= 17,	// One dimension array declared using ARRAY DATE
	eVK_ArrayText		= 18,	// One dimension array declared using ARRAY TEXT
	eVK_ArrayPicture	= 19,	// One dimension array declared using ARRAY PICTURE
	eVK_ArrayPointer	= 20,	// One dimension array declared using ARRAY POINTER
	eVK_ArrayString		= 21,	// One dimension array declared using ARRAY STRING
	eVK_ArrayBoolean	= 22,	// One dimension array declared using ARRAY BOOLEAN
	eVK_Pointer			= 23,	// Variable declared using C_POINTER
	eVK_String			= 24,	// Variable declared using C_STRING
	eVK_Blob			= 30	// Variable declared using C_BLOB
} PA_VariableKind;

// --------------------------------------------------------------------------------
// 4th Dimension query and sort operators
// --------------------------------------------------------------------------------

// Values for the field fOperator of LineBlock data structure
// These values define the logical connection between the lines of a query definition
typedef enum
{
   eQO_NoOperator = 0,	// Always 1st line of a query definition
   eQO_LogicalAND,		// AND     (& operator)
   eQO_LogicalOR,		// OR      (| operator)
   eQO_Except			// EXCEPT  (# operator)
} PA_QueryOperator;

// Values for the field fComparison of LineBlock data structure
// These values define the comparison operator for the line of query definition	
typedef enum
{
   eQC_NoOperation = 0,			// No query line can have this value
   eQC_IsEqual,				// Is equal to
   eQC_IsDifferent,			// Is not equal to
   eQC_IsGreater,				// Is greater than
   eQC_IsGreaterOrEqual,		// Is greater or equal to
   eQC_IsLess,					// Is less than
   eQC_IsLessOrEqual,			// Is less or equal to
   eQC_Contains,				// Contains
   eQC_NotContains				// Does not contains
} PA_QueryComparison;


// --------------------------------------------------------------------------------
// 4th Dimension errors codes
// --------------------------------------------------------------------------------

typedef enum
{
  // 4D database engine errors codes
  eER_IdxPgOutOfRange					= -10004,	// Index page is out of range
  eER_RecOutOfRange						= -10003,	// Record is out of range (*)
  eER_InvalidRecStruct					= -10002,	// Invalid record structure (*)
													// ( data file needs to be repaired )
  eER_InvalidIdxPage					= -10001,	// Invalid index page (*)
													// ( index needs to be repaired or rebuilt )
													// (*) if 4D Client, maybe due to error
													// of connection reported by network component.
  eER_BadRecordAddress					= -10000,	// Bad record address
  eER_NoMoreSpaceToSave					= -9999,	// No more space available to save the record
  eER_DuplicateKeyIndex					= -9998,	// Duplicated index key
  eER_MaxNumberOfRecords				= -9997,	// Maximum number of records has been reached
  eER_StackIsFull						= -9996,	// Stack is full
  eER_DemoVersionLimit					= -9995,	// Limits of demo version have been reached
  eER_CommInterruptedByUser				= -9994,	// Serial communication interrupted by user
  eER_NoEnoughPrivilege					= -9991,	// Access privileges error
  eER_TimeOut							= -9990,	// Time-out error
  eER_InvalidStruture					= -9989,	// Invalid structure
													// ( database needs to be repaired )
  eER_RelatedRecords					= -9987,	// Other records are related to this record
  eER_RecordLockedDuringDel				= -9986,	// Record(s) locked during an automatic deletion action
  eER_RecursiveIntegrity				= -9985,	// Recursive integrity
  eER_DuplicatedKeyInTrans				= -9984,	// Transaction cancelled because of a dup. index key 
  eER_RecordNotInSelection				= -9982,	// Record not in selection and could not be loaded
  eER_BadFileFieldTable					= -9981,	// Invalid Field/File definition table sent
  eER_StructureIsLocked					= -9980,	// File cannot be created because structure is locked
  eER_BadUserName						= -9979,	// Unknown User
  eER_BadPassword						= -9978,	// Invalid Password
  eER_BadSelection						= -9977,	// The selection does not exist
  eER_BackupInProgress					= -9976,	// Backup in progress, no modifications are allowed
  eER_CannotLoadTrIdxPage				= -9975,	// Transaction index page could not be loaded
  eER_RecordAlreadyDeleted				= -9974,	// The record has already been deleted
  eER_BadTRIC							= -9973,	// TRIC resources are not the same
  eER_BadFileNumber						= -9972,	// File number is out of range
  eER_BadFieldNumber					= -9971,	// Field number is out of range
  eER_FieldNotIndexed					= -9970,	// Field is not indexed
  eER_BadFieldType						= -9969,	// Invalid field type requested
  eER_BadSelectedRecordNumber			= -9968,	// Invalid selected record number requested
  eER_RecordNotLoaded					= -9967,	// The record was not modified because
													// it could not be loaded
  eER_IncompatibleTypes					= -9966,	// Invalid type requested
  eER_BadSearchDefinition				= -9965,	// Bad search definition table sent
  eER_BadSortDefinition					= -9964,	// Bad sort definition table sent
  eER_BadRecordNumber					= -9963,	// Invalid record number requested
  eER_BackupCannotLaunched				= -9962,	// The backup cannot be launched because
													// the server is shutting down
  eER_BackupNotRunning					= -9961,	// The backup process is currently not running
  eER_BackupNotInstalled				= -9960,	// 4D Backup is not installed on the server
  eER_BackupAlreadyRunning				= -9959,	// the backup process has already been started
													// by another user or process
  eER_ProcessNotStarted					= -9958,	// Process could not be started
  eER_ListIsLocked						= -9957,	// The list is locked
  eER_QTNotInstalled					= -9955,	// QuickTime is not installed
  eER_SNoCurrentRecord					= -9954,	// There is no current record
  eER_NoLogFile							= -9953,	// There is no Log file
  eER_BadDataSegmentHeader				= -9952,	// Invalid data segment
  eER_NotRelatedField					= -9951,	// Field is not related to another one
  eER_BadDataSegmentNumber				= -9950,	// Invalid data segment number
  eER_NamedSelNotExist					= -9946,	// Unable to clear the named selection because 
													// it does not exist
  eER_CDROMvers							= -9945,	// CD-ROM 4D Runtime error, 
													// writing operations are not allowed
  eER_Passportvers						= -9943,	// 4D Passport version error
  eER_LicensingError					= -9942,	// 4D Client Licensing scheme is incompatible 
													// with this version of 4D Server
  eER_UnknownGestaltSelector			= -9941,	// Unknown EX_GESTALT selector
  eER_ExtInitFailed						= -9940,	// 4D Extension initialization failed
  eER_ExtNotFound						= -9939,	// External routine not found
  eER_NotEnoughMemory					= -108,		// Not enough Memory

  eER_NoErr								= 0,

  // Additional entry points errors codes
  eER_CallNotImplemented				= -1,		// Entry point is not implemented
  eER_InvalidFileNumber					= 4001,		// Invalid file number
  eER_InvalidRecordNumber				= 4002,		// Invalid record number
  eER_InvalidFieldNumber				= 4003,		// Ivvalid field number
  eER_NoCurrentRecord					= 4004,		// There is no current record
  eER_BadEventCall						= 4005, 
  eER_InvalidOrderByRef					= 4006,
  eER_InvalidQueryRef					= 4007,
  eER_TooManyOrderBy					= 4008,
  eER_TooManyQueries					= 4009

} PA_ErrorCode;


// reset struct alignment
#if VERSIONWIN
	#pragma pack(pop)
#elif VERSIONMAC
	#pragma options align = reset
#endif

#endif
