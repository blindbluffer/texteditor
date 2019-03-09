#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>


struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;



hashtable_t *ht_create( int size ) {

	hashtable_t *hashtable = NULL;
	int i;
        if( size < 1 ) return NULL;
        if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}
        if( ( hashtable->table = malloc( sizeof( entry_t  ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;

	return hashtable;
}



int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval = 0;
	int i = 0;

	while(i< strlen( key ) ) {
		hashval = 5*hashval + key[i]; 
		i++;
	}
	return hashval % hashtable->size;
}




entry_t *htable_newpair( char *key, char *value ) {
	entry_t *newpair;
if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->key = strdup( key ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->value = strdup( value ) ) == NULL ) {
		return NULL;
	}

	newpair->next = NULL;

	return newpair;
}



void ht_set( hashtable_t *hashtable, char *key, char *value ) {
	int b = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	b = ht_hash( hashtable, key );

	next = hashtable->table[ b ];
        

	while( next != NULL) {
		last = next;
		next = next->next;
	}
		newpair = htable_newpair( key, value );
                

		if( next == hashtable->table[ b ] ) {
			hashtable->table[ b ] = newpair;

		} 
                else{
			last->next = newpair;

		} 
        newpair = NULL;
	next = NULL;
	last = NULL;
}


char *ht_get( hashtable_t *hashtable, char *key ) {
	int b = 0;
	entry_t *pair;
	b = ht_hash( hashtable, key );
	pair = hashtable->table[ b ];

	while( pair != NULL && strcmp( key, pair->key ) != 0 ) {
		pair = pair->next;
	}

	if( pair == NULL) {

		return NULL;

	} else {
		return pair->value;
	}

}
