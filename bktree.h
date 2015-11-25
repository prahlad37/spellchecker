
#ifndef BKTREE_H
#define BKTREE_H

#include<sys/queue.h>


#define KEY_STRING_SIZE_MAX	(100)

struct bknode {
	int node_level;
	int no_of_child;
	int lev_parent;
	char key_string[KEY_STRING_SIZE_MAX];
	TAILQ_ENTRY(bknode)	next;
	TAILQ_HEAD(child,bknode) child;
};

#define BK_NODE_INIT(node) do {						\
	(node)->node_level = -1;					\
	(node)->no_of_child = 0;					\
	(node)->lev_parent = 0;						\
	TAILQ_INIT(&(node)->child);					\
} while(0)

#define PR_H_BR		"─┬─"
#define PR_H		"───"
#define PR_V_BR		" ├─"
#define PR_V   		" │ "
#define PR_E   	 	" └─"


int bk_init(void);
int bk_add(char *key);
int bk_print_tree(void);
int bk_search_tree(char *search_str,struct bknode *list);
#endif
