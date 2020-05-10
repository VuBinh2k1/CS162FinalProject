#include "..\\controls\\class.h"

bool npclass::change(csv_line& user, const char* from, const char* to) {
	csv_line* student = nullptr;
	
	// Update class1.csv	[NULL: dont have class]
	std::string name = from;
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	std::string path = (std::string)(".\\data\\class\\") + name + ".csv";
	if (name.size()) {
		csv_file file1(path.c_str(), def_class);
		student = file::find(file1, user.pdata[1], nullptr, ON);
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
	student = file::find(file2, user.pdata[1], nullptr, ON);
	if (student != nullptr) {
		file::update(path.c_str(), student->id, 0, "1");
		return 1;
	}
	std::ofstream out(path, std::ios::app);
	out << "1," << user.pdata[1] << '\n';
	out.close();
	return 1;
}
