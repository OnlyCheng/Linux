#define _CRT_SECURE_NO_WARNINGS
#include "contact.h"
void menu()
{
	printf("****************************************\n");
	printf("******1 添加联系人    2 查找联系人******\n");
	printf("******3 显示联系人    4 删除联系人******\n");
	printf("******5 修改联系人    6 清空联系人******\n");
	printf("******7 联系人排序    0 退出通讯录******\n");
	printf("****************************************\n");
}
#include "contact.h"

int main()
{
	int input = 1;
	Contact con;
	InitContact(&con);
	LoadContact(&con);
	while (input)
	{
		menu();
		printf("请选择你要进行的操作:>");
		scanf("%d", &input);
		printf("\n");
		switch (input)
		{
		case  EXIT:
			SaveContact(&con);
			free(con.data);
			con.data = NULL;
			break;
		case ADD:
			AddContact(&con);
			SaveContact(&con);
			break;
		case SER:
			SearchContact(&con);
			break;
		case SHOW:
			ShowContact(&con);
			break;
		case DELE:
			DelContact(&con);
			SaveContact(&con);
			break;
		case MODIFY:
			ModifyContact(&con);
			SaveContact(&con);
			break;
		case CLEAR:
			ClearContact(&con);
			SaveContact(&con);
			break;
		case SORT:
			SortContact(&con);
			SaveContact(&con);
			break;
		default:
			printf("选择错误");
		}
	}

	return 0;
}