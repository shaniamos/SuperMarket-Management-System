#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255


char*	getStrExactLength(const char* msg);
char*	getsStrFixSize(char* buffer, int size, const char* msg);
char*	myGets(char* buffer, int size, FILE* source);
char*	getDynStr(char* str);
char**	splitCharsToWordsBySpace(char* str,int* pCount,int* pTotalLength);
char**	splitCharsToWords(char* str, int* pCount, const char* del);
void	generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));




#endif

