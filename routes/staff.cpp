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
}
