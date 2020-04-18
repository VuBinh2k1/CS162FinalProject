#include "layout.h"

void mess::init(std::istream& inp) {
	data = new char* [height];
	inp.ignore(1000, '\n');
	for (int i = 0; i < height; ++i) {
		data[i] = new char[width + 1];
		inp.getline(data[i], width + 1);
	}
}

void mess::realease() {
	for (int i = 0; i < height; ++i) {
		delete[] data[i];
	}
	delete[] data;
}

void mess::print() {
	gotoxy(xo, yo, color_code);
	for (int i = 0; i < height; ++i) {
		std::cout << data[i] << "\n";
	}
}

layout::layout(std::istream& inp) {
	inp >> image_count >> input_count;
	data = new mess[image_count];
	for (int i = 0; i < image_count; ++i) {
		inp >> data[i].xo >> data[i].yo;
		inp >> data[i].height >> data[i].width;
		inp >> data[i].color_code;
		data[i].init(inp);
	}

	if (input_count == 0) return;
	in = new std::pair<int, int>[input_count];
	for (int i = 0; i < input_count; ++i) {
		inp >> in[i].first >> in[i].second;
	}
}

layout::~layout() {
	for (int i = 0; i < image_count; ++i) data[i].realease();
	delete[] data;
	if (input_count) delete[] in;
}

void layout::print() {
	for (int i = 0; i < image_count; ++i) {
		data[i].print();
	}
}

void colorizing(WORD color_code) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, color_code);
}

void gotoxy(int column, int line, WORD color_code) {
	COORD coord = { column, line };
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, coord);
	colorizing(color_code);
}

void print_login_layout(char*& user, char*& pass, bool has_invalid) {
	std::ifstream inp(".\\layout\\login.layout");
	if (!inp.is_open()) {
		exit(0);
	}
	layout login_layout(inp);
	layout user_pass_layout(inp);
	layout error_layout(inp);

	if (has_invalid == 0) login_layout.print();
	else {
		error_layout.print();
		Sleep(2000);
	}

	char temp[20];
	user_pass_layout.print();
	gotoxy(user_pass_layout.in[0].first, user_pass_layout.in[0].second);
	std::cin >> temp; user = strdup(temp);
	gotoxy(user_pass_layout.in[1].first, user_pass_layout.in[1].second);
	std::cin >> temp; pass = strdup(temp);

	colorizing(default_code);
}
