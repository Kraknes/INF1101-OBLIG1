
#include "map.h"
#include "defs.h"

// Hashmap opplegg: Key kan være ordet/string -->Sendes gjennom hasher--> får ut en minne til en verdi/count

/* Usikker om det trengs node*/

struct node{
    char *key;
    int *value;
    struct node *next;
};
typedef struct node h_node;

/* må dobbeltsjekke en hashmap innhold, følgte bare en guide online: https://www.geeksforgeeks.org/implementation-of-hash-table-in-c-using-separate-chaining/ */
typedef struct map_t{
    cmp_fn *cmpfn;  /* compare item funksjon */
    hash64_fn *hashfn; /* hashing funksjon */
    size_t length;
    size_t capacity;
    int **arr;
};
typedef struct map_t map_t;

map_t *map_create(cmp_fn cmpfn, hash64_fn hashfn) {
    struct map_t *map = malloc(sizeof(struct map_t));
    if (map == NULL){
        printf("\nERROR: *map is null"); /* må sjekke mer på printing.h for errors */
        return NULL;
    }
    map->hashfn = hashfn; 
    map->cmpfn = cmpfn; 
    map->capacity = 12345;
    map->length = 0;
    map->arr[map->capacity];
    return map;
}

void map_destroy(map_t *map, free_fn val_freefn) { /* free_fn frigir innhold i values*/
    free(map);
}

size_t map_length(map_t *map) {
    if (map->length == 0){
        return 0;
    }
   return map->length;
}

void *map_insert(map_t *map, void *key, size_t key_size, void *value) {
    if (!map || !key || !key_size || !value){ 
        return NULL;
        }

    uint64_t hashed_key = map->hashfn(key);

    map->length++;
}

void *map_remove(map_t *map, void *key) {

}

void *map_get(map_t *map, void *key) {
    
}



