#pragma once
#include <string>
#include <list>
#include "../Writers/Writers.h"

class Books {
private:
	int id;
	int cipher_of_the_book;
	std::string name;
	std::list<Writers*> id_writers_book;
	std::string title;
	int circulation;
	std::string release_date;
	float cost_price;
	float sale_price;
	float fee;
public:
	Books(int id);

	int getId();
	void setCipherOfTheBook(int cipher_of_the_book);
	int getCipherOfTheBook();
	void setName(std::string name);
	std::string getName();
	void setIdWritersBook(Writers* writer);
	std::list<Writers*>* getIdWritersBook();
	void setTitle(std::string title);
	std::string getTitle();
	void setCirculation(int citculation);
	int getCirculation();
	void setReleaseDate(std::string release_date);
	std::string getReleaseDate();
	void setCostPrice(float cost_price);
	float getCostPrice();
	void setSalePrice(float sale_price);
	float getSalePrice();
	void setFee(float fee);
	float getFee();

	friend std::ostream& operator<< (std::ostream& os, Books& data);
};