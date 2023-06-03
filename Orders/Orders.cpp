#pragma once
#include <iomanip>
#include "Orders.h"
#include <sstream>

Orders::Orders(int id) {
	this->id = id;
}

int Orders::getId() {
	return this->id;
}

void Orders::setIdCustomers(Customers* customer) {
	this->id_customers = customer;
}

Customers* Orders::getIdCustomers() {
	return this->id_customers;
}

void Orders::setOrderNumber(int order_number) {
	this->order_number = order_number;
}

int Orders::getOrderNumber() {
	return this->order_number;
}

void Orders::setDateOfReceiptOrder(std::string date_of_receipt_order) {
	this->date_of_receipt_order = date_of_receipt_order;
}

std::string Orders::getDateOfReceiptOrder() {
	return this->date_of_receipt_order;
}

void Orders::setOrderCompletionDate(std::string order_completion_date) {
	this->order_completion_date = order_completion_date;
}

std::string Orders::getOrderCompletionDate() {
	return this->order_completion_date;
}

void Orders::setIdBooks(Books* book) {
	this->id_books = book;
}

Books* Orders::getIdBooks() {
	return this->id_books;
}

void Orders::setNumbersOfOrders(int numbers_of_orders) {
	this->numbers_of_orders = numbers_of_orders;
}

int Orders::getNumbersOfOrders() {
	return this->numbers_of_orders;
}

std::ostream& operator<<(std::ostream& os, Orders& order) {
	int q1 = 30 - order.getIdCustomers()->getFullNameCustomer().size(), q2 = 4, q3 = 4, q4 = 4, q5 = 40 - order.getIdBooks()->getTitle().size(), q6 = 10;
	if (order.getNumbersOfOrders() >= 0 && order.getNumbersOfOrders() < 10)
		q6 = 9;
	else if (order.getNumbersOfOrders() >= 10 && order.getNumbersOfOrders() <= 99)
		q6 = 8;
	else if (order.getNumbersOfOrders() >= 100 && order.getNumbersOfOrders() <= 999)
		q6 = 7;
	else if (order.getNumbersOfOrders() >= 1000 && order.getNumbersOfOrders() <= 9999)
		q6 = 6;
	else
		q6 = 5;
	os << "|" << order.getIdCustomers()->getFullNameCustomer() << std::setw(q1) << "|" << order.getOrderNumber() << std::setw(q2) << "|"
		<< order.getDateOfReceiptOrder() << std::setw(q3) << "|" << order.getOrderCompletionDate() << std::setw(q4) << "|"
		<< order.getIdBooks()->getTitle() << std::setw(q5) << "|" << order.getNumbersOfOrders() << std::setw(q6) << "|";
	return os;
}