#include<stdio.h>
#include<string.h>

#include"bktree.h"

#define LINE_SIZE	(256)


int main(int argc, char *argv[]){
	
	struct bknode *new_node;
	int ret,i;
	FILE *fp;
	char line[LINE_SIZE],*pos;

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

	bk_print_tree();

	/*now searching can be done*/
	
	return 0;
}
