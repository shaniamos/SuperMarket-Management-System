#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"
#include "SuperFile.h"
#include "myMacros.h"

static const char* sortOptStr[eNofSortOpt] = {
	"None","Name", "Barcode" , "Type", "Price" };



int		initSuperMarket(SuperMarket* pMarket, const char* fileName,
				const char* customersFileName, int bCompress)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
	pMarket->sortOpt = eNone;
	if (loadSuperMarketFromFile(pMarket, fileName, customersFileName,bCompress) == 1)
	{
		printf("Supermarket successfully loaded from files\n");
		//loadProductFromTextFile(pMarket, "Super.txt");
		return 1;
	}
	   
	//not from file, load from user!!
	pMarket->name = getStrExactLength("Enter market name");
	initAddress(&pMarket->location);
	return 1;
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
	printf("\n");
}


int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd != NULL) //This barcode exist in stock
		updateProductCount(pProd);
	else { //new product to stock
		if (!addNewProduct(pMarket, barcode))
			return 0;
	}

	return 1;
}

int		addNewProduct(SuperMarket* pMarket, const char* barcode)
{
	pMarket->productArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	
	CHECK_RETRUN_0(pMarket->productArr)

	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	strcpy(pProd->barcode, barcode);
	initProductNoBarcode(pProd);

	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	pMarket->sortOpt = eNone;
	return 1;
}

int		addCustomer(SuperMarket* pMarket)
{
	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	CHECK_RETRUN_0(pMarket->customerArr)

	if (!initCustomer(&pMarket->customerArr[pMarket->customerCount]))
		return 0;

	pMarket->customerCount++;
	return 1;
}

void	doShopping(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return;
	}
	
	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return;
	}
	
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name,sizeof(name),"Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket,name);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return;
	}

	ShoppingCart cart;
	initCart(&cart);
	fillCart(pMarket,&cart);
	printf("---------- Shopping ended ----------\n");
	printf("---------- Cart info and bill for %s ----------\n",name);
	printShoppingCart(&cart);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(&cart);
	printf("---------- Shopping  ----------\n");
}


void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
#ifdef DETAIL_PRINT
	printf("%-20s %-10s\t", "Name", "Barcode");
#else
	printf("%-20s\t", "Name");
#endif
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");
	generalArrayFunction((void*)pMarket->productArr, pMarket->productCount, sizeof(Product*), 
		printProductV);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	generalArrayFunction((void*)pMarket->customerArr, pMarket->customerCount, sizeof(Customer),
		printCustomer);
}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}

void	sortProducts(SuperMarket* pMarket)
{
	pMarket->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	compare = getCompareFunction(pMarket);

	if (compare != NULL)
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
	else
		printf("Error in sorting\n");
}



void	findProduct(const SuperMarket* pMarket)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Product p = { 0 };
	Product* pProd = &p;
	compare = getCompareFunction(pMarket);


	switch (pMarket->sortOpt)
	{
	case eName:
		initProductName(pProd);
		break;

	case eBarcode:
		getBorcdeCode(pProd->barcode);
		break;

	case eType:
		pProd->type = getProductType();
		break;

	case ePrice:
		printf("Enter product price\n");
		scanf("%f", &pProd->price);
		break;
	}

	if (compare != NULL)
	{
		Product** pP = bsearch(&pProd, pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
		if (pP == NULL)
			printf("Product was not found\n");
		else {
			printf("Product found, ");
			printProduct(*pP);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}
}


void* getCompareFunction(const SuperMarket* pMarket)
{
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pMarket->sortOpt)
	{
	case eName:
		compare = compareProductByName;
		break;
	case eBarcode:
		compare = compareProductByBarcode;
		break;
	case eType:
		compare = compareProductByType;
		break;
	case ePrice:
		compare = compareProductByPrice;
		break;

	}
	return compare;

}

void fillCart(SuperMarket* pMarket,ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		char barcode[BARCODE_LENGTH + 1];
		Product* pProd = getProductFromUser(pMarket, barcode);
		if (pProd == NULL)
			printf("No such product\n");
		else {
			if (pProd->count == 0)
			{
				printf("This product out of stock\n");
				continue;
			}
			int count;
			do {
				printf("How many items do you want? max %d\n", pProd->count);
				scanf("%d", &count);
			} while (count < 0 || count > pProd->count);
			if (!addItemToCart(pCart, pProd->name, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr[i]);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	for (int i = 0; i < pMarket->productCount; i++)
	{
		freeProduct(pMarket->productArr[i]);
		free(pMarket->productArr[i]);
	}
	free(pMarket->productArr);
	

	generalArrayFunction((void*)pMarket->customerArr, pMarket->customerCount, sizeof(Customer),
		freeCustomer);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBorcdeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;

}


Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}
