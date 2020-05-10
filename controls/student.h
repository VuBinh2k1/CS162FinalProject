#ifndef STUDENT_H
#define STUDENT_H
#include "control.h"
#include "class.h"
#include "course.h"
#include "attendance.h"

namespace npstudent {
	void list(csv_line& user, const char* class_id);
	void list(csv_line& user, const char* course_id, const char* course_cs);
	void info(const char* student_id, bool EDIT = OFF);
	int calendar(csv_line& user);
}
#endif