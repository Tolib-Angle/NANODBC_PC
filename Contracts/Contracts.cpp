#pragma once
#include "Contracts.h"
#include <iomanip>
#include <sstream>

Contracts::Contracts(int id) {
	this->id = id;
}

int Contracts::getId() {
	return this->id;
}

void Contracts::setIdWriters(Writers* writer) {
	this->id_writers = writer;
}

Writers* Contracts::getIdWriters() {
	return this->id_writers;
}

void Contracts::setContractNum(int contract_num) {
	this->contract_num = contract_num;
}

int Contracts::getContractNum() {
	return this->contract_num;
}

void Contracts::setDateOfConsContract(std::string date_of_cons_contract) {
	this->date_of_cons_contract = date_of_cons_contract;
}

std::string Contracts::getDateOfConsContract() {
	return this->date_of_cons_contract;
}

void Contracts::setTermOfTheContract(int term_of_the_contract) {
	this->term_of_the_contract = term_of_the_contract;
}

int Contracts::getTermOfTheContract() {
	return this->term_of_the_contract;
}

void Contracts::setValidyOfTheContract(bool validy_of_the_contract) {
	this->validy_of_the_contract = validy_of_the_contract;
}

bool Contracts::getValidyOfTheContract() {
	return this->validy_of_the_contract;
}

void Contracts::setDateOfTheTerminitionContract(std::string date_of_the_terminition_contract) {
	this->date_of_the_terminition_contract = date_of_the_terminition_contract;
}

std::string Contracts::getDateOfTheTerminitionContract() {
	return this->date_of_the_terminition_contract;
}

std::ostream& operator<<(std::ostream& os, Contracts& contract) {
	std::stringstream ss;
	ss << contract.getIdWriters()->getName() << " " << contract.getIdWriters()->getMiddleName()[0] << "." << contract.getIdWriters()->getSurname()[0] << ".  ";
	int q1 = 50 - ss.str().size(), q2 = 4, q3 = 4, q4 = 8, q5 = 8, q6 = 4;
	if (contract.term_of_the_contract >= 0 && contract.term_of_the_contract < 10)
		q4 = 7;
	else if (contract.term_of_the_contract >= 10 && contract.term_of_the_contract <= 99)
		q4 = 6;
	else if (contract.term_of_the_contract >= 100 && contract.term_of_the_contract <= 999)
		q4 = 5;
	else
		q4 = 4;
	if (contract.validy_of_the_contract)
		q5 -= 4;
	else
		q5 -= 5;
	os << "|" << ss.str() << std::setw(q1) << "|" << contract.contract_num << std::setw(q2) << "|"
		<< contract.date_of_cons_contract << std::setw(q3) << "|" << contract.term_of_the_contract << std::setw(q4) << "|"
		<< (contract.validy_of_the_contract ? "true" : "false") << std::setw(q5) << "|" << contract.date_of_the_terminition_contract
		<< std::setw(q6) << "|";
	return os;
}