#pragma once
#include <string>

class Customers {
private:
	int id;
	std::string customer_name;
	std::string address;
	std::string phone;
	std::string full_name_customer;
public:
	Customers(int id);

	int getId();
	void setCustomerName(std::string customer_name);
	std::string getCustomerName();
	void setAddress(std::string address);
	std::string getAddress();
	void setPhone(std::string phone);
	std::string getPhone();
	void setFullNameCustomer(std::string full_name_customer);
	std::string getFullNameCustomer();

	friend std::ostream& operator<< (std::ostream& os, Customers& customer);
};