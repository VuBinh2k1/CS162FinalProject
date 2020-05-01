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

		int cur = -1;
		while (course != nullptr) {
			char* courseid = course->pdata[1];
			char* coursecs = course->pdata[3];
			if (npcourse::info(courseid, coursecs, 33, 8, 1)) {
				std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
				csv_file process(propath.c_str(), def_process);
				for (int i = 0; i < process.count; ++i) {
					int y = 10 + (++cur);
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
					if (student->pdata[0][0] == '0') { gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << " Hide "; }
					else { gotoxy(87, y, COLOR_CODE + COLOR_GREEN); std::cout << "Public"; }
				}
			}
			course = npcourse::find(course + 1, course_list.data + course_list.count, courseid, 1);
		}
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
	int choose = 0;

	course_layout.print();
	academicmark();

	while ((course = npcourse::choose(my_course, choose)) != nullptr) {
		minibox_layout.print();
		gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << "  No.  | Student ID   | Class     | Mid | Lab |Bonus|Final| Status ";
		int cur = -1;
		while (course != nullptr) {
			char* courseid = course->pdata[2];
			char* coursecs = course->pdata[3];
			if (npcourse::info(courseid, coursecs, 33, 8, 1)) {
				std::string propath = COURSE_PATH("process\\") + courseid + "_" + coursecs + ".csv";
				csv_file process(propath.c_str(), def_process);
				for (int i = 0; i < process.count; ++i) {
					int y = 10 + (++cur);
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
					if (student->pdata[0][0] == '0') { gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << " Hide "; }
					else { gotoxy(87, y, COLOR_CODE + COLOR_GREEN); std::cout << "Public"; }
				}
			}
			course = npcourse::find(course + 1, my_course.data + my_course.count, course->pdata[2]);
		}
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
