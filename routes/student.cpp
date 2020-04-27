#include "student.h"

void npstudent::menu(csv_line& user) {
	std::ifstream inp(".\\layout\\menu.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("menu.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	layout menu_info_layout(inp);
	inp.close();

	csv_file infofile(".\\data\\student\\__student.csv");
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0;

MENU:
	colorizing(COLOR_DEFAULT); system("cls");
	menu_layout.print();
	if (infouser == nullptr) {
		gotoxy(33, 9, COLOR_RED); std::cout << "Sorry, I don't have your information.";
	}
	else {
		gotoxy(33, 9, COLOR_YELLOW); std::cout << "Welcome ";
		colorizing(COLOR_GREEN);
		std::cout << "St.";
		if (ENGLISHNAME) {
			std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		}
		else {
			std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];
		}
		colorizing(COLOR_YELLOW); std::cout << " to CS162FinalProject";
	}

	gotoxy(33, 11); std::cout << "This program is for managing student info, course info,";
	gotoxy(33, 12); std::cout << "study process, etc.";
	gotoxy(33, 15, COLOR_YELLOW); std::cout << "Development team:";
	gotoxy(33, 17); std::cout << "[19127186]: Le Thanh Khoi (Leader)";
	gotoxy(33, 18); std::cout << "[19127012]: Vu Nguyen Thai Binh";
	gotoxy(33, 19); std::cout << "[19127226]: Hoang Van Nguyen";
	gotoxy(33, 20); std::cout << "[19127348]: Bui Cong Danh";

	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "     Student      ";
	while (1) {
		int E = 5;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My course       ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Checkin result  ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My schedule     ";
		gotoxy(2,13, (choose == 4) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My scoreboard   ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) if (choose != E) choose = E; else goto NO_CHANGE;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				role::profile(user);
				goto MENU;
			}
			if (choose == 1) {
				npstudent::check_in(user);
				goto MENU;
			}
			if (choose == 2) {
				npstudent::check_in_result(user);
				goto MENU;
			}
			if (choose == 3) {

				goto MENU;
			}
			if (choose == 4) {

				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < 5) choose++;
			else goto NO_CHANGE;
		}
	}
}

void npstudent::check_in(csv_line& user) {
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
	if (npcourse::list(my_course, course_id, course_cs, chs1, id) == 0) return;

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
			gotoxy(46, 27, COLOR_WHITE); std::cout <<  "This course has not started.";
			goto LABLE_COURSE;
		}
	}
	// The last day of the course has ended
	gotoxy(49, 27, COLOR_WHITE); std::cout << "The course has ended.";
	goto LABLE_COURSE;
}

void npstudent::check_in_result(csv_line& user) {
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
	if (npcourse::list(my_course, course_id, course_cs, chs1, id) == 0) return;

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
