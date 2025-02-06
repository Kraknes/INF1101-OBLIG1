#include <stdlib.h>
#include "defs.h"
#include <stdio.h>
#include "map.h"

// OBS HAR ENDRET PÅ MAP.H MED TYPEDEF FOR MAP_T, KAN HENDE AT DET ØDELEGGER TING FRAMOVER 

// Hashmap opplegg: Key kan være ordet/string -->Sendes gjennom hasher--> får ut en minne til en verdi/count

#define SIZE_CAPACITY 100000;

struct node{
    void *key;
    void *value;
    size_t key_size;
    struct node *next;
};
typedef struct node h_node;

struct map_t{
    cmp_fn cmpfn;
    hash64_fn hashfn;
    size_t length;
    size_t capacity;
    h_node *head;
    struct node **hashtables;
};

map_t *map_create(cmp_fn cmpfn, hash64_fn hashfn) {
    // Allokere minne til hashmap
    struct map_t *map = malloc(sizeof(struct map_t));

    // Hvis mappen ikke blir allokert ordentlig
    if (map == NULL){
        printf("ERROR: Failed to malloc map_t in *map_create");
        return 0;
    }

    map->hashfn = hashfn;
    map->cmpfn = cmpfn;
    map->length = 0;    
    map->capacity = SIZE_CAPACITY;

    // allokere minne til hash table * capacity (antatt antall på tilførsel)
    map->hashtables = malloc(map->capacity * sizeof(h_node));

    // Hvis hashtable ikke får allokert minne ordentlig
    if (map->hashtables == NULL){
        printf("\nERROR: Failed to malloc map->hashtable in *map_create");
        return 0;
    }

    // setter alt av innhold til null for mer effetiv sjekk om index er tom
    for (size_t i = 0; i < map->capacity; i++)
    {
        map->hashtables[i] = NULL;
    }
    return map;
}

void map_destroy(struct map_t *map, free_fn val_freefn) {
    
    for (size_t i = 0; i < map->length; i++)
    {   
        h_node *tmp = map->head;
        map->head = map->head->next;
        val_freefn(tmp->value);
        free(tmp);
        tmp = NULL;
    }
    free(map);
    map = NULL;
}

size_t map_length(struct map_t *map) {
    return map->length;
}

void *map_insert(struct map_t *map, void *key, size_t key_size, void *value) {
    // lager node for entry i hash table
    h_node *node = malloc(sizeof(h_node));

    // Hvis noe galt skjer med allokering av minne
    if (node == NULL){
        printf("\nERROR: Failed to malloc h_node in *map_insert");
    }

    // Setter inn variabler
    node->key = key;
    node->value = value;
    node->key_size = key_size;

     // Har kontroll på medlemene med linkedlist for enklere map_destroy
    if (map->length == 0){
        map->head = node;
    }
    else{
        node->next = map->head;
        map->head = node;
    }

    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
    long long unsigned hashed_index = hashed_key % map->capacity;

    if (map->hashtables[hashed_index] != NULL)
    {
        return map->hashtables[hashed_index]->value;
    }
    else
    {
        map->hashtables[hashed_index] = node;
        // øker "lengden"/antall medlemmer av array
        map->length++;
        return NULL;
    }  
}

void *map_remove(struct map_t *map, void *key) {
    /* TODO:
    - finner fram til hashed key i array, returner verdien i keyen
    
    */
       // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
    long long unsigned hashed_index = hashed_key % map->capacity;
    
    // Hvis keyen ikke eksistere i hash tablen
    if (map->hashtables[hashed_index] == NULL){
        printf("\nERROR: No entry in this hash key in *map_remove");
        return 0;
    }
    // henter ut verdien i hashtable, frigjør noden og returnere verdien
    // OBS - VIl ikke rt_value aldri bli nullifisert? Memory overflow?

    void *rt_value = map->hashtables[hashed_index]->value;
    free(map->hashtables[hashed_index]);
    map->hashtables[hashed_index] = NULL;
    map->length--;
    return rt_value;
}

void *map_get(struct map_t *map, void *key) {
    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
    long long unsigned hashed_index = hashed_key % map->capacity;

    if (map->hashtables[hashed_index] == NULL){
        printf("\nERROR: No entry in this hash key in *map_get");
        return 0;
    }

    void *rt_value = map->hashtables[hashed_index]->value;
    free(map->hashtables[hashed_index]);
    return rt_value;
}



