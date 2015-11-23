#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"bktree.h"

#define LINE_SIZE	(256)


int main(int argc, char *argv[]){
	
	struct bknode *new_node,ret_list,*temp;
	int ret,i,choice;
	FILE *fp;
	char line[LINE_SIZE],*pos,search[LINE_SIZE];

	if(argc != 2){
		printf("usage: %s <input file>\n",argv[0]);
		return -1;
	}

	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		printf("Unable to open '%s'\n",argv[1]);
		return -1;
	}
	
	ret = bk_init();
	if(ret < 0)
	{
		printf("Initialisation Failed\n");
		return -1;
	}

	/*read the data file line by line*/
	while(fgets(line,sizeof(line),fp))
	{
//		printf("%s:%d %s\n",__func__,__LINE__,line);
		pos = strchr(line,'\n');
		*pos = '\0';
		bk_add(line);
	}


	/*now searching can be done*/
	while(1){
		printf("1.search\n2.print\n0.exit\n");
		scanf("%d",&choice);
		printf("choice %d\n",choice);
		switch(choice){
			case 1:printf("enter the kry string::\n");
				getchar();
			       fflush(stdin);
			       fgets(search,LINE_SIZE,stdin);
			       pos = strchr(search,'\n');
			       *pos = '\0';
			       ret = bk_search_tree(search,&ret_list);  
			       printf("search ret %d\n\n\n",ret);
			       LIST_FOREACH(temp,&ret_list.child,next)
			       {
				       printf("::::::::\t%s\t ::::::::\n",temp->key_string);
				       LIST_REMOVE(temp, next);
				       free(temp);
			       }
			       printf("\n\n");
			       break;
			case 2:
			       bk_print_tree();
			       break;
			case 0:
			       exit(0);
			       break;
			default:
			       break;
		}
	}

	return 0;
}
