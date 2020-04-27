#ifndef COURSE_H
#define COURSE_H
#define MAX_WEEK 11
#include "csv.h"
#include "layout.h"

namespace npcourse {
	bool list(const csv_file& my_course, char*& course_id, char*& course_cs, int& choose, int& id);
	bool info(const char* course_id, const char* course_cs, int x, int y, int n = 5);
}
#endif