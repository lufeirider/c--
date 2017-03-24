#include<iostream>
using namespace std;

typedef struct biNode
{
	char data;
	biNode* lchild;
	biNode* rchild;
}biNode;

void CreateBiTree(biNode* &T)  //把地址转成指针。可以达到既可以使用指针，又传入真实地址
{
	char ch;
	cin>>ch;
	if(ch == '#')
	{
		T = NULL;
	}
	else
	{
		T = new biNode;
		T->data = ch;
		CreateBiTree(T->lchild);
		CreateBiTree(T->rchild);
	}

}

void InOrderTraverse(biNode* T)
{
	if(T)
	{
		InOrderTraverse(T->lchild);
		cout<<T->data;
		InOrderTraverse(T->rchild);
	}
}

int main()
{
	/*   没有申请空间，还是野指针。
	biNode* tree;
	tree->data=100;
	*/

	biNode* tree;
	CreateBiTree(tree);
	
	InOrderTraverse(tree);
	return 0;
}
