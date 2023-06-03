#pragma once
#include <nanodbc/nanodbc.h>
#include <string>

class DateBaseConnect {
private:
	// for connection date base
	nanodbc::connection connection;
	DateBaseConnect();
	// check table, function and trigger in the DB
	bool check_table(std::string name);
	bool check_function(std::string name);
	bool check_trigger(std::string name);
	// Inittialization
	void init();
public:
	static DateBaseConnect& getInstance();
	nanodbc::result exec(std::string postgreSQL);
};