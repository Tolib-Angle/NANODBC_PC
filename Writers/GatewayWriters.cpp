#pragma once
#include "GatewayWriters.h"
#include "../DateBaseConnect/DateBaseConnect.h"
#include <sstream>
#include <iostream>

void GatewayWriters::add(Writers* data) {
	std::stringstream ss;
	// add new writer in date base
	ss << "INSERT INTO writers (id,passport_number,surname,name,middle_name,address,phone)"
		<< "VALUES (" << data->getId() << ", " << data->getPassportNumber() << ", '"
		<< data->getSurname() << "', '" << data->getName() << "', '" << data->getMiddleName()
		<< "', '" << data->getAddress() << "', '" << data->getPhone() << "');";
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError add new writer, Position \'GatewayWriters.cpp\' str = 18\n";
	}
}

int GatewayWriters::get_count() {
	std::stringstream ss;
	// get count elemnts in date base
	ss << "SELECT COUNT(*) FROM writers;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		return result.get<int>(0);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get count writers, Position \'GatewayWriters.cpp\' str = 31\n";
		return -1;
	}
}

int* GatewayWriters::get_id() {
	int size_res = get_count();
	int* res = new int[size_res];
	int i = 0;
	std::stringstream ss;
	// select all id from writers in date base
	ss << "SELECT id FROM writers;";
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
		std::cout << e.what() << "\nError get all id writers, Position \'GatewayWriters.cpp\' str = 50\n";
		return nullptr;
	}
}

void GatewayWriters::save(Writers* data) {
	std::stringstream ss;
	// update writer in date base
	ss << "UPDATE writers SET passport_number = " << data->getPassportNumber() << ", surname = '" << data->getSurname()
		<< "', name = '" << data->getName() << "', middle_name = '" << data->getMiddleName() << "', address = '"
		<< data->getAddress() << "', phone = '" << data->getPhone() << "' WHERE id = " << data->getId() << ";";
	std::string postgreSQL = ss.str();
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(postgreSQL);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError update writer, Position \'GatewayWriters.cpp\' str = 74\n";
	}
}

Writers* GatewayWriters::get(int id) {
	std::stringstream ss;
	// get object(type = Writers) where id = input(id)
	ss << "SELECT * FROM writers WHERE id = " << id << ";";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		Writers* writer = new Writers(result.get<int>(0));
		writer->setPassportNumber(result.get<int>(1));
		writer->setSurname(result.get<std::string>(2));
		writer->setName(result.get<std::string>(3));
		if (result.is_null(4))
			writer->setMiddleName("empty");
		else
			writer->setMiddleName(result.get<std::string>(4));
		writer->setAddress(result.get<std::string>(5));
		writer->setPhone(result.get<std::string>(6));
		return writer;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get writer, Position \'GatewayWriters.cpp\' str = 89\n";
		return nullptr;
	}
}

void GatewayWriters::remove(int id) {
	std::stringstream ss;
	// delete writer in date base
	ss << "DELETE FROM writers WHERE id = " << id << ";";
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError remove writer, Position \'GatewayWriters.cpp\' str = 117\n";
	}
}

std::list<Writers*>* GatewayWriters::get_N(int start, int end) {
	std::list<Writers*>* res = new std::list<Writers*>;
	std::stringstream ss;
	// get writers where id writer between start and end
	ss << "SELECT * FROM writers WHERE id BETWEEN " << start << " AND " << end << " ORDER BY id;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			Writers* writer = new Writers(result.get<int>(0));
			writer->setPassportNumber(result.get<int>(1));
			writer->setSurname(result.get<std::string>(2));
			writer->setName(result.get<std::string>(3));
			if (result.is_null(4))
				writer->setMiddleName("empty");
			else
				writer->setMiddleName(result.get<std::string>(4));
			writer->setAddress(result.get<std::string>(5));
			writer->setPhone(result.get<std::string>(6));
			res->push_back(writer);
		}
		return res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get N writers, Position \'GatewayWriters.cpp\' str = 133\n";
		return nullptr;
	}
}