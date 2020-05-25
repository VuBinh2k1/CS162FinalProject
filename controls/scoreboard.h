#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include "csv.h"

namespace npscoreboard {
	int staff(const char* course_id, const char* course_cs);
	int lecturer(const char* course_id, const char* course_cs);
	int student(csv_line& user);
}
#endif