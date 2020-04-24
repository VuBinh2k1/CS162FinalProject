#include "roles.h"

char char_pass(int x, int y, std::string& pw) {
	gotoxy(x, y);
	uint8_t c = -1;
	pw.resize(0);
	while (c != KEY_ENTER && c != KEY_ESC) {
		c = getch();
		// Keyboard: Printable characters
		if (c > 32 && c < 127) {
			if (pw.size() < 52) {
				std::cout << "*"; pw.push_back(c);
			}
			continue;
		}
		// Keyboard: BACKSPACE: 8
		if (c == KEY_BACKSPACE && pw.size()) {
			std::cout << "\b \b"; pw.pop_back();
			continue;
		}
		// Keyboard: DELETE: 224 83
		if (c == 0 || c == 224) {
			if (getch() == 83) {
				while (pw.size()) {
					std::cout << "\b \b"; pw.pop_back();
				}
			}
			continue;
		}
	}
	return c;
}

void login(csv_line& user) {
	system("cls");

	std::ifstream inp(".\\layout\\login.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("login.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout login_layout(inp);
	layout user_pass_layout(inp);
	layout error_layout(inp);
	inp.close();

	csv_file user_list(".\\data\\account.csv", 4);

	login_layout.print();
	user_pass_layout.print();
	for(std::string username, password;;) {
		char temp[20];

		gotoxy(24, 13);
		std::cin.get(temp, 20); username = temp;
		std::cin.clear(); std::cin.ignore(100, '\n');
		char_pass(24, 14, password);

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
		error_layout.print(); 
		Sleep(2000);
		user_pass_layout.print();
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
	inp.close();

	char* username = user.pdata[1];
	char* password = user.pdata[2];
	std::string pw_old;
	std::string pw_new;
	std::string pw_new_confirm;

	while (1) {
		password_layout.print();

		if (char_pass(32, 9, pw_old) == KEY_ESC) return;
		if (char_pass(32, 11, pw_new) == KEY_ESC) return;
		if (char_pass(32, 13, pw_new_confirm) == KEY_ESC) return;

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
			gotoxy(32, 10, COLOR_RED); std::cout << "Current password is incorrect";
			Sleep(2000); continue;
		}
		if (pw_new != pw_new_confirm) {
			gotoxy(32, 14, COLOR_RED); std::cout << "The Confirm Password confirmation does not match";
			Sleep(2000); continue;
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
	gotoxy(0, 20); Sleep(2000);
}
