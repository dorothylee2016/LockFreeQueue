#ifndef SPINLOCK_H
#define SPINLOCK_H

typedef int spinlock_t;

void spinlock_lock(spinlock_t *spin);
void spinlock_unlock(spinlock_t *spin);

#endif /* SPINLOCK_H */
