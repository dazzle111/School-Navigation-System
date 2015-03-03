#ifndef DAZZLE_H
#define DAZZLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVEX 30
#define INFINITY 32768

typedef struct ArcNode
{
	int adjvex;
	int weight;
	struct ArcNode *next;
}ArcNode;		///节点属性
typedef struct VertexNode
{
	int used;
	char name[20];
	char vex_information[100];
	ArcNode *head;
}VertexNode;		///节点信息
typedef struct AdjList
{
	VertexNode vertex[MAXVEX];
	int vexnum;
	int arcnum;
}AdjList;		///临接表属性

typedef struct node
{
	int data;
	struct node *next;
}Qnode;			
typedef struct
{
	Qnode *front;
	Qnode *rear;
}LQueue;		///队列结构体


AdjList  *Create();	///临接表的创建

LQueue *Init_LQueue();	///初始化队列
int In_Queue(LQueue *head,int i);	///入队
int Out_Queue(LQueue *head,int *i);	///出队
int Empty_Queue(LQueue *head);		///判队空


int find_weight(AdjList *G,int start,int end);	///通过初始地点和终点找对应权值
int find_vex(AdjList *G);			///查找未用过的节点位置
int find_name(AdjList *G,char *name);		///通过名字查找对应名字的节点下标

void display_school(AdjList *G);		///显示所有节点信息
void display_choice(AdjList *G);		///显示信息的选择提示
void display_vex(AdjList *G);			///显示某一点的信息

void add_choice(AdjList *G);			///增加边或者节点的选择提示
void add_arc(AdjList *G);			///增加边
void add_school(AdjList *G);     		///增加节点
void del_choice(AdjList *G);			///删除边或节点的选择提示
void del_arc(AdjList *G);			///删除边
void del_school(AdjList *G);			///删除节点

void trans(AdjList *G);				///将临接表转化成临接矩阵
			
void in_file(AdjList *G);			///将临接表信息写入文件
AdjList *out_file();				///将文件信息读到临接表

void mini_path(AdjList *G);			///构建最小生成树的提示
void Prim(AdjList *G,int start);		///最小生成树
void travel_graph(AdjList *G);			///进行广搜前的输入提示
void BFS(AdjList *G,int start,int end);		///广搜最短路径
void search_school(AdjList *G);			///寻找路径长度最短的提示
void Dijkstra(AdjList *G,int start,int end,int dist[],int path[][MAXVEX]);		///寻找路径最短的算法

int find_all_path(AdjList *G);
int find_path(AdjList *G,int start,int end,int lenght,int path[],int visited[]);

#endif
