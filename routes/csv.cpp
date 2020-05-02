#include "csv.h"

void csv_line::init(std::istream& inp) {
	char temp[1000];
	inp.getline(temp, 1000);

	count = std::count(temp, temp + strlen(temp), ',') + 1;
	data = new char[strlen(temp) + 1];
	pdata = new char* [count];

	strcpy_s(data, strlen(temp) + 1, temp);
	pdata[0] = data;
	for (int i = 0, cnt = 1; data[i] != '\0'; ++i) {
		if (data[i] == ',') {
			data[i] = '\0';
			pdata[cnt++] = data + i + 1;
		}
	}
}

csv_line& csv_line::operator=(const csv_line& x) {
	int data_size = 0;
	for (int i = 0; i < x.count; ++i) {
		data_size += strlen(x.pdata[i]) + 1;
	}

	delete[] data;
	delete[] pdata;

	id = x.id;
	count = x.count;
	data = new char[data_size];
	pdata = new char* [count];

	pdata[0] = data;
	for (int i = 0, cnt = 1; i < data_size; ++i) {
		data[i] = x.data[i];
		if (cnt < count && data[i] == '\0') {
			pdata[cnt++] = data + i + 1;
		}
	}
	return *this;
}

csv_file::csv_file(const char* FILE, const char* __def) {
	if (!file::exists(FILE)) {	// load __default.csv of file
		if (file::exists(__def)) file::copy(__def, FILE);
		else {
			MessageBox(NULL, TEXT("data file is not exist"), TEXT("error database"), MB_OK);
			exit(0);
		}
	}
	std::ifstream inp(FILE);
	count = std::count(std::istreambuf_iterator<char>(inp), std::istreambuf_iterator<char>(), '\n') - 1;
	inp.seekg(0, inp.beg);

	mark.init(inp);
	data = new csv_line[count];
	for (int i = 0; i < count; ++i) {
		data[i].init(inp);
		data[i].id = i;
	}
	inp.close();
}

void file::copy(const char* sre, const char* des) {
	std::ifstream inp(sre, std::ios::binary); std::ofstream out(des, std::ios::binary);
	out << inp.rdbuf();
	inp.close(); out.close();
}

void file::update(const char* FILE, int row, int column, const char* val) {
	csv_file user_list(FILE);
	std::ofstream out(FILE);

	for (int j = 0; j < user_list.mark.count; ++j) {
		out << user_list.mark.pdata[j];
		if (j + 1 != user_list.mark.count) out << ',';
	}
	out << "\n";
	for (int i = 0; i < user_list.count; out << "\n", ++i) {
		for (int j = 0; j < user_list.data[i].count; ++j) {
			if (i == row && j == column) {
				out << val;
			}
			else {
				out << user_list.data[i].pdata[j];
			}
			if (j + 1 != user_list.data[i].count) out << ',';
		}
	}
	out.close();
}

bool file::exists(const char* FILE) {
	if (FILE == nullptr) return 0;
	std::ifstream inp(FILE);
	if (!inp.is_open()) return 0;
	inp.close(); return 1;
}

csv_line* file::exists(csv_file& file, const char* username, const bool status) {
	for (int i = 0; i < file.count; ++i) {
		if (strcmp(file.data[i].pdata[1], username) == 0) {
			if (status && file.data[i].pdata[0][0] == '0') return nullptr;
			return &file.data[i];
		}
	}
	return nullptr;
}

