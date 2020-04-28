#ifndef STAFF_H
#define STAFF_H
#include "control.h"
#include "csv.h"
#include "layout.h"
#include "class.h"
#include "course.h"
#include "roles.h"
void editstudent(csv_line& user);
namespace npstaff {
	void menu(csv_line& user);
	void edit_student();
}
#endif