#define _CRT_SECURE_NO_WARNINGS
#include "contact.h"

void EnlContact(pContact pcon);

void LoadContact(pContact pcon)
{
	FILE* pfIn = fopen(FILENAME, "r");
	PeoInfo tmp = { 0 };
	while (fread(&tmp, sizeof(PeoInfo), 1, pfIn))
	{
		if (pcon->sz >= pcon->capacity)
		{
			EnlContact(pcon);
			//printf("增容成功！\n");
		}
		pcon->data[pcon->sz] = tmp;
		pcon->sz++;
	}
}

void SaveContact(pContact pcon)
{
	FILE* pfOut = fopen(FILENAME, "w");
	int i = 0;
	if (pfOut == NULL)
	{
		perror("SaveContactData::fopen");
		exit(EXIT_FAILURE);
	}
	//
	for (i = 0; i<pcon->sz; i++)
	{
		fwrite(pcon->data + i, sizeof(PeoInfo), 1, pfOut);
	}
	fclose(pfOut);
	printf("自动保存成功！\n");
}

void InitContact(pContact pcon)
{

	/*memset(pcon->data, 0, sizeof(PeoInfo));
	pcon->sz = 0;*/
	pcon->data = (PeoInfo*)malloc(CAPACITY*sizeof(PeoInfo));
	if (pcon->data == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	else
	{
		pcon->sz = 0;
		pcon->capacity = CAPACITY;
	}

}
void EnlContact(pContact pcon)
{
	PeoInfo* ptr=realloc(pcon->data, (pcon->capacity + ENLARGE)*sizeof(PeoInfo));
	if (ptr == NULL)
	{
		perror("realloc:");
		exit(EXIT_FAILURE);
	}
	else
	{
		pcon->data = ptr;
	}
	pcon->capacity += ENLARGE;
	printf("增容成功！\n");
}
void AddContact(pContact pcon)
{
	if (pcon->sz >= pcon->capacity)
	{
		//printf("增容成功！\n");
		EnlContact(pcon);
		printf("请输入姓名：>");
		scanf("%s", pcon->data[pcon->sz].name);
		printf("请输入年龄：>");
		scanf("%d", &pcon->data[pcon->sz].age);
		printf("请输入性别：>");
		scanf("%s", pcon->data[pcon->sz].sex);
		printf("请输入地址：>");
		scanf("%s", pcon->data[pcon->sz].addr);
		printf("请输入电话：>");
		scanf("%s", pcon->data[pcon->sz].tele);
		pcon->sz++;
		printf("添加成功！\n");
	}
	else
	{
		printf("请输入姓名：>");
		scanf("%s", pcon->data[pcon->sz].name);
		printf("请输入年龄：>");
		scanf("%d", &pcon->data[pcon->sz].age);
		printf("请输入性别：>");
		scanf("%s", pcon->data[pcon->sz].sex);
		printf("请输入地址：>");
		scanf("%s", pcon->data[pcon->sz].addr);
		printf("请输入电话：>");
		scanf("%s", pcon->data[pcon->sz].tele);
		pcon->sz++;
		printf("添加成功！\n");
	}
}
int find(pContact pcon, char* name)
{
	int i = 0;
	for (i = 0; i < pcon->sz; i++)
	{
		if (strcmp(pcon->data[i].name, name) == 0)
		{
			return i;
		}
	}
	return -1;
}
void SearchContact(pContact pcon)
{
	int ret = 0;
	char name[NAME_MAX] = { 0 };
	printf("请输入你要查找的姓名：>");
	scanf("%s", name);
	ret = find(pcon, name);
	if (ret == -1)
	{
		printf("该联系人不存在\n");
		return;
	}
	else
	{
		printf("%8s\t %5d\t %5s\t %15s\t %12s\n",
			pcon->data[ret].name,
			pcon->data[ret].age,
			pcon->data[ret].sex,
			pcon->data[ret].addr,
			pcon->data[ret].tele);
	}
}
void ShowContact(const pContact pcon)
{
	int i = 0;
	if (pcon->sz == 0)
	{
		printf("联系人为空！\n");

	}
	else
	{
		printf("  姓名   年龄   性别       地址         电话\n ");
		for (i = 0; i < pcon->sz; i++)
		{
			printf("%5s\t%3d\t%2s%20s\t%12s",
				pcon->data[i].name,
				pcon->data[i].age,
				pcon->data[i].sex,
				pcon->data[i].addr,
				pcon->data[i].tele);
			printf("\n");
		}
	}
}

void DelContact(pContact pcon)
{
	int ret = 0;
	int i = 0;
	char name[NAME_MAX] = { 0 };
	printf("请输入你要删除联系人的名字：>");
	scanf("%s", name);
	ret = find(pcon, name);
	if (ret == -1)
	{
		printf("该联系人不存在\n");
	}
	else
	{
		for (i = ret; i < pcon->sz; i++)
		{
			pcon->data[i] = pcon->data[i + 1];
		}
		printf("删除成功！\n");
		pcon->sz--;
	}
}
void ModifyContact(pContact pcon)
{
	int ret = 0;
	char name[NAME_MAX] = { 0 };
	printf("请输入你要修改联系人的名字：>");
	scanf("%s", name);
	ret = find(pcon, name);
	if (ret == -1)
	{
		printf("该联系人不存在\n");
		return;
	}
	else
	{
		printf("请输入修改后姓名：>");
		scanf("%s", pcon->data[ret].name);
		printf("请输入修改后年龄：>");
		scanf("%d", &pcon->data[ret].age);
		printf("请输入修改后性别：>");
		scanf("%s", pcon->data[ret].sex);
		printf("请输入修改后地址：>");
		scanf("%s", pcon->data[ret].addr);
		printf("请输入修改后电话：>");
		scanf("%s", pcon->data[ret].tele);
	}
	printf("修改成功！\n");
}
void ClearContact(pContact pcon)
{
	pcon->sz = 0;
	printf("联系人已清空！\n");
}
void SortContact(pContact pcon)
{
	int i = 0;
	int j = 0;
	for (i = 0; i<pcon->sz - 1; i++)
	{
		for (j = 0; j<pcon->sz - 1 - i; j++)
		{
			PeoInfo temp = { 0 };
			if (strcmp(pcon->data[j].name, pcon->data[j + 1].name)>0)
			{
				temp = pcon->data[j];
				pcon->data[j] = pcon->data[j + 1];
				pcon->data[j + 1] = temp;
			}
		}
	}
	printf("排序成功！\n");
}