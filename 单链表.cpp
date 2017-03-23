#include <windows.h>
#include <stdio.h>

typedef struct Node
{
	int age;
	Node* next;
}Node;

Node* CreateList()
{
	Node* head;
	head = (Node*)malloc(sizeof(Node));
	head->age = 20;
	head->next = NULL;
	return head;
}

void InsertList(Node* head,int age)
{
	Node* tempHead = head;
	Node* temp = (Node*)malloc(sizeof(Node));

	temp->age = age;

	while(tempHead->next!=NULL)
	{
		tempHead = tempHead->next;	
	}

	tempHead->next = temp;
	temp->next = NULL;

	
}

void DeleteList(Node* head,int age)
{
	Node* tempHead = head;
	Node* pre = NULL;
	while(tempHead->age != age)
	{
		pre = tempHead;
		tempHead = tempHead->next;
	}
	pre->next = tempHead->next;
	free(tempHead);
}

void DisplayList(Node* head)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	temp = head;
	while(temp!=NULL)
	{
		printf("age = %d\n",temp->age);
		temp = temp->next;
	}
	free(temp);
}

int main()
{
	Node* head;
	head = CreateList();

	InsertList(head,21);
	InsertList(head,22);
	InsertList(head,23);

	DisplayList(head);
	
	printf("==================================\n");

	DeleteList(head,22);
	DeleteList(head,23);
	DisplayList(head);


	return 0;
}
