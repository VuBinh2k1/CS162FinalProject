#include "course.h"

bool npcourse::show(const char* course_id, const char* course_cs, int x, int y) {
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

	gotoxy(x, y, COLOR_YELLOW); std::cout << "Course Name : " << course->pdata[2];
	gotoxy(x, y + 2); std::cout << "Course ID   : " << course->pdata[1];
	gotoxy(x, y + 4); std::cout << "Class  ID   : " << course->pdata[3];
	gotoxy(x, y + 6); std::cout << "Room        : " << course->pdata[10];
	gotoxy(x, y + 8); std::cout << "Lecturer ID : " << course->pdata[4];
	//gotoxy(x, y +10); std::cout << "Study time  : " << course->pdata[7] << " from "<< course->pdata[8] << " to " <<course->pdata[9];

	return 1;
}
