#ifndef ROLES_H
#define ROLES_H
#include "csv.h"
#include "sha256.h"
#include "lecturer.h"
#include "student.h"
#include "course.h"
#include "attendance.h"
#include "scoreboard.h"

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
	int lecturer_list();
	int calendar(csv_line& user);
	void settings();
}
#endif