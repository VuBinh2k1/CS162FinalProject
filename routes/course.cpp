#include "..\\controls\\course.h"

bool npcourse::now(const char* course_id, const char* course_cs, std::tm day) {
	csv_file schedule((COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv").c_str(), def_schedule);
	for (int i = 0; i < schedule.count; ++i) {
		if (control::now(day, schedule.data[i].pdata[1]) == 0) return 1;
	}
	return 0;
}

bool npcourse::info(const char* course_id, const char* course_cs, int x, int y, int n) {
	csv_file course_list(COURSE_PATH("__course.csv").c_str(), def_course);
	csv_line* course = nullptr;
	for (int i = 0; i < course_list.count; ++i) {
		if (strcmp(course_id, course_list.data[i].pdata[1])) continue;
		if (strcmp(course_cs, course_list.data[i].pdata[3])) continue;
		course = &course_list.data[i];
	}

	if (course == nullptr) {
		if (n > 0) { gotoxy(x, y, COLOR_RED); std::cout << "This course does not exist"; }
		return 0;
	}

	if (n > 0) { gotoxy(x, y, COLOR_YELLOW); std::cout << "Course Name : " << course->pdata[2]; }
	if (n > 1) { gotoxy(x, y + 2); std::cout << "Course ID   : " << course->pdata[1]; }
	if (n > 2) { gotoxy(x, y + 4); std::cout << "Class       : " << course->pdata[3]; }
	if (n > 3) { gotoxy(x, y + 6); std::cout << "Lecturer ID : " << course->pdata[4]; }
	if (n > 4) { gotoxy(x, y + 8); std::cout << "Room        : " << course->pdata[10]; }

	return 1;
}

csv_line* npcourse::find(csv_line* beg, csv_line* end, const char* courseid, int id) {
	for (csv_line* course = beg; course != end; ++course) {
		if (id == 2 && strcmp(course->pdata[0], ACADEMICYEAR.c_str())) continue;
		if (id == 2 && strcmp(course->pdata[1], SEMESTER.c_str())) continue;
		if (strcmp(course->pdata[id], courseid) == 0) return course;
	}
	return nullptr;
}

csv_line* npcourse::choose(const csv_file& my_course, int& choose, int id) {
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "    My courses    ";

	csv_line* course = nullptr; int overflow = 0;

	// Course ID without class: O(len.logn)
	std::map <std::string, bool> unClass;

	while (1) {	// Choose Up-down: [COURSE]
		int cur = -1;
		for (int i = 0; i < my_course.count; ++i) {
			if (id == 2 && strcmp(my_course.data[i].pdata[0], ACADEMICYEAR.c_str())) continue;
			if (id == 2 && strcmp(my_course.data[i].pdata[1], SEMESTER.c_str())) continue;

			std::string courseid = my_course.data[i].pdata[id];
			if (unClass.find(courseid) != unClass.end()) continue;
			int y = 9 + (++cur) + overflow;
			if (y < 9 || y > 27) continue;
			if (choose == cur) course = &my_course.data[i];
			unClass[courseid] = i;

			gotoxy(2, y, (choose == cur ? COLOR_WHITE_BACKGROUND : COLOR_WHITE)); std::cout << "                  ";
			gotoxy(7, y, (choose == cur ? COLOR_WHITE_BACKGROUND : COLOR_WHITE)); std::cout << my_course.data[i].pdata[id];
			
		}
		unClass.clear();
		gotoxy(2, min(28, 10 + cur), 224); std::cout << "  <-- Main menu   ";

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return nullptr;
		if (c == KEY_ENTER) {
			gotoxy(2, 9 + choose + overflow, 8); std::cout << "                  ";
			gotoxy(7, 9 + choose + overflow, 8); std::cout << course->pdata[id];
			return course;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 17) overflow--; }
			else if (c == KEY_LEFT) return nullptr;
			else goto UN_CHANGE;
		}
	}
}
