#pragma one
#include <Windows.h>
#define default_code 7
#define blue_code 1
#define green_code 10
#define red_code 4
#define white_code 15

void colorizing(WORD color_code);
void gotoxy(int column, int line, WORD color_code = default_code);
