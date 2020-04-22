#ifndef LAYOUT_H
#define LAYOUT_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#define default_code 7
#define blue_code 1
#define green_code 10
#define red_code 4
#define white_code 15
#define MAX_WIDTH = 100
#define MAX_HEIGHT = 30

typedef struct mess {
	int xo, yo, height, width, color_code;
	char** data;
	// MESS format: *.layout
	// 1. x0 y0 height width color_code
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
void gotoxy(int column, int line, WORD color_code = default_code);
#endif