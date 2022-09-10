#pragma once


typedef enum {eFruitVegtable, eFridge, eFrozen, eShelf, eNofProductType} eProductType;

const char* typeStr[eNofProductType];

#define NAME_LENGTH 20
#define BARCODE_LENGTH 7

typedef struct
{
	char		name[NAME_LENGTH+1];
	char		barcode[BARCODE_LENGTH + 1];
	eProductType	type;
	float		price;
	int			count;
}Product;

void	initProduct(Product* pProduct);
void	initProductNoBarcode(Product* pProduct);
void	initProductName(Product* pProduct);
void	printProduct(const Product* pProduct);
void	printProductV(const void* val);
int		saveProductToFile(const Product* pProduct, FILE* fp);
int		saveCompressProductToFile(const Product* pProduct, FILE* fp);
int		loadProductFromFile(Product* pProduct, FILE* fp);
void	initBarcodeFromCompressInt(char* barcodeArr, int size, int* codes);
void	createBarcodeCompressCode(const char* barcodeArr, int size, int* codes);
int		loadCompressProductFromFile(Product* pProduct, FILE* fp);
void	getBorcdeCode(char* code);
int		isProduct(const Product* pProduct,const char* barcode);
eProductType getProductType();
const char*	getProductTypeStr(eProductType type);

void	updateProductCount(Product* pProduct);
void	freeProduct(Product* pProduct);


int		compareProductByName(const void* var1, const void* var2);
int		compareProductByBarcode(const void* var1, const void* var2);
int		compareProductByType(const void* var1, const void* var2);
int		compareProductByPrice(const void* var1, const void* var2);