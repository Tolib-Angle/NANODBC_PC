#pragma once
#include "InterfaceTables.h"

InterfaceTables& InterfaceTables::getInstance() {
	static InterfaceTables instance;
	return instance;
}

void InterfaceTables::start() {
	InterfaceTables tables = getInstance();
	int user_table = -1;
	while (true) {
		std::cout << "\t\t------ Таблицы ------" << std::endl;
		user_table = enterNumber(1, 6, "1. Писатели\n2. Контракты\n3. Книги\n4. Заказчики\n5. Заказы\n6. Выход\nВведите номер таблицы");
		switch (user_table) {
		case 1: {
			system("cls");
			table_writers();
			system("cls");
		} break;
		case 2: {
			system("cls");
			table_contracts();
			system("cls");
		} break;
		case 3: {
			system("cls");
			table_books();
			system("cls");
		} break;
		case 4: {
			system("cls");
			table_customers();
			system("cls");
		} break;
		case 5: {
			system("cls");
			table_orders();
			system("cls");
		} break;
		case 6:
			return;
		}
		system("cls");
	}
}

void InterfaceTables::table_writers() {
	Writers* writer;
	int user_commands = -1, begin = 1, end = gateway_writers.get_count(), *all_id = gateway_writers.get_id(), user_id = 1;
	int page = 1, all_pages = (int)std::ceil((double)end / (double)BLOCK), setw_user_id = 8;
	std::string commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
	double used_memory = 0;
	while (true) {
		// list clear
		list_writers.clear();
		user_id = BLOCK * (page - 1) + 1;
		// get records in the cash or date base
		for (int i = begin; (i < begin + BLOCK); i++) {
			if (cash_writers.search(all_id[i - 1])) {
				list_writers.push_back(cash_writers.get(all_id[i - 1]));
			}
			else {
				try {
					writer = gateway_writers.get(all_id[i - 1]);
					cash_writers.add(writer, writer->getId());
					list_writers.push_back(writer);
				}
				catch (const std::exception& e) {
					std::cout << e.what() << std::endl;
				}
			}
			// check last page (maybe not 10 elment in the current page)
			if (page == all_pages) {
				if (i == end)
					break;
			}
		}
		// show writers
		std::cout << "\t\t\t\t\t\t------------- Писателей -------------" << std::endl;
		std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
		std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
		std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
		for (auto i = list_writers.begin(); i != list_writers.end(); i++) {
			if (user_id >= 1 && user_id < 10)
				setw_user_id = 7;
			if (user_id >= 10 && user_id <= 99)
				setw_user_id = 6;
			std::cout << "|" << user_id << std::setw(setw_user_id) << *(*i) << std::endl;
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			user_id++;
		}
		std::cout << "\t\t\t\t\t\t---------- Страница " << page << " из " << all_pages << " ----------\n";
		// calculate memory used
		used_memory = (double)(cash_writers.get_size_cash() + BLOCK * sizeof(std::list<Writers*>) + sizeof(Writers*) + sizeof(int) * end + cash_books.get_size_cash() + cash_contracts.get_size_cash() + cash_customers.get_size_cash() + cash_orders.get_size_cash());
		// memory used
		std::cout << "Использованная память: " << used_memory  << " байт" << std::endl;
		// commands for user
		std::cout << commands_for_user << std::endl;
		// enter user command
		if (page == 1) {
			user_commands = enterNumber(1, 6, "Введите номер команды");
			if (user_commands != 1)
				user_commands += 1;
		}
		else if (page == all_pages) {
			user_commands = enterNumber(1, 6, "Введите номер команды");
			user_commands += 1;
		}
		else {
			user_commands = enterNumber(1, 7, "Введите номер команды");
		}
		// all commands for table writers
		switch (user_commands)
		{
		// next 
		case 1: {
			system("cls");
			// next BLOCK and page
			begin += BLOCK;
			page += 1;
			// new commands for user but last page not have NEXT and first page not have PREV
			if (page == all_pages)
				commands_for_user = "1. Назад 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != 1)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// prev
		case 2: {
			system("cls");
			// prev BLOCK and page
			begin -= BLOCK;
			page -= 1;
			// new commands for user but last page not have NEXT and first page not have PREV
			if(page == 1)
				commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != all_pages)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// add
		case 3: {
			bool flag = false;
			// search new id for new writer
			int new_id = -1, setw_new_id = 8;
			// if id absent in the all_id, Example: all_id[1, 2, 3, 5, 6, 7,..., N] and new_id = 4 
			// else all_id[1, 2, 3, 4, 5, 6, 7,..., N] and new_id = N + 1
			for (int i = 1; i < end + 1; i++) {
				for (int j = 0; j < end; j++) {
					if (i == all_id[j]) {
						flag = true;
					}
				}
				if (!flag)
					new_id = i;
				flag = false;
			}
			if (new_id == -1)
				new_id = end + 1;
			if (end + 1 >= 1 && end + 1 <= 9)
				setw_new_id = 7;
			else if (end + 1 >= 10 && end + 1 <= 99)
				setw_new_id = 6;
			// create new writer
			Writers* temp_writer = new Writers(new_id);
			temp_writer->setPassportNumber(enterNumber(10000000, 99999999, "Введите номер пасспорта! 8 - цифр"));
			temp_writer->setSurname(enterString(2, 20, "Введите фамилия"));
			temp_writer->setName(enterString(2, 20, "Введите имя"));
			temp_writer->setMiddleName(enterString(2, 20, "Введите отчества"));
			temp_writer->setAddress(enterString(2, 40, "Введите адрес"));
			temp_writer->setPhone(enterString(2, 20, "Введите телефон"));
			// show new writer
			std::cout << "Новый запис: \n";
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			std::cout << "|" << end + 1 << std::setw(setw_new_id) << *temp_writer << std::endl;
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			// add or not add?
			int user = enterNumber(1, 2, "Подтвердите добавление\t\t 1 - подтвердить 2 - не подтвердить");
			if (user == 1) {
				// pushing new writer in date base and in cash
				gateway_writers.add(temp_writer);
				cash_writers.add(temp_writer, temp_writer->getId());
				// update all_id and end
				all_id = gateway_writers.get_id();
				end = gateway_writers.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// remove
		case 4: {
			int delete_id = -1, que = 0, setw_delete_id = 8;
			delete_id = enterNumber(1, end, "Введите номер записи котрого нужно удалить");
			// if id in date base started not 1 but uncomment next string code
			delete_id = all_id[delete_id - 1];
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			// search id writers where user_id = delete_id
			if (delete_id >= 1 && delete_id <= 9)
				setw_delete_id = 7;
			else if (delete_id >= 10 && delete_id <= 99)
				setw_delete_id = 6;
			// check cash if record in the cash get record but check date base
			if (cash_writers.search(delete_id)) {
				std::cout << "|" << delete_id << std::setw(setw_delete_id) << *(cash_writers.get(delete_id)) << std::endl;
			}
			else {
				writer = gateway_writers.get(delete_id);
				std::cout << "|" << delete_id << std::setw(setw_delete_id) << *writer << std::endl;
			}
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			std::cout << "Вы точно хотите удалить запись?\n1 - Да 2 - Нет" << std::endl;
			que = enterNumber(1, 2, "Подтвердите удаление");
			if (que == 1) {
				// delete record in the date base and cash
				gateway_writers.remove(delete_id);
				cash_writers.remove(delete_id);
				// update all_id and end
				all_id = gateway_writers.get_id();
				end = gateway_writers.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// update
		case 5: {
			// get update number
			int edit_id = -1, setw_edit_id = 8;
			edit_id = enterNumber(1, end, "Введите номер записи котрого нужно редактировать");
			// search update id
			edit_id = all_id[edit_id - 1];
			// show update id
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			// for setw update number for show
			if (edit_id >= 1 && edit_id <= 9)
				setw_edit_id = 7;
			else if (edit_id >= 10 && edit_id <= 99)
				setw_edit_id = 6;
			// check cash
			if (cash_writers.search(edit_id)) {
				std::cout << "|" << edit_id << std::setw(setw_edit_id) << *(cash_writers.get(edit_id)) << std::endl;
				writer = cash_writers.get(edit_id);
				cash_writers.remove(edit_id);
			}
			// get record in date base if not in the cash record
			else {
				writer = gateway_writers.get(edit_id);
				std::cout << "|" << edit_id << std::setw(setw_edit_id) << *writer << std::endl;
			}
			std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
			// update paarmetrs
			while (true) {
				std::cout << "Какое параметр хотите редактировать?\n1. Номер паспорта 2. Фамилия 3. Имя 4. Отчества 5. Адрес 6. Номер телефона 7. Сохранить изменение 8. Выход" << std::endl;
				int parametr = enterNumber(1, 8, "Выберите параметр");
				if (parametr == 1)
					writer->setPassportNumber(enterNumber(10000000, 99999999, "Введите новый номер паспорта! 8 - цифр"));
				if (parametr == 2)
					writer->setSurname(enterString(2, 20, "Введите новый фамилия"));
				if (parametr == 3)
					writer->setName(enterString(2, 20, "Введите новый имя"));
				if (parametr == 4)
					writer->setMiddleName(enterString(2, 20, "Введите новый отчества"));
				if (parametr == 5)
					writer->setAddress(enterString(2, 40, "Введите новый адрес"));
				if (parametr == 6)
					writer->setPhone(enterString(2, 20, "Введите новый номер телефона"));
				// save update
				if (parametr == 7) {
					// update parametrs in the date base
					gateway_writers.save(writer);
					cash_writers.add(writer, writer->getId());
					// update value all_id and end
					all_id = gateway_writers.get_id();
					end = gateway_writers.get_count();
				}
				// end update
				if (parametr == 8)
					break;
			}
			system("cls");
		} break;
		// show all
		case 6: {
			std::cout << "Исходные записи уже отображены подробно!" << std::endl;
			system("cls");
		} break;
		// exit
		case 7:
			return;
		}
	}
}

void InterfaceTables::table_contracts() {
	GatewayContracts gateway_contract;
	std::list<Contracts*> list_contracts;
	Contracts* contract;
	int user_commands = -1, begin = 1, end = gateway_contract.get_count(), * all_id = gateway_contract.get_id(), user_id = 1;
	int page = 1, all_pages = (int)std::ceil((double)end / (double)BLOCK), setw_user_id = 8, used_memory = 0;
	std::string commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход", for_show_all = "";
	bool flag_show_all = false;
	while (true) {
		list_contracts.clear();
		user_id = BLOCK * (page - 1) + 1;
		// get records in the cash or date base
		for (int i = begin; i < begin + BLOCK; i++) {
			// check cash
			if (cash_contracts.search(all_id[i - 1])) {
				list_contracts.push_back(cash_contracts.get(all_id[i - 1]));
			}
			// if record don't have in the cash but get in date base
			else {
				contract = gateway_contract.get(all_id[i - 1]);
				list_contracts.push_back(contract);
				cash_contracts.add(contract, contract->getId());
				cash_writers.add(contract->getIdWriters(), contract->getIdWriters()->getId());
			}
			// check last page maybe not 10 element
			if (page == all_pages) {
				if (i == end)
					break;
			}
		}
		// show BLOCK records in the console
		std::cout << "\t\t\t\t\t---------- Контракты ----------\n";
		std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
		std::cout << "| Номер |                   Писатель                      |№ Контракта| Дата(заклю.)|Срок к.|Действ.| Дата(окчан.)|\n";
		// show list contracts
		for (auto i = list_contracts.begin(); i != list_contracts.end(); i++) {
			if (user_id > 0 && user_id < 10)
				setw_user_id = 7;
			else if (user_id > 9 && user_id < 100)
				setw_user_id = 6;
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			std::cout << "|" << user_id << std::setw(setw_user_id) << *(*i) << std::endl;
			user_id++;
			if (flag_show_all) {
				std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
				std::cout << "Номер паспорта: " << (*(*i)).getIdWriters()->getPassportNumber() << ", Фамилия: " << (*(*i)).getIdWriters()->getSurname()
					<< ", Имя: " << (*(*i)).getIdWriters()->getName() << " Отчества: " << (*(*i)).getIdWriters()->getMiddleName()
					<< ", Адрес: " << (*(*i)).getIdWriters()->getAddress() << ", Телефон: " << (*(*i)).getIdWriters()->getPhone() << std::endl;;
			}
		}
		if(!flag_show_all)
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
		// pages
		std::cout << "\t\t\t\t\t-------- Страница " << page << " из " << all_pages << " --------" << std::endl;
		// used memory
		used_memory = cash_contracts.get_size_cash() + BLOCK * sizeof(std::list<Contracts*>) + sizeof(Contracts*) + sizeof(int) * end + cash_books.get_size_cash() + cash_writers.get_size_cash() + cash_customers.get_size_cash() + cash_orders.get_size_cash();
		std::cout << "Используемая память: " << used_memory << " байт" << std::endl;
		// commands for user
		std::cout << commands_for_user << std::endl;
		// get commands number from user
		if (page == 1) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			if (user_commands != 1)
				user_commands += 1;
		}
		else if (page == all_pages) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			user_commands += 1;
		}
		else {
			user_commands = enterNumber(1, 7, "Введите номер команды!");
		}
		// users commands
		switch (user_commands) {
		// next page
		case 1: {
			system("cls");
			begin += BLOCK;
			page += 1;
			if (page == all_pages)
				commands_for_user = "1. Назад 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != 1)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// prev page
		case 2: {
			system("cls");
			begin -= BLOCK;
			page -= 1;
			if (page == 1)
				commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != all_pages)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// add
		case 3: {
			// get id writers
			Writers* writer;
			int* id_writers = gateway_writers.get_id();
			int count_writers = gateway_writers.get_count(), num_writer = -1, id_writer = -1;
			bool flag = false;
			// search id for new record
			int new_id = -1, setw_new_id = 8;
			for (int i = 1; i < end + 1; i++) {
				for (int j = 0; j < end; j++) {
					if (i == all_id[j]) {
						flag = true;
					}
				}
				if (!flag)
					new_id = i;
				flag = false;
			}
			// if not empty id but new id = count redors in date base + 1
			if (new_id == -1)
				new_id = end + 1;
			// length for setw in the show console
			if (end + 1 >= 1 && end + 1 <= 9)
				setw_new_id = 7;
			else if (end + 1 >= 10 && end + 1 <= 99)
				setw_new_id = 6;
			// create new contract
			Contracts* tmp_contract = new Contracts(new_id);
			// add writer
			while (true) {
				// user enter number writer
				num_writer = enterNumber(1, count_writers, "Введите номер писателя!");
				// serach writer id
				id_writer = id_writers[num_writer - 1];
				// search writer in cash
				if (cash_writers.search(id_writer)) {
					writer = cash_writers.get(id_writer);
				}
				// but not in the cash writer his get date base
				else {
					writer = gateway_writers.get(id_writer);
					cash_writers.add(writer, writer->getId());
				}
				// for setw in get writer
				int setw_num_writer = 8;
				if (num_writer > 0 && num_writer < 10)
					setw_num_writer = 7;
				else if (num_writer > 9 && num_writer < 100)
					setw_num_writer = 6;
				// show writer
				std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
				std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
				std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
				std::cout << "|" << num_writer << std::setw(setw_num_writer) << *writer << std::endl;
				std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
				// ask the same writer?
				int add_writer = enterNumber(1, 2, "Это тот запись который нужно?\n1 - Да, 2 - Нет");
				// Yes in the breake while
				if (add_writer == 1)
					break;
			}
			// add writer
			tmp_contract->setIdWriters(writer);
			// add other parametrs
			tmp_contract->setContractNum(enterNumber(10000000, 99999999, "Введите номер контракта"));
			tmp_contract->setDateOfConsContract(enterDate(1998, 2050));
			tmp_contract->setTermOfTheContract(enterNumber(1, 50, "Введите срок действие контракта"));
			tmp_contract->setValidyOfTheContract((enterNumber(1, 2, "Действует ли контракт? 1 - Да, 2 - Нет") == 1 ? true : false));
			tmp_contract->setDateOfTheTerminitionContract(enterDate(2005, 2065));
			// show new contract
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			std::cout << "| Номер |                   Писатель                      |№ Контракта| Дата(заклю.)|Срок к.|Действ.| Дата(окчан.)|\n";
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			std::cout << "|" << end + 1 << std::setw(8 - (std::to_string(end + 1)).length()) << *tmp_contract << std::endl;
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			// ask add new writer or not
			int user_action = enterNumber(1, 2, "Данный запись добавить?\n1 - Да, 2 - Нет");
			// add new record
			if (user_action == 1) {
				gateway_contract.add(tmp_contract);
				cash_contracts.add(tmp_contract, tmp_contract->getId());
				end = gateway_contract.get_count();
				all_id = gateway_contract.get_id();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// delete
		case 4: {
			int delete_id = -1, setw_delete_id = -1;
			// user enter number delete record 
			int num_contract = enterNumber(1, end, "Введите номер записи который необходимо удалить!");
			// search id contract
			delete_id = all_id[num_contract - 1];
			// setw for show contract
			if (num_contract > 0 && num_contract < 10)
				setw_delete_id = 7;
			else if (num_contract > 9 && num_contract < 100)
				setw_delete_id = 6;
			// show contract
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			std::cout << "| Номер |                   Писатель                      |№ Контракта| Дата(заклю.)|Срок к.|Действ.| Дата(окчан.)|\n";
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			if (cash_contracts.search(delete_id)) {
				contract = cash_contracts.get(delete_id);
			}
			else {
				contract = gateway_contract.get(delete_id);
			}
			std::cout << "|" << num_contract << std::setw(setw_delete_id) << *contract << std::endl;
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			// ask delete or not
			int action_user = enterNumber(1, 2, "Вы уверены, что хотите удалить данный запись?\n1 - Да, 2 - Нет");
			if (action_user == 1) {
				gateway_contract.remove(delete_id);
				cash_contracts.remove(delete_id);
				all_id = gateway_contract.get_id();
				end = gateway_contract.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// update
		case 5: {
			// for update id writer
			int* id_writers = gateway_writers.get_id();
			int count_writers = gateway_writers.get_count();
			// for update contract
			int edit_id = -1, setw_edit_id = 8;
			// get number contract
			int user_num = enterNumber(1, end, "Введите номер записи котрого нужно редактировать");
			edit_id = all_id[user_num - 1];
			// show contract
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			std::cout << "| Номер |                   Писатель                      |№ Контракта| Дата(заклю.)|Срок к.|Действ.| Дата(окчан.)|\n";
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			// for setw for show contract
			if (user_num >= 1 && user_num <= 9)
				setw_edit_id = 7;
			else if (user_num >= 10 && user_num <= 99)
				setw_edit_id = 6;
			bool flag = false;
			// search contract in the cash
			if (cash_contracts.search(edit_id)) {
				flag = true;
				std::cout << "|" << user_num << std::setw(setw_edit_id) << *(cash_contracts.get(edit_id)) << std::endl;
				contract = cash_contracts.get(edit_id);
			}
			// get contract in the date base if not in the cash
			else {
				contract = gateway_contract.get(edit_id);
				cash_contracts.add(contract, contract->getId());
				std::cout << "|" << user_num << std::setw(setw_edit_id) << *contract << std::endl;
			}
			std::cout << "+-------+-------------------------------------------------+-----------+-------------+-------+-------+-------------+\n";
			// ask update parametrs
			while (true) {
				std::cout << "Какое параметр хотите редактировать?\n1. Писатель 2. Номер контракта 3. Дата заключение 4. Срок заключение 5. Действительность 6. Дата окончание 7. Сохранить изменение 8. Выход" << std::endl;
				int parametr = enterNumber(1, 8, "Выберите параметр");
				if (parametr == 1) {
					// search new writer for update
					Writers* writer;
					int num_writer = enterNumber(1, count_writers, "Введите новый номер писателя");
					if (cash_writers.search(id_writers[num_writer - 1])) {
						writer = cash_writers.get(id_writers[num_writer - 1]);
					}
					else {
						writer = gateway_writers.get(id_writers[num_writer - 1]);
						cash_writers.add(writer, writer->getId());
					}
					std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
					std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
					std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
					std::cout << "|" << num_writer << std::setw((8 - (int)std::to_string(num_writer).size())) << *writer << std::endl;
					std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
					contract->setIdWriters(writer);
				}
				if (parametr == 2)
					contract->setContractNum(enterNumber(10000000, 99999999, "Введите новый номер контракта"));
				if (parametr == 3) {
					std::cout << "Введите новую дату заключения контракта\n";
					contract->setDateOfConsContract(enterDate(2000, 2050));
				}
				if (parametr == 4)
					contract->setTermOfTheContract(enterNumber(1, 50, "Введите новый срок контракта"));
				if (parametr == 5)
					contract->setValidyOfTheContract((enterNumber(1, 2, "Введите новый статус контракта! 1 - Дйествителен, 2 - Не действителен")) == 1 ? true : false);
				if (parametr == 6) {
					std::cout << "Введите новую дату окончания контракта\n";
					contract->setDateOfTheTerminitionContract(enterDate(2000, 2050));
				}
				if (parametr == 7) {
					gateway_contract.save(contract);
					if (flag)
						cash_contracts.add(contract, contract->getId());
				}
				if (parametr == 8)
					break;
			}
			system("cls");
		} break;
		// show all
		case 6: {
			int action = enterNumber(1, 2, "Состояние просмотра таблицы польностью\n1 - Вкл. 2 - Выкл.");
			if (action == 1)
				flag_show_all = true;
			else
				flag_show_all = false;
			system("cls");
		} break;
		// exit
		case 7:
			return;
		}
	}
}

void InterfaceTables::table_customers() {
	Customers* customer;
	int user_commands = -1, begin = 1, end = gateway_customers.get_count(), * all_id = gateway_customers.get_id(), user_id = 1;
	int page = 1, all_pages = std::ceil((double)end / (double)BLOCK), setw_user_id = 8;
	std::string commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
	double used_memory = 0;
	while (true) {
		// clear list
		list_customers.clear();
		user_id = BLOCK * (page - 1) + 1;
		// get records in the cash or date base
		for (int i = begin; i < begin + BLOCK; i++) {
			// check cash
			if (cash_customers.search(all_id[i - 1])) {
				list_customers.push_back(cash_customers.get(all_id[i - 1]));
			}
			// get record in the date base
			else {
				customer = gateway_customers.get(all_id[i - 1]);
				list_customers.push_back(customer);
				cash_customers.add(customer, customer->getId());
			}
			// maybe last block get not 10 records 
			if (page == all_pages) {
				if (i == end)
					break;
			}
		}
		// show BLOCK in the console
		std::cout << "\t\t\t\t\t\t\t\t--------- Заказчики ---------\n";
		std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
		std::cout << "|   №   |               Название компании                 |                   Адрес               |             Телефон         |             Ф.И.О. Заказчика          |\n";
		std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
		// show records list
		for (auto i = list_customers.begin(); i != list_customers.end(); i++) {
			if (user_id > 0 && user_id < 10)
				setw_user_id = 7;
			else if (user_id > 9 && user_id < 100)
				setw_user_id = 6;
			std::cout << "|" << user_id << std::setw(setw_user_id) << *(*i) << std::endl;
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			user_id++;
		}
		// show pages
		std::cout << "\t\t\t\t\t\t\t----------- Страница " << page << " из " << all_pages << " -----------\n";
		// show used memory
		used_memory = cash_customers.get_size_cash() + BLOCK * sizeof(std::list<Customers*>) + sizeof(Customers*) + sizeof(int) * end + cash_books.get_size_cash() + cash_contracts.get_size_cash() + cash_writers.get_size_cash() + cash_orders.get_size_cash();
		std::cout << "Используемая память: " << used_memory << " байт" << std::endl;
		// show commadns for user
		std::cout << commands_for_user << std::endl;

		// get user command
		if (page == 1) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			if (user_commands != 1)
				user_commands += 1;
		}
		else if (page == all_pages) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			user_commands += 1;
		}
		else {
			user_commands = enterNumber(1, 7, "Введите номер команды!");
		}
		// users commands
		switch (user_commands) {
		// next
		case 1: {
			system("cls");
			begin += BLOCK;
			page += 1;
			if (page == all_pages)
				commands_for_user = "1. Назад 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != 1)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// prev
		case 2: {
			system("cls");
			begin -= BLOCK;
			page -= 1;
			if (page == 1)
				commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if(page != all_pages)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// add
		case 3: {
			// search id for new record
			bool flag = false;
			int new_id = -1, setw_new_id = 8;
			for (int i = 1; i < end + 1; i++) {
				for (int j = 0; j < end; j++) {
					if (i == all_id[j]) {
						flag = true;
					}
				}
				if (!flag)
					new_id = i;
				flag = false;
			}
			// if not empty id but new id = count redors in date base + 1
			if (new_id == -1)
				new_id = end + 1;
			// length for setw in the show console
			if (end + 1 >= 1 && end + 1 <= 9)
				setw_new_id = 7;
			else if (end + 1 >= 10 && end + 1 <= 99)
				setw_new_id = 6;
			// create new customer
			Customers* tmp_customer = new Customers(new_id);
			tmp_customer->setCustomerName(enterString(1, 45, "Введите название компании"));
			tmp_customer->setAddress(enterString(1, 35, "Введите адрес"));
			tmp_customer->setPhone(enterString(1, 25, "Введите номер телефона"));
			tmp_customer->setFullNameCustomer(enterString(1, 35, "Введите Ф.И.О. заказчика"));
			// show new customer
			std::cout << "Новый запись:\n";
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			std::cout << "|   №   |               Название компании                 |                   Адрес               |             Телефон         |             Ф.И.О. Заказчика          |\n";
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			std::cout << "|" << end + 1 << std::setw(setw_new_id) << *tmp_customer << std::endl;
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			// ask user save or not?
			int user_action = enterNumber(1, 2, "Сохранить данный запись?\n 1 - Да, 2 - Нет");
			if (user_action == 1) {
				// add in date base and in cash new record
				gateway_customers.add(tmp_customer);
				cash_customers.add(tmp_customer, tmp_customer->getId());
				// update values all id and count
				all_id = gateway_customers.get_id();
				end = gateway_customers.get_count();
				all_pages = std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// remove
		case 4: {
			int delete_id = -1, setw_delete_id = -1;
			// user enter number delete record 
			int num_customer = enterNumber(1, end, "Введите номер записи который необходимо удалить!");
			// search id contract
			delete_id = all_id[num_customer - 1];
			// setw for show contract
			if (num_customer > 0 && num_customer < 10)
				setw_delete_id = 7;
			else if (num_customer > 9 && num_customer < 100)
				setw_delete_id = 6;
			// serach record
			if (cash_customers.search(delete_id)) {
				customer = cash_customers.get(delete_id);
			}
			else {
				customer = gateway_customers.get(delete_id);
			}
			// show record
			std::cout << "Запись для удаление:\n";
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			std::cout << "|   №   |               Название компании                 |                   Адрес               |             Телефон         |             Ф.И.О. Заказчика          |\n";
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			std::cout << "|" << num_customer << std::setw(setw_delete_id) << *customer << std::endl;
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			// ask user delete or not
			int user_action = enterNumber(1, 2, "Вы точно хотите удалить данный запись без возможностью восстановить?\n1 - Да, 2 - Нет");
			if (user_action == 1) {
				// remove record
				gateway_customers.remove(delete_id);
				cash_customers.remove(delete_id);
				// update all id and count
				all_id = gateway_customers.get_id();
				end = gateway_customers.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// update
		case 5: {
			int update_num = -1, setw_update_num = 8;
			// get update number
			int user_num = enterNumber(1, end, "Введите номер записи который хотите редактировать");
			// search update id
			update_num = all_id[user_num - 1];
			// setw for show update record
			if (user_num > 0 && user_num < 10)
				setw_update_num = 7;
			else if (user_num > 9 && user_num < 100)
				setw_update_num = 8;
			// show records for update
			std::cout << "Запись для редактирование: \n";
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			std::cout << "|   №   |               Название компании                 |                   Адрес               |             Телефон         |             Ф.И.О. Заказчика          |\n";
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			// get record in the cash or date base
			if (cash_customers.search(update_num)) {
				customer = cash_customers.get(update_num);
				cash_customers.remove(update_num);
				std::cout << "|" << user_num << std::setw(setw_update_num) << *customer << std::endl;
			}
			else {
				customer = gateway_customers.get(update_num);
				std::cout << "|" << user_num << std::setw(setw_update_num) << *customer << std::endl;
			}
			std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
			// ask user how parametrs update
			while (true) {
				int action = enterNumber(1, 6, "Выбирите параметр для редактирование\n1 - Название компании, 2 - Адрес, 3 - Телефон, 4 - Ф.И.О. заказчика, 5 - Сохранить изменение, 6 - Выход");
				if (action == 1) {
					customer->setCustomerName(enterString(1, 45, "Введите новое название компании"));
				}
				else if (action == 2) {
					customer->setAddress(enterString(1, 35, "Введите новый адрес"));
				}
				else if (action == 3) {
					customer->setPhone(enterString(1, 25, "Введите новый номер телефона"));
				}
				else if (action == 4) {
					customer->setFullNameCustomer(enterString(1, 35, "Введите новый Ф.И.О. заказчика"));
				}
				// update record in date base and add update record in cash
				else if (action == 5) {
					gateway_customers.save(customer);
					cash_customers.add(customer, customer->getId());
				}
				// exit
				else {
					break;
				}
			}
			system("cls");
		} break;
		// show all
		case 6: {
			std::cout << "Данные уже отображены полностью!" << std::endl;
			system("cls");
		} break;
		// exit
		case 7:
			return;
		}
	}
}

void InterfaceTables::table_books() {
	Books* book;
	std::list<Books*> list_books;
	GatewayBooks gateway_books;
	int user_commands = -1, begin = 1, end = gateway_books.get_count(), * all_id = gateway_books.get_id(), user_id = 1;
	int page = 1, all_pages = (int)std::ceil((double)end / (double)BLOCK), setw_user_id = 8, used_memory = 0;
	std::string commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход", for_show_all = "";
	bool flag_show_all = false;
	while (true) {
		// clear list
		list_books.clear();
		user_id = BLOCK * (page - 1) + 1;
		// check cash if not record in the cash get from date base
		for (int i = begin; i < BLOCK + begin; i++) {
			if (cash_books.search(all_id[i - 1])) {
				list_books.push_back(cash_books.get(all_id[i - 1]));
			}
			else {
				book = gateway_books.get(all_id[i - 1]);
				// add cash book
				cash_books.add(book, book->getId());
				// and add cash writers
				for (auto i = book->getIdWritersBook()->begin(); i != book->getIdWritersBook()->end(); i++) {
					if((*i) != nullptr)
						cash_writers.add(*i, (*i)->getId());
				}
				list_books.push_back(book);
			}
			// check last page maybe not 10 element
			if (page == all_pages) {
				if (i == end)
					break;
			}
		}
		// show in the console
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t----------- Книги -----------\n";
		std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
		std::cout << "+  №    |    Шифр   |             Название книги            |                    Писатели                     |                       Издания                   | Кол-во издан. | Дата выпуск.| Себест., $ | Ст. про.,$ | Гонорар, $ |\n";
		for (auto i = list_books.begin(); i != list_books.end(); i++) {
			if (user_id > 0 && user_id < 10)
				setw_user_id = 7;
			else if (user_id > 9 && user_id < 100)
				setw_user_id = 6;
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "|" << user_id << std::setw(setw_user_id) << *(*i) << std::endl;
			user_id++;
			if (flag_show_all) {
				if ((*(*i)).getIdWritersBook()->empty()) {
					std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
					std::cout << "Автор отсуствует!" << std::endl;
				}
				else {
					for (auto j = (*(*i)).getIdWritersBook()->begin(); j != (*(*i)).getIdWritersBook()->end(); j++) {
						std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
						std::cout << "Номер паспорта: " << (*j)->getPassportNumber() << ", Фамилия: " << (*j)->getSurname()
							<< ", Имя: " << (*j)->getName() << " Отчества: " << (*j)->getMiddleName()
							<< ", Адрес: " << (*j)->getAddress() << ", Телефон: " << (*j)->getPhone() << std::endl;;
					}
				}
			}
		}
		if (!flag_show_all)
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
		// pages
		std::cout << "\t\t\t\t\t\t\t\t\t\t ----------- Страница " << page << " из " << all_pages << " -----------\n";
		// used memory
		used_memory = (double)(cash_books.get_size_cash() + BLOCK * sizeof(std::list<Books*>) + sizeof(Books*) + sizeof(int) * end + cash_writers.get_size_cash() + cash_contracts.get_size_cash() + cash_customers.get_size_cash() + cash_orders.get_size_cash());
		std::cout << "Использованная память: " << used_memory << " байт" << std::endl;
		// commands
		std::cout << commands_for_user << std::endl;
		// get commands number from user
		if (page == 1) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			if (user_commands != 1)
				user_commands += 1;
		}
		else if (page == all_pages) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			user_commands += 1;
		}
		else {
			user_commands = enterNumber(1, 7, "Введите номер команды!");
		}
		// complite all commands
		switch (user_commands) {
		// next 
		case 1: {
			system("cls");
			// next BLOCK and page
			begin += BLOCK;
			page += 1;
			// new commands for user but last page not have NEXT and first page not have PREV
			if (page == all_pages)
				commands_for_user = "1. Назад 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != 1)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// prev
		case 2: {
			system("cls");
			// prev BLOCK and page
			begin -= BLOCK;
			page -= 1;
			// new commands for user but last page not have NEXT and first page not have PREV
			if (page == 1)
				commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != all_pages)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// add
		case 3: {
			// for search writers for add from book
			int* all_writers = gateway_writers.get_id();
			int writer_count = gateway_writers.get_count();
			list_writers.clear();
			Writers* writer;
			std::list<int> writers_numbers;
			// search id for new record
			bool flag = false;
			int new_id = -1, setw_new_id = 8, setw_writers = 8;
			for (int i = 1; i < end + 1; i++) {
				for (int j = 0; j < end; j++) {
					if (i == all_id[j]) {
						flag = true;
					}
				}
				if (!flag)
					new_id = i;
				flag = false;
			}
			// if not empty id but new id = count redors in date base + 1
			if (new_id == -1)
				new_id = end + 1;
			// length for setw in the show console
			if (end + 1 >= 1 && end + 1 <= 9)
				setw_new_id = 7;
			else if (end + 1 >= 10 && end + 1 <= 99)
				setw_new_id = 6;
			// create new book
			Books* tmp_book = new Books(new_id);
			tmp_book->setCipherOfTheBook(enterNumber(10000000, 99999999, "Введите шифр книги"));
			tmp_book->setName(enterString(1, 30, "Введите название книги"));
			// search writers
			flag = false;
			bool exit = false;
			// maybe user add 2-N writers
			while (!exit) {
				// get all writers in the list
				if (!flag) {
					// get num writer
					int writer_num = enterNumber(1, writer_count, "Введите номер писателя");
					writers_numbers.push_back(writer_num);
					// search id writer
					int writer_id = all_writers[writer_num - 1];
					// check cash
					if (cash_writers.search(writer_id)) {
						list_writers.push_back(cash_writers.get(writer_id));
					}
					// if not record in the cash get record in the date base
					else {
						writer = gateway_writers.get(writer_id);
						cash_writers.add(writer, writer->getId());
						list_writers.push_back(writer);
					}
					// user add nore writers?
					int action = enterNumber(1, 2, "1 - Добавить ещё писателя 2 - Выйти");
					if (action == 2 && !exit) {
						flag = true;
					}
				}
				// show all writers
				else {
					std::cout << "Писатели для добавление:" << std::endl;
					std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
					std::cout << "| Номер | №  Паспорт |      Фамилия      |        Имя        |     Отчества      |                 Адрес                 |       Телефон     |\n";
					std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
					// show all writers and his numbers
					auto num = writers_numbers.begin();
					for (auto i = list_writers.begin(); i != list_writers.end(); i++) {
						if (*num > 0 && *num < 10)
							setw_writers = 7;
						else if (*num > 9 && *num < 100)
							setw_writers = 6;
						std::cout << "|" << *num << std::setw(setw_writers) << *(*i) << std::endl;
						std::cout << "+-------+------------+-------------------+-------------------+-------------------+---------------------------------------+-------------------+\n";
						num++;
					}
					// setIdWritersBooks complite
					exit = true;
				}
			}
			// add all writers in the book
			for (auto i = list_writers.begin(); i != list_writers.end(); i++) {
				tmp_book->setIdWritersBook(*i);
			}
			tmp_book->setTitle(enterString(1, 45, "Введите издания"));
			tmp_book->setCirculation(enterNumber(1, 99999999, "Введите количество издание"));
			std::cout << "Введите дату выпуска\n";
			tmp_book->setReleaseDate(enterDate(1995, 2050));
			tmp_book->setCostPrice(enterNumber((float)1.0, (float)10000.0, "Введите себестоимость книги"));
			tmp_book->setSalePrice(enterNumber((float)100, (float)100000, "Введите стоимость книги в продаже"));
			tmp_book->setFee(enterNumber((float)1, (float)10000, "Введите гонорар писателя"));
			// show new record
			std::cout << "Запись для добавление: " << std::endl;
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "+  №    |    Шифр   |             Название книги            |                    Писатели                     |                       Издания                   | Кол-во издан. | Дата выпуск.| Себест., $ | Ст. про.,$ | Гонорар, $ |\n";
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "|" << end + 1 << std::setw(setw_new_id) << *tmp_book << std::endl;
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			// user add record or not?
			int action = enterNumber(1, 2, "Вы точно хотите добавить данный запись?\n1 - Да 2 - Нет");
			if (action == 1) {
				// add record in the date base and cash
				gateway_books.add(tmp_book);
				cash_books.add(tmp_book, tmp_book->getId());
				// update all_id and end
				all_id = gateway_books.get_id();
				end = gateway_books.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// remove
		case 4: {
			int delete_id = -1, setw_delete_id = -1;
			// user enter number delete record 
			int num_book = enterNumber(1, end, "Введите номер записи который необходимо удалить!");
			// search id contract
			delete_id = all_id[num_book - 1];
			// setw for show contract
			if (num_book > 0 && num_book < 10)
				setw_delete_id = 7;
			else if (num_book > 9 && num_book < 100)
				setw_delete_id = 6;
			// serach record
			if (cash_customers.search(delete_id)) {
				book = gateway_books.get(delete_id);
			}
			else {
				book = gateway_books.get(delete_id);
				cash_books.add(book, book->getId());
			}
			// show record
			std::cout << "Запись для удаление:\n";
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "+  №    |    Шифр   |             Название книги            |                    Писатели                     |                       Издания                   | Кол-во издан. | Дата выпуск.| Себест., $ | Ст. про.,$ | Гонорар, $ |\n";
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "|" << num_book << std::setw(setw_delete_id) << *book << std::endl;
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			// ask user delete or not
			int user_action = enterNumber(1, 2, "Вы точно хотите удалить данный запись без возможностью восстановить?\n1 - Да, 2 - Нет");
			if (user_action == 1) {
				// remove record
				gateway_customers.remove(delete_id);
				cash_customers.remove(delete_id);
				// update all id and count
				all_id = gateway_books.get_id();
				end = gateway_books.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// idet
		case 5: {
			// for update id writer
			int* id_writers = gateway_writers.get_id();
			int count_writers = gateway_writers.get_count();
			// for update contract
			int edit_id = -1, setw_edit_id = 8;
			// get number contract
			int user_num = enterNumber(1, end, "Введите номер записи котрого нужно редактировать");
			edit_id = all_id[user_num - 1];
			if (user_num >= 1 && user_num <= 9)
				setw_edit_id = 7;
			else if (user_num >= 10 && user_num <= 99)
				setw_edit_id = 6;
			// get record
			if (cash_books.search(edit_id)) {
				book = cash_books.get(edit_id);
			}
			else {
				book = gateway_books.get(edit_id);
				cash_books.add(book, book->getId());
			}
			// show book
			std::cout << "Запись для редактирование:\n";
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "+  №    |    Шифр   |             Название книги            |                    Писатели                     |                       Издания                   | Кол-во издан. | Дата выпуск.| Себест., $ | Ст. про.,$ | Гонорар, $ |\n";
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "|" << user_num << std::setw(setw_edit_id) << *book << std::endl;
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			while (true) {
				int parametr = enterNumber(1, 11, "1 - Шифр, 2 - Название книги, 3 - Писатели, 4 - Издания, 5 - Количество изданных книг, 6 - Дата выпуска, 7 - Себесстоимость, 8 - Стоимость продаж, 9 - Гонорар, 10 - Сохранить, 11 - Выход");
				if (parametr == 1) {
					book->setCipherOfTheBook(enterNumber(10000000, 99999999, "Введите новый шифр книги! Длина 8-цифр!"));
				}
				else if (parametr == 2) {
					book->setName(enterString(1, 35, "Введите название книги"));
				}
				else if (parametr == 3) {
					// update writers
					bool exit = false;
					int i = 0, action = -1, new_writer_num = -1;
					while (!exit) {
						i = 1;
						if (*(book->getIdWritersBook()->begin()) == nullptr && book->getIdWritersBook()->size() == 1) {
							int new_writer = enterNumber(1, count_writers, "Писателей отсуствует! Введите номер писателя котрого хотите добавить");
							new_writer = id_writers[new_writer - 1];
							if (cash_writers.search(new_writer)) {
								book->getIdWritersBook()->push_back(cash_writers.get(new_writer));
							}
							else {
								Writers* writer = gateway_writers.get(new_writer);
								cash_writers.add(writer, writer->getId());
								book->getIdWritersBook()->push_back(writer);
							}
						}
						else {
							for (auto j = book->getIdWritersBook()->begin(); j != book->getIdWritersBook()->end() && *j != nullptr; j++) {
								std::cout << i << ". " << (*j)->getName() << " " << ((*j)->getSurname())[0] << "." << ((*j)->getMiddleName())[0] << ".\n";
								i++;
							}
							action = enterNumber(1, book->getIdWritersBook()->size(), "Выбирите писателя который хотите редактировать");
							i = 0;
							auto writer = book->getIdWritersBook()->begin();
							while (i != action + 1) {
								i++;
								writer++;
							}
							book->getIdWritersBook()->erase(writer);
							int new_writer = enterNumber(1, count_writers, "Введите номер нового писателя");
							new_writer = id_writers[new_writer - 1];
							if (cash_writers.search(new_writer)) {
								book->getIdWritersBook()->push_back(cash_writers.get(new_writer));
							}
							else {
								Writers* writer = gateway_writers.get(new_writer);
								cash_writers.add(writer, writer->getId());
								book->getIdWritersBook()->push_back(writer);
							}
						}
						exit = true;
					}
				}
				else if (parametr == 4) {
					book->setTitle(enterString(1, 35, "Введите издания"));
				}
				else if (parametr == 5) {
					book->setCirculation(enterNumber(1, 100000000, "Введите количества выпущенных книг"));
				}
				else if (parametr == 6) {
					book->setReleaseDate(enterDate(1950, 2050));
				}
				else if (parametr == 7) {
					book->setCostPrice(enterNumber(1.0f, 100000000.0f, "Введите себестоимость"));
				}
				else if (parametr == 8) {
					book->setSalePrice(enterNumber(1.0f, 100000000.0f, "Введите стоимость при продаже"));
				}
				else if (parametr == 9) {
					book->setFee(enterNumber(1.0f, 10000000.0f, "Введите гонорар"));
				}
				else if (parametr == 10) {
					cash_books.remove(edit_id);
					gateway_books.save(book);
					cash_books.add(book, book->getId());
				}
				else {
					break;
				}
			}
			system("cls");
		} break;
		// show all
		case 6: {
			int action = enterNumber(1, 2, "Состояние просмотра таблицы польностью\n1 - Вкл. 2 - Выкл.");
			if (action == 1)
				flag_show_all = true;
			else
				flag_show_all = false;
			system("cls");
		} break;
		case 7:
			return;
		}
	}
}

void InterfaceTables::table_orders() {
	Orders* order;
	std::list<Orders*> list_orders;
	GatewayOrders gateway_orders;
	int user_commands = -1, begin = 1, end = gateway_orders.get_count(), * all_id = gateway_orders.get_id(), user_id = 1;
	int page = 1, all_pages = (int)std::ceil((double)end / (double)BLOCK), setw_user_id = 8, used_memory = 0;
	std::string commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход", for_show_all = "";
	bool flag_show_all = false;
	while (true) {
		// clear list
		list_orders.clear();
		user_id = BLOCK * (page - 1) + 1;
		// get list orders records
		for (int i = begin; i < BLOCK + begin; i++) {
			// check cash
			if (cash_orders.search(all_id[i - 1])) {
				list_orders.push_back(cash_orders.get(all_id[i - 1]));
			}
			// get records in the date base
			else {
				order = gateway_orders.get(all_id[i - 1]);
				cash_orders.add(order, order->getId());
				list_orders.push_back(order);
				// add book in the cash books
				if (order->getIdBooks() != nullptr)
					cash_books.add(order->getIdBooks(), order->getIdBooks()->getId());
				// add customer in the cash customers
				if (order->getIdCustomers() != nullptr)
					cash_customers.add(order->getIdCustomers(), order->getIdCustomers()->getId());
			}
			// check last page maybe not 10 element
			if (page == all_pages) {
				if (i == end)
					break;
			}
		}
		// show all
		std::cout << "\t\t\t\t\t----------- Заказы --------------\n";
		std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
		std::cout << "|   №   |       Ф.И.О. Заказчика      | №  заказа | Дата(прин.) | Дата(окон.) |            Название книги             | К-в книг|\n";
		for (auto i = list_orders.begin(); i != list_orders.end(); i++) {
			if (user_id > 0 && user_id < 10)
				setw_user_id = 7;
			else if (user_id > 9 && user_id < 100)
				setw_user_id = 6;
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|" << user_id << std::setw(setw_user_id) << *(*i) << std::endl;
			user_id++;
			if (flag_show_all) {
				std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
				// show customer
				std::cout << "Название компании: " << (*i)->getIdCustomers()->getCustomerName() << " Адрес: "
					<< (*i)->getIdCustomers()->getAddress() << " Телефон: " << (*i)->getIdCustomers()->getPhone()
					<< " Ф.И.О. Заказчика: " << (*i)->getIdCustomers()->getFullNameCustomer() << std::endl;
				// show book
				std::cout << "Шифр книги: " << (*i)->getIdBooks()->getCipherOfTheBook() << " Название книги: "
					<< (*i)->getIdBooks()->getName() << " Издания: " << (*i)->getIdBooks()->getTitle()
					<< " Количество выпущенных книг: " << (*i)->getIdBooks()->getCirculation()
					<< " Дата выпуска: " << (*i)->getIdBooks()->getReleaseDate() << " Себестоимость: "
					<< (*i)->getIdBooks()->getCostPrice() << " $  Стоимость продажи: " << (*i)->getIdBooks()->getSalePrice()
					<< " $  Гонорар писателя: " << (*i)->getIdBooks()->getFee() << " $\n";
			}
		}
		if (!flag_show_all)
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
		// pages
		std::cout << "\t\t\t\t----------- Страница " << page << " из " << all_pages << " -----------" << std::endl;
		// used memory
		used_memory = cash_contracts.get_size_cash() + BLOCK * sizeof(std::list<Orders*>) + sizeof(Orders*) + sizeof(int) * end + cash_books.get_size_cash() + cash_writers.get_size_cash() + cash_customers.get_size_cash() + cash_orders.get_size_cash();
		std::cout << "Используемая память: " << used_memory << " байт" << std::endl;
		// commands for user
		std::cout << commands_for_user << std::endl;
		// get commands number from user
		if (page == 1) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			if (user_commands != 1)
				user_commands += 1;
		}
		else if (page == all_pages) {
			user_commands = enterNumber(1, 6, "Введите номер команды!");
			user_commands += 1;
		}
		else {
			user_commands = enterNumber(1, 7, "Введите номер команды!");
		}
		// complite user commands
		switch (user_commands) {
		// next 
		case 1: {
			system("cls");
			// next BLOCK and page
			begin += BLOCK;
			page += 1;
			// new commands for user but last page not have NEXT and first page not have PREV
			if (page == all_pages)
				commands_for_user = "1. Назад 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != 1)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// prev
		case 2: {
			system("cls");
			// prev BLOCK and page
			begin -= BLOCK;
			page -= 1;
			// new commands for user but last page not have NEXT and first page not have PREV
			if (page == 1)
				commands_for_user = "1. Далее 2. Добавить 3. Удалить 4. Редактировать 5. Посмотреть подробно 6. Выход";
			else if (page != all_pages)
				commands_for_user = "1. Далее 2. Назад 3. Добавить 4. Удалить 5. Редактировать 6. Посмотреть подробно 7. Выход";
		} break;
		// add
		case 3: {
			Customers* customer;
			int count_customers = gateway_customers.get_count();
			int* all_customers = gateway_customers.get_id();
			Books* book;
			int count_books = gateway_books.get_count();
			int* all_books = gateway_books.get_id();
			bool exit = false;
			// search id for new record
			int new_id = -1, setw_new_id = 8, setw_writers = 8;
			for (int i = 0; i < end; i++) {
				if (i + 1 != all_id[i]) {
					new_id = i + 1;
					break;
				}
			}
			// if not empty id but new id = count redors in date base + 1
			if (new_id == -1)
				new_id = end + 1;
			// length for setw in the show console
			if (end + 1 >= 1 && end + 1 <= 9)
				setw_new_id = 7;
			else if (end + 1 >= 10 && end + 1 <= 99)
				setw_new_id = 6;
			// varibale for search customer
			int customer_num, setw_customer_num;
			// create new order
			Orders* tmp_order = new Orders(new_id);
			// search customer
			customer = nullptr;
			while (!exit) {
				int user_action = enterNumber(1, 3, "Введите номер команды\n1 - Добавить заказчика по номеру 2 - Сделать поиск заказчика по ... 3 - Выход");
				if (user_action == 1) {
					customer_num = enterNumber(1, count_customers, "Введите номер заказчика");
					int customer_id = all_customers[customer_num - 1];
					setw_customer_num = 8;
					// setw customer number
					if (customer_num > 0 && customer_num < 10)
						setw_customer_num = 7;
					else if (customer_num > 9 && customer_num < 100)
						setw_customer_num = 6;
					// check cash and show customer
					if (cash_customers.search(customer_id)) {
						customer = cash_customers.get(customer_id);
					}
					else {
						customer = gateway_customers.get(customer_id);
						cash_customers.add(customer, customer->getId());
					}
				}
				else if (user_action == 2) {
					bool flag_exit = false;
					while (!flag_exit) {
						int parametrs = enterNumber(1, 5, "1 - Найти по название комапании, 2 - Найти по адресу, 3 - Найти по телефону, 4 - Найти по Ф.И.О., 5 - Выход");
						if (parametrs == 1) {
							std::string value = enterString(1, 45, "Введите название компании");
							customer = gateway_customers.get_parametrs(value, "customer_name", "string");
							if (customer == nullptr)
								std::cout << "Заказчик с названием компании " << value << " не найден!\n";
						}
						else if (parametrs == 2) {
							std::string value = enterString(1, 35, "Введите адрес");
							customer = gateway_customers.get_parametrs(value, "address", "string");
							if (customer == nullptr)
								std::cout << "Заказчик с адресом " << value << " не найден!\n";
						}
						else if (parametrs == 3) {
							std::string value = enterString(1, 25, "Введите номер телефон");
							customer = gateway_customers.get_parametrs(value, "phone", "string");
							if (customer == nullptr)
								std::cout << "Заказчик с номером телефона " << value << " не найден!\n";
						}
						else if (parametrs == 4) {
							std::string value = enterString(1, 35, "Введите Ф.И.О. заказчика");
							customer = gateway_customers.get_parametrs(value, "full_name_customer", "string");
							if (customer == nullptr)
								std::cout << "Заказчик с Ф.И.О. " << value << " не найден!\n";
						}
						else {
							flag_exit = true;
						}
					}
				} 
				else if (user_action == 3) {
					exit = true;
				}
				if (customer == nullptr) {
					std::cout << "Вы не выбрали заказчика! Прежди чем выйти необходимо выбрать заказчика!\n";
					exit = false;
				}
				if (!exit) {
					std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
					std::cout << "|   №   |               Название компании                 |                   Адрес               |             Телефон         |             Ф.И.О. Заказчика          |\n";
					std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
					std::cout << "|" << customer_num << std::setw(setw_customer_num) << *customer << std::endl;
					std::cout << "+-------+-------------------------------------------------+---------------------------------------+-----------------------------+---------------------------------------+\n";
				}
			}
			tmp_order->setIdCustomers(customer);
			tmp_order->setOrderNumber(enterNumber(10000000, 99999999, "Введите номер заказа! Номер заказа должен содержать 8 цифр!"));
			std::cout << "Введите дату принятие заказа! ";
			tmp_order->setDateOfReceiptOrder(enterDate(1950, 2050));
			std::cout << "\nВведите дату когда заказ будет готов! ";
			tmp_order->setOrderCompletionDate(enterDate(1980, 2080));
			std::cout << std::endl;
			// serach book
			int book_num = enterNumber(1, count_books, "Введите номер книги который необходимо добавить");
			int book_id = all_books[book_num - 1], setw_book_num = 8;
			// for setw book number
			if (book_num > 0 && book_num < 10)
				setw_book_num = 7;
			else if (book_num > 9 && book_num < 100)
				setw_book_num = 6;
			// get book
			if (cash_books.search(book_id)) {
				book = cash_books.get(book_id);
			}
			else {
				book = gateway_books.get(book_id);
				cash_books.add(book, book->getId());
			}
			// show book
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "+  №    |    Шифр   |             Название книги            |                    Писатели                     |                       Издания                   | Кол-во издан. | Дата выпуск.| Себест., $ | Ст. про.,$ | Гонорар, $ |\n";
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			std::cout << "|" << book_num << std::setw(setw_book_num) << *book << std::endl;
			std::cout << "+-------+-----------+---------------------------------------+-------------------------------------------------+-------------------------------------------------+---------------+-------------+------------+------------+------------+\n";
			tmp_order->setIdBooks(book);
			tmp_order->setNumbersOfOrders(enterNumber(1, 10000000, "Введите количество заказанных книг"));
			// show record for add
			std::cout << "Запись для добавление: \n";
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|   №   |       Ф.И.О. Заказчика      | №  заказа | Дата(прин.) | Дата(окон.) |            Название книги             | К-в книг|\n";
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|" << end + 1 << std::setw(setw_new_id) << *tmp_order << std::endl;
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			int action = enterNumber(1, 2, "Вы хотите добавить исходный запись?\n1 - Да 2 - Нет");
			if (action == 1) {
				gateway_orders.add(tmp_order);
				cash_orders.add(tmp_order, tmp_order->getId());
				// update value
				all_id = gateway_orders.get_id();
				end = gateway_orders.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// remove
		case 4: {
			int delete_id = -1, setw_delete_id = -1;
			// user enter number delete record 
			int num_order = enterNumber(1, end, "Введите номер записи который необходимо удалить!");
			// search id contract
			delete_id = all_id[num_order - 1];
			// setw for show contract
			if (num_order > 0 && num_order < 10)
				setw_delete_id = 7;
			else if (num_order > 9 && num_order < 100)
				setw_delete_id = 6;
			// serach record
			if (cash_orders.search(delete_id)) {
				order = cash_orders.get(delete_id);
				cash_orders.remove(delete_id);
			}
			else {
				order = gateway_orders.get(delete_id);
			}
			std::cout << "Запись для удаление: " << std::endl;
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|   №   |       Ф.И.О. Заказчика      | №  заказа | Дата(прин.) | Дата(окон.) |            Название книги             | К-в книг|\n";
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|" << num_order << std::setw(setw_delete_id) << *order << std::endl;
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			int action = enterNumber(1, 2, "Вы хотите удалить данный запись?\n1 - Да 2 - Нет");
			if (action == 1) {
				gateway_orders.remove(delete_id);
				// update value
				all_id = gateway_orders.get_id();
				end = gateway_orders.get_count();
				all_pages = (int)std::ceil((double)end / (double)BLOCK);
			}
			system("cls");
		} break;
		// edit
		case 5: {
			Customers* customer;
			int count_customer = gateway_customers.get_count(), *all_customers = gateway_customers.get_id();
			Books* book;
			int count_book = gateway_books.get_count(), * all_book = gateway_books.get_id();
			int update_num = -1, setw_update_num = 8;
			// get update number
			int user_num = enterNumber(1, end, "Введите номер записи который хотите редактировать");
			// search update id
			update_num = all_id[user_num - 1];
			// setw for show update record
			if (user_num > 0 && user_num < 10)
				setw_update_num = 7;
			else if (user_num > 9 && user_num < 100)
				setw_update_num = 8;
			// show records for update
			if (cash_orders.search(update_num)) {
				order = cash_orders.get(update_num);
				cash_orders.remove(update_num);
			}
			else {
				order = gateway_orders.get(update_num);
			}
			std::cout << "Запись для удаление: " << std::endl;
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|   №   |       Ф.И.О. Заказчика      | №  заказа | Дата(прин.) | Дата(окон.) |            Название книги             | К-в книг|\n";
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			std::cout << "|" << user_num << std::setw(setw_update_num) << *order << std::endl;
			std::cout << "+-------+-----------------------------+-----------+-------------+-------------+---------------------------------------+---------+\n";
			while (true) {
				int action = enterNumber(1, 7, "1 - Заказчик, 2 - Номер заказа, 3 - Дата принятие заказа, 4 - Дата окончание заказа, 5 - Книга, 6 - Количество книг, 7 - Сохранить и выйти");
				if (action == 1) {
					int id_customer = enterNumber(1, count_customer, "Введите новый номер заказчика");
					if (cash_customers.search(all_customers[id_customer - 1])) {
						customer = cash_customers.get(all_customers[id_customer - 1]);
					}
					else {
						customer = gateway_customers.get(all_customers[id_customer - 1]);
						cash_customers.add(customer, customer->getId());
					}
					order->setIdCustomers(customer);
				}
				else if (action == 2) {
					order->setOrderNumber(enterNumber(10000000, 99999999, "Введите новый номер заказа"));
				}
				else if (action == 3) {
					order->setDateOfReceiptOrder(enterDate(1950, 2050));
				}
				else if (action == 4) {
					order->setOrderCompletionDate(enterDate(1980, 2080));
				}
				else if (action == 5) {
					int id_book = enterNumber(1, count_book, "Введите новый номер книги");
					if (cash_books.search(all_book[id_book - 1])) {
						book = cash_books.get(all_book[id_book - 1]);
					}
					else {
						book = gateway_books.get(all_book[id_book - 1]);
						cash_books.add(book, book->getId());
					}
					order->setIdBooks(book);
				}
				else if (action == 6) {
					order->setNumbersOfOrders(enterNumber(1, 1000000, "Введите новый количество заказов"));
				}
				else {
					gateway_orders.save(order);
					cash_orders.add(order, order->getId());
					// update value
					break;
				}
			}
			system("cls");
		} break;
		// show all
		case 6: {
			int action = enterNumber(1, 2, "Состояние просмотра таблицы польностью\n1 - Вкл. 2 - Выкл.");
			if (action == 1)
				flag_show_all = true;
			else
				flag_show_all = false;
			system("cls");
		} break;
		// exit
		case 7:
			return;
		}
	}
}
