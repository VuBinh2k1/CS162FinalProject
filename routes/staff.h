#ifndef STAFF_H
#define STAFF_H

#pragma warning(disable : 4996)
#include "control.h"
#include "csv.h"
#include "layout.h"
#include "course.h"
#include "roles.h"
using namespace std;
namespace classes
{
	void viewofclass();
	void changeclass();
	bool checkstudent(string id,char *c1);
}

namespace npstaff {
	void menu(csv_line& user);
}
#endif