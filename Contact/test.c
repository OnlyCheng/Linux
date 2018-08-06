#define _CRT_SECURE_NO_WARNINGS
#include "contact.h"
void menu()
{
	printf("****************************************\n");
	printf("******1 �����ϵ��    2 ������ϵ��******\n");
	printf("******3 ��ʾ��ϵ��    4 ɾ����ϵ��******\n");
	printf("******5 �޸���ϵ��    6 �����ϵ��******\n");
	printf("******7 ��ϵ������    0 �˳�ͨѶ¼******\n");
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
		printf("��ѡ����Ҫ���еĲ���:>");
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
			printf("ѡ�����");
		}
	}

	return 0;
}