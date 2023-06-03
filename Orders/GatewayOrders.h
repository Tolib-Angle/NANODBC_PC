#pragma once
#include "../Orders/Orders.h"
#include "../Gateway.h"
#include "../Customers/Customers.h"
#include "../Books/Books.h"
#include "../Books/GatewayBooks.h"
#include "../Customers/GatewayCustomers.h"

class GatewayOrders : public Gateway<Orders> {
public:
	int get_count() override;
	void save(Orders* data) override;
	Orders* get(int id) override;
	void remove(int id) override;
	std::list<Orders*>* get_N(int start, int end) override;
	int* get_id() override;
	void add(Orders* data) override;
};