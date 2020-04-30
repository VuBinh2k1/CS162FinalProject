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

#define ENGLISHNAME atoi(control::config("ENGLISHNAME").c_str())
#define ACADEMICYEAR control::config("ACADEMICYEAR")
#define SEMESTER control::config("SEMESTER")


namespace control {
	std::tm gtime(char* _date, char* _time = nullptr);
	int now(char* date, char* start, char* end);
	int now(std::tm ltm, char* date);
	void print(std::tm date);

	std::string config(const char* DEFINE);
}

bool exists(const char* FILE);
std::string COURSE_PATH(const char* FILE);
void academicmark();
#endif