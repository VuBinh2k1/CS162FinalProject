#ifndef LECTURER_H
#define LECTURER_H
#include "control.h"

namespace nplecturer {
<<<<<<< HEAD
	int list();
	void info(const char* lecturer_id, bool EDIT);
	void courseslist(const char* lecturer_id);
	void edit(const char* lecturer_id);
	void remove(const char* lecturer_id);
=======
	int courses_list(const char* lecturer_id);
	void info(const char* lecturer_id, bool EDIT = OFF);
	// EDIT: student
	void edit(const char* lecturer_id);
	int remove(const char* lecturer_id);
>>>>>>> master
}
void removelecturer_course(const char* lecturer_id);
#endif