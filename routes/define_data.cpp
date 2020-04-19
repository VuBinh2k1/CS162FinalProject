#include "define_data.h"

void user_mark::init(std::istream& inp) {
	char temp[1000];
	inp.getline(temp, 1000);
	data = strdup(temp);

	for (int i = 0; i < strlen(temp); ++i)
		if (temp[i] == ',') count++;
	count++;
	pdata = new char* [count];

	int cnt = 0, cur = 0;
	pdata[cnt++] = data;
	while (cnt < count && data[cur] != '\0') {
		if (data[cur] == ',') {
			data[cur] = '\0';
			pdata[cnt++] = data + cur + 1;
		}
		cur++;
	}
}

void user_data::init(std::istream& inp, int mark_count) {
	char temp[1000];
	inp.getline(temp, 1000);

	count = mark_count;
	data = strdup(temp);
	pdata = new char* [count];

	int cnt = 0, cur = 0;
	pdata[cnt++] = data;
	while (cnt < count && data[cur] != '\0') {
		if (data[cur] == ',') {
			data[cur] = '\0';
			pdata[cnt++] = data + cur + 1;
		}
		cur++;
	}
}

void user_data::init(const user_data& x) {
	int data_size = 0;
	for (int i = 0; i < x.count; ++i) {
		data_size += strlen(x.pdata[i]) + 1;
	}

	count = x.count;
	data = (char*)malloc(data_size);
	pdata = new char* [count];

	int cnt = 0, cur = 0;
	pdata[cnt++] = data;
	while (cnt < count) {
		data[cur] = x.data[cur];
		if (data[cur] == '\0') {
			pdata[cnt++] = data + cur + 1;
		}
		cur++;
	}
}

puser::puser(const char* FILE) {
	std::ifstream inp(FILE);
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("data file is not exist"), TEXT("error database"), MB_OK);
		exit(0);
	}
	count = std::count(std::istreambuf_iterator<char>(inp), std::istreambuf_iterator<char>(), '\n') - 1;
	inp.seekg(0, inp.beg);

	mark.init(inp);
	data = new user_data[count];
	for (int i = 0; i < count; ++i) {
		data[i].init(inp, mark.count);
	}
	inp.close();
}
