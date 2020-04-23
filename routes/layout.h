#ifndef LAYOUT_H
#define LAYOUT_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#define MAX_WIDTH = 100
#define MAX_HEIGHT = 30

#define COLOR_DEFAULT 7
#define COLOR_BLUE 1
#define COLOR_GREEN 10
#define COLOR_RED 4
#define COLOR_WHITE 15

#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

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
#endif