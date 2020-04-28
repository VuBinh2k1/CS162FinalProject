#ifndef CLASS_H
#define CLASS_H
#include "control.h"

namespace npclass {
	void info();
	bool change(csv_line& user, const char* class1, const char* class2);
}
#endif