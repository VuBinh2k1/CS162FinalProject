#include "..\\controls\\scoreboard.h"

int npscoreboard::staff(const char* course_id, const char* course_cs) {
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
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    " << "  Student list   " << "  Attendance     ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Course   | Class    | Student ID | Mid | Lab |Bonus|Final| Status ";

	int choose = 0, cur = -1;
	while ((cur = -1)) {
		csv_file process(PROCESS(course_id, course_cs), def_process);
		for (int i = 0; i < process.count; ++i) {
			csv_line* student = &process.data[i];
			if (file::find(__STUDENT, student->pdata[1], nullptr, ON) == -1) continue;

			int y = 10 + (++cur) - choose;
			if (y < 10 || y > 27) continue;
			
			WORD COLOR_CODE = (cur % 2 ? 112 : 240);
			gotoxy(27, y, COLOR_CODE); std::cout << "          |          |            |     |     |     |     |        ";
			gotoxy(28, y, COLOR_CODE); std::cout << course_id;
			gotoxy(39, y, COLOR_CODE); std::cout << course_cs;
			gotoxy(50, y, COLOR_CODE); std::cout << student->pdata[1];
			gotoxy(63, y, COLOR_CODE); std::cout << student->pdata[2];
			gotoxy(69, y, COLOR_CODE); std::cout << student->pdata[3];
			gotoxy(75, y, COLOR_CODE); std::cout << student->pdata[4];
			gotoxy(81, y, COLOR_CODE); std::cout << student->pdata[5];
			if (student->pdata[0][0] == '0') { gotoxy(86, y, COLOR_CODE + COLOR_RED); std::cout << "Private"; }
			else { gotoxy(86, y, COLOR_CODE + COLOR_GREEN); std::cout << " Public"; }
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
			if (path.back() != '\\') path += '\\'; path += ACADEMICYEAR + '-' + SEMESTER + '-' + course_id + '_' + course_cs + ".csv";

			std::ofstream out(path);
			if (!out.is_open()) {
				gotoxy(33, 17, 132); std::cout << "Can't open file!";
				goto SAVE_AS;
			}

			out << "No,Student ID,Midterm,Lab,Bonus,Final,GPA,GRADE\n";

			int cnt = -1;
			csv_file process(PROCESS(course_id, course_cs), def_process);
			for (int i = 0; i < process.count; ++i) {
				csv_line* student = &process.data[i];
				if (file::find(__STUDENT, student->pdata[1], nullptr, ON) == -1) continue;

				out << ++cnt << ',' << student->pdata[1] << ',';
				out << student->pdata[2] << ',' << student->pdata[3] << ',' << student->pdata[4] << ',' << student->pdata[5] << ',';
				out << US_GPA(student->pdata[5]) << ',' << US_Grade(student->pdata[5]) << '\n';
			}

			out.close();
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			gotoxy(33, 17, 129); std::cout << "Export to ";
			colorizing(134); std::cout << ACADEMICYEAR << '-' << SEMESTER << '-' << course_id << '_' << course_cs << ".csv";
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
			else goto UN_CHANGE;
			continue;
		}
		goto UN_CHANGE;
	}
	return 0;
}

int npscoreboard::lecturer(const char* course_id, const char* course_cs) {
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
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    " << "  Student list   " << "  Attendance     ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Course   | Class    | Student ID | Mid | Lab |Bonus|Final| Status ";


	int choose = 0, cur = -1, overflow = 0, editrow = 0;
	while ((cur = -1)) {
		csv_file process(PROCESS(course_id, course_cs), def_process);
		for (int i = 0; i < process.count; ++i) {
			csv_line* student = &process.data[i];
			if (file::find(__STUDENT, student->pdata[1], nullptr, ON) == -1) continue;

			int y = 10 + (++cur) + overflow;
			if (y < 10 || y > 27) continue;
			
			WORD COLOR_CODE = (cur % 2 ? 112 : 240);
			if (choose == cur) COLOR_CODE = 176, editrow = i;

			gotoxy(27, y, COLOR_CODE); std::cout << "          |          |            |     |     |     |     |        ";
			gotoxy(28, y, COLOR_CODE); std::cout << course_id;
			gotoxy(39, y, COLOR_CODE); std::cout << course_cs;
			gotoxy(50, y, COLOR_CODE); std::cout << student->pdata[1];
			gotoxy(63, y, COLOR_CODE); std::cout << student->pdata[2];
			gotoxy(69, y, COLOR_CODE); std::cout << student->pdata[3];
			gotoxy(75, y, COLOR_CODE); std::cout << student->pdata[4];
			gotoxy(81, y, COLOR_CODE); std::cout << student->pdata[5];
			if (student->pdata[0][0] == '0') { gotoxy(86, y, COLOR_CODE + COLOR_RED); std::cout << "Private"; }
			else { gotoxy(86, y, COLOR_CODE + COLOR_GREEN); std::cout << " Public"; }
		}
	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " Open       Ctrl+O  ";
			gotoxy(78, 9, 128); std::cout << " Edit       E, e    ";
			gotoxy(78,10, 128); std::cout << "                    ";
			getch();
			gotoxy(78, 8); std::cout << "                    ";
			gotoxy(78, 9); std::cout << "                    ";
			gotoxy(78,10); std::cout << "                    ";
			goto LAYOUT;
		}
		if (KEY_EDIT(c)) {
			csv_file process(PROCESS(course_id, course_cs));
			csv_line* student = &process.data[editrow];
			std::string mid, lab, bonus, final; int status = student->pdata[0][0] == '1';
			int y = 10 + choose + overflow;
			WORD COLOR_CODE = 176;
			gotoxy(63, y, COLOR_CODE + 8); std::cout << student->pdata[2];
			gotoxy(69, y, COLOR_CODE + 8); std::cout << student->pdata[3];
			gotoxy(75, y, COLOR_CODE + 8); std::cout << student->pdata[4];
			gotoxy(81, y, COLOR_CODE + 8); std::cout << student->pdata[5];
			if (read(63, y, COLOR_CODE, mid, 4, SHOW, student->pdata[2]) == KEY_ESC) continue;
			if (read(69, y, COLOR_CODE, lab, 4, SHOW, student->pdata[3]) == KEY_ESC) continue;
			if (read(75, y, COLOR_CODE, bonus, 4, SHOW, student->pdata[4]) == KEY_ESC) continue;
			if (read(81, y, COLOR_CODE, final, 4, SHOW, student->pdata[5]) == KEY_ESC) continue;
			while (1) {
				if (status) { gotoxy(86, y, COLOR_GREEN_BACKGROUND); std::cout << " Public "; }
				else { gotoxy(86, y, COLOR_RED_BACKGROUND); std::cout << "Private "; }

				uint8_t c = getch();
				if (c == KEY_ESC) { status = -1; break; }
				if (c == KEY_ENTER) break;
				if (c == 224 || c == 0) {
					c = getch();
					if (c == KEY_LEFT || c == KEY_RIGHT) {
						status = !status;
					}
				}
			}

			if (status == -1) continue;
			// Update scoreboard
			
			if (mid.size()) file::update(PROCESS(course_id, course_cs), student->id, 2, mid.c_str());
			if (lab.size()) file::update(PROCESS(course_id, course_cs), student->id, 3, lab.c_str());
			if (bonus.size()) file::update(PROCESS(course_id, course_cs), student->id, 4, bonus.c_str());
			if (final.size()) file::update(PROCESS(course_id, course_cs), student->id, 5, final.c_str());
			file::update(PROCESS(course_id, course_cs), student->id, 0, (status) ? "1" : "0");
			continue;
		}
		if (c == KEY_OPEN) {
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Open                                                    ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";

		OPEN:
			gotoxy(32, 16, 128); std::cout << " File:                                                   ";
			
			std::string path;
			if (type(39, 16, 128, path, 48) == KEY_ESC) {
				// Clear import file box
				gotoxy(32, 15); std::cout << "                                                         ";
				gotoxy(32, 16); std::cout << "                                                         ";
				gotoxy(32, 17); std::cout << "                                                         ";
				continue;
			}
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			if (!file::exists(path.c_str())) {
				gotoxy(33, 17, 132); std::cout << "Can't open file!";
				goto OPEN;
			}

			// Check title:
			csv_file imfile(path.c_str());
			if (file::find(imfile, 0, "Student ID") == nullptr) { gotoxy(33, 17, 132); std::cout << "Can't find \"Student ID\""; goto OPEN; }
			csv_file process(PROCESS(course_id, course_cs), def_process);

			for (int i = 0; i < imfile.count; ++i) {
				const char* studentID = file::find(imfile, i, "Student ID");
				if (file::find(__STUDENT, studentID, nullptr, ON) == -1) continue;
				csv_line* student = file::find(process, studentID, nullptr, OFF);

				// Add new student
				std::ofstream app;
				if (student == nullptr) { app.open(PROCESS(course_id, course_cs), std::ios::app); app << "0," << studentID << ','; }

				const char* score = nullptr;
				// Midterm score
				score = file::find(imfile, i, "Mid");
				if (score == nullptr) score = file::find(imfile, i, "Midterm");
				if (score == nullptr) score = file::find(imfile, i, "Mid-term");
				if (score != nullptr) {
					if (student == nullptr) app << score << ',';
					else file::update(PROCESS(course_id, course_cs), student->id, 2, score);
				}
				else if (student == nullptr) app << ',';
				// Lab score
				if ((score = file::find(imfile, i, "Lab")) != nullptr) {
					if (student == nullptr) app << score << ',';
					else file::update(PROCESS(course_id, course_cs), student->id, 3, score);
				}
				else if (student == nullptr) app << ',';
				// Bonus score
				if ((score = file::find(imfile, i, "Bonus")) != nullptr) {
					if (student == nullptr) app << score << ',';
					else file::update(PROCESS(course_id, course_cs), student->id, 4, score);
				}
				else if (student == nullptr) app << ',';
				// Final score
				if ((score = file::find(imfile, i, "Final")) != nullptr) {
					if (student == nullptr) app << score << ',';
					else file::update(PROCESS(course_id, course_cs), student->id, 5, score);
				}
				else if (student == nullptr) app << ',';

				// Fill checkin region
				if (student == nullptr) { for (int i = 6; i < process.mark.count - 1; ++i) app << "0,"; app << "0\n"; }

				gotoxy(32, 17, 128); std::cout << "                                                         ";
				gotoxy(33, 17, 129); std::cout << "Success add "; colorizing(134); std::cout << studentID;
				Sleep(500);
			}
	
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			gotoxy(33, 17, 129); std::cout << "Import file success."; 
			PAUSE;
			// Clear import file box
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			continue;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_LEFT) return 1;
			else goto UN_CHANGE;
			continue;
		}
		goto UN_CHANGE;
	}
	return 0;
}

int npscoreboard::student(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	minibox_layout.print();
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    " << "  Calendar       " << "  Attendance     ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " Semester    | Course ID       | Mid | Lab |Bonus|Final| GPA |GRADE";

	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);

	int choose = 0;
	while (1) {
		for (int i = choose; i < min(choose + 18, my_course.count); ++i) {
			int y = 10 + i - choose;
			csv_line* course = &my_course.data[i];
			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			gotoxy(27, y, COLOR_CODE); std::cout << "             |                 |     |     |     |     |     |     ";
			gotoxy(27, y, COLOR_CODE); std::cout << course->pdata[0] << " - ";
			if (course->pdata[1][0] == '1') std::cout << "Spring";
			if (course->pdata[1][0] == '2') std::cout << "Summer";
			if (course->pdata[1][0] == '3') std::cout << "Autumn";
			if (course->pdata[1][0] == '4') std::cout << "Winter";
			gotoxy(42, y, COLOR_CODE); std::cout << course->pdata[2];

			std::string propath = (std::string)".\\data\\course\\" + course->pdata[0] + "-" + course->pdata[1] + "\\process\\" + course->pdata[2] + "_" + course->pdata[3] + ".csv";
			if (!file::exists(propath.c_str())) { gotoxy(51, y, COLOR_CODE + 8); std::cout << "(hide)"; continue; }
			csv_file process(propath.c_str(), def_process);
			csv_line* mycou = nullptr;
			if ((mycou = file::find(process, user.pdata[1], nullptr, OFF)) == nullptr) { gotoxy(51, y, COLOR_CODE + 8); std::cout << "(hide)"; continue; }
			if ((mycou->pdata[2][0] == '\0')) continue;
			gotoxy(60, y, COLOR_CODE); std::cout << mycou->pdata[2];	// Mid-term
			gotoxy(66, y, COLOR_CODE); std::cout << mycou->pdata[3];	// Lab
			gotoxy(72, y, COLOR_CODE); std::cout << mycou->pdata[4];	// Bonus
			gotoxy(78, y, COLOR_CODE); std::cout << mycou->pdata[5];	// Final
			gotoxy(84, y, COLOR_CODE); std::cout << US_GPA(mycou->pdata[5]);
			gotoxy(90, y, COLOR_CODE); std::cout << US_Grade(mycou->pdata[5]);
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < my_course.count - 18) choose++;
			else if (c == KEY_LEFT) return -1;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	return 0;
}
