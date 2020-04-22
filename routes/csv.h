#ifndef CSV_H
#define CSV_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

class csv_line {
public:
	int count;		// Number of columns
	char* data;
	char** pdata;
	void init(std::istream& inp, int mark_count);
	void init(const csv_line& x);
	~csv_line() { free(data); delete[] pdata; }
};

class csv_file {
public:
	int count;		// Number of cows
	csv_line mark;
	csv_line* data;
	csv_file(const char* FILE, int mark_count);
	~csv_file() { delete[] data; }
};
#endif