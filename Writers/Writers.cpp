#pragma once
#include "Writers.h"
#include <iomanip>

Writers::Writers(int id) {
	this->id = id;
}

int Writers::getId() const {
	return this->id;
}

const int Writers::getPassportNumber() {
	return this->passport_number;
}

void Writers::setPassportNumber(int passport_number) {
	this->passport_number = passport_number;
}

const std::string Writers::getSurname() {
	return this->surname;
}

void Writers::setSurname(std::string surname) {
	this->surname = surname;
}

const std::string Writers::getName() {
	return this->name;
}

void Writers::setName(std::string name) {
	this->name = name;
}

const std::string Writers::getMiddleName() {
	return this->middle_name;
}

void Writers::setMiddleName(std::string middle_name) {
	this->middle_name = middle_name;
}

const std::string Writers::getAddress() {
	return this->address;
}

void Writers::setAddress(std::string address) {
	this->address = address;
}

const std::string Writers::getPhone() {
	return this->phone;
}

void Writers::setPhone(std::string phone) {
	this->phone = phone;
}

std::ostream& operator<< (std::ostream& os, Writers& data) {
	int q1 = 2, q2 = 5, q3 = 20 - data.surname.size(), q4 = 20 - data.name.size(), q5 = 20 - data.getMiddleName().size();
	int q6 = 40 - data.getAddress().size(), q7 = 20 - data.getPhone().size();
	os << "|" << data.getPassportNumber() << std::setw(q2) << "|" << data.getSurname() << std::setw(q3) << "|"
		<< data.getName() << std::setw(q4) << "|" << data.getMiddleName() << std::setw(q5) << "|" << data.getAddress()
		<< std::setw(q6) << "|" << data.getPhone() << std::setw(q7) << "|";
	return os;
}