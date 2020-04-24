#include "roles.h"

void login(csv_line& user) {
	system("cls");

	std::ifstream inp(".\\layout\\login.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("login.layout is not exist"), TEXT("error layout"), MB_OK); 
		exit(0);
	}
	layout login_layout(inp);
	layout user_pass_layout(inp);
	inp.close();

	csv_file user_list(".\\data\\account.csv", 4);

	login_layout.print();
	for(std::string username, password;;) {
	LOGIN:

		user_pass_layout.print();
		if (read(24, 12, username, SHOW) == KEY_ESC) EXIT(0,20);
		if (read(24, 13, password, HIDE) == KEY_ESC) EXIT(0,20);

		// Choose Left-right: [Login][Cancel]
		for (WORD C1 = COLOR_WHITE_BACKGROUND, C2 = COLOR_WHITE;;) {
			gotoxy(40, 17, C1); std::cout << "[Login]";
			gotoxy(48, 17, C2);  std::cout << "[Cancel]";
			colorizing(COLOR_DEFAULT);

			uint8_t c = getch();
			if (c == KEY_ESC) EXIT(0,20);
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
					WORD temp = C1;
					C1 = C2;
					C2 = temp;
				}
			}
		}

		for (int i = 0; i < user_list.count; ++i) {
			char* _username = user_list.data[i].pdata[1];
			char* _password = user_list.data[i].pdata[2];
			if (strcmp(username.c_str(), _username) == 0 && 
				strcmp(password.c_str(), _password) == 0) {
				user.init(user_list.data[i]);

				colorizing(COLOR_DEFAULT);
				return;
			}
		}
		gotoxy(33, 17, COLOR_RED); std::cout << "Incorrect username or password";
		PAUSE;
	}
}

void menu(csv_line& user) {

}

void profile(csv_line& user) {

}

void password(csv_line& user) {
	system("cls");
	
	std::ifstream inp(".\\layout\\password.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("password.layout is not exist"), TEXT("error layout"), MB_OK); 
		exit(0);
	}
	layout password_layout(inp);
	layout change_password_layout(inp);
	inp.close();

	char* username = user.pdata[1];
	char* password = user.pdata[2];
	std::string pw_old;
	std::string pw_new;
	std::string pw_new_confirm;

	password_layout.print();
	while (1) {
		change_password_layout.print();

		if (read(32, 9, pw_old, HIDE) == KEY_ESC) return;
		if (read(32, 11, pw_new, HIDE) == KEY_ESC) return;
		if (read(32, 13, pw_new_confirm, HIDE) == KEY_ESC) return;

		// Choose Left-right: [Save change][Cancel]
		for (WORD C1 = COLOR_WHITE_BACKGROUND, C2 = COLOR_WHITE;;) {
			gotoxy(37, 17, C1); std::cout << "[Save change]";
			gotoxy(51, 17, C2);  std::cout << "[Cancel]";
			colorizing(COLOR_DEFAULT);

			uint8_t c = getch();
			if (c == KEY_ESC) return;
			if (c == KEY_ENTER) {
				if (C1 == 240) break;
				else return;
			}
			if (c == 224 || c == 0) {
				if (C1 == 240 && getch() == KEY_RIGHT ||
					C2 == 240 && getch() == KEY_LEFT) {
					WORD temp = C1;
					C1 = C2;
					C2 = temp;
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
	csv_file user_list(".\\data\\account.csv", 4);
	std::ofstream out(".\\data\\account.csv");
	for (int j = 0; j < user_list.mark.count; ++j) {
		out << user_list.mark.pdata[j] << ",";
	}
	out << "\n";
	for (int i = 0; i < user_list.count; out << "\n", ++i) {
		char* _username = user_list.data[i].pdata[1];
		for (int j = 0; j < user_list.mark.count; ++j) {
			if (j == 2 && strcmp(username, _username) == 0) {
				out << pw_new << ",";
			}
			else {
				out << user_list.data[i].pdata[j] << ",";
			}
		}
	}
	out.close();
	gotoxy(36, 17, COLOR_GREEN); std::cout << "Save changes successfully.";
	gotoxy(0, 20); PAUSE;
}
