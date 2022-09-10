#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{
	pCustomer->name = getStrExactLength("Enter customer name\n");
	if (!pCustomer->name)
		return 0;

	return 1;
}

void	printCustomer(const Customer* pCustomer)
{
	printf("Name: %s\n", pCustomer->name);
}

int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(Customer* pCust)
{
	free(pCust->name);
}