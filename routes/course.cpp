#include "course.h"

bool npcourse::list(const csv_file& my_course, int& choose, int& id) {
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "    My courses    ";
	int cur, overflow = 0;
	for (;cur = -1;) {	// Choose Up-down: [COURSE]
		for (int i = 0; i < my_course.count; ++i) {
			if (strcmp(my_course.data[i].pdata[0], "0") == 0) continue;		// Status: 0/1
			if (++cur + overflow < 0 || cur + overflow > 16) continue;		// Overflow menu
			if (choose == cur) id = i;										// Found course

			WORD COLOR_CODE = (choose == cur) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE;
			gotoxy(2, 9 + cur + overflow, COLOR_CODE);
			std::cout << "     " << my_course.data[i].pdata[1] << "     ";
		}

		if (cur == -1) {		// If student dont have any course
			gotoxy(7, 11, 8); std::cout << "(empty)";
			uint8_t c = getch();
			while (c != KEY_ENTER && c != KEY_ESC) c = getch();
			colorizing(COLOR_DEFAULT); return 0;
		}

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) return 1;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) {
				if (--choose + overflow < 0) overflow++;
			} else
			if (c == KEY_DOWN && choose < cur) {
				if (++choose + overflow > 16) overflow--;
			} else goto UN_CHANGE;
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

void npcourse::check_in(csv_line& user) {
	colorizing(COLOR_DEFAULT); system("cls");
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("course.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout course_layout(inp);
	layout course_info_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	char* course_id = nullptr;
	char* course_cs = nullptr;
	std::string stupath = (std::string)".\\data\\student\\" + username + ".csv";
	csv_file my_course(stupath.c_str());

	course_layout.print();
	int chs1 = 0, id = -1;

LABLE_COURSE:	// Label: LABLE_COURSE (use goto)
	if (npcourse::list(my_course, chs1, id) == 0) return;
	course_id = my_course.data[id].pdata[1];
	course_cs = my_course.data[id].pdata[2];

	// Print Course Info:
	course_info_layout.print();
	if (npcourse::info(course_id, course_cs, 33, 9) == 0) {
		PAUSE; return;
	}

	// Load SCHEDULE: COURSE_CLASS.csv or make __dafault
	std::string schpath = (std::string)".\\data\\course\\SCHEDULE\\" + course_id + "_" + course_cs + ".csv";
	if (!exists(schpath.c_str())) {
		// If .\\SCHEDULE\\COURSEID_COURSECLASS.csv does not exist
		goto LABLE_COURSE;
	}
	csv_file schedule(schpath.c_str());

	for (int i = 0; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		csv_line* mycou = &my_course.data[id];

		if (strcmp(date->pdata[1], "0") == 0) continue;
		int now = nptime::now(date->pdata[1], date->pdata[2], date->pdata[3]);

		if (now == 0) {
			gotoxy(33, 19); std::cout << "Date        : " << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";

			if (strcmp(mycou->pdata[3 + i], "1") != 0) {

				int chs = 0;
				while (1) {	// Choose Left-right: [Checkin][Cancel]
					gotoxy(51, 27, (chs == 0) ? COLOR_GREEN_BACKGROUND : COLOR_WHITE); std::cout << "[Checkin]";
					gotoxy(61, 27, (chs == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Cancel]";
					colorizing(COLOR_DEFAULT);

					uint8_t c = getch();
					if (c == KEY_ESC) { chs = 1; break; }
					if (c == KEY_ENTER) break;
					if (c == 224 || c == 0) {
						if (chs == 0 && getch() == KEY_RIGHT ||
							chs == 1 && getch() == KEY_LEFT) {
							chs = !chs;
						}
					}
				}

				if (chs == 1) {
					gotoxy(36, 27, COLOR_YELLOW); std::cout << "You will miss class if you don't take attendance.";
					goto LABLE_COURSE;
				}

				mycou->pdata[3 + i][0] = '1';
				npcsv::update(stupath.c_str(), mycou->id, 3 + i, "1");
			}

			gotoxy(40, 27, COLOR_GREEN); std::cout << "You have already checked in this course.";
			goto LABLE_COURSE;
		}
		else if (now == -1) {
			// The nearest day the course will start
			gotoxy(33, 19); std::cout << "Date        : " << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";
			gotoxy(46, 27, COLOR_WHITE); std::cout << "This course has not started.";
			goto LABLE_COURSE;
		}
	}
	// The last day of the course has ended
	gotoxy(49, 27, COLOR_WHITE); std::cout << "The course has ended.";
	goto LABLE_COURSE;
}

void npcourse::check_in_result(csv_line& user) {
	colorizing(COLOR_DEFAULT); system("cls");
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("student_schedule.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout course_layout(inp);
	layout course_info_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	char* course_id = nullptr;
	char* course_cs = nullptr;
	std::string stupath = (std::string)".\\data\\student\\" + username + ".csv";
	csv_file my_course(stupath.c_str());

	course_layout.print();
	gotoxy(7, 9, COLOR_YELLOW); std::cout << "[COURSE]";
	int chs1 = 0, id = -1;

LABLE_COURSE:	// Label: LABLE_COURSE (use goto)
	if (npcourse::list(my_course, chs1, id) == 0) return;
	course_id = my_course.data[id].pdata[1];
	course_cs = my_course.data[id].pdata[2];

	// Print Course Info:
	course_info_layout.print();
	if (npcourse::info(course_id, course_cs, 33, 9, 1) == 0) {
		PAUSE; return;
	}
	gotoxy(29, 27, COLOR_YELLOW_BACKGROUND); std::cout << " UNREGISTER ";
	gotoxy(46, 27, COLOR_GREEN_BACKGROUND);	 std::cout << " CHECKED IN ";
	gotoxy(63, 27, COLOR_RED_BACKGROUND);    std::cout << "   MISSED   ";
	gotoxy(80, 27, COLOR_WHITE_BACKGROUND);  std::cout << "  UPCOMING  ";

	// Load SCHEDULE: COURSE_CLASS.csv or make __dafault
	std::string schpath = (std::string)".\\data\\course\\SCHEDULE\\" + course_id + "_" + course_cs + ".csv";
	if (!exists(schpath.c_str())) {
		// If .\\SCHEDULE\\COURSEID_COURSECLASS.csv does not exist
		goto LABLE_COURSE;
	}
	csv_file schedule(schpath.c_str());

	for (int i = 0, x = 12, y = 12; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		csv_line* mycou = &my_course.data[id];
		if ((x += 11) > 90) x = 23, y += 2;

		WORD COLOR_CODE = COLOR_WHITE_BACKGROUND;

		if (strcmp(date->pdata[1], "0") != 0) {
			int now = nptime::now(date->pdata[1], date->pdata[2], date->pdata[3]);
			if (strcmp(mycou->pdata[3 + i], "1") == 0) COLOR_CODE = COLOR_GREEN_BACKGROUND;
			else if (now == 0) COLOR_CODE = COLOR_YELLOW_BACKGROUND;
			else if (now == 1) COLOR_CODE = COLOR_RED_BACKGROUND;
		}
		gotoxy(x, y, COLOR_CODE); std::cout << " " << date->pdata[0] << " ";
	}
	goto LABLE_COURSE;
}