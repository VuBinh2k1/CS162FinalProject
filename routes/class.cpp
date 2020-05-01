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
	gotoxy(33, 9, COLOR_YELLOW_BACKGROUND); std::cout << " No.   | Class name | Information                      ";

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

bool npclass::change(csv_line& user, const char* from, const char* to) {
	std::string class1 = from;
	std::string class2 = to;
	std::string path1, path2;
	std::transform(class1.begin(), class1.end(), class1.begin(), ::toupper);
	std::transform(class2.begin(), class2.end(), class2.begin(), ::toupper);
	path1 = (std::string)(".\\data\\class\\") + class1 + ".csv";
	path2 = (std::string)(".\\data\\class\\") + class2 + ".csv";

	csv_line* student = nullptr;

	if (exists(path1.c_str())) {
		csv_file file1(path1.c_str());
		student = npcsv::exists(file1, user.pdata[1]);

		if (student == nullptr) {
			gotoxy(38, 27, COLOR_RED); std::cout << "This student could not be found in " << class1 << ".";
			PAUSE; return 0;
		}
		if (student->pdata[0][0] == '0') {
			gotoxy(38, 27, COLOR_RED); std::cout << "This student had been removed from " << class1 << ".";
			PAUSE; return 0;
		}

		// Update class1.csv
		npcsv::update(path1.c_str(), student->id, 0, "0");
	}

	// Update class2.csv
	if (exists(path2.c_str())) {
		csv_file file2(path2.c_str());
		student = npcsv::exists(file2, user.pdata[1], 0);

		if (student != nullptr) {
			npcsv::update(path2.c_str(), student->id, 0, "1");
			return 1;
		}
		std::ofstream out(path2, std::ios::app);
		out << "1," << user.pdata[1] << '\n';
		out.close();
		return 1;
	}
	std::ofstream out(path2);
	out << "Status,Student ID\n";
	out << "1," << user.pdata[1] << '\n';
	out.close();
	return 1;
}
void npclass::studentlist()
{
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return;
	}
	layout minibox_layout(inp);
	inp.close();
	minibox_layout.print();
	std::string class1;
	gotoxy(33, 9);
	std::cout << "Input the class you want to view: "; std::cin.ignore('\0'); getline(std::cin, class1);
	std::transform(class1.begin(),class1.end(),class1.begin(),::toupper);	
	std::string filename;
	filename = (std::string)(".\\data\\class\\") + class1 + ".csv";
	gotoxy(33, 9, COLOR_YELLOW_BACKGROUND); std::cout << " No.   |    Status     |      ID                       ";
	csv_file file1(filename.c_str());
	int choose = 0;
	while (1)
	{
		int j = 0;
		for (int i = choose; i < min(choose + 18, file1.count);++i)
		{
			int y = 10 + j - choose;
			WORD COLOR_CODE = (i % 2) ? 112 : 240;
			csv_line* student = &file1.data[i];
			if (atoi(file1.data[i].pdata[0]) == 1)
			{
				gotoxy(33, y, COLOR_CODE); std::cout << "       |               |                               ";
				gotoxy(34, y, COLOR_CODE); std::cout << j;
				gotoxy(43, y, COLOR_CODE); std::cout << file1.data[i].pdata[0];
				gotoxy(57, y, COLOR_CODE); std::cout << file1.data[i].pdata[1];
				j++;
			}
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) break;
		if (c == KEY_ENTER);
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < file1.count - 18) choose++;
			else if (c == KEY_LEFT) break;
			else goto NO_CHANGE;
		}
	}
}