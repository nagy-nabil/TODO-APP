#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#define SIZE 200
typedef struct {
	char post[SIZE];
}Data;
typedef struct node {
	Data todo;
	struct node* next;
}Node;
typedef struct list {
	Node* head, * cur;
	int size, curPos;
}List;
void interFace(void);
void create(List* pl);
int listEmpty(List* pl);
void insert(int pos, char* entry, List* pl);
void seeTODO(List* pl);
void deleteTODO(int pos, List* pl);
int listSize(List* pl);
void save(FILE* fptr, List* pl);
void load(FILE* fptr, List* pl);
char choice(void);
char fName[] = "todo.bin";
int main() {
	List TODO;
	FILE* fptr = NULL;
	char post[SIZE], flag;
	int pos;
	create(&TODO);
	load(fptr, &TODO);
	interFace();
	while (1) {
		system("cls");
		system("color 3F");
		switch (choice()) {
		case '1':
			if (!listEmpty(&TODO))
				seeTODO(&TODO);
			else {
				puts("no todo for today\n\n\n");
				system("pause");
			}
			break;
		case '2':
			do {
				puts("type it .......");
				while ((getchar()) != '\n');
				gets(post);
				if (listEmpty(&TODO)) {
					insert(0, post, &TODO);
				}
				else {
					puts("1. to insert on the top");
					puts("2. to insert in default order");
					scanf("%c", &flag);
					if (flag == '1') {
						insert(0, post, &TODO);
					}
					else {
						insert(listSize(&TODO), post, &TODO);
					}
				}
				puts("want to add new toDo ?? press \'y\' for yes and \'n\' for no :-)");
				while ((getchar()) != '\n');
				scanf("%c", &flag);
			} while (flag == 'y');
			break;
		case '3':
			if (!listEmpty(&TODO)) {
				puts("enter the toDo\'s number you want to remove");
				if (scanf("%d", &pos) != 1) {
					puts("wrong input\n\n\n");
					break;
				}
				if (pos > listSize(&TODO) || pos < 0) {
					puts("wrong input\n\n\n");
				}
				else {
					deleteTODO(pos - 1, &TODO);
				}
			}
			else {
				puts("no todo for today\n\n\n");
			}
			system("pause");
			break;
		case '4':
			goto label1;
			break;
		case '\n':
		case '\t':
			break;
		default:
			puts("wrong input\n\n\n");
			system("pause");
		}
	}
label1:save(fptr, &TODO);
	return 0;
}
void interFace(void) {
	system("color 4F");
	puts("\n\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
	puts("\t\t} : } : } : } : } : } : } : \tWlcome To The TODO APP\t } : } : } : } : } : } : } : ");
	puts("\n\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("\t\t@nagy_nabil");
	system("pause");
}
void create(List* pl)
{
	pl->head = NULL;
	pl->size = 0;
}
int listEmpty(List* pl) {
	return !pl->head;
}

void insert(int pos, char* entry, List* pl)
{
	Node* pn = (Node*)malloc(sizeof(Node));
	strcpy(pn->todo.post, entry);
	pn->next = NULL;
	if (pos == 0) {
		pn->next = pl->head;
		pl->head = pn;
		pl->cur = pl->head;
		pl->curPos = 0;
	}
	else {
		if (pos <= pl->curPos) {
			pl->cur = pl->head;
			pl->curPos = 0;
		}
		while (pl->curPos < pos - 1) {
			pl->cur = pl->cur->next;
			pl->curPos++;
		}
		pn->next = pl->cur->next;
		pl->cur->next = pn;
	}
	pl->size++;
}

void seeTODO(List* pl)
{
	Node* pn = pl->head;
	system("cls");
	int i = 1;
	while (pn) {
		printf("%d.)%s\n", i, pn->todo.post);
		pn = pn->next;
		++i;
	}
	puts("\n\n");
	system("pause");
}

void deleteTODO(int pos, List* pl)
{
	Node* pn = NULL;
	if (pos == 0) {
		pn = pl->head;
		pl->head = pn->next;
		free(pn);
		pl->cur = pl->head;
		pl->curPos = 0;
	}
	else {
		if (pos <= pl->curPos) {
			pl->cur = pl->head;
			pl->curPos = 0;
		}
		while (pl->curPos < pos - 1) {
			pl->cur = pl->cur->next;
			pl->curPos++;
		}
		pn = pl->cur->next;
		pl->cur->next = pn->next;
		free(pn);
	}
	pl->size--;
}

int listSize(List* pl)
{
	return pl->size;
}

void save(FILE* fptr, List* pl)
{
	Node* pn = pl->head;
	fptr = fopen(fName, "wb");
	if (fptr) {
		while (pn) {
			fwrite(&pn->todo, sizeof(Data), 1, fptr);
			pn = pn->next;
		}
		fclose(fptr);
	}
	else
	{
		perror("error in saving");
		return;
	}
}

void load(FILE* fptr, List* pl)
{
	Data temp;
	fptr = fopen(fName, "rb");
	if (fptr) {
		while (!feof(fptr)) {
			if (fread(&temp, sizeof(Data), 1, fptr) == 1)
				//to insert in same order as the file
				insert(pl->size, temp.post, pl);
		}
		fclose(fptr);
	}
	else {

		return;
	}
}

char choice(void)
{
	char ch;
	puts("1. To see your TODO list");
	puts("2. To create new TODO");
	puts("3. To delete your TODO");
	puts("4. EXIT\n\n\n");
	printf("%s", "Enter your choice   : ");
	scanf("%c", &ch);
	return ch;
}
