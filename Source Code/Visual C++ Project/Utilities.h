#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

char * getTextParameter(PA_PluginParameters, short);
void freeTextParameter(char *);
void setError (long value); // MJG 7/5/05 #8372