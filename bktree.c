
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"bktree.h"

struct bknode root;

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
	LIST_FOREACH(temp,&parent->child,next)
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
	//printf("%s:%d added %s at parent %s\n",__func__,__LINE__,node->key_string,parent->key_string);
	/*add node to the parent*/
	LIST_INSERT_HEAD(&parent->child,node,next);
	node->node_level = parent->node_level + 1;
	parent->no_of_child++;
	node->lev_parent = lev_dis;
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
	printf("%s:%d adding node %s\n",__func__,__LINE__,node->key_string);

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

	LIST_FOREACH(temp,&node->child,next)
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
