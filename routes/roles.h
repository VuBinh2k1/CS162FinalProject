#ifndef ROLES_H
#define ROLES_H
#include <conio.h>
#include "csv.h"
#include "layout.h"

void login(csv_line& user);
void menu(csv_line& user);
void profile(csv_line& user);
void password(csv_line& user);
#endif