#include <stdlib.h>
#include "defs.h"
#include <stdio.h>
#include "map.h"
<<<<<<< HEAD
=======
#include <string.h>
>>>>>>> master

// OBS HAR ENDRET PÅ MAP.H MED TYPEDEF FOR MAP_T, KAN HENDE AT DET ØDELEGGER TING FRAMOVER 

// Hashmap opplegg: Key kan være ordet/string -->Sendes gjennom hasher--> får ut en minne til en verdi/count

<<<<<<< HEAD
#define SIZE_CAPACITY 100000;
=======
#define SIZE_CAPACITY 100000
>>>>>>> master

struct node{
    void *key;
    void *value;
<<<<<<< HEAD
=======
    size_t key_size;
>>>>>>> master
    struct node *next;
};
typedef struct node h_node;

struct map_t{
    cmp_fn cmpfn;
    hash64_fn hashfn;
    size_t length;
<<<<<<< HEAD
    int capacity;
    h_node *head;
    struct node **hashtables;
=======
    size_t capacity;
    struct node **hashtables; 
    int iterator;
>>>>>>> master
};

map_t *map_create(cmp_fn cmpfn, hash64_fn hashfn) {
    // Allokere minne til hashmap
    struct map_t *map = malloc(sizeof(struct map_t));

<<<<<<< HEAD
=======
    // Hvis mappen ikke blir allokert ordentlig
>>>>>>> master
    if (map == NULL){
        printf("ERROR: Failed to malloc map_t in *map_create");
        return 0;
    }
<<<<<<< HEAD

=======
    // Legger til variabler
>>>>>>> master
    map->hashfn = hashfn;
    map->cmpfn = cmpfn;
    map->length = 0;    
    map->capacity = SIZE_CAPACITY;
<<<<<<< HEAD

    // allokere minne til hash table * capacity (antatt antall på tilførsel)
    map->hashtables = malloc(sizeof(h_node)*map->capacity);

    if (map->hashtables == NULL){
        printf("ERROR: Failed to malloc map->hashtable in *map_create");
        return 0;
    }

    // setter alt av innhold til null for mer effetiv sjekk om index er tom
    for (int i = 0; i < map->capacity; i++)
    {
        map->hashtables[i] = NULL;
    }
=======
    map->iterator = 0;

    // allokere minne til hash table * capacity (antatt antall på tilførsel). Bruker calloc for å sette alt til NULL;
    map->hashtables = calloc(map->capacity, sizeof(h_node *));

    // Hvis hashtable ikke får allokert minne ordentlig
    if (map->hashtables == NULL){
        printf("\nERROR: Failed to malloc map->hashtable in *map_create");
        return 0;
    }

>>>>>>> master
    return map;
}

void map_destroy(struct map_t *map, free_fn val_freefn) {
<<<<<<< HEAD
    
    for (size_t i = 0; i < map->length; i++)
    {   
        h_node *tmp = map->head;
        map->head = map->head->next;
        val_freefn(tmp);
        tmp = NULL;
    }
=======
    // itererer gjennom alle index i hashmapen
    for (size_t i = 0; i < map->capacity; i++)
    {   
        // Hvis ingenting i index, går videre.
        if (map->hashtables[i] == NULL)
        {
            continue;
        }
        // Ellers blir den å iterere gjennom alle nodene i lenketliste i indexen
        else
        {
            h_node *tmp = map->hashtables[i]->next;
            while (map->hashtables[i] != NULL)
            {
                // frigjører value, og noden i plasseringen i index.
                val_freefn(map->hashtables[i]->value);
                free(map->hashtables[i]);
                map->hashtables[i] = NULL;
                // En sjekk for at tmp ikke er null før man iterer videre.
                if (tmp != NULL){
                    map->hashtables[i] = tmp;
                    tmp = tmp->next;
                }
            }
            free(tmp);
            tmp = NULL;
            
        }
    }
    free(map->hashtables);
    map->hashtables = NULL;
>>>>>>> master
    free(map);
    map = NULL;
}

size_t map_length(struct map_t *map) {
    return map->length;
}

void *map_insert(struct map_t *map, void *key, size_t key_size, void *value) {
<<<<<<< HEAD
    /* TODO LIST:
    1. Lage node av key og value
    x. Må returnere 0 hvis alt gikk bra, eller returnere pointer til gamle key hvis det har blitt kollisjon
    x. Finne ut av key_size, ka e funskjonen    
    */

    // lager node for entry i hash table
    h_node *node = malloc(sizeof(h_node));

    if (node == NULL){
        printf("ERROR: Failed to malloc h_node in *map_insert");
        return NULL;
    }

    node->key = key;
    node->value = value;
=======
    
    // Hvis en NULL key blir gitt, returneres NULL for å unngå segment fault med hashing
    if (key == NULL){
        return NULL;
    }
    // lager node for entry i hash table
    h_node *node = malloc(sizeof(h_node));

    // Hvis noe galt skjer med allokering av minne
    if (node == NULL){
        printf("\nERROR: Failed to malloc h_node in *map_insert");
    }

    // Setter inn variabler
    node->key = key; // skrev (char *) for å troubleshoote, er kanskje ikke vits
    node->next = NULL;
    node->value = (int*) value; // skrev (int *) for å troubleshoote, er kanskje ikke vits
    node->key_size = key_size;

>>>>>>> master

    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
<<<<<<< HEAD
    int hashed_index = hashed_key % map->capacity;

    while (map->hashtables[hashed_index] != NULL)
    {
        return map->hashtables[hashed_index];
    }
    
    map->hashtables[hashed_index] = node;

    // øker "lengden"/antall medlemmer av array
    map->length++;
    return 0;
}

void *map_remove(struct map_t *map, void *key) {
    /* TODO:
    - finner fram til hashed key i array, returner verdien i keyen
    
    */
       // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
    int hashed_index = hashed_key % map->capacity;
    
    // Hvis keyen ikke eksistere i hash tablen
    if (map->hashtables[hashed_index] == NULL){
        printf("ERROR: No entry in this hash key in *map_remove");
        return 0;
    }
    // henter ut verdien i hashtable, frigjør noden og returnere verdien
    void *rt_value = map->hashtables[hashed_index]->value;
    free(map->hashtables[hashed_index]);
    return rt_value;
}

void *map_get(struct map_t *map, void *key) {
=======
    long long unsigned hashed_index = hashed_key % map->capacity;

    // kollisjonsfunksjon med lenket liste i hashmap. Hvis det er noe allerede i index, går det til denne funksjon
    if (map->hashtables[hashed_index]) 
    {   
        // Lager en itererings node
        h_node *tmp = map->hashtables[hashed_index];

        // Hvis første instans av noden er samme key, så returneres verdien av "gamle key" og setter inn nye verdi
        if(map->cmpfn(tmp->key, node->key) == 0)
        {
            void *return_value = tmp->value;
            tmp->value = value;
            free(node);
            node = NULL;
            return return_value;
        }

        // Sjekker next node om det er samme key, ellers iterere videre i lenketliste. Hvis funnet, returnerer gamle verdi og setter inn ny verdi
        while (tmp->next)
        {
            if (map->cmpfn(tmp->next->key, node->key) == 0)
            {
                void *return_value = tmp->next->value;
                tmp->next->value = value;
                free(node);
                node = NULL;
                return return_value;
            }
            else
            {
                tmp = tmp->next;
            }
        }
        // Hvis det itereres og ikke funnet key, blir den lagt til som *next i siste node i listen
        tmp->next = node; 
        map->length++;
        return NULL;
        
    }
    else
    // Hvis indexen i hashmp er tom, plasserer ny node i indexen. 
    {
        map->hashtables[hashed_index] = node;
        map->length++;
        return NULL;
    }  
}

void *map_remove(struct map_t *map, void *key) {
>>>>>>> master
    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
<<<<<<< HEAD
    int hashed_index = hashed_key % map->capacity;

    if (map->hashtables[hashed_index] == NULL){
        printf("ERROR: No entry in this hash key in *map_get");
        return 0;
    }

    void *rt_value = map->hashtables[hashed_index]->value;
    free(map->hashtables[hashed_index]);
    return rt_value;
=======
    long long unsigned hashed_index = hashed_key % map->capacity;
    

    if (map->hashtables[hashed_index]){
        h_node *tmp = map->hashtables[hashed_index];

        // Hvis første instans av noden er samme key, så returneres verdien av "key". Noden til key fjernes også. 
        if(map->cmpfn(tmp->key, key) == 0)
        {
            void *rt_value = tmp->value;
            map->hashtables[hashed_index] = tmp->next;
            free(tmp);
            tmp = NULL;
            map->length--;
            return rt_value;
        }

        // Hvis ikke har samme key, så vil den iterere gjennom linkedlist til den finner riktig key
        while (tmp->next)
        {
            if (map->cmpfn(tmp->next->key, key) == 0)
            {
                void *rt_value = tmp->next->value;
                // lager en node for noden som eier key som skal slettes
                h_node *node_delete = tmp->next;
                tmp->next = tmp->next->next;
                free(node_delete);
                node_delete = NULL;
                map->length--;
                return rt_value;
            }
            else
            {
                tmp = tmp->next;
            }
        }
        // Hvis den ikke er i listen, vil den returnere NULL;
        return NULL;

    }
    // Hvis det er ingen node i indexen som blir gitt av key
    else{
        return NULL;
    }
}

void *map_get(struct map_t *map, void *key) {

    if (key == NULL){
        return NULL;
    }

    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
    long long unsigned hashed_index = hashed_key % map->capacity;
 
    // Hvis index har node, vil den gå gjennom iterering gjennom linkedlist i indexen
    if (map->hashtables[hashed_index]){
        h_node *tmp = map->hashtables[hashed_index];

        // Hvis første instans av noden er samme key, så returneres verdien av "gamle key"
        if(map->cmpfn(tmp->key, key) == 0)
        {
            map->iterator++;
            return tmp->value;
        }

        // Hvis ikke har samme key, så vil den iterere gjennom linkedlist til den finner riktig key
        while (tmp->next)
        {
            map->iterator++;
            if (map->cmpfn(tmp->next->key, key) == 0)
            {
                
                return tmp->next->value;
                
            }
            else
            {
                tmp = tmp->next;
            }
        }
        // Hvis den ikke er i listen, vil den returnere NULL;
        return NULL;

    }
    // Hvis det er ingen node i indexen som blir gitt av key
    else{
        return NULL;
    }
>>>>>>> master
}



