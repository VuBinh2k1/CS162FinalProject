#ifndef LAYOUT_H
#define LAYOUT_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <conio.h>
#define MAX_WIDTH 100
#define MAX_HEIGHT 30
#define SHOW 0
#define HIDE 1

#define COLOR_DEFAULT 7
#define COLOR_BLUE 1
#define COLOR_RED 4
#define COLOR_YELLOW 6
#define COLOR_GREEN 10
#define COLOR_WHITE 15
#define COLOR_BLUE_BACKGROUND 16
#define COLOR_RED_BACKGROUND 64
#define COLOR_YELLOW_BACKGROUND 96
#define COLOR_GREEN_BACKGROUND 160
#define COLOR_WHITE_BACKGROUND 240

// KEYBOARD_CODE (some keys have 2 characters)
#define KEY_BACKSPACE 8		// Backspace.
#define KEY_ENTER 13		// Enter.
#define KEY_ESC 27			// Esc.
#define KEY_UP 72			// 0 - 224	+ Up.
#define KEY_LEFT 75			// 0 - 224	+ Left.
#define KEY_RIGHT 77		// 0 - 224	+ Right.
#define KEY_DOWN 80			// 0 - 224	+ Down.
#define KEY_DELETE 83		// 0		+ Delete.
#define KEY_SEARCH 6		// Ctrl		+ F.
#define KEY_NEW 14			// Ctrl		+ N.
#define KEY_OPEN 15			// Ctrl		+ O.
#define KEY_SAVE 19			// Ctrl		+ S.
#define KEY_FUNCTION 28		// Ctrl		+ \,
#define KEY_CKIN(c) (c == 'C' || c == 'c')
#define KEY_EDIT(c) (c == 'E' || c == 'e')
#define KEY_HELP(c) (c == 'H' || c == 'h')
#define KEY_EROL(c) (c == 'R' || c == 'r')
#define PAUSE Sleep(2000)

typedef struct mess {
	int xo, yo, height, width, color_code;
	char** data;
	// MESS format: *.layout
	// 1. x0 y0 height width COLOR_CODE
	// 2. data[0](width)
	// ...
	// k. data[height-1](width)

	void init(std::istream& inp);
	void realease();
	void print();
}mess;

typedef struct layout {
	int image_count;	// Number of mess each image
	mess* data;
	// layout format: *.layout
	// 1. image_count
	// 2. mess[0]
	// ...
	// k. mess[image_count-1]

	layout(std::istream& inp);
	~layout();
	void print();
}layout;

// Change colored text at the console
void colorizing(WORD color_code);

// Move console pointer
void gotoxy(int column, int line, WORD color_code = COLOR_DEFAULT);

// Read std::string with conio.h
char read(int x, int y, WORD COLOR_CODE, std::string& str, int max_size, bool status, const char* comment = "");
char type(int x, int y, WORD COLOR_CODE, std::string& str, int max_size, const char* type = ".csv");
char date(int x, int y, WORD COLOR_CODE, std::string& str);
char time(int x, int y, WORD COLOR_CODE, std::string& str);
void loading();

#endif