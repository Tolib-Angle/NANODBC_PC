#pragma once
#include "../Gateway.h"
#include "Books.h"

class GatewayBooks : public Gateway<Books> {
public:
	int get_count() override;
	void save(Books* data) override;
	Books* get(int id) override;
	void remove(int id) override;
	std::list<Books*>* get_N(int start, int end) override;
	int* get_id() override;
	void add(Books* data) override;
};