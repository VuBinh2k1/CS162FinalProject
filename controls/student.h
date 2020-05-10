#ifndef STUDENT_H
#define STUDENT_H
#include "control.h"
#include "attendance.h"
#include "class.h"
#include "course.h"
#include "scoreboard.h"

namespace npstudent {
	void list(csv_line& user, const char* class_id);
	void list(csv_line& user, const char* course_id, const char* course_cs);
	void info();
	void edit(csv_line& user);
	bool remove(csv_line& user);
	int calendar(csv_line& user);
}
#endif