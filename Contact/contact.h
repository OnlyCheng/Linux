#ifndef __CONTACT_H__
#define __CONTACT_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1000
#define NAME_MAX 20
#define SEX_MAX 5
#define ADDR_MAX 15
#define TELE_MAX 12
#define CAPACITY 3
#define ENLARGE 2

#define FILENAME "contact_file"

enum po
{
	EXIT,
	ADD,
	SER,
	SHOW,
	DELE,
	MODIFY,
	CLEAR,
	SORT
};

typedef struct PeoInfo
{
	char name[NAME_MAX];
	int age;
	char sex[SEX_MAX];
	char addr[ADDR_MAX];
	char tele[TELE_MAX];
}PeoInfo;

typedef struct Contact
{
	PeoInfo* data;
	int sz;
	int capacity;
}Contact, *pContact;


void InitContact(pContact pcon);
void AddContact(pContact pcon);
void EnlContact(pContact pcon);
void SearchContact(pContact pcon);
void ShowContact(const pContact pcon);
void DelContact(pContact pcon);
void ModifyContact(pContact pcon);
void ClearContact(pContact pcon);
void SortContact(pContact pcon);
void LoadContact(pContact pcon);
void SaveContact(pContact pcon);

#endif