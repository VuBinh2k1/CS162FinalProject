#ifndef CSV_H
#define CSV_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

#define ON 1
#define OFF 0

#define ACCOUNT ".\\data\\account.csv"
#define __CLASS ".\\data\\class\\__class.csv"
#define __STAFF ".\\data\\staff\\__staff.csv"
#define __STUDENT ".\\data\\student\\__student.csv"
#define __LECTURER ".\\data\\lecturer\\__lecturer.csv"

#define def_user ".\\data\\__default\\user.csv"
#define def_class ".\\data\\__default\\class.csv"
#define def_course ".\\data\\__default\\course.csv"
#define def_process ".\\data\\__default\\process.csv"
#define def_schedule ".\\data\\__default\\schedule.csv"

class csv_line {
public:
	int id, count;		// Number of columns
	char** pdata;

	void init(std::istream& inp);
	~csv_line() { delete[] data; delete[] pdata; }
	csv_line& operator=(const csv_line& x);
	bool operator==(const char* position);

private:
	char* data;
};

class csv_file {
public:
	int count;		// Number of cows
	csv_line mark;
	csv_line* data;

	csv_file(const char* FILE, const char* __def = nullptr);
	~csv_file() { delete[] data; }
};

namespace file {
	void copy(const char* sre, const char* des);
	void update(const char* FILE, int row, int column, const char* val);
	void remove(const char* FILE, int row);
	bool exists(const char* FILE);
	const char* find(csv_file& file, int row, const char* mark);
	int find(std::string path, const char* data1, const char* data2, bool status);
	csv_line* find(csv_file& file, const char* data1, const char* data2, bool status);
}
#endif