#pragma once
#include "Books.h"
#include <iomanip>
#include <sstream>

Books::Books(int id) {
	this->id = id;
}

int Books::getId() {
	return this->id;
}

void Books::setCipherOfTheBook(int cipher_of_the_book) {
	this->cipher_of_the_book = cipher_of_the_book;
}

int Books::getCipherOfTheBook() {
	return this->cipher_of_the_book;
}

void Books::setName(std::string name) {
	this->name = name;
}

std::string Books::getName() {
	return this->name;
}

void Books::setIdWritersBook(Writers* writer) {
	this->id_writers_book.push_back(writer);
}

std::list<Writers*>* Books::getIdWritersBook() {
	return &this->id_writers_book;
}

void Books::setTitle(std::string title) {
	this->title = title;
}

std::string Books::getTitle() {
	return this->title;
}

void Books::setCirculation(int citculation) {
	this->circulation = citculation;
}

int Books::getCirculation() {
	return this->circulation;
}

void Books::setReleaseDate(std::string release_date) {
	this->release_date = release_date;
}

std::string Books::getReleaseDate() {
	return this->release_date;
}

void Books::setCostPrice(float cost_price) {
	this->cost_price = cost_price;
}

float Books::getCostPrice() {
	return this->cost_price;
}

void Books::setSalePrice(float sale_price) {
	this->sale_price = sale_price;
}

float Books::getSalePrice() {
	return this->sale_price;
}

void Books::setFee(float fee) {
	this->fee = fee;
}

float Books::getFee(){
	return this->fee;
}

std::ostream& operator<<(std::ostream& os, Books& data) {
	std::stringstream ss;
	for (auto i = data.getIdWritersBook()->begin(); i != data.getIdWritersBook()->end(); i++) {
		if ((*i) == nullptr)
			ss << "Автор отсутствует! ";
		else
			ss << (*i)->getName() << " " << (*i)->getMiddleName()[0] << "." << (*i)->getSurname()[0] << ".  ";
	}
	int q1 = 12 - (std::to_string(data.getCipherOfTheBook())).size(),  q4 = 50 - data.getTitle().size(), q5 = 16 - (std::to_string(data.getCirculation())).size();
	int q2 = 40 - data.getName().size(), q3 = 50 - ss.str().size(), q6 = 4;
	int q7 = 20 - (std::to_string(data.getCostPrice())).size(), q8 = 20 - (std::to_string(data.getSalePrice())).size();
	int q9 = 20 - (std::to_string(data.getFee())).size();
	os << "|" << data.getCipherOfTheBook() << std::setw(q1) << "|" << data.getName() << std::setw(q2) << "|"
		<< ss.str() << std::setw(q3) << "|" << data.getTitle() << std::setw(q4) << "|" << data.getCirculation() << std::setw(q5)
		<< "|" << data.getReleaseDate() << std::setw(q6) << "|" << data.getCostPrice() << std::setw(q7) << "|"
		<< data.getSalePrice() << std::setw(q8) << "|" << data.getFee() << std::setw(q9) << "|";
	return os;
}