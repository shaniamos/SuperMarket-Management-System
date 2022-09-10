#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"

typedef enum { eNone, eName, eBarcode, eType, ePrice, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];


typedef struct
{
	char*		name;
	Address		location;
	Customer*	customerArr;
	int			customerCount;
	Product**	productArr;
	int			productCount;
	eSortOption	sortOpt;
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket,const char* fileName,
			const char* customersFileName,int bCompress);
void	printSuperMarket(const SuperMarket* pMarket);

int		addProduct(SuperMarket* pMarket);
int		addNewProduct(SuperMarket* pMarket, const char* barcode);
int		addCustomer(SuperMarket* pMarket);
void	doShopping(SuperMarket* pMarket);
void	printProductByType(SuperMarket* pMarket);

void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);

eSortOption showSortMenu();
void	sortProducts(SuperMarket* pMarket);
void	findProduct(const SuperMarket* pMarket);
void*	getCompareFunction(const SuperMarket* pMarket);


int		getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

void fillCart(SuperMarket* pMarket, ShoppingCart* pCart);

void	freeMarket(SuperMarket* pMarket);

