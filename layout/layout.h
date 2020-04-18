#pragma one
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <utility>
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

	void init(std::istream& inp);
	void realease();
	void print();
}mess;

typedef struct layout {
	int image_count, input_count;
	mess* data;
	std::pair <int, int>* in;

	layout(std::istream& inp);
	~layout();
	void print();
}layout;

void colorizing(WORD color_code);
void gotoxy(int column, int line, WORD color_code = default_code);

void print_login_layout(char*& user, char*& pass, bool has_invalid = 0);
