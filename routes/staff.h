#ifndef STAFF_H
#define STAFF_H
#include "control.h"
#include "csv.h"
#include "layout.h"
#include "class.h"
#include "course.h"
#include "roles.h"
#include "student.h"

namespace npstaff {
	void menu(csv_line& user);
	void menu_class();
}
#endif