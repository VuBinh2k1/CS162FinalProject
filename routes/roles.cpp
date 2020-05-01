#include "roles.h"

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
	for(std::string username, password;;) {
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

		WORD COLOR_CODE;
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

		if (read(32,  9, COLOR_DEFAULT, pw_old, 52, HIDE) == KEY_ESC) return 0;
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
