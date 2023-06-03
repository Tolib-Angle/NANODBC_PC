#pragma once
#include <string.h>
#include <list>
#include <sstream>
#include "functions.h"
#include <iostream>

using namespace std;

int enterNumber(int min, int max, string textCommand) {
    int number = min - 1;
    while (true) {
        cout << textCommand << endl;
        cout << ">> ";
        cin >> number;
        if ((number >= min) && (number <= max) && (cin.peek() == '\n')) {
            break;
        }
        else {
            cout << "Некорректное значение!" << endl;
            cin.clear();
            while (cin.get() != '\n') {}
        }
        cout << endl;
    }
    return number;
}

float enterNumber(float min, float max, string textCommand) {
    float number = min - 1;
    while (true) {
        cout << textCommand << endl;
        cout << ">> ";
        cin >> number;
        if ((number >= min) && (number <= max) && (cin.peek() == '\n')) {
            break;
        }
        else {
            cout << "Некорректное значение!" << endl;
            cin.clear();
            while (cin.get() != '\n') {}
        }
        cout << endl;
    }
    return number;
}

string enterString(int min, int max, string textCommand) {
    string res = "empty";
    while (true) {
        cout << textCommand << endl;
        cout << ">> ";
        std::getline(cin, res);
        if (res.length() >= min && res.length() <= max)
            break;
        else
            cout << "Некорректное значение! Строка должен содержать от " << min << " до " << max << " символов!" << endl;
    }
    return res;
}

string enterDate(int begin_year, int end_year) {
    string date = "01-01-2023";
    cout << "Введите новое значение даты" << endl;
    int day = 1, month = 1, year = 2020;
    stringstream str;
    day = enterNumber(1, 31, "Введите день!");
    month = enterNumber(1, 12, "Введите месяц!");
    year = enterNumber(begin_year, end_year, "Введите год!");
    if (day < 10) str << "0" << day; else str << day;
    if (month < 10) str << "-0" << month; else str << "-" << month;
    str << "-" << year;
    return str.str().c_str();
}