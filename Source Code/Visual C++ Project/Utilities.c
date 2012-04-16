#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"


char * getTextParameter(PA_PluginParameters param, short index)
{

	char *textValue = NULL;
	long textLength = 0;

	textLength = PA_GetTextParameter( param, index, 0L ) + 1;
	
	// modified by Mark de Wever on 20060721
  // textLength was always > 0 due to the +1
  // revert change since sys_ShellExecute
  // fails if NULL pointer is returned...
  // So if malloc fails it will crash 4D
  // if(textLength > 1) {
	if(textLength > 0) {
		textValue = (char *)malloc(textLength);

		if(textValue != NULL) {

			memset(textValue, 0, textLength);
			textLength = PA_GetTextParameter( param, index, textValue );

		}

	}

	return textValue;

}



void freeTextParameter(char *textParam)
{

	if(textParam != NULL) {
		free(textParam);
		textParam = NULL;
	}

}

char * concatStr(const char *str1, const char *str2)
{
	char *newStr = NULL;
	int newStrlen = 0;

	newStrlen = strlen(str1) + strlen(str2) + 1;
	newStr = (char *)malloc(newStrlen);

	strcat(newStr, str1);
	strcat(newStr, str2);

	return newStr;
}

// MJG 7/5/05 #8372	
void setError (long value)
{
	PA_Variable   errorVar;
	
	errorVar = PA_GetVariable((char *) "Error");
	
	if(PA_GetLastError() == eER_NoErr)
	{
		switch (PA_GetVariableKind(errorVar))
		{
		case eVK_Real:
			PA_SetRealVariable(&errorVar, (double) value);
			break;
			
		case eVK_Longint:
			PA_SetLongintVariable(&errorVar, value);
			break;
		}
		
		PA_SetVariable((char *) "Error", errorVar, 1);
	}
}
