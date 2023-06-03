#pragma once
#include <list>
#include <string>

template <class T>
class Gateway {
public:
	// add new object
	virtual void add(T* data) = 0;
	// return all id from table
	virtual int* get_id() = 0;
	// return count elemnts in the table
	virtual int get_count() = 0;
	// save data in DateBase
	virtual void save(T* data) = 0;
	// get data from DateBase where id = id
	virtual T* get(int id) = 0;
	// remove data from DateBase where id = id
	virtual void remove(int id) = 0;
	// get N data from DateBase
	virtual std::list<T*>* get_N(int start, int end) = 0;
};

class GatewayException : public std::exception {
public:
	GatewayException(GatewayException& it) noexcept = default;
	explicit GatewayException(std::string msg) : msg_(msg) {}
	~GatewayException() noexcept override = default;
	[[nodiscard]] const char* what() const noexcept override {
		return msg_.c_str();
	}
private:
	std::string msg_;
};