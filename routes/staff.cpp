#include "staff.h"
#include<string>
#include <sstream>
void editstudent(csv_line& user);

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
	colorizing(COLOR_DEFAULT); system("cls");
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
		int E = 4;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit student    ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit courses    ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Scoreboard      ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) if (choose != E) choose = E; else goto NO_CHANGE;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				role::profile(user);
				goto MENU;
			}
			if (choose == 1) {
				npstaff::edit_student();
				goto MENU;
			}
			if (choose == 2) {
				
				goto MENU;
			}
			if (choose == 3) {
				
				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < 4) choose++;
			else goto NO_CHANGE;
		}
	}
}

void editstudent(csv_line& user)
{
	system("cls");
	csv_file infofile(".\\data\\student\\__student.csv");
	int count = 0;
	std::cout << "Enter student ID you want to change: ";
	std::string chr;
	std::getline(std::cin, chr);
	for (int i = 0; i < infofile.count; i++) {
		char* ID = infofile.data[i].pdata[1];
		if (strcmp(chr.c_str(), ID) == 0)
		{
			count++;
			std::cout << "Student ID: " << infofile.data[i].pdata[1] << std::endl;
			std::cout << "Last name: " << infofile.data[i].pdata[2] << std::endl;
			std::cout << "First name: " << infofile.data[i].pdata[3] << std::endl;
			std::cout << "Day of birth: " << infofile.data[i].pdata[4] << std::endl;
			std::cout << "What infomation do you want to change: \n1 - Student ID	  2 - First name     3 - Last name	 	 4 - Gender      5 - DoB\n";
			int Key;
			std::cin >> Key;
			std::cin.ignore(100, '\n');
			switch (Key)
			{
			case 1: {
				std::string result;
				std::ostringstream convert;
				int temp;				
				std::cout << "Enter new ID: ";
				std::cin >> temp;
				convert << temp;
				result = convert.str();
				if (temp == 0) {
					std::cout << "Sorry but student ID cannot = 0 ";
				}
				npcsv::update(".\\data\\student\\__student.csv", i, 1, result.c_str());
				std::cout << "Successfully!!";
				system("cls");
				break;
			}
			case 2: {
				std::string temp;
				std::cout << "Enter new first name: ";
				std::getline(std::cin, temp);
				npcsv::update(".\\data\\student\\__student.csv", i, 2, temp.c_str());
				std::cout << "Successfully.\n";
				system("cls");
				break;
			}
			case 3: {
				std::string temp;
				std::cout << "Enter new last name: ";				
				std:getline(std::cin, temp);
				npcsv::update(".\\data\\student\\__student.csv", i, 3, temp.c_str());
				std::cout << "Successfully.\n";
				system("cls");
				break;
			}
			case 4: {
				std::string temp;
				std::cout << "Enter new Gender: ";
				std::getline(std::cin, temp);
				npcsv::update(".\\data\\student\\__student.csv", i, 4, temp.c_str());
				std::cout << "Successfully.\n";
				system("cls");
				break;
			}
			case 5: {
				std::string temp;
				std::cout << "Enter new DoB (DD/MM/YYYY): ";				
				std::getline(std::cin, temp);
				npcsv::update(".\\data\\student\\__student.csv", i, 5, temp.c_str());
				std::cout << "Successfully.\n";
				system("cls");
				break;
			}
			default:
				break;
			}
			break;
		}
	}
	if (count == 0) {
		std::cout << "Cannot find student ID you want!!!!";
	}
	else {

	}
}

void removestudent(csv_line& user)
{
	system("cls");
	csv_file infofile(".\\data\\student\\__student.csv");
	int count = 0;
	std::cout << "Enter student ID you want to change: ";
	std::string chr;
	std::getline(std::cin, chr);
	for (int i = 0; i < infofile.count; i++) {
		char* ID = infofile.data[i].pdata[1];
		if (strcmp(chr.c_str(), ID) == 0)
		{
			count++;
			if (infofile.data[i].pdata[0] == 0) {
				std::cout << "remove roi con doi remove nua -_-* \n";
			}
			npcsv::update(".\\data\\student\\__student.csv", i, 0, 0);
			std::cout << "Successfully. \n";
		}
	}
	if (count == 0)
		std::cout << "cannot find student ID!!!!!! \n";
}

void npstaff::edit_student() {
	std::ifstream inp(".\\layout\\student.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("student.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout student_layout(inp);
	layout student_info_layout(inp);
	inp.close();

	int choose = 0;

	colorizing(COLOR_DEFAULT); system("cls");
	student_layout.print();

MENU:
	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "   Edit student   ";
	while (1) {
		int E = 6;	// END MENU
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Classes list    ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  View student    ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Add student     ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit info       ";
		gotoxy(2,13, (choose == 4) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Change class    ";
		gotoxy(2,14, (choose == 5) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Remove student  ";
		gotoxy(2,28, (choose == E) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "    Main menu     ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) if (choose != E) choose = E; else goto NO_CHANGE;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				student_info_layout.print();
				gotoxy(2, 9); std::cout << "  Classes list    ";
				npclass::info();
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
			if (choose == 4) {
				student_info_layout.print();
				gotoxy(2, 13); std::cout << "  Change class    ";
				npclass::change();
				goto MENU;
			}
			if (choose == 5) {

				goto MENU;
			}
			if (choose == E) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < 6) choose++;
			else goto NO_CHANGE;
		}
	}
}
