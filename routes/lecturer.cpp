#include "..\\controls\\lecturer.h"

int nplecturer::courses_list(const char* lecturer_id) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	int choose = 0, cur = -1; int* row = nullptr;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Lecturer list  ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Course list    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Course ID    | Class     | Lecturer ID  | Room   | Status ";
	while ((cur = -1)) {
		csv_file course_list(__COURSE, def_course);
		csv_line* course = nullptr;

		if (row) delete[] row;
		row = new int[course_list.count];

		for (int i = 0; i < course_list.count; ++i) {
			course = &course_list.data[i];
			if (strcmp(course->pdata[4], lecturer_id) != 0) continue;
			int y = 10 + (++cur) + choose; row[cur] = i;
			if (y < 10 || y > 27) continue;

			WORD COLOR_CODE = (cur % 2) ? 112 : 240;

			gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |              |        |        ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(36, y, COLOR_CODE); std::cout << course->pdata[1];
			gotoxy(51, y, COLOR_CODE); std::cout << course->pdata[3];
			gotoxy(63, y, COLOR_CODE); std::cout << course->pdata[4];
			gotoxy(78, y, COLOR_CODE); std::cout << course->pdata[10];

			// Status
			if (course->pdata[0][0] == '0') { gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << "private"; }
			else { gotoxy(87, y, COLOR_CODE + 2); std::cout << "public"; }
			
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " Search     Ctrl+F  ";
			gotoxy(78, 9, 128); std::cout << "                    ";
			getch();
			gotoxy(78, 8); std::cout << "                    ";
			gotoxy(78, 9); std::cout << "                    ";
			goto LAYOUT;
		}
		course = &course_list.data[row[choose]];
		if (c == KEY_SEARCH) {
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < cur - 17) choose++;
			else if (c == KEY_LEFT) return -1;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	return 0;
}

void nplecturer::info(const char* lecturer_id, bool EDIT, bool _DELETE) {
LAYOUT:
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, ON);
	if (lecturer == nullptr) {
		gotoxy(32, 15, COLOR_BLUE_BACKGROUND); std::cout << " Lecturer info                                            ";
		gotoxy(32, 16, 128); std::cout << "                                                         ";
		gotoxy(33, 16, 132); std::cout << "This lecturer does not exist";
		PAUSE; return;
	}

	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " Lecturer info                                           ";
	gotoxy(32, 14, 143); std::cout << " Lecturer name:                                          ";
	gotoxy(32, 15, 143); std::cout << "                                                         ";
	gotoxy(32, 16, 128); std::cout << " Lecturer id  :                                          ";
	gotoxy(32, 17, 128); std::cout << " Ranking      :                                          ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "                                                         ";

	gotoxy(48, 14, 143);
	if (ENGLISHNAME) std::cout << lecturer->pdata[3] << ' ' << lecturer->pdata[2];
	else std::cout << lecturer->pdata[2] << ' ' << lecturer->pdata[3];
	gotoxy(48, 16, 128); std::cout << lecturer->pdata[1];
	gotoxy(48, 17, 128); std::cout << lecturer->pdata[4];
	
	if (EDIT) {
		if (_DELETE == ON) { nplecturer::remove(lecturer_id); return; }
		for (int choose = 0;;) {
			gotoxy(51, 19, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "  Edit  ";
			gotoxy(60, 19, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Remove ";

			uint8_t c = getch();
			if (c == KEY_ESC)break;
			if (c == KEY_ENTER) {
				if (choose == 0) {
					nplecturer::edit(lecturer_id);
					goto LAYOUT;
				}
				else if (choose == 1) {
					if (nplecturer::remove(lecturer_id)) return;
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
		gotoxy(54, 19, COLOR_WHITE_BACKGROUND); std::cout << "    Back    ";
		uint8_t c = getch();
		while (c != KEY_ESC && c != KEY_ENTER) c = getch();
	}
}

void nplecturer::search(csv_file& lecturer_list, int cur, int& choose, int& overflow, int* row){
	int old = choose; std::string search;
	csv_line* lecturer = nullptr;

	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			lecturer = &lecturer_list.data[row[choose]];
			if (strstr(lecturer->pdata[1], search.c_str()) ||
				strstr(lecturer->pdata[2], search.c_str()) ||
				strstr(lecturer->pdata[3], search.c_str())) return;
		} while (choose != old);

		lowercase(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			lecturer = &lecturer_list.data[row[choose]];
			if (strstr(lecturer->pdata[1], search.c_str()) ||
				strstr(lecturer->pdata[2], search.c_str()) ||
				strstr(lecturer->pdata[3], search.c_str())) return;
		} while (choose != old);

		capitalize(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			lecturer = &lecturer_list.data[row[choose]];
			if (strstr(lecturer->pdata[1], search.c_str()) ||
				strstr(lecturer->pdata[2], search.c_str()) ||
				strstr(lecturer->pdata[3], search.c_str())) return;
		} while (choose != old);
	}
}

// [EDIT]::lecturer //==========================================================================================================================//

void nplecturer::add() {
	std::string lname, fname, lecid, rankg;

	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " New lecturer                                            ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "                                                         ";
	gotoxy(46, 19, 128); std::cout << "    Save     ";
	gotoxy(60, 19, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << " First name   :                                          ";
	gotoxy(32, 15, 143); std::cout << " Last name    :                                          ";
	gotoxy(32, 16, 128); std::cout << " Lecturer id  :                                          ";
	gotoxy(32, 17, 128); std::cout << " Ranking      :                                          ";

	while (fname.empty()) if (read(48, 14, 143, fname, 20, SHOW) == KEY_ESC) return;
	while (lname.empty()) if (read(48, 15, 143, lname, 20, SHOW) == KEY_ESC) return;
	while (1) {
		if (read(48, 16, 128, lecid, 20, SHOW, "                    ") == KEY_ESC) return;
		// default_lecture_id
		if (lecid.empty()) {
			lecid = lname[0];
			for (int i = 1; i < lname.size(); ++i) {
				if (lname[i - 1] == ' ') lecid += lname[i];
			}
			lecid = lecid + fname;
		}
		if (file::find(ACCOUNT, lecid.c_str(), nullptr, ON) != -1) continue;
		gotoxy(48, 16, 128); std::cout << lecid; break;
	}
	while (rankg.empty()) if (read(48, 17, 128, rankg, 20, SHOW) == KEY_ESC) return;

	capitalize(fname);
	capitalize(lname);
	rankg[0] = toupper(rankg[0]);

	for (int choose = 0;;) {
		gotoxy(46, 19, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "    Save     ";
		gotoxy(60, 19, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "   Cancel    ";

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				std::ofstream lecturer(__LECTURER, std::ios::app);
				lecturer << "1," << lecid << ',' << lname << ',' << fname << ',' << rankg << '\n';
				lecturer.close();

				std::ofstream account(ACCOUNT, std::ios::app);
				account << "1," << lecid << ",0" << ",lecturer\n";
				account.close();

				gotoxy(46, 19, 128 + COLOR_BLUE); std::cout << " Save successfully.        ";
				PAUSE;
			}
			return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_RIGHT && choose == 0) choose++;
			if (c == KEY_LEFT && choose == 1) choose--;
		}
	}
}

bool nplecturer::sort() {
	gotoxy(23, 28, 8); std::cout << "(Ctrl + \\) was pressed. Waiting for second key of chord...";
	uint8_t c = getch();
	if (c == '1') file::sort(__LECTURER, 1);			// sort: lecturer id
	else if (c == '2') file::sort(__LECTURER, 4, 1);	// sort: ranking
	else if (c == '3') file::sort(__LECTURER, 3, 2);	// sort: first name
	else {
		gotoxy(23, 28); std::cout << "                                                          ";
		return 0;
	}
	return 1;
}

void nplecturer::edit(const char* lecturer_id) {
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, ON);
	if (lecturer == nullptr) return;

	gotoxy(46, 19, 128); std::cout << " Save change ";
	gotoxy(60, 19, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << " Firstname    :                                          ";
	gotoxy(32, 15, 143); std::cout << " Lastname     :                                          ";
	gotoxy(48, 14, 143); std::cout << lecturer->pdata[3];
	gotoxy(48, 15, 143); std::cout << lecturer->pdata[2];

	std::string fname, lname, rankg;

	if (read(48, 14, 143, fname, 20, SHOW, lecturer->pdata[3]) == KEY_ESC) return;
	if (read(48, 15, 143, lname, 20, SHOW, lecturer->pdata[2]) == KEY_ESC) return;
	if (read(48, 17, 128, rankg, 20, SHOW, lecturer->pdata[4]) == KEY_ESC) return;

	// Save
	for (int choose = 0;;) {
		gotoxy(46, 19, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Save change ";
		gotoxy(60, 19, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "   Cancel    ";

		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				// Update: __lecturer.csv
				capitalize(fname);
				capitalize(lname);

				if (fname.size()) file::update(__LECTURER, lecturer->id, 3, fname.c_str());
				if (lname.size()) file::update(__LECTURER, lecturer->id, 2, lname.c_str());
				if (rankg.size()) file::update(__LECTURER, lecturer->id, 4, rankg.c_str());

				gotoxy(46, 19, 128 + COLOR_BLUE); std::cout << " Save changes successfully.";
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

int nplecturer::remove(const char* lecturer_id) {
	gotoxy(33, 18, 128 + COLOR_RED); std::cout << "Are you sure to remove this lecturer, cannot be undone.";
	for (int choose = 1;;) {
		gotoxy(51, 19, (choose == 0) ? COLOR_RED_BACKGROUND : 128); std::cout << " Remove ";
		gotoxy(60, 19, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Cancel ";
	
		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		else if (c == KEY_ENTER) {
			if (choose == 0) {
				file::remove(__LECTURER, file::find(__LECTURER, lecturer_id, nullptr, ON));
				file::remove(ACCOUNT, file::find(ACCOUNT, lecturer_id, nullptr, ON));

				csv_file course_list(__COURSE);
				for (int i = 0; i < course_list.count; ++i) {
					if (strcmp(course_list.data[i].pdata[4], lecturer_id) == 0) file::update(__COURSE, i, 4, "");
				}

				gotoxy(46, 19, 128 + COLOR_BLUE); std::cout << " Remove successfully.";
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
