#include <iostream>
#include "controls/roles.h"

int main(int argc, char* agrv[]) {
    SetConsoleTitle(TEXT("Student management system"));
    csv_line user;
    while (role::login(user)) {
	//loading(1, 18, 98);
        if (user == "staff") role::staff(user);
        if (user == "lecturer") role::lecturer(user);
        if (user == "student") role::student(user);
    }
    gotoxy(0, 20); return 0;
}
