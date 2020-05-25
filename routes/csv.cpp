#include "..\\controls\\csv.h"

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

bool csv_line::operator==(const char* position) {
	return strcmp(pdata[3], position) == 0;
}

csv_file::csv_file(const char* FILE, const char* __def) {
	if (!file::exists(FILE))	// load __default.csv of file
		if (file::exists(__def)) file::copy(__def, FILE);	
	if (!file::exists(FILE)) {
		MessageBox(NULL, TEXT("data file is not exist"), TEXT("error database"), MB_OK);
		exit(0);
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

csv_file::csv_file(const char* FILE, const char* course_id, const char* course_cs) {
	if (!file::exists(FILE)) {
		file::copy(def_schedule, FILE);
		file::mksche(course_id, course_cs);		// Make schedule.csv automatic
	}
	if (!file::exists(FILE)) {
		MessageBox(NULL, TEXT("data file is not exist"), TEXT("error database"), MB_OK);
		exit(0);
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

// [Function]::comparator //====================================================================================================================//

bool file::sort_cmp_default(const char* x, const char* y) {
	return strcmp(x, y) < 1;
}

// [EDIT]::file.csv //==========================================================================================================================//

void file::copy(const char* sre, const char* des) {
	std::ifstream inp(sre, std::ios::binary); std::ofstream out(des, std::ios::binary);
	out << inp.rdbuf();
	inp.close(); out.close();
}

// file::sort: overloads
void file::sort(const char* FILE, int col1, int col2, int col3) { file::sort(FILE, col1, col2, col3, file::sort_cmp_default); }
void file::sort(const char* FILE, int col1, comparator _cmp) { file::sort(FILE, col1, -1, -1, _cmp); }
void file::sort(const char* FILE, int col1, int col2, comparator _cmp) { file::sort(FILE, col1, col2, -1, _cmp); }
// file::sort: main function
void file::sort(const char* FILE, int col1, int col2, int col3, comparator _cmp) {
	csv_file file(FILE);
	// Init
	int n = file.count;
	int* row = new int[n];
	for (int i = 0; i < n; ++i)
		if (file.data[i].count < max(col1, max(col1, col2)) + 1) { delete[] row; return; }
		else row[i] = i;

	std::ofstream out(FILE);
	// Sort O(n^2)
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (col1 > -1 && !_cmp(file.data[row[i]].pdata[col1], file.data[row[j]].pdata[col1])) std::swap(row[i], row[j]);
			if (strcmp(file.data[row[i]].pdata[col1], file.data[row[j]].pdata[col1])) continue;

			if (col2 > -1 && !_cmp(file.data[row[i]].pdata[col2], file.data[row[j]].pdata[col2])) std::swap(row[i], row[j]);
			if (strcmp(file.data[row[i]].pdata[col2], file.data[row[j]].pdata[col2])) continue;

			if (col3 > -1 && !_cmp(file.data[row[i]].pdata[col3], file.data[row[j]].pdata[col3])) std::swap(row[i], row[j]);
		}
	}

	// Save file
	for (int j = 0; j < file.mark.count; ++j) {
		out << file.mark.pdata[j];
		if (j + 1 != file.mark.count) out << ',';
	}
	out << "\n";
	for (int i = 0; i < n; out << "\n", ++i) {
		for (int j = 0; j < file.data[row[i]].count; ++j) {
			out << file.data[row[i]].pdata[j];
			if (j + 1 != file.data[row[i]].count) out << ',';
		}
	}
	out.close();
	delete[] row;
}

void file::update(const char* FILE, int row, int column, const char* val) {
	csv_file file(FILE);
	std::ofstream out(FILE);

	for (int j = 0; j < file.mark.count; ++j) {
		out << file.mark.pdata[j];
		if (j + 1 != file.mark.count) out << ',';
	}
	out << "\n";
	for (int i = 0; i < file.count; out << "\n", ++i) {
		for (int j = 0; j < file.data[i].count; ++j) {
			if (i == row && j == column) {
				out << val;
			}
			else {
				out << file.data[i].pdata[j];
			}
			if (j + 1 != file.data[i].count) out << ',';
		}
	}
	out.close();
}

void file::remove(const char* FILE, int row) {
	if (!file::exists(FILE)) return;
	csv_file file(FILE);
	std::ofstream out(FILE);

	for (int j = 0; j < file.mark.count; ++j) {
		out << file.mark.pdata[j];
		if (j + 1 != file.mark.count) out << ',';
	}
	out << "\n";
	for (int i = 0; i < file.count; ++i) {
		if (i == row) continue;
		for (int j = 0; j < file.data[i].count; ++j) {
			out << file.data[i].pdata[j];
			if (j + 1 != file.data[i].count) out << ',';
			
		}
		out << '\n';
	}
	out.close();
}

bool file::exists(const char* FILE) {
	if (FILE == nullptr) return 0;
	std::ifstream inp(FILE);
	if (!inp.is_open()) return 0;
	inp.close(); return 1;
}

void file::mksche(const char* course_id, const char* course_cs) {
	csv_file course_list(COURSE_PATH("__course.csv").c_str(), def_course);
	csv_line* course = file::find(course_list, course_id, course_cs, OFF);
	if (course == nullptr) return;

	std::string schpath = COURSE_PATH("schedule\\") + course_id + '_' + course_cs + ".csv";
	csv_file schedule(schpath.c_str(), def_schedule);
	if (strcmp(course->pdata[5], "0") == 0) return;
	if (strcmp(course->pdata[6], "0") == 0) return;

	std::tm cur = control::gtime(course->pdata[5]);
	std::mktime(&cur);

	int wday = 0;
	if (strcmp(course->pdata[7], "Sun") == 0) wday = 0;
	if (strcmp(course->pdata[7], "Mon") == 0) wday = 1;
	if (strcmp(course->pdata[7], "Tue") == 0) wday = 2;
	if (strcmp(course->pdata[7], "Wed") == 0) wday = 3;
	if (strcmp(course->pdata[7], "Thu") == 0) wday = 4;
	if (strcmp(course->pdata[7], "Fri") == 0) wday = 5;
	if (strcmp(course->pdata[7], "Sat") == 0) wday = 6;
	while (cur.tm_wday != wday) {
		cur.tm_mday++;
		std::mktime(&cur);
	}

	for (int week = 0; week < MAX_WEEK; ++week) {
		if (control::now(cur, course->pdata[6]) == 1) break;

		std::string data = "";
		if (cur.tm_mday < 10) data += '0'; data += std::to_string(cur.tm_mday) + '/';
		if (cur.tm_mon < 9) data += '0'; data += std::to_string(cur.tm_mon + 1) + '/';
		data += std::to_string(cur.tm_year + 1900);

		file::update(schpath.c_str(), week, 1, data.c_str());
		file::update(schpath.c_str(), week, 2, course->pdata[8]);
		file::update(schpath.c_str(), week, 3, course->pdata[9]);
		cur.tm_mday += 7;
		std::mktime(&cur);
	}
}

// [Data]::find //==============================================================================================================================//

const char* file::find(csv_file& file, int row, const char* mark) {
	for (int col = 0; col < min(file.mark.count, file.data[row].count); ++col) {
		if (strcmp(file.mark.pdata[col], mark) == 0) {
			return file.data[row].pdata[col];
		}
	}
	return nullptr;
}

int file::find(std::string path, const char* data1, const char* data2, bool status) {
	csv_file file(path.c_str());

	for (int i = 0; i < file.count; ++i) {
		if (status && file.data[i].pdata[0][0] == '0') continue;
		if (strcmp(file.data[i].pdata[1], data1) == 0) {
			if (data2 == nullptr || strcmp(file.data[i].pdata[3], data2) == 0) return i;
		}
	}
	return -1;
}

csv_line* file::find(csv_file& file, const char* data1, const char* data2, bool status) {
	for (int i = 0; i < file.count; ++i) {
		if (status && file.data[i].pdata[0][0] == '0') continue;
		if (strcmp(file.data[i].pdata[1], data1) == 0) {
			if (data2 == nullptr || strcmp(file.data[i].pdata[3], data2) == 0) return &file.data[i];
		}
	}
	return nullptr;
}
