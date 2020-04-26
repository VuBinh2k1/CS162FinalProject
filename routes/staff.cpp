#include "staff.h"

void npstaff::menu(csv_line& user) {
	std::ifstream inp(".\\layout\\menu.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("menu.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	layout menu_info_layout(inp);
	inp.close();

	csv_file infofile(".\\data\\staff\\__staff.csv");
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0;

MENU:
	system("cls");
	menu_layout.print();
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
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit student    ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit courses    ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Scoreboard      ";
		gotoxy(2,28, (choose == 4) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

		uint8_t c = getch();
		if (c == KEY_ESC) choose = 4;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				
				goto MENU;
			}
			if (choose == 1) {
				
				goto MENU;
			}
			if (choose == 2) {
				
				goto MENU;
			}
			if (choose == 3) {
				
				goto MENU;
			}
			if (choose == 4) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			if (c == KEY_DOWN && choose < 4) choose++;
		}
	}
}
