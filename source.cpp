#include <iostream>
#include "routes/login.h"

using std::cin;
using std::cout;

int main(int argc, char* agrv[]) {
    user_data user;
    login(user);

    //puser Student(".\\data\\student_data.csv");
    //puser User(".\\data\\account.csv");
    
    return 0;
}