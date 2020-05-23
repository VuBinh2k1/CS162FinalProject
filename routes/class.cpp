#include "..\\controls\\class.h"

bool npclass::change(csv_line& user, const char* class_from, const char* class_to) {
	std::string fm = class_from;
	std::string to = class_to;
	std::string path;
	std::transform(fm.begin(), fm.end(), fm.begin(), ::toupper);
	std::transform(to.begin(), to.end(), to.begin(), ::toupper);
	if (fm == to) return 1;
	if (to.size()) { if (file::find(__CLASS, to.c_str(), nullptr, OFF) == -1) return 0; }
	else return 1;

	// Update class1.csv	[NULL: dont have class]
	if (fm.size() && file::find(__CLASS, fm.c_str(), nullptr, OFF) != -1) {
		path = (std::string)(".\\data\\class\\") + fm + ".csv";
		file::remove(path.c_str(), file::find(path.c_str(), user.pdata[1], nullptr, OFF));
	}

	// Update class2.csv
	path = (std::string)(".\\data\\class\\") + to + ".csv";
	csv_file student_list(path.c_str(), def_class);
	csv_line* student = file::find(student_list, user.pdata[1], nullptr, OFF);
	if (student != nullptr) {
		file::update(path.c_str(), student->id, 0, "1");
		return 1;
	}
	std::ofstream out(path, std::ios::app);
	out << "1," << user.pdata[1] << '\n';
	out.close();
	return 1;
}
