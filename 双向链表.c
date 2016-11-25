#include<cstdio>   
#include<cstdlib>   
typedef struct DoubleLinkedList  
{  
  int data;  
  struct DoubleLinkedList *pre;  
  struct DoubleLinkedList *next;  
}DlinkedList_Node;  
//建立链表   
DlinkedList_Node* createDLink()  
{  
  DlinkedList_Node *head,*p,*s;  
  int x;  
  head = (DlinkedList_Node*)malloc(sizeof(DlinkedList_Node));  
  p = head;  
  while(1)  
  {  
    printf("please input the data: \n");  
    scanf("%d",&x);  
    if(x != 65535)  
    {  
      s = (DlinkedList_Node*)malloc(sizeof(DlinkedList_Node));  
      s ->data = x;  
      s-> pre = p;  
      p->next = s;  
      p=s;  
    }  
    else 
      {  
        printf("\n数据输入结束\n");  
        break;  
      }  
  }  
  p->next = NULL;  
  head = head ->next;  
  head->pre = NULL;  
  return head;  
}  
//顺序、反序打印链表   
void printDLink(DlinkedList_Node *head)  
{  
  DlinkedList_Node *p,*s;  
  p = head;  
  printf("正序输出双向链表：\n");  
  while(p)  
  {  
    printf("%d ",p->data);  
    s = p;  
    p = p->next;  
  }  
  printf("\n 逆序输出双向链表: \n");  
  while(s)  
  {  
    printf("%d ",s->data);  
    s = s->pre;  
  }  
  printf("\n \n");  
}  
//删除一个结点   
DlinkedList_Node* deleteDlinkedList_Node(DlinkedList_Node *head,int i)  
{  
  DlinkedList_Node *p;  
  p = head;  
  if(p->data == i)  
  {  
    head = p->next;  
    head->pre = NULL;  
    free(p);  
    return head;  
  }  
  while(p)  
  {  
    if(p->data == i)  
    {  
    p->pre->next = p->next;  
    p->next->pre = p->pre;  
    free(p);  
    return head;  
    }  
    p = p->next;  
  }  
  printf("没有找到想要删除的数据\n");  
  return head;  
}  
//插入一个结点   
DlinkedList_Node* insertDlinkedList_Node(DlinkedList_Node *head,int i)  
{  
  DlinkedList_Node *p,*temp;  
  p = head;  
  temp = (DlinkedList_Node*)malloc(sizeof(DlinkedList_Node));  
  temp ->data = i;  
  if(i < p->data)//比头结点数据小，插入到链表头部   
  {  
    head = temp;  
    head->next = p;//此处p为原来的head   
    head->pre = NULL;  
    p->pre = head;//此处p为原来的head   
    return head;  
  }  
  while(p != NULL && i > p->data)//寻找合适的插入位置   
  {  
    p = p->next;  
  }  
  if(i < p->data)//在链表中间某处找到合适插入位置   
  {  
    temp ->next = p;  
    temp ->pre = p->pre;  
    p ->pre->next = temp;  
    p ->pre = temp;  
    return head;  
  }  
  else//没有找到合适的位置，只有将数据插入到链表尾部   
  {  
    p->next = temp; //遍历到链表尾部，p==NULL   
    temp ->pre = p;  
    temp ->next = NULL;  
    return head;  
  }  
}  
int main()  
{  
  DlinkedList_Node *head;  
  head = createDLink();  
  printDLink(head);  
  head = insertDlinkedList_Node(head,1012);  
  head = deleteDlinkedList_Node(head,1991);  
  printDLink(head);  
}  
/*****************************  
运行结果如下：  
please input the data:  
1991  
please input the data:  
1992  
please input the data:  
2013  
please input the data:  
2014  
please input the data:  
512  
please input the data:  
420  
please input the data:  
65535  
   
数据输入结束  
正序输出双向链表：  
1991 1992 2013 2014 512 420  
 逆序输出双向链表:  
420 512 2014 2013 1992 1991  
   
正序输出双向链表：  
1012 1992 2013 2014 512 420  
 逆序输出双向链表:  
420 512 2014 2013 1992 1012  
   
******************************/
