#pragma once
#define SIZE_CASH 30

template<class T>
class Cash {
private:
	T* cash[SIZE_CASH];
	int keys[SIZE_CASH];
	int current_position;
	int size;
public:
	Cash() {
		for (int i = 0; i < SIZE_CASH; i++) {
			cash[i] = nullptr;
			keys[i] = -1;
		}
		current_position = 0;
		size = 0;
	}
	void add(T* object, int key) {
		if (current_position >= SIZE_CASH) {
			current_position = 0;
			cash[current_position] = object;
			keys[current_position] = key;
		}
		else {
			cash[current_position] = object;
			keys[current_position] = key;
			current_position++;
		}
		size++;
	}
	bool search(int key) {
		for (int i = 0; i < SIZE_CASH; i++) {
			if (keys[i] == key)
				return true;
		}
		return false;
	}
	T* get(int key) {
		for (int i = 0; i < SIZE_CASH; i++) {
			if (keys[i] == key)
				return cash[i];
		}
		return nullptr;
	}
	void remove(int key) {
		int position = -1;
		for (int i = 0; i < SIZE_CASH; i++) {
			if (keys[i] == key)
				position = -1;
		}
		if (position != -1) {
			for (int i = position; i < SIZE_CASH - 1; i++) {
				cash[i] = cash[i + 1];
				keys[i] = keys[i + 1];
				if (size != SIZE_CASH)
					size--;
			}
		}

	}
	int get_size() {
		return this->size;
	}
	size_t get_size_cash() {
		size_t all_size = 0;
		for (int i = 0; i < SIZE_CASH; i++) {
			if (cash[i] != nullptr)
				all_size += sizeof(T);
		}
		return all_size;
	}
};