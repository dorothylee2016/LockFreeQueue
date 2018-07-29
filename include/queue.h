#ifndef QUEUE_H
#define QUEUE_H

struct node {
	struct node *next;
	void *value;
} __attribute__((aligned(2)));

int queue_add(void *value);
void *queue_remove();

#endif /* QUEUE_H */
