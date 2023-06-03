#pragma once
#include "GatewayOrders.h"
#include "../DateBaseConnect/DateBaseConnect.h"
#include <sstream>
#include <iostream>

void GatewayOrders::add(Orders* data) {
	if (data == nullptr)
		throw new std::exception("input data is nullptr");
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	// add new order in date base
	ss << "INSERT INTO orders (id,id_customer,order_number,date_of_receipt_order,order_completion_date,id_book,numbers_of_order)"
		<< "VALUES (" << data->getId() << ", " << data->getIdCustomers()->getId() << ", "
		<< data->getOrderNumber() << ", '" << data->getDateOfReceiptOrder() << "', '" << data->getOrderCompletionDate()
		<< "', " << data->getIdBooks()->getId() << ", " << data->getNumbersOfOrders() << ");";
	try {
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError add new order, Position \'GatewayOrders.cpp\' str = 18\n";
	}
}

int GatewayOrders::get_count() {
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	// get count elemnts in date base
	ss << "SELECT COUNT(*) FROM orders;";
	try {
		auto result = date_base.exec(ss.str());
		result.next();
		return result.get<int>(0);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get count orders, Position \'GatewayOrders.cpp\' str = 31\n";
		return -1;
	}
}

int* GatewayOrders::get_id() {
	auto date_base = DateBaseConnect::getInstance();
	int size_res = get_count();
	int* res = new int[size_res];
	int i = 0;
	std::stringstream ss;
	// select all id from orders in date base
	ss << "SELECT id FROM orders ORDER BY id;";
	try {
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			res[i] = result.get<int>(0);
			i++;
		}
		return res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get all id orders, Position \'GatewayOrders.cpp\' str = 50\n";
		return nullptr;
	}
}

void GatewayOrders::save(Orders* data) {
	if (data == nullptr)
		throw new std::exception("input data is nullptr");
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	// update order in date base
	ss << "UPDATE writers SET id_customer = " << data->getIdCustomers()->getId() << ", order_number = " << data->getOrderNumber()
		<< ", date_of_receipt_order = '" << data->getDateOfReceiptOrder() << "', order_completion_date = '" << data->getOrderCompletionDate() << "', id_book = "
		<< data->getIdBooks()->getId() << ", numbers_of_order = " << data->getNumbersOfOrders() << " WHERE id = " << data->getId() << ";";
	std::string postgreSQL = ss.str();
	try {
		date_base.exec(postgreSQL);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError update order, Position \'GatewayOrders.cpp\' str = 74\n";
	}
}

Orders* GatewayOrders::get(int id) {
	if (id == NULL || id <= 0)
		throw new std::exception("input id is null or value <= 0");
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	int id_customer = -1, id_book = -1;
	// get object(type = Orders) where id = input(id)
	ss << "SELECT * FROM orders WHERE id = " << id << ";";
	try {
		auto result = date_base.exec(ss.str());
		result.next();
		Orders* order = new Orders(result.get<int>(0));
		id_customer = result.get<int>(1);
		order->setOrderNumber(result.get<int>(2));
		order->setDateOfReceiptOrder(result.get<std::string>(3));
		order->setOrderCompletionDate(result.get<std::string>(4));
		id_book = result.get<int>(5);
		order->setNumbersOfOrders(result.get<int>(6));

		GatewayCustomers customer_gateway;
		GatewayBooks book_gateway;

		order->setIdCustomers(customer_gateway.get(id_customer));
		order->setIdBooks(book_gateway.get(id_book));

		return order;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get writer, Position \'GatewayWriters.cpp\' str = 89\n";
		return nullptr;
	}
}

void GatewayOrders::remove(int id) {
	if (id == NULL || id <= 0)
		throw new std::exception("input id is null or value <= 0");
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	// delete writer in date base
	ss << "DELETE FROM orders WHERE id = " << id << ";";
	try {
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError remove order, Position \'GatewayOrders.cpp\' str = 117\n";
	}
}

std::list<Orders*>* GatewayOrders::get_N(int start, int end) {
	if (start == NULL || start <= 0 || end == NULL || end <= 0)
		throw new std::exception("input start or end is null or value <= 0");
	std::list<Orders*> res;
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	int id_customer = -1, id_book = -1;
	// get writers where id writer between start and end
	ss << "SELECT * FROM writers WHERE id BETWEEN " << start << " AND " << end << " ORDER BY id;";
	try {
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			Orders* order = new Orders(result.get<int>(0));
			id_customer = result.get<int>(1);
			order->setOrderNumber(result.get<int>(2));
			order->setDateOfReceiptOrder(result.get<std::string>(3));
			order->setOrderCompletionDate(result.get<std::string>(4));
			id_book = result.get<int>(5);
			order->setNumbersOfOrders(result.get<int>(6));

			GatewayCustomers customer_gateway;
			GatewayBooks book_gateway;

			order->setIdCustomers(customer_gateway.get(id_customer));
			order->setIdBooks(book_gateway.get(id_book));
			res.push_back(order);
		}

		return &res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get N orders, Position \'GatewayOrders.cpp\' str = 133\n";
		return nullptr;
	}
}