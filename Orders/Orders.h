#pragma once
#include <string>
#include <list>
#include "../Customers/Customers.h"
#include "../Books/Books.h"

class Orders {
private:
	int id;
	Customers* id_customers;
	int order_number;
	std::string date_of_receipt_order;
	std::string order_completion_date;
	Books* id_books;
	int numbers_of_orders;
public:
	Orders(int id);

	int getId();
	void setIdCustomers(Customers* customer);
	Customers* getIdCustomers();
	void setOrderNumber(int order_number);
	int getOrderNumber();
	void setDateOfReceiptOrder(std::string date_of_receipt_order);
	std::string getDateOfReceiptOrder();
	void setOrderCompletionDate(std::string order_completion_date);
	std::string getOrderCompletionDate();
	void setIdBooks(Books* book);
	Books* getIdBooks();
	void setNumbersOfOrders(int numbers_of_orders);
	int getNumbersOfOrders();

	friend std::ostream& operator<< (std::ostream& os, Orders& order);
};