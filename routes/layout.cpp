#include "layout.h"

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
	inp >> image_count;
	data = new mess[image_count];
	for (int i = 0; i < image_count; ++i) {
		inp >> data[i].xo >> data[i].yo;
		inp >> data[i].height >> data[i].width;
		inp >> data[i].color_code;
		data[i].init(inp);
	}
}

layout::~layout() {
	for (int i = 0; i < image_count; ++i) data[i].realease();
	delete[] data;
}

void layout::print() {
	for (int i = 0; i < image_count; ++i) {
		data[i].print();
	}
}
