#pragma once

#include <stdio.h>
#include "Supermarket.h"

int		saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
			const char* customersFileName, int bCompress);
int		saveUnCompressSuperData(const SuperMarket* pMarket, FILE* fp);
int		saveCompressSuperData(const SuperMarket* pMarket, FILE* fp);


int		saveCustomerToTextFile(const SuperMarket* pMarket, const char* customersFileName);
int		loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
			const char* customersFileName, int bCompress);
int		loadProductsFromFile(SuperMarket* pMarket, FILE* fp, int compressed);
int		readCompressSuperData(SuperMarket* pMarket, FILE* fp);
int		readUnCompressSuperData(SuperMarket* pMarket, FILE* fp);


int		loadCustomerFromTextFile(SuperMarket* pMarket, const char* customersFileName);

int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName);