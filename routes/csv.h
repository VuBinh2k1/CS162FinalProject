#ifndef CSV_H
#define CSV_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

class csv_line {
public:
	int id, count;		// Number of columns
	char* data;
	char** pdata;

	void init(std::istream& inp);
	~csv_line() { delete[] data; delete[] pdata; }
	csv_line& operator=(const csv_line& x);
};

class csv_file {
public:
	int count;		// Number of cows
	csv_line mark;
	csv_line* data;

	csv_file(const char* FILE);
	~csv_file() { delete[] data; }
};

namespace npcsv {
	void update(const char* FILE, int row, int column, const char* val);
}
#endif