#include "control.h"

std::tm nptime::gtime(char* _date, char* _time) {
	std::tm ttm;

	//_date: dd/mm/yyyy\0
	ttm.tm_mday = _date[0] * 10 + _date[1] - 11 * '0';
	ttm.tm_mon = _date[3] * 10 + _date[4] - 11 * '0' - 1;	// month since 0
	ttm.tm_year = atoi(_date + 6) - 1900;					// year since 1900

	//_time: hh:mm\0
	ttm.tm_hour = _time[0] * 10 + _time[1] - 11 * '0';
	ttm.tm_min = _time[3] * 10 + _time[4] - 11 * '0';
	return ttm;
}

int nptime::now(char* date, char* start, char* end) {
	std::time_t now = time(0);
	std::tm ltm = *localtime(&now);
	std::tm stm = nptime::gtime(date, start);
	std::tm etm = nptime::gtime(date, end);

	if (ltm.tm_year < stm.tm_year) return -1;
	if (ltm.tm_year > stm.tm_year) return 1;
	if (ltm.tm_mon < stm.tm_mon) return -1;
	if (ltm.tm_mon > stm.tm_mon) return 1;
	if (ltm.tm_mday < stm.tm_mday) return -1;
	if (ltm.tm_mday > stm.tm_mday) return 1;
	if (ltm.tm_hour < stm.tm_hour) return -1;
	if (ltm.tm_hour > etm.tm_hour) return 1;
	if (ltm.tm_hour == stm.tm_hour && ltm.tm_min < stm.tm_min) return -1;
	if (ltm.tm_hour == etm.tm_hour && ltm.tm_min > etm.tm_min) return 1;
	return 0;
}
