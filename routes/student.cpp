#include "student.h"

void npstudent::menu(csv_line& user) {

}

void npstudent::check_in(csv_line& user) {
	system("cls");

	std::ifstream inp(".\\layout\\student_schedule.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("student_schedule.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout student_schedule(inp);
	inp.close();

	int id = 0;
	char* username = user.pdata[1];
	char* course_id = nullptr;
	char* course_cs = nullptr;
	std::string stupath = (std::string)".\\data\\student\\" + username + ".csv";
	csv_file my_course(stupath.c_str());

	CHECK_IN:	// CHECK_IN lable:
	student_schedule.print();
	// Choose Up-down: [COURSE]
	for (int choose = 0, u = -1;; u = -1) {
		for (int i = 0; i < my_course.count; ++i) {
			if (strcmp(my_course.data[i].pdata[0], "0") == 0) continue;		// Status: 0/1
			if (choose == ++u) {
				id = i;
				gotoxy(7, 10 + u, COLOR_WHITE_BACKGROUND);
			}
			else {
				gotoxy(7, 10 + u, COLOR_WHITE);
			}
			std::cout << my_course.data[i].pdata[1];
		}
		colorizing(COLOR_DEFAULT);

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) {
			course_id = my_course.data[id].pdata[1];
			course_cs = my_course.data[id].pdata[2];
			break;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			if (c == KEY_DOWN && choose < u) choose++;
		}
	}
	
	// Print Course Info:
	if (npcourse::show(course_id, course_cs, 33, 9) == 0) return;
	
	// Load SCHEDULE: COURSE_CLASS.csv or make __dafault
	std::string schpath = (std::string)".\\data\\course\\SCHEDULE\\" + course_id + "_" + course_cs + ".csv";
	if (!exists(schpath.c_str())) {
		// If .\\SCHEDULE\\COURSEID_COURSECLASS.csv does not exist

	}
	csv_file schedule(schpath.c_str());
	
	for (int i = 0; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		csv_line* mysch = &my_course.data[id];
		if (strcmp(date->pdata[1], "0") == 0) continue;
		int now = nptime::now(date->pdata[1], date->pdata[2], date->pdata[3]);

		if (now == 0) {
			if (strcmp(mysch->pdata[3 + i], "1") == 0) {
				gotoxy(29, 17, COLOR_GREEN); std::cout << "You have already checked in this course.";
				PAUSE; goto CHECK_IN;		// Check in another course
			}

			// Choose Left-right: [Checkin][Cancel]
			int choose = 0;
			while(1) {
				gotoxy(38, 17, (choose == 0) ? COLOR_GREEN_BACKGROUND : COLOR_GREEN); std::cout << "[Checkin]";
				gotoxy(49, 17, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Cancel]";
				colorizing(COLOR_DEFAULT);

				uint8_t c = getch();
				if (c == KEY_ESC) { choose = 1; break; }
				if (c == KEY_ENTER) break;
				if (c == 224 || c == 0) {
					if (choose == 0 && getch() == KEY_RIGHT ||
						choose == 1 && getch() == KEY_LEFT) {
						choose = !choose;
					}
				}
			}

			if (choose == 1) {
				gotoxy(25, 17, COLOR_YELLOW); std::cout << "You will miss class if you don't take attendance.";
				PAUSE; goto CHECK_IN;
			}
			else {
				csv::update(stupath.c_str(), mysch->id, 3 + i, (std::string)"1");
			}
		}
		else if (now == -1) {

		}
	}

	gotoxy(0, 30);
}
