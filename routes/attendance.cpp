#include "..\\controls\\attendance.h"

void npattendance::staff() {
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("course.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout course_layout(inp);
	layout minibox_layout(inp);
	inp.close();

	csv_file course_list((COURSE_PATH("__course.csv").c_str()), def_course);
	csv_line* course = nullptr;
	int chscrs = 0;

	course_layout.print();
	academicmark();

	while ((course = npcourse::choose(course_list, chscrs, 1)) != nullptr) {
		minibox_layout.print();
		gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Student ID   | Class     |01|02|03|04|05|06|07|08|09|10|11";
		gotoxy(33, 28); std::cout << "[E]: Export to file";

		csv_line* mycou = nullptr;
		int choose = 0, cur = -1;

	MENU_CHOOSEN:
		mycou = course; cur = -1;
		while (mycou != nullptr) {
			char* courseid = mycou->pdata[1];
			char* coursecs = mycou->pdata[3];
			if (npcourse::info(courseid, coursecs, 33, 8, 1)) {
				std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
				std::string schpath = COURSE_PATH("schedule\\") + courseid + "_" + coursecs + ".csv";
				csv_file process(propath.c_str(), def_process);
				csv_file schedule(schpath.c_str(), def_schedule);
				for (int i = 0; i < process.count; ++i) {
					int y = 10 + (++cur) - choose;
					if (y < 10 || y > 27) continue;
					csv_line* student = &process.data[i];
					WORD COLOR_CODE = (cur % 2 ? 112 : 240);
					gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |  |  |  |  |  |  |  |  |  |  |  ";
					gotoxy(28, y, COLOR_CODE); std::cout << cur;
					gotoxy(36, y, COLOR_CODE); std::cout << student->pdata[1];
					gotoxy(51, y, COLOR_CODE); std::cout << coursecs;

					for (int j = 0, x = 59; x += 3, j < min(MAX_WEEK, schedule.count); ++j) {
						csv_line* date = &schedule.data[j];

						WORD color = COLOR_CODE;
						if (strcmp(date->pdata[1], "0") != 0) {
							int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
							if (strcmp(student->pdata[WEEK_COLUMN + j], "1") == 0) color = COLOR_CODE + COLOR_GREEN;
							else if (now > 0) color = COLOR_CODE + COLOR_RED;
						}
						gotoxy(x, y, color);
						if (color == COLOR_CODE) std::cout << "__";
						if (color == COLOR_CODE + COLOR_GREEN) std::cout << "CK";
						if (color == COLOR_CODE + COLOR_RED) std::cout << "MS";
					}
				}
			}
			mycou = npcourse::find(mycou + 1, course_list.data + course_list.count, courseid, 1);
		}
	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) continue;
		if (c == 'E' || c == 'e') {
		SAVE_AS:
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Save as                                                 ";
			gotoxy(32, 16, 128); std::cout << " Path:                                                   ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			std::string path;
			if (read(39, 16, 128, path, 48, SHOW) == KEY_ESC) {
				// Clear export file box
				gotoxy(32, 15); std::cout << "                                                         ";
				gotoxy(32, 16); std::cout << "                                                         ";
				gotoxy(32, 17); std::cout << "                                                         ";
				goto MENU_CHOOSEN;
			}
			if (path.empty()) goto SAVE_AS;
			if (path.back() != '\\') path += '\\'; path += ACADEMICYEAR + '-' + SEMESTER + '-' + course->pdata[1] + "_attendance.csv";

			std::ofstream out(path);
			if (!out.is_open()) {
				gotoxy(33, 17, 132); std::cout << "Can't open file!";
				PAUSE; goto SAVE_AS;
			}

			out << "No,Student ID,Class,Week 01,Week 02,Week 03,Week 04,Week 05,Week 06,Week 07,Week 08,Week 09,Week 10,Week 11\n";
			mycou = course; int cnt = -1;
			while (mycou != nullptr) {
				char* courseid = mycou->pdata[1];
				char* coursecs = mycou->pdata[3];
				if (npcourse::info(courseid, coursecs, 33, 8, 0)) {
					std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
					std::string schpath = COURSE_PATH("schedule\\") + courseid + "_" + coursecs + ".csv";
					csv_file process(propath.c_str(), def_process);
					csv_file schedule(schpath.c_str(), def_schedule);
					for (int i = 0; i < process.count; ++i) {
						csv_line* student = &process.data[i];
						out << ++cnt << ',' << student->pdata[1] << ',' << coursecs << ',';
						for (int j = 0; j < min(MAX_WEEK, schedule.count); ++j) {
							csv_line* date = &schedule.data[j];

							int status = 2;
							if (strcmp(date->pdata[1], "0") != 0) {
								int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
								if (strcmp(student->pdata[WEEK_COLUMN + j], "1") == 0) status = 1;
								else if (now > 0) status = 0;
							}
							if (status == 2) out << "_";
							if (status == 1) out << "Check";
							if (status == 0) out << "Miss";
							if (j < min(MAX_WEEK, schedule.count) - 1) out << ',';
						}
						out << '\n';
					}
				}
				mycou = npcourse::find(mycou + 1, course_list.data + course_list.count, courseid, 1);
			}
			out.close();
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			gotoxy(33, 17, 129); std::cout << "Export to ";
			colorizing(134); std::cout << ACADEMICYEAR << '-' << SEMESTER << '-' << course->pdata[1] << "_attendance.csv";
			colorizing(129); std::cout << " success.";
			PAUSE;
			// Clear export file box
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < cur - 17) choose++;
			else if (c == KEY_LEFT) continue;
			else goto UN_CHANGE;
		}
		goto MENU_CHOOSEN;
	}
}

void npattendance::lecturer(csv_line& user) {
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("course.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout course_layout(inp);
	layout minibox_layout(inp);
	inp.close();

	csv_file my_course(((std::string)".\\data\\lecturer\\" + user.pdata[1] + ".csv").c_str(), def_user);
	csv_file course_list((COURSE_PATH("__course.csv").c_str()), def_course);
	csv_line* course = nullptr;
	int chscrs = 0;

	course_layout.print();
	academicmark();

	while ((course = npcourse::choose(my_course, chscrs)) != nullptr) {
		minibox_layout.print();
		gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Student ID   | Class     |01|02|03|04|05|06|07|08|09|10|11";
		gotoxy(33, 28); std::cout << "[E]: Edit attendance";

		csv_line* mycou = nullptr;
		int choose = 0, cur = -1, overflow = 0;
		std::string editpath;
		int editrow = -1;

	MENU_CHOOSEN:
		mycou = course; cur = -1;
		while (mycou != nullptr) {
			char* courseid = mycou->pdata[2];
			char* coursecs = mycou->pdata[3];
			if (npcourse::info(courseid, coursecs, 33, 8, 1)) {
				std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
				std::string schpath = COURSE_PATH("schedule\\") + courseid + "_" + coursecs + ".csv";
				csv_file process(propath.c_str(), def_process);
				csv_file schedule(schpath.c_str(), def_schedule);
				for (int i = 0; i < process.count; ++i) {
					int y = 10 + (++cur) + overflow;
					if (y < 10 || y > 27) continue;
					csv_line* student = &process.data[i];
					WORD COLOR_CODE = (cur % 2 ? 112 : 240);
					if (choose == cur) COLOR_CODE = 176, editrow = i, editpath = (std::string)courseid + "_" + coursecs + ".csv";

					gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |  |  |  |  |  |  |  |  |  |  |  ";
					gotoxy(28, y, COLOR_CODE); std::cout << cur;
					gotoxy(36, y, COLOR_CODE); std::cout << student->pdata[1];
					gotoxy(51, y, COLOR_CODE); std::cout << coursecs;

					for (int j = 0, x = 59; x += 3, j < min(MAX_WEEK, schedule.count); ++j) {
						csv_line* date = &schedule.data[j];

						WORD color = COLOR_CODE;
						if (strcmp(date->pdata[1], "0") != 0) {
							int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
							if (strcmp(student->pdata[WEEK_COLUMN + j], "1") == 0) color = COLOR_CODE + COLOR_GREEN;
							else if (now > 0) color = COLOR_CODE + COLOR_RED;
						}
						gotoxy(x, y, color);
						if (color == COLOR_CODE) std::cout << "__";
						if (color == COLOR_CODE + COLOR_GREEN) std::cout << "CK";
						if (color == COLOR_CODE + COLOR_RED) std::cout << "MS";
					}
				}
			}
			mycou = npcourse::find(mycou + 1, my_course.data + my_course.count, courseid);
		}

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) continue;
		if (c == 'E' || c == 'e') {
			csv_file process((COURSE_PATH("process\\") + editpath).c_str());
			csv_file schedule((COURSE_PATH("schedule\\") + editpath).c_str());
			csv_line* student = &process.data[editrow];
			int y = 10 + choose + overflow;

			for (int i = 0, x = 59; x += 3, i < min(MAX_WEEK, schedule.count); ++i) {
				csv_line* date = &schedule.data[i];

				int status = 2;
				if (strcmp(date->pdata[1], "0") != 0) {
					int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
					if (strcmp(student->pdata[WEEK_COLUMN + i], "1") == 0) status = 1;
					else if (now > 0) status = 0;
				}
				if (status == 2) continue;
				while (1) {
					if (status) { gotoxy(x, y, COLOR_GREEN_BACKGROUND); std::cout << "CK"; }
					else { gotoxy(x, y, COLOR_RED_BACKGROUND); std::cout << "MS"; }

					uint8_t c = getch();
					if (c == KEY_ESC) goto MENU_CHOOSEN;
					if (c == KEY_ENTER) {
						if (status) { gotoxy(x, y, 176 + COLOR_GREEN); std::cout << "CK"; }
						else { gotoxy(x, y, 176 + COLOR_RED); std::cout << "MS"; }
						file::update((COURSE_PATH("process\\") + editpath).c_str(), student->id, WEEK_COLUMN + i, (status ? "1" : "0"));
						break;
					}
					if (c == 224 || c == 0) {
						c = getch();
						if (c == KEY_LEFT || c == KEY_RIGHT) {
							status = !status;
						}
					}
				}
			}
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_LEFT) continue;
			else goto UN_CHANGE;
		}
		goto MENU_CHOOSEN;
	}
}

void npattendance::student(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	minibox_layout.print();
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                           My Attendance                           ";
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Semester    | Course ID       |01|02|03|04|05|06|07|08|09|10|11|  ";
	gotoxy(29, 28, COLOR_YELLOW_BACKGROUND); std::cout << " UNREGISTER ";
	gotoxy(46, 28, COLOR_GREEN_BACKGROUND);	 std::cout << " CHECKED IN ";
	gotoxy(63, 28, COLOR_RED_BACKGROUND);    std::cout << "   MISSED   ";
	gotoxy(80, 28, COLOR_WHITE_BACKGROUND);  std::cout << "  UPCOMING  ";

	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);
	csv_file course_list((COURSE_PATH("__course.csv").c_str()), def_course);

	int choose = 0;
	while (1) {
		for (int i = choose; i < min(choose + 17, my_course.count); ++i) {
			int y = 10 + i - choose;
			csv_line* course = &my_course.data[i];
			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			gotoxy(27, y, COLOR_CODE); std::cout << "             |                 |  |  |  |  |  |  |  |  |  |  |  |  ";
			gotoxy(27, y, COLOR_CODE); std::cout << course->pdata[0] << " - ";
			if (course->pdata[1][0] == '1') std::cout << "Spring";
			if (course->pdata[1][0] == '2') std::cout << "Summer";
			if (course->pdata[1][0] == '3') std::cout << "Autumn";
			if (course->pdata[1][0] == '4') std::cout << "Winter";
			gotoxy(42, y, COLOR_CODE); std::cout << course->pdata[2];

			std::string propath = (std::string)".\\data\\course\\" + course->pdata[0] + "-" + course->pdata[1] + "\\process\\" + course->pdata[2] + "_" + course->pdata[3] + ".csv";
			std::string schpath = (std::string)".\\data\\course\\" + course->pdata[0] + "-" + course->pdata[1] + "\\schedule\\" + course->pdata[2] + "_" + course->pdata[3] + ".csv";
			csv_file process(propath.c_str(), def_process);
			csv_file schedule(schpath.c_str(), def_schedule);

			csv_line* mycou = nullptr;
			if ((mycou = file::exists(process, user.pdata[1], 0)) == nullptr) continue;

			for (int j = 0, x = 56; x += 3, j < min(MAX_WEEK, schedule.count); ++j) {
				csv_line* date = &schedule.data[j];

				WORD color = COLOR_CODE;
				if (strcmp(date->pdata[1], "0") != 0) {
					int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
					if (strcmp(mycou->pdata[WEEK_COLUMN + j], "1") == 0) color = COLOR_CODE + COLOR_GREEN;
					else if (now == 0) color = COLOR_CODE + COLOR_YELLOW;
					else if (now == 1) color = COLOR_CODE + COLOR_RED;
				}
				gotoxy(x, y, color);
				if (color == COLOR_CODE) std::cout << "__";
				if (color == COLOR_CODE + COLOR_YELLOW) std::cout << "UR";
				if (color == COLOR_CODE + COLOR_GREEN) std::cout << "CK";
				if (color == COLOR_CODE + COLOR_RED) std::cout << "MS";
			}
		}
	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < my_course.count - 17) choose++;
			else if (c == KEY_LEFT) break;
			else goto UN_CHANGE;
		}
	}
}
