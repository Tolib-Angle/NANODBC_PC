#pragma once
#include "GatewayCustomers.h"
#include "../DateBaseConnect/DateBaseConnect.h"
#include <sstream>
#include <iostream>

Customers* GatewayCustomers::get_parametrs(std::string value, std::string parametr, std::string type) {
	std::stringstream ss;
	// get customer in date base
	if (type == "int" || type == "float" || type == "double") {
		ss << "SELECT * FROM customers WHERE " << parametr << " = " << value << ";";
	}
	else if (type == "string") {
		ss << "SELECT * FROM customers WHERE " << parametr << " = '" << value << "';";
	}
	// get records
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		Customers* customer = new Customers(result.get<int>(0));
		customer->setCustomerName(result.get<std::string>(1));
		customer->setAddress(result.get<std::string>(2));
		customer->setPhone(result.get<std::string>(3));
		customer->setFullNameCustomer(result.get<std::string>(4));
		return customer;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get customer, Position \'GatewayCustomers.cpp\' str = 88\n";
		return nullptr;
	}
}

void GatewayCustomers::add(Customers* data) {
	std::stringstream ss;
	// add new customer in date base
	ss << "INSERT INTO customers (id,customer_name,address,phone,full_name_customer)"
		<< "VALUES (" << data->getId() << ", '" << data->getCustomerName() << "', '"
		<< data->getAddress() << "', '" << data->getPhone() << "', '" << data->getFullNameCustomer() << "');";
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError add new customer, Position \'GatewayCustomers.cpp\' str = 17\n";
	}
}

int GatewayCustomers::get_count() {
	std::stringstream ss;
	// get count customers in date base
	ss << "SELECT COUNT(*) FROM customers;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		return result.get<int>(0);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get count customer, Position \'GatewayCustomers.cpp\' str = 30\n";
		return -1;
	}
}

int* GatewayCustomers::get_id() {
	int* res = new int[get_count()];
	int i = 0;
	std::stringstream ss;
	// get all id customers
	ss << "SELECT id FROM customers ORDER BY id;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			res[i] = result.get<int>(0);
			i++;
		}
		return res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get count customer, Position \'GatewayCustomers.cpp\' str = 48\n";
		return nullptr;
	}
}

void GatewayCustomers::save(Customers* data) {
	std::stringstream ss;
	// update customer in date base
	ss << "UPDATE customers SET customer_name = '" << data->getCustomerName() << "', address = '" << data->getAddress()
		<< "', phone = '" << data->getPhone() << "', full_name_customer = '" << data->getFullNameCustomer()
		<< "' WHERE id = " << data->getId() << ";";
	std::string postgreSQL = ss.str();
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(postgreSQL);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError update customer, Position \'GatewayCustomers.cpp\' str = 72\n";
	}
	
}

Customers* GatewayCustomers::get(int id) {
	std::stringstream ss;
	// get customer in date base
	ss << "SELECT * FROM customers WHERE id = " << id << ";";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		Customers* customer = new Customers(result.get<int>(0));
		customer->setCustomerName(result.get<std::string>(1));
		customer->setAddress(result.get<std::string>(2));
		customer->setPhone(result.get<std::string>(3));
		customer->setFullNameCustomer(result.get<std::string>(4));
		return customer;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get customer, Position \'GatewayCustomers.cpp\' str = 88\n";
		return nullptr;
	}
}

void GatewayCustomers::remove(int id) {
	std::stringstream ss;
	// delete customer in date base
	ss << "DELETE FROM customers WHERE id = " << id << ";";
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError delete customer, Position \'GatewayCustomers.cpp\' str = 111\n";
	}
}

std::list<Customers*>* GatewayCustomers::get_N(int start, int end) {
	std::list<Customers*> *res = new std::list<Customers*>;
	std::stringstream ss;
	// select customers where id betweeen start and end
	ss << "SELECT * FROM customers WHERE id BETWEEN " << start << " AND " << end << " ORDER BY id;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			Customers* customer = new Customers(result.get<int>(0));
			customer->setCustomerName(result.get<std::string>(1));
			customer->setAddress(result.get<std::string>(2));
			customer->setPhone(result.get<std::string>(3));
			customer->setFullNameCustomer(result.get<std::string>(4));
			res->push_back(customer);
		}
		return res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError delete customer, Position \'GatewayCustomers.cpp\' str = 127\n";
		return nullptr;
	}
	
}