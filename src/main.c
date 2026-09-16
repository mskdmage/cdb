#include <stdio.h>
#include "kv.h"

int main(void) {
	
	kv_t *table = kv_init(20);
	
	if (table == NULL) {
		printf("%s\n", "Something terrible happened!");
		return -1;
	}

	printf("Address:	%p\n", table);
	printf("Capacity:	%d\n", (int)table->capacity);
	printf("Entries:	%d\n", (int)table->count);
	
	kv_put(table, "Hear", "no evil");
	kv_put(table, "See", "no evil");
	kv_put(table, "Speak", "no evil");

	for (int i=0; i < table->capacity - 1; i++) {
		if (table->entries[i].key) {
			printf("%d	| %s	| %s\n", i, table->entries[i].key, table->entries[i].value);
		}
	}

	printf("Looking for %s -> found: %s\n", "See", kv_get(table, "See"));
	printf("Looking for %s -> found: %s\n", "Monkey", kv_get(table, "Monkey"));

	kv_delete(table, "See");

	printf("Looking for %s -> found: %s\n", "See", kv_get(table, "See"));

	free(table->entries);
	table->entries = NULL;

	free(table);
	table = NULL;

	return 0;
}
