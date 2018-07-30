#include "spinlock.h"

void spinlock_lock(spinlock_t *spin){
	while(__sync_lock_test_and_set(spin, 1) == 1);
}

void spinlock_unlock(spinlock_t *spin){
	__sync_lock_release(spin);
}
