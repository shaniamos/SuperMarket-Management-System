#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ShoppingCart.h"



void	initCart(ShoppingCart* pCart)
{
	//pCart->count = 0;
	//pCart->itemArr = NULL;
	L_init(&pCart->itemList);
}

float	getTotalPrice(const ShoppingCart* pCart)
{
	float price = 0;
	ShoppingItem* pItem;
	NODE* pN = pCart->itemList.head.next;
	while(pN !=  NULL)
	{
		pItem = (ShoppingItem*)pN->key;
		price += (pItem->price * pItem->count);
		pN = pN->next;
	}
	return price;
}

int		addItemToCart(ShoppingCart* pCart, const char* name, float price, int count)
{

	NODE* pN = pCart->itemList.head.next;
	NODE* pPrevNode = &pCart->itemList.head;
	ShoppingItem* pItem;
	int compRes;
	while (pN != NULL)
	{
		pItem = (ShoppingItem*)pN->key;
		compRes = strcmp(pItem->name, name);
		if (compRes == 0) //found a product with this name is cart
		{
			pItem->count += count;
			return 1;
		}
		else if (compRes > 0) {//found a place for new item, the next one bigger	
			return insertItemToList(pPrevNode,name, price, count);
		}
		pPrevNode = pN;
		pN = pN->next;
	}
	
	return insertItemToList(pPrevNode, name, price, count);
}

int insertItemToList(NODE* pPrevN, const char* name, float price, int count)
{
	ShoppingItem* pItem = createItem(name, price, count);
	if (!pItem)
		return 0;
	NODE* pN =  L_insert(pPrevN, pItem);
	if (!pN)
		return 0;
	return 1;
}

void	printShoppingCart(const ShoppingCart* pCart)
{
	float totalPrice = getTotalPrice(pCart);
	L_print(&pCart->itemList, printItem);
	printf("Total bill toprice pay: %.2f\n", totalPrice);
}

void	freeShoppingCart(ShoppingCart* pCart)
{
	L_free(&pCart->itemList, freeItem);
}