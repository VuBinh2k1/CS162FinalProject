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
	WORD COLOR_COMM = (COLOR_CODE == 128 ? 128 : (COLOR_CODE == COLOR_DEFAULT ? 8 : (COLOR_CODE % 16 == 0 ? COLOR_CODE + 8 : COLOR_CODE)));
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
				colorizing(COLOR_COMM); std::cout << comment;
				gotoxy(x, y, COLOR_CODE);
			}
			continue;
		}
	}
	return c;
}

char type(int x, int y, WORD COLOR_CODE, std::string& str, int max_size, const char* type) {
	uint8_t c;
	str.resize(0); max_size -= strlen(type);
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
	str += type;
	return c;
}

char date(int x, int y, WORD COLOR_CODE, std::string& str) {
	uint8_t c; str = '0';

	gotoxy(x, y, COLOR_CODE); std::cout << "  /  /    "; gotoxy(x, y, COLOR_CODE);
	std::string day, mon, year;
	std::string* cur = &day;
	while ((c = getch()) != KEY_ENTER && c != KEY_ESC) {
		// Keyboard: Printable characters
		if (c >= '0' && c <= '9') {
			if (cur == &day) {
				if (day.size() == 2) cur = &mon, std::cout << '/';
				else day.push_back(c), std::cout << (char)c;
			}
			if (cur == &mon) {
				if (mon.size() == 2) cur = &year, std::cout << '/';
				else mon.push_back(c), std::cout << (char)c;
			}
			if (cur == &year) {
				if (year.size() == 4) continue;
				else year.push_back(c), std::cout << (char)c;
			}
		}
		// Keyboard: BACKSPACE: 8
		if (c == KEY_BACKSPACE) {
			if (cur->size()) {
				std::cout << "\b \b"; cur->pop_back();
			}
			if (cur->size() == 0) {
				if (cur == &day) { continue; }
				if (cur == &mon) { std::cout << "\b"; cur = &day; }
				if (cur == &year) { std::cout << "\b"; cur = &mon; }
			}
			continue;
		}
		// Keyboard: DELETE: 224 83
		if (c == 0 || c == 224) {
			if ((c = getch()) == KEY_DELETE) {
				day.resize(0); mon.resize(0); year.resize(0); cur = &day;
				gotoxy(x, y, COLOR_CODE); std::cout << "  /  /    "; gotoxy(x, y, COLOR_CODE);
			}
			continue;
		}
	}
	if (c == KEY_ESC) return c;

	// Config date:
	if (day.empty() && mon.empty() && year.empty()) str = "1";
	if (day.size() == 2 && mon.size() == 2 && year.size() == 4) {
		if (day[0] > '3' || mon[0] > '1' || std::stoi(year) < 1900 || std::stoi(year) > 3000) return c;

		std::time_t now = time(0);
		std::tm date = *localtime(&now);
		date.tm_mday = std::stoi(day);
		date.tm_mon = std::stoi(mon) - 1;
		date.tm_year = std::stoi(year) - 1900;
		std::mktime(&date);
		
		if (date.tm_mday != std::stoi(day) || date.tm_mon != std::stoi(mon) - 1 || date.tm_year != std::stoi(year) - 1900) return c;
		str = day + '/' + mon + '/' + year;
	}
	return c;
}

char time(int x, int y, WORD COLOR_CODE, std::string& str) {
	uint8_t c; str = '0';

	gotoxy(x, y, COLOR_CODE); std::cout << "  :  "; gotoxy(x, y, COLOR_CODE);

	std::string hour, min;
	std::string* cur = &hour;
	while ((c = getch()) != KEY_ENTER && c != KEY_ESC) {
		// Keyboard: Printable characters
		if (c >= '0' && c <= '9') {
			if (cur == &hour) {
				if (hour.size() == 2) cur = &min, std::cout << ':';
				else hour.push_back(c), std::cout << (char)c;
			}
			if (cur == &min) {
				if (min.size() == 2) continue;
				else min.push_back(c), std::cout << (char)c;
			}
		}
		// Keyboard: BACKSPACE: 8
		if (c == KEY_BACKSPACE) {
			if (cur->size()) {
				std::cout << "\b \b"; cur->pop_back();
			}
			if (cur->size() == 0) {
				if (cur == &hour) { continue; }
				if (cur == &min) { std::cout << "\b"; cur = &hour; }
			}
			continue;
		}
		// Keyboard: DELETE: 224 83
		if (c == 0 || c == 224) {
			if ((c = getch()) == KEY_DELETE) {
				hour.resize(0); min.resize(0); cur = &hour;
				gotoxy(x, y, COLOR_CODE); std::cout << "  :  "; gotoxy(x, y, COLOR_CODE);
			}
			continue;
		}
	}
	if (c == KEY_ESC) return c;

	// Config time:
	if (hour.empty() && min.empty()) str = "1";
	if (hour.size() == 2 && min.size() == 2) {
		if (hour == "24") hour = "00";
		if (min == "60") min = "00";
		if (hour[0] > '2' || (hour[0] == '2' && hour[1] > '3') || min[0] > '5') return c;
		str = hour + ':' + min;
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
void loading()
{
	gotoxy(1, 18,COLOR_GREEN);
	for (int i = 0; i < 20; i++)
	{
		putchar(219);
		Sleep(20);
	}
	putchar(219);
	Sleep(800);
	for (int i = 20; i < 60; i++)
	{
		putchar(219);
		Sleep(20);
	}
	putchar(219);
	Sleep(1000);
	for (int i = 60; i < 96; i++)
	{
		putchar(219);
		Sleep(20);
	}
}