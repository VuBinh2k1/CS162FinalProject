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
	academicmark();
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
				npcourse::check_in(user);
				goto MENU;
			}
			if (choose == 2) {
				npcourse::check_in_result(user);
				goto MENU;
			}
			if (choose == 3) {
				gotoxy(2, 12, 8); std::cout << "  My schedule     ";
				npstudent::calendar(user);
				goto MENU;
			}
			if (choose == 4) {
				gotoxy(2, 13, 8); std::cout << "  My scoreboard   ";
				npscoreboard::student(user);
				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < E) choose++;
			else goto NO_CHANGE;
		}
	}
}

void npstudent::info() {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	csv_line* student = nullptr;
	std::string username;

ENTER_STUDENT_ID:
	while (1) {
		minibox_layout.print();
		gotoxy(51, 9, COLOR_YELLOW);  std::cout << "Student information";
		gotoxy(33, 11); std::cout << "Student ID  : ";
		gotoxy(33, 25); std::cout << "~ Could not find this student id.";
		gotoxy(33, 26); std::cout << "~ Go to ";
		colorizing(COLOR_WHITE_BACKGROUND); std::cout << " New student ";
		colorizing(COLOR_DEFAULT); std::cout << " to add new student.";
		if (read(47, 11, COLOR_DEFAULT, username, 8, SHOW) == KEY_ESC) return;

		csv_file infofile(".\\data\\student\\__student.csv");
		if ((student = file::exists(infofile, username.c_str())) == nullptr) {
			gotoxy(47, 11, COLOR_RED); std::cout << "this student is does not exist.";
			PAUSE; continue;
		}
		break;
	}

	while (1) {
		csv_file infofile(".\\data\\student\\__student.csv");
		student = file::exists(infofile, username.c_str());

		minibox_layout.print();
		gotoxy(51, 9, COLOR_YELLOW);  std::cout << "Student information";
		gotoxy(33, 11); std::cout << "Student name: ";
		if (ENGLISHNAME) {
			std::cout << student->pdata[3] << ' ' << student->pdata[2];
		}
		else {
			std::cout << student->pdata[2] << ' ' << student->pdata[3];
		}
		gotoxy(33, 14); std::cout << "Student ID  : " << student->pdata[1];
		gotoxy(33, 16); std::cout << "Class       : " << student->pdata[6];
		gotoxy(33, 18); std::cout << "Day of birth: " << student->pdata[5];
		gotoxy(33, 20); std::cout << "Gender      : " << student->pdata[4]; 
		
		// Choose Left-right: [ Edit ][Remove]
		for (int choose = 0;;) {
			gotoxy(51, 27, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[ Edit ]";
			gotoxy(60, 27, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Remove]";

			uint8_t c = getch();
			if (c == KEY_ESC) return;
			if (c == KEY_ENTER) {
				if (choose == 0) {
					npstudent::edit(*student);
					break;
				}
				if (choose == 1) {
					if (npstudent::remove(*student) == 1) goto ENTER_STUDENT_ID;
					break;
				}
			}
			if (c == 224 || c == 0) {
				c = getch();
				if (c == KEY_LEFT && choose == 1) choose--;
				else if (c == KEY_RIGHT && choose == 0) choose++;
				else if (c == KEY_LEFT) goto ENTER_STUDENT_ID;
			}
		}
	}
}

void npstudent::edit(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	const char* FILE = ".\\data\\student\\__student.csv";

	std::string fname, lname, newcs, birth, gende;
	while (1) {
		minibox_layout.print();
		gotoxy(51, 9, COLOR_YELLOW);  std::cout << "Student information";
		gotoxy(33, 11); std::cout << "First name  : "; colorizing(8); std::cout << user.pdata[3];
		gotoxy(33, 12); std::cout << "Last  name  : "; colorizing(8); std::cout << user.pdata[2];
		gotoxy(33, 14); std::cout << "Student ID  : " << user.pdata[1];
		gotoxy(33, 16); std::cout << "Class       : "; colorizing(8); std::cout << user.pdata[6];
		gotoxy(33, 18); std::cout << "Day of birth: "; colorizing(8); std::cout << user.pdata[5];
		gotoxy(33, 20); std::cout << "Gender      : "; colorizing(8); std::cout << user.pdata[4];
		gotoxy(46, 27); std::cout << "[Save change] [  Cancel   ]";

		if (read(47, 11, COLOR_DEFAULT, fname, 20, SHOW, user.pdata[3]) == KEY_ESC) return;
		if (read(47, 12, COLOR_DEFAULT, lname, 20, SHOW, user.pdata[2]) == KEY_ESC) return;
		if (read(47, 16, COLOR_DEFAULT, newcs, 20, SHOW, user.pdata[6]) == KEY_ESC) return;
		if (read(47, 18, COLOR_DEFAULT, birth, 20, SHOW, user.pdata[5]) == KEY_ESC) return;
		if (read(47, 20, COLOR_DEFAULT, gende, 20, SHOW, user.pdata[4]) == KEY_ESC) return;

		// Choose Left-right: [ Edit ][Remove]
		for (int choose = 0;;) {
			gotoxy(46, 27, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Save change]";
			gotoxy(60, 27, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[  Cancel   ]";

			uint8_t c = getch();
			if (c == KEY_ESC) return;
			if (c == KEY_ENTER) {
				if (choose == 0) {
					if (newcs.size()) {
						std::transform(newcs.begin(), newcs.end(), newcs.begin(), ::toupper);
						if (npclass::change(user, user.pdata[6], newcs.c_str()) == 0) return;
						file::update(FILE, user.id, 6, newcs.c_str());
					}
					if (fname.size()) file::update(FILE, user.id, 3, fname.c_str());
					if (lname.size()) file::update(FILE, user.id, 2, lname.c_str());
					if (birth.size()) file::update(FILE, user.id, 5, birth.c_str());
					if (gende.size()) file::update(FILE, user.id, 4, gende.c_str());
					
					csv_file file(FILE); user = file.data[user.id];
					gotoxy(46, 27, COLOR_GREEN); std::cout << " Save changes successfully.";
					PAUSE; return;
				}
				if (choose == 1) return;
			}
			if (c == 224 || c == 0) {
				c = getch();
				if (c == KEY_LEFT && choose == 1) choose--;
				else if (c == KEY_RIGHT && choose == 0) choose++;
			}
		}
	}
}

bool npstudent::remove(csv_line& user) {
	gotoxy(33, 25); std::cout << "Are you sure to remove this student, cannot be undone.";

	for (int choose = 1;;) {
		gotoxy(51, 27, (choose == 0) ? COLOR_RED_BACKGROUND : COLOR_WHITE); std::cout << "[Remove]";
		gotoxy(60, 27, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Cancel]";

		uint8_t c = getch();
		csv_line* student = nullptr;
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				csv_file infofile(".\\data\\student\\__student.csv");
				student = file::exists(infofile, user.pdata[1]);
				if (student == nullptr) return 0;
				file::update(".\\data\\student\\__student.csv", student->id, 0, "0");

				csv_file account(".\\data\\account.csv");
				student = file::exists(account, user.pdata[1]);
				if (student == nullptr) return 0;
				file::update(".\\data\\account.csv", student->id, 0, "0");

				return 1;
			}
			return 0;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0) choose++;
		}
	}
}

void npstudent::calendar(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	std::time_t now = time(0);
	std::tm ltm = *localtime(&now); ltm.tm_hour = 0; std::mktime(&ltm);
	
	minibox_layout.print();
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                            My Calendar                            ";
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << "    Date    | Course                               | Start |  End  ";

	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);
	csv_file course_list((COURSE_PATH("__course.csv").c_str()), def_course);

	int choose = 0, maxChoose = 13; bool has_change = 0;
	while (1) {
		int cur = 0;
		std::tm day = ltm; day.tm_mday -= 7;
		for (int i = 0; cur < choose + 18 && i < 31; ++i) {
			int y = 10 + cur - choose;
			day.tm_mday++; std::mktime(&day);

			WORD COLOR_CODE = (day.tm_mday % 2) ? 112 : 240;
			if (day.tm_mday == ltm.tm_mday) COLOR_CODE = COLOR_RED_BACKGROUND;
			if (9 < y && y < 28) {
				gotoxy(27, y, COLOR_CODE); std::cout << "            |                                      |       |       ";
				gotoxy(28, y, COLOR_CODE); control::print(day);
			}

			csv_line* course = nullptr;
			bool empty = 1;
			for (int j = 0; j < my_course.count; ++j) {
				course = &my_course.data[j];
				if (strcmp(course->pdata[0], ACADEMICYEAR.c_str())) continue;
				if (strcmp(course->pdata[1], SEMESTER.c_str())) continue;
				if (npcourse::now(course->pdata[2], course->pdata[3], day) == 0) continue;

				y = 10 + cur - choose;		
				if (y < 10) { cur++; empty = 0; continue; }
				if (y > 27) {
					if (!has_change && choose == 13) maxChoose++;
					continue;
				}
				
				if (!empty) { gotoxy(27, y, COLOR_CODE); std::cout << "            |                                      |       |       "; }
				gotoxy(41, y, COLOR_CODE); std::cout << course->pdata[2] << ": ";

				for (int k = 0; k < course_list.count; ++k) {
					if (strcmp(course->pdata[2], course_list.data[k].pdata[1])) continue;
					std::cout << course_list.data[k].pdata[2];
					gotoxy(80, y, COLOR_CODE); std::cout << course_list.data[k].pdata[8];
					gotoxy(88, y, COLOR_CODE); std::cout << course_list.data[k].pdata[9];
					break;
				}
				cur++;  empty = 0;
			} 
			if (empty) cur++;
			if (maxChoose != 13) has_change = 1;
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < maxChoose) choose++;
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
		}
	}
}

