#ifndef ROLES_H
#define ROLES_H
#include "control.h"
#include "student.h"
#include "course.h"
#include "attendance.h"
#include "scoreboard.h"
#include "lecturer.h"

namespace role {
	// All roles
	bool login(csv_line& user);
	bool profile(csv_line& user);
	bool new_password(csv_line& user);
	bool password(csv_line& user);
	// Main menu
	void staff(csv_line& user);
	void lecturer(csv_line& user);
	void student(csv_line& user);
	// Sub menu
	int classes_list(csv_line& user);
	int courses_list(csv_line& user);
	int calendar(csv_line& user);
	void settings();
}
#endif