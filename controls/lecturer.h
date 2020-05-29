#ifndef LECTURER_H
#define LECTURER_H
#include "csv.h"

namespace nplecturer {
	// FUNCTION: lecturer
	int courses_list(const char* lecturer_id);
	void info(const char* lecturer_id, bool EDIT = OFF, bool _DELETE = OFF);
	void search(csv_file& lecturer_list, int cur, int& choose, int& overflow, int* row);
	// EDIT: lecturer
	void add();		//void newlecturer();
	bool sort();
	void edit(const char* lecturer_id);
	int remove(const char* lecturer_id);
}
#endif
