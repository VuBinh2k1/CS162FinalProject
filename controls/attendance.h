#ifndef ATTENDANCE_H
#define ATTENDANCE_H
#include "control.h"
#include "course.h"
#include "scoreboard.h"

namespace npattendance {
	int staff(const char* course_id, const char* course_cs);
	int lecturer(const char* course_id, const char* course_cs);
	int student(csv_line& user);
}
#endif