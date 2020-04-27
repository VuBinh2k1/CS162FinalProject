#include <iostream>
#include "routes/roles.h"
#include "routes/student.h"
#include "routes/staff.h"
using std::cin;
using std::cout;

int main(int argc, char* agrv[]) {
    SetConsoleTitle(TEXT("Student management system"));

    csv_line user;
    //role::login(user);
    //role::password(user);
    //npstudent::check_in(user);
    //npstudent::check_in_result(user);
	//classes::viewofclass();
	classes::changeclass();
	return 0;
}