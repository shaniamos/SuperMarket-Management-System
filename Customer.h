#pragma once

typedef struct
{
	char*	name;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);

int		isCustomer(const Customer* pCust, const char* name);
void	freeCustomer(Customer* pCust);



