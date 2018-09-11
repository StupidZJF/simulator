#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include "simulator.h"

#define OK 1 
#define ERROR 0
#define TURE 1
#define FALSE 0
#define MAXSIZE 20 /* 队列最大的成员个数 即数组的长度 */

typedef int QElemType;


/* 初始化一个空的队列 */
int InitQueue(struct SqQueue *Q)/* 改变 在主函数的 队列中元素的数值 故形参用指针 */
{
	Q -> front = 0;
	Q -> rear = 0;
	return OK;
}


/* 将队列清空 */
int ClearQueue(struct SqQueue *Q)
{
	Q -> front = 0;
	Q -> rear = 0;
	return OK;
}


/* 判断队列是否为空 是则返回Ture */
int QueueEmpty(struct SqQueue Q)/* 不改变 在主函数的 队列中元素的数值，仅copy该队列，对相关元素进行判断操作 */
{
	if(Q.front == Q.rear)
		return TURE;
	else 
		return FALSE;
}


/* 返回队列中元素的个数 */
int QueueLength(struct SqQueue Q)
{
	return ( (Q.rear - Q.front + MAXSIZE) % MAXSIZE );
}


/* 返回队首元素 */
int FrontQueue(struct SqQueue Q,int *e)/* *e 作用 中转队首元素的数值 */
{
	if(Q.front == Q.rear)
		return ERROR;
	*e = Q.data[Q.front];/* 分清 是copy判断队列状态 还是中转or改变队列元素  前者形参正常结构类型 后者指针  */
	return OK;
}


/* 返回队尾元素 */
int RearQueue(struct SqQueue Q,int *e)
{
	if(Q.front == Q.rear)
		return ERROR;
	*e = Q.data[Q.rear-1];/* 假设队尾元素是a0 那么Q.rear实际指向a1 */
	return OK;
}


/* 队列未满 队尾插入操作  插入一个元素 队尾指针+1 */
int InsertQueue(struct SqQueue *Q,int e)
{
	if( (Q -> rear + 1) % MAXSIZE == Q -> front ) 
		return ERROR;
	Q -> data[Q -> rear] = e;
	Q -> rear = (Q -> rear + 1) % MAXSIZE;/* 若到了数组最后 则转到头部 */
	return OK;
}


/* 队列未空 队首删除操作 并返回删除的那个元素的数值  与插入类似，每删除一个，队首指针+1 */
int DeleteQueue(struct SqQueue *Q,int *e)
{
	if( Q -> rear == Q -> front ) 
		return ERROR;
	*e = Q -> data[Q -> front];
	Q -> front = (Q -> front + 1) % MAXSIZE;/* 若到了数组最后 则转到头部 */
	return OK;
}


int visit(int c)
{
	printf("%d ",c);
	return OK;
}


/* 遍历整个队列 从头到尾输出每一个元素的数值 */
int TraverseQueue(struct SqQueue Q)
{
	int j;
	j = QueueEmpty(Q);
	if( j == TURE)
		printf("该队列为空队列\n");

	int i = Q.front;
	while(i != Q.rear)
	{
		visit(Q.data[i]);
		i = (i+1) % MAXSIZE;
	}
	printf("\n");
	return OK;
}


int main()
{
	int a,b,Qlength;
	struct SqQueue Q;
	int e,f;

	/* 队列初始化 */
	a = InitQueue(&Q);
	if(a == OK)
		printf("队列初始化成功\n");
	else 
		return ERROR;
	
	/* 判断队列是否为空 */
	a = QueueEmpty(Q);
	if(a == TURE)
		printf("队列为空\n");
	else
		return ERROR;


	/* 通过插入构造新队列 */
	int i;
	printf("请输入你想插入的元素的个数:%d 队列长度为:%d 其中有一个元素存在且为空\n",MAXSIZE-1,MAXSIZE);
	for(i = 1;i <= (MAXSIZE - 1);i++)
	{
		e = i;
		InsertQueue(&Q,e);
	} 
	/* 遍历新队列 */
	TraverseQueue(Q);


	/* 求取队列长度 */
	Qlength = QueueLength(Q);
	printf("队列长度为:%d\n",Qlength);


	/* 返回队首元素 */
	a = FrontQueue(Q,&e);
	if(a == OK)
		printf("返回队首元素操作成功\n");
	else
		return ERROR;
	printf("队首元素:%d\n",e);


	/* 返回队尾元素 */
	a = RearQueue(Q,&e);
	if(a == OK)
		printf("返回队尾元素操作成功\n");
	else
		return ERROR;
	printf("队尾元素:%d\n",e);

	printf("是否执行接下来的删除操作?yes or no(yes 输入1  no 输入0)\n");
	scanf("%d",&b);
	switch(b)
	{
		case 0:break;
		case 1:
		/* 删除队首的两个元素 */
		DeleteQueue(&Q,&e);
		printf("第一次删除操作返回的队首元素:%d\n",e);
		DeleteQueue(&Q,&f);
		printf("第二次删除操作返回的队首元素:%d\n",f);
		/* 遍历此时的队列 */
		TraverseQueue(Q);
		break;
	}

	ClearQueue(&Q);
	a = QueueEmpty(Q);
	if(a == TURE)
		printf("队列为空\n");
	else
		return ERROR;

	printf("\n 您已经完成队列的复习 :) \n");
	return 0;
}
