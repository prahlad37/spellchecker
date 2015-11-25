#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"bktree.h"

#define LINE_SIZE	(256)

struct timespec diff_time(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}


int main(int argc, char *argv[]){
	
	struct bknode *new_node,ret_list,*temp;
	int ret,i,choice,wc=0;
	FILE *fp;
	char line[LINE_SIZE],*pos,search[LINE_SIZE];
	struct timespec prev,now,diff;

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

	clock_gettime(CLOCK_MONOTONIC,&prev);
	/*read the data file line by line*/
	while(fgets(line,sizeof(line),fp))
	{
//		printf("%s:%d %s\n",__func__,__LINE__,line);
		pos = strchr(line,'\n');
		*pos = '\0';
		bk_add(line);
		wc++;
	}
	clock_gettime(CLOCK_MONOTONIC,&now);
	diff = diff_time(prev,now);
	printf("loaded %d words in %lu sec %lu ns\n\n",wc,diff.tv_sec,diff.tv_nsec);


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
			       clock_gettime(CLOCK_MONOTONIC,&prev);
			       ret = bk_search_tree(search,&ret_list);  
			       clock_gettime(CLOCK_MONOTONIC,&now);
			       printf("search ret %d\n\n\n",ret);
			       diff = diff_time(prev,now);
			       printf("search time %lu sec %lu ns\n\n",diff.tv_sec,diff.tv_nsec);
			       TAILQ_FOREACH(temp,&ret_list.child,next)
			       {
				       printf("::::::::\t%s(%d)\t ::::::::\n",temp->key_string,temp->lev_parent);
				       TAILQ_REMOVE(&ret_list.child,temp, next);
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
