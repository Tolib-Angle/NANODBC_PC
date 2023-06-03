#pragma once
#include "GatewayBooks.h"
#include "../Writers/Writers.h"
#include "../Writers/GatewayWriters.h"
#include "../DateBaseConnect/DateBaseConnect.h"
#include <sstream>
#include <iostream>

void GatewayBooks::add(Books* data) {
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	int id_writer = -1;
	// find id writers from writers_books table
	for (auto i = data->getIdWritersBook()->begin(); i != data->getIdWritersBook()->end(); i++) {
		ss << "SELECT id FROM writers_books WHERE writer_id = " << (*(*i)).getId() << ";";
		try {
			auto result = date_base.exec(ss.str());
			result.next();
			if (id_writer != result.get<int>(0))
				id_writer = result.get<int>(0);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\nError search id from writers_books, Position \'GatewayBooks.cpp\' str = 19\n";
		}
	}
	// insert new book in date base
	ss << "INSERT INTO books (id,cipher_of_the_book,name,id_writers,title,circulation,release_date,cost_price,sale_price,fee)"
		<< "VALUES (" << data->getId() << ", " << data->getCipherOfTheBook() << ", '"
		<< data->getName() << "', " << id_writer << ", '" << data->getTitle()
		<< "', " << data->getCirculation() << ", '" << data->getReleaseDate() << "', " << data->getCostPrice() << ", "
		<< data->getSalePrice() << ", " << data->getFee() << ");";
	try {
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror add new book, Position \'GatewayBooks.cpp\' str = 35\n";
	}
}

int GatewayBooks::get_count() {
	std::stringstream ss;
	// get number of records in table books
	ss << "SELECT COUNT(*) FROM books;";
	try {
		auto date_base = DateBaseConnect::getInstance();
		auto result = date_base.exec(ss.str());
		result.next();
		return result.get<int>(0);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror get count book, Position \'GatewayBooks.cpp\' str = 48\n";
		return -1;
	}
}

int* GatewayBooks::get_id() {
	int* res = new int[get_count()];
	int i = 0;
	std::stringstream ss;
	// get all id from table books and sorter her
	ss << "SELECT id FROM books ORDER BY id;";
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
		std::cout << e.what() << "\nEror get all id book, Position \'GatewayBooks.cpp\' str = 66\n";
		return nullptr;
	}
}

void GatewayBooks::save(Books* data) {
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	int id_writer = -1;
	std::string writer;
	// search id writers for update book 
	for (auto i = data->getIdWritersBook()->begin(); i != data->getIdWritersBook()->end() && (*i) != nullptr; i++) {
		ss << "SELECT id FROM writers_books WHERE writer_id = " << (*(*i)).getId() << ";";
		try {
			auto result = date_base.exec(ss.str());
			result.next();
			if (id_writer != result.get<int>(0))
				id_writer = result.get<int>(0);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\nEror get id from writers_books, Position \'GatewayBooks.cpp\' str = 81\n";
		}
	}
	if (id_writer == -1)
		writer = "NULL";
	else
		writer = std::to_string(id_writer);
	// update book
	ss << "UPDATE books SET cipher_of_the_book = " << data->getCipherOfTheBook() << ", name = '" << data->getName()
		<< "', id_writers = " << writer << ", title = '" << data->getTitle() << "', circulation = "
		<< data->getCirculation() << ", release_date = '" << data->getReleaseDate() << "', cost_price = "
		<< data->getCostPrice() << ", sale_price = " << data->getSalePrice() << ", fee = " << data->getFee()
		<< " WHERE id = " << data->getId() << ";";
	std::string postgreSQL = ss.str();
	try {
		date_base.exec(postgreSQL);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror update book, Position \'GatewayBooks.cpp\' str = 100\n";
	}
}

Books* GatewayBooks::get(int id) {
	std::list<int> writers;
	int writer_books = -1;
	GatewayWriters gateway_writers;
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	Books* book = nullptr;
	ss << "SELECT id,cipher_of_the_book,name,id_writers,title,circulation,release_date,cost_price,sale_price,fee FROM books WHERE id = " << id << ";";
	// get object(type = Books) from table book
	try {
		auto result = date_base.exec(ss.str());
		result.next();
		book = new Books(result.get<int>(0));
		book->setCipherOfTheBook(result.get<int>(1));
		book->setName(result.get<std::string>(2));
		writer_books = result.get<int>(3);
		book->setTitle(result.get<std::string>(4));
		book->setCirculation(result.get<int>(5));
		book->setReleaseDate(result.get<std::string>(6));
		book->setCostPrice(result.get<float>(7));
		book->setSalePrice(result.get<float>(8));
		book->setFee(result.get<float>(9));
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror get book in date base, Position \'GatewayBooks.cpp\' str = 117\n";
	}
	// select writer id
	ss << "SELECT writer_id FROM writers_books WHERE id = " << writer_books << ";";
	try {
		if (writer_books < 0 || writer_books == NULL) {
			book->setIdWritersBook(nullptr);
		}
		else {
			auto result = date_base.exec(ss.str());
			while (result.next()) {
				writers.push_back(result.get<int>(0));
			}
			for (auto i = writers.begin(); i != writers.end(); i++) {
				if ((*i) == NULL) {
					book->setIdWritersBook(nullptr);
				}
				else {
					book->setIdWritersBook(gateway_writers.get(*i));
				}
			}
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror get writer id from writers books in date base, Position \'GatewayBooks.cpp\' str = 136\n";
	}
	return book;
}

void GatewayBooks::remove(int id) {
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	// remove book in date base
	ss << "DELETE FROM books WHERE id = " << id << ";";
	try {
		date_base.exec(ss.str());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror remove from books, Position \'GatewayBooks.cpp\' str = 156\n";
	}
}

std::list<Books*>* GatewayBooks::get_N(int start, int end) {
	int writer_books = -1;
	std::list<int> writers;
	GatewayWriters gateway_writers;
	std::list<Books*> res;
	auto date_base = DateBaseConnect::getInstance();
	std::stringstream ss;
	// get all books where her id between start and end 
	ss << "SELECT * FROM books WHERE id BETWEEN " << start << " AND " << end << " ORDER BY id;";
	try {
		auto result = date_base.exec(ss.str());
		while (result.next()) {
			Books* book = new Books(result.get<int>(0));
			book->setCipherOfTheBook(result.get<int>(1));
			book->setName(result.get<std::string>(2));
			writer_books = result.get<int>(3);
			book->setTitle(result.get<std::string>(4));
			book->setCirculation(result.get<int>(5));
			book->setReleaseDate(result.get<std::string>(6));
			book->setCostPrice(result.get<float>(7));
			book->setSalePrice(result.get<float>(8));
			book->setFee(result.get<float>(9));
			// select id writer from writers_books
			ss << "SELECT writer_id FROM writers_books WHERE id = " << writer_books << ";";
			auto result = date_base.exec(ss.str());

			while (result.next()) {
				writers.push_back(result.get<int>(0));
			}
			// get all writers from one book
			for (auto i = writers.begin(); i != writers.end(); i++) {
				if ((*i) == NULL) {
					book->setIdWritersBook(nullptr);
				}
				else {
					book->setIdWritersBook(gateway_writers.get(*i));
				}
			}
			// clear list writers it is used more
			writers.clear();

			res.push_back(book);
		}
		return &res;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << "\nEror get N, Position \'GatewayBooks.cpp\' str = 173 or 187\n";
		return nullptr;
	}
}