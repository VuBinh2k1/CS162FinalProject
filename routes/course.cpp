#include "..\\controls\\course.h"

void npcourse::list(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

LAYOUT:
	minibox_layout.print();
	if (user == "staff") { gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                            Course list                            "; }
	else {
		gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "  Course list  ";
		colorizing(COLOR_YELLOW); std::cout << "  My courses  ";
	}
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << "  No.  | Course ID    | Class     | Lecturer ID  | Room   | Status ";
	if (user == "student") { gotoxy(33, 28); std::cout << "[E]: Enrol in course"; }
	else { 
		gotoxy(33, 27); std::cout << "[V]: View student list of course";
		gotoxy(33, 28); std::cout << "[E]: Enrol student on course"; 
	}

	csv_file course_list((COURSE_PATH("__course.csv")).c_str(), def_course);
	int choose = 0;
	while (1) {
		for (int i = choose; i < min(choose + 18, course_list.count); i++)
		{
			int y = 10 + i - choose;
			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			csv_line* course = &course_list.data[i];
			if (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0) COLOR_CODE = (i % 2) ? 48 : 176;

			gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |              |        |        ";
			gotoxy(28, y, COLOR_CODE); std::cout << i;
			gotoxy(36, y, COLOR_CODE); std::cout << course->pdata[1];
			gotoxy(51, y, COLOR_CODE); std::cout << course->pdata[3];
			gotoxy(63, y, COLOR_CODE); std::cout << course->pdata[4];
			gotoxy(78, y, COLOR_CODE); std::cout << course->pdata[10];

			// Status
			if (user == "staff" || (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0)) {
				if (course->pdata[0][0] == '0') {
					gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << "private";
				}
				else {
					gotoxy(87, y, COLOR_CODE + 2); std::cout << "public";
				}
			}
			if (user == "student") {
				csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);
				for (int i = 0; i < my_course.count; ++i) {
					if (strcmp(my_course.data[i].pdata[0], ACADEMICYEAR.c_str())) continue;
					if (strcmp(my_course.data[i].pdata[1], SEMESTER.c_str())) continue;
					if (strcmp(my_course.data[i].pdata[2], course->pdata[1]) == 0)
						if (strcmp(my_course.data[i].pdata[3], course->pdata[3]) == 0) {
							gotoxy(87, y, COLOR_CODE + 2); std::cout << "enrol";
						}
				}
			}
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == 'E' || c == 'e') {
			npcourse::enrol(user);
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
		}
		if (c == 'V' || c == 'v')
		{
			npcourse::viewstudent(user);
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			gotoxy(32, 18); std::cout << "                                                         ";
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < course_list.count - 18) choose++;
			else if (c == KEY_RIGHT) {

				goto NO_CHANGE;
			}
			else goto NO_CHANGE;
		}
	}
}

void npcourse::enrol(csv_line& user) {
	csv_file student(".\\data\\student\\__student.csv");
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Enrol                                                   ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	std::string studentid, courseid, coursecs;

ENROL_DATA:
	gotoxy(32, 16, 128); std::cout << " Student ID:                                             ";
	gotoxy(32, 17, 128); std::cout << " Course ID :               Class:                        ";
	if (user == "student") studentid = user.pdata[1]; else if (read(45, 16, 128, studentid, 8, SHOW) == KEY_ESC) return;
	gotoxy(45, 16, 128); std::cout << studentid;
	if (read(45, 17, 128, courseid, 8, SHOW) == KEY_ESC) return;
	if (read(66, 17, 128, coursecs, 8, SHOW) == KEY_ESC) return;
	std::transform(courseid.begin(), courseid.end(), courseid.begin(), ::toupper);
	std::transform(coursecs.begin(), coursecs.end(), coursecs.begin(), ::toupper);
	gotoxy(32, 18, 128); std::cout << "                                                         ";

	// Check student exist "__student.csv"
	if (!file::exists(student, studentid.c_str())) {
		gotoxy(33, 18, 132); std::cout << "Can't find Student ID: "; colorizing(134); std::cout << studentid;
		goto ENROL_DATA;
	}

	// Check course exist "__course.csv"
	bool course_exist = 0;
	csv_file course_list((COURSE_PATH("__course.csv")).c_str(), def_course);
	for (int i = 0; i < course_list.count; ++i) {
		if (courseid == course_list.data[i].pdata[1] && coursecs == course_list.data[i].pdata[3]) {
			if (user == "student" && course_list.data[i].pdata[0][0] == '0') {
				gotoxy(33, 18, 132); std::cout << "You can't self enrol in this course!";
				goto ENROL_DATA;
			}
			if (user == "lecturer" && strcmp(course_list.data[i].pdata[4], user.pdata[1])) {
				gotoxy(33, 18, 132); std::cout << "You can't enrol student on this course!";
				goto ENROL_DATA;
			}
			course_exist = 1; break;
		}
	}
	if (!course_exist) {
		gotoxy(33, 18, 132); std::cout << "Can't find Course: "; colorizing(134); std::cout << courseid << " - " << coursecs;
		goto ENROL_DATA;
	}

	// Add course to "studentID.csv"
	csv_file my_course(((std::string)".\\data\\student\\" + studentid + ".csv").c_str(), def_user);
	for (int i = 0; i < my_course.count; ++i) {
		// Had enrol in this course (only enrol in 1 class of a course)
		if (courseid == my_course.data[i].pdata[2]) {
			gotoxy(33, 18, 132); std::cout << "Had enrol in ";
			colorizing(134); std::cout << courseid << " - " << my_course.data[i].pdata[3];
			goto ENROL_DATA;
		}
	}
	std::ofstream out((std::string)".\\data\\student\\" + studentid + ".csv", std::ios::app);
	out << ACADEMICYEAR << ',' << SEMESTER << ',' << courseid << ',' << coursecs << '\n'; out.close();

	// Add student to "courseID_courseCS.csv"
	csv_file process((COURSE_PATH("process\\") + courseid + '_' + coursecs + ".csv").c_str(), def_process);
	for (int i = 0; i < process.count; ++i) {
		// Old data (don't know why it here)
		if (studentid == process.data[i].pdata[1]) goto SUCCESS;
	}
	out.open(COURSE_PATH("process\\") + courseid + '_' + coursecs + ".csv", std::ios::app);
	out << "0," << studentid << ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"; out.close();

SUCCESS: 
	gotoxy(33, 18, 129); std::cout << "Enrol success.";
	goto ENROL_DATA;
}

bool npcourse::now(const char* course_id, const char* course_cs, std::tm day) {
	csv_file schedule((COURSE_PATH("schedule\\") + course_id + "_" + course_cs + ".csv").c_str(), def_schedule);
	for (int i = 0; i < schedule.count; ++i) {
		if (control::now(day, schedule.data[i].pdata[1]) == 0) return 1;
	}
	return 0;
}

bool npcourse::info(const char* course_id, const char* course_cs, int x, int y, int n) {
	csv_file course_list(COURSE_PATH("__course.csv").c_str(), def_course);
	csv_line* course = nullptr;
	for (int i = 0; i < course_list.count; ++i) {
		if (strcmp(course_id, course_list.data[i].pdata[1])) continue;
		if (strcmp(course_cs, course_list.data[i].pdata[3])) continue;
		course = &course_list.data[i];
	}

	if (course == nullptr) {
		if (n > 0) { gotoxy(x, y, COLOR_RED); std::cout << "This course does not exist"; }
		return 0;
	}

	if (n > 0) { gotoxy(x, y, COLOR_YELLOW); std::cout << "Course Name : " << course->pdata[2]; }
	if (n > 1) { gotoxy(x, y + 2); std::cout << "Course ID   : " << course->pdata[1]; }
	if (n > 2) { gotoxy(x, y + 4); std::cout << "Class       : " << course->pdata[3]; }
	if (n > 3) { gotoxy(x, y + 6); std::cout << "Lecturer ID : " << course->pdata[4]; }
	if (n > 4) { gotoxy(x, y + 8); std::cout << "Room        : " << course->pdata[10]; }

	return 1;
}

csv_line* npcourse::find(csv_line* beg, csv_line* end, const char* courseid, int id) {
	for (csv_line* course = beg; course != end; ++course) {
		if (id == 2 && strcmp(course->pdata[0], ACADEMICYEAR.c_str())) continue;
		if (id == 2 && strcmp(course->pdata[1], SEMESTER.c_str())) continue;
		if (strcmp(course->pdata[id], courseid) == 0) return course;
	}
	return nullptr;
}

csv_line* npcourse::choose(const csv_file& my_course, int& choose, int id) {
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "    My courses    ";

	csv_line* course = nullptr; int overflow = 0;

	// Course ID without class: O(len.logn)
	std::map <std::string, bool> unClass;

	while (1) {	// Choose Up-down: [COURSE]
		int cur = -1;
		for (int i = 0; i < my_course.count; ++i) {
			if (id == 2 && strcmp(my_course.data[i].pdata[0], ACADEMICYEAR.c_str())) continue;
			if (id == 2 && strcmp(my_course.data[i].pdata[1], SEMESTER.c_str())) continue;

			std::string courseid = my_course.data[i].pdata[id];
			if (unClass.find(courseid) != unClass.end()) continue;
			int y = 9 + (++cur) + overflow;
			if (y < 9 || y > 27) continue;
			if (choose == cur) course = &my_course.data[i];
			unClass[courseid] = i;

			gotoxy(2, y, (choose == cur ? COLOR_WHITE_BACKGROUND : COLOR_WHITE)); std::cout << "                  ";
			gotoxy(7, y, (choose == cur ? COLOR_WHITE_BACKGROUND : COLOR_WHITE)); std::cout << my_course.data[i].pdata[id];
			
		}
		unClass.clear();
		gotoxy(2, min(28, 10 + cur), 224); std::cout << "  <-- Main menu   ";

	UN_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) return nullptr;
		if (c == KEY_ENTER) {
			gotoxy(2, 9 + choose + overflow, 8); std::cout << "                  ";
			gotoxy(7, 9 + choose + overflow, 8); std::cout << course->pdata[id];
			return course;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 17) overflow--; }
			else if (c == KEY_LEFT) return nullptr;
			else goto UN_CHANGE;
		}
	}
}
void npcourse::viewstudent(csv_line& user)
{
	std::string file(".\\data\\course\\2020-2\\process\\");
	gotoxy(32, 20, COLOR_RED_BACKGROUND);  std::cout << " View                                                    ";
	gotoxy(32, 22, 128); std::cout << "                                                         ";
	std::string  courseid, coursecs;
VIEW_DATA:
	while (1) {
		gotoxy(32, 21, 128); std::cout << " Course ID :               Class:                        ";
		if (read(45, 21, 128, courseid, 8, SHOW) == KEY_ESC)return;
		if (read(66, 21, 128, coursecs, 8, SHOW) == KEY_ESC)return;
		std::transform(courseid.begin(), courseid.end(), courseid.begin(), std::toupper);
		if (courseid.empty())
			continue;
		std::transform(coursecs.begin(), coursecs.end(), coursecs.begin(), std::toupper);
		if (coursecs.empty())
			continue;
		else
			break;
	}
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                           Student list                            ";
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << "  No.  |                    Student ID                             ";
	csv_file student_list(((std::string)file + courseid + "_" + coursecs + ".csv").c_str());
	bool course_exist = 0;
	int choose = 0,cur=-1;
	while (1)
	{
		for (int i = 10; i <= 26; i++)
		{
			gotoxy(27, i, 0); std::cout << "                                                                   ";
		}
		cur = -1;
		for (int i = 0; i < student_list.count; i++)
		{
			int y = 10 + (++cur) - choose;
			if (y < 10 || y>26)continue;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			gotoxy(27, y, COLOR_CODE); std::cout << "         |                                                         ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(56, y, COLOR_CODE); std::cout << student_list.data[i].pdata[1];
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < cur - 17) choose++;
			else if (c == KEY_LEFT) goto VIEW_DATA;
			else goto NO_CHANGE;
		}
	}
}
