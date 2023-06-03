#include "DateBaseConnect.h"
#include <sstream>

DateBaseConnect::DateBaseConnect() {
	auto const connect_str = NANODBC_TEXT("DRIVER={PostgreSQL Unicode};SERVER=localhost,1433;UID=postgres;PWD=12345;database=PublishingCenter;");
	connection = nanodbc::connection(connect_str);
	init();
}

DateBaseConnect& DateBaseConnect::getInstance() {
	static DateBaseConnect instance;
	return instance;
}

nanodbc::result DateBaseConnect::exec(std::string posgreSQL) {
	return nanodbc::execute(connection, posgreSQL);
}

bool DateBaseConnect::check_table(std::string name) {
	std::stringstream str;
	str << "SELECT * FROM pg_tables where schemaname = 'public' and tablename = '" << name << "';";
	auto response = exec(str.str());
	while (response.next()) {
		return true;
	}
	return false;
}

bool DateBaseConnect::check_trigger(std::string name) {
	std::stringstream str;
	str << "SELECT * FROM pg_trigger where tgname = '" << name << "';";
	auto response = exec(str.str());
	while (response.next()) {
		return true;
	}
	return false;
}

bool DateBaseConnect::check_function(std::string name) {
	std::stringstream str;
	str << "SELECT routine_name FROM information_schema.routines where routine_type = 'FUNCTION' and routine_schema = 'public' and routine_name = '" << name << "';";
	auto response = exec(str.str());
	while (response.next()) {
		return true;
	}
	return false;
}

void DateBaseConnect::init() {
	// tables
	bool writers = check_table("writers");
	bool contracts = check_table("contracts");
	bool books = check_table("books");
	bool customers = check_table("customers");
	bool orders = check_table("orders");
	bool writers_books = check_table("writers_books");
	// functions
	bool func_customers = check_function("dell_customers_trig");
	bool func_books = check_function("dell_books_trig");
	bool func_writers = check_function("dell_writers_trig");
	bool func_orders = check_function("dell_orders_trig");
	// triggers
	bool trig_customers = check_trigger("customers_delete");
	bool trig_books = check_trigger("books_delete");
	bool trig_writers = check_trigger("writers_delete");
	bool trig_orders = check_trigger("orders_delete");

	// create table writers
	if (!writers) {
		std::stringstream str;
		str << "CREATE TABLE writers( id serial, passport_number int NOT NULL CHECK(passport_number BETWEEN 10000000 AND 99999999),"
			<< "surname varchar NOT NULL CHECK(surname <> ''), name varchar NOT NULL CHECK(name <> ''),"
			<< "middle_name varchar, address varchar NOT NULL CHECK(address <> ''), phone text NOT NULL,"
			<< "CONSTRAINT primary_key_id_writers PRIMARY KEY(id));";
		exec(str.str());
		writers = !writers;
	}
	// create table contracts
	if (!contracts && writers) {
		std::stringstream str;
		str << "CREATE TABLE contracts(id serial, id_writer int NOT NULL CHECK(id_writer > 0), "
			<< "contract_number int NOT NULL CHECK(contract_number BETWEEN 10000000 AND 99999999), date_of_cons_contract date NOT NULL, "
			<< "term_of_the_contract int NOT NULL, validy_of_the_contract bool NOT NULL, "
			<< "date_of_terminition_contract date NOT NULL,	CONSTRAINT primary_key_id_contracts PRIMARY KEY(id), "
			<< "CONSTRAINT foreign_key_id_writers FOREIGN KEY(id_writer) REFERENCES writers(id));";
		exec(str.str());
		contracts = !contracts;
	}
	// create table customers
	if (!customers) {
		std::stringstream str;
		str << "CREATE TABLE customers(id serial, customer_name varchar NOT NULL CHECK(customer_name < > ''), "
			<< "address varchar NOT NULL CHECK(address <> ''), phone text NOT NULL, full_name_customer varchar NOT NULL CHECK(full_name_customer < > ''), "
			<< "CONSTRAINT primary_key_id_customers PRIMARY KEY(id));";
		exec(str.str());
		customers = !customers;
	}
	// create help_table writers_books
	if (!writers_books && writers) {
		std::stringstream str;
		str << "CREATE TABLE writers_books(id serial, writer_id int CHECK(writer_id > 0), "
			<< "CONSTRAINT primary_key_writers_books PRIMARY KEY(id),"
			<< "CONSTRAINT foreign_key_id_writers FOREIGN KEY(writer_id) REFERENCES writers(id));";
		exec(str.str());
		writers_books = !writers_books;
	}
	// create table books
	if (!books && writers_books) {
		std::stringstream str;
		str << "CREATE TABLE books(id serial, cipher_of_the_book int NOT NULL CHECK(cipher_of_the_book BETWEEN 10000000 AND 99999999), "
			<< "name varchar NOT NULL CHECK(name <> ''), id_writers int NOT NULL CHECK(id_writers > 0), title varchar NOT NULL CHECK(title < > ''), "
			<< "circulation int NOT NULL CHECK(circulation > 0), release_date date NOT NULL, cost_price real NOT NULL CHECK(cost_price > 0), "
			<< "sale_price real NOT NULL CHECK(sale_price > 0), fee real NOT NULL CHECK(fee > 0), "
			<< "CONSTRAINT primary_key_id_books PRIMARY KEY(id), "
			<< "CONSTRAINT foreign_key_id_writers_books FOREIGN KEY(id_writers) REFERENCES writers_books(id));";
		exec(str.str());
		books = !books;
	}
	// create table orders
	if (!orders && customers && books) {
		std::stringstream str;
		str << "CREATE TABLE orders("
			<< "id serial,"
			<< "id_customer int NOT NULL CHECK(id_customer > 0),"
			<< "order_number int NOT NULL CHECK(order_number BETWEEN 10000000 AND 99999999),"
			<< "date_of_receipt_order date NOT NULL,"
			<< "order_completion_date date NOT NULL, "
			<< "id_book int NOT NULL CHECK(id_book > 0), "
			<< "numbers_of_order int NOT NULL CHECK(numbers_of_order > 0), "
			<< "CONSTRAINT primary_key_id_orders PRIMARY KEY(id), "
			<< "CONSTRAINT foreign_key_id_costomer FOREIGN KEY(id_customer) REFERENCES customers(id), "
			<< "CONSTRAINT foreign_key_id_book FOREIGN KEY(id_book) REFERENCES books(id)); ";
		exec(str.str());
		orders = !orders;
	}
	// create functions
	if (!func_books) {
		std::stringstream str;
		str << "CREATE OR REPLACE FUNCTION dell_books_trig() RETURNS TRIGGER AS $$"
			<< "BEGIN"
			<< "DELETE FROM orders WHERE id_book = OLD.id;"
			<< "RETURN OLD;"
			<< "END;"
			<< "$$ LANGUAGE plpgsql; ";
		exec(str.str());
		func_books = !func_books;
	}
	if (!func_customers) {
		std::stringstream str;
		str << "CREATE OR REPLACE FUNCTION dell_customers_trig() RETURNS TRIGGER AS $$"
			<< "BEGIN"
			<< "DELETE FROM orders WHERE id_customer = OLD.id;"
			<< "RETURN OLD;"
			<< "END;"
			<< "$$ LANGUAGE plpgsql; ";
		exec(str.str());
		func_customers = !func_customers;
	}
	if (!func_orders) {
		std::stringstream str;
		str << "CREATE OR REPLACE FUNCTION dell_writers_trig() RETURNS TRIGGER AS $$"
			<< "BEGIN"
			<< "DELETE FROM contracts WHERE id = OLD.id;"
			<< "UPDATE writers_books SET writer_id = NULL WHERE writer_id = OLD.id;"
			<< "RETURN OLD;"
			<< "END;"
			<< "$$ LANGUAGE plpgsql; ";
		exec(str.str());
		func_orders = !func_orders;
	}
	if (!func_writers) {
		std::stringstream str;
		str << "CREATE OR REPLACE FUNCTION dell_orders_trig() RETURNS TRIGGER AS $$"
			<< "BEGIN"
			<< "DELETE FROM customers WHERE customers.id = OLD.id_customer;"
			<< "RETURN OLD;"
			<< "END;"
			<< "$$ LANGUAGE plpgsql; ";
		exec(str.str());
		func_writers = !func_writers;
	}
	// create triggers
	if (!trig_books && func_books) {
		std::stringstream str;
		str << "CREATE TRIGGER books_delete"
			<< "BEFORE DELETE ON books"
			<< "FOR EACH ROW EXECUTE PROCEDURE dell_books_trig(); ";
		exec(str.str());
	}
	if (!trig_customers && func_customers) {
		std::stringstream str;
		str << "CREATE TRIGGER customers_delete"
			<< "BEFORE DELETE ON customers"
			<< "FOR EACH ROW EXECUTE PROCEDURE dell_customers_trig(); ";
		exec(str.str());
	}
	if (!trig_orders && func_orders) {
		std::stringstream str;
		str << "CREATE TRIGGER orders_delete"
			<< "BEFORE DELETE ON orders"
			<< "FOR EACH ROW EXECUTE PROCEDURE dell_orders_trig(); ";
		exec(str.str());
	}
	if (!trig_writers && func_writers) {
		std::stringstream str;
		str << "CREATE TRIGGER writers_delete"
			<< "BEFORE DELETE ON writers"
			<< "FOR EACH ROW EXECUTE PROCEDURE dell_writers_trig(); ";
		exec(str.str());
	}
}