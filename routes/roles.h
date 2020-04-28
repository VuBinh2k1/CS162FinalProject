#ifndef ROLES_H
#define ROLES_H
#include "control.h"

namespace role {
	bool login(csv_line& user);
	bool profile(csv_line& user);
	bool new_password(csv_line& user);
	bool password(csv_line& user);
}
#endif