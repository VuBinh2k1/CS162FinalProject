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

char read(int x, int y, std::string& pw, bool status) {
	gotoxy(x, y);
	uint8_t c = -1;
	pw.resize(0);
	while (c != KEY_ENTER && c != KEY_ESC) {
		c = getch();
		// Keyboard: Printable characters
		if (c > 32 && c < 127) {
			if (pw.size() < 52) {
				if (status == SHOW) std::cout << (char)c;
				else std::cout << "*";
				pw.push_back(c);
			}
			continue;
		}
		// Keyboard: BACKSPACE: 8
		if (c == KEY_BACKSPACE && pw.size()) {
			std::cout << "\b \b"; pw.pop_back();
			continue;
		}
		// Keyboard: DELETE: 224 83
		if (c == 0 || c == 224) {
			if (getch() == 83) {
				while (pw.size()) {
					std::cout << "\b \b"; pw.pop_back();
				}
			}
			continue;
		}
	}
	return c;
}

// mess::funtion
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
	for (int i = 0; i < height; ++i) {
		gotoxy(xo, yo + i, color_code); 
		std::cout << data[i];
	}
}

// layout::funtion
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
