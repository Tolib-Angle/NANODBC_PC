#pragma once
#include "../Gateway.h"
#include "Customers.h"

class GatewayCustomers : public Gateway<Customers> {
public:
	int get_count() override;
	void save(Customers* data) override;
	Customers* get(int id) override;
	void remove(int id) override;
	std::list<Customers*>* get_N(int start, int end) override;
	int* get_id() override;
	void add(Customers* data) override;
	Customers* get_parametrs(std::string value, std::string parametr, std::string type);
};