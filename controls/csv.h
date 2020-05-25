#ifndef CSV_H
#define CSV_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "control.h"

// ON-OFF checking status of data file
#define ON 1
#define OFF 0

// Path of default files
#define def_user ".\\data\\__default\\user.csv"
#define def_class ".\\data\\__default\\class.csv"
#define def_course ".\\data\\__default\\course.csv"
#define def_process ".\\data\\__default\\process.csv"
#define def_schedule ".\\data\\__default\\schedule.csv"

// Path of data files
#define ACCOUNT ".\\data\\account.csv"
#define __CLASS ".\\data\\class\\__class.csv"
#define __STAFF ".\\data\\staff\\__staff.csv"
#define __STUDENT ".\\data\\student\\__student.csv"
#define __LECTURER ".\\data\\lecturer\\__lecturer.csv"
#define __COURSE COURSE_PATH("__course.csv").c_str()
#define PROCESS(course_id, course_cs) (COURSE_PATH("process\\") + course_id + '_' + course_cs + ".csv").c_str()
#define SCHEDULE(course_id, course_cs) (COURSE_PATH("schedule\\") + course_id + '_' + course_cs + ".csv").c_str()

class csv_line {
public:
	int id;			// Location row
	int count;		// Number of columns
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
	int count;		// Number of rows
	csv_line mark;
	csv_line* data;

	csv_file(const char* FILE, const char* __def = nullptr);
	csv_file(const char* FILE, const char* course_id, const char* course_cs);
	~csv_file() { delete[] data; }
};

namespace file {
	// FUNCTION: comparator
	bool sort_cmp_default(const char* x, const char* y);
	// EDIT: file.csv
	void copy(const char* sre, const char* des);
	void sort(const char* FILE, int col1, int col2 = -1, int col3 = -1);
	void sort(const char* FILE, int col1, comparator _cmp);
	void sort(const char* FILE, int col1, int col2, comparator _cmp);
	void sort(const char* FILE, int col1, int col2, int col3, comparator _cmp);
	void update(const char* FILE, int row, int column, const char* val);
	void remove(const char* FILE, int row);
	bool exists(const char* FILE);
	void mksche(const char* course_id, const char* course_cs);
	// DATA: find
	const char* find(csv_file& file, int row, const char* mark);						// Find by mark, row: data
	int find(std::string path, const char* data1, const char* data2, bool status);		// Find by data: row
	csv_line* find(csv_file& file, const char* data1, const char* data2, bool status);	// Find by data: csv_line*
}
#endif