#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "hashtable.h"

/**************** global variables ****************/

/**************** local types ****************/
//we do not need local types

/**************** global types ****************/
typedef struct hashtable {
  int num_slots;
  struct list* slots[];  //a pointer to an array of lists
} hashtable_t;//for opaque use


/**************** local functions ****************/
/* not visible outside this file */
unsigned long JenkinsHash(const char *str, unsigned long mod);


hashtable_t *hashtable_new(const int num_slots){
	hashtable_t *hashtable = malloc(sizeof(hashtable_t));
	if (hashtable == NULL) {
    return NULL; // error allocating list
  } else {
    hashtable->slots[num_slots] = calloc(num_slots,sizeof(struct list*));
    hashtable->num_slots = num_slots;
    return hashtable;
  }
}

bool hashtable_insert(hashtable_t *ht, char *key, void *data){
	if(ht == NULL){//th hashtable was non-existant
		return false;
	}
	int hashValue = JenkinsHash(key, ht->num_slots);
	if(ht->slots[hashValue] == NULL){//that slot int he hashtable is empty 
		ht->slots[hashValue] = list_new();
		return list_insert(ht->slots[hashValue], key, data);//dont need to stringcopy because list insert does

	} else {
		return list_insert(ht->slots[hashValue], key, data);
	}
}

void *hashtable_find(hashtable_t *ht, char *key){
	if(ht == NULL){//the hast tabel did not exist
		return NULL;
	}
	int hashValue = JenkinsHash(key, ht->num_slots);
	if(ht->slots[hashValue] == NULL){//no way it could be found if there is no list
		return NULL;
	} else {
		return list_find(ht->slots[hashValue], key);
	}
}

// JenkinsHash - see header file for usage
unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    if (str == NULL)
      return 0;

    size_t len = strlen(str);
    unsigned long hash, i;

    for (hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

