#ifndef STUDENT_H
#define STUDENT_H
#include "control.h"
#include "attendance.h"
#include "class.h"
#include "course.h"
#include "scoreboard.h"

namespace npstudent {
	void info();
	void edit(csv_line& user);
	bool remove(csv_line& user);
	void checkin(csv_line& user);
	void calendar(csv_line& user);
}
#endif