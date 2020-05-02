#include "class.h"

void npclass::info() {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();

	minibox_layout.print();
	gotoxy(33, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                      Class list                       ";
	gotoxy(33, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Class name | Information                      ";
	

	csv_file class_list(".\\data\\class\\__class.csv");
	int choose = 0;
	while (1) {
		for (int i = choose; i < min(choose + 18, class_list.count); i++)
		{
			int y = 10 + i - choose;
			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			csv_line* clas = &class_list.data[i];

			gotoxy(33, y, COLOR_CODE); std::cout << "       |            |                                  ";
			gotoxy(34, y, COLOR_CODE); std::cout << i;
			gotoxy(42, y, COLOR_CODE); std::cout << class_list.data[i].pdata[1];
			if (class_list.data[i].count > 2) {
				gotoxy(55, y, COLOR_CODE); std::cout << class_list.data[i].pdata[2];
			}
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < class_list.count - 18) choose++;
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
		}
	}
}

bool npclass::change(csv_line& user, const char* from, const char* to) {
	csv_line* student = nullptr;
	
	// Update class1.csv	[NULL: dont have class]
	std::string name = from;
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	std::string path = (std::string)(".\\data\\class\\") + name + ".csv";
	if (name.size()) {
		csv_file file1(path.c_str(), def_class);
		student = file::exists(file1, user.pdata[1]);
		if (student == nullptr) {
			gotoxy(38, 27, COLOR_RED); std::cout << "This student could not be found in " << name << ".";
			PAUSE; return 0;
		}
		if (student->pdata[0][0] == '0') {
			gotoxy(38, 27, COLOR_RED); std::cout << "This student had been removed from " << name << ".";
			PAUSE; return 0;
		}
		file::update(path.c_str(), student->id, 0, "0");
	}

	// Update class2.csv
	name = to;
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	path = (std::string)(".\\data\\class\\") + name + ".csv";
	csv_file file2(path.c_str(), def_class);
	student = file::exists(file2, user.pdata[1], 0);
	if (student != nullptr) {
		file::update(path.c_str(), student->id, 0, "1");
		return 1;
	}
	std::ofstream out(path, std::ios::app);
	out << "1," << user.pdata[1] << '\n';
	out.close();
	return 1;
}

void npclass::studentlist() {
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();
	csv_line* csinfo = nullptr;
	std::string csname;

ENTER_CLASS_ID:
	while (1) {
		minibox_layout.print();
		gotoxy(54, 9, COLOR_YELLOW);  std::cout << "Student list";
		gotoxy(33, 11); std::cout << "Class ID    : ";
		if (read(47, 11, COLOR_DEFAULT, csname, 8, SHOW) == KEY_ESC) return;
		if (csname.empty()) continue;
		std::transform(csname.begin(), csname.end(), csname.begin(), ::toupper);

		csv_file infofile(".\\data\\class\\__class.csv");
		if ((csinfo = file::exists(infofile, csname.c_str(), 0)) == nullptr) {
			gotoxy(47, 11, COLOR_RED); std::cout << "this class is does not exist.";
			PAUSE; continue;
		}
		break;
	}
	
	minibox_layout.print();
	gotoxy(33, 8, COLOR_YELLOW_BACKGROUND); std::cout << "                                                       ";
	gotoxy(54, 8, COLOR_YELLOW_BACKGROUND); std::cout << csname;
	gotoxy(33, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Student ID | Information                      ";

	csv_file info(((std::string)".\\data\\class\\" + csname + ".csv").c_str(), def_class);
	int choose = 0, cur = -1;
	while (1) {
		cur = -1;
		for (int i = 0; i < info.count; ++i) {
			if (info.data[i].pdata[0][0] == '0') continue;
			int y = 10 + (++cur) - choose;
			if (y < 10 || y > 27) continue;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			csv_line* student = &info.data[i];
			gotoxy(33, y, COLOR_CODE); std::cout << "       |            |                                  ";
			gotoxy(34, y, COLOR_CODE); std::cout << cur;
			gotoxy(42, y, COLOR_CODE); std::cout << info.data[i].pdata[1];
			if (info.data[i].count > 2) {
				gotoxy(55, y, COLOR_CODE); std::cout << info.data[i].pdata[2];
			}
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER) goto NO_CHANGE;
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < cur - 17) choose++;
			else if (c == KEY_LEFT) goto ENTER_CLASS_ID;
			else goto NO_CHANGE;
		}
	}
}