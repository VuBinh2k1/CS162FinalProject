#include "..\\controls\\staff.h"

void npstaff::menu_class() {
	std::ifstream inp(".\\layout\\student.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("student.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout student_layout(inp);
	inp.close();

	int choose = 0;

	colorizing(COLOR_DEFAULT); system("cls");
	student_layout.print();
	academicmark();

MENU:
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "   Edit student   ";
	while (1) {
		int E = 4;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Classes list    ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Students list   ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  New student     ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Info student    ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "    Main menu     ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) if (choose != E) choose = E; else goto NO_CHANGE;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				gotoxy(2, 9); std::cout << "  Classes list    ";
				npclass::list();
				goto MENU;
			}
			if (choose == 1) {
				gotoxy(2, 10); std::cout << "  Students list    ";
				npclass::studentlist();
				goto MENU;
			}
			if (choose == 2) {

				goto MENU;
			}
			if (choose == 3) {
				gotoxy(2, 12); std::cout << "  Info student    ";
				npstudent::info();
				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < E) choose++;
			else goto NO_CHANGE;
		}
	}
}

void npstaff::settings() {
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
