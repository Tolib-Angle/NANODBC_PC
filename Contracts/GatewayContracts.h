#pragma once 
#include "../Gateway.h"
#include "Contracts.h"

class GatewayContracts : public Gateway<Contracts> {
public:
	int get_count() override;
	void save(Contracts* data) override;
	Contracts* get(int id) override;
	void remove(int id) override;
	std::list<Contracts*>* get_N(int start, int end) override;
	int* get_id() override;
	void add(Contracts* data) override;
};