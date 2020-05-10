#ifndef COURSE_H
#define COURSE_H
#include "control.h"

namespace npcourse {
	bool now(const char* course_id, const char* course_cs, std::tm day);
	void enrol(csv_line& user, const char* course_id, const char* course_cs);
	void chkin(csv_line& user, const char* course_id, const char* course_cs);
	void info(csv_line& user, const char* course_id, const char* course_cs);
}
#endif