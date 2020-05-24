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
			if (course->pdata[0][0] == '0') {
				gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << "private";
			}
			else {
				gotoxy(87, y, COLOR_CODE + 2); std::cout << "public";
			}
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

void nplecturer::info(const char* lecturer_id, bool EDIT) {
LAYOUT:
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, ON);
	if (lecturer == nullptr) {
		gotoxy(32, 15, COLOR_BLUE_BACKGROUND); std::cout << " Lecturer info                                            ";
		gotoxy(32, 16, 128); std::cout << "                                                         ";
		gotoxy(33, 16, 132); std::cout << "This lecturer does not exist";
		goto END;
	}

	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " Lecturer info                                           ";
	gotoxy(32, 14, 128); std::cout << "                                                         ";
	gotoxy(32, 15, 128); std::cout << "                                                         ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "                                                         ";

	gotoxy(33, 14, 143); std::cout << "Lecturer name: ";
	if (ENGLISHNAME) std::cout << lecturer->pdata[3] << ' ' << lecturer->pdata[2];
	else std::cout << lecturer->pdata[2] << ' ' << lecturer->pdata[3];
	gotoxy(33, 16, 128); std::cout << "Lecturer id  : " << lecturer->pdata[1];
	gotoxy(33, 17, 128); std::cout << "Ranking      : " << lecturer->pdata[4];
	//nplecturer::courseslist(lecturer->pdata[1]);
	if (EDIT) {
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
					if (nplecturer::remove(lecturer_id)) goto END;
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
END:
	gotoxy(27, 13); std::cout << "                                                                  ";
	gotoxy(27, 14); std::cout << "                                                                  ";
	gotoxy(27, 15); std::cout << "                                                                  ";
	gotoxy(27, 16); std::cout << "                                                                  ";
	gotoxy(27, 17); std::cout << "                                                                  ";
	gotoxy(27, 18); std::cout << "                                                                  ";
	gotoxy(27, 19); std::cout << "                                                                  ";
}

// [EDIT]::lecturer //==========================================================================================================================//

void nplecturer::edit(const char* lecturer_id) {
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, ON);
	if (lecturer == nullptr) return;

	gotoxy(46, 19, 128); std::cout << " Save change ";
	gotoxy(60, 19, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << "                                                         ";
	gotoxy(32, 15, 143); std::cout << "                                                         ";
	gotoxy(33, 14, 143); std::cout << "Firstname    : "; std::cout << lecturer->pdata[3];
	gotoxy(33, 15, 143); std::cout << "Lastname     : "; std::cout << lecturer->pdata[2];

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
