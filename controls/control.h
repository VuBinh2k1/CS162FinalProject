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
#include <direct.h>
#include "layout.h"

#define MAX_WEEK 11
#define WEEK_COLUMN 6
#define INVALID_MEMORY_ADDRESS 0xcccccccccccccccc

namespace control {
	std::tm gtime(char* _date = nullptr, char* _time = nullptr);
	int now(char* date, char* start, char* end);
	int now(std::tm ltm, char* date);
	std::string config(const char* DEFINE);
}

// Function 
std::string COURSE_PATH(const char* FILE);
void academicmark();
// Lower all letters of std::string
void lowercase(std::string& str);
// Upper all letters of std::string
void uppercase(std::string& str);
// Capital first letters of std::string
void capitalize(std::string& str);
const char* US_GPA(const char* strsco);
const char* US_Grade(const char* strsco);

// Funtion pointer
typedef bool (__cdecl *comparator)(const char* x, const char* y);

// .config data
#define ENGLISHNAME atoi(control::config("ENGLISHNAME").c_str())
#define ACADEMICYEAR control::config("ACADEMICYEAR")
#define SEMESTER control::config("SEMESTER")
#endif