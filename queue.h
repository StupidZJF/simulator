/* 循环队列的顺序存储结构 */ /* 结构体 作用 构成一个新的数据结构 */
struct SqQueue
{
	int node_no;
	int arrive_time;
	int delay_time;
	int front;/* 头指针 指向当前元素 而不是下一个 说是指针，其实就是数组下标 */
	int rear; /* 尾指针 指向元素的下一个位置 这样可以与第几个的意思吻合 */
};

int InitQueue(struct SqQueue *Q);
int ClearQueue(struct SqQueue *Q);
int QueueEmpty(struct SqQueue Q);
int QueueLength(struct SqQueue Q);
int FrontQueue(struct SqQueue Q,int *e);
int RearQueue(struct SqQueue Q,int *e);
int InsertQueue(struct SqQueue *Q,int e);
int DeleteQueue(struct SqQueue *Q,int *e);
int TraverseQueue(struct SqQueue Q);
