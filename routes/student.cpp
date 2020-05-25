#include "..\\controls\\student.h"

void npstudent::list(csv_line& user, const char* class_id) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	int choose = 0, cur = -1, overflow = 0; int* row = nullptr;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Class list     ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Student list   ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << " No.      | Class    | Student ID | Full name                      ";

	while ((cur = -1)) {
		csv_file student_list(((std::string)".\\data\\class\\" + class_id + ".csv").c_str(), def_class);
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
		// KEY_HELP:
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " New       Ctrl+N   ";
			gotoxy(78, 9, 128); std::cout << " Open      Ctrl+O   ";
			gotoxy(78,10, 128); std::cout << " Search    Ctrl+F   ";
			gotoxy(78,11, 128); std::cout << " Delete    Delete   ";
			gotoxy(78,12, 128); std::cout << "____________________";
			gotoxy(78,13, 128); std::cout << " Sort      Ctrl+\\,  ";
			gotoxy(78,14, 128); std::cout << "  - Student ID    1 ";
			gotoxy(78,15, 128); std::cout << "  - First name    2 ";
			gotoxy(78,16, 128); std::cout << "                    ";
			getch();
			goto LAYOUT;
		}
		// KEY_NEW:
		// KEY_OPEN:
		if (c == KEY_SEARCH) { npstudent::search(student_list, cur, choose, overflow, row); goto LAYOUT; }
		if (c == KEY_FUNCTION) { if (npstudent::sort(((std::string)".\\data\\class\\" + class_id + ".csv").c_str())) goto LAYOUT; else goto NO_CHANGE; }
		if (c == KEY_ENTER) { npstudent::info(student_list.data[row[choose]].pdata[1], ON); goto LAYOUT; }
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_DELETE) { npstudent::info(student_list.data[row[choose]].pdata[1], ON, ON); goto LAYOUT; }
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

	int choose = 0, cur = -1, overflow = 0; int* row = nullptr;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Student list   ";
	colorizing(COLOR_YELLOW); std::cout << "  Attendance     " << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << " Course   | Class    | Student ID | Full name                      ";
	
	while ((cur = -1)) {
		csv_file student_list(PROCESS(course_id, course_cs), def_process);
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
		// KEY_HELP:
		if (KEY_HELP(c)) {
			if (user == "staff") {
				gotoxy(78, 8, 128); std::cout << " Search    Ctrl+F   ";
				gotoxy(78, 9, 128); std::cout << " Delete    Delete   ";
				gotoxy(78,10, 128); std::cout << "____________________";
				gotoxy(78,11, 128); std::cout << " Sort      Ctrl+\\,  ";
				gotoxy(78,12, 128); std::cout << "  - Student ID    1 ";
				gotoxy(78,13, 128); std::cout << "  - First name    2 ";
				gotoxy(78,14, 128); std::cout << "____________________";
				gotoxy(78,15, 128); std::cout << " Enrol     R, r     ";
				gotoxy(78,16, 128); std::cout << "                    ";
			}
			else {
				gotoxy(78, 8, 128); std::cout << " Search    Ctrl+F   ";
				gotoxy(78, 9, 128); std::cout << "____________________";
				gotoxy(78,10, 128); std::cout << " Sort      Ctrl+\\,  ";
				gotoxy(78,11, 128); std::cout << "  - Student ID    1 ";
				gotoxy(78,12, 128); std::cout << "  - First name    2 ";
				gotoxy(78,13, 128); std::cout << "____________________";
				gotoxy(78,14, 128); std::cout << " Enrol     R, r     ";
				gotoxy(78,15, 128); std::cout << "                    ";
			}
			getch();
			goto LAYOUT;
		}
		if (c == KEY_SEARCH) { npstudent::search(student_list, cur, choose, overflow, row); goto LAYOUT; }
		// POSITION: STAFF
		if (user == "staff") {
			// KEY_OPEN:
		}
		if (c == KEY_FUNCTION) { if (npstudent::sort(PROCESS(course_id, course_cs))) goto LAYOUT; else goto NO_CHANGE; }
		if (c == KEY_ENTER) { npstudent::info(student_list.data[row[choose]].pdata[1], OFF); goto LAYOUT; }
		if (KEY_EROL(c)) { npcourse::enrol(user, course_id, course_cs); continue; }
		if (c == 224 || c == 0) {
			c = getch();
			//if (c == KEY_DELETE && user == "staff") { goto LAYOUT; }
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

void npstudent::info(const char* student_id, bool EDIT, bool _DELETE) {
LAYOUT:
	csv_file student_list(__STUDENT);
	csv_line* student = file::find(student_list, student_id, nullptr, ON);
	if (student == nullptr) {
		gotoxy(32, 15, COLOR_BLUE_BACKGROUND); std::cout << " Student info                                            ";
		gotoxy(32, 16, 128); std::cout << "                                                         ";
		gotoxy(33, 16, 132); std::cout << "This student does not exist";
		PAUSE; return;
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
		if (_DELETE == ON) { npstudent::remove(student_id); return; }
		for (int choose = 0;;) {
			gotoxy(51, 21, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "  Edit  ";
			gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Remove ";

			uint8_t c = getch();
			if (c == KEY_ESC) break;
			if (c == KEY_ENTER) {
				if (choose == 0) {
					npstudent::edit(student_id);
					goto LAYOUT;
				}
				if (choose == 1) {
					if (npstudent::remove(student_id)) return;
					goto LAYOUT;
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
}

void npstudent::search(csv_file& student_list, int cur, int& choose, int& overflow, int* row) {
	int old = choose; std::string search;
	csv_line* student = nullptr;
	csv_file student_info(__STUDENT);

	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
		std::transform(search.begin(), search.end(), search.begin(), ::tolower);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			student = file::find(student_info, student_list.data[row[choose]].pdata[1], nullptr, OFF);
			if (strstr(student->pdata[1], search.c_str()) ||
				strstr(student->pdata[2], search.c_str()) ||
				strstr(student->pdata[3], search.c_str())) return;
		} while (choose != old);

		capitalize(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			student = file::find(student_info, student_list.data[row[choose]].pdata[1], nullptr, OFF);
			if (strstr(student->pdata[1], search.c_str()) ||
				strstr(student->pdata[2], search.c_str()) ||
				strstr(student->pdata[3], search.c_str())) return;
		} while (choose != old);
	}
}

// [EDIT]::student //===========================================================================================================================//

void npstudent::sort(const char* FILE, int col1, int col2, int col3) {
	csv_file file(FILE);
	// Init
	int n = file.count;
	int* row = new int[n];
	for (int i = 0; i < n; ++i) row[i] = i;

	csv_file info(__STUDENT);
	std::ofstream out(FILE);
	// Sort O(n^2)
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			csv_line* istu = file::find(info, file.data[row[i]].pdata[1], nullptr, ON);
			csv_line* jstu = file::find(info, file.data[row[j]].pdata[1], nullptr, ON);
			if (istu == nullptr || jstu == nullptr) continue;

			if (col1 > -1 && !file::sort_cmp_default(istu->pdata[col1], jstu->pdata[col1])) std::swap(row[i], row[j]);
			if (strcmp(istu->pdata[col1], jstu->pdata[col1])) continue;

			if (col2 > -1 && !file::sort_cmp_default(istu->pdata[col2], jstu->pdata[col2])) std::swap(row[i], row[j]);
			if (strcmp(istu->pdata[col2], jstu->pdata[col2])) continue;

			if (col3 > -1 && !file::sort_cmp_default(istu->pdata[col3], jstu->pdata[col3])) std::swap(row[i], row[j]);
		}
	}

	// Save file
	for (int j = 0; j < file.mark.count; ++j) {
		out << file.mark.pdata[j];
		if (j + 1 != file.mark.count) out << ',';
	}
	out << "\n";
	for (int i = 0; i < n; out << "\n", ++i) {
		for (int j = 0; j < file.data[row[i]].count; ++j) {
			out << file.data[row[i]].pdata[j];
			if (j + 1 != file.data[row[i]].count) out << ',';
		}
	}
	out.close();
	delete[] row;
}

bool npstudent::sort(const char* FILE) {
	gotoxy(23, 28, 8); std::cout << "(Ctrl + \\) was pressed. Waiting for second key of chord...";
	uint8_t c = getch();
	if (c == '1') npstudent::sort(FILE, 1);				// sort: student id
	else if (c == '2') npstudent::sort(FILE, 3, 2, 1);	// sort: first name
	else {
		gotoxy(23, 28); std::cout << "                                                          ";
		return 0;
	}
	return 1;
}

void npstudent::edit(const char* student_id) {
	csv_file student_list(__STUDENT);
	csv_line* student = file::find(student_list, student_id, nullptr, ON);
	if (student == nullptr) return;

	gotoxy(46, 21, 128); std::cout << " Save change ";
	gotoxy(60, 21, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << "                                                         ";
	gotoxy(32, 15, 143); std::cout << "                                                         ";
	gotoxy(33, 14, 143); std::cout << "Firstname   : "; std::cout << student->pdata[3];
	gotoxy(33, 15, 143); std::cout << "Lastname    : "; std::cout << student->pdata[2];

	std::string fname, lname, newcs, birth, gende;

	if (read(47, 14, 143, fname, 20, SHOW, student->pdata[3]) == KEY_ESC) return;
	if (read(47, 15, 143, lname, 20, SHOW, student->pdata[2]) == KEY_ESC) return;
	if (read(47, 17, 128, newcs, 20, SHOW, student->pdata[6]) == KEY_ESC) return;
	while (birth != "1" && birth.size() != 10) if (date(47, 18, 128, birth) == KEY_ESC) return;
	if (birth.size() != 10) { gotoxy(47, 18, 128); std::cout << student->pdata[5]; }
	if (read(47, 19, 128, gende, 20, SHOW, student->pdata[4]) == KEY_ESC) return;

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
				capitalize(fname); //std::transform(fname.begin(), fname.end(), fname.begin(), ::toupper);
				capitalize(lname); //std::transform(lname.begin(), lname.end(), lname.begin(), ::toupper);
				if (npclass::change(*student, student->pdata[6], newcs.c_str()) == 0) {
					gotoxy(46, 21, 128 + COLOR_RED); std::cout << " Save changes failed.      ";
					PAUSE; return;
				}

				if (newcs.size()) file::update(__STUDENT, student->id, 6, newcs.c_str());
				if (fname.size()) file::update(__STUDENT, student->id, 3, fname.c_str());
				if (lname.size()) file::update(__STUDENT, student->id, 2, lname.c_str());
				if (gende.size()) file::update(__STUDENT, student->id, 4, gende.c_str());
				if (birth.size() == 10) file::update(__STUDENT, student->id, 5, birth.c_str());

				gotoxy(46, 21, 128 + COLOR_BLUE); std::cout << " Save changes successfully.";
				PAUSE; return;
			}
			return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0) choose++;
		}
	}
}

int npstudent::remove(const char* student_id) {
	gotoxy(33, 20, 128 + COLOR_RED); std::cout << "Are you sure to remove this student, cannot be undone.";
	for (int choose = 1;;) {
		gotoxy(51, 21, (choose == 0) ? COLOR_RED_BACKGROUND : 128); std::cout << " Remove ";
		gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Cancel ";

		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				file::remove(__STUDENT, file::find(__STUDENT, student_id, nullptr, ON));
				file::remove(ACCOUNT, file::find(ACCOUNT, student_id, nullptr, ON));
				gotoxy(46, 21, 128 + COLOR_BLUE); std::cout << " Remove successfully.";
				PAUSE; return 1;
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
