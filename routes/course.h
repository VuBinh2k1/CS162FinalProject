#ifndef COURSE_H
#define COURSE_H
#define MAX_WEEK 11
#include "control.h"

namespace npcourse {
	bool list(const csv_file& my_course, int& choose);
	bool info(const char* course_id, const char* course_cs, int x, int y, int n = 5);
	bool now(const char* course_id, const char* course_cs, std::tm day);
	void check_in(csv_line& user);
	void check_in_result(csv_line& user);
}
#endif