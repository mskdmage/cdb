#include <stdio.h>
#include "kv.h"

int main(void) {
	
	kv_t *table = kv_init(3);
	
	if (table == NULL) {
		printf("%s\n", "Something terrible happened!");
		return -1;
	}

	printf("Address:	%p\n", table);
	printf("Capacity:	%d\n", (int)table->capacity);
	printf("Entries:	%d\n", (int)table->count);
	
	free(table->entries);
	table->entries = NULL;

	free(table);
	table = NULL;

	return 0;
}
