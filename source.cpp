#include <iostream>
#include "layout/layout.h"
#include "routes/login.h"

using std::cin;
using std::cout;

int main(int argc, char* agrv[]) {
    char* a, * b;
    print_login_layout(a, b);
    while (strcmp(a, "vubinh"))print_login_layout(a, b, 1);
    return 0;
}