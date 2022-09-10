#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"
#include "myMacros.h"
#include "SuperFile.h"

int menu();

const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping",
								"Print Product By Type","Sort Products", "Search Product"};

int main(int  argc, char*  argv[])
{
	if (argc != 3)
		ERROR_MSG_PAUSE_RETURN_0("Error in main parameters");

	int bCompress;
	sscanf(argv[1], "%d", &bCompress);
	char* superMarketFileName = argv[2];

	SuperMarket	market;

	if (!initSuperMarket(&market, superMarketFileName, CUSTOMER_FILE_NAME, bCompress))
	{
		printf("error init  Super Market");
		return 0;
	}

	int option;
	int stop = 0;
	
	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;

		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			doShopping(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case eSortProduct:
			sortProducts(&market);
			break;
			
		case eSearchProduct:
			findProduct(&market);
			break;

		case EXIT:
			printMessage("Thank", "You", "For", "Shopping", "With", "Us", NULL);
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	if (!saveSuperMarketToFile(&market, superMarketFileName, CUSTOMER_FILE_NAME, bCompress))
		printf("Error saving supermarket to file\n");

	freeMarket(&market);
	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

void printMessage(const char* str, ...)
{
	va_list list;

	va_start(list, str);
	const char* word = str;

	while (word != NULL)
	{
		printf("%s ", word);
		word = va_arg(list, char*);
	}

	printf("\n");
	va_end(list);
}

