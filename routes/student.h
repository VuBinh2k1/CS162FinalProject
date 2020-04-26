#ifndef STUDENT_H
#define STUDENT_H
#include "control.h"
#include "csv.h"
#include "layout.h"
#include "course.h"
#include "roles.h"

namespace npstudent {
	void menu(csv_line& user);
	void check_in(csv_line& user);
	void check_in_result(csv_line& user);
}
#endif