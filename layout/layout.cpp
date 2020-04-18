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
