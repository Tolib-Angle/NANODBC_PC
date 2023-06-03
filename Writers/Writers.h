#pragma once
#include <string>

class Writers {
private:
	int id;
	int passport_number;
	std::string surname;
	std::string name;
	std::string middle_name;
	std::string address;
	std::string phone;
public:
	Writers(int id);

	int getId() const;
	const int getPassportNumber();
	void setPassportNumber(int passport_number);
	const std::string getSurname();
	void setSurname(std::string surname);
	const std::string getName();
	void setName(std::string name);
	const std::string getMiddleName();
	void setMiddleName(std::string middle_name);
	const std::string getAddress();
	void setAddress(std::string address);
	const std::string getPhone();
	void setPhone(std::string phone);

	friend std::ostream& operator<< (std::ostream& os, Writers& data);
};