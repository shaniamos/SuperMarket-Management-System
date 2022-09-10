#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  "ShoppingItem.h"
#include "myMacros.h"

void	printItem(const ShoppingItem* pItem)
{
	printf("Item %s count %d price per item %.2f\n", pItem->name, pItem->count, pItem->price);
}

ShoppingItem*	createItem(const char* name, float price, int count)
{
	ShoppingItem* pItem = (ShoppingItem*)malloc(1 * sizeof(ShoppingItem));
	CHECK_RETURN_NULL(pItem);
	pItem->name = _strdup(name);
	pItem->price = price;
	pItem->count = count;
	return pItem;
}


void	freeItem(ShoppingItem* pItem)
{
	free(pItem->name);
}