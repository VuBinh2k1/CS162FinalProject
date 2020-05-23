#ifndef COURSE_H
#define COURSE_H
#include "control.h"

namespace npcourse {
	void info(csv_line& user, const char* course_id, const char* course_cs);
	void enrol(csv_line& user, const char* course_id, const char* course_cs);
	void chkin(csv_line& user, const char* course_id, const char* course_cs);
	// EDIT: course
	bool now(const char* course_id, const char* course_cs, std::tm day);
	void edit(const char* course_id, const char* course_cs);
	int remove(const char* course_id, const char* course_cs);
	void schedule(const char* course_id, const char* course_cs);
}
#endif