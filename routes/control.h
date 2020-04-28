#ifndef CONTROL_H
#define CONTROL_H
#pragma warning(disable : 4996)
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <stdlib.h>

#include "csv.h"
#include "layout.h"
#include "sha256.h"

#define ENGLISHNAME atoi(control::config("English_name").c_str())
#define ACADEMIC_YEAR control::config("Academic_year")
#define SEMESTER control::config("Semester")


namespace control {
	std::tm gtime(char* _date, char* _time);
	int now(char* date, char* start, char* end);

	std::string config(const char* DEFINE);
}

bool exists(const char* FILE);
std::string COURSE_PATH(const char* FILE);
#endif