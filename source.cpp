#include <iostream>
#include "routes/roles.h"

using std::cin;
using std::cout;

int main(int argc, char* agrv[]) {
    SetConsoleTitle(TEXT("Student management system"));

    csv_line user;
    login(user);
    password(user);
    return 0;
}