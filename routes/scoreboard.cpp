#include "scoreboard.h"

void npscoreboard::staff() {
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
	int choose = 0;

	course_layout.print();
	academicmark();

	while ((course = npcourse::choose(course_list, choose, 1)) != nullptr) {
		minibox_layout.print();
		gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << "  No.  | Student ID   | Class     | Mid | Lab |Bonus|Final| Status ";
		gotoxy(33, 28); std::cout << "[E]: Export to file";

		csv_line* mycou = nullptr;
		int choose = 0, cur = -1;

	MENU_CHOOSEN:
		// Clear export file box
		gotoxy(32, 15); std::cout << "                                                         ";
		gotoxy(32, 16); std::cout << "                                                         ";
		gotoxy(32, 17); std::cout << "                                                         ";

		mycou = course; cur = -1;
		while (mycou != nullptr) {
			char* courseid = mycou->pdata[1];
			char* coursecs = mycou->pdata[3];
			if (npcourse::info(courseid, coursecs, 33, 8, 1)) {
				std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
				csv_file process(propath.c_str(), def_process);
				for (int i = 0; i < process.count; ++i) {
					int y = 10 + (++cur) - choose;
					if (y < 10 || y > 27) continue;
					csv_line* student = &process.data[i];
					WORD COLOR_CODE = (cur % 2 ? 112 : 240);
					gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |     |     |     |     |        ";
					gotoxy(28, y, COLOR_CODE); std::cout << cur;
					gotoxy(36, y, COLOR_CODE); std::cout << student->pdata[1];
					gotoxy(51, y, COLOR_CODE); std::cout << coursecs;
					gotoxy(63, y, COLOR_CODE); std::cout << student->pdata[2];
					gotoxy(69, y, COLOR_CODE); std::cout << student->pdata[3];
					gotoxy(75, y, COLOR_CODE); std::cout << student->pdata[4];
					gotoxy(81, y, COLOR_CODE); std::cout << student->pdata[5];
					if (student->pdata[0][0] == '0') { gotoxy(86, y, COLOR_CODE + COLOR_RED); std::cout << "Private"; }
					else { gotoxy(86, y, COLOR_CODE + COLOR_GREEN); std::cout << " Public"; }
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
			if (read(39, 16, 128, path, 48, SHOW) == KEY_ESC) goto MENU_CHOOSEN;
			if (path.back() != '\\') path += '\\'; path += ACADEMICYEAR + '-' + SEMESTER + '-' + course->pdata[1] + ".csv";

			std::ofstream out(path);
			if (!out.is_open()) {
				gotoxy(33, 17, 132); std::cout << "can't open file!";
				PAUSE; goto SAVE_AS;
			}

			out << "No,Student ID,Class,Midterm,Lab,Bonus,Final,GPA,GRADE\n";
			mycou = course; int cnt = -1;
			while (mycou != nullptr) {
				char* courseid = mycou->pdata[1];
				char* coursecs = mycou->pdata[3];
				if (npcourse::info(courseid, coursecs, 33, 8, 0)) {
					std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
					csv_file process(propath.c_str(), def_process);
					for (int i = 0; i < process.count; ++i) {
						csv_line* student = &process.data[i];
						out << ++cnt << ',' << student->pdata[1] << ',' << coursecs << ',';
						out << student->pdata[2] << ',' << student->pdata[3] << ',' << student->pdata[4] << ',' << student->pdata[5] << ',';
						std::string fscore = student->pdata[5];
						out << US_GPA(std::stoi(fscore)) << ',' << US_Grade(std::stoi(fscore)) << '\n';
					}
				}
				mycou = npcourse::find(mycou + 1, course_list.data + course_list.count, courseid, 1);
			}
			out.close();
			gotoxy(33, 17, 129); std::cout << "Export file success.";
			PAUSE;
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

void npscoreboard::lecturer(csv_line& user) {
	std::ifstream inp(".\\layout\\course.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("course.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout course_layout(inp);
	layout minibox_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	csv_file my_course(((std::string)".\\data\\lecturer\\" + username + ".csv").c_str(), def_user);
	csv_file course_list((COURSE_PATH("__course.csv").c_str()), def_course);
	csv_line* course = nullptr;
	int chscrs = 0;

	course_layout.print();
	academicmark();

	while ((course = npcourse::choose(my_course, chscrs)) != nullptr) {
		minibox_layout.print();
		gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << "  No.  | Student ID   | Class     | Mid | Lab |Bonus|Final| Status ";
		gotoxy(33, 28); std::cout << "[E]: Edit score";

		csv_line* mycou = nullptr;
		int choose = 0, cur = -1, overflow = 0;
		std::string editpath; int editrow = -1;
		
	MENU_CHOOSEN:
		mycou = course; cur = -1;
		while (mycou != nullptr) {
			char* courseid = mycou->pdata[2];
			char* coursecs = mycou->pdata[3];
			if (npcourse::info(courseid, coursecs, 33, 8, 1)) {
				std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
				csv_file process(propath.c_str(), def_process);
				for (int i = 0; i < process.count; ++i) {
					int y = 10 + (++cur) + overflow;
					if (y < 10 || y > 27) continue;
					csv_line* student = &process.data[i];
					WORD COLOR_CODE = (cur % 2 ? 112 : 240);
					if (choose == cur) COLOR_CODE = 176, editpath = propath, editrow = i;

					gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |     |     |     |     |        ";
					gotoxy(28, y, COLOR_CODE); std::cout << cur;
					gotoxy(36, y, COLOR_CODE); std::cout << student->pdata[1];
					gotoxy(51, y, COLOR_CODE); std::cout << coursecs;
					gotoxy(63, y, COLOR_CODE); std::cout << student->pdata[2];
					gotoxy(69, y, COLOR_CODE); std::cout << student->pdata[3];
					gotoxy(75, y, COLOR_CODE); std::cout << student->pdata[4];
					gotoxy(81, y, COLOR_CODE); std::cout << student->pdata[5];
					if (student->pdata[0][0] == '0') { gotoxy(86, y, COLOR_CODE + COLOR_RED); std::cout << "Private"; }
					else { gotoxy(86, y, COLOR_CODE + COLOR_GREEN); std::cout << " Public"; }
				}
			}
			mycou = npcourse::find(mycou + 1, my_course.data + my_course.count, courseid);
		}

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) continue;
		if (c == KEY_ENTER || c == 'E' || c == 'e') {
			csv_file process(editpath.c_str());
			csv_line* student = &process.data[editrow];
			std::string mid, lab, bonus, final; bool status = student->pdata[0][0] == '1';
			int y = 10 + choose + overflow;
			WORD COLOR_CODE = 176;
			gotoxy(63, y, COLOR_CODE + 8); std::cout << student->pdata[2];
			gotoxy(69, y, COLOR_CODE + 8); std::cout << student->pdata[3];
			gotoxy(75, y, COLOR_CODE + 8); std::cout << student->pdata[4];
			gotoxy(81, y, COLOR_CODE + 8); std::cout << student->pdata[5];
			if (read(63, y, COLOR_CODE, mid,   4, SHOW, student->pdata[2]) == KEY_ESC) goto MENU_CHOOSEN;
			if (read(69, y, COLOR_CODE, lab,   4, SHOW, student->pdata[3]) == KEY_ESC) goto MENU_CHOOSEN;
			if (read(75, y, COLOR_CODE, bonus, 4, SHOW, student->pdata[4]) == KEY_ESC) goto MENU_CHOOSEN;
			if (read(81, y, COLOR_CODE, final, 4, SHOW, student->pdata[5]) == KEY_ESC) goto MENU_CHOOSEN;
			while (1) {
				if (status) { gotoxy(86, y, COLOR_GREEN_BACKGROUND); std::cout << " Public "; }
				else { gotoxy(86, y, COLOR_RED_BACKGROUND); std::cout << "Private "; }

				uint8_t c = getch();
				if (c == KEY_ESC) return;
				if (c == KEY_ENTER) break;
				if (c == 224 || c == 0) {
					if ((c = getch()) == KEY_LEFT || c == KEY_RIGHT) {
						status = !status;
					}
				}
			}

			// Update scoreboard
			file::update(editpath.c_str(), student->id, 2, mid.c_str());
			file::update(editpath.c_str(), student->id, 3, lab.c_str());
			file::update(editpath.c_str(), student->id, 4, bonus.c_str());
			file::update(editpath.c_str(), student->id, 5, final.c_str());
			file::update(editpath.c_str(), student->id, 0, (status) ? "1" : "0");
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

void npscoreboard::student(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	minibox_layout.print();
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                           My Scoreboard                           ";
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << "  Semester   | Course ID       | Mid | Lab |Bonus|Final| GPA |GRADE";
	
	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);

	int choose = 0;
	while (1) {
		int cur = 0;
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

			std::string process = (std::string)".\\data\\course\\" + course->pdata[0] + "-" + course->pdata[1] + "\\process\\" + course->pdata[2] + "_" + course->pdata[3] + ".csv";
			if (!file::exists(process.c_str())) { gotoxy(51, y, COLOR_CODE + 8); std::cout << "(hide)"; continue; }
			csv_file score(process.c_str(), def_process);
			csv_line* mycou = nullptr;
			if ((mycou = file::exists(score, user.pdata[1])) == nullptr) { gotoxy(51, y, COLOR_CODE + 8); std::cout << "(hide)"; continue; }
			gotoxy(60, y, COLOR_CODE); std::cout << mycou->pdata[2];	// Mid-term
			gotoxy(66, y, COLOR_CODE); std::cout << mycou->pdata[3];	// Lab
			gotoxy(72, y, COLOR_CODE); std::cout << mycou->pdata[4];	// Bonus
			gotoxy(78, y, COLOR_CODE); std::cout << mycou->pdata[5];	// Final
			std::string fscore = mycou->pdata[5];
			gotoxy(84, y, COLOR_CODE); std::cout << US_GPA(std::stoi(fscore));
			gotoxy(90, y, COLOR_CODE); std::cout << US_Grade(std::stoi(fscore));
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < my_course.count - 18) choose++;
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
		}
	}
}
