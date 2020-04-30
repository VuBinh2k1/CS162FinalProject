#include "course.h"
#define WEEK_COLUMN 6

bool npcourse::now(const char* course_id, const char* course_cs, std::tm day) {
	std::string path = COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv";
	if (!exists(path.c_str())) return 0;
	csv_file schedule(path.c_str());
	for (int i = 0; i < schedule.count; ++i) {
		if (control::now(day, schedule.data[i].pdata[1]) == 0) return 1;
	}
	return 0;
}

bool npcourse::info(const char* course_id, const char* course_cs, int x, int y, int n) {
	csv_file course_list(COURSE_PATH("__course.csv").c_str());
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
	if (n > 2) { gotoxy(x, y + 4); std::cout << "Class       : " << course->pdata[3]; }
	if (n > 3) { gotoxy(x, y + 6); std::cout << "Lecturer ID : " << course->pdata[4]; }
	if (n > 4) { gotoxy(x, y + 8); std::cout << "Room        : " << course->pdata[10]; }

	return 1;
}

csv_line* npcourse::choose(const csv_file& my_course, int& choose) {
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "    My courses    ";

	csv_line* course = nullptr; int overflow = 0;
	while (1) {	// Choose Up-down: [COURSE]
		int cur = -1;
		for (int i = 0; i < my_course.count; ++i) {
			if (strcmp(my_course.data[i].pdata[0], ACADEMICYEAR.c_str())) continue;
			if (strcmp(my_course.data[i].pdata[1], SEMESTER.c_str())) continue;
			int y = 9 + (++cur) + overflow;
			if (y < 9 || y > 27) continue;
			if (choose == cur) course = &my_course.data[i];

			gotoxy(2, y, (choose == cur ? COLOR_WHITE_BACKGROUND : COLOR_WHITE)); std::cout << "                  ";
			gotoxy(3, y, (choose == cur ? COLOR_WHITE_BACKGROUND : COLOR_WHITE)); std::cout << my_course.data[i].pdata[2] << "-" << my_course.data[i].pdata[3];
		}
		gotoxy(2, min(28, 10 + cur), 224); std::cout << "  <-- Main menu   ";

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return nullptr;
		if (c == KEY_ENTER) return course;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose - overflow > 18) overflow--; }
			else if (c == KEY_LEFT) return nullptr;
			else goto UN_CHANGE;
		}
	}
}

void npcourse::check_in(csv_line& user) {
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("course.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout course_layout(inp);
	layout minibox_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	csv_file my_course(((std::string)".\\data\\student\\" + username + ".csv").c_str());
	csv_file course_list((COURSE_PATH("__course.csv").c_str()));
	csv_line* course = nullptr;
	int chs1 = 0;

	course_layout.print();
	academicmark();

COURSE:
	if ((course = npcourse::choose(my_course, chs1)) == nullptr) return;
	minibox_layout.print();
	if (npcourse::info(course->pdata[2], course->pdata[3], 33, 9) == 0) goto COURSE;

	// Load SCHEDULE: COURSE_CLASS.csv or make __dafault
	std::string schpath = COURSE_PATH("schedule\\") + course->pdata[2] + "_" + course->pdata[3] + ".csv";
	std::string propath = COURSE_PATH("process\\") + course->pdata[2] + "_" + course->pdata[3] + ".csv";
	if (!exists(schpath.c_str()) || !exists(propath.c_str())) {
		// If .\\SCHEDULE\\COURSEID_COURSECLASS.csv does not exist
		goto COURSE;
	}
	csv_file process(propath.c_str());
	csv_file schedule(schpath.c_str());

	csv_line* mycou = nullptr;
	if ((mycou = npcsv::exists(process, user.pdata[1], 0)) == nullptr) goto COURSE;

	for (int i = 0; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		if (strcmp(date->pdata[1], "0") == 0) continue;
		
		int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
		if (now == 0) {
			gotoxy(33, 17); std::cout << "Date        : " << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";

			if (strcmp(mycou->pdata[WEEK_COLUMN + i], "1") != 0) {
				int chs = 0;
				while (1) {	// Choose Left-right: [Check in][ Cancel ]
					gotoxy(50, 27, (chs == 0) ? COLOR_GREEN_BACKGROUND : COLOR_WHITE); std::cout << "[Check in]";
					gotoxy(61, 27, (chs == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[ Cancel ]";
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
					goto COURSE;
				}

				mycou->pdata[WEEK_COLUMN + i][0] = '1';
				npcsv::update(propath.c_str(), mycou->id, WEEK_COLUMN + i, "1");
			}

			gotoxy(40, 27, COLOR_GREEN); std::cout << "You have already checked in this course.";
			goto COURSE;
		}
		else if (now == -1) {
			// The nearest day the course will start
			gotoxy(33, 17); std::cout << "Date        : " << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";
			gotoxy(46, 27, COLOR_WHITE); std::cout << "This course has not started.";
			goto COURSE;
		}
	}
	// The last day of the course has ended
	gotoxy(49, 27, COLOR_WHITE); std::cout << "The course has ended.";
	goto COURSE;
}

void npcourse::check_in_result(csv_line& user) {
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("course.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout course_layout(inp);
	layout minibox_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	csv_file my_course(((std::string)".\\data\\student\\" + username + ".csv").c_str());
	csv_file course_list((COURSE_PATH("__course.csv").c_str()));
	csv_line* course = nullptr;
	int chs1 = 0;

	course_layout.print();
	academicmark();
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "     Student      ";
	gotoxy(2, 9, 224); std::cout << "    My courses    ";

COURSE:
	if ((course = npcourse::choose(my_course, chs1)) == nullptr) return;
	minibox_layout.print();
	if (npcourse::info(course->pdata[2], course->pdata[3], 33, 9, 1) == 0) goto COURSE;

	gotoxy(29, 27, COLOR_YELLOW_BACKGROUND); std::cout << " UNREGISTER ";
	gotoxy(46, 27, COLOR_GREEN_BACKGROUND);	 std::cout << " CHECKED IN ";
	gotoxy(63, 27, COLOR_RED_BACKGROUND);    std::cout << "   MISSED   ";
	gotoxy(80, 27, COLOR_WHITE_BACKGROUND);  std::cout << "  UPCOMING  ";

	// Load SCHEDULE: COURSE_CLASS.csv or make __dafault
	std::string schpath = COURSE_PATH("schedule\\") + course->pdata[2] + "_" + course->pdata[3] + ".csv";
	std::string propath = COURSE_PATH("process\\") + course->pdata[2] + "_" + course->pdata[3] + ".csv";
	if (!exists(schpath.c_str()) || !exists(propath.c_str())) {
		// If .\\SCHEDULE\\COURSEID_COURSECLASS.csv does not exist
		goto COURSE;
	}
	csv_file process(propath.c_str());
	csv_file schedule(schpath.c_str());

	csv_line* mycou = nullptr;
	if ((mycou = npcsv::exists(process, user.pdata[1], 0)) == nullptr) goto COURSE;

	for (int i = 0, x = 12, y = 12; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		if ((x += 11) > 90) x = 23, y += 2;

		WORD COLOR_CODE = COLOR_WHITE_BACKGROUND;

		if (strcmp(date->pdata[1], "0") != 0) {
			int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
			if (strcmp(mycou->pdata[WEEK_COLUMN + i], "1") == 0) COLOR_CODE = COLOR_GREEN_BACKGROUND;
			else if (now == 0) COLOR_CODE = COLOR_YELLOW_BACKGROUND;
			else if (now == 1) COLOR_CODE = COLOR_RED_BACKGROUND;
		}
		gotoxy(x, y, COLOR_CODE); std::cout << " " << date->pdata[0] << " ";
	}
	goto COURSE;
}