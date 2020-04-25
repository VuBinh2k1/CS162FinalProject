#ifndef CONTROL_H
#define CONTROL_H
#pragma warning(disable : 4996)
#include <fstream>
#include <ctime>
#include <stdlib.h>

namespace nptime {
	std::tm gtime(char* _date, char* _time);
	int now(char* date, char* start, char* end);
}

bool exists(const char* FILE);
#endif