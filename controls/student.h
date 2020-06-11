#ifndef STUDENT_H
#define STUDENT_H
#include "csv.h"
#include "class.h"
#include "course.h"
#include "attendance.h"

namespace npstudent {
	void list(csv_line& user, const char* class_id);
	void list(csv_line& user, const char* course_id, const char* course_cs);
	void info(const char* student_id,const char* class_id, bool EDIT = OFF, bool _DELETE = OFF);
	void search(csv_file& student_list, int cur, int& choose, int& overflow, int* row);
	// EDIT: student
	void addnewstudent(const char* class_id);
	void open(const char* class_id);
	void sort(const char* FILE, int col1, int col2 = -1, int col3 = -1);
	bool sort(const char* FILE);
	void edit(const char* student_id);
	int remove(const char* student_id,const char* class_id);
	int remove(const char* student_id, const char* course_id, const char* course_cs);
}
#endif