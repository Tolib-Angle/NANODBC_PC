#pragma once
#include <iomanip>
#include "Customers.h"

Customers::Customers(int id) {
	this->id = id;
}

int Customers::getId() {
	return this->id;
}

void Customers::setCustomerName(std::string customer_name) {
	this->customer_name = customer_name;
}

std::string Customers::getCustomerName() {
	return this->customer_name;
}

void Customers::setAddress(std::string address) {
	this->address = address;
}

std::string Customers::getAddress() {
	return this->address;
}

void Customers::setPhone(std::string phone) {
	this->phone = phone;
}

std::string Customers::getPhone() {
	return this->phone;
}

void Customers::setFullNameCustomer(std::string full_name_customer) {
	this->full_name_customer = full_name_customer;
}

std::string Customers::getFullNameCustomer() {
	return this->full_name_customer;
}

std::ostream& operator<<(std::ostream& os, Customers& customer) {
	int q1 = 50 - customer.getCustomerName().size(), q2 = 40 - customer.getAddress().size(), q3 = 30 - customer.getPhone().size();
	int q4 = 40 - customer.getFullNameCustomer().size();
	os << "|" << customer.getCustomerName() << std::setw(q1) << "|" << customer.getAddress() << std::setw(q2) << "|"
		<< customer.getPhone() << std::setw(q3) << "|" << customer.getFullNameCustomer() << std::setw(q4) << "|";
	return os;
}