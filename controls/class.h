#ifndef CLASS_H
#define CLASS_H
#include "csv.h"

namespace npclass {
	// FUNCTION: comparator
	bool sort_cmp_class(const char* x, const char* y);
	// EDIT: class
	void add();
	bool sort();
	bool remove(const char* class_name);
	void search(csv_file& class_list, int cur, int& choose, int& overflow);
	bool change(csv_line& user, const char* class1, const char* class2);
}
#endif