#pragma once
#include "Writers.h" 
#include "../Gateway.h"

class GatewayWriters : public Gateway<Writers> {
public:
	int get_count() override;
	void save(Writers* data) override;
	Writers* get(int id) override;
	void remove(int id) override;
	std::list<Writers*>* get_N(int start, int end) override;
	int* get_id() override;
	void add(Writers* data) override;
};