#pragma once
#include "../Cash/Cash.h"
#include "../Writers/Writers.h"
#include "../Writers/GatewayWriters.h"
#include "../functions/functions.h"
#include "../Contracts/Contracts.h"
#include "../Contracts/GatewayContracts.h"
#include "../Books/Books.h"
#include "../Books/GatewayBooks.h"
#include "../Customers/Customers.h"
#include "../Customers/GatewayCustomers.h"
#include "../Orders/Orders.h" 
#include "../Orders/GatewayOrders.h"

#define BLOCK 10

#include <iostream>
#include <iomanip>

class InterfaceTables {
private:
	// gateway for all tables
	GatewayBooks gateway_books;
	GatewayWriters gateway_writers;
	GatewayCustomers gateway_customers;
	// list T* for all tables
	std::list<Books*> list_books;
	std::list<Writers*> list_writers;
	std::list<Customers*> list_customers;
	// cash
	Cash<Books> cash_books;
	Cash<Writers> cash_writers;
	Cash<Customers> cash_customers;
	Cash<Orders> cash_orders;
	Cash<Contracts> cash_contracts;

	static InterfaceTables& getInstance();
	void table_writers();
	void table_contracts();
	void table_books();
	void table_customers();
	void table_orders();
public:
	void start();
};