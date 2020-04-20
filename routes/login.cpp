#include "login.h"
#include "layout.h"

void login(csv_line& user) {
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

	for(char* username, *password;;) {
		char temp[20];

		gotoxy(24, 13);
		std::cin.get(temp, 20); username = strdup(temp);
		std::cin.clear(); std::cin.ignore(100, '\n');

		gotoxy(24, 14); 
		std::cin.get(temp, 20); password = strdup(temp);
		std::cin.clear(); std::cin.ignore(100, '\n');

		for (int i = 0; i < user_list.count; ++i) {

			char* _username = user_list.data[i].pdata[1];
			char* _password = user_list.data[i].pdata[2];
			if (strcmp(username, _username) == 0 && 
				strcmp(password, _password) == 0) {
				if (username) free(username);
				if (password) free(password);
				user.init(user_list.data[i]);

				colorizing(default_code);
				return;
			}
		}
		error_layout.print(); 
		Sleep(2000);
		user_pass_layout.print();

		if (username) free(username);
		if (password) free(password);
	}
}