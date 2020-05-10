#ifndef COURSE_H
#define COURSE_H
#define MAX_WEEK 11
#define WEEK_COLUMN 6
#include "control.h"

namespace npcourse {
	bool now(const char* course_id, const char* course_cs, std::tm day);
	void enrol(csv_line& user, const char* course_id, const char* course_cs);
	void chkin(csv_line& user, const char* course_id, const char* course_cs);
	void info(csv_line& user, const char* course_id, const char* course_cs);
}
#endif