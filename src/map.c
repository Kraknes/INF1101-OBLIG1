
#include "map.h"
#include "defs.h"

# test
// Hashmap opplegg: Key kan være ordet/string -->Sendes gjennom hasher--> får ut en minne til en verdi/count

typedef struct node h_node;
struct node{
    char *key;
    char *value;
    h_node *next;
};

typedef struct map_t hashmap;
struct map_t{
    cmp_fn *cmpfn;
    hash64_fn *hashfn;
    int number_of_Elements, capacity;
    struct h_node *arr;
};

map_t *map_create(cmp_fn cmpfn, hash64_fn hashfn) {
    hashmap *mapper = malloc(sizeof(hashmap));
    mapper->hashfn = hashfn;
    mapper->cmpfn = cmpfn;
    
}

void map_destroy(map_t *map, free_fn val_freefn) {
    free(map);
}

size_t map_length(map_t *map) {
    
}

void *map_insert(map_t *map, void *key, size_t key_size, void *value) {

}

void *map_remove(map_t *map, void *key) {

}

void *map_get(map_t *map, void *key) {
    
}



