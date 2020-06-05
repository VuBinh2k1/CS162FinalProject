#include "..\\controls\\class.h"

// [Function]::comparator //====================================================================================================================//

bool npclass::sort_cmp_class(const char* x, const char* y) {
	// CLASS ID: year_type_num (Ex: 19 - CLC - 1 = 19CLC1;
	std::string str[2]; str[0] = x; str[1] = y;
	std::string year[2], type[2], num[2];
	for (int t = 0; t < 2; ++t) {	// Convert const char* to std::string year - type - num
		std::string* cur = &year[t];
		for (int i = 0; i < str[t].size(); ++i) {
			if (cur == &year[t]) {
				if (str[t][i] >= '0' && str[t][i] <= '9') *cur += str[t][i];
				else cur = &type[t];
			}
			if (cur == &type[t]) {
				if ((str[t][i] >= 'A' && str[t][i] <= 'Z')) *cur += str[t][i];
				else cur = &num[t];
			}
			if (cur == &num[t]) {
				if (str[t][i] >= '0' && str[t][i] <= '9') *cur += str[t][i];
				else break;
			}
		}
	}

	if (std::stoi(year[0]) != std::stoi(year[1])) return std::stoi(year[0]) <= std::stoi(year[1]);
	if (type[0] != type[1]) return type[0] <= type[1];
	if (std::stoi(num[0]) != std::stoi(num[1])) return std::stoi(num[0]) <= std::stoi(num[1]);
	return 1;
}

// [EDIT]::class //=============================================================================================================================//

void npclass::add() {
	gotoxy(32, 17, 128); std::cout << "                                                         ";
NEW:
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " New                                                     ";
	gotoxy(32, 16, 128); std::cout << " Class name:                                             ";

	std::string class_id;
	if (read(45, 16, 128, class_id, 10, SHOW) == KEY_ESC) return;
	uppercase(class_id);
	gotoxy(32, 17, 128); std::cout << "                                                         ";

	if (file::find(__CLASS, class_id.c_str(), nullptr, OFF) != -1) {
		gotoxy(33, 17, 132); std::cout << "This class already exists.";
		goto NEW;
	}
	
	std::ofstream out(__CLASS, std::ios::app);
	out << "1," << class_id << '\n';
	out.close();
	gotoxy(33, 17, 129); std::cout << "Add class success.";
	PAUSE;
}

bool npclass::sort() {
	gotoxy(23, 28, 8); std::cout << "(Ctrl + \\) was pressed. Waiting for second key of chord...";
	uint8_t c = getch();
	if (c == '1') file::sort(__CLASS, 1, npclass::sort_cmp_class);	// sort: class name
	else {
		gotoxy(23, 28); std::cout << "                                                          ";
		return 0;
	}
	return 1;
}

bool npclass::remove(const char* class_id) {
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Delete                                                  ";
	gotoxy(32, 16, 128); std::cout << " Class name:                                             ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(45, 16, 128); std::cout << class_id;
	gotoxy(33, 17, 132); std::cout << "Are you sure to remove this class, cannot be undone.";
	for (int choose = 1;;) {
		gotoxy(51, 18, (choose == 0) ? COLOR_RED_BACKGROUND : 128); std::cout << " Remove ";
		gotoxy(60, 18, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Cancel ";

		uint8_t c = getch();
		if (c == KEY_ESC) return 0;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				file::remove(__CLASS, file::find(__CLASS, class_id, nullptr, OFF));
				gotoxy(46, 18, 128 + COLOR_BLUE); std::cout << " Remove successfully.";
				PAUSE; return 1;
			}
			return 0;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0) choose++;
		}
	}
}

void npclass::search(csv_file& class_list, int cur, int& choose, int& overflow) {
	csv_line* cls = nullptr;
	int old = choose; std::string search;
	gotoxy(32, 15, COLOR_BLUE_BACKGROUND);  std::cout << " Search                                                  ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	if (read(33, 16, 128, search, 55, SHOW) != KEY_ESC) {
		lowercase(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			cls = &class_list.data[choose];
			if (strstr(cls->pdata[1], search.c_str()) || (cls->count > 2 &&
				strstr(cls->pdata[2], search.c_str()))) return;
		} while (choose != old);

		uppercase(search);
		do {
			if (choose < cur) { if (++choose < cur - 16) overflow--; }
			else choose = overflow = 0;

			cls = &class_list.data[choose];
			if (strstr(cls->pdata[1], search.c_str()) || (cls->count > 2 &&
				strstr(cls->pdata[2], search.c_str()))) return;
		} while (choose != old);
	}
}

bool npclass::change(csv_line& user, const char* class_from, const char* class_to) {
	std::string fm = class_from;
	std::string to = class_to;
	uppercase(fm);
	uppercase(to);
	if (fm == to) return 1;
	if (to.size()) { if (file::find(__CLASS, to.c_str(), nullptr, OFF) == -1) return 0; }
	else return 1;

	// Update class1.csv	[NULL: dont have class]
	if (fm.size() && file::find(__CLASS, fm.c_str(), nullptr, OFF) != -1) {
		file::remove(CLASS(fm), file::find(CLASS(fm), user.pdata[1], nullptr, OFF));
	}

	// Update class2.csv
	csv_file student_list(CLASS(to), def_class);
	csv_line* student = file::find(student_list, user.pdata[1], nullptr, OFF);
	if (student != nullptr) {
		file::update(CLASS(to), student->id, 0, "1");
		return 1;
	}
	std::ofstream out(CLASS(to), std::ios::app);
	out << "1," << user.pdata[1] << '\n';
	out.close();
	return 1;
}
