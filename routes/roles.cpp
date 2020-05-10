#include "..\\controls\\roles.h"

bool role::login(csv_line& user) {
	std::ifstream inp(".\\layout\\login.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("login.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout login_layout(inp);
	layout user_pass_layout(inp);
	inp.close();

	csv_file user_list(".\\data\\account.csv");

	colorizing(COLOR_DEFAULT); system("cls");
	login_layout.print();
	for (std::string username, password;;) {
	LOGIN:

		user_pass_layout.print();
		if (read(24, 12, COLOR_DEFAULT, username, 52, SHOW) == KEY_ESC) return 0;
		if (read(24, 13, COLOR_DEFAULT, password, 52, HIDE) == KEY_ESC) return 0;

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
				strcmp(password.c_str(), _password) == 0) {

				user = user_list.data[i];
				colorizing(COLOR_DEFAULT);
				if (password.size() < 5 && role::new_password(user) == 0) return 0;
				return 1;
			}
		}
		gotoxy(33, 17, COLOR_RED); std::cout << "Incorrect username or password";
		PAUSE;
	}
}

bool role::profile(csv_line& user) {
	std::ifstream inp(".\\layout\\profile.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("profile.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout profile_layout(inp);
	inp.close();

	std::string position = user.pdata[3];
	std::string infopath = (std::string)(".\\data\\") + position + "\\__" + position + ".csv";
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
		if (position == "staff") COLOR_CODE = COLOR_RED;
		if (position == "lecturer") COLOR_CODE = COLOR_BLUE;
		if (position == "student") COLOR_CODE = COLOR_GREEN;

		gotoxy(47, 9, COLOR_CODE);
		if (ENGLISHNAME) {
			std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		}
		else {
			std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];
		}

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
		gotoxy(2, 10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Change password ";
		gotoxy(2, 28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "    Main menu     ";

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
	layout change_password_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	char* password = user.pdata[2];
	std::string pw_old;
	std::string pw_new;
	std::string pw_new_confirm;

	colorizing(COLOR_DEFAULT); system("cls");
	password_layout.print();
	while (1) {
		change_password_layout.print();

		if (read(32, 9, COLOR_DEFAULT, pw_old, 52, HIDE) == KEY_ESC) return 0;
		if (read(32, 11, COLOR_DEFAULT, pw_new, 52, HIDE) == KEY_ESC) return 0;
		if (read(32, 13, COLOR_DEFAULT, pw_new_confirm, 52, HIDE) == KEY_ESC) return 0;

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

		if (strcmp(password, pw_old.c_str())) {
			gotoxy(32, 10, COLOR_RED); std::cout << "Current Password is incorrect.";
			PAUSE; continue;
		}
		if (pw_new.size() < 5) {
			gotoxy(32, 12, COLOR_RED); std::cout << "The Password field must be least 5 characters.";
			PAUSE; continue;
		}
		if (pw_new != pw_new_confirm) {
			gotoxy(32, 14, COLOR_RED); std::cout << "The Confirm Password confirmation does not match.";
			PAUSE; continue;
		}
		break;
	}

	// Update "account.csv"
	file::update(".\\data\\account.csv", user.id, 2, pw_new.c_str());
	gotoxy(36, 17, COLOR_GREEN); std::cout << "Save changes successfully.";

	// Update current user
	csv_file user_list(".\\data\\account.csv");
	user = user_list.data[user.id];

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
	char* password = user.pdata[2];
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

		if (strcmp(password, pw_old.c_str())) {
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
	file::update(".\\data\\account.csv", user.id, 2, pw_new.c_str());
	gotoxy(46, 27, COLOR_GREEN); std::cout << " Save changes successfully. ";

	// Update current user
	csv_file user_list(".\\data\\account.csv");
	user = user_list.data[user.id];

	PAUSE; return 1;
}

// [Main menu]::allclass //=====================================================================================================================//

void role::staff(csv_line& user) {
	std::ifstream inp(".\\layout\\menu.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("menu.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	layout menu_info_layout(inp);
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
		std::cout << infouser->pdata[3];
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
		int E = 4;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Classes         ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Courses         ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Settings        ";
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
				role::cls(user);
				goto MENU;
			}
			if (choose == 2) {
				gotoxy(2, 11, 8); std::cout << "  Courses         ";
				role::crs(user);
				goto MENU;
			}
			if (choose == 3) {
				gotoxy(2, 12, 8); std::cout << "  Settings        ";
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
	std::ifstream inp(".\\layout\\menu.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("menu.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	layout menu_info_layout(inp);
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
		colorizing(COLOR_BLUE); std::cout << infouser->pdata[4] << '.' << infouser->pdata[3];
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
				role::crs(user);
				
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
	std::ifstream inp(".\\layout\\menu.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("menu.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	layout menu_info_layout(inp);
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
		if (ENGLISHNAME) {
			std::cout << infouser->pdata[3] << ' ' << infouser->pdata[2];
		}
		else {
			std::cout << infouser->pdata[2] << ' ' << infouser->pdata[3];
		}
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
				if ((choose += role::crs(user)) != 1) submenu_change = 1;
				continue;
			}
			if (choose == 2) {
				gotoxy(2, 11, 8); std::cout << "  My calendar     ";
				if ((choose += npstudent::calendar(user)) != 2) submenu_change = 1;
				continue;
			}
			if (choose == 3) {
				gotoxy(2, 12, 8); std::cout << "  My attendance   ";
				if ((choose += npattendance::student(user)) != 3) submenu_change = 1;
				continue;
			}
			if (choose == 4) {
				gotoxy(2, 13, 8); std::cout << "  My scoreboard   ";
				if ((choose += npscoreboard::student(user)) != 4) submenu_change = 1;
				continue;
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

int role::cls(csv_line& user) {
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
		if (c == KEY_ESC) break;
		if (KEY_HELP(c));
		if (c == KEY_SEARCH) {
			int old = choose; std::string search;
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
			gotoxy(32, 16, 128); std::cout << "                                                         ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
				std::transform(search.begin(), search.end(), search.begin(), ::tolower);
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;
					if (strstr(class_list.data[choose].pdata[1], search.c_str()) || (class_list.data[choose].count > 2 &&
						strstr(class_list.data[choose].pdata[2], search.c_str()))) goto SUCCESS_SEARCH;
				} while (choose != old);

				std::transform(search.begin(), search.end(), search.begin(), ::toupper);
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;
					if (strstr(class_list.data[choose].pdata[1], search.c_str()) || (class_list.data[choose].count > 2 &&
						strstr(class_list.data[choose].pdata[2], search.c_str()))) goto SUCCESS_SEARCH;
				} while (choose != old);
			}

		SUCCESS_SEARCH:
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			continue;
		}
		cls = &class_list.data[choose];
		if (c == KEY_ENTER) {

			goto NO_CHANGE;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_RIGHT) {
				npstudent::list(user, cls->pdata[1]);
				goto LAYOUT;
			}
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}
	return 0;
}

int role::crs(csv_line& user) {
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
	// Title
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "  Course list    ";
	if (user == "student") { colorizing(COLOR_YELLOW); std::cout << "  Calendar       " << "  Attendance     " << "  Scoreboard    "; }
	else { colorizing(COLOR_YELLOW); std::cout << "  Student list   " << "  Attendance     " << "  Scoreboard    "; }
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Course ID    | Class     | Lecturer ID  | Room   | Status ";
	while ((cur = -1)) {
		csv_file course_list((COURSE_PATH("__course.csv")).c_str(), def_course);
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
				if (course->pdata[0][0] == '0') {
					gotoxy(87, y, COLOR_CODE + COLOR_RED); std::cout << "private";
				}
				else {
					gotoxy(87, y, COLOR_CODE + 2); std::cout << "public";
				}
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
		if (c == KEY_ESC) break;
		if (KEY_HELP(c));
		if (c == KEY_SEARCH) {
			int old = choose; std::string search;
			gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
			gotoxy(32, 16, 128); std::cout << "                                                         ";
			gotoxy(32, 17, 128); std::cout << "                                                         ";
			if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
				std::transform(search.begin(), search.end(), search.begin(), ::tolower);
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;
					if (strstr(course_list.data[choose].pdata[1], search.c_str()) ||
						strstr(course_list.data[choose].pdata[3], search.c_str()) ||
						strstr(course_list.data[choose].pdata[4], search.c_str()) ||
						strstr(course_list.data[choose].pdata[10], search.c_str())) goto SUCCESS_SEARCH;
					if (search.empty() && permit[choose]) goto SUCCESS_SEARCH;
				} while (choose != old);

				std::transform(search.begin(), search.end(), search.begin(), ::toupper);
				do {
					if (choose < cur) { if (++choose < cur - 16) overflow--; }
					else choose = overflow = 0;
					if (strstr(course_list.data[choose].pdata[1], search.c_str()) ||
						strstr(course_list.data[choose].pdata[3], search.c_str()) ||
						strstr(course_list.data[choose].pdata[4], search.c_str()) ||
						strstr(course_list.data[choose].pdata[10], search.c_str())) goto SUCCESS_SEARCH;
				} while (choose != old);
			}

		SUCCESS_SEARCH:
			gotoxy(32, 15); std::cout << "                                                         ";
			gotoxy(32, 16); std::cout << "                                                         ";
			gotoxy(32, 17); std::cout << "                                                         ";
			continue;
		}
		course = &course_list.data[choose];
		if (c == KEY_ENTER) {
			if (user == "staff" || (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0)) {
				npcourse::info(user, course->pdata[1], course->pdata[3]);
				goto LAYOUT;
			}
			if (user == "student") {
				npcourse::chkin(user, course->pdata[1], course->pdata[3]);
				continue;
			}
			goto NO_CHANGE;
		}
		if (KEY_EROL(c)) {
			npcourse::enrol(user, course->pdata[1], course->pdata[3]);
			continue;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) { if (--choose + overflow < 0) overflow++; }
			else if (c == KEY_DOWN && choose < cur) { if (++choose < cur - 16) overflow--; }
			else if (c == KEY_RIGHT) {
				if (user == "staff" || (user == "lecturer" && strcmp(user.pdata[1], course->pdata[4]) == 0)) {
					npstudent::list(user, course->pdata[1], course->pdata[3]);
					goto LAYOUT;
				}
				if (user == "student") {
					delete[] permit;
					return 1;
				}
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
	if (chsAS == 1) { gotoxy(72, 18, 192); std::cout << "[Spring]"; }
	if (chsAS == 2) { gotoxy(72, 18, 160); std::cout << "[Summer]"; }
	if (chsAS == 3) { gotoxy(72, 18, 224); std::cout << "[Autumn]"; }
	if (chsAS == 4) { gotoxy(72, 18, 176); std::cout << "[Winter]"; }

	while (1) {
		if (chsEN) { gotoxy(72, 12, COLOR_GREEN_BACKGROUND); std::cout << "[ON]"; colorizing(COLOR_DEFAULT); std::cout << ' '; }
		else { gotoxy(72, 12, COLOR_RED_BACKGROUND); std::cout << "[OFF]"; }

		uint8_t c = getch();
		if (c == KEY_ESC) return;
		if (c == KEY_ENTER) break;
		if (c == 224 || c == 0) {
			if ((c = getch()) == KEY_LEFT || c == KEY_RIGHT) {
				chsEN = 1 - chsEN;
			}
		}
	}
	read(72, 15, COLOR_DEFAULT, chsAY, 4, SHOW, ACADEMICYEAR.c_str());
	while (1) {
		if (chsAS == 1) { gotoxy(72, 18, 192); std::cout << "[Spring]"; }
		if (chsAS == 2) { gotoxy(72, 18, 160); std::cout << "[Summer]"; }
		if (chsAS == 3) { gotoxy(72, 18, 224); std::cout << "[Autumn]"; }
		if (chsAS == 4) { gotoxy(72, 18, 176); std::cout << "[Winter]"; }

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
