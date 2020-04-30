#include "control.h"

std::tm control::gtime(char* _date, char* _time) {
	std::tm ttm;

	//_date: dd/mm/yyyy\0
	ttm.tm_mday = _date[0] * 10 + _date[1] - 11 * '0';
	ttm.tm_mon = _date[3] * 10 + _date[4] - 11 * '0' - 1;	// month since 0
	ttm.tm_year = atoi(_date + 6) - 1900;					// year since 1900

	//_time: hh:mm\0
	if (_time == nullptr) return ttm;
	ttm.tm_hour = _time[0] * 10 + _time[1] - 11 * '0';
	ttm.tm_min = _time[3] * 10 + _time[4] - 11 * '0';
	return ttm;
}

int control::now(char* date, char* start, char* end) {
	std::time_t now = time(0);
	std::tm ltm = *localtime(&now);
	std::tm stm = control::gtime(date, start);
	std::tm etm = control::gtime(date, end);

	if (ltm.tm_year < stm.tm_year) return -1;
	if (ltm.tm_year > stm.tm_year) return 1;
	if (ltm.tm_mon  < stm.tm_mon) return -1;
	if (ltm.tm_mon  > stm.tm_mon) return 1;
	if (ltm.tm_mday < stm.tm_mday) return -1;
	if (ltm.tm_mday > stm.tm_mday) return 1;
	if (ltm.tm_hour < stm.tm_hour) return -1;
	if (ltm.tm_hour > etm.tm_hour) return 1;
	if (ltm.tm_hour == stm.tm_hour && ltm.tm_min < stm.tm_min) return -1;
	if (ltm.tm_hour == etm.tm_hour && ltm.tm_min > etm.tm_min) return 1;
	return 0;
}

int control::now(std::tm ltm, char* date) {
	std::tm cur = control::gtime(date);
	if (ltm.tm_year < cur.tm_year) return -1;
	if (ltm.tm_year > cur.tm_year) return 1;
	if (ltm.tm_mon  < cur.tm_mon) return -1;
	if (ltm.tm_mon  > cur.tm_mon) return 1;
	if (ltm.tm_mday < cur.tm_mday) return -1;
	if (ltm.tm_mday > cur.tm_mday) return 1;
	return 0;
}

void control::print(std::tm date) {
	if (date.tm_mday < 10) std::cout << '0';
	std::cout << date.tm_mday << '/';
	if (date.tm_mon < 9) std::cout << '0';
	std::cout << date.tm_mon + 1 << '/';
	std::cout << date.tm_year + 1900;
}

std::string control::config(const char* DEFINE) {
	std::ifstream cfg(".\\.config");

	char temp[1000];
	while (cfg.getline(temp, 1000)) {
		char* CONFIG_CODE = strstr(temp, DEFINE);
		if (CONFIG_CODE != NULL) {
			return std::string(strchr(temp, ' ') + 1);
		}
	}
	return std::string(0);
}

bool exists(const char* FILE) {
	std::ifstream inp(FILE);
	if (!inp.is_open()) return 0;
	inp.close(); return 1;
}

std::string COURSE_PATH(const char* FILE) {
	return std::string(".\\data\\course\\") + ACADEMICYEAR + "-" + SEMESTER + "\\" + FILE;
}

void academicmark() {
	gotoxy(2, 1, 95); std::cout << "        ";
	gotoxy(4, 1, 95); std::cout << ACADEMICYEAR;
	if (atoi(SEMESTER.c_str()) == 1) { gotoxy(2, 2, 192); std::cout << " Spring "; }
	if (atoi(SEMESTER.c_str()) == 2) { gotoxy(2, 2, 160); std::cout << " Summer "; }
	if (atoi(SEMESTER.c_str()) == 3) { gotoxy(2, 2, 224); std::cout << "  Fall  "; }
	if (atoi(SEMESTER.c_str()) == 4) { gotoxy(2, 2, 176); std::cout << " Winter "; }
}
