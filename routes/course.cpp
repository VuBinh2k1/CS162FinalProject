#include "course.h"

bool npcourse::layout(const csv_file& my_course, char*& course_id, char*& course_cs, int& choose, int& id) {
	for (int u = -1;; u = -1) {	// Choose Up-down: [COURSE]
		for (int i = 0; i < my_course.count; ++i) {
			if (strcmp(my_course.data[i].pdata[0], "0") == 0) continue;		// Status: 0/1

			if (choose == ++u) id = i, gotoxy(7, 11 + u, COLOR_WHITE_BACKGROUND);
			else gotoxy(7, 11 + u, COLOR_WHITE);
			std::cout << my_course.data[i].pdata[1];
		}

		if (u == -1) {		// If dont student dont have any course
			gotoxy(7, 11, 8); std::cout << "(empty)";
			uint8_t c = getch();
			while (c != KEY_ENTER && c != KEY_ESC) c = getch();
			colorizing(COLOR_DEFAULT); return 0;
		}

		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) {
			course_id = my_course.data[id].pdata[1];
			course_cs = my_course.data[id].pdata[2];
			return 1;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			if (c == KEY_DOWN && choose < u) choose++;
		}
	}
}


bool npcourse::info(const char* course_id, const char* course_cs, int x, int y, int n) {
	csv_file course_list(".\\data\\course\\__course.csv");
	csv_line* course = nullptr;
	for (int i = 0; i < course_list.count; ++i) {
		if (strcmp(course_id, course_list.data[i].pdata[1])) continue;
		if (strcmp(course_cs, course_list.data[i].pdata[3])) continue;
		course = &course_list.data[i];
	}

	if (course == nullptr) {
		gotoxy(x, y, COLOR_RED); std::cout << "This course does not exist";
		return 0;
	}

	if (n > 0) { gotoxy(x, y, COLOR_YELLOW); std::cout << "Course Name : " << course->pdata[2]; }
	if (n > 1) { gotoxy(x, y + 2); std::cout << "Course ID   : " << course->pdata[1]; }
	if (n > 2) { gotoxy(x, y + 4); std::cout << "Class  ID   : " << course->pdata[3]; }
	if (n > 3) { gotoxy(x, y + 6); std::cout << "Lecturer ID : " << course->pdata[4]; }
	if (n > 4) { gotoxy(x, y + 8); std::cout << "Room        : " << course->pdata[10]; }

	return 1;
}
