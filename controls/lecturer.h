#ifndef LECTURER_H
#define LECTURER_H
#include "control.h"

namespace nplecturer {
	int courses_list(const char* lecturer_id);
	void info(const char* lecturer_id, bool EDIT = OFF);
	// EDIT: student
	void edit(const char* lecturer_id);
	int remove(const char* lecturer_id);
}
#endif