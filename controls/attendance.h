#ifndef ATTENDANCE_H
#define ATTENDANCE_H
#include "csv.h"
#include "scoreboard.h"

namespace npattendance {
	int staff(const char* course_id, const char* course_cs, int first_choose);
	int lecturer(const char* course_id, const char* course_cs, int first_choose, int first_overflow);
	int student(csv_line& user);
	// EDIT: attendance
	void save(const char* course_id, const char* course_cs);
	void edit(const char* course_id, const char* course_cs, int row, int i);
}
#endif