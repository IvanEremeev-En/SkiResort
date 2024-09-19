
#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255
#define SEP_CHAR '_'

char* getStrExactName(const char* msg);
char* myGets(char* buffer, int size);
char* myGetsF(char* buffer, int size, FILE* source);
char* getDynStr(char* str);
char** splitCharsToWords(char* str, int* pCount, int* pTotalLength);
char* allocateRightLength(char** wordsArray, int count, int totalLength);
void changeName(char* name, int count, char** wordsArray);
void changeNameOneWord(char* name, const char* word);

#endif