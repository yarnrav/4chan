#ifndef STRING_FUNCS
#define STRING_FUNCS

#include <string.h>
#include <stdlib.h>

char * stringReplace(const char *search, const char *replace, char *string) {
	char *tempString, *searchStart;
	int len=0;

	searchStart = strstr(string, search);
	if(searchStart == NULL) {
		return string;
	}
	
	tempString = (char*) malloc((strlen(string) + 1) * sizeof(char));

	if(tempString == NULL) {
		return NULL;
	}
	
	strcpy(tempString, string);

	len = searchStart - string;
	string[len] = '\0';

	strcat(string, replace);

	len += strlen(search);
	strcat(string, (char*)tempString+len);
	
	free(tempString);
	
	stringReplace(search, replace, string);

	return string;
}
char *string_del(const char *start, const char *end, char *string)
{
	char *temp_string, *startString;
	int len = 0;
	startString = strstr(string, start);
	if(startString == NULL)
		return string;
	
	temp_string = (char*) malloc((strlen(string) + 1) * sizeof(char));
	
	if(temp_string == NULL) {
		return NULL;
	}

	strcpy(temp_string, string);
	
	len = startString - string;
	
	char *end_String = strstr(temp_string + len, end);
	
	string[len] = '\0';
	
	strcat(string, end_String + strlen(end));
	
	free(temp_string);
	
	string_del(start, end, string);
	
	return string;
}

#endif
