#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include "csv.h"

namespace npscoreboard {
	int staff(const char* course_id, const char* course_cs, int first_choose);
	int lecturer(const char* course_id, const char* course_cs, int first_choose, int first_overflow);
	int student(csv_line& user);
	// EDIT: scoreboard
	void save(const char* course_id, const char* course_cs);
	void open(const char* course_id, const char* course_cs);
	void edit(const char* course_id, const char* course_cs, int y, int i);
}
#endif