#ifndef STAFF_H
#define STAFF_H
#include "control.h"
#include "roles.h"
#include "class.h"
#include "course.h"
#include "student.h"
#include "scoreboard.h"

namespace npstaff {
	void menu(csv_line& user);
	void menu_class();
	void settings();
}
#endif