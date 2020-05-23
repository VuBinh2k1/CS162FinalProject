#ifndef CONTROL_H
#define CONTROL_H
#pragma warning(disable : 4996)
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <map>
#include <stdlib.h>

#include "csv.h"
#include "layout.h"
#include "sha256.h"

#define MAX_WEEK 11
#define WEEK_COLUMN 6
#define INVALID_MEMORY_ADDRESS 0xcccccccccccccccc

// .config data
#define ENGLISHNAME atoi(control::config("ENGLISHNAME").c_str())
#define ACADEMICYEAR control::config("ACADEMICYEAR")
#define SEMESTER control::config("SEMESTER")

namespace control {
	std::tm gtime(char* _date = nullptr, char* _time = nullptr);
	int now(char* date, char* start, char* end);
	int now(std::tm ltm, char* date);
	void print(std::tm date);
	std::string config(const char* DEFINE);
}

//void back(uint8_t KEY_ARROW_BREAK);
std::string COURSE_PATH(const char* FILE);
void academicmark();
const char* US_GPA(const char* strsco);
const char* US_Grade(const char* strsco);
#endif