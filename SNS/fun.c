
#include "dazzle.h"

int graph[MAXVEX][MAXVEX]={0};
int check[MAXVEX]={0};

int find_vex(AdjList *G)
{
	int i;
	for(i=1;i<=G->vexnum;i++)
		if(G->vertex[i].used==0)
		return i;
	return (G->vexnum+1);

}

int find_name(AdjList *G,char *name)
{
	int i;
	for(i=1;i<=G->vexnum;i++)
	if(strcmp(G->vertex[i].name,name)==0 && G->vertex[i].used==1)
		return i;
	return -1;
}

AdjList  *Create()
{

	int i,j,k,weight;
	AdjList *G;
	ArcNode *p;
	G= (AdjList *)malloc(sizeof(AdjList));
	printf("请输入顶点数和边数:\n");
	scanf("%d,%d",&G->vexnum,&G->arcnum);

	for(i=1;i<=G->vexnum;i++)
	{
		printf("地点名称:");
		scanf("%s",G->vertex[i].name);
		printf("地点信息:");
		scanf("%s",G->vertex[i].vex_information);
		G->vertex[i].head=NULL;
		G->vertex[i].used=1;
	}

	for(k=1;k<=G->arcnum;k++)
	{
		printf("请输入边(vi,vj)上的顶点序号:\n");
		scanf("%d,%d",&i,&j);
		printf("距离:");
		scanf("%d",&weight);
		p=(ArcNode *)malloc(sizeof(ArcNode));
		p->adjvex=j;
		p->weight=weight;
		p->next=G->vertex[i].head;
		G->vertex[i].head=p;

		p=(ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex=i;
		p->weight=weight;
		p->next=G->vertex[j].head;
		G->vertex[j].head=p;

	}
	in_file(G);
	return G;
}

void in_file(AdjList *G)
{
	FILE *fp;
	int i,j,k,count=0;
	ArcNode *p;

	if((fp=fopen("information.txt","w"))==NULL)
	{
		printf("打开文件失败!");
		return ;
	}
	
	for(i=1;i<=G->vexnum;i++)
	{
		if(G->vertex[i].used == 0 || G->vertex[i].head == NULL)
		continue;

		count = 0;
		fprintf(fp,"%s %s\n",G->vertex[i].name,G->vertex[i].vex_information);
		p = G->vertex[i].head;
		while(p)
		{
			count++;
			p=p->next;
		}
		fprintf(fp,"%d\n",count);
		p=G->vertex[i].head;
		while(p)
		{
			fprintf(fp,"%d %d\n",p->adjvex,p->weight);
			p=p->next;
		}
	}

	fclose(fp);
}

AdjList *out_file()
{
	FILE *fp;
	int i=1,k,count;
	ArcNode *p;
	AdjList *G;
	G=(AdjList *)malloc(sizeof(AdjList));
	
	if((fp=fopen("information.txt","r"))==NULL)
	{
		printf("文件打开失败!");
		return ;
	}
	
	G->vexnum=0;

	while(feof(fp) != 1)
	{
		G->vertex[i].head=NULL;
		G->vertex[i].used=1;
		G->vexnum++;
		fscanf(fp,"%s %s\n",G->vertex[i].name,G->vertex[i].vex_information);	
		fscanf(fp,"%d\n",&count);
		for(k=1;k<=count;k++)
		{
			p=(ArcNode *)malloc(sizeof(ArcNode));
			fscanf(fp,"%d %d\n",&p->adjvex,&p->weight);
			p->next=G->vertex[i].head;
			G->vertex[i].head=p;
		
		}
		i++;
	}

	fclose(fp);
	return G;
}


void search_school(AdjList *G)
{
	char city[20];
	int start,end;
	int dist[MAXVEX],path[MAXVEX][MAXVEX]={0};

	while(1)
	{
		printf("请输入起点:");
		scanf("%s",city);
		start=find_name(G,city);	
		if(start!=-1)
			break;
		else
			printf("无此地点!\n");
	}
	while(1)
	{
		printf("输入终点:");
		scanf("%s",city);
		end=find_name(G,city);
		if(end != -1)
			break;
		else
			printf("无此地点!\n");
	}

	Dijkstra(G,start,end,dist,path);

}


void Dijkstra(AdjList *G,int start,int end,int dist[],int path[][MAXVEX])
{
	int mindist,i,j,k,t=1;

	trans(G);
	for(i=1;i<=G->vexnum;i++)
	{
		dist[i]=graph[start][i];
		if(graph[start][i]!=INFINITY)
		path[i][1]=start;
	}
	path[start][0]=1;
	for(i=2;i<=G->vexnum;i++)
	{
		mindist=INFINITY;

		for(j=1;j<=G->vexnum;j++)
			if(!path[j][0] && dist[j]<mindist)
			{
				k=j;mindist=dist[j];
			}

		if(mindist == INFINITY) return ;
		path[k][0]=1;

		for(j=1;j<=G->vexnum;j++)
		{
			if(!path[j][0] && graph[k][j]<INFINITY && dist[k]+graph[k][j]<dist[j])
			{
				dist[j]=dist[k]+graph[k][j];
				t=1;	
				while(path[k][t])
				{
					path[j][t]=path[k][t];
					t++;
				}
				path[j][t]=k;
				path[j][t+1]=0;
			}
		}
	}

	for(i=1;i<=G->vexnum;i++)
		if(i==end)
			break;

	printf("%s--->%s的最短路线为:从%s",G->vertex[start].name,G->vertex[end].name,G->vertex[start].name);
	for(j=2;path[i][j]!=0;j++)
		printf("->%s",G->vertex[path[i][j]].name);
	printf("->%s,距离为%dm\n",G->vertex[end].name,dist[i]);
	
}


void display_school(AdjList *G)
{
	int i,j;
	ArcNode *p;

	for(i=1;i <= G->vexnum;i++)
	{
		if(!G->vertex[i].used || !G->vertex[i].head)
		continue;
		printf("地点名称:%s\n",G->vertex[i].name);
		printf("地点信息:%s\n",G->vertex[i].vex_information);
		p=G->vertex[i].head;
		while(p)
		{
			printf("相邻地点:%s\n",G->vertex[p->adjvex].name);
			printf("距离:%dm\n",p->weight);
			p=p->next;

		}
		printf("\n");
		
	}
		
}

void add_choice(AdjList *G)
{
	int i;
	
	printf("\n1.添加一个地点:");
	printf("\n2.添加一条边:\n");
	printf("0.返回\n");
	printf("请选择:");
	scanf("%d",&i);
	switch(i)
	{
		case 1:add_school(G);break;
		case 2:add_arc(G);break;
		case 0:return;
	}
	
}

void add_arc(AdjList *G)
{
	int i,j,k,weight,start,end,num;
	ArcNode *p;
	char name[20];

	while(1)
	{
		printf("请输入起点名称:");
		scanf("%s",name);
		start = find_name(G,name);
		if(start != -1)
			break;
		else
			printf("此点不存在!\n");
	}
	while(1)
	{
		printf("请输入终点名称:");
		scanf("%s",name);
		end = find_name(G,name);
		if(end != -1)
			break;
		else
			printf("此点不存在!\n");
	}

	printf("请输入距离:");
	scanf("%d",&weight);

	p=(ArcNode *)malloc(sizeof(ArcNode));
	p->adjvex=end;
	p->weight=weight;
	p->next=G->vertex[start].head;
	G->vertex[start].head=p;

	p=(ArcNode *)malloc(sizeof(ArcNode));
	p->adjvex = start;
	p->weight=weight;
	p->next=G->vertex[end].head;
	G->vertex[end].head=p;

	G->arcnum++;

}
void add_school(AdjList *G)	//增加地点
{
	int i,j,k,weight,key,num;
	ArcNode *p;
	char city[20];

	if(G->vexnum >=MAXVEX)
	{
		printf("学校占地面积的使用已达上限!");
		return;
	}
	
	printf("请输入地点名称:");
	scanf("%s",city);
	key =find_name(G,city);
	if(key != -1)
	{
		printf("地点已存在!\n");
		return ;
	}
	key = find_vex(G);
	G->vertex[key].used=1;
	G->vertex[key].head=NULL;

	strcpy(G->vertex[key].name,city);
	printf("请输入地点信息:");
	scanf("%s",G->vertex[key].vex_information);
	printf("请输入相邻的边数:");
	scanf("%d",&num);
	for(i=1;i<=num;i++)
	{
		printf("请输入相邻的地点:");
		scanf("%s",city);
		j = find_name(G,city);
		if(j == -1)
		{
			printf("不存在该地点!\n");
			i = i-1;		
			continue;
		}
		printf("请输入距离:");
		scanf("%d",&weight);

		p = (ArcNode *)malloc(sizeof(ArcNode));
		p->adjvex = j;
		p->weight = weight;
		p->next = G->vertex[key].head;
		G->vertex[key].head = p;

		p = (ArcNode *)malloc(sizeof(ArcNode));
		p->adjvex = key;
		p->weight = weight;
		p->next = G->vertex[j].head;
		G->vertex[j].head = p;
		
	}

	G->vexnum++;
	G->arcnum += num;
	
}

void del_choice(AdjList *G)
{
	int i;
	
	printf("\n1.删除一个地点:");
	printf("\n2.删除一条边:\n");
	printf("0.返回\n");
	printf("请选择:");
	scanf("%d",&i);
	switch(i)
	{
		case 1:del_school(G);break;
		case 2:del_arc(G);break;
		case 0:return;
	}
	

	
}

void del_arc(AdjList *G)
{

	int i,j,k,weight,start,end,num;
	ArcNode *p,*r,*s;
	char name[20];

	while(1)
	{
		printf("请输入起点名称:");
		scanf("%s",name);
		start = find_name(G,name);
		if(start != -1)
			break;
		else
			printf("此点不存在!\n");
	}
	while(1)
	{
		printf("请输入终点名称:");
		scanf("%s",name);
		end = find_name(G,name);
		if(end != -1)
			break;
		else
			printf("此点不存在!\n");
	}
	
	p=G->vertex[start].head;
	if(p->adjvex == end)
		p=p->next;
	else
	{
		r=p;
		p=p->next;
		while(p)
		{
			if(p->adjvex == end)
				r->next=p->next;
			else
			{
				r=p;
				p=p->next;
			}

		}
	}
	p=G->vertex[end].head;
	if(p->adjvex == start)
	{
		p=p->next;
		G->arcnum--;
		return;
	}
	else
	{
		r=p;
		p=p->next;
		while(p)
		{
			if(p->adjvex == start)
			{
				r->next=p->next;
				G->arcnum--;
			}
			else
			{
				r=p;
				p=p->next;
			}

		}
	}
}

void del_school(AdjList *G)
{

	int i,j,num,key;
	ArcNode *p,*r,*s;
	char city[20];

	printf("要删除的地点:");
	scanf("%s",city);
	num = find_name(G,city);
	if(num == -1)
	{
		printf("地点不存在!\n");
		return ;
	}
	p=G->vertex[num].head;
	while(p)
	{
		key = p->adjvex;
		r = G->vertex[key].head;
		if(r->adjvex == num)
			r=r->next;
		s=r;
		r=r->next;
		while(r)
		{
			if(r->adjvex == num)
			{
				s->next=r->next;
				G->arcnum--;
				break;
			}
			else
			{
				s=r;
				r=r->next;
			}
		}
		p=p->next;
	}
	
	G->vertex[num].used=0;
	G->vertex[num].head=NULL;
}

void trans(AdjList *G)
{
	int i,j,k;
	ArcNode *p;

	for(i=1;i<=MAXVEX;i++)
		for(j=1;j<=MAXVEX;j++)
			graph[i][j]=INFINITY;

	for(i=1;i<=G->vexnum;i++)
	{
		p=G->vertex[i].head;
		while(p)
		{
			graph[i][p->adjvex]=p->weight;
			graph[p->adjvex][i]=p->weight;
			p=p->next;
		}
	}
	
	
}

void mini_path(AdjList *G)
{
	char city[20];
	int start;
	while(1)
	{
		printf("请输入起点:");
		scanf("%s",city);
		start = find_name(G,city);
		if(start != -1)
			break;
		
		else
			printf("无此节点!\n");
	}
	Prim(G,start);
}

void Prim(AdjList *G,int start)
{
	struct
	{
		int adjvex;
		int lowcost;
	}closedge[MAXVEX];
	int i,e,k,m,min;
	closedge[start].lowcost=0;
	
	trans(G);
	for(i=1;i<=G->vexnum;i++)
		if(i != start)
		{
			closedge[i].adjvex=start;
			closedge[i].lowcost=graph[start][i];	
		}
	for(e=1;e<=G->vexnum;e++)
	{
		min=INFINITY;
		for(k=1;k<=G->vexnum;k++)
		{
			if(closedge[k].lowcost!=0 && closedge[k].lowcost<min)
			{
				m=k;min=closedge[k].lowcost;
			}
		}
		if(closedge[m].lowcost ==0)
		continue;
		printf("从%s-->%s:%dm\n",G->vertex[closedge[m].adjvex].name,G->vertex[m].name,closedge[m].lowcost);
		closedge[m].lowcost=0;

		for(i=1;i<=G->vexnum;i++)
			if(i!=m &&graph[m][i]<closedge[i].lowcost)
			{
				closedge[i].lowcost=graph[m][i];
				closedge[i].adjvex=m;
			}
	}
}

void display_choice(AdjList *G)
{
	int i;
	
	printf("\n1.查询学校各点的信息:");
	printf("\n2.查询学校某点的信息:\n");
	printf("请选择:");
	scanf("%d",&i);
	switch(i)
	{
		case 1:display_school(G);break;
		case 2:display_vex(G);break;
		case 0:return;
	}
	
}

void display_vex(AdjList *G)
{

	char city[20];
	int num;
	ArcNode *p;

	while(1)
	{
		printf("请输入要查看的地点名称:");
		scanf("%s",city);
		num = find_name(G,city);
		if(num !=-1)
			break;
		else
			printf("无此节点!\n");
	}
	
	printf("地点名称:%s\n",G->vertex[num].name);
	printf("地点信息:%s\n",G->vertex[num].vex_information);

	p=G->vertex[num].head;
	while(p)
	{
		printf("相邻地点名称:%s\n",G->vertex[p->adjvex].name);
		printf("相邻地点距离:%dm\n",p->weight);
		p=p->next;
		
	}
}

LQueue *Init_LQueue()
{
	LQueue *q;
	Qnode *p;
	q = (LQueue *)malloc(sizeof(LQueue));
	p = (Qnode *)malloc(sizeof(Qnode));

	q->front = q->rear;
	q->rear = q->front;

	return q;
}

int In_Queue(LQueue *head,int i)
{
	Qnode *p;
	p = (Qnode *)malloc(sizeof(Qnode));

	p->data = i;
	p->next = NULL;
	head->rear->next=p;
	head->rear=p;

	return 1;	
}

int Out_Queue(LQueue *head,int *i)
{
	Qnode *p;

	if(Empty_Queue(head))
	{
		printf("队空!");
		return 0;
	}

	p=head->front->next;
	head->front->next=p->next;
	*i=p->data;

	if(head->rear == p)
		head->rear=head->front;
	free(p);

	return 1;
}

int Empty_Queue(LQueue *head)
{
	if(head->front == head->rear)
		return 1;
	else 
		return 0;
}

void travel_graph(AdjList *G)
{
	char name[20];
	int start,end,i,num;
	
	while(1)
	{
		printf("请输入起点名称:");
		scanf("%s",name);
		start = find_name(G,name);
		if(num != -1)
			break;
		else
			printf("此点不存在!");
	}
	while(1)
	{
		printf("请输入终点名称:");
		scanf("%s",name);
		end = find_name(G,name);
		if(end != -1)
			break;
		else
			printf("此点不存在!");
	}

	BFS(G,start,end);
}

void BFS(AdjList *G,int start,int end)
{

	int i,num,j,k=1,count=0,length=0;
	ArcNode *p;
	LQueue *head;
	int path[MAXVEX]={0};
	int visited[MAXVEX]={0};

	head = Init_LQueue();

	for(i=1;i<=G->vexnum;i++)
		visited[i]=0;
	for(i=1;i<=G->vexnum;i++)
		check[i]=0;

	if(!visited[start])
	{

		visited[start]=1;
		In_Queue(head,start);
		while(!Empty_Queue(head))
		{
			Out_Queue(head,&num);
			visited[num]=1;
			p =G->vertex[num].head;
			while(p)
			{
				if(!visited[p->adjvex])
				{
					if(check[p->adjvex] == 0 )
						check[p->adjvex]=num;
					In_Queue(head,p->adjvex);
				}
				p=p->next;
			}
		}
	}

	count=0;
	i = end;
	k=1;

	while(i != start)
	{
		path[k] = check[i];
		j=check[i];
		i=j;
		++count;
		k++;
	}

	i = end;
	while(check[i] != 0)
	{
		num=find_weight(G,i,check[i]);
		length += num;
		j = check[i];
		i = j;
	}	
	
	for(i=1;i<=count;i++)
		printf("%4d",path[i]);
	printf("%s--->%s的中转次数最少的路线为:从%s",G->vertex[start].name,G->vertex[end].name,G->vertex[start].name);
		for(j = count-1;j >= 1;j--)
		{
			printf("->%s",G->vertex[path[j]].name);

		}
	printf("->%s,距离为%dm\n",G->vertex[end].name,length);

}

int find_weight(AdjList *G,int start,int end)
{
	ArcNode *p;
	
	p = G->vertex[start].head;

	while(p)
	{
		if(p->adjvex == end)
			return p->weight;
		else 
			p=p->next;
	}
	
}


//求start到end的所有简单路径，length表示当前路径长度
 

int find_path(AdjList *G, int start, int end, int length, int path[], int visited[])
{
	//加入当前路径中
	path[length] = start;
	visited[start] = 1;
	int num = 0;
	int i = 0;
	int weight=0;
	ArcNode *edge;
	//找到了一条简单路径
	if (start == end)
	{
		printf("\npath:");
		for (i = 0; path[i]; i++)
		{
			printf(" %s ", G->vertex[path[i]].name);
			if (path[i + 1])
			{
				printf("->");
			}
		}
		for(i=0;path[i];i++)
		{
			weight += find_weight(G,path[i],path[i+1]);
		}
		printf("  %dm",weight);
	}
	else
	{
		for (edge = G->vertex[start].head; edge; edge = edge->next)
		{
			num = edge->adjvex;
			if (!visited[num]) 
			{
				find_path(G, num, end, length + 1, path, visited);
			}
		}
	}

	visited[start] = 0;
	path[length] = 0; //回溯

	return 0;
}


//给出任意两个景点间的所有简单路径
 

int find_all_path(AdjList *G)
{
	char choicejudge;
	char startplace[20];
	char endplace[20];
	int a_judge = 0;
	int b_judge = 0;
	int flag = 0;
	int i = 0;
	int path[MAXVEX];
	int visited[MAXVEX];

	//初始化path和visited数组
	for (i = 0; i<= MAXVEX; i++)
	{
		path[i] = 0;
		visited[i] = 0;
	}

	//输入出发点和终点
	while(1)
	{
		printf("请输入出发点: ");
		scanf("%s", startplace);
		if( (a_judge = find_name(G,startplace)) != -1)
			break;
		else
			printf("输入有误!\n");
	}
	while(1)
	{
		printf("请输入终点: ");
		scanf("%s", endplace);
		if( (b_judge = find_name(G,endplace)) != -1)
			break;
		else
			printf("输入有误!\n");
	}
	if (!strcmp(startplace, endplace))
	{
		printf("终点和起点相同，查实后再输入！\n");
		return ;
	}
	//求从出发点到终点的所有简单路径
	find_path(G, a_judge, b_judge, 0, path, visited);
	printf("\n");
	return 0;
}


