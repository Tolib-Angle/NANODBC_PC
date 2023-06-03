#pragma once
#include "GatewayContracts.h"
#include "../Writers/Writers.h"
#include "../Writers/GatewayWriters.h"
#include "../DateBaseConnect/DateBaseConnect.h"
#include <sstream>
#include <iostream>

void GatewayContracts::add(Contracts* data) {
	std::stringstream ss;
	ss << "INSERT INTO contracts (id,id_writer,contract_number,date_of_cons_contract,term_of_the_contract,validy_of_the_contract,date_of_terminition_contract)"
		<< "VALUES (" << data->getId() << ", " << data->getIdWriters()->getId() << ", "
		<< data->getContractNum() << ", '" << data->getDateOfConsContract() << "', " << data->getTermOfTheContract()
		<< ", '" << (data->getValidyOfTheContract() ? "True" : "False") << "', '" << data->getDateOfTheTerminitionContract() << "');";
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError add new contracts, Position \'GatewayContracts.cpp\' str = 19\n";
	}
}

int GatewayContracts::get_count() {
	std::stringstream ss;
	ss << "SELECT COUNT(*) FROM contracts;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		return result.get<int>(0);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get count contracts, Position \'GatewayContracts.cpp\' str = 31\n";
		return -1;
	}
}

int* GatewayContracts::get_id() {
	int* res = new int[get_count()];
	int i = 0;
	std::stringstream ss;
	ss << "SELECT id FROM contracts;";
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
		std::cout << e.what() << "\nError get all id contracts, Position \'GatewayContracts.cpp\' str = 48\n";
		return nullptr;
	}
}

void GatewayContracts::save(Contracts* data) {
	std::stringstream ss;
	ss << "UPDATE contracts SET id_writer = " << data->getIdWriters()->getId() << ", contract_number = " << data->getContractNum()
		<< ", date_of_cons_contract = '" << data->getDateOfConsContract() << "', term_of_the_contract = " << data->getTermOfTheContract() << ", validy_of_the_contract = '"
		<< (data->getValidyOfTheContract() ? "True" : "False") << "', date_of_terminition_contract = '" << data->getDateOfTheTerminitionContract() << "' WHERE id = " << data->getId() << ";";
	std::string postgreSQL = ss.str();
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(postgreSQL);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError save contracts, Position \'GatewayContracts.cpp\' str = 71\n";
	}
}

Contracts* GatewayContracts::get(int id) {
	int id_writer = -1;
	GatewayWriters gateway_writers;
	Writers* writer;
	std::stringstream ss;
	ss << "SELECT * FROM contracts WHERE id = " << id << ";";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		Contracts* contract = new Contracts(result.get<int>(0));
		id_writer = result.get<int>(1);
		contract->setContractNum(result.get<int>(2));
		contract->setDateOfConsContract(result.get<std::string>(3));
		contract->setTermOfTheContract(result.get<int>(4));
		contract->setValidyOfTheContract((result.get<std::string>(5) == "1" ? true : false));
		contract->setDateOfTheTerminitionContract(result.get<std::string>(6));
		if (id_writer != -1) {
			writer = gateway_writers.get(id_writer);
			contract->setIdWriters(writer);
		}
		return contract;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get contracts, Position \'GatewayContracts.cpp\' str = 88\n";
		return nullptr;
	}
}

void GatewayContracts::remove(int id) {
	std::stringstream ss;
	ss << "DELETE FROM contracts WHERE id = " << id << ";";
	try {
		auto date_base = DateBaseConnect::getInstance();
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError remove contracts, Position \'GatewayContracts.cpp\' str = 116\n";
	}
}

std::list<Contracts*>* GatewayContracts::get_N(int start, int end) {
	int id_writer = -1;
	GatewayWriters gateway_writers;
	Writers* writer;
	std::list<Contracts*> res;
	std::stringstream ss;
	ss << "SELECT * FROM contracts WHERE id BETWEEN " << start << " AND " << end << " ORDER BY id;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			Contracts* contract = new Contracts(result.get<int>(0));
			id_writer = result.get<int>(1);
			contract->setContractNum(result.get<int>(2));
			contract->setDateOfConsContract(result.get<std::string>(3));
			contract->setTermOfTheContract(result.get<int>(4));
			contract->setValidyOfTheContract((result.get<std::string>(5) == "t" ? true : false));
			contract->setDateOfTheTerminitionContract(result.get<std::string>(6));
			if (id_writer != -1)
				writer = gateway_writers.get(id_writer);
			contract->setIdWriters(writer);
			res.push_back(contract);
		}
		return &res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nError get N contracts, Position \'GatewayContracts.cpp\' str = 134\n";
		return nullptr;
	}
}