#include "dazzle.h"
int main()
{
	AdjList *G;
	int i=0;
	pid_t pid;
	
	pid=fork();
	if(pid > 0)
	{
		do
		{
			printf("-------西邮路线查询系统--------\n");
			printf("1.显示学校信息\n");
			printf("2.从某点出发到某点的所有简单路径\n");
			printf("3.从某点出发到某点的最短路线\n");
			printf("4.中转次数最少的路线\n");
			printf("5.最佳学校地图\n");
			printf("6.添加新地点/路线\n");
			printf("7.删除旧地点/路线\n");
			printf("8.创建地图\n");
			printf("0.退出\n");
			printf("请选择:");
			scanf("%d",&i);
			switch(i)
			{
				case 1:G=out_file();display_choice(G);break;
				case 2:G=out_file();find_all_path(G);break;
				case 3:G=out_file();search_school(G);break;
				case 4:G=out_file();travel_graph(G);break;
				case 5:G=out_file();mini_path(G);break;
				case 6:G=out_file();add_choice(G);in_file(G);break;
				case 7:G=out_file();del_choice(G);in_file(G);break;
				case 8:G=Create();break;
				case 0:return 0;
			}

		}while(1);
	}

	else if(pid == 0)
	{
		system("xdg-open /home/dazzle/jsp/MainSchool.jpg");
	}
}

