#include "..\\controls\\layout.h"

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

char read(int x, int y, WORD COLOR_CODE, std::string& str, int max_size, bool status, const char* comment) {
	uint8_t c; 
	str.resize(0);
	WORD COLOR_COMM = (COLOR_CODE == COLOR_DEFAULT) ? 8 : COLOR_CODE + 8;
	gotoxy(x, y, COLOR_COMM); std::cout << comment; gotoxy(x, y, COLOR_CODE);
	while ((c = getch()) != KEY_ENTER && c != KEY_ESC) {
		// Keyboard: Printable characters
		if (c > 31 && c < 127) {
			if (str.size() < max_size) {
				if (str.size() == 0) {
					for (int i = 0; i < max_size; ++i) std::cout << ' ';
					gotoxy(x, y, COLOR_CODE);
				}
				if (status == SHOW) std::cout << (char)c;
				else std::cout << "*";
				str.push_back(c);
			}
			continue;
		}
		// Keyboard: BACKSPACE: 8
		if (c == KEY_BACKSPACE && str.size()) {
			std::cout << "\b \b"; str.pop_back();
			if (str.size() == 0) {
				colorizing(COLOR_COMM); std::cout << comment;
				gotoxy(x, y, COLOR_CODE);
			}
			continue;
		}
		// Keyboard: DELETE: 224 83
		if (c == 0 || c == 224) {
			if ((c = getch()) == KEY_DELETE) {
				while (str.size()) {
					std::cout << "\b \b"; str.pop_back();
				}
				colorizing(8); std::cout << comment;
				gotoxy(x, y, COLOR_CODE);
			}
			continue;
		}
	}
	return c;
}

std::string get(int x, int y, WORD COLOR_CODE, int max_size, const char* type) {
	uint8_t c;
	std::string str; max_size -= strlen(type);
	gotoxy(x, y, COLOR_CODE); std::cout << type; for (int i = 0; i < strlen(type); ++i) std::cout << '\b';
	while ((c = getch()) != KEY_ENTER && c != KEY_ESC) {
		// Keyboard: Printable characters
		if (c > 31 && c < 127) {
			if (str.size() < max_size) {
				std::cout << (char)c;
				std::cout << type; for (int i = 0; i < strlen(type); ++i) std::cout << '\b';
				str.push_back(c);
			}
			continue;
		}
		// Keyboard: BACKSPACE: 8
		if (c == KEY_BACKSPACE && str.size()) {
			std::cout << "\b \b"; str.pop_back();
			std::cout << type << ' '; for (int i = 0; i <= strlen(type); ++i) std::cout << '\b';
			continue;
		}
		// Keyboard: DELETE: 224 83
		if (c == 0 || c == 224) {
			if ((c = getch()) == KEY_DELETE) {
				while (str.size()) {
					std::cout << "\b \b"; str.pop_back();
					std::cout << type << ' '; for (int i = 0; i <= strlen(type); ++i) std::cout << '\b';
				}
			}
			continue;
		}
	}
	if (c == KEY_ESC) return (std::string)"";
	return str + type;
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
