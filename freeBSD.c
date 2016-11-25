#include <cstdio>
#include <cstdlib>
#include <windows.h>

typedef struct ipasFrag
{
	int ipId;
	int mf;
	int off;
	int ipTtl;
	char ipSrc[100];
	char ipDst[100];
	char data[100];
	struct ipasFrag *pre;
	struct ipasFrag *next;
}ipasFragNode;

typedef struct ipq
{
	int ipqId;
	int ipTtl;
	char ipSrc[100];
	char ipDst[100];
	struct ipasFrag *ipaPre;
	struct ipasFrag *ipaNext;
	struct ipq *pre;
	struct ipq *next;
}ipqNode;

/* 建立链表 
ipqNode* createDLink()
{
	ipqNode *head, *tail, *temp;
	int x;
	head = (ipqNode *) malloc( sizeof(ipqNode) );
	tail = head;
	while ( 1 )
	{
		printf( "please input the ipqId: \n" );
		scanf( "%d", &x );
		if ( x != 65535 )
		{
			temp = (ipqNode *) malloc( sizeof(ipqNode) );
			temp->ipqId = x;
			temp->pre = tail;
			temp->next=NULL;
			temp->ipaNext=NULL;
			temp->ipaPre=NULL;
			tail->next = temp;
			tail = temp;
		}else  {
			printf( "\n数据输入结束\n" );
			break;
		}
	}
	tail->next = NULL;
	head = head->next;
	head->pre = NULL;
	return(head);
}
*/

/* 顺序、反序打印链表 */
void printDLink( ipqNode *head )
{
	ipqNode *p, *s;
	p = head;
	if(p->ipqId==0)
	{
		printf( "输出ipq节点：\n" );
	}

	while ( p )
	{
		if(p->ipqId!=0)
		{
			printf( "%d ", p->ipqId );
		}
		s = p;
		p = p->next;
	}

	printf( "\n \n" );
}


/*删除一个结点 
ipqNode* deleteIpqNode( ipqNode *head, int i )
{
	ipqNode *p;
	p = head;
	if ( p->ipqId == i )
	{
		head = p->next;
		head->pre = NULL;
		free( p );
		return(head);
	}
	while ( p )
	{
		if ( p->ipqId == i )
		{
			p->pre->next = p->next;
			p->next->pre = p->pre;
			free( p );
			return(head);
		}
		p = p->next;
	}
	printf( "没有找到想要删除的数据\n" );
	return(head);
}
*/

/* 插入一个结点 */
ipqNode* insertIpqNode( ipqNode *head, ipqNode* i )
{
	ipqNode *p, *temp;
	p = head;
	temp = (ipqNode *) malloc( sizeof(ipqNode) );
	temp = i;
	if ( i->ipqId < p->ipqId )                              /* 比头结点数据小，插入到链表头部 */
	{
		head = temp;
		head->next = p;                    /* 此处p为原来的head */
		head->pre = NULL;
		p->pre = head;                 /* 此处p为原来的head */
		return(head);
	}
	while ( p->next != NULL && i->ipqId > p->ipqId )        /* 寻找合适的插入位置 */
	{
		p = p->next;
	}
	if ( i->ipqId < p->ipqId )                              /* 在链表中间某处找到合适插入位置 */
	{
		temp->next = p;
		temp->pre = p->pre;
		p->pre->next = temp;
		p->pre = temp;
		return(head);
	}else  { /* 没有找到合适的位置，只有将数据插入到链表尾部 */
		p->next = temp; /* 遍历到链表尾部，p==NULL */
		temp->pre = p;
		temp->next = NULL;
		return(head);
	}
}


void printIpasFrag(ipqNode* head)
{
	char data[100]={0};
	ipasFragNode *temp = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	temp = head->ipaNext;
	if(temp)
	{
		printf("ttl值为%d ip源地址是%s ip目的地址是%s\n",temp->ipTtl,temp->ipSrc,temp->ipDst);
		printf("打印id为%d的所有数据包分片\n",temp->ipId);
	}
	
	while ( temp )
	{	
		strcat(data,temp->data);
		printf( "%d ", temp->off );
		temp = temp->next;
	}
	printf("%s",data);
	printf("\n");
}

ipqNode* insertIpasFragNode( ipqNode *head, ipasFragNode *i )
{
	ipasFragNode *p, *temp;
	p = head->ipaNext;
	temp = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	temp = i;

	while ( p->next != NULL && temp->off > p->off )        /* 寻找合适的插入位置 */
	{
		p = p->next;
	}
	if ( temp->off < p->off )                              /* 在链表中间某处找到合适插入位置 */
	{
		temp->next = p;
		temp->pre = p->pre;
		p->pre->next = temp;
		p->pre = temp;
		return(head);
	}else  { /* 没有找到合适的位置，只有将数据插入到链表尾部 */
		p->next = temp; /* 遍历到链表尾部，p==NULL */
		temp->pre = p;
		temp->next = NULL;
		return(head);
	}
}

int checkExit(ipqNode* head,int id)
{
	ipqNode* p;
	p=head;
	while ( p )
	{
		if ( p->ipqId == id )
		{
			return 1;
		}
		p = p->next;
	}
	return 0;
}


void insert(ipqNode* head,ipasFragNode *test)
{
	ipqNode* p=NULL;
	p=head;
	ipqNode *htest = (ipqNode *) malloc( sizeof(ipqNode) );
	if(checkExit(head,test->ipId))
	{
		while ( p )
		{
			if ( p->ipqId == test->ipId )
			{
				if(p->ipaNext==NULL){
					test->pre = (ipasFragNode *)p;
					test->next = NULL;
					p->ipaNext = test;
				}else{
					insertIpasFragNode(p,test);
				}
				break;
			}
			p = p->next;
		}
	}else{
		p=head;
		htest->ipaNext=NULL;
		htest->ipqId=test->ipId;
		htest->ipTtl=test->ipTtl;
		strcpy(htest->ipSrc,test->ipSrc);
		strcpy(htest->ipDst,test->ipDst);
		head = insertIpqNode( head, htest );
		while ( p )
		{
			if ( p->ipqId == test->ipId )
			{
				if(p->ipaNext==NULL){
					test->pre = (ipasFragNode *)p;
					test->next = NULL;
					p->ipaNext = test;
				}else{
					insertIpasFragNode(p,test);
				}
				break;
			}
			p = p->next;
		}
	}
}


int main()
{
	ipqNode *head=(ipqNode *) malloc( sizeof(ipqNode) );
	head->ipqId=NULL;
	head->ipaNext=NULL;
	head->ipaPre=NULL;
	head->next=NULL;

	ipqNode* p=NULL;
	ipqNode *htest = (ipqNode *) malloc( sizeof(ipqNode) );
	p=head;

	ipasFragNode *test = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test1 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test2 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test3 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test4 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test5 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test6 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
	ipasFragNode *test7 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );

	test->ipId=1;
	test->mf=0;
	test->off=0;
	test->ipTtl=128;
	strcpy(test->ipSrc,"192.168.1.2");
	strcpy(test->ipDst,"192.168.1.3");
	strcpy(test->data,"aaaa");

	test1->ipId=1;
	test1->mf=1;
	test1->off=2960;
	test1->ipTtl=128;
	strcpy(test1->ipSrc,"192.168.1.2");
	strcpy(test1->ipDst,"192.168.1.3");
	strcpy(test1->data,"ccccc");

	test2->ipId=1;
	test2->mf=0;
	test2->off=1480;
	test2->ipTtl=128;
	strcpy(test2->ipSrc,"192.168.1.2");
	strcpy(test2->ipDst,"192.168.1.3");
	strcpy(test2->data,"bbbbb");

	test3->ipId=7;
	test3->mf=0;
	test3->off=0;
	test3->ipTtl=128;
	strcpy(test3->ipSrc,"192.168.1.2");
	strcpy(test3->ipDst,"192.168.1.3");
	strcpy(test3->data,"dddd");

	test4->ipId=7;
	test4->mf=0;
	test4->off=1480;
	test4->ipTtl=128;
	strcpy(test4->ipSrc,"192.168.1.2");
	strcpy(test4->ipDst,"192.168.1.3");
	strcpy(test4->data,"eeeee");

	test5->ipId=22;
	test5->mf=0;
	test5->off=0;
	test5->ipTtl=128;
	strcpy(test5->ipSrc,"192.168.1.2");
	strcpy(test5->ipDst,"192.168.1.3");
	strcpy(test5->data,"fffff");

	test6->ipId=22;
	test6->mf=0;
	test6->off=1480;
	test6->ipTtl=128;
	strcpy(test6->ipSrc,"192.168.1.2");
	strcpy(test6->ipDst,"192.168.1.3");
	strcpy(test6->data,"ggggg");


	insert(head,test);
	insert(head,test1);
	insert(head,test2);
	insert(head,test3);
	insert(head,test4);
	insert(head,test5);
	insert(head,test6);

	
	for(int i=0;i<7;i++)
	{
		test7 = (ipasFragNode *) malloc( sizeof(ipasFragNode) );
		scanf( "%d %d %d %s %s %d %s", &test7->ipId,&test7->mf,&test7->off,&test7->ipSrc,&test7->ipDst,&test7->ipTtl,&test7->data);
		insert(head,test7);
	}

	printf("===============================================\n");
	printDLink(head);
	printf("===============================================\n");
	while ( p )
	{
		printIpasFrag(p);
		p = p->next;
	}
	printf("\n===============================================\n");


	return 0;
}

