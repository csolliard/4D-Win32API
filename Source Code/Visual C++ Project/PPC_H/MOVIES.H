/************************************************************

Created: Wednesday, August 14, 1991 at 12:23 PM
 Movies.h
 C Interface to the Macintosh Libraries


  Copyright Apple Computer, Inc. 1991
  All rights reserved


************************************************************/


#ifndef __MOVIES__
#define __MOVIES__

#ifndef __QUICKDRAW__
#if LONG_FILENAMES
#include <Quickdraw.H>
#else
#include <Quickdra.H>
#endif
#endif

#ifndef __QDOFFSCREEN__
#if LONG_FILENAMES
#include <QDOffscreen.H>
#else
#include <QDOffscr.H>
#endif
#endif

#ifndef __EVENTS__
#include <Events.H>
#endif

#ifndef __ALIASES__
#include <Aliases.H>
#endif

#ifndef __COMPONENTS__
#if LONG_FILENAMES
#include <Components.H>
#else
#include <Componen.H>
#endif
#endif

#ifndef __IMAGECOMPRESSION__
#if LONG_FILENAMES
#include <ImageCompression.H>
#else
#include <ImageCom.H>
#endif
#endif


#define gestaltQuickTime OST_qtim

#define MovieFileType OST_MooV

#define VideoMediaType OST_vide
#define SoundMediaType OST_soun

#define DoTheRightThing 0L

#if 0
// These are enums in Sound.h  causing compile error and
// were not in the original MAC version of Movies.h,
// include Sound.h to reference these.  RHK 07/06/95
#define kFullVolume 0x100			/* 8.8 format */
#define kNoVolume 0
#endif

struct MovieRecord {
 long data[1];
};

typedef struct MovieRecord MovieRecord;
typedef MovieRecord *Movie;

struct TrackRecord {
 long data[1];
};

typedef struct TrackRecord TrackRecord;
typedef TrackRecord *Track;

struct MediaRecord {
 long data[1];
};

typedef struct MediaRecord MediaRecord;
typedef MediaRecord *Media;

struct UserDataRecord {
 long data[1];
};

typedef struct UserDataRecord UserDataRecord;
typedef UserDataRecord *UserData;

struct TrackEditStateRecord {
 long data[1];
};

typedef struct TrackEditStateRecord TrackEditStateRecord;
typedef TrackEditStateRecord *TrackEditState;

struct MovieEditStateRecord {
 long data[1];
};

typedef struct MovieEditStateRecord MovieEditStateRecord;
typedef MovieEditStateRecord *MovieEditState;


typedef long TimeValue;
typedef long TimeScale;

struct TimeBaseRecord {
 long data[1];
};

typedef struct TimeBaseRecord TimeBaseRecord;
typedef TimeBaseRecord *TimeBase;

struct CallBackRecord {
 long data[1];
};

typedef struct CallBackRecord CallBackRecord;
typedef CallBackRecord *QTCallBack;


struct Int64 {
#if WINVER	// RB 5/6/94: QTW reverses these because of Intel byte order
 long lo;
 long hi;
#else
 long hi;
 long lo;
#endif
};

typedef struct Int64 Int64;

struct TimeRecord {
 Int64 value;
 TimeScale scale;
 TimeBase base;
};

typedef struct TimeRecord TimeRecord;

struct SampleDescription {
 long descSize;
 long dataFormat;
 long resvd1;
 short resvd2;
 short dataRefIndex;
};

typedef struct SampleDescription SampleDescription;
typedef SampleDescription *SampleDescriptionPtr, **SampleDescriptionHandle;

struct SoundDescription {
 long descSize;
 long dataFormat;
 long resvd1;
 short resvd2;
 short dataRefIndex;
 short version;
 short revlevel;
 long vendor;
 short numChannels;
 short sampleSize;
 short compressionID;
 short packetSize;
 Fixed sampleRate;
};

typedef struct SoundDescription SoundDescription;
typedef SoundDescription *SoundDescriptionPtr, **SoundDescriptionHandle;


enum {movieProgressOpen = 0,movieProgressUpdatePercent = 1,movieProgressClose = 2};
typedef unsigned char movieProgressMessages;

enum {progressOpFlatten = 1,progressOpInsertTrackSegment = 2,progressOpInsertMovieSegment = 3,
 progressOpPaste = 4,progressOpAddMovieSelection = 5,progressOpCopy = 6,
 progressOpCut = 7,progressOpLoadMovieIntoRam = 8,progressOpLoadTrackIntoRam = 9,
	progressOpLoadMediaIntoRam = 10};
typedef unsigned char movieProgressOperations;

enum {mediaQualityDraft = 0x0000, mediaQualityNormal = 0x0040, mediaQualityBetter = 0x0080,
	mediaQualityBest = 0x00C0};

typedef OSErr (pascal * MovieProgressProcPtr)	(Movie m, short message, short whatOperation, Fixed percentDone, long refCon);        
typedef OSErr (pascal * MovieRgnCoverProc)		(Movie m, RgnHandle changedRgn, long refcon);  

typedef Handle MediaInformationHandle;
typedef ComponentInstance MediaHandler;
typedef Component MediaHandlerComponent;
typedef ComponentInstance DataHandler;
typedef Component DataHandlerComponent;
typedef ComponentResult HandlerError;
typedef ComponentInstance MovieController;

typedef void (pascal * ErrorProcPtr)(OSErr theErr, long refcon);        
typedef Boolean (pascal * MoviePreviewCallOutProc)(long refcon);        

enum {newMovieActive = 1<<0,newMovieDontResolveDataRefs = 1<<1,newMovieDontAskUnresolvedDataRefs = 1<<2};
typedef unsigned char newMovieFlagsEnum;

enum {trackUsageInMovie = 1<<1,trackUsageInPreview = 1<<2,trackUsageInPoster = 1<<3};
typedef unsigned char trackUsageEnum;

enum {mediaSampleNotSync = 1};
typedef unsigned char mediaSampleFlagsEnum;

enum {nextTimeMediaSample = 1<<0,nextTimeMediaEdit = 1<<1,nextTimeTrackEdit = 1<<2,
 nextTimeSyncSample = 1<<3,nextTimeEdgeOK = 1<<14,nextTimeIgnoreActiveSegment = 1<<15};
typedef unsigned short nextTimeFlagsEnum;

#define createMovieFileDeleteCurFile	(1L << 31)
#define createMovieFileDontCreateMovie	(1L << 30)
#define createMovieFileDontOpenFile	(1L << 29)

typedef unsigned long createMovieFileFlagsEnum;

enum {flattenAddMovieToDataFork = 1L<<0,flattenActiveTracksOnly = 1L<<2,
 flattenDontInterleaveFlatten = 1L<<3};
typedef unsigned char movieFlattenFlagsEnum;

enum {movieScrapDontZeroScrap = 1<<0,movieScrapOnlyPutMovie = 1<<1};
typedef unsigned char movieScrapFlagsEnum;

enum {hintsScrubMode = 1<<0};
typedef unsigned char playHintsEnum;






#ifdef __cplusplus
extern "C" {
#endif
/*  Initialization Routines  */
F_PASCAL( OSErr ) EnterMovies(void); 
F_PASCAL( void ) ExitMovies(void); 

/*  error Routines  */
F_PASCAL( OSErr ) GetMoviesError(void); 
F_PASCAL( void ) ClearMoviesStickyError(void); 
F_PASCAL( OSErr ) GetMoviesStickyError(void); 

F_PASCAL( void ) SetMoviesErrorProc(ErrorProcPtr errProc,long refcon); 

F_PASCAL( ComponentResult ) GetTrackStatus(Track t); 
F_PASCAL( ComponentResult ) GetMovieStatus(Movie m,Track *firstProblemTrack); 

/*  Playback Routines  */
F_PASCAL( void ) MoviesTask(Movie m,long maxMilliSecToUse); 

F_PASCAL( void ) StartMovie(Movie m); 
F_PASCAL( void ) StopMovie(Movie m); 
F_PASCAL( void ) GoToBeginningOfMovie(Movie m); 
F_PASCAL( void ) GoToEndOfMovie(Movie m); 
F_PASCAL( Boolean ) IsMovieDone(Movie m); 

F_PASCAL( Boolean ) GetMoviePreviewMode(Movie m); 
F_PASCAL( void ) SetMoviePreviewMode(Movie m,Boolean usePreview); 
F_PASCAL( void ) ShowMoviePoster(Movie m); 

F_PASCAL( void ) GetMovieGWorld(Movie m,CGrafPtr *port,GDHandle *gdh); 
F_PASCAL( void ) SetMovieGWorld(Movie m,CGrafPtr port,GDHandle gdh); 

F_PASCAL( OSErr ) PrerollMovie(Movie m,TimeValue time,Fixed Rate); 
F_PASCAL( OSErr ) LoadMovieIntoRam(Movie m,TimeValue time,TimeValue duration,
 long flags); 
F_PASCAL( OSErr ) LoadTrackIntoRam(Track t,TimeValue time,TimeValue duration,
 long flags); 
F_PASCAL( OSErr ) LoadMediaIntoRam(Media m,TimeValue time,TimeValue duration,
 long flags); 

F_PASCAL( void ) SetMovieActive(Movie m,Boolean active); 
F_PASCAL( Boolean ) GetMovieActive(Movie m); 

F_PASCAL( void ) PlayMoviePreview(Movie m,MoviePreviewCallOutProc callOutProc,
 long refcon); 

/*  Movie PICT Routines  */
F_PASCAL( PicHandle ) GetMoviePict(Movie m,TimeValue time); 
F_PASCAL( PicHandle ) GetTrackPict(Track t,TimeValue time); 

F_PASCAL( PicHandle ) GetMoviePosterPict(Movie m); 

/*  movie TimeBase Routines  */
F_PASCAL( TimeBase ) GetMovieTimeBase(Movie m); 

F_PASCAL( void ) SetMovieMasterTimeBase(Movie m,TimeBase tb,const TimeRecord *slaveZero); 
F_PASCAL( void ) SetMovieMasterClock(Movie m,Component clockMeister,const TimeRecord *slaveZero); 

/*  spatial movie Routines  */
F_PASCAL( void ) GetMovieBox(Movie m,Rect *boxRect); 
F_PASCAL( void ) SetMovieBox(Movie m,const Rect *boxRect); 

/*  movie display clip  */
F_PASCAL( RgnHandle ) GetMovieDisplayClipRgn(Movie m); 
F_PASCAL( void ) SetMovieDisplayClipRgn(Movie m,RgnHandle theClip); 

/*  movie clip  */
F_PASCAL( RgnHandle ) GetMovieClipRgn(Movie m); 
F_PASCAL( void ) SetMovieClipRgn(Movie m,RgnHandle theClip); 

/*  track clip  */
F_PASCAL( RgnHandle ) GetTrackClipRgn(Track t); 
F_PASCAL( void ) SetTrackClipRgn(Track t,RgnHandle theClip); 

/*  bounds in display space  */
F_PASCAL( RgnHandle ) GetMovieDisplayBoundsRgn(Movie m); 
F_PASCAL( RgnHandle ) GetTrackDisplayBoundsRgn(Track t); 

/*  bounds in movie space  */
F_PASCAL( RgnHandle ) GetMovieBoundsRgn(Movie m); 
F_PASCAL( RgnHandle ) GetTrackMovieBoundsRgn(Track t); 

/*  bounds in track space  */
F_PASCAL( RgnHandle ) GetTrackBoundsRgn(Track t); 

/*  movie poster box  */
F_PASCAL( void ) GetPosterBox(Movie m,Rect *boxRect); 
F_PASCAL( void ) SetPosterBox(Movie m,const Rect *boxRect); 

/*  movie/track bounds over a range of time  */
F_PASCAL( RgnHandle ) GetMovieSegmentDisplayBoundsRgn(Movie m,TimeValue time,
 TimeValue duration); 
F_PASCAL( RgnHandle ) GetTrackSegmentDisplayBoundsRgn(Track t,TimeValue time,
 TimeValue duration); 

F_PASCAL( void ) SetMovieCoverProcs(Movie m,MovieRgnCoverProc uncoverProc,MovieRgnCoverProc coverProc,
 long refCon); 

/*  mattes - always in track space  */
F_PASCAL( PixMapHandle ) GetTrackMatte(Track t); 
F_PASCAL( void ) SetTrackMatte(Track t,PixMapHandle theMatte); 
F_PASCAL( void ) DisposeMatte(PixMapHandle theMatte); 

/*  movie creation Routines  */
F_PASCAL( OSErr ) NewMovieFromHandle(Movie *m,Handle h,short newMovieFlags,Boolean *dataRefWasChanged); 
F_PASCAL( OSErr ) PutMovieIntoHandle(Movie m,Handle h); 

F_PASCAL( Movie ) NewMovie(long newMovieFlags); 
F_PASCAL( void ) DisposeMovie(Movie m); 

/*  Movie Routines  */
F_PASCAL( unsigned long ) GetMovieCreationTime(Movie m); 
F_PASCAL( unsigned long ) GetMovieModificationTime(Movie m); 

F_PASCAL( TimeScale ) GetMovieTimeScale(Movie m); 
F_PASCAL( void ) SetMovieTimeScale(Movie m,TimeScale timeScale); 

F_PASCAL( TimeValue ) GetMovieDuration(Movie m); 

F_PASCAL( Fixed ) GetMovieRate(Movie m); 
F_PASCAL( void ) SetMovieRate(Movie m,Fixed rate); 

F_PASCAL( Fixed ) GetMoviePreferredRate(Movie m); 
F_PASCAL( void ) SetMoviePreferredRate(Movie m,Fixed rate); 

F_PASCAL( short ) GetMoviePreferredVolume(Movie m); 
F_PASCAL( void ) SetMoviePreferredVolume(Movie m,short volume); 

F_PASCAL( short ) GetMovieVolume(Movie m); 
F_PASCAL( void ) SetMovieVolume(Movie m,short volume); 

F_PASCAL( void ) GetMovieMatrix(Movie m,MatrixRecord *matrix); 
F_PASCAL( void ) SetMovieMatrix(Movie m,const MatrixRecord *matrix); 

F_PASCAL( void ) GetMoviePreviewTime(Movie m,TimeValue *previewTime,TimeValue *previewDuration); 
F_PASCAL( void ) SetMoviePreviewTime(Movie m,TimeValue previewTime,TimeValue previewDuration); 

F_PASCAL( TimeValue ) GetMoviePosterTime(Movie m); 
F_PASCAL( void ) SetMoviePosterTime(Movie m,TimeValue posterTime); 

F_PASCAL( void ) GetMovieSelection(Movie m,TimeValue *selectionTime,TimeValue *selectionDuration); 
F_PASCAL( void ) SetMovieSelection(Movie m,TimeValue selectionTime,TimeValue selectionDuration); 

F_PASCAL( void ) SetMovieActiveSegment(Movie m,TimeValue startTime,TimeValue duration); 
F_PASCAL( void ) GetMovieActiveSegment(Movie m,TimeValue *startTime,TimeValue *duration); 

F_PASCAL( TimeValue ) GetMovieTime(Movie m,TimeRecord *currentTime); 
F_PASCAL( void ) SetMovieTime(Movie m,const TimeRecord *newtime); 
F_PASCAL( void ) SetMovieTimeValue(Movie m,TimeValue newtime); 

F_PASCAL( UserData ) GetMovieUserData(Movie m); 

F_PASCAL( void ) SetMoviePlayHints(Movie movie,long flags,long flagsMask);

/*  Movie/Track/Media finding Routines  */
F_PASCAL( long ) GetMovieTrackCount(Movie m); 
F_PASCAL( Track ) GetMovieTrack(Movie m,long trackID); 

F_PASCAL( Track ) GetMovieIndTrack(Movie m,long index); 

F_PASCAL( long ) GetTrackID(Track t); 

F_PASCAL( Movie ) GetTrackMovie(Track t); 

F_PASCAL( Media ) GetTrackMedia(Track t); 
F_PASCAL( Track ) GetMediaTrack(Media m); 

/*  Track creation Routines  */
F_PASCAL( Track ) NewMovieTrack(Movie m,Fixed width,Fixed height,short trackVolume); 
F_PASCAL( void ) DisposeMovieTrack(Track t); 

/*  Track Routines  */
F_PASCAL( unsigned long ) GetTrackCreationTime(Track t); 
F_PASCAL( unsigned long ) GetTrackModificationTime(Track t); 

F_PASCAL( Boolean ) GetTrackEnabled(Track t); 
F_PASCAL( void ) SetTrackEnabled(Track t,Boolean isEnabled); 

F_PASCAL( long ) GetTrackUsage(Track t); 
F_PASCAL( void ) SetTrackUsage(Track t,long usage); 

F_PASCAL( TimeValue ) GetTrackDuration(Track t); 

F_PASCAL( TimeValue ) GetTrackOffset(Track t); 
F_PASCAL( void ) SetTrackOffset(Track t,TimeValue offset); 

F_PASCAL( short ) GetTrackLayer(Track t); 
F_PASCAL( void ) SetTrackLayer(Track t,short layer); 

F_PASCAL( Track ) GetTrackAlternate(Track t); 
F_PASCAL( void ) SetTrackAlternate(Track t,Track alternateT); 

F_PASCAL( short ) GetTrackVolume(Track t); 
F_PASCAL( void ) SetTrackVolume(Track t,short volume); 

F_PASCAL( void ) GetTrackMatrix(Track t,MatrixRecord *matrix); 
F_PASCAL( void ) SetTrackMatrix(Track t,const MatrixRecord *matrix); 

F_PASCAL( void ) GetTrackDimensions(Track t,Fixed *width,Fixed *height); 
F_PASCAL( void ) SetTrackDimensions(Track t,Fixed width,Fixed height); 

F_PASCAL( UserData ) GetTrackUserData(Track t); 

/*  Media creation Routines  */
F_PASCAL( Media ) NewTrackMedia(Track t,OSType mediaType,TimeScale timeScale,
 Handle dataRef,OSType dataRefType); 
F_PASCAL( void ) DisposeTrackMedia(Media m); 

/*  Media Routines  */
F_PASCAL( unsigned long ) GetMediaCreationTime(Media m); 
F_PASCAL( unsigned long ) GetMediaModificationTime(Media m); 

F_PASCAL( TimeScale ) GetMediaTimeScale(Media m); 
F_PASCAL( void ) SetMediaTimeScale(Media m,TimeScale timeScale); 

F_PASCAL( TimeValue ) GetMediaDuration(Media m); 

F_PASCAL( short ) GetMediaLanguage(Media m); 
F_PASCAL( void ) SetMediaLanguage(Media m,short language); 

F_PASCAL( short ) GetMediaQuality(Media m); 
F_PASCAL( void ) SetMediaQuality(Media m,short quality); 

F_PASCAL( UserData ) GetMediaUserData(Media m); 

F_PASCAL( void ) SetMediaPlayHints(Movie movie,long flags,long flagsMask);

/*  Media Handler Routines  */
F_PASCAL( MediaHandler ) GetMediaHandler(Media m); 
F_PASCAL( OSErr ) SetMediaHandler(Media m,MediaHandlerComponent mH); 

F_PASCAL( void ) GetMediaHandlerDescription(Media m,OSType *mediaType, Str255 creatorName,
 OSType *creatorManufacturer); 

#ifdef __cplusplus
}
#endif
enum {dataRefSelfReference = 1<<0,dataRefWasNotResolved = 1<<1};
typedef unsigned char dataRefAttributesFlags;


#ifdef __cplusplus
extern "C" {
#endif
/*  Media dataRef Routines  */
F_PASCAL( OSErr ) GetMediaDataRef(Media m,short index,Handle *dataRef,OSType *dataRefType,
 long *dataRefAttributes); 
F_PASCAL( OSErr ) AddMediaDataRef(Media m,short *index,Handle dataRef,OSType dataRefType); 

F_PASCAL( OSErr ) GetMediaDataRefCount(Media m,short *count); 

/*  Media data handler Routines  */
F_PASCAL( DataHandler ) GetMediaDataHandler(Media m,short index); 
F_PASCAL( OSErr ) SetMediaDataHandler(Media m,short index,DataHandlerComponent dataHandler); 

F_PASCAL( void ) GetMediaDataHandlerDescription(Media m,short index,OSType *dhType,
 Str255 creatorName,OSType *creatorManufacturer); 

/*  Media sample Routines  */
F_PASCAL( long ) GetMediaSampleDescriptionCount(Media m); 
F_PASCAL( void ) GetMediaSampleDescription(Media m,long index,SampleDescriptionHandle descH); 
F_PASCAL( long ) GetMediaSampleCount(Media m); 
F_PASCAL( void ) SampleNumToMediaTime(Media m,long logicalSampleNum,TimeValue *sampleTime,
 TimeValue *sampleDuration); 
F_PASCAL( void ) MediaTimeToSampleNum(Media m,TimeValue time,long *sampleNum,
 TimeValue *sampleTime,TimeValue *sampleDuration); 

F_PASCAL( OSErr ) AddMediaSample(Media m,Handle dataIn,long inOffset,unsigned long size,
 TimeValue durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long numberOfSamples,short sampleFlags,TimeValue *sampleTime); 
F_PASCAL( OSErr ) AddMediaSampleReference(Media m,long dataOffset,unsigned long size,
 TimeValue durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long numberOfSamples,short sampleFlags,TimeValue *sampleTime); 
F_PASCAL( OSErr ) GetMediaSample(Media m,Handle dataOut,long maxSizeToGrow,long *size,
 TimeValue time,TimeValue *sampleTime,TimeValue *durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long *sampleDescriptionIndex,long maxNumberOfSamples,long *numberOfSamples,
 short *sampleFlags); 
F_PASCAL( OSErr ) GetMediaSampleReference(Media m,long *dataOffset,long *size,
 TimeValue time,TimeValue *sampleTime,TimeValue *durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long *sampleDescriptionIndex,long maxNumberOfSamples,long *numberOfSamples,
 short *sampleFlags); 

/*  High level editing Routines  */
F_PASCAL( Movie ) CutMovieSelection(Movie m); 
F_PASCAL( Movie ) CopyMovieSelection(Movie m); 
F_PASCAL( void ) PasteMovieSelection(Movie m,Movie src); 
F_PASCAL( void ) AddMovieSelection(Movie m,Movie src); 
F_PASCAL( void ) ClearMovieSelection(Movie m); 

/*  Middle-level Editing Routines  */
F_PASCAL( OSErr ) InsertTrackSegment(Track srcTrack,Track dstTrack,TimeValue srcIn,
 TimeValue srcDuration,TimeValue dstIn); 
F_PASCAL( OSErr ) InsertMovieSegment(Movie srcMovie,Movie dstMovie,TimeValue srcIn,
 TimeValue srcDuration,TimeValue dstIn); 
F_PASCAL( OSErr ) InsertEmptyTrackSegment(Track dstTrack,TimeValue dstIn,TimeValue dstDuration); 
F_PASCAL( OSErr ) InsertEmptyMovieSegment(Movie dstMovie,TimeValue dstIn,TimeValue dstDuration); 
F_PASCAL( OSErr ) DeleteTrackSegment(Track t,TimeValue dstIn,TimeValue duration); 
F_PASCAL( OSErr ) DeleteMovieSegment(Movie m,TimeValue dstIn,TimeValue duration); 
F_PASCAL( OSErr ) ScaleTrackSegment(Track t,TimeValue dstIn,TimeValue oldDuration,
 TimeValue newDuration); 
F_PASCAL( OSErr ) ScaleMovieSegment(Movie m,TimeValue dstIn,TimeValue oldDuration,
 TimeValue newDuration); 

F_PASCAL( OSErr ) CopyTrackSettings(Track srcTrack,Track dstTrack); 
F_PASCAL( OSErr ) CopyMovieSettings(Movie srcMovie,Movie dstMovie); 

/*  low-level Editing Routines  */
F_PASCAL( OSErr ) InsertMediaIntoTrack(Track t,TimeValue trackStart,TimeValue mediaTime,
 TimeValue mediaDuration,Fixed mediaRate); 

F_PASCAL( OSErr ) BeginMediaEdits(Media m); 
F_PASCAL( OSErr ) EndMediaEdits(Media m); 

/*  movie & track edit state Routines  */
F_PASCAL( MovieEditState ) NewMovieEditState(Movie m); 
F_PASCAL( OSErr ) UseMovieEditState(Movie m,MovieEditState toState); 
F_PASCAL( OSErr ) DisposeMovieEditState(MovieEditState state); 

F_PASCAL( TrackEditState ) NewTrackEditState(Track t); 
F_PASCAL( OSErr ) UseTrackEditState(Track t,TrackEditState state); 
F_PASCAL( OSErr ) DisposeTrackEditState(TrackEditState state); 

/*  Miscellaneous Routines  */
F_PASCAL( TimeValue ) TrackTimeToMediaTime(TimeValue value,Track t); 

F_PASCAL( long ) GetMovieDataSize(Movie m,TimeValue startTime,TimeValue duration); 
F_PASCAL( long ) GetTrackDataSize(Track t,TimeValue startTime,TimeValue duration); 
F_PASCAL( long ) GetMediaDataSize(Media m,TimeValue startTime,TimeValue duration); 

F_PASCAL( Boolean ) PtInMovie(Movie m,Point pt); 
F_PASCAL( Boolean ) PtInTrack(Track t,Point pt); 

F_PASCAL( OSErr ) UpdateMovie(Movie m); 

/*  Alternate Group Selection Routines  */
F_PASCAL( void ) SetMovieLanguage(Movie m,long language); 

F_PASCAL( void ) SetAutoTrackAlternatesEnabled(Movie m,Boolean enable); 
F_PASCAL( void ) SelectMovieAlternates(Movie m); 

/*  UserData Routines  */
F_PASCAL( OSErr ) GetUserData(UserData u,Handle data, OSType udType,long index); 
F_PASCAL( OSErr ) AddUserData(UserData u,Handle data,OSType udType); 
F_PASCAL( OSErr ) RemoveUserData(UserData u,OSType udType,long index); 
F_PASCAL( OSType ) GetNextUserDataType(UserData u, OSType udType); 
F_PASCAL( short ) CountUserDataType(UserData u,OSType udType); 
F_PASCAL( OSErr ) AddUserDataText(UserData u,Handle data,OSType udType,long index,
 short itlRegionTag); 
F_PASCAL( OSErr ) GetUserDataText(UserData u,Handle data,OSType udType,long index,
 short itlRegionTag); 
F_PASCAL( OSErr ) RemoveUserDataText(UserData u,OSType udType,long index,short itlRegionTag); 

/*  interesting time Routines  */
F_PASCAL( void ) GetMediaNextInterestingTime(Media m,short interestingTimeFlags,
 TimeValue time,Fixed rate,TimeValue *interestingTime,TimeValue *interestingDuration); 
F_PASCAL( void ) GetTrackNextInterestingTime(Track t,short interestingTimeFlags,
 TimeValue time,Fixed rate,TimeValue *interestingTime,TimeValue *interestingDuration); 
F_PASCAL( void ) GetMovieNextInterestingTime(Movie m,short interestingTimeFlags,
 short numMediaTypes,const OSType *whichMediaTypes,TimeValue time,Fixed rate,
 TimeValue *interestingTime,TimeValue *interestingDuration); 

/*  movie file Routines  */
F_PASCAL( OSErr ) CreateMovieFile(const FSSpec *fileSpec,OSType creator,ScriptCode scriptTag,
 long createMovieFileFlags,short *resRefNum,Movie *newMovie); 
F_PASCAL( OSErr ) DeleteMovieFile(const FSSpec *fileSpec); 

F_PASCAL( OSErr ) OpenMovieFile(const FSSpec *fileSpec,short *resRefNum, char perms); 
F_PASCAL( OSErr ) CloseMovieFile(short resRefNum); 
#ifdef __cplusplus
}
#endif
enum {

 movieInDataForkResID = -1			/* magic res ID */

};

#ifdef __cplusplus
extern "C" {
#endif
F_PASCAL( OSErr ) NewMovieFromFile(Movie *m,short resRefNum,short *resId, StringPtr resName,
 short newMovieFlags,Boolean *dataRefWasChanged); 

F_PASCAL( OSErr ) AddMovieResource(Movie m,short resRefNum,short *resId,const StringPtr resName); 
F_PASCAL( OSErr ) RemoveMovieResource(short resRefNum,short resId); 
F_PASCAL( OSErr ) UpdateMovieResource(Movie m,short resRefNum,short resId,const StringPtr resName); 

F_PASCAL( Boolean ) HasMovieChanged(Movie m); 
F_PASCAL( void ) ClearMovieChanged(Movie m); 

F_PASCAL( void ) FlattenMovie(Movie m,long movieFlattenFlags,const FSSpec *theFile,
 OSType creator,ScriptCode scriptTag,long createMovieFileFlags,short *resId,
 const StringPtr resName); 
F_PASCAL( Movie ) FlattenMovieData(Movie m,long movieFlattenFlags,const FSSpec *theFile,
 OSType creator,ScriptCode scriptTag,long createMovieFileFlags); 
F_PASCAL( void ) SetMovieProgressProc(Movie m,MovieProgressProcPtr p,long refCon); 

F_PASCAL( OSErr ) PutMovieOnScrap(Movie m,long movieScrapFlags); 
F_PASCAL( Movie ) NewMovieFromScrap(long newMovieFlags); 

/*  Video Media Routines  */
F_PASCAL( HandlerError ) GetVideoMediaGraphicsMode(MediaHandler mh,long *graphicsMode,
 RGBColor *opColor); 
F_PASCAL( HandlerError ) SetVideoMediaGraphicsMode(MediaHandler mh,long graphicsMode,
 const RGBColor *opColor); 

/*  Sound Media Routines  */
F_PASCAL( HandlerError ) GetSoundMediaBalance(MediaHandler mh,short *balance); 
F_PASCAL( HandlerError ) SetSoundMediaBalance(MediaHandler mh,short balance); 

#ifdef __cplusplus
}
#endif

#ifndef __ERRORS__
enum {
 couldNotResolveDataRef = -2000,
 badImageDescription = -2001,
 badPublicMovieAtom = -2002,
 cantFindHandler = -2003,
 cantOpenHandler = -2004,
 badComponentType = -2005,
 noMediaHandler = -2006,
 noDataHandler = -2007,
 invalidMedia = -2008,
 invalidTrack = -2009,
 invalidMovie = -2010,
 invalidSampleTable = -2011,
 invalidDataRef = -2012,
 invalidHandler = -2013,
 invalidDuration = -2014,
 invalidTime = -2015,
 cantPutPublicMovieAtom = -2016,
 badEditList = -2017,
 mediaTypesDontMatch = -2018,
 progressProcAborted = -2019
};
enum {
 movieToolboxUnitialized = -2020,
 wfFileNotFound = -2021,
 cantCreateSingleForkFile = -2022,
 invalidEditState = -2023,
 nonMatchingEditState = -2024,
 staleEditState = -2025,
 userDataItemNotFound = -2026,
 maxSizeToGrowTooSmall = -2027,
 badTrackIndex = -2028,
 trackIDNotFound = -2029,
 trackNotInMovie = -2030,
 timeNotInTrack = -2031,
 timeNotInMedia = -2032,
 badEditIndex = -2033,
 internalQuickTimeError = -2034,
 cantEnableTrack = -2035,
 invalidRect = -2036,
 invalidSampleNum = -2037,
 invalidChunkNum = -2038,
 invalidSampleDescIndex = -2039
};
enum {
 invalidChunkCache = -2040,
 invalidSampleDescription = -2041,
 dataNotOpenForRead = -2042,
 dataNotOpenForWrite = -2043,
 dataAlreadyOpenForWrite = -2044,
 dataAlreadyClosed = -2045,
 endOfDataReached = -2046,
 dataNoDataRef = -2047,
 noMovieFound = -2048,
 invalidDataRefContainer = -2049,
 badDataRefIndex = -2050,
 noDefaultDataRef = -2051,
 couldNotUseAnExistingSample = -2052,
 featureUnsupported = -2053
};
#endif

/* TimeBase Routines */
enum {loopTimeBase = 1, palindromeLoopTimeBase = 2};
typedef unsigned char TimeBaseFlags;

enum {
 triggerTimeFwd       = 0x0001,  	/* when curTime exceeds triggerTime going forward */
 triggerTimeBwd       = 0x0002,  	/* when curTime exceeds triggerTime going backwards */
 triggerTimeEither    = 0x0003,  	/* when curTime exceeds triggerTime going either direction */
 triggerRateLT        = 0x0004,  	/* when rate changes to less than trigger value */
 triggerRateGT        = 0x0008,  	/* when rate changes to greater than trigger value */
 triggerRateEqual     = 0x0010,  	/* when rate changes to equal trigger value */
 triggerRateLTE       = triggerRateLT | triggerRateEqual,
 triggerRateGTE       = triggerRateGT | triggerRateEqual,
 triggerRateNotEqual  = triggerRateGT | triggerRateEqual | triggerRateLT,
 triggerRateChange    = 0
};
typedef unsigned char QTCallBackFlags;

enum {timeBaseBeforeStartTime = 1,timeBaseAfterStopTime = 2};
typedef unsigned char TimeBaseStatus;

enum {callBackAtTime = 1,callBackAtRate = 2,callBackAtTimeJump = 3};
#define	callBackAtInterrupt	0x8000

typedef unsigned short QTCallBackType;

typedef void (pascal * QTCallBackProc)(QTCallBack cb,long refCon);

enum {
 qtcbNeedsRateChanges = 1,
 qtcbNeedsTimeChanges = 2
};

/* CallBack equates */
struct QTCallBackHeader {
 long callBackFlags;
 long reserved1;
 char qtPrivate[40];
};

typedef struct QTCallBackHeader QTCallBackHeader;


#ifdef __cplusplus
extern "C" {
#endif
/*  TimeBase Routines  */
F_PASCAL( TimeBase ) NewTimeBase(void); 
F_PASCAL( void ) DisposeTimeBase(TimeBase tb); 

F_PASCAL( TimeValue ) GetTimeBaseTime(TimeBase tb,TimeScale s,TimeRecord *out); 
F_PASCAL( void ) SetTimeBaseTime(TimeBase tb,const TimeRecord *tr); 
F_PASCAL( void ) SetTimeBaseValue(TimeBase tb,TimeValue t,TimeScale s); 

F_PASCAL( Fixed ) GetTimeBaseRate(TimeBase tb); 
F_PASCAL( void ) SetTimeBaseRate(TimeBase tb,Fixed r); 

F_PASCAL( TimeValue ) GetTimeBaseStartTime(TimeBase tb,TimeScale s,TimeRecord *out); 
F_PASCAL( void ) SetTimeBaseStartTime(TimeBase tb,const TimeRecord *tr); 
F_PASCAL( TimeValue ) GetTimeBaseStopTime(TimeBase tb,TimeScale s,TimeRecord *out); 
F_PASCAL( void ) SetTimeBaseStopTime(TimeBase tb,const TimeRecord *tr); 

F_PASCAL( long ) GetTimeBaseFlags(TimeBase tb); 
F_PASCAL( void ) SetTimeBaseFlags(TimeBase tb,long timeBaseFlags); 

F_PASCAL( void ) SetTimeBaseMasterTimeBase(TimeBase slave,TimeBase master,const TimeRecord *slaveZero); 
F_PASCAL( TimeBase ) GetTimeBaseMasterTimeBase(TimeBase tb); 
F_PASCAL( void ) SetTimeBaseMasterClock(TimeBase slave,Component clockMeister,
 const TimeRecord *slaveZero); 
F_PASCAL( ComponentInstance ) GetTimeBaseMasterClock(TimeBase tb); 

F_PASCAL( void ) ConvertTime(TimeRecord *inout,TimeBase newBase); 
F_PASCAL( void ) ConvertTimeScale(TimeRecord *inout,TimeScale newScale); 
F_PASCAL( void ) AddTime(TimeRecord *dst,const TimeRecord *src); 
F_PASCAL( void ) SubtractTime(TimeRecord *dst,const TimeRecord *src); 

F_PASCAL( long ) GetTimeBaseStatus(TimeBase tb,TimeRecord *unpinnedTime); 

F_PASCAL( void ) SetTimeBaseZero(TimeBase tb,TimeRecord *zero); 

/*  CallBack Routines  */
F_PASCAL( QTCallBack ) NewCallBack(TimeBase tb,short cbType); 

F_PASCAL( void ) DisposeCallBack(QTCallBack qtCall); 

F_PASCAL( short ) GetCallBackType(QTCallBack qtCall); 
F_PASCAL( TimeBase ) GetCallBackTimeBase(QTCallBack qtCall); 

F_PASCAL( OSErr ) CallMeWhen(QTCallBack qtCall,QTCallBackProc callBackProc,long refCon,
 long param1,long param2,long param3); 

F_PASCAL( void ) CancelCallBack(QTCallBack cb); 

/*  Clock CallBack support Routines  */
F_PASCAL( OSErr ) AddCallBackToTimeBase(QTCallBack cb); 
F_PASCAL( OSErr ) RemoveCallBackFromTimeBase(QTCallBack cb); 
F_PASCAL( QTCallBack ) GetFirstCallBack(TimeBase tb); 
F_PASCAL( QTCallBack ) GetNextCallBack(QTCallBack cb); 
F_PASCAL( void ) ExecuteCallBack(QTCallBack cb); 
#ifdef __cplusplus
}
#endif



/* Movie Controller Routines */

#define MovieControllerComponentType OST_play

enum {
	mcActionIdle = 1,						// no param
	mcActionDraw = 2,						// param is WindowPtr
	mcActionActivate = 3,					// no param
	mcActionDeactivate = 4,					// no param
	mcActionMouseDown = 5,					// param is pointer to EventRecord
	mcActionKey = 6,						// param is pointer to EventRecord
	mcActionPlay = 8,						// param is Fixed, play rate
	mcActionGoToTime = 12,					// param is TimeRecord
	mcActionSetVolume = 14,					// param is a short
	mcActionGetVolume = 15,					// param is pointer to a short
	mcActionStep = 18,						// param is number of steps (short)
	mcActionSetLooping = 21,				// param is Boolean
	mcActionGetLooping = 22,				// param is pointer to a Boolean
	mcActionSetLoopIsPalindrome = 23,		// param is Boolean
	mcActionGetLoopIsPalindrome = 24,		// param is pointer to a Boolean
	mcActionSetGrowBoxBounds = 25,			// param is a Rect
	mcActionControllerSizeChanged = 26,		// no param
	mcActionSetSelectionBegin = 29,			// param is TimeRecord
	mcActionSetSelectionDuration = 30,		// param is TimeRecord, action only taken on set-duration
	mcActionSetPosterTime = 31,				// param is TimeRecord
	mcActionSetKeysEnabled = 32,			// param is Boolean
	mcActionGetKeysEnabled = 33,			// param is pointer to Boolean
	mcActionSetPlaySelection = 34,			// param is Boolean
	mcActionGetPlaySelection = 35,			// param is pointer to Boolean
	mcActionSetUseBadge = 36,				// param is Boolean
	mcActionGetUseBadge = 37,				// param is pointer to Boolean
	mcActionSetFlags = 38,					// param is long of flags
	mcActionGetFlags = 39,					// param is pointer to a long of flags
	mcActionSetPlayEveryFrame = 40,			// param is Boolean
	mcActionGetPlayEveryFrame = 41,			// param is pointer to Boolean
	mcActionGetPlayRate = 42,				// param is pointer to Fixed
	mcActionShowBalloon = 43,				// param is a pointer to a boolean. set to false to stop balloon
	mcActionBadgeClick = 44					// no param
};
typedef unsigned short mcAction;

enum {mcFlagSuppressMovieFrame = 1<<0,mcFlagSuppressStepButtons = 1<<1,
 mcFlagSuppressSpeakerButton = 1<<2,mcFlagsUseWindowPalette = 1<<3};

typedef Boolean (pascal * MCActionFilter)(MovieController mc,short *action,long param);

enum {mcTopLeftMovie = 1<<0,mcScaleMovieToFit = 1<<1,mcWithBadge = 1<<2,
 mcNotVisible = 1<<3,mcWithFrame = 1<<4};
typedef unsigned char mcCreationFlagsEnum;

enum {mcInfoUndoAvailable = 1<<0,mcInfoCutAvailable = 1<<1,mcInfoCopyAvailable = 1<<2,
 mcInfoPasteAvailable = 1<<3,mcInfoClearAvailable = 1<<4,mcInfoHasSound = 1<<5,
 mcInfoIsPlaying = 1<<6,mcInfoIsLooping = 1<<7,mcInfoIsInPalindrome = 1<<8,
 mcInfoEditingEnabled = 1<<9};
typedef unsigned short mcInfoFlagsEnum;


#ifdef __cplusplus
extern "C" {
#endif
/*  target management  */
F_PASCAL( MovieController ) NewMovieController(Movie m,const Rect *movieRect,
 long controllerCreationFlags); 
F_PASCAL( void ) DisposeMovieController(MovieController mc); 

F_PASCAL( ComponentResult ) MCSetMovie(MovieController mc,Movie m,WindowPtr movieWindow,
 Point where); 
F_PASCAL( Movie ) MCGetMovie(MovieController mc); 
F_PASCAL( ComponentResult ) MCRemoveMovie(MovieController mc); 

/*  event handling etc.  */
F_PASCAL( ComponentResult ) MCIsPlayerEvent(MovieController mc,const EventRecord *e); 
F_PASCAL( ComponentResult ) MCSetActionFilter(MovieController mc,MCActionFilter filter); 
F_PASCAL( ComponentResult ) MCDoAction(MovieController mc,short action,void *params); 

/*  state type things  */
F_PASCAL( ComponentResult ) MCSetControllerAttached(MovieController mc,Boolean attach); 
F_PASCAL( ComponentResult ) MCIsControllerAttached(MovieController mc); 
F_PASCAL( ComponentResult ) MCSetControllerPort(MovieController mc,CGrafPtr gp); 
F_PASCAL( CGrafPtr ) MCGetControllerPort(MovieController mc); 

F_PASCAL( ComponentResult ) MCSetVisible(MovieController mc,Boolean show); 
F_PASCAL( ComponentResult ) MCGetVisible(MovieController mc); 

F_PASCAL( ComponentResult ) MCGetControllerBoundsRect(MovieController mc,Rect *bounds); 
F_PASCAL( ComponentResult ) MCSetControllerBoundsRect(MovieController mc,const Rect *bounds); 
F_PASCAL( RgnHandle ) MCGetControllerBoundsRgn(MovieController mc); 
F_PASCAL( RgnHandle ) MCGetWindowRgn(MovieController mc,WindowPtr w); 

/*  other stuff  */
F_PASCAL( ComponentResult ) MCMovieChanged(MovieController mc,Movie m); 
F_PASCAL( ComponentResult ) MCSetDuration(MovieController mc,TimeValue duration); 
F_PASCAL( TimeValue ) MCGetCurrentTime(MovieController mc,TimeScale *scale); 
F_PASCAL( ComponentResult ) MCNewAttachedController(MovieController mc,Movie m,
 WindowPtr w,Point where); 
F_PASCAL( ComponentResult ) MCPositionController(MovieController mc,Rect *movieRect,
 Rect *controllerRect,long controllerCreationFlags); 

/*  direct event handlers  */
F_PASCAL( ComponentResult ) MCDraw(MovieController mc,WindowPtr w); 
F_PASCAL( ComponentResult ) MCActivate(MovieController mc,WindowPtr w,Boolean activate); 
F_PASCAL( ComponentResult ) MCIdle(MovieController mc); 
F_PASCAL( ComponentResult ) MCKey(MovieController mc,char key,long modifiers); 
F_PASCAL( ComponentResult ) MCClick(MovieController mc,WindowPtr w,Point where,
 long when,long modifiers); 

/*   calls for editing  */
F_PASCAL( ComponentResult ) MCEnableEditing(MovieController mc,Boolean enabled); 
F_PASCAL( long ) MCIsEditingEnabled(MovieController mc); 
F_PASCAL( Movie ) MCCopy(MovieController mc); 
F_PASCAL( Movie ) MCCut(MovieController mc); 
F_PASCAL( ComponentResult ) MCPaste(MovieController mc,Movie srcMovie); 
F_PASCAL( ComponentResult ) MCClear(MovieController mc); 
F_PASCAL( ComponentResult ) MCUndo(MovieController mc); 

/*  menu related stuff  */
F_PASCAL( ComponentResult ) MCGetControllerInfo(MovieController mc,long *mcInfoFlags); 

F_PASCAL( ComponentResult ) MCSetClip(MovieController mc, RgnHandle theClip, RgnHandle movieClip);
F_PASCAL( ComponentResult ) MCGetClip(MovieController mc, RgnHandle *theClip, RgnHandle *movieClip);

F_PASCAL( ComponentResult ) MCDrawBadge(MovieController mc, RgnHandle movieRgn, RgnHandle *badgeRgn);

#ifdef __cplusplus
}
#endif

#ifndef __ERRORS__
enum {
/* movie controller error codes */
 cannotMoveAttachedController = -9999,
 controllerHasFixedHeight = -9998,
 cannotSetWidthOfAttachedController = -9997,
 controllerBoundsNotExact = -9996,
 editingNotAllowed = -9995,
 badControllerHeight = -9994
};
#endif

enum {
	kMCSetMovieSelect = 2,
	kMCRemoveMovieSelect = 3,
	kMCIsPlayerEventSelect = 7,
	kMCSetActionFilterSelect = 8,
	kMCDoActionSelect = 9,
	kMCSetControllerAttachedSelect = 10,
	kMCIsControllerAttachedSelect = 11,
	kMCSetControllerPortSelect = 12,
	kMCGetControllerPortSelect = 13,
	kMCGetVisibleSelect = 14,
	kMCSetVisibleSelect = 15,
	kMCGetControllerBoundsRectSelect = 16,
	kMCSetControllerBoundsRectSelect = 17,
	kMCGetControllerBoundsRgnSelect = 18,
	kMCGetWindowRgnSelect = 19,
	kMCMovieChangedSelect = 20,
	kMCSetDurationSelect = 21,
	kMCGetCurrentTimeSelect = 22,
	kMCNewAttachedControllerSelect = 23,
	kMCDrawSelect = 24,
	kMCActivateSelect = 25,
	kMCIdleSelect = 26,
	kMCKeySelect = 27,
	kMCClickSelect = 28,
	kMCEnableEditingSelect = 29,
	kMCIsEditingEnabledSelect = 30,
	kMCCopySelect = 31,
	kMCCutSelect = 32,
	kMCPasteSelect = 33,
	kMCClearSelect = 34,
	kMCUndoSelect = 35,
	kMCPositionControllerSelect = 36,
	kMCGetControllerInfoSelect = 37,
	kMCNewControllerFromFileSelect = 38,
	kMCSetBadgePictSelect = 39,
	kMCSetClipSelect = 40,
	kMCGetClipSelect = 41,
	kMCDrawBadgeSelect = 42
};

#endif

