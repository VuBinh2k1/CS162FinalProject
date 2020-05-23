#ifndef LECTURER_H
#define LECTURER_H
#include "control.h"

namespace nplecturer {
	int list();
	void info(const char* lecturer_id, bool EDIT);
	void courseslist(const char* lecturer_id);
	void edit(const char* lecturer_id);
	void remove(const char* lecturer_id);
}
void removelecturer_course(const char* lecturer_id);
#endif