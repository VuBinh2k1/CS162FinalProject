#ifndef STUDENT_H
#define STUDENT_H
#include "control.h"
#include "class.h"
#include "course.h"
#include "roles.h"

namespace npstudent {
	void menu(csv_line& user);
	void info();
	void edit(csv_line& user);
	bool remove(csv_line& user);
	void calender(csv_line& user);
}
#endif