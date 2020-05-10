#include "..\\controls\\student.h"

void npstudent::list(csv_line& user, const char* class_id) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

LAYOUT:
	minibox_layout.print();
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Class list     ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Student list   ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << " No.      | Class    | Student ID | Full name                      ";

	int choose = 0, cur = -1, overflow = 0; int* row = nullptr;
	while ((cur = -1)) {
		csv_file student_list(((std::string)".\\data\\class\\" + class_id + ".csv").c_str(), def_course);
		csv_file student_info(__STUDENT);
		csv_line* student = nullptr;
		if (row) delete[] row;
		row = new int[student_list.count];

		for (int i = 0; i < student_list.count; ++i) {
			if (student_list.data[i].pdata[0][0] == '0') continue;
			if ((student = file::find(student_info, student_list.data[i].pdata[1], nullptr, ON)) == nullptr) continue;

			int y = 10 + (++cur) + overflow; row[cur] = i;
			if (y < 10 || y > 27) continue;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			if (choose == cur) COLOR_CODE = 176;


			gotoxy(27, y, COLOR_CODE); std::cout << "          |          |            |                                ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(39, y, COLOR_CODE); std::cout << class_id;
			gotoxy(50, y, COLOR_CODE); std::cout << student->pdata[1];
			gotoxy(63, y, COLOR_CODE);
			if (ENGLISHNAME) std::cout << student->pdata[3] << ' ' << student->pdata[2];
			else std::cout << student->pdata[2] << ' ' << student->pdata[3];
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (KEY_HELP(c)) {

		}
		student = &student_list.data[row[choose]];
		if (c == KEY_SEARCH) {
			int old = choose; std::string search;
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
			gotoxy(32, 16, 128); std::cout << "                                                         ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
				std::transform(search.begin(), search.end(), search.begin(), ::tolower);
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;

					student = file::find(student_info, student_list.data[row[choose]].pdata[1], nullptr, ON);

					if (strstr(student->pdata[1], search.c_str()) ||
						strstr(student->pdata[2], search.c_str()) ||
						strstr(student->pdata[3], search.c_str())) goto SUCCESS_SEARCH;
				} while (choose != old);

				//std::transform(search.begin(), search.end(), search.begin(), ::toupper);
				search.front() = toupper(search.front());
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;

					student = file::find(student_info, student_list.data[row[choose]].pdata[1], nullptr, ON);

					if (strstr(student->pdata[1], search.c_str()) ||
						strstr(student->pdata[2], search.c_str()) ||
						strstr(student->pdata[3], search.c_str())) goto SUCCESS_SEARCH;
				} while (choose != old);
			}

		SUCCESS_SEARCH:
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			continue;
		}
		if (c == KEY_ENTER) {
			npstudent::info(student->pdata[1], ON);
			goto LAYOUT;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	delete[] row;
}

void npstudent::list(csv_line& user, const char* course_id, const char* course_cs) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

LAYOUT:
	minibox_layout.print();
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Student list   ";
	colorizing(COLOR_YELLOW); std::cout << "  Attendance     " << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << " Course   | Class    | Student ID | Full name                      ";

	std::string stustpath = COURSE_PATH("process\\") + course_id + '_' + course_cs + ".csv";

	int choose = 0, cur = -1, overflow = 0; int* row = nullptr;
	while ((cur = -1)) {
		csv_file student_list(stustpath.c_str(), def_course);
		csv_file student_info(__STUDENT);
		csv_line* student = nullptr;
		if (row) delete[] row;
		row = new int[student_list.count];

		for (int i = 0; i < student_list.count; ++i) {
			if ((student = file::find(student_info, student_list.data[i].pdata[1], nullptr, ON)) == nullptr) continue;

			int y = 10 + (++cur) + overflow; row[cur] = i;
			if (y < 10 || y > 27) continue;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			if (choose == cur) COLOR_CODE = 176;


			gotoxy(27, y, COLOR_CODE); std::cout << "          |          |            |                                ";
			gotoxy(28, y, COLOR_CODE); std::cout << course_id;
			gotoxy(39, y, COLOR_CODE); std::cout << course_cs;
			gotoxy(50, y, COLOR_CODE); std::cout << student->pdata[1];
			gotoxy(63, y, COLOR_CODE);
			if (ENGLISHNAME) std::cout << student->pdata[3] << ' ' << student->pdata[2];
			else std::cout << student->pdata[2] << ' ' << student->pdata[3];
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (KEY_HELP(c)) {

		}
		student = &student_list.data[row[choose]];
		if (c == KEY_SEARCH) {
			int old = choose; std::string search;
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
			gotoxy(32, 16, 128); std::cout << "                                                         ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
				std::transform(search.begin(), search.end(), search.begin(), ::tolower);
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;

					student = file::find(student_info, student_list.data[row[choose]].pdata[1], nullptr, ON);

					if (strstr(student->pdata[1], search.c_str()) ||
						strstr(student->pdata[2], search.c_str()) ||
						strstr(student->pdata[3], search.c_str())) goto SUCCESS_SEARCH;
				} while (choose != old);

				//std::transform(search.begin(), search.end(), search.begin(), ::toupper);
				search.front() = toupper(search.front());
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;

					student = file::find(student_info, student_list.data[row[choose]].pdata[1], nullptr, ON);

					if (strstr(student->pdata[1], search.c_str()) ||
						strstr(student->pdata[2], search.c_str()) ||
						strstr(student->pdata[3], search.c_str())) goto SUCCESS_SEARCH;
				} while (choose != old);
			}

		SUCCESS_SEARCH:
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			continue;
		}
		
		if (c == KEY_ENTER) {
			npstudent::info(student->pdata[1], OFF);
			continue;
		}
		if (KEY_EROL(c)) {
			npcourse::enrol(user, course_id, course_cs);
			continue;
		}
	
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_RIGHT) {
				if (user == "staff") if (npattendance::staff(course_id, course_cs) == 0) break;
				if (user == "lecturer") if (npattendance::lecturer(course_id, course_cs) == 0) break;
				if (user == "student") goto NO_CHANGE;
				goto LAYOUT;
			}
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	delete[] row;
}

void npstudent::info(const char* student_id, bool EDIT) {
LAYOUT:
	csv_file student_list(__STUDENT);
	csv_line* student = file::find(student_list, student_id, nullptr, ON);
	if (student == nullptr) {
		gotoxy(32, 15, COLOR_BLUE_BACKGROUND); std::cout << " Student info                                            ";
		gotoxy(32, 16, 128); std::cout << "                                                         ";
		gotoxy(33, 16, 132); std::cout << "This student does not exist";
		goto END;
	}

	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " Student info                                            ";
	gotoxy(32, 14, 128); std::cout << "                                                         ";
	gotoxy(32, 15, 128); std::cout << "                                                         ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "                                                         ";
	gotoxy(32, 20, 128); std::cout << "                                                         ";
	gotoxy(32, 21, 128); std::cout << "                                                         ";

	gotoxy(33, 14, 143); std::cout << "Student name: ";
	if (ENGLISHNAME) std::cout << student->pdata[3] << ' ' << student->pdata[2];
	else std::cout << student->pdata[2] << ' ' << student->pdata[3];
	gotoxy(33, 16, 128); std::cout << "Student id  : " << student->pdata[1];
	gotoxy(33, 17, 128); std::cout << "Class       : " << student->pdata[6];
	gotoxy(33, 18, 128); std::cout << "Day of birth: " << student->pdata[5];
	gotoxy(33, 19, 128); std::cout << "Gender      : " << student->pdata[4];

	if (EDIT) {
		for (int choose = 0;;) {
			gotoxy(51, 21, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "  Edit  ";
			gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Remove ";

			uint8_t c = getch();
			if (c == KEY_ESC) break;
			if (c == KEY_ENTER) {
				if (choose == 0) {
					gotoxy(46, 21, 128); std::cout << " Save change ";
					gotoxy(60, 21, 128); std::cout << "   Cancel    ";
					// Get detail
					gotoxy(32, 14, 143); std::cout << "                                                         ";
					gotoxy(32, 15, 143); std::cout << "                                                         ";
					gotoxy(33, 14, 143); std::cout << "Firstname   : "; std::cout << student->pdata[3];
					gotoxy(33, 15, 143); std::cout << "Lastname    : "; std::cout << student->pdata[2];

					std::string fname, lname, newcs, birth, gende;

					if (read(47, 14, 143, fname, 20, SHOW, student->pdata[3]) == KEY_ESC) goto LAYOUT;
					if (read(47, 15, 143, lname, 20, SHOW, student->pdata[2]) == KEY_ESC) goto LAYOUT;
					if (read(47, 17, 128, newcs, 20, SHOW, student->pdata[6]) == KEY_ESC) goto LAYOUT;
					while (birth != "1" && birth.size() != 10) if (date(47, 18, 128, birth) == KEY_ESC) goto LAYOUT;
					if (birth.size() != 10) { gotoxy(47, 18, 128); std::cout << student->pdata[5]; }
					if (read(47, 19, 128, gende, 20, SHOW, student->pdata[4]) == KEY_ESC) goto LAYOUT;

					// Save
					for (int choose = 0;;) {
						gotoxy(46, 21, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Save change ";
						gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "   Cancel    ";

						uint8_t c = getch();
						if (c == KEY_ESC) break;
						if (c == KEY_ENTER) {
							if (choose == 0) {
								// Update: __student.csv
								std::transform(newcs.begin(), newcs.end(), newcs.begin(), ::toupper);
								std::transform(fname.begin(), fname.end(), fname.begin(), ::toupper);
								std::transform(lname.begin(), lname.end(), lname.begin(), ::toupper);
								if (npclass::change(*student, student->pdata[6], newcs.c_str()) == 0) {
									gotoxy(46, 21, 128 + COLOR_RED); std::cout << " Save changes failed.      ";
									PAUSE; goto LAYOUT;
								}
									
								if (newcs.size()) file::update(__STUDENT, student->id, 6, newcs.c_str());
								if (fname.size()) file::update(__STUDENT, student->id, 3, fname.c_str());
								if (lname.size()) file::update(__STUDENT, student->id, 2, lname.c_str());
								if (gende.size()) file::update(__STUDENT, student->id, 4, gende.c_str());
								if (birth.size() == 10) file::update(__STUDENT, student->id, 5, birth.c_str());

								gotoxy(46, 21, 128 + COLOR_BLUE); std::cout << " Save changes successfully.";
								PAUSE; goto LAYOUT;
							}
							break;
						}
						if (c == 224 || c == 0) {
							c = getch();
							if (c == KEY_LEFT && choose == 1) choose--;
							else if (c == KEY_RIGHT && choose == 0) choose++;
						}
					}
					goto LAYOUT;

				}
				if (choose == 1) {
					gotoxy(33, 20, 128 + COLOR_RED); std::cout << "Are you sure to remove this student, cannot be undone.";
					for (int choose = 1;;) {
						gotoxy(51, 21, (choose == 0) ? COLOR_RED_BACKGROUND : 128); std::cout << " Remove ";
						gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Cancel ";

						uint8_t c = getch();
						if (c == KEY_ESC) goto LAYOUT;
						if (c == KEY_ENTER) {
							if (choose == 0) {
								file::remove(__STUDENT, student->id);
								file::remove(ACCOUNT, file::find(ACCOUNT, student->pdata[1], nullptr, ON));
								gotoxy(46, 21, 128 + COLOR_BLUE); std::cout << " Remove successfully.";
								PAUSE; goto END;
							}
							goto LAYOUT;
						}
						if (c == 224 || c == 0) {
							c = getch();
							if (c == KEY_LEFT && choose == 1) choose--;
							else if (c == KEY_RIGHT && choose == 0) choose++;
						}
					}
					break;
				}
			}
			if (c == 224 || c == 0) {
				c = getch();
				if (c == KEY_LEFT && choose == 1) choose--;
				else if (c == KEY_RIGHT && choose == 0) choose++;
			}
		}
	}
	else {
		gotoxy(54, 21, COLOR_WHITE_BACKGROUND); std::cout << "    Back    ";
		uint8_t c = getch();
		while (c != KEY_ESC && c != KEY_ENTER) c = getch();
	}

END:
	gotoxy(32, 13); std::cout << "                                                         ";
	gotoxy(32, 14); std::cout << "                                                         ";
	gotoxy(32, 15); std::cout << "                                                         ";
	gotoxy(32, 16); std::cout << "                                                         ";
	gotoxy(32, 17); std::cout << "                                                         ";
	gotoxy(32, 18); std::cout << "                                                         ";
	gotoxy(32, 19); std::cout << "                                                         ";
	gotoxy(32, 20); std::cout << "                                                         ";
	gotoxy(32, 21); std::cout << "                                                         ";
}

int npstudent::calendar(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	std::time_t now = time(0);
	std::tm ltm = *localtime(&now); ltm.tm_hour = 0; std::mktime(&ltm);

	minibox_layout.print();
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Calendar       ";
	colorizing(COLOR_YELLOW); std::cout << "  Attendance     " << "  Scoreboard    ";
	// Detail
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
			else if (c == KEY_LEFT) return -1;
			else if (c == KEY_RIGHT) return 1;
			else goto NO_CHANGE;
		}
	}
	return 0;
}
