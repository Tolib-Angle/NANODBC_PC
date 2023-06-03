#pragma once
#include <string>
#include <list>
#include "../Writers/Writers.h"

class Contracts {
private:
	int id;
	Writers* id_writers;
	int contract_num;
	std::string date_of_cons_contract;
	int term_of_the_contract;
	bool validy_of_the_contract;
	std::string date_of_the_terminition_contract;
public:
	Contracts(int id);

	int getId();
	void setIdWriters(Writers* writer);
	Writers* getIdWriters();
	void setContractNum(int contract_num);
	int getContractNum();
	void setDateOfConsContract(std::string date_of_cons_contract);
	std::string getDateOfConsContract();
	void setTermOfTheContract(int term_of_the_contract);
	int getTermOfTheContract();
	void setValidyOfTheContract(bool validy_of_the_contract);
	bool getValidyOfTheContract();
	void setDateOfTheTerminitionContract(std::string date_of_the_terminition_contract);
	std::string getDateOfTheTerminitionContract();

	friend std::ostream& operator<< (std::ostream& os, Contracts& contract);
};