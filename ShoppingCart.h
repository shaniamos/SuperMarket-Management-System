#pragma once

#include "ShoppingItem.h"
#include "GeneralList.h"

typedef struct
{
	//ShoppingItem**	itemArr;
	LIST			itemList;
	//int				count;
}ShoppingCart;




void	initCart(ShoppingCart* pCart);
float	getTotalPrice(const ShoppingCart* pCart);
int		insertItemToList(NODE* pPrevN, const char* name, float price, int count);
//ShoppingItem*	getItemByName(ShoppingCart* pCart, const char* name);
int		addItemToCart(ShoppingCart* pCart, const char* name, float price, int count);
void	printShoppingCart(const ShoppingCart* pCart);
void	freeShoppingCart(ShoppingCart* pCart);