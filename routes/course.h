#ifndef COURSE_H
#define COURSE_H
#define MAX_WEEK 11
#define WEEK_COLUMN 6
#include "control.h"

namespace npcourse {
	bool now(const char* course_id, const char* course_cs, std::tm day);
	bool info(const char* course_id, const char* course_cs, int x, int y, int n = 5);
	void check_in(csv_line& user);
	void check_in_result(csv_line& user);
	csv_line* find(csv_line* beg, csv_line* end, const char* courseid, int id = 2);
	csv_line* choose(const csv_file& my_course, int& choose, int id = 2);
}
#endif