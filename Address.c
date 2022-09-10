#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Address.h"
#include "General.h"
#include "fileHelper.h"

#define WORD_SEP "#"

int	initAddress(Address* pAd)
{
	char allData[MAX_STR_LEN];
	char** elements;
	int count;
	int ok = 1;
	do {
		getsStrFixSize(allData, sizeof(allData), 
			"Enter address data\nFormat: street!house number!city\nstreet and city can have spaces\n");
		elements = splitCharsToWords(allData, &count, "!");
		ok = checkElements(elements, count);
		if (!ok)
			freeElements(elements, count);
	} while (!ok);

	//get the house number
	sscanf(elements[1], "%d", &pAd->num);
	pAd->street = fixAddressParam(elements[0]);
	if (!pAd->street)
	{
		freeElements(elements, count);
		return 0;
	}

	pAd->city = fixAddressParam(elements[2]);
	if (!pAd->city)
	{
		free(pAd->street);
		freeElements(elements, count);
		return 0;
	}

	freeElements(elements, count);
	return 1;
}


void printAddress(const Address* pAd)
{
	printf("%s %d, %s\n", pAd->street, pAd->num, pAd->city);
}

int saveAddressToFile(const Address* pAdd, FILE* fp)
{
	if (!writeIntToFile(pAdd->num, fp, "error write house number\n"))
		return 0;

	if(!writeStringToFile(pAdd->street,fp,"error write street name\n"))
		return 0;

	if (!writeStringToFile(pAdd->city, fp, "error write city name\n"))
		return 0;
	return 1;
}

int		saveCompressAddressToFile(const Address* pAdd, FILE* fp)
{
	BYTE data[2];

	int citylen = (int)strlen(pAdd->city);
	int streetlen = (int)strlen(pAdd->street);

	data[0] = (pAdd->num << 1) | ((citylen >> 3) & 0x1);
	data[1] = ((citylen & 0x7) << 5) | (streetlen & 0x1F);

	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	if (fwrite(pAdd->city, sizeof(char), citylen, fp) != citylen)
		return 0;

	if (fwrite(pAdd->street, sizeof(char), streetlen, fp) != streetlen)
		return 0;

	return 1;
}

int loadAddressFromFile(Address* pAdd, FILE* fp)
{
	if (!readIntFromFile(&pAdd->num, fp, "error read house number\n"))
		return 0;

	pAdd->street = readStringFromFile(fp, "error read street name\n");
	if(!pAdd->street)
		return 0;

	pAdd->city = readStringFromFile(fp, "error read city name\n");
	if(!pAdd->city)
		return 0;
	return 1;
}

int		loadCompressAddressFromFile(Address* pAdd, FILE* fp)
{
	BYTE data[2];

	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	pAdd->num = data[0] >> 1;
	int citylen = (data[0] & 0x1) << 3 | (data[1] >> 5) & 0x7;
	int streetlen = data[1] & 0x1F;

	pAdd->city = (char*)calloc(citylen + 1, sizeof(char));
	if (!pAdd->city)
		return 0;

	pAdd->street = (char*)calloc(streetlen + 1, sizeof(char));
	if (!pAdd->city)
	{
		free(pAdd->city);
		return 0;
	}


	if (fread(pAdd->city, sizeof(char), citylen, fp) != citylen)
	{
		free(pAdd->city);
		free(pAdd->street);
		return 0;
	}


	if (fread(pAdd->street, sizeof(char), streetlen, fp) != streetlen)
	{
		free(pAdd->city);
		free(pAdd->street);
		return 0;
	}
	return 1;
}

char*	fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWordsBySpace(param, &count, &totalLength);
	//add size for the WORD_SEP between words and for '\0'
	fixParamStr = (char*)calloc(totalLength + count, sizeof(char));
	if (!fixParamStr)
	{
		for (int i = 0; i < count; i++)
			free(wordsArray[i]);
		free(wordsArray);
		return NULL;
	}

	int len;
	for (int i = 0; i < count; i++)
	{
		len = (int)strlen(wordsArray[i]);
		wordsArray[i][0] = toupper(wordsArray[i][0]); //upper
		for(int j = 1; j < len-1; j++)
		{
			wordsArray[i][j] = tolower(wordsArray[i][j]); //small
		}
		if(count == 1)
			wordsArray[i][len-1] = toupper(wordsArray[i][len - 1]); //upper
		else
			wordsArray[i][len - 1] = tolower(wordsArray[i][len - 1]); //small

		strcat(fixParamStr, wordsArray[i]);
		if (i != count - 1) //not last
			strcat(fixParamStr, WORD_SEP);
	}

	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);

	return fixParamStr;

}


int checkElements(char**  elements, int count)
{
	if (count != 3)
		return 0;
	//check second word is a number;
	char* streetNum = elements[1];
	while (*streetNum)
	{
		if (!isdigit(*streetNum))
			return 0;
		streetNum++;
	}
	return 1;
}

void freeElements(char**  elements, int count)
{
	for (int i = 0; i < count; i++)
		free(elements[i]);
	free(elements);
}

void	freeAddress(Address* pAd)
{
	free(pAd->city);
	free(pAd->street);
}