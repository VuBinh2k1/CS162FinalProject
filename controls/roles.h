#ifndef ROLES_H
#define ROLES_H
#include "control.h"
#include "lecturer.h"
#include "staff.h"
#include "student.h"

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
	int cls(csv_line& user);
	int crs(csv_line& user);
}
#endif