#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include "control.h"
#include "course.h"

namespace npscoreboard {
	void staff();
	void lecturer(csv_line& user);
	void student(csv_line& user);
}
#endif