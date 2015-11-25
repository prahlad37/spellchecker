
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"bktree.h"

struct bknode root;

#define ALLOW_DISTANCE	(1)


#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein(char *s1, char *s2) {
        unsigned int x, y, s1len, s2len;
        s1len = strlen(s1);
 	s2len = strlen(s2);
        unsigned int matrix[s2len+1][s1len+1];
        matrix[0][0] = 0;
  
        for (x = 1; x <= s2len; x++){
                matrix[x][0] = matrix[x-1][0] + 1;
        }
        for (y = 1; y <= s1len; y++){
                matrix[0][y] = matrix[0][y-1] + 1;
        }

        for (x = 1; x <= s2len; x++){
                for (y = 1; y <= s1len; y++){
                        matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));
                }
        }

        return(matrix[s2len][s1len]);
}
                    

int add_node(struct bknode *node,struct bknode *parent)
{	
	int lev_dis;
	struct bknode *temp;

	lev_dis = levenshtein(parent->key_string,node->key_string);
	//printf("%s:%d parent %s node %s lev %d\n",__func__,__LINE__,parent->key_string,node->key_string,lev_dis);	
	TAILQ_FOREACH(temp,&parent->child,next)
	{
	//	printf("%s:%d elm %s\n",__func__,__LINE__,temp->key_string);
		if(lev_dis == temp->lev_parent)
		{
			/*recursive call,return;*/
	//		printf("%s:%d rec node %s parent %s\n",__func__,__LINE__,node->key_string,temp->key_string);
			add_node(node,temp);
			return 0;
		}
	}
	/*add node to the parent*/
	TAILQ_INSERT_HEAD(&parent->child,node,next);
	node->node_level = parent->node_level + 1;
	parent->no_of_child++;
	node->lev_parent = lev_dis;
	//printf("%s:%d added %s(%d) at parent %s\n",__func__,__LINE__,node->key_string,node->lev_parent,parent->key_string);
	return 0;
}

int bk_init(void)
{
	BK_NODE_INIT(&root);
	strcpy(root.key_string,"bknode");
	root.node_level = 0;
	return 0;
}


int bk_add(char *key)
{
	struct bknode *node;
	int ret;

	if(key == NULL)
	{
		printf("%s:%d Received NULL pointer\n",__func__,__LINE__);
		return -1;
	}
	
	if((node = malloc(sizeof(*node))) == NULL)
	{
		printf("%s:%d malloc failure\n",__func__,__LINE__);
		return -1;
	}

	BK_NODE_INIT(node);
	strcpy(node->key_string,key);
//	printf("%s:%d adding node %s\n",__func__,__LINE__,node->key_string);

	ret = add_node(node,&root);


	return 0;
}

void print_space(int num)
{
	while(--num){
		printf(" ");
	}
}

int pr_tree_node(struct bknode *node,char* repeat_str)
{
	struct bknode *temp;
	int node_level,no_of_child,child_count=0,len;
	char temp_string[256];

	node_level = node->node_level;
	no_of_child = node->no_of_child;
	len = strlen(node->key_string);
	printf("%s",node->key_string);
	if(no_of_child == 0){
		printf("\n");
		return 0;
	}

	TAILQ_FOREACH(temp,&node->child,next)
	{
		len = strlen(temp->key_string);
//		printf("key %s len %d\n",temp->key_string,len);
		if(child_count == 0 && no_of_child == 1){
			printf("%s",PR_H);
			sprintf(temp_string,"%s%*c",repeat_str,(int)strlen(temp->key_string)+3,' ');
		}
		else if(child_count == 0 && no_of_child > 1){
			printf("%s",PR_H_BR);
			sprintf(temp_string,"%s%s%*c",repeat_str,PR_V,(int)strlen(temp->key_string),' ');
		}
		else if(no_of_child > 1 && child_count == no_of_child -1){
			printf("%s%s",repeat_str,PR_E);
			sprintf(temp_string,"%s%*c",repeat_str,(int)strlen(temp->key_string)+3,' ');
		}
		else{
			printf("%s%s",repeat_str,PR_V_BR);
			sprintf(temp_string,"%s%s%*c",repeat_str,PR_V,(int)strlen(temp->key_string),' ');
		}
		
		/*recursive call*/
		pr_tree_node(temp,temp_string);
		child_count++;
	}
	
	return 0;
}

int bk_print_tree(void)
{
	char line[256];
	memset(line,0,256);

	printf("no_of_child %d\n",root.no_of_child);
	if(root.no_of_child > 1)
	{
		sprintf(line,"%*c",(int)strlen(root.key_string),' ');
	}	
	
	pr_tree_node(&root,line);

	return 0;	
}

int list_insert_sorted(struct bknode *list,struct bknode *node)
{
	struct bknode *temp;
	int lev_parent;
	lev_parent = node->lev_parent;
	
//	printf("%s:%d key %s\n",__func__,__LINE__,node->key_string);

	if(TAILQ_EMPTY(&list->child))
	{
//		printf("%s:%d empty list %s\n",__func__,__LINE__,node->key_string);
		TAILQ_INSERT_HEAD(&list->child,node,next);
		return 0;
	}
	TAILQ_FOREACH(temp,&list->child,next)
	{
//		printf("%s:%d list elm %s(%d)\n",__func__,__LINE__,temp->key_string,temp->lev_parent);
		if(temp->lev_parent > lev_parent)
		{
//			printf("%s:%d insert before %s\n",__func__,__LINE__,temp->key_string);
			TAILQ_INSERT_BEFORE(temp,node,next);
			return 0;
		}
	}	

//	printf("%s:%d insert at tail\n",__func__,__LINE__);
	TAILQ_INSERT_TAIL(&list->child,node,next);
	return 0;
}

int search_count;	/*FIXME*/


int bk_search(char *search_str,struct bknode *parent,int allow_lev,struct bknode *list)
{
	struct bknode *temp;
	int lev_parent,ret=-1,lev_min,lev_max;

//	printf("Started Search for key string '%s'\n",search_str);

	search_count++;
	lev_parent = levenshtein(search_str,parent->key_string);
	if(lev_parent <= allow_lev){
		printf("********* %s(%d) ********\n",parent->key_string,lev_parent);
		temp = malloc(sizeof(*temp));
		strcpy(temp->key_string,parent->key_string);
		temp->lev_parent = lev_parent;
		list->no_of_child++;
		list_insert_sorted(list,temp);
		//LIST_INSERT_HEAD(&list->child,temp,next);
		return 0;
	}

	lev_max = lev_parent + allow_lev;
	lev_min = lev_parent - allow_lev;
	(lev_min < 0)? (lev_min = 0) : (1);
//	printf("search in parent %s(%d) for child with lev [%d,%d]\n",parent->key_string,lev_parent,lev_min,lev_max);

	TAILQ_FOREACH(temp,&parent->child,next)
	{
//		printf("check the child '%s(%d)' [%d,%d]\n",temp->key_string,temp->lev_parent,lev_min,lev_max);
		if(temp->lev_parent <= lev_max && temp->lev_parent >= lev_min)
		{
//			printf("child within allow lev %s(%d)\n",temp->key_string,temp->lev_parent);
			ret = bk_search(search_str,temp,allow_lev,list);
		}
	}
//	printf("return of %s ret %d\n",parent->key_string,ret);
	return ret;
}


int bk_search_tree(char *search_str,struct bknode *list)
{
	int ret;
	BK_NODE_INIT(list);
	
	search_count = 0;

	ret = bk_search(search_str,&root,ALLOW_DISTANCE,list);
	
	printf("words checked::: %d\n",search_count);
	return ret;
}






