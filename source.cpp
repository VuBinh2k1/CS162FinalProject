#include <iostream>
#include "controls/roles.h"

int main(int argc, char* agrv[]) {
    SetConsoleTitle(TEXT("Student management system"));
    csv_line user;
    while (role::login(user)) {
        if (strcmp(user.pdata[3], "staff")    == 0) role::staff(user);
        if (strcmp(user.pdata[3], "lecturer") == 0) role::lecturer(user);
        if (strcmp(user.pdata[3], "student")  == 0) role::student(user);
    }
    gotoxy(0, 20); return 0;
}