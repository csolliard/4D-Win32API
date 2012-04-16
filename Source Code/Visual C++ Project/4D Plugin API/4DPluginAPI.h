// ---------------------------------------------------------------
//
// 4D Plugin API
//
// File : 4DPluginAPI.h
//
// rev : 2004.7
//
// ---------------------------------------------------------------

#ifndef __4DPLUGINAPI__
#define __4DPLUGINAPI__

#include "Flags.h"
#include "PublicTypes.h"

#ifdef __cplusplus
extern "C" {
#endif



// ---------------------------------------------------------------
// interface that must be provided by user
// ---------------------------------------------------------------
void PluginMain( long selector, PA_PluginParameters params );


// ---------------------------------------------------------------
// Returns the last error returned by any call to the API
// ---------------------------------------------------------------

PA_ErrorCode PA_GetLastError();


// ---------------------------------------------------------------
// After a call to PA_UseVirtualStructure(), all pending calls to
// the API will use the virtual structure references if supported.
// Use PA_UseRealStructure() to switch back to real structure.
// By default, real structure is used. 
// ---------------------------------------------------------------

// reads or set 4D Plugin API internal flag
void PA_UseVirtualStructure      ();
void PA_UseRealStructure         ();

// asks 4D to know if a virtual structure is defined
char  PA_VirtualStructureDefined ( );
short PA_GetTrueTableNumber      ( short virtualTable );
void  PA_GetTrueFieldNumber      ( short virtualTable, short virtualField, short* trueTable, short* trueField );
short PA_GetVirtualTableNumber   ( short trueTable );
void  PA_GetVirtualFieldNumber   ( short trueTable, short trueField, short* virtualTable, short* virtualField );




// ---------------------------------------------------------------
// When 4D Plugin API accepts or return a string or a text,
// this string can use Macintosh or Ansi character sets.
// A string can also be a Pascal string or a C string.
//
// By default, 4D Plugin API uses:
//	- C strings on both Macintosh and Windows,
//	- Macintosh caracters set on Macintosh
//	- ANSI characters set on Windows.
//
// For specific needs, you can use the following functions to
// tell 4D Plugin API which kind of strings you want to pass
// or receive. 4D Plugin API will make the appropriate translations.
// ---------------------------------------------------------------

void PA_UseMacCharacters  ();
void PA_UseAnsiCharacters ();
PA_CharSet PA_GetUserCharSet();

void PA_UsePStrings ();
void PA_UseCStrings ();
PA_StringKind PA_GetUserStringKind();

void PA_ReturnNullTerminatedText( char returnNullTerminatedText );
char PA_IsReturningNullTerminatedText();


// ---------------------------------------------------------------
// These functions are used to manage 4th Dimensions handle.
// they are used internally by 4D Plugin API.
// On Macintosh, you can use these handles as regular Mac handles.
// ---------------------------------------------------------------

PA_Handle	PA_NewHandle		( long len );
void		PA_DisposeHandle	( PA_Handle handle );
long		PA_GetHandleSize	( PA_Handle handle );
char		PA_SetHandleSize	( PA_Handle handle, long newlen );
char*		PA_LockHandle		( PA_Handle handle );
void		PA_UnlockHandle		( PA_Handle handle );
void		PA_MoveBlock		( void *source, void *dest, long len );
char		PA_GetHandleState	( PA_Handle handle );
void		PA_SetHandleState	( PA_Handle handle, char state );

// ---------------------------------------------------------------
// these commands returns or set informations 
// on 4th Dimension application
// ---------------------------------------------------------------

unsigned long PA_Get4DVersion ();
char PA_IsDemoVersion         ();
char PA_IsDatabaseLocked      ();	// may happen when running on CD-ROM
char PA_IsCompiled            ();

char PA_Is4DClient ();
char PA_Is4DServer ();
char PA_Is4DMono   ();

char PA_IsWebProcess ();

char PA_GetMessagesStatus  ( );
void PA_SetMessagesStatus  ( char showMessages );
void PA_SetThermometerRect ( PA_Rect rect );

void PA_GetStructureName   ( char* structName );
void PA_GetDataName        ( char* dataName );
void PA_GetLogName         ( char* logName );

void PA_GetApplicationFullPath ( char* applicationPath );
void PA_GetStructureFullPath   ( char* structurePath );

short PA_FindPackage ( short packageID );
void  PA_PackageInfo ( short packageNumber, short* packageID, void** procPtr, void** data, short* resfile, char* name );

void  PA_Quit4D();

void  PA_Get4DPreferencesFilename    ( char* prefsName );
void  PA_Get4DPreferencesFolder      ( char* folderPath );
short PA_Open4DPreferencesMacResFile ( );
void  PA_Close4DPreferences          ( );

void PA_Get4Dfolder ( char* folderPath, PA_FolderKind folderKind, char createIfNotFound );

PA_PlatformInterface PA_GetPlatformInterface();

void PA_GetToolBarInfo  ( char* displayed, short* toolbarHeight );
void PA_ShowHideToolBar ( char displayed );

char PA_GetTipsEnabled  ( );
void PA_SetTipsEnabled  ( char enabled );

void* PA_SetGrowZone    ( void* growZoneHandler );
long PA_CheckFreeStack  ( long requestedStack );

void* PA_Get4DWinMacOSGlobals ( );

long PA_Get4DHInstance();

long PA_MethodNames( PA_Handle* hnames, PA_Handle* hids );

void PA_GetCenturyInfo( long* pivotYear, long* defaultCentury );

// ---------------------------------------------------------------
// those commands can format strings, numbers, date and time
// as the String command in 4D. FormatNumber are the same as
// in 4D for formating date and times
// (see String command documentation)
// ---------------------------------------------------------------

void PA_FormatString   ( char* string, char* format, char* result );
void PA_FormatReal     ( double value, char* format, char* result );
void PA_FormatLongint  ( long value,   char* format, char* result );
void PA_FormatDate     ( short day, short month, short year, short formatNumber, char* result );
void PA_FormatTime     ( long time, short formatNumber, char* result );


long   PA_EvalLongint  ( char* string );
double PA_EvalReal     ( char* string );
void   PA_EvalDate     ( char* string, short* day, short* month, short* year );
long   PA_EvalTime     ( char* string );

char PA_CompareStrings ( char* text1, long len1, char* text2, long len2, char diacritic );

void PA_ConvertStrings ( char* string1, PA_StringKind kind1, PA_CharSet charset1, 
					     char* string2, PA_StringKind kind2, PA_CharSet charset2 );

// ---------------------------------------------------------------
// some functions needs to know if you want automatic relations to be activated.
// Default is both automatic relations On.
// calling this function tells explicitly 4D if you want automatic relations activated or not.
// ---------------------------------------------------------------

void PA_UseAutomaticRelations ( char manyToOne, char oneToMany );

// ---------------------------------------------------------------
// inforation about import/export filters.
// ---------------------------------------------------------------

char* PA_GetExportFilter ();
char* PA_GetImportFilter ();

// ---------------------------------------------------------------
// Selection
// Warning : these functions do not use virtual structure.
// they can use automatic relations depending 
// of the use of PA_UseAutomaticRelations().
// ---------------------------------------------------------------

long PA_SelectedRecordNumber ( short table );
void PA_GotoSelectedRecord   ( short table, long record );
long PA_RecordsInSelection   ( short table );
void PA_DeleteSelection      ( short table );
void PA_FirstRecord          ( short table );
void PA_NextRecord           ( short table );
void PA_PreviousRecord       ( short table );
char PA_BeforeSelection      ( short table );
char PA_EndSelection         ( short table );

// ---------------------------------------------------------------
// Tables and fields
// ---------------------------------------------------------------

PA_TableRef PA_CreateTableRef();
void PA_AddField( PA_TableRef tableRef, char* fieldName, PA_FieldKind kind, short alphaLength, PA_FieldAttributes attributes );
void PA_CreateTable( PA_TableRef tableRef, char* tableName, char * templateListName, char * templateDetailName );
void PA_GetTemplateName(PA_Variable * templateArray );

// ---------------------------------------------------------------
// Records
// ---------------------------------------------------------------

void PA_CreateRecord ( short table );
void PA_SaveRecord   ( short table );
void PA_GotoRecord   ( short table, long record );
long PA_RecordNumber ( short table );


// ---------------------------------------------------------------
// Relations
// ---------------------------------------------------------------

void PA_RelateOne  ( short table );
void PA_RelateMany ( short table );
void PA_RelateOneSelection  ( short manyTable, short oneTable  );
void PA_RelateManySelection ( short manyTable, short manyField );


// ---------------------------------------------------------------
// Records locking
// ---------------------------------------------------------------

char PA_Locked     ( short table );
void PA_ReadWrite  ( short table );
void PA_ReadOnly   ( short table );
void PA_LoadRecord ( short table );


// ---------------------------------------------------------------
// Named selections
// ---------------------------------------------------------------

void PA_CopyNamedSelection  ( short table, char* name );
void PA_CutNamedSelection   ( short table, char* name );
void PA_UseNamedSelection   ( char* name );
void PA_ClearNamedSelection ( char* name );


// ---------------------------------------------------------------
// Sets
// ---------------------------------------------------------------

void PA_CreateEmptySet ( short table, char* name );
void PA_CreateSet	   ( short table, char* name );
void PA_AddToSet       ( short table, char* name );
void PA_UseSet         ( char* name );
void PA_DeleteSet      ( char* name );


// ---------------------------------------------------------------
// database informations
// ---------------------------------------------------------------

// ---- tables
short PA_CountTables    ( );
void  PA_GetTableName   ( short table, char* tableName );
char  PA_IsTableVisible ( short table );

// request 4D Client to ask 4D Server for the current number of records for a table 
void PA_UpdateInternalCache ( short table );

//	tableAndFieldNames should be as "[Table1]field1"
void  PA_GetTableAndFieldNumbers( char* tableAndFieldNames, short* table, short* field );

// ---- fields
short PA_CountFields        ( short table );
void  PA_GetFieldName       ( short table, short field, char* fieldName );
void  PA_GetFieldProperties ( short table, short field, PA_FieldKind* kind,
                              short* stringlength, char* indexed, long* attributes );
void  PA_GetFieldRelation   ( short table, short field, 
                              short* relatedTable, short* relatedField );
void  PA_GetFieldList       ( short table, short field, char* listName );

void PA_TableAndFieldPopup  ( short x, short y, short* table, short* field );
void PA_FieldPopup          ( short x, short y, short table, short* field );


// ---------------------------------------------------------------
// Order by
// ---------------------------------------------------------------

// easy function used to order selection using only one field, located in the table
void PA_OrderBy ( short table, short field, char ascending );

// set of functions used to order selection using multiple fields
PA_OrderByRef PA_OpenOrderBy ( short table );
void PA_AddFieldToOrderBy ( PA_OrderByRef orderby, short table, short field, char ascending );
void PA_CloseOrderBy      ( PA_OrderByRef orderby );



// ---------------------------------------------------------------
// Query 
// ---------------------------------------------------------------

// set of functions used to query in selection using multiple criterias

PA_QueryRef PA_OpenQuery ( short table );
void PA_QueryString   ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, char* string );
void PA_QueryReal     ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, double value );
void PA_QueryTime     ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, long value );
void PA_QueryDate     ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short day, short month, short year );
void PA_QueryLongint  ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, long value );
void PA_QueryInteger  ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short value );
void PA_QueryBoolean  ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, char value );
void PA_CloseQuery    ( PA_QueryRef query );



// ---------------------------------------------------------------
// Get fields from database
// ---------------------------------------------------------------

void   PA_GetStringField  ( short table, short field, char* string );
long   PA_GetTextField    ( short table, short field, char* text   );
long   PA_GetBlobField    ( short table, short field, void* blob   );
long   PA_GetPictureField ( short table, short field, void* picture, PA_PictureInfo* info );
double PA_GetRealField    ( short table, short field );
long   PA_GetLongintField ( short table, short field );
short  PA_GetIntegerField ( short table, short field );
long   PA_GetTimeField    ( short table, short field );
void   PA_GetDateField    ( short table, short field, short* day, short* month, short* year );
char   PA_GetBooleanField ( short table, short field );

PA_Handle PA_GetBlobHandleField    ( short table, short field );
PA_Handle PA_GetPictureHandleField ( short table, short field, PA_PictureInfo* info );

// ---------------------------------------------------------------
// Set fields in database
// ---------------------------------------------------------------

void PA_SetStringField  ( short table, short field, char* string );
void PA_SetTextField    ( short table, short field, char* text,    long len );
void PA_SetBlobField    ( short table, short field, void* blob,    long len );
void PA_SetPictureField ( short table, short field, void* picture, long len, PA_PictureInfo info );
void PA_SetRealField    ( short table, short field, double value );
void PA_SetLongintField ( short table, short field, long  value );
void PA_SetIntegerField ( short table, short field, short value );
void PA_SetTimeField    ( short table, short field, long  value );
void PA_SetDateField    ( short table, short field, short day, short month, short year );
void PA_SetBooleanField ( short table, short field, char value );

void PA_SetBlobHandleField    ( short table, short field, PA_Handle hblob );
void PA_SetPictureHandleField ( short table, short field, PA_Handle hpicture, PA_PictureInfo info );


// -----------------------------------------
// 4th Dimension Packed Records
// -----------------------------------------

long PA_GetPackedRecord ( short table, void* buffer );
void PA_SetPackedRecord ( short table, void* buffer, long len );



// -----------------------------------------------------
// Read parameters when 4th Dimension calls
// a plugin command
// NOTE: the first parameter starts at index 1
// -----------------------------------------------------

short	PA_GetShortParameter        ( PA_PluginParameters params, short index );
long	PA_GetLongParameter         ( PA_PluginParameters params, short index );
double	PA_GetDoubleParameter       ( PA_PluginParameters params, short index );
void	PA_GetStringParameter       ( PA_PluginParameters params, short index, char* string );
long	PA_GetTextParameter         ( PA_PluginParameters params, short index, char* text );
long	PA_GetBlobParameter         ( PA_PluginParameters params, short index, void* blob );
long	PA_GetPictureParameter      ( PA_PluginParameters params, short index, void* picture, PA_PictureInfo* info );
void	PA_GetDateParameter         ( PA_PluginParameters params, short index, short* day, short* month, short* year );
long	PA_GetTimeParameter         ( PA_PluginParameters params, short index );
PA_Variable PA_GetVariableParameter ( PA_PluginParameters params, short index );

PA_Handle PA_GetBlobHandleParameter    ( PA_PluginParameters params, short index );
PA_Handle PA_GetPictureHandleParameter ( PA_PluginParameters params, short index, PA_PictureInfo* info );


PA_Pointer     PA_GetPointerParameter  ( PA_PluginParameters params, short index );

// obsolete, use PA_GetPointerValue and PA_SetPointerValue for easier use and support of pointers to locals
PA_PointerKind PA_GetPointerKind       ( PA_Pointer pointer );
void           PA_GetPointerTableField ( PA_Pointer pointer, short* table, short* field );
PA_Variable    PA_GetPointerVariable   ( PA_Pointer pointer, long* indice );
void           PA_SetPointerVariable   ( PA_Pointer pointer, PA_Variable variable );

// 2004.1
PA_Pointer	   PA_GetPointer(const PA_Pointer inPointerBlock,PA_Pointer outPointerBlock);
PA_Variable    PA_GetPointerValue      ( PA_Pointer pointer ); // Call PA_ClearVariable to clear the returned PA_Variable.
void           PA_SetPointerValue      ( PA_Pointer pointer, PA_Variable variable );


// -----------------------------------------
// Return value when 4th Dimension calls
// a plugin command
// -----------------------------------------

void PA_ReturnShort    ( PA_PluginParameters params, short value );
void PA_ReturnLong     ( PA_PluginParameters params, long value );
void PA_ReturnDouble   ( PA_PluginParameters params, double value );
void PA_ReturnString   ( PA_PluginParameters params, char* string );
void PA_ReturnText     ( PA_PluginParameters params, char* text, long len );
void PA_ReturnBlob     ( PA_PluginParameters params, void* blob, long len );
void PA_ReturnPicture  ( PA_PluginParameters params, void* picture, long len, PA_PictureInfo info );
void PA_ReturnDate     ( PA_PluginParameters params, short day, short month, short year );
void PA_ReturnTime     ( PA_PluginParameters params, long value );

void PA_ReturnBlobHandle    ( PA_PluginParameters params, PA_Handle hblob );
void PA_ReturnPictureHandle ( PA_PluginParameters params, PA_Handle hpicture, PA_PictureInfo info );


// -----------------------------------------------------
// Set parameters when 4th Dimension calls
// a plugin command
// NOTE: the first parameter starts at index 1
// -----------------------------------------------------

void PA_SetShortParameter    ( PA_PluginParameters params, short index, short value );
void PA_SetLongParameter     ( PA_PluginParameters params, short index, long value );
void PA_SetDoubleParameter   ( PA_PluginParameters params, short index, double value );
void PA_SetStringParameter   ( PA_PluginParameters params, short index, char* string );
void PA_SetTextParameter     ( PA_PluginParameters params, short index, char* text, long len );
void PA_SetBlobParameter     ( PA_PluginParameters params, short index, void* blob, long len );
void PA_SetPictureParameter  ( PA_PluginParameters params, short index, void* picture, long len, PA_PictureInfo info );
void PA_SetDateParameter     ( PA_PluginParameters params, short index, short day, short month, short year );
void PA_SetTimeParameter     ( PA_PluginParameters params, short index, long value );
void PA_SetVariableParameter ( PA_PluginParameters params, short index, PA_Variable variable, char clearOldValue );

void PA_SetBlobHandleParameter    ( PA_PluginParameters params, short index, PA_Handle hblob );
void PA_SetPictureHandleParameter ( PA_PluginParameters params, short index, PA_Handle hpicture, PA_PictureInfo info );


// -----------------------------------------------------
// manage events on a plugin area
// -----------------------------------------------------

PA_AreaEvent PA_GetAreaEvent      ( PA_PluginParameters params );
PA_Rect PA_GetAreaRect            ( PA_PluginParameters params );
void    PA_GetAreaName            ( PA_PluginParameters params, char* name );
void    PA_GetPluginProperties    ( PA_PluginParameters params, PA_PluginProperties* properties );
void    PA_SetAreaReference       ( PA_PluginParameters params, void* ref );
void*   PA_GetAreaReference       ( PA_PluginParameters params );
void    PA_SetAreaFocusable       ( PA_PluginParameters params, char focusable );
char    PA_IsActivated            ( PA_PluginParameters params );
char    PA_IsDeActivated          ( PA_PluginParameters params );
void    PA_AcceptSelect           ( PA_PluginParameters params, char accept );
void    PA_AcceptDeselect         ( PA_PluginParameters params, char accept );
void    PA_GetClick               ( PA_PluginParameters params, short* x, short* y );
short	PA_GetMouseWheelIncrement ( PA_PluginParameters params );
char    PA_GetKey                 ( PA_PluginParameters params );
void    PA_CustomizeDesignMode    ( PA_PluginParameters params );
void    PA_DontTakeEvent          ( PA_PluginParameters params );
void    PA_CallPluginAreaMethod   ( PA_PluginParameters params );
void    PA_SetMenuIcon            ( PA_PluginParameters params, short id );
long    PA_GetUpdateHDC           ( );
void	PA_PublishWebPicture      ( PA_PluginParameters params, char pictureMap );
void	PA_SendHTML               ( PA_PluginParameters params, void* webData, char* HTMLbuffer, long len );
void	PA_SendWebPicture         ( PA_PluginParameters params, void* webData, void* picture, long len, PA_WebPictureKind kind );
void*	PA_GetWebDataToDispose    ( PA_PluginParameters params );
void	PA_GotoNextField          ( PA_PluginParameters params );
void	PA_GotoPreviousField      ( PA_PluginParameters params );
void	PA_GetPageChange          ( PA_PluginParameters params, short *pageFrom, short *pageTo );
void	PA_RequestRedraw          ( PA_PluginParameters params );


// -----------------------------------------------------
// manage dropping events on a plugin area
// -----------------------------------------------------

PA_DragAndDropInfo PA_GetDragAndDropInfo ( PA_PluginParameters params );
void PA_AllowDrop          ( PA_PluginParameters params, char allow );
void PA_GetDragPositions   ( PA_PluginParameters params, PA_Rect* rect, short* x, short* y );
void PA_CustomizeDragOver  ( PA_PluginParameters params );


// -----------------------------------------------------
// Getting informations on a Drag And Drop Info structure
// -----------------------------------------------------

PA_DragKind PA_GetDragAndDropKind     ( PA_DragAndDropInfo info );
PA_Variable PA_GetDragAndDropVariable ( PA_DragAndDropInfo info, long* indice );
void PA_GetDragAndDropTableField      ( PA_DragAndDropInfo info, short* table, short* field );


// -----------------------------------------------------
// Initiate a drag and drop
// to be called on a eAE_MouseDown event
// -----------------------------------------------------

void PA_DragAndDrop( short startX, short startY, char useCustomRect, PA_Rect customRect );


// -----------------------------------------------------
// Advanced Properties of a plugin area
// -----------------------------------------------------

void  PA_SetAdvancedPropertiesEditable  ( PA_PluginParameters params, char editable );
void  PA_SetAdvancedProperties          ( PA_PluginParameters params, void* data, long datasize );
void* PA_GetAdvancedPropertiesToDispose ( PA_PluginParameters params );
void* PA_GetAdvancedProperties          ( PA_PluginProperties* properties, long* datasize );



// ---------------------------------------------------------------
// Get 4th Dimension variables
// ---------------------------------------------------------------

PA_Variable	PA_CreateVariable      ( PA_VariableKind kind, short stringSize );
PA_Variable	PA_GetVariable         ( char* variableName );
PA_VariableKind PA_GetVariableKind ( PA_Variable variable );
PA_VariableKind PA_GetPointerValueKind( PA_Pointer inVarPtr );

void   PA_GetStringVariable  ( PA_Variable variable, char* string );
long   PA_GetTextVariable    ( PA_Variable variable, char* text   );
long   PA_GetBlobVariable    ( PA_Variable variable, void* blob   );
long   PA_GetPictureVariable ( PA_Variable variable, void* picture, PA_PictureInfo* info );
double PA_GetRealVariable    ( PA_Variable variable );
long   PA_GetLongintVariable ( PA_Variable variable );
long   PA_GetTimeVariable    ( PA_Variable variable );
void   PA_GetDateVariable    ( PA_Variable variable, short* day, short* month, short* year );
char   PA_GetBooleanVariable ( PA_Variable variable );

PA_Handle PA_GetBlobHandleVariable    ( PA_Variable variable );
PA_Handle PA_GetPictureHandleVariable ( PA_Variable variable, PA_PictureInfo* info );



// ---------------------------------------------------------------
// Set 4th Dimension variables
// ---------------------------------------------------------------

void PA_SetVariable ( char* variableName, PA_Variable variable, char clearOldValue );

void PA_SetStringVariable  ( PA_Variable* variable, char* string );
void PA_SetTextVariable    ( PA_Variable* variable, char* text,    long len );
void PA_SetBlobVariable    ( PA_Variable* variable, void* blob,    long len );
void PA_SetPictureVariable ( PA_Variable* variable, void* picture, long len, PA_PictureInfo info );
void PA_SetRealVariable    ( PA_Variable* variable, double value );
void PA_SetLongintVariable ( PA_Variable* variable, long value   );
void PA_SetTimeVariable    ( PA_Variable* variable, long value   );
void PA_SetDateVariable    ( PA_Variable* variable, short day, short month, short year );
void PA_SetBooleanVariable ( PA_Variable* variable, char value );

void PA_SetBlobHandleVariable    ( PA_Variable* variable, PA_Handle hblob );
void PA_SetPictureHandleVariable ( PA_Variable* variable, PA_Handle hpicture, PA_PictureInfo info );



// ---------------------------------------------------------------
// CleanUp of a 4th Dimension variable
// ---------------------------------------------------------------

void PA_ClearVariable( PA_Variable* variable );

// ---------------------------------------------------------------
// 4th Dimension arrays
// ---------------------------------------------------------------

// general functions on arrays
long  PA_GetArrayNbElements ( PA_Variable array );
void  PA_ResizeArray        ( PA_Variable* array, long nb );

long  PA_GetArrayCurrent    ( PA_Variable array );
void  PA_SetArrayCurrent    ( PA_Variable* array, long current );

short PA_GetArrayStringSize ( PA_Variable array );

// Reading values on arrays
short        PA_GetIntegerInArray ( PA_Variable array, long i );
long         PA_GetLongintInArray ( PA_Variable array, long i );
double       PA_GetRealInArray    ( PA_Variable array, long i );
void         PA_GetDateInArray    ( PA_Variable array, long i, short* day, short* month, short* year );
long         PA_GetPictureInArray ( PA_Variable array, long i, void* picture, PA_PictureInfo* info );
long         PA_GetTextInArray    ( PA_Variable array, long i, char* text   );
void         PA_GetStringInArray  ( PA_Variable array, long i, char* string );
char         PA_GetBooleanInArray ( PA_Variable array, long i );
PA_Variable  PA_GetArrayInArray   ( PA_Variable array, long i );
PointerBlock PA_GetPointerInArray ( PA_Variable array, long i );


PA_Handle PA_GetPictureHandleInArray ( PA_Variable array, long i, PA_PictureInfo* info );

// Setting values on arrays
void PA_SetIntegerInArray   ( PA_Variable array, long i, short value );
void PA_SetLongintInArray   ( PA_Variable array, long i, long value  );
void PA_SetRealInArray      ( PA_Variable array, long i, double value );
void PA_SetDateInArray      ( PA_Variable array, long i, short day, short month, short year );
void PA_SetStringInArray    ( PA_Variable array, long i, char* string );
void PA_SetTextInArray      ( PA_Variable array, long i, char* text, long len );
void PA_SetPictureInArray   ( PA_Variable array, long i, void* picture, long len, PA_PictureInfo info );
void PA_SetBooleanInArray   ( PA_Variable array, long i, char value );
void PA_SetArrayInArray     ( PA_Variable array, long i, PA_Variable value );
void PA_SetPointerInArray   ( PA_Variable array, long i, PointerBlock value );

void PA_SetPictureHandleInArray ( PA_Variable array, long i, PA_Handle hpicture, PA_PictureInfo info );

// ---------------------------------------------------------------
// 4th Dimension Methods and functions
// ---------------------------------------------------------------


void		PA_CreateMethod(char * methodName, PA_MethodFlags ref, char * methodCode, long methodCodeLen, char * folderName );
void        PA_ExecuteMethod   ( char* text, long len );
PA_Variable PA_ExecuteFunction ( char* text, long len );

long        PA_Tokenize                ( char* text, long len, void* tokens );
long        PA_Detokenize              ( void* tokens, long len, char* text );
PA_Handle   PA_DetokenizeInTEHandle    ( void* tokens, long len );
void        PA_ByteSwapTokens          ( void* tokens, long len );
void        PA_ExecuteTokens           ( void* tokens, long len );
PA_Variable PA_ExecuteTokensAsFunction ( void* tokens, long len );

long        PA_GetMethodID             ( char* methodName );
PA_Variable PA_ExecuteMethodByID       ( long id, PA_Variable* parameters, short nbParameters );



// ---------------------------------------------------------------
// Log and Backup operations
// ---------------------------------------------------------------

void PA_LockDatabase            ( );
void PA_UnlockDatabase          ( );
char PA_GetBackupStatus         ( );
void PA_NewLog                  ( char* logName );

// name of the log (<32), the log being located next to the data file
void PA_OpenLog                 ( char* logName );

void PA_GetLogInfo              ( PA_LogHeader* logInfo );
void PA_GetLogField             ( PA_LogTag* logtag, short field, char* logdata, long len, char* string );
void PA_GetTextLogField         ( PA_LogTag* logtag, short field, char* logdata, long logdatalen, char* text );
long PA_GetPictureLogField      ( PA_LogTag* logtag, short field, char* logdata, long logdatalen, void* picture, PA_PictureInfo* info );
void PA_PerformLogAction        ( PA_LogTag* logtag, char* logdata, long logdatalen );
PA_FieldKind PA_GetLogFieldKind ( PA_LogTag* logtag, short field, char* logdata, long len );



// ---------------------------------------------------------------
// Multiprocess Printing Management
// ---------------------------------------------------------------

char PA_TryToOpenPrinterSession();
char PA_OpenPrinterSession();
void PA_ClosePrinterSession();
void* PA_GetCarbonPrintSettings();
void* PA_GetCarbonPageFormat();
void* PA_GetWindowsPRINTDLG();
void* PA_GetWindowsPrintingDC();


// ---------------------------------------------------------------
// Processes
// ---------------------------------------------------------------

long PA_CountActiveProcess      ( );
long PA_CountTotalProcess       ( );
void PA_GetProcessInfo          ( long process, char* name, long* state, long* time );
void PA_FreezeProcess           ( long process );
void PA_UnfreezeProcess         ( long process );
char PA_IsProcessDying          ( );
void PA_KillProcess             ( );
void PA_PutProcessToSleep       ( long process, long time );
long PA_GetCurrentProcessNumber ( );
long PA_GetWindowProcess        ( PA_WindowRef windowRef );
long PA_GetFrontWindowProcess   ( char withPalette );
void PA_SetWindowProcess        ( PA_WindowRef windowRef, long process );
void PA_Yield                   ( );
void PA_YieldAbsolute           ( );
char PA_WaitNextEvent		    ( PA_Event* event );
void PA_UpdateProcessVariable   ( long process );
void PA_BringProcessToFront     ( long process );
long PA_NewProcess              ( void* procPtr, long stackSize, char* name );
void PA_PostMacEvent            ( long process, PA_Event* event );



// ---------------------------------------------------------------
// Sending/Receiving documents on 4D Client from 4D Server
// ---------------------------------------------------------------

long PA_ReceiveDocumentFromServer ( char* docName, void* buffer );
void PA_SendDocumentToServer      ( char* docName, void* buffer, long len, unsigned long type, unsigned long creator );
char PA_DocumentExistOnServer     ( char* docName, unsigned long type, unsigned long creator );



// ---------------------------------------------------------------
// Sending/Receiving data between 4D Client and 4D Server
// ---------------------------------------------------------------

void PA_SendDataToServer      ( long pluginRef, long dataRef, long dataKind, void* buffer, long len );
long PA_ReceiveDataFromServer ( long pluginRef, long dataRef, long dataKind, void* buffer );



// ---------------------------------------------------------------
// 4th Dimension internal resource manager used by 4D structure
// ---------------------------------------------------------------

// To read a resource, read first the length of the resource by passing a null buffer to PA_GetResource
// then allocate a buffer of the proper size, and finally read it in the buffer calling PA_GetResource again.
// Before manipulating a resource, you need to lock it over the network so no one can modify it
// during the time you read or modify it.

short PA_CreateResource                 ( short resfile, unsigned long kind, short id, char* name, char* data, long len );
void  PA_RemoveResourceByID             ( short resfile, unsigned long kind, short id );
void  PA_RemoveResourceByName           ( short resfile, unsigned long kind, char* name );
long  PA_GetResource                    ( short resfile, unsigned long kind, short id, char* data );
void  PA_SetResource                    ( short resfile, unsigned long kind, short id, char* data, long len );
char  PA_LockResource                   ( short resfile, unsigned long kind, short id );
void  PA_UnlockResource                 ( short resfile, unsigned long kind, short id );
void  PA_ReleaseResource                ( short resfile, unsigned long kind, short id );
long  PA_GetIndexedResource             ( short resfile, unsigned long kind, short index, char* data );
void  PA_GetResourceName                ( short resfile, unsigned long kind, short id, char* name );
void  PA_SetResourceName                ( short resfile, unsigned long kind, short id, char* name );
long  PA_GetResourceSize                ( short resfile, unsigned long kind, short id );
long  PA_GetResourceTimeStamp           ( short resfile, unsigned long kind, short id );
long  PA_GetResourceIDList              ( short resfile, unsigned long kind, short* IDlist );
short PA_GetUniqueResID                 ( short resfile, unsigned long kind );
long  PA_GetResourceKindList            ( short resfile, unsigned long* kindlist );
long  PA_GetResourceNameList            ( short resfile, unsigned long kind, char* namelist );
long  PA_CountResources                 ( short resfile, unsigned long kind );
long  PA_CountResourceKinds             ( short resfile );
short PA_OpenResFile                    ( char* filename );
short PA_CreateResFile                  ( char* filename );
void  PA_UpdateResFile                  ( short resfile );
void  PA_UseResFile                     ( short resfile );
void  PA_CloseResFile                   ( short resfile );
short PA_GetDatabaseResFile             ( );
PA_Handle PA_GetResourceHandle          ( short resfile, unsigned long kind, short id );
void  PA_WriteResourceHandle            ( short resfile, PA_Handle handle );
short PA_CreateResourceFromHandle       ( short resfile, unsigned long kind, short id, char* name, PA_Handle resourceHandle );
void  PA_DetachResource                 ( short resfile, PA_Handle resourceHandle );
void  PA_LockResourceHandle             ( short resfile, PA_Handle resourceHandle );
void  PA_UnlockResourceHandle           ( short resfile, PA_Handle resourceHandle );
void  PA_ReleaseResourceHandle          ( short resfile, PA_Handle resourceHandle );
unsigned long PA_GetIndexedResourceKind ( short resfile, short index );

// -----------------------------------------
//	4th Dimension users info
// -----------------------------------------

void PA_GetUserName      ( char* name );
long PA_GetCurrentUserID ( );
long PA_CountUsers       ( PA_UserKind kind );
void PA_GetIndUserName   ( long index, PA_UserKind kind, char* userName );
long PA_CountUserGroups  ( PA_UserKind kind );
void PA_GetIndGroupName  ( long index, PA_UserKind kind, char* userName );


// -----------------------------------------
//	4th Dimension serial key and users info
// -----------------------------------------

long PA_GetSerialKey             ( );
void PA_GetRegisteredUserName    ( char *name );
void PA_GetRegisteredUserCompany ( char *company );
long PA_CountConnectedUsers      ( );
long PA_GetMaxAllowedUsers       ( );



// -----------------------------------------
//	4th Dimension Plugin Windows
// -----------------------------------------


PA_PluginRef PA_OpenPluginWindow ( char* areaName, char* windowTitle, PA_Rect rect );
PA_WindowRef PA_NewWindow        ( PA_Rect rect, PA_WindowLevel level, short kind, char* title, char closeBox );

void  PA_SetWindowFocusable    ( PA_WindowRef windowRef, char focusable );
char  PA_IsWindowFocusable     ( PA_WindowRef windowRef );

PA_WindowRef PA_GetWindowFocused ( );
void         PA_SetWindowFocused ( PA_WindowRef windowRef );
char         PA_IsWindowFocused  ( PA_WindowRef windowRef );

void  PA_RedrawWindow          ( );
void  PA_UpdateVariables       ( );
long  PA_GetHWND               ( PA_WindowRef windowRef );
long  PA_GetWindowPtr          ( PA_WindowRef windowRef );
void  PA_ClosePluginWindow     ( PA_PluginRef pluginRef );
void  PA_SetPluginWindowTitle  ( PA_PluginRef pluginRef, char* windowTitle );
void  PA_SetPluginAreaClipMode ( PA_PluginRef pluginRef, char clipChildren );
short PA_GetCurrentPage        ( );
void  PA_CreateTip             ( char* string, short posX, short posY, PA_Rect rect );
void  PA_GotoArea              ( char* variableName );
void  PA_UpdateEditMenu        ( char* undoString, char undo, char redo, char cut, char copy, char paste, char clear, char selectAll );
void  PA_CloseWindow           ( PA_WindowRef windowRef );


// -----------------------------------------
//	4th Dimension Text Editors
// -----------------------------------------

PA_Handle PA_GetCurrentTEHandle();


// -----------------------------------------
//	4th Dimension Web Server
// -----------------------------------------

void PA_SetWebListeners  ( void* listenProcPtr,
                           void* closeProcPtr, 
                           void* readProcPtr,
                           void* writeProcPtr,
                           void* idleProcPtr,
                           void* errorProcPtr );
void PA_SetWebFilters    ( void* filterInProcPtr,
                           void* filterOutProcPtr, 
                           void* releasePtrProcPtr,
                           char webFilterMacChars );
void PA_StartWebServer   ( );
void PA_StopWebServer    ( );
void PA_GetWebServerInfo ( long* webServerProcess, long* TCPport );
void PA_SetWebTCPport    ( long TCPport );
void PA_GetWebContext    ( long* context, long* subContext, char* name );
long PA_GetWebTimeOut    ( );
void PA_SetWebTimeOut    ( long timeOut );
void PA_OpenURL          ( char* url, long len );


// -----------------------------------------
//	4th Dimension SpellCheckers
// -----------------------------------------

void  PA_Install4DWriteSpeller    ( void* spellcheckerProcPtr );
void* PA_Get4DWriteSpellerProcPtr ( );


// -----------------------------------------
//	4th Dimension Editors
// -----------------------------------------

char PA_FormulaEditor      ( short defaultTable, char* text, long* len );
void PA_QueryDialog        ( short table );
void PA_OrderByDialog      ( short table );
void PA_Alert              ( char* message );
char PA_Confirm            ( char* message );
char PA_Request            ( char* message, char* value, char* okButton, char* cancelButton );
PA_Handle PA_PictureEditor ( char* windowTitle,  void* picture, long len );
void PA_MethodEditor       ( char* methodName, long lineNumber );
void PA_AboutDialog        ( );


// -----------------------------------------
//	4th Dimension Pictures
// -----------------------------------------

// Quicktime is mandatory for this routine, format needs to be a valid quicktime format
PA_Handle PA_ConvertPicture( void* picture, long len, unsigned long format );

// returns a EMF handle
long PA_PictureToEMF ( void* picture, long len );

PA_Handle PA_PictureToGIF  ( void* picture, long len );


// -----------------------------------------
//	4th Dimension Dialogs
// -----------------------------------------

// dialog
PA_Dial4D   PA_NewDialog           ( );
PA_Dial4D   PA_OpenDialog          ( PA_Dial4D dialog, char* dialogName, char closeBox );
char PA_ModalDialog                ( PA_Dial4D dialog, char* variableName );
void PA_CloseDialog                ( PA_Dial4D dialog );
void PA_Dial4DCancelValidate       ( PA_Dial4D dialog, char cancel );
void PA_Dial4DDisposeEmptyDialog   ( PA_Dial4D dialog );

// variables
long        PA_Dial4DGetLong       ( PA_Dial4D dialog, char* variable );
double      PA_Dial4DGetReal       ( PA_Dial4D dialog, char* variable );
void        PA_Dial4DGetDate       ( PA_Dial4D dialog, char* variable, short* day, short* month, short* year );
long        PA_Dial4DGetTime       ( PA_Dial4D dialog, char* variable );
void        PA_Dial4DGetString     ( PA_Dial4D dialog, char* variable, char* string );
long        PA_Dial4DGetText       ( PA_Dial4D dialog, char* variable, char* text );
PA_Variable PA_Dial4DGetVariable   ( PA_Dial4D dialog, char* variable );

void PA_Dial4DSetLong              ( PA_Dial4D dialog, char* variable, long value );
void PA_Dial4DSetReal              ( PA_Dial4D dialog, char* variable, double value );
void PA_Dial4DSetDate              ( PA_Dial4D dialog, char* variable, short day, short month, short year );
void PA_Dial4DSetTime              ( PA_Dial4D dialog, char* variable, long time );
void PA_Dial4DSetString            ( PA_Dial4D dialog, char* variable, char* string );
void PA_Dial4DSetPictureHandle     ( PA_Dial4D dialog, char* variable, PA_Handle picture, PA_PictureInfo info );
void PA_Dial4DSetText              ( PA_Dial4D dialog, char* variable, char* text, long len );
void PA_Dial4DSetVariable          ( PA_Dial4D dialog, char* variable, PA_Variable var );

// arrays 
void PA_Dial4DNewArrayLong         ( PA_Dial4D dialog, char* variable );
void PA_Dial4DNewArrayReal         ( PA_Dial4D dialog, char* variable );
void PA_Dial4DNewArrayString       ( PA_Dial4D dialog, char* variable );
void PA_Dial4DNewArrayPicture      ( PA_Dial4D dialog, char* variable );
void PA_Dial4DNewArrayText         ( PA_Dial4D dialog, char* variable );

long PA_Dial4DGetArraySize         ( PA_Dial4D dialog, char* variable );
void PA_Dial4DDeleteArrayElements  ( PA_Dial4D dialog, char* variable, long position, long amount );

void PA_Dial4DSetArrayTextFromResource      ( PA_Dial4D dialog, char* variable, short resourceID );
void PA_Dial4DSetArrayTextFromTableList     ( PA_Dial4D dialog, char* variable );
void PA_Dial4DSetArrayTextFromFieldList     ( PA_Dial4D dialog, char* variable, short table );
void PA_Dial4DSetArrayTextFromTTR           ( PA_Dial4D dialog, char* variable, char* text, long len );
void PA_Dial4DSetArrayPictureFromResources  ( PA_Dial4D dialog, char* variable, short* idArray, long nb );

void PA_Dial4DSetArrayLong           ( PA_Dial4D dialog, char* variable, long value, long position );
void PA_Dial4DSetArrayReal           ( PA_Dial4D dialog, char* variable, double value, long position );
void PA_Dial4DSetArrayString         ( PA_Dial4D dialog, char* variable, char* string, long position );
void PA_Dial4DSetArrayPicture        ( PA_Dial4D dialog, char* variable, void* picture, long len, PA_PictureInfo info, long position );
void PA_Dial4DSetArrayPictureHandle  ( PA_Dial4D dialog, char* variable, PA_Handle hpicture, PA_PictureInfo info, long position );
void PA_Dial4DSetArrayText           ( PA_Dial4D dialog, char* variable, char* text, long len, long position );

long   PA_Dial4DGetArrayLong         ( PA_Dial4D dialog, char* variable, long indice );
double PA_Dial4DGetArrayReal         ( PA_Dial4D dialog, char* variable, long indice );
void   PA_Dial4DGetArrayString       ( PA_Dial4D dialog, char* variable, char* string, long indice );
long   PA_Dial4DGetArrayText         ( PA_Dial4D dialog, char* variable, char* text, long position );

long PA_Dial4DFindArrayLong          ( PA_Dial4D dialog, char* variable, long value, long startPosition );
long PA_Dial4DFindArrayReal          ( PA_Dial4D dialog, char* variable, double value, long startPosition );

// context switch from process variables to dialog variables
void* PA_Dial4DSaveVariables         ( PA_Dial4D dialog );
void  PA_Dial4DRestoreVariables      ( PA_Dial4D dialog, void* env );

// update modified variables in dialog
void  PA_Dial4DBeginUpdateVariables  ( PA_Dial4D dialog );
void  PA_Dial4DEndUpdateVariables    ( PA_Dial4D dialog );

// external areas
void  PA_Dial4DSetAreaHandler        ( PA_Dial4D dialog, char* variable, void* handler, void* privateData );
void  PA_Dial4DRedrawExternalArea    ( PA_Dial4D dialog, char* variable );

// user datas
void  PA_Dial4DSetUserData           ( PA_Dial4D dialog, void* userData );
void* PA_Dial4DGetUserData           ( PA_Dial4D dialog );

// pages
void  PA_Dial4DGotoPage              ( PA_Dial4D dialog, short page );
short PA_Dial4DGetCurrentPage        ( PA_Dial4D dialog );

// objects
void  PA_Dial4DGetVariableRect       ( PA_Dial4D dialog, char* variable, PA_Rect* rect );
void  PA_Dial4DSetVariableRect       ( PA_Dial4D dialog, char* variable, PA_Rect rect );
void  PA_Dial4DShowHideVariable      ( PA_Dial4D dialog, char* variable, char visible );
void  PA_Dial4DShowHideObject        ( PA_Dial4D dialog, char* objectName, char visible );
void  PA_Dial4DSetEnterable          ( PA_Dial4D dialog, char* variable, char enterable );
void  PA_Dial4DSetEnable             ( PA_Dial4D dialog, char* variable, char enabled );
void  PA_Dial4DSetMin                ( PA_Dial4D dialog, char* variable, double minValue );
void  PA_Dial4DSetMinDate            ( PA_Dial4D dialog, char* variable, short day, short month, short year );
void  PA_Dial4DSetMax                ( PA_Dial4D dialog, char* variable, double maxValue );
void  PA_Dial4DSetMaxDate            ( PA_Dial4D dialog, char* variable, short day, short month, short year );
void  PA_Dial4DSet3StatesCheckBox    ( PA_Dial4D dialog, char* variable, char state );
void  PA_Dial4DSetEntryFilter        ( PA_Dial4D dialog, char* variable, char* entryFilter );
void  PA_Dial4DSetDisplayFormat      ( PA_Dial4D dialog, char* variable, char* displayFormat );
void  PA_Dial4DSetObjectTitle        ( PA_Dial4D dialog, char* objectName, char* title );
void  PA_Dial4DUpdateObjectTitle     ( PA_Dial4D dialog, char* variable );
void  PA_Dial4DGetLastObject         ( PA_Dial4D dialog, char* objectName );
void  PA_Dial4DGotoVariable          ( PA_Dial4D dialog, char* variable );
void  PA_Dial4DHighlightText         ( PA_Dial4D dialog, char* variable, short startSelection, short endSelection );

// hlists
void  PA_Dial4DNewTableFieldHList          ( PA_Dial4D dialog, char* variable, short table, char showRelatedTables, char showRelatedFields, char useIcons, char sorted );
void  PA_Dial4DGetTableFieldHListCurrent   ( PA_Dial4D dialog, char* variable, short* table, short* field );
void  PA_Dial4DSetTableFieldHListCurrent   ( PA_Dial4D dialog, char* variable, short table, short field );
void  PA_Dial4DDisposeHList                ( PA_Dial4D dialog, char* variable );

// drag and drop
void  PA_Dial4DGetDragInfo                 ( PA_Dial4D dialog, char* dropVariable, short* dropX, short* dropY, PA_Dial4D* dragDialog, char* dragVariable, short* dragX, short* dragY );

// windows
PA_Dial4D    PA_Dial4DDial4DFromWindow     ( PA_WindowRef window );
PA_WindowRef PA_Dial4DWindowFromDial4D     ( PA_Dial4D dialog );
void         PA_Dial4DAllowXResize         ( PA_Dial4D dialog, char allowResize );
void         PA_Dial4DAllowYResize         ( PA_Dial4D dialog, char allowResize );
void         PA_Dial4DGetWindowMinMaxInfo  ( PA_Dial4D dialog, long* minXresize, long* maxXresize, long* minYresize, long* maxYresize );
void         PA_Dial4DSetWindowSize        ( PA_Dial4D dialog, long width, long height );

#ifdef __cplusplus
}
#endif

#endif
