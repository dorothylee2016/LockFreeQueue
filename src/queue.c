#include "queue.h"
#include <stdlib.h>
#include <stdint.h>

static volatile struct node *head = NULL;
static volatile struct node *tail = NULL;

static int mark_node(struct node *node){
	uint64_t next = 0;
	uint64_t next_marked = 0;

	__sync_bool_compare_and_swap(&next, 0, (uint64_t) node->next);	
	next_marked = next | 0x1;

	return __sync_bool_compare_and_swap(&node->next, next, next_marked);	
}

static uint64_t unmark_next(uint64_t node_next){
	uint64_t next = 0;

	__sync_bool_compare_and_swap(&next, 0, node_next);	
	next = next & ~((uint64_t) 0x1);

	return next;
}

int queue_add(void *value){
	struct node *new = NULL;
	struct node *tmp = NULL;
	uint64_t tmp_next = 0;
	int retval = 0;
	
	new = malloc(sizeof(struct node));
	if(!new){
		return -1;
	}
	new->value = value;
	new->next = NULL;

	while(!retval){
		tmp = NULL;
		tmp_next = 0;

		__sync_bool_compare_and_swap(&tmp, NULL, tail);
		if(tmp == NULL){
			retval = __sync_bool_compare_and_swap(&tail, NULL, new);

			if(retval == 0){
				continue;
			}

			__sync_bool_compare_and_swap(&head, NULL, new);
			return 0;
		}

		__sync_bool_compare_and_swap(&tmp, NULL, tail);
		__sync_bool_compare_and_swap(&tmp_next, 0, (uint64_t) tmp->next);
		retval = __sync_bool_compare_and_swap(&tail, tmp, new);
		if(retval == 0){
			continue;
		}
		retval = __sync_bool_compare_and_swap(&tmp->next, (struct node *) unmark_next(tmp_next), new);
		if(retval == 0){
			__sync_bool_compare_and_swap(&head, head, new);
		}
		
		retval = 1;

	}

	return 0;
}


void *queue_delete(struct node *head){
	struct node *tmp = NULL;
	uint64_t tmp_next = 0;
	int retval = 0;
	void * ret;

	while(!retval){
		tmp = NULL;
		tmp_next = 0;

		__sync_bool_compare_and_swap(&tmp, NULL, head);
		if(tmp == NULL){
			return NULL;
		}

		__sync_bool_compare_and_swap(&tmp_next, 0, tmp->next);
		mark_node(tmp);

		retval = __sync_bool_compare_and_swap(&head, tmp, (struct node *) tmp_next);
		if(retval == 0){
			continue;
		}

		ret = tmp->value;
	}

	return ret; 

}
