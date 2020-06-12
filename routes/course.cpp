#include "..\\controls\\course.h"

void npcourse::info(csv_line& user, const char* course_id, const char* course_cs, bool _DELETE) {
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
	gotoxy(32, 14, 143); std::cout << "                                                         ";
	gotoxy(32, 15, 128); std::cout << " Lecturer   :                                            ";
	gotoxy(32, 16, 128); std::cout << " Timeline   :                                            ";
	gotoxy(32, 17, 128); std::cout << " Weekdays   :                                            ";
	gotoxy(32, 18, 128); std::cout << " Room       :                                            ";
	gotoxy(32, 19, 128); std::cout << " Self enrol :                                            ";
	gotoxy(32, 20, 128); std::cout << "                                                         ";
	gotoxy(32, 21, 128); std::cout << "                                                         ";

	gotoxy(33, 14, 143); std::cout << course->pdata[2] << " - " << course->pdata[1] << "(" << course->pdata[3] << ")";
	gotoxy(46, 15, 128); std::cout << course->pdata[4];
	gotoxy(46, 16, 128); std::cout << course->pdata[5] << " to " << course->pdata[6];
	gotoxy(46, 17, 128); std::cout << course->pdata[7] << " (" << course->pdata[8] << " - " << course->pdata[9] << ")";
	gotoxy(46, 18, 128); std::cout << course->pdata[10];
	gotoxy(46, 19, 128);
	if (course->pdata[0][0] == '1') { colorizing(128 + COLOR_BLUE); std::cout << "Student can self enrol"; }
	else { colorizing(128 + COLOR_RED); std::cout << "Student can't self enrol"; }

	if (user == "staff") {
		if (_DELETE == ON) { npcourse::remove(course_id, course_cs); return; }
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
					if (npcourse::remove(course_id, course_cs)) return;
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
		if (c == KEY_ESC) return;
		npcourse::schedule(course_id, course_cs);
		goto LAYOUT;
	}
}

void npcourse::enrol(csv_line& user, const char* course_id, const char* course_cs) {
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Enrol                                                   ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	std::string studentid;

ENROL_DATA:
	gotoxy(32, 16, 143); std::cout << " Student ID  :                                           ";
	gotoxy(32, 17, 128); std::cout << " Course ID   :               Class:                      ";
	gotoxy(47, 17, 128); std::cout << course_id;
	gotoxy(68, 17, 128); std::cout << course_cs;
	if (user == "student") studentid = user.pdata[1]; else if (read(47, 16, 143, studentid, 8, SHOW) == KEY_ESC) return;
	gotoxy(47, 16, 128); std::cout << studentid;
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
					getch(); return;
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
				if (user == "student") { getch(); return; }
				goto ENROL_DATA;
			}
		}
		std::ofstream out((std::string)".\\data\\student\\" + studentid + ".csv", std::ios::app);
		out << ACADEMICYEAR << ',' << SEMESTER << ',' << course_id << ',' << course_cs << '\n'; out.close();

	} {// Add student to "courseID_courseCS.csv"
		csv_file process(PROCESS(course_id, course_cs), def_process);
		for (int i = 0; i < process.count; ++i) {
			// Old data (don't know why it here)
			if (studentid == process.data[i].pdata[1]) goto SUCCESS;
		}
		std::ofstream out(PROCESS(course_id, course_cs), std::ios::app);
		out << "0," << studentid << ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"; out.close();
	}
SUCCESS:
	gotoxy(33, 18, 129); std::cout << "Enrol success.";
	if (user == "student") { getch(); return; }
	goto ENROL_DATA;
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

	csv_file process(PROCESS(course_id, course_cs), def_process);
	csv_file schedule(SCHEDULE(course_id, course_cs), course_id, course_cs);

	csv_line* mycou = nullptr;
	if ((mycou = file::find(process, user.pdata[1], nullptr, OFF)) == nullptr) {
		gotoxy(33, 18, 128 + COLOR_RED); std::cout << "You have not enrolled in this course.";
		goto END;
	}
	for (int i = 0; i < schedule.count; ++i) {
		csv_line* date = &schedule.data[i];

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
				file::update(PROCESS(course_id, course_cs), mycou->id, WEEK_COLUMN + i, "1");
			}

			gotoxy(33, 18, 128 + COLOR_BLUE); std::cout << "You have already checked in this course.";
			return;
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
}

void npcourse::search(csv_file& course_list, int cur, int& choose, int& overflow, bool* permit) {
	csv_line* course = nullptr;
	int old = choose; std::string search;
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
		if (search.empty() && permit[choose]) return;

		lowercase(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			csv_line* course = &course_list.data[choose];
			if (strstr(course->pdata[1], search.c_str()) ||
				strstr(course->pdata[3], search.c_str()) ||
				strstr(course->pdata[4], search.c_str()) ||
				strstr(course->pdata[10], search.c_str())) return;
		} while (choose != old);

		uppercase(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			csv_line* course = &course_list.data[choose];
			if (strstr(course->pdata[1], search.c_str()) ||
				strstr(course->pdata[3], search.c_str()) ||
				strstr(course->pdata[4], search.c_str()) ||
				strstr(course->pdata[10], search.c_str())) return;
		} while (choose != old);
	}
}

// [EDIT]::course //============================================================================================================================//

void npcourse::add() {
	std::string cname, couid, clsid, lectu, sdate, fdate, wdays, stime, ftime, iroom;
	
	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " New course                                              ";
	gotoxy(32, 21, 128); std::cout << "                                                         ";
	gotoxy(32, 22, 128); std::cout << "                                                         ";
	gotoxy(46, 22, 128); std::cout << "    Save     ";
	gotoxy(60, 22, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << " Course name:                                            ";
	gotoxy(32, 15, 128); std::cout << " Course id  :                                            ";
	gotoxy(32, 16, 128); std::cout << " Class id   :                                            ";
	gotoxy(32, 17, 128); std::cout << " Lecturer   :                                            ";
	gotoxy(32, 18, 128); std::cout << " Timeline   :                                            ";
	gotoxy(32, 19, 128); std::cout << " Weekdays   :                                            ";
	gotoxy(32, 20, 128); std::cout << " Room       :                                            ";
	
	while (cname.empty()) if (read(46, 14, 143, cname, 30, SHOW) == KEY_ESC) return;
	while (couid.empty()) if (read(46, 15, 128, couid,  8, SHOW) == KEY_ESC) return;
	while (clsid.empty()) if (read(46, 16, 128, clsid, 10, SHOW) == KEY_ESC) return;
	while (lectu.empty()) if (read(46, 17, 128, lectu, 42, SHOW) == KEY_ESC) return;
	while (sdate.size() != 10) if (date(46, 18, 128, sdate) == KEY_ESC) return; std::cout << " to ";
	while (fdate.size() != 10) if (date(60, 18, 128, fdate) == KEY_ESC) return;
	while (1) {
		if (read(46, 19, 128, wdays, 3, SHOW, "   ") == KEY_ESC) return;
		if (wdays.size()) wdays[0] = toupper(wdays[0]);
		if (wdays == "Mon" || wdays == "Tue" || wdays == "Wed" || wdays == "Thu" || wdays == "Fri" || wdays == "Sat" || wdays == "Sun") break;
	}
	std::cout << " (      -      )";
	while (stime.size() != 5) if (time(51, 19, 128, stime) == KEY_ESC) return;
	while (ftime.size() != 5) if (time(59, 19, 128, ftime) == KEY_ESC) return;
	while (iroom.empty()) if (read(46, 20, 128, iroom, 10, SHOW) == KEY_ESC) return;

	capitalize(cname);
	uppercase(couid);
	uppercase(clsid);
	uppercase(iroom);

	for (int choose = 0;;) {
		gotoxy(46, 22, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "    Save     ";
		gotoxy(60, 22, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "   Cancel    ";

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				// Save: "__course.csv"
				if (file::find(__COURSE, couid.c_str(), clsid.c_str(), OFF) != -1) {
					gotoxy(46, 22, 134); std::cout << couid << '-' << clsid << " ";
					colorizing(128 + COLOR_RED); std::cout << " exists! ";
					PAUSE;
					return;
				}
				else {
					std::ofstream course(__COURSE, std::ios::app);
					course << "1," << couid << ',' << cname << ',' << clsid << ',' << lectu << ',' << sdate << ',' << fdate << ',' << wdays << ',' << stime << ',' << ftime << ',' << iroom << '\n';
					course.close();
				}

				// Save: "__lecturer.csv"
				if (file::find(__LECTURER, lectu.c_str(), nullptr, ON) == -1) {
					std::ofstream lecturer(__LECTURER, std::ios::app);
					lecturer << "1," << lectu << ",,,\n";
					lecturer.close();

					std::ofstream account(ACCOUNT, std::ios::app);
					account << "1," << lectu << ",0" << ",lecturer\n";
					account.close();
				}

				gotoxy(46, 22, 128 + COLOR_BLUE); std::cout << " Save successfully.        ";
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

void npcourse::open() {
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Open                                                    ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";

OPEN:
	gotoxy(32, 16, 128); std::cout << " File:                                                   ";

	std::string path;
	if (type(39, 16, 128, path, 48) == KEY_ESC) return;
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	if (!file::exists(path.c_str())) {
		gotoxy(33, 17, 132); std::cout << "Can't open file!";
		goto OPEN;
	}

	// Check title:
	csv_file imfile(path.c_str());
	if (file::find(imfile, 0, "Course ID") == nullptr) { gotoxy(33, 17, 132); std::cout << "Can't find \"Course ID\""; goto OPEN; }
	if (file::find(imfile, 0, "Class") == nullptr) { gotoxy(33, 17, 132); std::cout << "Can't find \"Class\""; goto OPEN; }
	csv_file course_list(__COURSE, def_course);

	for (int i = 0; i < imfile.count; ++i) {
		const char* course_id = file::find(imfile, i, "Course ID");
		const char* course_cs = file::find(imfile, i, "Class");
		csv_line* course = file::find(course_list, course_id, course_cs, OFF);

		// Add new course
		std::ofstream app;
		if (course == nullptr) { app.open(__COURSE, std::ios::app); app << "1," << course_id << ','; }

		const char* __str = nullptr;
		// Course name
		if ((__str = file::find(imfile, i, "Course name")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 2, __str);
		} else if (course == nullptr) app << ',';
		// Course class
		if (course == nullptr) app << course_cs << ',';
		// Lecturer ID
		if ((__str = file::find(imfile, i, "Lecturer ID")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 4, __str);
			// Save: "__lecturer.csv"
			if (file::find(__LECTURER, __str, nullptr, ON) == -1) {
				std::ofstream lecturer(__LECTURER, std::ios::app);
				lecturer << "1," << __str << ",,,\n";
				lecturer.close();

				std::ofstream account(ACCOUNT, std::ios::app);
				account << "1," << __str << ",0" << ",lecturer\n";
				account.close();
			}
		} else if (course == nullptr) app << ',';
		// Start Date
		if ((__str = file::find(imfile, i, "Start Date")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 5, __str);
		} else if (course == nullptr) app << "0,";
		// End Date
		if ((__str = file::find(imfile, i, "End Date")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 6, __str);
		} else if (course == nullptr) app << "0,";
		// Day of Week
		if ((__str = file::find(imfile, i, "Day of Week")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 7, __str);
		} else if (course == nullptr) app << "Mon,";
		// Start Time
		if ((__str = file::find(imfile, i, "Start Time")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 8, __str);
		} else if (course == nullptr) app << "07:30,";
		// End Time
		if ((__str = file::find(imfile, i, "End Time")) != nullptr) {
			if (course == nullptr) app << __str << ',';
			else file::update(__COURSE, course->id, 9, __str);
		} else if (course == nullptr) app << "07:30,";
		// Room
		if ((__str = file::find(imfile, i, "Room")) != nullptr) {
			if (course == nullptr) app << __str << '\n';
			else file::update(__COURSE, course->id, 10, __str);
		} else if (course == nullptr) app << '\n';

		gotoxy(32, 17, 128); std::cout << "                                                         ";
		gotoxy(33, 17, 129); std::cout << "Success add "; colorizing(134); std::cout << course_id << '_' << course_cs;
		Sleep(500);
	}

	gotoxy(32, 17, 129); std::cout << " Import file success.                                    ";
	PAUSE;
}

bool npcourse::sort() {
	gotoxy(23, 28, 8); std::cout << "(Ctrl + \\) was pressed. Waiting for second key of chord...";
	uint8_t c = getch();
	if (c == '1') file::sort(__COURSE, 1, 3);			// sort: course id
	else if (c == '2') file::sort(__COURSE, 3, 1);		// sort: class
	else if (c == '3') file::sort(__COURSE, 4, 1, 3);	// sort: lecturer id
	else if (c == '4') file::sort(__COURSE, 10, 1, 3);	// sort: room
	else {
		gotoxy(23, 28); std::cout << "                                                          ";
		return 0;
	}
	return 1;
}

bool npcourse::now(const char* course_id, const char* course_cs, std::tm day) {
	csv_file schedule(SCHEDULE(course_id, course_cs), course_id, course_cs);
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

	if (read(46, 18, 128, iroom, 10, SHOW, course->pdata[10]) == KEY_ESC) return;
	uppercase(iroom);

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
				
				capitalize(cname);
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

void npcourse::schedule(const char* course_id, const char* course_cs) {
	if (file::find(__COURSE, course_id, course_cs, OFF) == -1) return;
	gotoxy(32, 17, COLOR_BLUE_BACKGROUND); std::cout << " Schedule                                                ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "_________________________________________________________";

	int week = 0;
	while (1) {
	SCHEDULE:
		csv_file schedule(SCHEDULE(course_id, course_cs), course_id, course_cs);
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

			if (sdate.size() ==10) file::update(SCHEDULE(course_id, course_cs), sche->id, 1, sdate.c_str());
			if (stime.size() == 5) file::update(SCHEDULE(course_id, course_cs), sche->id, 2, stime.c_str());
			if (ftime.size() == 5) file::update(SCHEDULE(course_id, course_cs), sche->id, 3, ftime.c_str());
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
