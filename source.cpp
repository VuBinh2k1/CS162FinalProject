#include <iostream>
#include "routes/roles.h"
#include "routes/lecturer.h"
#include "routes/staff.h"
#include "routes/student.h"
#include "routes/staff.h"
using std::cin;
using std::cout;

int main(int argc, char* agrv[]) {
    SetConsoleTitle(TEXT("Student management system"));
    //*/
    csv_line user;
    while (role::login(user)) {
        if (strcmp(user.pdata[3], "staff") == 0) npstaff::menu(user);
        else if (strcmp(user.pdata[3], "lecturer") == 0) nplecturer::menu(user);
        else npstudent::menu(user);

        colorizing(COLOR_DEFAULT);
    }
    //*/
    gotoxy(0, 20);
    return 0;
}