#include "csv.h"

void csv_line::init(std::istream& inp, int mark_count) {
	char temp[1000];
	inp.getline(temp, 1000);

	count = mark_count;
	data = strdup(temp);
	pdata = new char* [count];

	pdata[0] = data;
	for (int i = 0, cnt = 1; data[i] != '\0'; ++i) {
		if (data[i] == ',') {
			data[i] = '\0';
			if (cnt < count) pdata[cnt++] = data + i + 1;
		}
	}
}

void csv_line::init(const csv_line& x) {
	int data_size = 0;
	for (int i = 0; i < x.count; ++i) {
		data_size += strlen(x.pdata[i]) + 1;
	}

	count = x.count;
	data = (char*)malloc(data_size);
	pdata = new char* [count];

	pdata[0] = data;
	for (int i = 0, cnt = 1; i < data_size; ++i) {
		data[i] = x.data[i];
		if (cnt < count && data[i] == '\0') {
			pdata[cnt++] = data + i + 1;
		}
	}
}

csv_file::csv_file(const char* FILE, int mark_count) {
	std::ifstream inp(FILE);
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("data file is not exist"), TEXT("error database"), MB_OK);
		exit(0);
	}
	count = std::count(std::istreambuf_iterator<char>(inp), std::istreambuf_iterator<char>(), '\n') - 1;
	inp.seekg(0, inp.beg);

	mark.init(inp, mark_count);
	data = new csv_line[count];
	for (int i = 0; i < count; ++i) {
		data[i].init(inp, mark_count);
	}
	inp.close();
}
