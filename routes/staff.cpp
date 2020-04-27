#include "staff.h"

void classes::viewofclass()
{
	system("cls");
	csv_file myfile(".\\data\\class\\__class.csv");
	for (int i = 0; i < myfile.count; i++)
	{
		std::cout << myfile.data[i].pdata[1]<<std::endl;
	}
	
}
bool classes::checkstudent(string id, char*c1)
{
	csv_file file1(c1);
	for (int i = 0; i < file1.count; i++)
	{
		if (file1.data[i].pdata[1] == id)
			return true;
	}
	return false;
}
void classes::changeclass()
{
	string id, class1,class2;
	cout << "Input the id student you want to change: "; 
	cin.ignore('\0');
	getline(cin, id);
	cout << "Change from class: ";
	cin.ignore('\0');
	getline(cin,class1);
	cout << "Change to class: ";
	cin.ignore('\0');
	getline(cin, class2);
	strupr((char*)&class1);
	strupr((char*)&class2);
	string filename = { ".\\data\\class\\" };
	string temp1, temp2;
	temp1 = filename + class1 + ".csv";
	char* c1, *c2;
	c1 = new char[temp1.size() + 1];
	copy(temp1.begin(), temp1.end(), c1);
	c1[temp1.size()] = '\0';

	temp2= filename + class2 + ".csv";
	c2 = new char[temp2.size() + 1];
	copy(temp2.begin(), temp2.end(), c2);
	c2[temp2.size()] = '\0';
	csv_file file1(c1);
	bool ok=classes::checkstudent(id, c1);
	
	if (ok == true)
	{
		for (int i = 0; i < file1.count; i++)
		{

			if (file1.data[i].pdata[1] == id && atoi(file1.data[i].pdata[0]) == 0)
			{
				cout << "This student had been removed from class" << endl;
			}
			else if (file1.data[i].pdata[1] == id && atoi(file1.data[i].pdata[0]) == 1)
			{
				file1.data[i].pdata[0][0] = 0 + 48;
				cout << file1.data[i].pdata[0] << endl;
				npcsv::update(c1, i, 0, file1.data[i].pdata[0]);
			}
		}
	}
	
	else
	{
		cout << "This student is not in class "<<class1;
	}

	delete[]c1;
	delete[]c2;

void npstaff::menu(csv_line& user) {
	std::ifstream inp(".\\layout\\menu.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("menu.layout is not exist"), TEXT("error layout"), MB_OK);
		exit(0);
	}
	layout menu_layout(inp);
	layout menu_info_layout(inp);
	inp.close();

	csv_file infofile(".\\data\\staff\\__staff.csv");
	csv_line* infouser = nullptr;
	for (int i = 0; i < infofile.count; ++i) {
		if (strcmp(user.pdata[1], infofile.data[i].pdata[1]) == 0) {
			infouser = &infofile.data[i];
			break;
		}
	}

	int choose = 0;

MENU:
	colorizing(COLOR_DEFAULT); system("cls");
	menu_layout.print();
	if (infouser == nullptr) {
		gotoxy(33, 9, COLOR_RED); std::cout << "Sorry, I don't have your information.";
	}
	else {
		gotoxy(33, 9, COLOR_YELLOW); std::cout << "Welcome ";
		colorizing(COLOR_RED);
		if (strcmp(infouser->pdata[4], "male") == 0) std::cout << "Mr.";
		else std::cout << "Ms.";
		std::cout << infouser->pdata[3];
		colorizing(COLOR_YELLOW); std::cout << " to CS162FinalProject";
	}

	gotoxy(33, 11); std::cout << "This program is for managing student info, course info,";
	gotoxy(33, 12); std::cout << "study process, etc.";
	gotoxy(33, 15, COLOR_YELLOW); std::cout << "Development team:";
	gotoxy(33, 17); std::cout << "[19127186]: Le Thanh Khoi (Leader)";
	gotoxy(33, 18); std::cout << "[19127012]: Vu Nguyen Thai Binh";
	gotoxy(33, 19); std::cout << "[19127226]: Hoang Van Nguyen";
	gotoxy(33, 20); std::cout << "[19127348]: Bui Cong Danh";

	gotoxy(2, 8, COLOR_YELLOW_BACKGROUND); std::cout << "      Staff       ";
	while (1) {
		gotoxy(2, 9, (choose == 0) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Profile         ";
		gotoxy(2,10, (choose == 1) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit student    ";
		gotoxy(2,11, (choose == 2) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Edit courses    ";
		gotoxy(2,12, (choose == 3) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "  Scoreboard      ";
		gotoxy(2,28, (choose == 4) ? COLOR_WHITE_BACKGROUND : COLOR_WHITE); std::cout << "     Log out      ";

	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC) choose = 4;
		if (c == KEY_ENTER) {
			if (choose == 0) {
				role::profile(user);
				goto MENU;
			}
			if (choose == 1) {
				
				goto MENU;
			}
			if (choose == 2) {
				
				goto MENU;
			}
			if (choose == 3) {
				
				goto MENU;
			}
			if (choose == 4) return;
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0) choose--;
			else if (c == KEY_DOWN && choose < 4) choose++;
			else goto NO_CHANGE;
		}
	}

}
