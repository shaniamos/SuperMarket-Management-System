#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Address.h"
#include "General.h"
#include "fileHelper.h"
#include "SuperFile.h"
#include "myMacros.h"

int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName, int bCompress)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp, "Error open supermarket file to write\n");

	int ok;
	if (bCompress)
		ok = saveCompressSuperData(pMarket, fp);
	else
		ok = saveUnCompressSuperData(pMarket, fp);
	fclose(fp);

	if (!ok)
		return 0;

	return saveCustomerToTextFile(pMarket, customersFileName);
}

int saveUnCompressSuperData(const SuperMarket* pMarket, FILE* fp)
{
	if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n"))
	{
		fclose(fp);
		return 0;
	}

	if (!saveAddressToFile(&pMarket->location, fp))
	{
		fclose(fp);
		return 0;
	}

	if (!writeIntToFile((int)pMarket->sortOpt, fp, "Error write sort option\n"))
	{
		fclose(fp);
		return 0;
	}

	if (!writeIntToFile(pMarket->productCount, fp, "Error write product count\n"))
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (!saveProductToFile(pMarket->productArr[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}

	return 1;
}

int		saveCompressSuperData(const SuperMarket* pMarket, FILE* fp)
{
	BYTE data[2];

	int len = (int)strlen(pMarket->name);
	
	data[0] = pMarket->productCount >> 1;
	data[1] = (pMarket->productCount & 0x1) << 7 | (pMarket->sortOpt << 4) | (len & 0xF);

	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	if (fwrite(pMarket->name, sizeof(char), len, fp) != len)
		return 0;
	
	if (!saveCompressAddressToFile(&pMarket->location, fp))
		return 0;
	
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (!saveCompressProductToFile(pMarket->productArr[i], fp))
			return 0;
	}

	return 1;
}

int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName, int bCompress)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp, "Error open company file\n");

	pMarket->productArr = NULL;
	int ok;
	if (bCompress)
		ok = readCompressSuperData(pMarket, fp);
	else
		ok = readUnCompressSuperData(pMarket, fp);

	fclose(fp);
	if (!ok)
		return 0;

	return	loadCustomerFromTextFile(pMarket, customersFileName);
}


int		readUnCompressSuperData(SuperMarket* pMarket, FILE* fp)
{
	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n");
	CHECK_NULL_CLOSE_FILE_RETURN_0(pMarket->name, fp);

	if (!loadAddressFromFile(&pMarket->location, fp))
	{
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	int opt;
	if (!readIntFromFile(&opt, fp, "Error reading sort option\n"))
	{
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	pMarket->sortOpt = (eSortOption)opt;

	if (!readIntFromFile(&pMarket->productCount, fp, "Error reading product count\n"))
	{
		free(pMarket->name);
		fclose(fp);
		return 0;
	}
	return loadProductsFromFile(pMarket, fp, 0);
}

int		readCompressSuperData(SuperMarket* pMarket, FILE* fp)
{

	BYTE data[2];

	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	pMarket->productCount = data[0] << 1 | (data[1] >> 7) & 0x1;
	pMarket->sortOpt = (data[1] >> 4) & 0x7;
	int len = data[1] & 0xF;

	pMarket->name = (char*)calloc(len + 1, sizeof(char));
	CHECK_NULL_CLOSE_FILE_RETURN_0(pMarket->name, fp);

	if (fread(pMarket->name, sizeof(char), len, fp) != len)
	{
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	if (!loadCompressAddressFromFile(&pMarket->location, fp))
	{
		free(pMarket->name);
		fclose(fp);
		return 0;
	}

	return loadProductsFromFile(pMarket, fp, 1);
}


int loadProductsFromFile(SuperMarket* pMarket, FILE* fp, int compressed)
{
	if (pMarket->productCount > 0)
	{
		pMarket->productArr = (Product**)malloc(pMarket->productCount * sizeof(Product*));
		CHECK_NULL_MSG_FREE_CLOSE_FILE_RETURN_0(pMarket->productArr, pMarket->name, fp, "Allocation error\n");
	}


	for (int i = 0; i < pMarket->productCount; i++)
	{
		pMarket->productArr[i] = (Product*)calloc(1, sizeof(Product));
		if (!pMarket->productArr[i])
		{
			printf("Allocation error\n");
			for (int j = 0; j < i; j++) //free all that was allocated
				free(pMarket->productArr[j]);
			free(pMarket->productArr);
			free(pMarket->name);
			fclose(fp);
			return 0;
		}
		int res;
		if (compressed)
			res = loadCompressProductFromFile(pMarket->productArr[i], fp);
		else
			res = loadProductFromFile(pMarket->productArr[i], fp);
		if (!res)
		{
			for (int i = 0; i < pMarket->productCount; i++) //free all that was allocated
				free(pMarket->productArr[i]);
			free(pMarket->productArr);
			free(pMarket->name);
			fclose(fp);
			return 0;
		}
	}
	return 1;
}

int	saveCustomerToTextFile(const SuperMarket* pMarket, const char* customersFileName)
{
	FILE* fp;

	fp = fopen(customersFileName, "w");
	if (!fp) {
		printf("Error open customers file to write\n");
		return 0;
	}

	fprintf(fp, "%d\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		fprintf(fp, "%s\n", pMarket->customerArr[i].name);
	}
	fclose(fp);
	return 1;
}

int		loadCustomerFromTextFile(SuperMarket* pMarket, const char* customersFileName)
{
	FILE* fp;

	fp = fopen(customersFileName, "r");
	if (!fp) {
		printf("Error open customers file to write\n");
		return 0;
	}
	pMarket->customerArr = NULL;
	fscanf(fp, "%d\n", &pMarket->customerCount);
	if (pMarket->customerCount > 0)
	{
		pMarket->customerArr = (Customer*)malloc(pMarket->customerCount * sizeof(Customer));
		if (!pMarket->customerArr)
		{
			fclose(fp);
			return 0;
		}
		for (int i = 0; i < pMarket->customerCount; i++)
		{
			pMarket->customerArr[i].name = readDynStringFromTextFile(fp);
			if (!pMarket->customerArr[i].name)
			{
				for (int j = 0; j < i; j++)
					free(pMarket->customerArr[j].name);
				free(pMarket->customerArr);
				fclose(fp);
				return 0;
			}
		}
	}

	fclose(fp);
	return 1;
}






int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName)
{
	FILE* fp;

	fp = fopen(fileName, "r");
	int count;
	fscanf(fp, "%d\n", &count);

	pMarket->productArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + count) * sizeof(Product*));

	Product p;
	for (int i = 0; i < count; i++)
	{
		myGets(p.name, MAX_STR_LEN, fp);
		myGets(p.barcode, MAX_STR_LEN, fp);
		fscanf(fp, "%d %f %d\n", &p.type, &p.price, &p.count);
		pMarket->productArr[pMarket->productCount] = (Product*)calloc(1, sizeof(Product));
		*pMarket->productArr[pMarket->productCount] = p;
		pMarket->productCount++;
	}

	fclose(fp);
	return 1;
}
