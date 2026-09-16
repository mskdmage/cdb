#include <string.h>
#include "kv.h"

#define TOMBSTONE 0x1

size_t hash(char *val, int capacity) {
	
	size_t hash = 0x13371337abc123;

	while(*val) {
	
		hash ^= *val;
		hash = hash << 8;
		hash += *val;

		val++;
	}

	return hash % capacity;

}

// fn kv_put
// params:
// 	- db: a pointer to a database of type kv_t
// 	- key: a pointer to the key
// 	- value: a pointer to the value
// returns:
// 	the index of the key, on error -1, on not found -2
int kv_put(kv_t *db, char *key, char *value) {
	
	if (!db || !key || !value) {
		return -1;
	}

	size_t idx = hash(key, db->capacity);

	for (int i=0; i < db->capacity - 1; i++) {
		
		size_t real_idx = (idx + i) % db->capacity;

		kv_entry_t *selected_entry = &(db->entries[real_idx]);

		// The key already exists, verify entry key matches, update value.
		if (selected_entry->key && selected_entry->key != (void*)TOMBSTONE && !strcmp(selected_entry->key, key)) {
			
			char *newval = strdup(value);
			
			if (!newval) {
				return -1;
			}
			
			free(selected_entry->value);

			selected_entry->value = newval;
			return 0;

		}

		// Land in empty slot
		// NULL or TOMBSTONE
		if (!selected_entry->key || selected_entry->key == (void*)TOMBSTONE) {
			
			char *newval = strdup(value);
			char *newkey = strdup(key);

			if (!newval || !newkey) {
				free(newkey);
				free(newval);
				return -1;
			}

			selected_entry->key = newkey;
			selected_entry->value = newval;
			db->count++;
			return 0;
		}


	}

	return -2;
}

// fn kv_get
// params:
// 	- db: a pointer to a database of type kv_t
// 	- key: a pointer to the key
// returns:
// 	a pointer to the key, NULL on failure to get
char *kv_get(kv_t *db, char *key) {
	
	if (!db || !key ) {
		return NULL;
	}

	size_t idx = hash(key, db->capacity);

	for (int i=0; i < db->capacity - 1; i++) {
		
		size_t real_idx = (idx + i) % db->capacity;

		kv_entry_t *selected_entry = &(db->entries[real_idx]);

		if (selected_entry->key == NULL) {
			return NULL;
		}

		if (selected_entry->key && selected_entry->key != (void*)TOMBSTONE && !strcmp(selected_entry->key, key)) {
			return selected_entry->value;
		}
	}

	return NULL;
}

// fn kv_delete
// params:
// 	- db: a pointer to a database of type kv_t
// 	- key: a pointer to the key
// returns:
// 	the pointer to the dleeted index, on not found -1
int kv_delete(kv_t *db, char *key) {
	
	if (!db || !key) {
		return -1;
	}
	
	size_t idx = hash(key, db->capacity);

	for (int i=0; i < db->capacity - 1; i++) {
		
		size_t real_idx = (idx + i) % db->capacity;

		kv_entry_t *selected_entry = &(db->entries[real_idx]);

		if (selected_entry->key == NULL) {
			return -1;
		}

		if (selected_entry->key && selected_entry->key != (void*)TOMBSTONE && !strcmp(selected_entry->key, key)) {
			free(selected_entry->key);
			free(selected_entry->value);
			db->count--;
			selected_entry->key = (void*)TOMBSTONE;
			selected_entry->value = NULL;
			return real_idx;
		}
	}

	return -1;

}

// fn kv_free
// params:
// 	- db: a pointer to a database of type kv_t
// returns:
// 	0 on success, -1 on failure
int kv_free(kv_t *db) {
	if (!db) {
		return -1;
	}

	for (int i=0; i < db->capacity -1; i++) {
		
		kv_entry_t *selected_entry = &(db->entries[i]);

		if (selected_entry->key != NULL && selected_entry->key != (void*)TOMBSTONE) {
			free(selected_entry->key);
			free(selected_entry->value);
			selected_entry->key = NULL;
			selected_entry->value = NULL;
			db->count--;
		}
	}

	free(db->entries);
	db->entries = NULL;
	free(db);

	return 0;
}

kv_t *kv_init(size_t capacity) {

	if (capacity == 0) {
		return NULL;
	}

	kv_t *table = malloc(sizeof(kv_t));

	if (table == NULL) {
		return NULL;
	}

	table->capacity = capacity;
	table->count = 0;
	
	table->entries = calloc(sizeof(kv_entry_t), capacity);

	if (table->entries == NULL) {
		return NULL;
	}

	return table;

}
