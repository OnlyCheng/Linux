#define _CRT_SECURE_NO_WARNINGS
#include "Maze.h"

void menu()
{
	cout << "----------- 0、退出 ------------" << endl;
	cout << "----------- 1、简单 ------------" << endl;
	cout << "----------- 0、中等 ------------" << endl;
	cout << "----------- 0、困难 ------------" << endl;
}

int main()
{
	int** map_arr=NULL;
	FILE* pfIn1 = NULL;
	pfIn1 = fopen("mapfile1.cpp", "r");
	if (pfIn1 == NULL)
	{
		perror("open a file for read");
		exit(EXIT_FAILURE);
	}
	vector<vector<int>> map = Creat_map(5, 5);
	SetMap(map,5, 5, pfIn1);
	Print_Map(map, 5, 5);


	/*int** map_arr;
	menu();
	cout << "请选择：>  ";
	int input = 0;
	cin >> input;
	cout << endl;
	switch (input)
	{
	case 1:
		Pos entry(4, 4);
		FILE* pfIn1 = NULL;
		pfIn1 = fopen("mapfile1.cpp", "r");
		if (pfIn1 == NULL)
		{
			perror("open a file for read");
			exit(EXIT_FAILURE);
		}
		Creat_map(5, 5, map_arr, pfIn1);
		Print_Map(map_arr, 5, 5);
		Play(map_arr, entry, 5, 5);
		break;
	case 2:
		FILE* pfIn2 = NULL;
		pfIn2 = fopen("mapfile2.cpp", "r");
		if (pfIn1 == NULL)
		{
			perror("open a file for read");
			exit(EXIT_FAILURE);
		}
		break;
	case 3:
		FILE* pfIn3 = NULL;
		pfIn3 = fopen("mapfile1.cpp", "r");
		if (pfIn3 == NULL)
		{
			perror("open a file for read");
			exit(EXIT_FAILURE);
		}
		break;
	case 0:
		break;
	}*/
	
	return 0;
}