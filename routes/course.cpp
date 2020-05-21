#include "..\\controls\\course.h"

void npcourse::info(csv_line& user, const char* course_id, const char* course_cs) {
LAYOUT:
	csv_file course_list(__COURSE, def_course);
	csv_line* course = file::find(course_list, course_id, course_cs, OFF);

	if (course == nullptr) {
		gotoxy(32, 15, COLOR_BLUE_BACKGROUND); std::cout << " Course info                                             ";
		gotoxy(32, 16, 128); std::cout << "                                                         ";
		gotoxy(33, 16, 132); std::cout << "This course does not exist";
		return;
	}

	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " Course info                                             ";
	gotoxy(32, 14, 128); std::cout << "                                                         ";
	gotoxy(32, 15, 128); std::cout << "                                                         ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "                                                         ";
	gotoxy(32, 20, 128); std::cout << "                                                         ";
	gotoxy(32, 21, 128); std::cout << "                                                         ";

	gotoxy(33, 14, 143); std::cout << course->pdata[2] << " - " << course->pdata[1] << "(" << course->pdata[3] << ")";
	gotoxy(33, 15, 128); std::cout << "Lecturer   : " << course->pdata[4];
	gotoxy(33, 16, 128); std::cout << "Timeline   : " << course->pdata[5] << " to " << course->pdata[6];
	gotoxy(33, 17, 128); std::cout << "Weekdays   : " << course->pdata[7] << " (" << course->pdata[8] << " - " << course->pdata[9] << ")";
	gotoxy(33, 18, 128); std::cout << "Room       : " << course->pdata[10];
	gotoxy(33, 19, 128); std::cout << "Self enrol : ";
	if (course->pdata[0][0] == '1') { colorizing(128 + COLOR_BLUE); std::cout << "Student can self enrol"; }
	else { colorizing(128 + COLOR_RED); std::cout << "Student can't self enrol"; }

	if (user == "staff") {
		for (int choose = 0;;) {
			gotoxy(51, 21, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "  Edit  ";
			gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Remove ";

			uint8_t c = getch();
			if (c == KEY_ESC) break;
			if (c == KEY_ENTER) {
				if (choose == 0) {
					npcourse::edit(course_id, course_cs);
					goto LAYOUT;
				}
				if (choose == 1) {
					if (npcourse::remove(course_id, course_cs)) goto END;
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
	if (user == "lecturer") {
		gotoxy(54, 21, COLOR_WHITE_BACKGROUND); std::cout << "  Schedule  ";
		uint8_t c = getch();
		while (c != KEY_ESC && c != KEY_ENTER) c = getch();
		if (c == KEY_ESC) goto END;
		npcourse::schedule(course_id, course_cs);
		goto LAYOUT;
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

void npcourse::enrol(csv_line& user, const char* course_id, const char* course_cs) {
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Enrol                                                   ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	std::string studentid;

ENROL_DATA:
	gotoxy(32, 16, 128); std::cout << " Student ID:                                             ";
	gotoxy(32, 17, 128); std::cout << " Course ID :               Class:                        ";
	gotoxy(45, 17, 128); std::cout << course_id;
	gotoxy(66, 17, 128); std::cout << course_cs;
	if (user == "student") studentid = user.pdata[1]; else if (read(45, 16, 128, studentid, 8, SHOW) == KEY_ESC) goto END;
	gotoxy(45, 16, 128); std::cout << studentid;
	gotoxy(32, 18, 128); std::cout << "                                                         ";

	// Check student exist "__student.csv"
	if (file::find(__STUDENT, studentid.c_str(), nullptr, ON) == -1) {
		gotoxy(33, 18, 132); std::cout << "Can't find Student ID: "; colorizing(134); std::cout << studentid;
		goto ENROL_DATA;

	} {// Check data of course "__course.csv"
		csv_file course_list(__COURSE, def_course);
		for (int i = 0; i < course_list.count; ++i) {
			if (strcmp(course_id, course_list.data[i].pdata[1]) == 0 && strcmp(course_cs, course_list.data[i].pdata[3]) == 0) {
				if (user == "student" && course_list.data[i].pdata[0][0] == '0') {
					gotoxy(33, 18, 132); std::cout << "You can't self enrol in this course!";
					getch(); goto END;
				}
				if (user == "lecturer" && strcmp(course_list.data[i].pdata[4], user.pdata[1])) {
					gotoxy(33, 18, 132); std::cout << "You can't enrol student on this course!";
					goto ENROL_DATA;
				}
			}
		}

	} {// Add course to "studentID.csv"
		csv_file my_course(((std::string)".\\data\\student\\" + studentid + ".csv").c_str(), def_user);
		for (int i = 0; i < my_course.count; ++i) {
			// Had enrol in this course (only enrol in 1 class of a course)
			if (strcmp(course_id, my_course.data[i].pdata[2]) == 0) {
				gotoxy(33, 18, 132); std::cout << "Had enrol in ";
				colorizing(134); std::cout << course_id << " - " << my_course.data[i].pdata[3];
				if (user == "student") { getch(); goto END; }
				goto ENROL_DATA;
			}
		}
		std::ofstream out((std::string)".\\data\\student\\" + studentid + ".csv", std::ios::app);
		out << ACADEMICYEAR << ',' << SEMESTER << ',' << course_id << ',' << course_cs << '\n'; out.close();

	} {// Add student to "courseID_courseCS.csv"
		csv_file process((COURSE_PATH("process\\") + course_id + '_' + course_cs + ".csv").c_str(), def_process);
		for (int i = 0; i < process.count; ++i) {
			// Old data (don't know why it here)
			if (studentid == process.data[i].pdata[1]) goto SUCCESS;
		}
		std::ofstream out(COURSE_PATH("process\\") + course_id + '_' + course_cs + ".csv", std::ios::app);
		out << "0," << studentid << ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"; out.close();
	}
SUCCESS:
	gotoxy(33, 18, 129); std::cout << "Enrol success.";
	if (user == "student") { getch(); goto END; }
	goto ENROL_DATA;
END:
	gotoxy(32, 15); std::cout << "                                                         ";
	gotoxy(32, 16); std::cout << "                                                         ";
	gotoxy(32, 17); std::cout << "                                                         ";
	gotoxy(32, 18); std::cout << "                                                         ";
}

void npcourse::chkin(csv_line& user, const char* course_id, const char* course_cs) {
	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str());
	gotoxy(32, 14, COLOR_BLUE_BACKGROUND);  std::cout << " Check in                                                ";
	gotoxy(32, 15, 128); std::cout << " Student ID:                                             ";
	gotoxy(32, 16, 128); std::cout << " Course ID :               Class:                        ";
	gotoxy(32, 17, 128); std::cout << " Date      :                                             ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(45, 15, 128); std::cout << user.pdata[1];
	gotoxy(45, 16, 128); std::cout << course_id;
	gotoxy(66, 16, 128); std::cout << course_cs;

	std::string propath = COURSE_PATH("process\\") + course_id + "_" + course_cs + ".csv";
	std::string schpath = COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv";
	csv_file process(propath.c_str(), def_process);
	csv_file schedule(schpath.c_str(), def_schedule, course_id, course_cs);

	csv_line* mycou = nullptr;
	if ((mycou = file::find(process, user.pdata[1], nullptr, OFF)) == nullptr) {
		gotoxy(33, 18, 128 + COLOR_RED); std::cout << "You have not enrolled in this course.";
		goto END;
	}
	for (int i = 0; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];
		//if (strcmp(date->pdata[1], "0") == 0) npcourse::mksch(course_id, course_cs);	//////////////////////

		int now = control::now(date->pdata[1], date->pdata[2], date->pdata[3]);
		if (now == 0) {
			gotoxy(45, 17, 128); std::cout << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";

			if (mycou->pdata[WEEK_COLUMN + i][0] == '0') {
				gotoxy(57, 18, COLOR_BLUE_BACKGROUND); std::cout << "[Check in]";
				if (getch() != KEY_ENTER) {
					gotoxy(33, 18, 128 + COLOR_YELLOW); std::cout << "You will miss class if you don't take attendance.";
					goto END;
				}

				mycou->pdata[WEEK_COLUMN + i][0] = '1';
				file::update(propath.c_str(), mycou->id, WEEK_COLUMN + i, "1");
			}

			gotoxy(33, 18, 128 + COLOR_BLUE); std::cout << "You have already checked in this course.";
			goto END;
		}
		else if (now == -1) {
			// The nearest day the course will start
			gotoxy(45, 17, 128); std::cout << date->pdata[1] << " (" << date->pdata[2] << " - " << date->pdata[3] << ")";
			gotoxy(33, 18, 128); std::cout << "This course has not started.";
			goto END;
		}
	}
	// The last day of the course has ended
	gotoxy(33, 18, 128); std::cout << "The course has ended.";
END:
	getch();
	gotoxy(32, 14); std::cout << "                                                         ";
	gotoxy(32, 15); std::cout << "                                                         ";
	gotoxy(32, 16); std::cout << "                                                         ";
	gotoxy(32, 17); std::cout << "                                                         ";
	gotoxy(32, 18); std::cout << "                                                         ";
}

// [EDIT]::course //============================================================================================================================//

bool npcourse::now(const char* course_id, const char* course_cs, std::tm day) {
	csv_file schedule((COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv").c_str(), def_schedule, course_id, course_cs);
	for (int i = 0; i < schedule.count; ++i) {
		if (control::now(day, schedule.data[i].pdata[1]) == 0) return 1;
	}
	return 0;
}

void npcourse::edit(const char* course_id, const char* course_cs) {
	csv_file course_list(__COURSE, def_course);
	csv_line* course = file::find(course_list, course_id, course_cs, OFF);
	if (course == nullptr) return;

	gotoxy(46, 21, 128); std::cout << " Save change ";
	gotoxy(60, 21, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << "                                                         ";
	std::string cname, lectu, sdate, fdate, wdays, stime, ftime, iroom;

	if (read(33, 14, 143, cname, 30, SHOW, course->pdata[2]) == KEY_ESC) return; std::cout << " - " << course->pdata[1] << "(" << course->pdata[3] << ")";
	if (read(46, 15, 128, lectu, 42, SHOW, course->pdata[4]) == KEY_ESC) return;
	while (sdate != "1" && sdate.size() != 10) if (date(46, 16, 128, sdate) == KEY_ESC) return;
	if (sdate.size() != 10) { gotoxy(46, 16, 128); std::cout << course->pdata[5]; } std::cout << " to ";
	while (fdate != "1" && fdate.size() != 10) if (date(60, 16, 128, fdate) == KEY_ESC) return;
	if (fdate.size() != 10) { gotoxy(60, 16, 128); std::cout << course->pdata[6]; }

	while (1) {
		if (read(46, 17, 128, wdays, 3, SHOW, course->pdata[7]) == KEY_ESC) return;
		if (wdays.size()) wdays[0] = toupper(wdays[0]);
		else { std::cout << course->pdata[7]; break; }
		if (wdays == "Mon" || wdays == "Tue" || wdays == "Wed" || wdays == "Thu" || wdays == "Fri" || wdays == "Sat" || wdays == "Sun") break;
	}

	std::cout << " (";
	while (stime != "1" && stime.size() != 5) if (time(51, 17, 128, stime) == KEY_ESC) return;
	if (stime.size() != 5) { gotoxy(51, 17, 128); std::cout << course->pdata[8]; } std::cout << " - ";
	while (ftime != "1" && ftime.size() != 5) if (time(59, 17, 128, ftime) == KEY_ESC) return;
	if (ftime.size() != 5) { gotoxy(59, 17, 128); std::cout << course->pdata[9]; } std::cout << ")";

	if (read(46, 18, 128, iroom, 5, SHOW, course->pdata[10]) == KEY_ESC) return;
	std::transform(iroom.begin(), iroom.end(), iroom.begin(), ::toupper);

	gotoxy(32, 19, 128); std::cout << "                                                         ";
	int status = (course->pdata[0][0] == '1');
	while (1) {
		gotoxy(33, 19, 128); std::cout << "Self enrol : ";
		if (status) { colorizing(128 + COLOR_BLUE); std::cout << "enable "; }
		else { colorizing(128 + COLOR_RED); std::cout << "disable"; }

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) break;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT || c == KEY_RIGHT) {
				status = !status;
			}
		}
	}

	// Save
	for (int choose = 0;;) {
		gotoxy(46, 21, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Save change ";
		gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "   Cancel    ";

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				// Update: __course.csv
				
				file::update(__COURSE, course->id, 0, (status ? "1" : "0"));
				if (cname.size()) file::update(__COURSE, course->id, 2, cname.c_str());
				if (lectu.size()) file::update(__COURSE, course->id, 4, lectu.c_str());
				if (wdays.size()) file::update(__COURSE, course->id, 7, wdays.c_str());
				if (iroom.size()) file::update(__COURSE, course->id, 10, iroom.c_str());
				if (sdate.size() ==10) file::update(__COURSE, course->id, 5, sdate.c_str());
				if (fdate.size() ==10) file::update(__COURSE, course->id, 6, fdate.c_str());
				if (stime.size() == 5) file::update(__COURSE, course->id, 8, stime.c_str());
				if (ftime.size() == 5) file::update(__COURSE, course->id, 9, ftime.c_str());

				gotoxy(46, 21, 128 + COLOR_BLUE); std::cout << " Save changes successfully.";
				PAUSE; break;
			}
			break;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0) choose++;
		}
	}
}

int npcourse::remove(const char* course_id, const char* course_cs) {
	csv_file course_list(__COURSE, def_course);
	csv_line* course = file::find(course_list, course_id, course_cs, OFF);
	if (course == nullptr) return 0;

	gotoxy(33, 20, 128 + COLOR_RED); std::cout << "Are you sure to remove this course, cannot be undone.";
	for (int choose = 1;;) {
		gotoxy(51, 21, (choose == 0) ? COLOR_RED_BACKGROUND : 128); std::cout << " Remove ";
		gotoxy(60, 21, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Cancel ";

		uint8_t c = getch();
		csv_line* student = nullptr;
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				file::remove(__COURSE, course->id);
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

void npcourse::schedule(const char* course_id, const char* course_cs) {
	if (file::find(__COURSE, course_id, course_cs, OFF) == -1) return;
	gotoxy(32, 17, COLOR_BLUE_BACKGROUND); std::cout << " Schedule                                                ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "_________________________________________________________";

	int week = 0;
	while (1) {
	SCHEDULE:
		csv_file schedule((COURSE_PATH("schedule\\") + course_id + '_' + course_cs + ".csv").c_str(), def_schedule, course_id, course_cs);
		csv_line* sche = &schedule.data[week];

		gotoxy(32, 18, 128); std::cout << "                                                         ";
		gotoxy(33, 18, 128); std::cout << sche->pdata[0] << "    : " << sche->pdata[1];
		gotoxy(57, 18, 128); std::cout << "(" << sche->pdata[2];

		gotoxy(64, 18, 128); std::cout << "- " << sche->pdata[3] << ")";
		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (KEY_EDIT(c)) {
			std::string sdate, stime, ftime;
			while (sdate != "1" && sdate.size() != 10) if (date(46, 18, 128, sdate) == KEY_ESC) goto SCHEDULE;
			if (sdate.size() != 10) { gotoxy(46, 18, 128); std::cout << sche->pdata[1]; }

			gotoxy(57, 18, 128); std::cout << "(";
			while (stime != "1" && stime.size() != 5) if (time(58, 18, 128, stime) == KEY_ESC) goto SCHEDULE;
			if (stime.size() != 5) { gotoxy(58, 18, 128); std::cout << sche->pdata[2]; }

			gotoxy(64, 18, 128); std::cout << "- ";
			while (ftime != "1" && ftime.size() != 5) if (time(66, 18, 128, ftime) == KEY_ESC) goto SCHEDULE;
			if (ftime.size() != 5) { gotoxy(66, 18, 128); std::cout << sche->pdata[3]; } std::cout << ")";

			if (sdate.size() ==10) file::update((COURSE_PATH("schedule\\") + course_id + '_' + course_cs + ".csv").c_str(), sche->id, 1, sdate.c_str());
			if (stime.size() == 5) file::update((COURSE_PATH("schedule\\") + course_id + '_' + course_cs + ".csv").c_str(), sche->id, 2, stime.c_str());
			if (ftime.size() == 5) file::update((COURSE_PATH("schedule\\") + course_id + '_' + course_cs + ".csv").c_str(), sche->id, 3, ftime.c_str());
			colorizing(128 + COLOR_BLUE); std::cout << " success.";
			PAUSE; goto SCHEDULE;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && week > 0) week--;
			else if (c == KEY_DOWN && week < MAX_WEEK - 1) week++;
		}
	}
}