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
	layout student_schedule_layout(inp);
	layout course_background_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	char* course_id = nullptr;
	char* course_cs = nullptr;
	std::string stupath = (std::string)".\\data\\student\\" + username + ".csv";
	csv_file my_course(stupath.c_str());
	student_schedule_layout.print();

	int chs1 = 0, id = -1;

	CHECK_IN:		// Label: CHECK_IN (use goto)
	
	for (int u = -1;; u = -1) {	// Choose Up-down: [COURSE]
		for (int i = 0; i < my_course.count; ++i) {
			if (strcmp(my_course.data[i].pdata[0], "0") == 0) continue;		// Status: 0/1
			if (chs1 == ++u) {
				id = i;
				gotoxy(7, 10 + u, COLOR_WHITE_BACKGROUND);
			}
			else {
				gotoxy(7, 10 + u, COLOR_WHITE);
			}
			std::cout << my_course.data[i].pdata[1];
		}
		colorizing(COLOR_DEFAULT);

		// If dont student dont have any course
		if (u == -1) break;

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) {
			course_id = my_course.data[id].pdata[1];
			course_cs = my_course.data[id].pdata[2];
			break;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && chs1 > 0) chs1--;
			if (c == KEY_DOWN && chs1 < u) chs1++;
		}
	}
	
	// If dont student dont have any course
	if (id == -1) {
		gotoxy(7, 10, 8); std::wcout << "(empty)";
		uint8_t c = getch();
		while (c != KEY_ENTER && c != KEY_ESC) c = getch();
		colorizing(COLOR_DEFAULT); return;
	}

	// Print Course Info:
	course_background_layout.print();
	if (npcourse::show(course_id, course_cs, 33, 9) == 0) {
		PAUSE; return;
	}
	
	// Load SCHEDULE: COURSE_CLASS.csv or make __dafault
	std::string schpath = (std::string)".\\data\\course\\SCHEDULE\\" + course_id + "_" + course_cs + ".csv";
	if (!exists(schpath.c_str())) {
		// If .\\SCHEDULE\\COURSEID_COURSECLASS.csv does not exist
		goto CHECK_IN;
	}
	csv_file schedule(schpath.c_str());
	
	for (int i = 0; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		csv_line* mysch = &my_course.data[id];

		if (strcmp(date->pdata[1], "0") == 0) continue;
		int now = nptime::now(date->pdata[1], date->pdata[2], date->pdata[3]);

		if (now == 0) {
			gotoxy(33, 19); std::cout << "Date        : " << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";

			if (strcmp(mysch->pdata[3 + i], "1") != 0) {
				
				int chs = 0;
				while (1) {	// Choose Left-right: [Checkin][Cancel]
					gotoxy(39, 27, (chs == 0) ? COLOR_GREEN_BACKGROUND : COLOR_WHITE); std::cout << "[Checkin]";
					gotoxy(49, 27, (chs == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Cancel]";
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
					gotoxy(30, 27, COLOR_YELLOW); std::cout << "You will miss class if you don't take attendance.";
					goto CHECK_IN;
				}

				mysch->pdata[3 + i][0] = '1';
				npcsv::update(stupath.c_str(), mysch->id, 3 + i, "1");
			}
			
			gotoxy(34, 27, COLOR_GREEN); std::cout << "You have already checked in this course.";
			goto CHECK_IN;
		}
		else if (now == -1) {
			// The nearest day the course will start
			gotoxy(33, 19); std::cout << "Date        : " << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";
			gotoxy(40, 27, COLOR_WHITE); std::cout <<  "This course has not started.";
			goto CHECK_IN;
		}
	}
	// The last day of the course has ended
	gotoxy(43, 27, COLOR_WHITE); std::cout << "The course has ended.";
	goto CHECK_IN;
}
