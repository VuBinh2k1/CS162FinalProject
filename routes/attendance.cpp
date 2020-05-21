#include "..\\controls\\attendance.h"

int npattendance::staff(const char* course_id, const char* course_cs) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    " << "  Student list   ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Attendance     ";
	colorizing(COLOR_YELLOW); std::cout << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Course   | Class    | Student ID |01|02|03|04|05|06|07|08|09|10|11";

	int choose = 0, cur = -1;
	while ((cur = -1)) {
		std::string propath = COURSE_PATH("process\\") + course_id + "_" + course_cs + ".csv";
		std::string schpath = COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv";
		csv_file process(propath.c_str(), def_process);
		csv_file schedule(schpath.c_str(), def_schedule, course_id, course_cs);
		for (int i = 0; i < process.count; ++i) {
			csv_line* student = &process.data[i];
			if (file::find(__STUDENT, student->pdata[1], nullptr, ON) == -1) continue;

			int y = 10 + (++cur) - choose;
			if (y < 10 || y > 27) continue;
			WORD COLOR_CODE = (cur % 2 ? 112 : 240);

			gotoxy(27, y, COLOR_CODE); std::cout << "          |          |            |  |  |  |  |  |  |  |  |  |  |  ";
			gotoxy(28, y, COLOR_CODE); std::cout << course_id;
			gotoxy(39, y, COLOR_CODE); std::cout << course_cs;
			gotoxy(50, y, COLOR_CODE); std::cout << student->pdata[1];

			for (int j = 0, x = 59; x += 3, j < min(MAX_WEEK, schedule.count); ++j) {
				csv_line* date = &schedule.data[j];

				WORD color = COLOR_CODE;
				if (strcmp(date->pdata[1], "0")) {
					int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
					if (student->pdata[WEEK_COLUMN + j][0] == '1') color = COLOR_CODE + COLOR_GREEN;
					else if (now > 0) color = COLOR_CODE + COLOR_RED;
				}
				gotoxy(x, y, color);
				if (color == COLOR_CODE) std::cout << "__";
				if (color == COLOR_CODE + COLOR_GREEN) std::cout << "CK";
				if (color == COLOR_CODE + COLOR_RED) std::cout << "MS";
			}
		}
	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " Save as    Ctrl+S  ";
			gotoxy(78, 9, 128); std::cout << "                    ";
			getch();
			gotoxy(78, 8); std::cout << "                    ";
			gotoxy(78, 9); std::cout << "                    ";
			goto LAYOUT;
		}
		if (c == KEY_SAVE) {
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Save as                                                 ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";

		SAVE_AS:
			gotoxy(32, 16, 128); std::cout << " Path:                                                   ";
			std::string path;
			if (read(39, 16, 128, path, 48, SHOW) == KEY_ESC) {
				// Clear export file box
				gotoxy(32, 15); std::cout << "                                                         ";
				gotoxy(32, 16); std::cout << "                                                         ";
				gotoxy(32, 17); std::cout << "                                                         ";
				continue;
			}
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			if (path.empty()) goto SAVE_AS;
			if (path.back() != '\\') path += '\\'; path += ACADEMICYEAR + '-' + SEMESTER + '-' + course_id + '_' + course_cs + "_attendance.csv";

			std::ofstream out(path);
			if (!out.is_open()) {
				gotoxy(33, 17, 132); std::cout << "Can't open file!";
				PAUSE; goto SAVE_AS;
			}

			out << "No,Student ID,Week 01,Week 02,Week 03,Week 04,Week 05,Week 06,Week 07,Week 08,Week 09,Week 10,Week 11\n";

			int cnt = -1;
			std::string propath = COURSE_PATH("process\\") + course_id + "_" + course_cs + ".csv";
			std::string schpath = COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv";
			csv_file process(propath.c_str(), def_process);
			csv_file schedule(schpath.c_str(), def_schedule, course_id, course_cs);
			for (int i = 0; i < process.count; ++i) {
				csv_line* student = &process.data[i];
				if (file::find(__STUDENT, student->pdata[1], nullptr, ON) == -1) continue;

				out << ++cnt << ',' << student->pdata[1] << ',';
				for (int j = 0; j < min(MAX_WEEK, schedule.count); ++j) {
					csv_line* date = &schedule.data[j];

					int status = 2;
					if (strcmp(date->pdata[1], "0")) {
						int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
						if (student->pdata[WEEK_COLUMN + j][0] == '1') status = 1;
						else if (now > 0) status = 0;
					}
					if (status == 2) out << "_";
					if (status == 1) out << "Check";
					if (status == 0) out << "Miss";
					if (j < min(MAX_WEEK, schedule.count) - 1) out << ',';
				}
				out << '\n';
			}

			out.close();
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			gotoxy(33, 17, 129); std::cout << "Export to ";
			colorizing(134); std::cout << ACADEMICYEAR << '-' << SEMESTER << '-' << course_id << '_' << course_cs << "_attendance.csv";
			colorizing(129); std::cout << " success.";
			PAUSE;
			// Clear export file box
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			continue;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < cur - 17) choose++;
			else if (c == KEY_LEFT) return 1;
			else if (c == KEY_RIGHT) {
				if (npscoreboard::staff(course_id, course_cs) == 0) return 0;
				goto LAYOUT;
			}
			else goto UN_CHANGE;
			continue;
		}
		goto UN_CHANGE;
	}
	return 0;
}

int npattendance::lecturer(const char* course_id, const char* course_cs) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    " << "  Student list   ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Attendance     ";
	colorizing(COLOR_YELLOW); std::cout << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Course   | Class    | Student ID |01|02|03|04|05|06|07|08|09|10|11";

	int choose = 0, cur = -1, overflow = 0, editrow = 0;
	while ((cur = -1)) {
		std::string propath = COURSE_PATH("process\\") + course_id + "_" + course_cs + ".csv";
		std::string schpath = COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv";
		csv_file process(propath.c_str(), def_process);
		csv_file schedule(schpath.c_str(), def_schedule, course_id, course_cs);
		for (int i = 0; i < process.count; ++i) {
			csv_line* student = &process.data[i];
			if (file::find(__STUDENT, student->pdata[1], nullptr, ON) == -1) continue;

			int y = 10 + (++cur) + overflow;
			if (y < 10 || y > 27) continue;
			WORD COLOR_CODE = (cur % 2 ? 112 : 240);
			if (choose == cur) COLOR_CODE = 176, editrow = i;

			gotoxy(27, y, COLOR_CODE); std::cout << "          |          |            |  |  |  |  |  |  |  |  |  |  |  ";
			gotoxy(28, y, COLOR_CODE); std::cout << course_id;
			gotoxy(39, y, COLOR_CODE); std::cout << course_cs;
			gotoxy(50, y, COLOR_CODE); std::cout << student->pdata[1];

			for (int j = 0, x = 59; x += 3, j < min(MAX_WEEK, schedule.count); ++j) {
				csv_line* date = &schedule.data[j];

				WORD color = COLOR_CODE;
				if (strcmp(date->pdata[1], "0")) {
					int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
					if (student->pdata[WEEK_COLUMN + j][0] == '1') color = COLOR_CODE + COLOR_GREEN;
					else if (now > 0) color = COLOR_CODE + COLOR_RED;
				}
				gotoxy(x, y, color);
				if (color == COLOR_CODE) std::cout << "__";
				if (color == COLOR_CODE + COLOR_GREEN) std::cout << "CK";
				if (color == COLOR_CODE + COLOR_RED) std::cout << "MS";
			}
		}

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " Edit       E, e    ";
			gotoxy(78, 9, 128); std::cout << "                    ";
			getch();
			gotoxy(78, 8); std::cout << "                    ";
			gotoxy(78, 9); std::cout << "                    ";
			goto LAYOUT;
		}
		if (KEY_EDIT(c)) {
			csv_file process((COURSE_PATH("process\\") + course_id + "_" + course_cs + ".csv").c_str());
			csv_file schedule((COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv").c_str());
			csv_line* student = &process.data[editrow];
			int y = 10 + choose + overflow;

			for (int i = 0, x = 59; x += 3, i < min(MAX_WEEK, schedule.count); ++i) {
				csv_line* date = &schedule.data[i];

				int status = 2;
				if (strcmp(date->pdata[1], "0")) {
					int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
					if (student->pdata[WEEK_COLUMN + i][0] == '1') status = 1;
					else if (now > 0) status = 0;
				}
				if (status == 2) continue;
				while (1) {
					if (status) { gotoxy(x, y, COLOR_GREEN_BACKGROUND); std::cout << "CK"; }
					else { gotoxy(x, y, COLOR_RED_BACKGROUND); std::cout << "MS"; }

					uint8_t c = getch();
					if (c == KEY_ESC) { status = -1; break; }
					if (c == KEY_ENTER) {
						if (status) { gotoxy(x, y, 176 + COLOR_GREEN); std::cout << "CK"; }
						else { gotoxy(x, y, 176 + COLOR_RED); std::cout << "MS"; }
						file::update((COURSE_PATH("process\\") + course_id + "_" + course_cs + ".csv").c_str(), student->id, WEEK_COLUMN + i, (status ? "1" : "0"));
						break;
					}
					if (c == 224 || c == 0) {
						c = getch();
						if (c == KEY_LEFT || c == KEY_RIGHT) {
							status = !status;
						}
					}
				}
				if (status == -1) break;
			}
			continue;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_LEFT) return 1;
			else if (c == KEY_RIGHT) {
				if (npscoreboard::lecturer(course_id, course_cs) == 0) return 0;
				goto LAYOUT;
			}
			else goto UN_CHANGE;
			continue;
		}
		goto UN_CHANGE;
	}
	return 0;
}

int npattendance::student(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	minibox_layout.print();
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    " << "  Calendar       ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Attendance     ";
	colorizing(COLOR_YELLOW); std::cout << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Semester    | Course ID       |01|02|03|04|05|06|07|08|09|10|11|  ";
	gotoxy(29, 28, COLOR_YELLOW_BACKGROUND); std::cout << " UNREGISTER ";
	gotoxy(46, 28, COLOR_GREEN_BACKGROUND);	 std::cout << " CHECKED IN ";
	gotoxy(63, 28, COLOR_RED_BACKGROUND);    std::cout << "   MISSED   ";
	gotoxy(80, 28, COLOR_WHITE_BACKGROUND);  std::cout << "  UPCOMING  ";

	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);
	csv_file course_list(__COURSE, def_course);

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
			if ((mycou = file::find(process, user.pdata[1], nullptr, OFF)) == nullptr) continue;

			for (int j = 0, x = 56; x += 3, j < min(MAX_WEEK, schedule.count); ++j) {
				csv_line* date = &schedule.data[j];

				WORD color = COLOR_CODE;
				if (strcmp(date->pdata[1], "0")) {
					int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
					if (mycou->pdata[WEEK_COLUMN + j][0] == '1') color = COLOR_CODE + COLOR_GREEN;
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
			else if (c == KEY_LEFT) return -1;
			else if (c == KEY_RIGHT) return 1;
			else goto UN_CHANGE;
		}
	}
	return 0;
}