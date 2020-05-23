#include "..\\controls\\lecturer.h"
int nplecturer::list()
{
	std::ifstream inp(".\\layout\\minibox.layout");
	if (!inp.is_open()) {
		MessageBox(NULL, TEXT("minibox.layout is not exist"), TEXT("error layout"), MB_OK);
		return 0;
	}
	layout minibox_layout(inp);
	inp.close();
	int choose = 0, cur = -1, overflow = 0,*row=nullptr;
LAYOUT:
	minibox_layout.print();
	gotoxy(78, 7); std::cout << "[Help]";
	// Title
	gotoxy(27, 8, COLOR_YELLOW_BACKGROUND); std::cout << "  Lecturer list     ";
	// Detail
	gotoxy(27, 9, COLOR_BLUE_BACKGROUND);   std::cout << " No.   | Last name       | First name   | Ranking                 ";
	while ((cur = -1))
	{
		csv_line *lecturer = nullptr;
		csv_file path(__LECTURER);
		if (row)delete[]row;
		row = new int[path.count];
		for (int i = 0; i < path.count; i++)
		{
			if ((lecturer = file::find(path, path.data[i].pdata[1], nullptr, ON)) == nullptr) continue;
			int y = 10 + (++cur) + overflow;
			row[cur] = i;
			if (y < 10 || y>27) continue;
			WORD COLOR_CODE = (cur % 2) ? 112 : 240;
			if (choose == cur)COLOR_CODE = 176;
			gotoxy(27, y, COLOR_CODE); std::cout << "       |                 |              |                         ";
			gotoxy(28, y, COLOR_CODE); std::cout << cur;
			gotoxy(36, y, COLOR_CODE); std::cout << path.data[i].pdata[2];
			gotoxy(56, y, COLOR_CODE); std::cout << path.data[i].pdata[3];
			gotoxy(70, y, COLOR_CODE); std::cout << path.data[i].pdata[4];
		}
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC)break;
		lecturer = &path.data[row[choose]];
		if (KEY_HELP(c))
		{		
			gotoxy(78, 8, 128); std::cout << " Create          C  ";
			getch();
			gotoxy(78, 8); std::cout << "                    ";

			goto LAYOUT;
		}
		else if (c==KEY_ENTER)
		{
			nplecturer::info(lecturer->pdata[1], ON);
			continue;
		}
		else if (c == 224 || c == 0)
		{
			c = getch();
			if (c == KEY_UP && choose > 0)
			{
				if ((--choose + overflow) < 0)
					overflow++;
			}
			else if (c == KEY_DOWN && choose < cur)
			{
				if (++choose < cur - 16)
					overflow--;
			}
			else goto NO_CHANGE;
			continue;
		}
		goto NO_CHANGE;
	}

}
void nplecturer::info(const char* lecturer_id, bool EDIT) {
LAYOUT:
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, OFF);
	if (lecturer == nullptr) {
		gotoxy(32, 15, COLOR_BLUE_BACKGROUND); std::cout << " Lecturer info                                            ";
		gotoxy(32, 16, 128); std::cout << "                                                         ";
		gotoxy(33, 16, 132); std::cout << "This lecturer does not exist";
		goto END;
	}

	gotoxy(32, 13, COLOR_BLUE_BACKGROUND); std::cout << " Lecturer info                                           ";
	gotoxy(32, 14, 128); std::cout << "                                                         ";
	gotoxy(32, 15, 128); std::cout << "                                                         ";
	gotoxy(32, 16, 128); std::cout << "                                                         ";
	gotoxy(32, 17, 128); std::cout << "                                                         ";
	gotoxy(32, 18, 128); std::cout << "                                                         ";
	gotoxy(32, 19, 128); std::cout << "                                                         ";
	gotoxy(32, 20, 128); std::cout << "                                                         ";
	gotoxy(33, 14, 143); std::cout << "Lecturer name: ";
	if (ENGLISHNAME) std::cout << lecturer->pdata[3] << ' ' << lecturer->pdata[2];
	else std::cout << lecturer->pdata[2] << ' ' << lecturer->pdata[3];
	gotoxy(33, 15, 128); std::cout << "Lecturer id  : " << lecturer->pdata[1];
	gotoxy(33, 16, 128); std::cout << "Ranking      : " << lecturer->pdata[4];
	gotoxy(33, 17, 128); std::cout << "COURSES ID      |       Class ";
	nplecturer::courseslist(lecturer->pdata[1]);
	if (EDIT)
	{
		for (int choose = 0;;) {
			gotoxy(51, 20, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "  Edit  ";
			gotoxy(60, 20, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Remove ";
			uint8_t c = getch();
			if (c == KEY_ESC)break;
			if (c == KEY_ENTER)
			{
				if (choose == 0)
				{
					nplecturer::edit(lecturer_id); 
					goto LAYOUT;
				}
				else if (choose == 1)
				{
					nplecturer::remove(lecturer_id); goto END;
					goto LAYOUT;

				}
			}
			if (c == 224 || c == 0) {
				c = getch();
				if (c == KEY_LEFT && choose == 1) choose--;
				else if (c == KEY_RIGHT && choose == 0) choose++;
			}
		}
	}
	else {
		gotoxy(54, 20, COLOR_WHITE_BACKGROUND); std::cout << "    Back    ";
		uint8_t c = getch();
		while (c != KEY_ESC && c != KEY_ENTER) c = getch();
	}
END:
	gotoxy(27, 13); std::cout << "                                                                  ";
	gotoxy(27, 14); std::cout << "                                                                  ";
	gotoxy(27, 15); std::cout << "                                                                  ";
	gotoxy(27, 16); std::cout << "                                                                  ";
	gotoxy(27, 17); std::cout << "                                                                  ";
	gotoxy(27, 18); std::cout << "                                                                  ";
	gotoxy(27, 19); std::cout << "                                                                  ";
	gotoxy(27, 20); std::cout << "                                                                  ";
	gotoxy(27, 21); std::cout << "                                                                  ";
}
void nplecturer::edit(const char* lecturer_id)
{
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, ON);
	if (lecturer == nullptr) return;

	gotoxy(46, 20, 128); std::cout << " Save change ";
	gotoxy(60, 20, 128); std::cout << "   Cancel    ";
	// Get detail
	gotoxy(32, 14, 143); std::cout << "                                                         ";
	gotoxy(32, 15, 143); std::cout << "                                                         ";
	gotoxy(32, 17, 143); std::cout << "                                                         ";
	gotoxy(33, 14, 143); std::cout << "Firstname    : "; std::cout << lecturer->pdata[3];
	gotoxy(33, 15, 143); std::cout << "Lastname     : "; std::cout << lecturer->pdata[2];
	gotoxy(33, 16, 128); std::cout << "Ranking      : "; std::cout << lecturer->pdata[4];
	gotoxy(33, 17, 128); std::cout << "ID             "; std::cout << lecturer->pdata[1];
	gotoxy(33, 18, 128); std::cout << "                              ";
	gotoxy(33, 19, 128); std::cout << "                              ";
	std::string fname, lname, ranking,id;
	if (read(48, 14, 143, fname, 20, SHOW, lecturer->pdata[3]) == KEY_ESC) return;
	if (read(48, 15, 143, lname, 20, SHOW, lecturer->pdata[2]) == KEY_ESC) return;
	if (read(48, 16, 128, ranking, 20, SHOW, lecturer->pdata[4]) == KEY_ESC) return;
	if (read(48, 17, 128, id, 20, SHOW, lecturer->pdata[1]) == KEY_ESC)return;
	for (int choose = 0;;)
	{
		gotoxy(46, 20, (choose == 0) ? COLOR_WHITE_BACKGROUND : 128); std::cout << " Save change ";
		gotoxy(60, 20, (choose == 1) ? COLOR_WHITE_BACKGROUND : 128); std::cout << "   Cancel    ";
		uint8_t c = getch();
		if (c == KEY_ESC)
			return;
		if (c == KEY_ENTER)
		{
			if (choose == 0)
			{
				if (id.size()) file::update(__LECTURER, lecturer->id, 1, id.c_str());
				if (fname.size()) file::update(__LECTURER, lecturer->id, 3, fname.c_str());
				if (lname.size()) file::update(__LECTURER, lecturer->id, 2, lname.c_str());
				if (ranking.size()) file::update(__LECTURER, lecturer->id, 4, ranking.c_str());
				gotoxy(46, 20, 128 + COLOR_BLUE); std::cout << " Save changes successfully.";
				PAUSE; return;
			}
			else if (choose == 1)
			{
				return;
			}
		}
		if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0) choose++;
		}
	}
}
void nplecturer::courseslist(const char* lecturer_id)
{
	int overflow = 0;
	csv_file courses_list((COURSE_PATH("__course.csv")).c_str(), def_course);
	int choose=0,cur=-1;
	while (cur=-1)
	{
		for (int i = choose; i < courses_list.count; i++)
		{
			int y = 19 + cur +overflow;
			if (y < 18 || y>19)continue;
			WORD COLOR_CODE = 128;
			if (strcmp(lecturer_id, courses_list.data[i].pdata[4]) == 0)
			{
				gotoxy(32, y, COLOR_CODE); std::cout << "                 |                                       ";
				gotoxy(33, y, COLOR_CODE); std::cout << courses_list.data[i].pdata[1];
				gotoxy(57, y, COLOR_CODE); std::cout << courses_list.data[i].pdata[3];
				cur++;
			}
			else
				continue;
		}
	NO_CHANGE:
		uint8_t c = getch();

		if (c == KEY_ESC) break;
		else if (c == KEY_ENTER) goto NO_CHANGE;
		else if (c == 224 || c == 0) {
			c = getch();
			if (c == KEY_UP && choose > 0)
			{
				if ((--choose + overflow) < 0)
					overflow++;
			}
			else if (c == KEY_DOWN && choose < cur)
			{
				if (++choose < cur - 3)
					overflow--;
			}
			else if (c == KEY_LEFT||c==KEY_RIGHT) break;
			else goto NO_CHANGE;
		}
	}
}
void nplecturer::remove(const char* lecturer_id)
{
	csv_file lecturer_list(__LECTURER);
	csv_line* lecturer = file::find(lecturer_list, lecturer_id, nullptr, OFF);
	if (lecturer == nullptr)return;
	gotoxy(33, 16,128+ COLOR_RED_BACKGROUND); std::cout << "Warning                                                ";
	gotoxy(33, 17, 112 + COLOR_RED); std::cout << "Are you sure to remove this lecturer, cannot be undone.";
	gotoxy(33, 18, 112); std::cout << "                                                       ";
	for (int choose = 0;;)
	{
		gotoxy(53, 18, (choose == 0) ? COLOR_WHITE_BACKGROUND : 112 + COLOR_RED); std::cout << "Yes";
		gotoxy(58, 18, (choose == 1) ? COLOR_WHITE_BACKGROUND : 112 + COLOR_RED); std::cout << "No";
	NO_CHANGE:
		uint8_t c = getch();
		if (c == KEY_ESC)return;
		else if (c == KEY_ENTER)
		{
			if (choose == 0)
			{
				file::remove(__LECTURER, lecturer->id);
				file::remove(ACCOUNT, file::find(ACCOUNT, lecturer->pdata[1], nullptr, OFF));
				file::update(COURSE_PATH("__course.csv").c_str(), file::find(__LECTURER,lecturer->pdata[1],nullptr,OFF), 4, "0");
				gotoxy(48, 18, 112 + COLOR_BLUE); std::cout << "Save change successfully";
				PAUSE;
				return;
			}
			if (choose == 1)
			{
				return;
			}
		}
		else if (c == 224 || c == 0)
		{
			c = getch();
			if (c == KEY_UP || c == KEY_DOWN) goto NO_CHANGE;
			else if (c == KEY_LEFT && choose == 1) choose--;
			else if (c == KEY_RIGHT && choose == 0)choose++;
		}
	}
}
