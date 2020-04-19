#pragma one
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <cstring>

typedef struct user_mark {
	int count;
	char* data;
	char** pdata;
	void init(std::istream& inp);
	~user_mark() { free(data); delete[] pdata; }
}user_mark;

class user_data {
public:
	int count;
	char* data;
	char** pdata;
	void init(std::istream& inp, int mark_count);
	void init(const user_data& x);

	~user_data() { free(data); delete[] pdata; }
};

class puser {
public:
	int count;
	user_mark mark;
	user_data* data;
	puser(const char* FILE);
	~puser() { delete[] data; }
};
