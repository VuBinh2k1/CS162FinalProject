#include "..\\controls\\control.h"

std::tm control::gtime(char* _date, char* _time) {
	std::time_t now = time(0);
	std::tm ttm = *localtime(&now);

	//_date: dd/mm/yyyy\0
	if (_date == nullptr || strlen(_date) < 10) return ttm;
	ttm.tm_mday = _date[0] * 10 + _date[1] - 11 * '0';
	ttm.tm_mon = _date[3] * 10 + _date[4] - 11 * '0' - 1;	// month since 0
	ttm.tm_year = atoi(_date + 6) - 1900;					// year since 1900

	//_time: hh:mm\0
	if (_time == nullptr || strlen(_time) < 5) return ttm;
	ttm.tm_hour = _time[0] * 10 + _time[1] - 11 * '0';
	ttm.tm_min = _time[3] * 10 + _time[4] - 11 * '0';
	return ttm;
}

int control::now(char* date, char* start, char* end) {
	std::tm ltm = control::gtime();
	std::tm stm = control::gtime(date, start);
	std::tm etm = control::gtime(date, end);

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

int control::now(std::tm ltm, char* date) {
	std::tm cur = control::gtime(date);
	if (ltm.tm_year < cur.tm_year) return -1;
	if (ltm.tm_year > cur.tm_year) return 1;
	if (ltm.tm_mon < cur.tm_mon) return -1;
	if (ltm.tm_mon > cur.tm_mon) return 1;
	if (ltm.tm_mday < cur.tm_mday) return -1;
	if (ltm.tm_mday > cur.tm_mday) return 1;
	return 0;
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
	return std::string();
}

std::string COURSE_PATH(const char* FILE) {
	return std::string(".\\data\\course\\") + ACADEMICYEAR + "-" + SEMESTER + "\\" + FILE;
}

void academicmark() {
	gotoxy(2, 1, 95); std::cout << "        ";
	gotoxy(4, 1, 95); std::cout << ACADEMICYEAR;
	if (atoi(SEMESTER.c_str()) == 1) { gotoxy(2, 2, 192); std::cout << " Spring "; }
	if (atoi(SEMESTER.c_str()) == 2) { gotoxy(2, 2, 160); std::cout << " Summer "; }
	if (atoi(SEMESTER.c_str()) == 3) { gotoxy(2, 2, 224); std::cout << " Autumn "; }
	if (atoi(SEMESTER.c_str()) == 4) { gotoxy(2, 2, 176); std::cout << " Winter "; }
}

void lowercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void uppercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void capitalize(std::string& str) {
	for (int i = 0; i < str.size(); ++i) {
		if (i == 0 || str[i - 1] == ' ') str[i] = toupper(str[i]);
		else str[i] = tolower(str[i]);
	}
}

const char* US_GPA(const char* strsco) {
	if (strsco[0] == '\0') return "";
	double score = std::stoi((std::string)strsco);
	if (score >= 9.0) return "4.0";
	if (score >= 8.0) return "3.5";
	if (score >= 7.0) return "3.0";
	if (score >= 6.0) return "2.5";
	if (score >= 5.0) return "2.0";
	if (score >= 4.0) return "1.5";
	return "0.0";
}

const char* US_Grade(const char* strsco) {
	if (strsco[0] == '\0') return "";
	double score = std::stoi((std::string)strsco);
	if (score >= 9.5) return "A+";
	if (score >= 9.0) return "A";
	if (score >= 8.5) return "A-";
	if (score >= 8.0) return "B+";
	if (score >= 7.5) return "B";
	if (score >= 7.0) return "B-";
	if (score >= 6.5) return "C+";
	if (score >= 6.0) return "C";
	if (score >= 5.5) return "C-";
	if (score >= 5.0) return "D+";
	if (score >= 4.5) return "D";
	if (score >= 4.0) return "D-";
	return "F";
}
