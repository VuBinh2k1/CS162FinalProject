#include "..\\controls\\roles.h"

bool role::login(csv_line& user) {
	std::ifstream inp(".\\layout\\login.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("login.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout login_layout(inp);
	inp.close();

	std::string username, password;

LOGIN:
	csv_file user_list(".\\data\\account.csv");

	colorizing(COLOR_DEFAULT); system("cls");
	login_layout.print();

	if (read(24, 12, COLOR_DEFAULT, username, 52, SHOW) == KEY_ESC) return 0;
	if (read(24, 13, COLOR_DEFAULT, password, 52, HIDE) == KEY_ESC) return 0;
	password = sha256(password);	// sha256

	// Choose Left-right: [Login][Cancel]
	for (WORD C1 = COLOR_WHITE_BACKGROUND, C2 = COLOR_WHITE;;) {
		gotoxy(40, 17, C1); std::cout << "[Login]";
		gotoxy(48, 17, C2);  std::cout << "[Cancel]";

		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) {
			if (C1 == 240) break;
			else {
				gotoxy(34, 17, COLOR_YELLOW); std::cout << "Pls login to use the program";
				PAUSE; goto LOGIN;
			}
		}
		if (c == 224 || c == 0) {
			if (C1 == 240 && getch() == KEY_RIGHT ||
				C2 == 240 && getch() == KEY_LEFT) {
				WORD temp = C1; C1 = C2; C2 = temp;
			}
		}
	}

	for (int i = 0; i < user_list.count; ++i) {
		if (strcmp(user_list.data[i].pdata[0], "0") == 0) continue;		// Status: 0/1
		char* _username = user_list.data[i].pdata[1];
		char* _password = user_list.data[i].pdata[2];
		if (strcmp(username.c_str(), _username) == 0 &&
			(strcmp(password.c_str(), _password) == 0 || strcmp("0", _password) == 0)) {

			user = user_list.data[i];
			colorizing(COLOR_DEFAULT);

			// New account (password: '0')
			if (strcmp("0", _password) == 0)
				if (role::new_password(user) == 0) return 0; 
				else goto LOGIN;
			return 1;
		}
	}
	gotoxy(33, 17, COLOR_RED); std::cout << "Incorrect username or password";
	PAUSE;
	goto LOGIN;
}

bool role::profile(csv_line& user) {
	std::ifstream inp(".\\layout\\profile.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("profile.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout profile_layout(inp);
	inp.close();

	std::string infopath = (std::string)(".\\data\\") + user.pdata[3] + "\\__" + user.pdata[3] + ".csv";
	csv_file infofile(infopath.c_str());
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0;

PROFILE:
	colorizing(COLOR_DEFAULT); system("cls");
	profile_layout.print();
	academicmark();
	if (infouser == nullptr) {
		gotoxy(33, 9, COLOR_RED); std::cout << "Sorry, I don't have your information.";
	}
	else {

		WORD COLOR_CODE = 0;
		if (user == "staff") COLOR_CODE = COLOR_RED;
		if (user == "lecturer") COLOR_CODE = COLOR_BLUE;
		if (user == "student") COLOR_CODE = COLOR_GREEN;

		gotoxy(47, 9, COLOR_CODE);
		if (ENGLISHNAME) std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		else std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];

		gotoxy(33, 11); std::cout << infofile.mark.pdata[1]; gotoxy(45, 11); std::cout << ": " << infouser->pdata[1];
		for (int i = 4; i < infofile.mark.count; ++i) {
			if (strlen(infofile.mark.pdata[i]) == 0) continue;
			gotoxy(33, 13 + 2 * (i - 4)); std::cout << infofile.mark.pdata[i];
			if (infouser->count < i) continue;
			gotoxy(45, 13 + 2 * (i - 4)); std::cout << ": " << infouser->pdata[i];
		}
	}

	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "     Profile      ";
	while (1) {
		int E = 2;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit profile    ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Change password ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "    Main menu     ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) if (choose != E) choose = E; else goto NO_CHANGE;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				goto NO_CHANGE;
				goto PROFILE;
			}
			if (choose == 1) {
				gotoxy(2, 10, 8); std::cout << "  Change password ";
				role::password(user);
				goto PROFILE;
			}
			if (choose == E) return 1;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < E) choose++;
			else goto NO_CHANGE;
		}
	}
}

bool role::new_password(csv_line& user) {
	std::ifstream inp(".\\layout\\password.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("password.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout password_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	std::string pw_new;
	std::string pw_new_confirm;

	colorizing(COLOR_DEFAULT); system("cls");
	
	while (1) {
		password_layout.print();

		if (read(32, 10, COLOR_DEFAULT, pw_new, 52, HIDE) == KEY_ESC) return 0;
		if (read(32, 12, COLOR_DEFAULT, pw_new_confirm, 52, HIDE) == KEY_ESC) return 0;

		// Choose Left-right: [Save change][Cancel]
		for (WORD C1 = COLOR_WHITE_BACKGROUND, C2 = COLOR_WHITE;;) {
			gotoxy(37, 17, C1); std::cout << "[Save change]";
			gotoxy(51, 17, C2);  std::cout << "[Cancel]";

			uint8_t c = getch();
			if (c == KEY_ESC) return 0;
			if (c == KEY_ENTER) {
				if (C1 == 240) break;
				else return 0;
			}
			if (c == 224 || c == 0) {
				if (C1 == 240 && getch() == KEY_RIGHT ||
					C2 == 240 && getch() == KEY_LEFT) {
					WORD temp = C1; C1 = C2; C2 = temp;
				}
			}
		}

		if (pw_new.size() < 5) {
			gotoxy(32, 11, COLOR_RED); std::cout << "The Password field must be least 5 characters.";
			PAUSE; continue;
		}
		if (pw_new != pw_new_confirm) {
			gotoxy(32, 13, COLOR_RED); std::cout << "The Confirm Password confirmation does not match.";
			PAUSE; continue;
		}
		break;
	}

	// Update "account.csv"
	file::update(".\\data\\account.csv", user.id, 2, sha256(pw_new).c_str());	// sha256
	gotoxy(36, 17, COLOR_GREEN); std::cout << "Save changes successfully.";
	gotoxy(0, 20); PAUSE;
	return 1;
}

bool role::password(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	char* password = user.pdata[2];		// sha256
	std::string pw_old;
	std::string pw_new;
	std::string pw_new_confirm;

	while (1) {
		minibox_layout.print();
		gotoxy(52, 9, COLOR_YELLOW); std::cout << " Change Password ";
		gotoxy(24, 12); std::cout << "Current password:[                                                     ]";
		gotoxy(24, 15); std::cout << "New password    :[                                                     ]";
		gotoxy(24, 18); std::cout << "Confirm password:[                                                     ]";
		gotoxy(47, 27); std::cout << "[Save change] [  Cancel   ]";

		if (read(42, 12, COLOR_DEFAULT, pw_old, 52, HIDE) == KEY_ESC) return 0;
		if (read(42, 15, COLOR_DEFAULT, pw_new, 52, HIDE) == KEY_ESC) return 0;
		if (read(42, 18, COLOR_DEFAULT, pw_new_confirm, 52, HIDE) == KEY_ESC) return 0;

		// Choose Left-right: [Save change][Cancel]
		for (WORD C1 = COLOR_WHITE_BACKGROUND, C2 = COLOR_WHITE;;) {
			gotoxy(47, 27, C1); std::cout << "[Save change]";
			gotoxy(61, 27, C2); std::cout << "[  Cancel   ]";

			uint8_t c = getch();
			if (c == KEY_ESC) return 0;
			if (c == KEY_ENTER) {
				if (C1 == 240) break;
				else return 0;
			}
			if (c == 224 || c == 0) {
				if (C1 == 240 && getch() == KEY_RIGHT ||
					C2 == 240 && getch() == KEY_LEFT) {
					WORD temp = C1; C1 = C2; C2 = temp;
				}
			}
		}

		if (strcmp(password, sha256(pw_old).c_str())) {	// sha256
			gotoxy(42, 13, COLOR_RED); std::cout << "Current Password is incorrect.";
			PAUSE; continue;
		}
		if (pw_new.size() < 5) {
			gotoxy(42, 16, COLOR_RED); std::cout << "The Password field must be least 5 characters.";
			PAUSE; continue;
		}
		if (pw_new != pw_new_confirm) {
			gotoxy(42, 19, COLOR_RED); std::cout << "The Confirm Password confirmation does not match.";
			PAUSE; continue;
		}
		break;
	}

	// Update "account.csv"
	file::update(".\\data\\account.csv", user.id, 2, sha256(pw_new).c_str());	// sha256
	gotoxy(46, 27, COLOR_GREEN); std::cout << " Save changes successfully. ";

	// Update current user
	csv_file user_list(".\\data\\account.csv");
	user = user_list.data[user.id];

	PAUSE; return 1;
}

// [Main menu]::allclass //=====================================================================================================================//

void role::staff(csv_line& user) {
	std::ifstream inp(".\\layout\\staff.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("staff.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	inp.close();

	csv_file infofile(__STAFF);
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0;

MENU:
	colorizing(COLOR_DEFAULT); system("cls");
	menu_layout.print();
	academicmark();
	if (infouser == nullptr) {
		gotoxy(33, 9, COLOR_RED); std::cout << "Sorry, I don't have your information.";
	}
	else {
		gotoxy(33, 9, COLOR_YELLOW); std::cout << "Welcome ";
		colorizing(COLOR_RED);
		if (strcmp(infouser->pdata[4], "male") == 0) std::cout << "Mr.";
		else std::cout << "Ms.";
		if (ENGLISHNAME) std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		else std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];
		colorizing(COLOR_YELLOW); std::cout << " to CS162FinalProject";
	}

	gotoxy(33, 11); std::cout << "This program is for managing student info, course info,";
	gotoxy(33, 12); std::cout << "study process, etc.";
	gotoxy(33, 15, COLOR_YELLOW); std::cout << "Development team:";
	gotoxy(33, 17); std::cout << "[19127186]: Le Thanh Khoi (Leader)";
	gotoxy(33, 18); std::cout << "[19127012]: Vu Nguyen Thai Binh";
	gotoxy(33, 19); std::cout << "[19127226]: Hoang Van Nguyen";
	gotoxy(33, 20); std::cout << "[19127348]: Bui Cong Danh";

	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "      Staff       ";
	while (1) {
		int E = 5;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Classes         ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Courses         ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Lecturer        ";
		gotoxy(2,13, (choose == 4) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Settings        ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC && choose != E) { choose = E; continue; }
		if (c == KEY_ENTER) {
			if (choose == 0) {
				role::profile(user);
				goto MENU;
			}
			if (choose == 1) {
				gotoxy(2, 10, 8); std::cout << "  Classes         ";
				role::classes_list(user);
				goto MENU;
			}
			if (choose == 2) {
				gotoxy(2, 11, 8); std::cout << "  Courses         ";
				role::courses_list(user);
				goto MENU;
			}
			if (choose == 3) {
				gotoxy(2, 12, 8); std::cout << "  Lecturer        ";
				role::lecturer_list();
				goto MENU;
			}
			if (choose == 4) {
				gotoxy(2, 13, 8); std::cout << "  Settings        ";
				role::settings();
				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < E) choose++;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
}

void role::lecturer(csv_line& user) {
	std::ifstream inp(".\\layout\\lecturer.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("lecturer.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	inp.close();

	csv_file infofile(__LECTURER);
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0;

MENU:
	colorizing(COLOR_DEFAULT); system("cls");
	menu_layout.print();
	academicmark();
	if (infouser == nullptr) {
		gotoxy(33, 9, COLOR_RED); std::cout << "Sorry, I don't have your information.";
	}
	else {
		gotoxy(33, 9, COLOR_YELLOW); std::cout << "Welcome ";
		colorizing(COLOR_BLUE); std::cout << infouser->pdata[4] << '.';
		if (ENGLISHNAME) std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		else std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];
		colorizing(COLOR_YELLOW); std::cout << " to CS162FinalProject";
	}

	gotoxy(33, 11); std::cout << "This program is for managing student info, course info,";
	gotoxy(33, 12); std::cout << "study process, etc.";
	gotoxy(33, 15, COLOR_YELLOW); std::cout << "Development team:";
	gotoxy(33, 17); std::cout << "[19127186]: Le Thanh Khoi (Leader)";
	gotoxy(33, 18); std::cout << "[19127012]: Vu Nguyen Thai Binh";
	gotoxy(33, 19); std::cout << "[19127226]: Hoang Van Nguyen";
	gotoxy(33, 20); std::cout << "[19127348]: Bui Cong Danh";

	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "     Lecturer     ";
	while (1) {
		int E = 2;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My courses      ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC && choose != E) { choose = E; continue; }
		if (c == KEY_ENTER) {
			if (choose == 0) {
				role::profile(user);
				goto MENU;
			}
			if (choose == 1) {
				gotoxy(2, 10, 8); std::cout << "  My courses      ";
				role::courses_list(user);
				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < E) choose++;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
}

void role::student(csv_line& user) {
	std::ifstream inp(".\\layout\\student.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("student.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	inp.close();

	csv_file infofile(__STUDENT);
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0; bool submenu_change = 0;

MENU:
	colorizing(COLOR_DEFAULT); system("cls");
	menu_layout.print();
	academicmark();
	if (infouser == nullptr) {
		gotoxy(33, 9, COLOR_RED); std::cout << "Sorry, I don't have your information.";
	}
	else {
		gotoxy(33, 9, COLOR_YELLOW); std::cout << "Welcome ";
		colorizing(COLOR_GREEN);
		std::cout << "St.";
		if (ENGLISHNAME) std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		else std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];
		colorizing(COLOR_YELLOW); std::cout << " to CS162FinalProject";
	}

	gotoxy(33, 11); std::cout << "This program is for managing student info, course info,";
	gotoxy(33, 12); std::cout << "study process, etc.";
	gotoxy(33, 15, COLOR_YELLOW); std::cout << "Development team:";
	gotoxy(33, 17); std::cout << "[19127186]: Le Thanh Khoi (Leader)";
	gotoxy(33, 18); std::cout << "[19127012]: Vu Nguyen Thai Binh";
	gotoxy(33, 19); std::cout << "[19127226]: Hoang Van Nguyen";
	gotoxy(33, 20); std::cout << "[19127348]: Bui Cong Danh";

	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "     Student      ";
	while (1) {
		int E = 5;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My courses      ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My calendar     ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My attendance   ";
		gotoxy(2,13, (choose == 4) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  My scoreboard   ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

	NO_CHANGE:
		uint8_t c = KEY_ENTER;
		if (submenu_change) submenu_change = 0; else c = getch();
		if (c == KEY_ESC && choose != E) { choose = E; continue; }
		if (c == KEY_ENTER) {
			if (choose == 0) {
				role::profile(user);
				goto MENU;
			}
			if (choose == 1) {
				gotoxy(2, 10, 8); std::cout << "  My courses      ";
				if ((choose += role::courses_list(user)) != 1) { submenu_change = 1; continue; }
				goto MENU;
			}
			if (choose == 2) {
				gotoxy(2, 11, 8); std::cout << "  My calendar     ";
				if ((choose += role::calendar(user)) != 2) { submenu_change = 1; continue; }
				goto MENU;
			}
			if (choose == 3) {
				gotoxy(2, 12, 8); std::cout << "  My attendance   ";
				if ((choose += npattendance::student(user)) != 3) { submenu_change = 1; continue; }
				goto MENU;
			}
			if (choose == 4) {
				gotoxy(2, 13, 8); std::cout << "  My scoreboard   ";
				if ((choose += npscoreboard::student(user)) != 4) { submenu_change = 1; continue; }
				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < E) choose++;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
}

// [Sub menu]::allclass //======================================================================================================================//

int role::classes_list(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	int choose = 0, cur = -1, overflow = 0;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "  Class list     "; colorizing(COLOR_YELLOW); std::cout << "  Student list   ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Class name   | Infomation                                 ";
	while ((cur = -1)) {
		csv_file class_list(__CLASS, def_class);
		csv_line* cls = nullptr;

		for (int i = 0; i < class_list.count; ++i) {
			int y = 10 + (++cur) + overflow;
			if (y < 10 || y > 27) continue;
			cls = &class_list.data[i];
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			if (choose == cur) COLOR_CODE = 176;

			gotoxy(27, y, COLOR_CODE); std::cout << "       |              |                                            ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(36, y, COLOR_CODE); std::cout << cls->pdata[1];
			if (cls->count > 2) { gotoxy(51, y, COLOR_CODE); std::cout << cls->pdata[2]; }
		}
	NO_CHANGE:
		uint8_t c = getch();
		cls = &class_list.data[choose];
		if (c == KEY_ESC) break;
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " New       Ctrl+N   ";
			gotoxy(78, 9, 128); std::cout << " Search    Ctrl+F   "; 
			gotoxy(78,10, 128); std::cout << " Delete    Dalete   "; 
			gotoxy(78,11, 128); std::cout << "____________________";
			gotoxy(78,12, 128); std::cout << " Sort      Ctrl+\\,  ";
			gotoxy(78,13, 128); std::cout << "  - Class name    1 ";
			gotoxy(78,14, 128); std::cout << "                    ";
			getch();
			goto LAYOUT;
		}
		if (c == KEY_NEW) { npclass::add(); choose = overflow = 0; goto LAYOUT; }
		if (c == KEY_SEARCH) { npclass::search(class_list, cur, choose, overflow); goto LAYOUT; }
		if (c == KEY_FUNCTION) { if (npclass::sort()) goto LAYOUT; else goto NO_CHANGE; }
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_DELETE) { npclass::remove(cls->pdata[1]); choose = overflow = 0; goto LAYOUT; }
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_RIGHT) { npstudent::list(user, cls->pdata[1]); goto LAYOUT; }
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	return 0;
}

int role::courses_list(csv_line& user) {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	int choose = 0, cur = -1, overflow = 0; bool* permit = nullptr;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "  Course list    ";
	if (user == "student") { colorizing(COLOR_YELLOW); std::cout << "  Calendar       " << "  Attendance     " << "  Scoreboard    "; }
	else { colorizing(COLOR_YELLOW); std::cout << "  Student list   " << "  Attendance     " << "  Scoreboard    "; }
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Course ID    | Class     | Lecturer ID  | Room   | Status ";
	while ((cur = -1)) {
		csv_file course_list(__COURSE, def_course);
		csv_line* course = nullptr;

		if (permit) delete[] permit;
		permit = new bool[course_list.count];

		for (int i = 0; i < course_list.count; ++i) {
			int y = 10 + (++cur) + overflow;
			if (y < 10 || y > 27) continue;
			course = &course_list.data[i]; permit[i] = 0;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			if (choose == cur) COLOR_CODE = 176;

			gotoxy(27, y, COLOR_CODE); std::cout << "       |              |           |              |        |        ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(36, y, COLOR_CODE); std::cout << course->pdata[1];
			gotoxy(51, y, COLOR_CODE); std::cout << course->pdata[3];
			gotoxy(63, y, COLOR_CODE); std::cout << course->pdata[4];
			gotoxy(78, y, COLOR_CODE); std::cout << course->pdata[10];

			// Status
			if (user == "staff" || (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0)) {
				if (course->pdata[0][0] == '0') { gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << "private"; }
				else { gotoxy(87, y, COLOR_CODE + 2); std::cout << "public"; }
				permit[i] = 1;
			}
			if (user == "student") {
				csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);

				for (int j = 0; j < my_course.count; ++j) {
					if (strcmp(my_course.data[j].pdata[0], ACADEMICYEAR.c_str())) continue;
					if (strcmp(my_course.data[j].pdata[1], SEMESTER.c_str())) continue;
					if (strcmp(my_course.data[j].pdata[2], course->pdata[1]) == 0)
						if (strcmp(my_course.data[j].pdata[3], course->pdata[3]) == 0) {
							gotoxy(87, y, COLOR_CODE + 2); std::cout << "enrol";
							permit[i] = 1; break;
						}
				}
			}
		}
	NO_CHANGE:
		uint8_t c = getch();
		course = &course_list.data[choose];
		if (c == KEY_ESC) break;
		if (KEY_HELP(c)) {
			if (user == "staff") {
				gotoxy(78, 8, 128); std::cout << " New       Ctrl+N   ";
				gotoxy(78, 9, 128); std::cout << " Open      Ctrl+O   ";
				gotoxy(78,10, 128); std::cout << " Search    Ctrl+F   ";
				gotoxy(78,11, 128); std::cout << " Delete    Delete   ";
				gotoxy(78,12, 128); std::cout << "____________________";
				gotoxy(78,13, 128); std::cout << " Sort      Ctrl+\\,  ";
				gotoxy(78,14, 128); std::cout << "  - Course ID     1 ";
				gotoxy(78,15, 128); std::cout << "  - Class         2 ";
				gotoxy(78,16, 128); std::cout << "  - Lecturer ID   3 ";
				gotoxy(78,17, 128); std::cout << "  - Room          4 ";
				gotoxy(78,18, 128); std::cout << "____________________";
				gotoxy(78,19, 128); std::cout << " Enrol     R, r     ";
				gotoxy(78,20, 128); std::cout << "                    ";
			} else {
				gotoxy(78, 8, 128); std::cout << " Search    Ctrl+F   ";
				gotoxy(78, 9, 128); std::cout << "____________________";
				gotoxy(78,10, 128); std::cout << " Sort      Ctrl+\\,  ";
				gotoxy(78,11, 128); std::cout << "  - Course ID     1 ";
				gotoxy(78,12, 128); std::cout << "  - Class         2 ";
				gotoxy(78,13, 128); std::cout << "  - Lecturer ID   3 ";
				gotoxy(78,14, 128); std::cout << "  - Room          4 ";
				gotoxy(78,15, 128); std::cout << "____________________";
				gotoxy(78,16, 128); std::cout << " Enrol     R, r     ";
				gotoxy(78,17, 128); std::cout << "                    ";
			}
			getch();
			goto LAYOUT;
		}
		if (c == KEY_NEW && user == "staff") { npcourse::add(); choose = overflow = 0; goto LAYOUT; }
		if (c == KEY_OPEN && user == "staff") { npcourse::open(); choose = overflow = 0; goto LAYOUT; }
		if (c == KEY_SEARCH) { npcourse::search(course_list, cur, choose, overflow, permit); goto LAYOUT; }
		if (c == KEY_FUNCTION) { if (npcourse::sort()) goto LAYOUT; else goto NO_CHANGE; }
		if (c == KEY_ENTER) {
			if (user == "staff" || (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0)) {
				npcourse::info(user, course->pdata[1], course->pdata[3]);
				goto LAYOUT;
			}
			if (user == "student") {
				npcourse::chkin(user, course->pdata[1], course->pdata[3]);
				goto LAYOUT;
			}
			goto NO_CHANGE;
		}
		if (KEY_EROL(c)) { npcourse::enrol(user, course->pdata[1], course->pdata[3]); goto LAYOUT; }
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_DELETE && user == "staff") { npcourse::info(user, course->pdata[1], course->pdata[3], ON); choose = overflow = 0; goto LAYOUT; }
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_RIGHT) {
				if (user == "staff" || (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0)) {
					npstudent::list(user, course->pdata[1], course->pdata[3]);
					goto LAYOUT;
				}
				if (user == "student") { delete[] permit; return 1; }
				goto NO_CHANGE;
			}
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	delete[] permit;
	return 0;
}

int role::lecturer_list() {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	int choose = 0, cur = -1, overflow = 0; int* row = nullptr;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "  Lecturer list  ";
	colorizing(COLOR_YELLOW); std::cout << "  Course list    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND); std::cout << " No.   | Lecturer ID | Ranking    | Full name                      ";

	while ((cur = -1)) {
		csv_file lecturer_list(__LECTURER);
		csv_line* lecturer = nullptr;
		if (row) delete[]row;
		row = new int[lecturer_list.count];

		for (int i = 0; i < lecturer_list.count; i++) {
			if ((lecturer = file::find(lecturer_list, lecturer_list.data[i].pdata[1], nullptr, ON)) == nullptr) continue;

			int y = 10 + (++cur) + overflow; row[cur] = i;
			if (y < 10 || y>27) continue;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			if (choose == cur)COLOR_CODE = 176;

			gotoxy(27, y, COLOR_CODE); std::cout << "       |             |            |                                ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(36, y, COLOR_CODE); std::cout << lecturer->pdata[1];
			gotoxy(50, y, COLOR_CODE); std::cout << lecturer->pdata[4];
			gotoxy(63, y, COLOR_CODE);
			if (ENGLISHNAME) std::cout << lecturer->pdata[3] << ' ' << lecturer->pdata[2];
			else std::cout << lecturer->pdata[2] << ' ' << lecturer->pdata[3];
		}
	NO_CHANGE:
		uint8_t c = getch();
		lecturer = &lecturer_list.data[row[choose]];
		if (c == KEY_ESC) break;
		if (KEY_HELP(c)) {
			gotoxy(78, 8, 128); std::cout << " New       Ctrl+N   ";
			gotoxy(78, 9, 128); std::cout << " Open      Ctrl+O   ";
			gotoxy(78,10, 128); std::cout << " Search    Ctrl+F   ";
			gotoxy(78,11, 128); std::cout << " Delete    Delete   ";
			gotoxy(78,12, 128); std::cout << "____________________";
			gotoxy(78,13, 128); std::cout << " Sort      Ctrl+\\,  ";
			gotoxy(78,14, 128); std::cout << "  - Lecturer ID   1 ";
			gotoxy(78,15, 128); std::cout << "  - Ranking       2 ";
			gotoxy(78,16, 128); std::cout << "  - First name    3 ";
			gotoxy(78,17, 128); std::cout << "                    ";
			getch();
			goto LAYOUT;
		}
		if (c == KEY_NEW){ nplecturer::add(); choose = overflow = 0; goto LAYOUT; }
		// KEY_OPEN:
		if (c == KEY_SEARCH) { nplecturer::search(lecturer_list, cur, choose, overflow, row); goto LAYOUT; }
		if (c == KEY_FUNCTION) { if (nplecturer::sort()) goto LAYOUT; else goto NO_CHANGE; }
		if (c == KEY_ENTER) { nplecturer::info(lecturer->pdata[1], ON); goto LAYOUT; }
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_DELETE) { nplecturer::info(lecturer->pdata[1], ON, ON); choose = overflow = 0;  goto LAYOUT; }
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_RIGHT) { nplecturer::courses_list(lecturer->pdata[1]); goto LAYOUT; }
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	delete[] row;
	return 0;
}

int role::calendar(csv_line& user){
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();

	std::time_t now = time(0);
	std::tm ltm = *localtime(&now); ltm.tm_hour = 0; std::mktime(&ltm);

	minibox_layout.print();
	// Title
	gotoxy(27, 8, COLOR_YELLOW); std::cout << "  Course list    ";
	colorizing(COLOR_YELLOW_BACKGROUND); std::cout << "  Calendar       ";
	colorizing(COLOR_YELLOW); std::cout << "  Attendance     " << "  Scoreboard    ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << "    Date    | Course                               | Start |  End  ";

	csv_file my_course(((std::string)".\\data\\student\\" + user.pdata[1] + ".csv").c_str(), def_user);
	csv_file course_list(__COURSE, def_course);

	int choose = 0, maxChoose = 13; bool has_change = 0;
	while (1) {
		int cur = 0;
		std::tm day = ltm; day.tm_mday -= 7;
		for (int i = 0; cur < choose + 18 && i < 31; ++i) {
			int y = 10 + cur - choose;
			day.tm_mday++; std::mktime(&day);

			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			if (day.tm_mday == ltm.tm_mday) COLOR_CODE = COLOR_RED_BACKGROUND;
			if (9 < y && y < 28) {
				gotoxy(27, y, COLOR_CODE); std::cout << "            |                                      |       |       ";
				gotoxy(28, y, COLOR_CODE); 
				if (day.tm_mday <10) std::cout << '0'; std::cout << day.tm_mday << '/';
				if (day.tm_mon  < 9) std::cout << '0'; std::cout << day.tm_mon + 1 << '/';
				std::cout << day.tm_year + 1900;
			}

			csv_line* course = nullptr;
			bool empty = 1;
			for (int j = 0; j < my_course.count; ++j) {
				course = &my_course.data[j];
				if (strcmp(course->pdata[0], ACADEMICYEAR.c_str())) continue;
				if (strcmp(course->pdata[1], SEMESTER.c_str())) continue;
				if (npcourse::now(course->pdata[2], course->pdata[3], day) == 0) continue;

				y = 10 + cur - choose;
				if (y < 10) { cur++; empty = 0; continue; }
				if (y > 27) {
					if (!has_change && choose == 13) maxChoose++;
					continue;
				}

				if (!empty) { gotoxy(27, y, COLOR_CODE); std::cout << "            |                                      |       |       "; }
				gotoxy(41, y, COLOR_CODE); std::cout << course->pdata[2] << ": ";

				for (int k = 0; k < course_list.count; ++k) {
					if (strcmp(course->pdata[2], course_list.data[k].pdata[1])) continue;

					csv_line* mycou = file::find(course_list, course->pdata[2], course->pdata[3], OFF);
					if (mycou == nullptr) continue;
					std::cout << mycou->pdata[2];
					gotoxy(80, y, COLOR_CODE); std::cout << mycou->pdata[8];
					gotoxy(88, y, COLOR_CODE); std::cout << mycou->pdata[9];
					break;
				}
				cur++;  empty = 0;
			}
			if (empty) cur++;
			if (maxChoose != 13) has_change = 1;
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < maxChoose) choose++;
			else if (c == KEY_LEFT) return -1;
			else if (c == KEY_RIGHT) return 1;
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	return 0;
}

void role::settings() {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	int chsEN = ENGLISHNAME;
	std::string chsAY = ACADEMICYEAR;
	int chsAS = atoi(SEMESTER.c_str());

	minibox_layout.print();
	gotoxy(53, 9, COLOR_YELLOW);  std::cout << "Program setting";
	gotoxy(33, 12); std::cout << "English name format        : ";
	gotoxy(33, 15); std::cout << "Current Academic year      : "; gotoxy(72, 15); std::cout << chsAY;
	gotoxy(33, 18); std::cout << "Current Academic semester  : "; gotoxy(72, 18); std::cout << chsAS;
	gotoxy(46, 27); std::cout << "[Save change] [  Cancel   ]";
	if (chsAS == 1) { gotoxy(72, 18, 192); std::cout << " Spring "; }
	if (chsAS == 2) { gotoxy(72, 18, 160); std::cout << " Summer "; }
	if (chsAS == 3) { gotoxy(72, 18, 224); std::cout << " Autumn "; }
	if (chsAS == 4) { gotoxy(72, 18, 176); std::cout << " Winter "; }

	while (1) {
		if (chsEN) { gotoxy(72, 12, COLOR_GREEN_BACKGROUND); std::cout << " ON "; colorizing(COLOR_DEFAULT); std::cout << ' '; }
		else { gotoxy(72, 12, COLOR_RED_BACKGROUND); std::cout << " OFF "; }

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) break;
		if (c == 224 || c == 0) {
			if ((c = getch()) == KEY_LEFT || c == KEY_RIGHT) {
				chsEN = 1 - chsEN;
			}
		}
	}
	while (1) {
		gotoxy(72, 15); std::cout << "                        ";
		read(72, 15, COLOR_DEFAULT, chsAY, 4, SHOW, ACADEMICYEAR.c_str());
		if (chsAY.empty()) break;
		if (std::stoi(chsAY) > 3000) { gotoxy(72, 15, COLOR_RED); std::cout << "Academic year too big!"; }
		else if (std::stoi(chsAY) < 2000) { gotoxy(72, 15, COLOR_RED); std::cout << "Academic year too small!"; }
		else break;
		PAUSE;
	}
	while (1) {
		if (chsAS == 1) { gotoxy(72, 18, 192); std::cout << " Spring "; }
		if (chsAS == 2) { gotoxy(72, 18, 160); std::cout << " Summer "; }
		if (chsAS == 3) { gotoxy(72, 18, 224); std::cout << " Autumn "; }
		if (chsAS == 4) { gotoxy(72, 18, 176); std::cout << " Winter "; }

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) break;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && chsAS > 1) chsAS--;
			if (c == KEY_DOWN && chsAS < 4) chsAS++;
		}
	}

	for (int choose = 0;;) {
		gotoxy(46, 27, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[Save change]";
		gotoxy(60, 27, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "[  Cancel   ]";

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				if (chsAY.empty()) chsAY = ACADEMICYEAR;
				
				// Create directory (single folder)
				std::string Folder = (std::string)".\\data\\course\\" + chsAY + '-' + (char)(chsAS + '0');
				if (mkdir(Folder.c_str()) || (errno != EEXIST));
				if (mkdir((Folder + "\\schedule").c_str()) || (errno != EEXIST));
				if (mkdir((Folder + "\\process").c_str())  || (errno != EEXIST));				

				// Save to ".config"
				std::ofstream cfg(".config");
				cfg << "ENGLISHNAME " << chsEN << "\n";
				cfg << "ACADEMICYEAR " << chsAY << "\n";
				cfg << "SEMESTER " << chsAS << "\n";
				gotoxy(46, 27, COLOR_GREEN); std::cout << " Save changes successfully.";
				cfg.close();
				PAUSE; return;
			}
			if (choose == 1) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0) choose++;
		}
	}
}
