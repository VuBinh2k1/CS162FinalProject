#ifndef ROLES_H
#define ROLES_H
#include "csv.h"
#include "layout.h"

namespace role {
	void login(csv_line& user);
	void menu(csv_line& user);
	void profile(csv_line& user);
	bool password(csv_line& user);
}
#endif