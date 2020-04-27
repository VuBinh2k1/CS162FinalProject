#include "class.h"

void npclass::info() {
	csv_file class_list(".\\data\\class\\__class.csv");
	gotoxy(33, 9, COLOR_YELLOW_BACKGROUND);
	std::cout << " No.   | Class name | Information                      ";
	int choose = 0;
	while (1) {
		for (int i = choose; i < min(choose + 18, class_list.count); i++)
		{
			int y = 10 + i - choose;
			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			csv_line* clas = &class_list.data[i];

			gotoxy(33, y, COLOR_CODE); std::cout << "       |            |                                  ";
			gotoxy(34, y, COLOR_CODE); std::cout << class_list.data[i].pdata[0];
			gotoxy(42, y, COLOR_CODE); std::cout << class_list.data[i].pdata[1];
			if (class_list.data[i].count > 2) {
				gotoxy(55, y, COLOR_CODE);
				std::cout << class_list.data[i].pdata[2];
			}
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER);
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < class_list.count - 18) choose++;
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
		}
	}
}

void npclass::change() {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();
	
	std::string id, class1, class2, path1, path2;
	while (1) {
		minibox_layout.print();
		gotoxy(48, 9, COLOR_YELLOW);  std::cout << "Change class of a student";
		gotoxy(33, 12); std::cout << "Student ID  :";
		gotoxy(33, 15); std::cout << "From class  :";
		gotoxy(33, 18); std::cout << "To class    :";
		gotoxy(49, 27); std::cout << "[Save change] [Cancel]";

		if (read(47, 12, id, 8, SHOW) == KEY_ESC) return;
		if (read(47, 15, class1, 20, SHOW) == KEY_ESC) return;
		if (read(47, 18, class2, 20, SHOW) == KEY_ESC) return;
		std::transform(class1.begin(), class1.end(), class1.begin(), ::toupper);
		std::transform(class2.begin(), class2.end(), class2.begin(), ::toupper);

		path1 = (std::string)(".\\data\\class\\") + class1 + ".csv";
		path2 = (std::string)(".\\data\\class\\") + class2 + ".csv";

		// Choose Left-right: [Save change][Cancel]
		for (WORD C1 = COLOR_WHITE_BACKGROUND, C2 = COLOR_WHITE;;) {
			gotoxy(49, 27, C1); std::cout << "[Save change]";
			gotoxy(63, 27, C2);  std::cout << "[Cancel]";

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

		if (!exists(path1.c_str())) {
			gotoxy(47, 16, COLOR_RED); std::cout << "Can't find this class.";
			PAUSE; continue;
		}

		csv_line* student = nullptr;
		csv_file file1(path1.c_str());
		for (int i = 0; i < file1.count; ++i) {
			if (strcmp(file1.data[i].pdata[1], id.c_str()) == 0) {
				student = &file1.data[i];
			}
				
		}
		if (student == nullptr){
			gotoxy(38, 27, COLOR_RED); std::cout << "This student could not be found in " << class1 << ".";
			PAUSE; continue;
		}
		if (student->pdata[0][0] == '0') {
			gotoxy(38, 27, COLOR_RED); std::cout << "This student had been removed from " << class1 << ".";
			PAUSE; continue;
		}

		// Update class1.csv
		npcsv::update(path1.c_str(), student->id, 0, "0");

		// Update class2.csv
		if (exists(path2.c_str())) {
			student = nullptr;
			csv_file file2(path2.c_str());
			for (int i = 0; i < file2.count; ++i) {
				if (strcmp(file2.data[i].pdata[1], id.c_str()) == 0) {
					student = &file2.data[i];
				}

			}
			if (student != nullptr) {
				npcsv::update(path2.c_str(), student->id, 0, "1");
				break;
			}
		}
		std::ofstream out(path2, std::ios::app);
		out << "1," << id << '\n';
		out.close();
		break;
	}
	gotoxy(47, 27, COLOR_GREEN); std::cout << "Save changes successfully.";
	PAUSE;
}
