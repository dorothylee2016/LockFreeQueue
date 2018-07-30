#include "queue.h"
#include "spinlock.h"
#include <stdlib.h>

static struct node *head = NULL;
static struct node *tail = NULL;
static spinlock_t lock = 0;

int queue_add(void *value){
	struct node *new = malloc(sizeof(struct node*));
	if(new == NULL){
		return -1;
	}

	new->value = value;
	new->next = NULL;

	spinlock_lock(&lock);

	if(head == NULL){
		head = new;
		tail = new;
	} else {
		struct node *tmp = tail;
		tail = new;
		tmp->next = new;
	}

	spinlock_unlock(&lock);
	return 0;
}

void *queue_remove(){
	void * retval;

	spinlock_lock(&lock);
	if(head == NULL){
		spinlock_unlock(&lock);
		return NULL;
	} else {
		struct node *tmp = head;
		retval = head->value;
		head = head->next;
		free(tmp);
	}
	spinlock_unlock(&lock);
	return retval;
}
