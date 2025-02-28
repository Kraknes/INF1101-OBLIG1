#include <stdlib.h>
#include "defs.h"
#include <stdio.h>
#include "map.h"
#include <string.h>


#define SIZE_CAPACITY 10 // Satt lavt for dynamisk hashmap, må endres til et høyere tall for statisk hashmap

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
    struct node **hashtables; 
    int iterator;
};

map_t *map_create(cmp_fn cmpfn, hash64_fn hashfn) {
    // Allokere minne til hashmap
    struct map_t *map = malloc(sizeof(struct map_t));

    // Hvis mappen ikke blir allokert ordentlig
    if (map == NULL){
        printf("ERROR: Failed to malloc map_t in *map_create");
        return 0;
    }
    // Legger til variabler
    map->hashfn = hashfn;
    map->cmpfn = cmpfn;
    map->length = 0;    
    map->capacity = SIZE_CAPACITY;
    map->iterator = 0;

    // allokere minne til hash table * capacity (antatt antall på tilførsel). Bruker calloc for å sette alt til NULL;
    map->hashtables = calloc(map->capacity, sizeof(h_node *));

    // Hvis hashtable ikke får allokert minne ordentlig
    if (map->hashtables == NULL){
        printf("\nERROR: Failed to malloc map->hashtable in *map_create");
        return 0;
    }

    return map;
}
// ødelegger map
void map_destroy(struct map_t *map, free_fn val_freefn) {
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
    free(map);
    map = NULL;
}


// returnere map->length
size_t map_length(struct map_t *map) {
    return map->length;
}

// Ødelegger hashtable. Funksjonen blir brukt for dynamisk hashtable dannelse funksjon. 
// Ødelegger forrige og lager en ny og større en.  
void hashtable_destroy(struct map_t *map) {
    // itererer gjennom alle index i hashmapen
    for (size_t i = 0; i < map->capacity; i++)
    {   
        // Hvis ingen node i index, går videre til neste index.
        if (!map->hashtables[i])
        {
            continue;
        }
        else // Ellers blir den å iterere gjennom alle nodene i lenketliste i indexen
        {
            h_node *tmp = map->hashtables[i]->next;
            while (map->hashtables[i]) // Så lenge det er noe i den posisjonen, så vil den fortsette
            {
                // frigjører value, og noden i plasseringen i index.
                free(map->hashtables[i]);
                map->hashtables[i] = NULL;
                // En sjekk for at tmp ikke er null før man iterer videre.
                if (tmp){
                    map->hashtables[i] = tmp; // Legger den i indexen, slik at while loopen fortsetter
                    tmp = tmp->next;
                }
            }
            free(tmp);
            tmp = NULL;
        }
    }
    free(map->hashtables); // Ødelegger hashtablen
    map->hashtables = NULL;
}

// Lager ny node. En del av hashtable creation function.
// Lager en ny node av samme pekere som eksisterte i forrige hashmap utenom ->next, siden den kan endre seg i den nye hashmap
h_node *create_newnode(h_node *node){
    h_node *new_node = calloc(1, sizeof(h_node)); 
    new_node->key = node->key;
    new_node->value = node->value;
    new_node->key_size = node->key_size;
    return new_node;
}

// Auto allokering av større hashtable hvis antall inserts går over 70% av størrelsen av originale hashtable. 
// Gjør hashtable dobbel så stor
void new_hashtable(struct map_t *map){
    int new_cap = map->capacity * 2; // Omordne til ny kapasitet størrelse (2x)

    struct node **new_hashtable = calloc(new_cap, sizeof(h_node *)); // Lager ny hashtable, dobbel så stor

    for (size_t i = 0; i < map->capacity; i++) // Itererer gjennom alle i forrige hashtable og plasserer i den nyeste
    {
        h_node *iter_node = map->hashtables[i];

        while(iter_node)
        {
            uint64_t hashed_key = map->hashfn(iter_node->key);
            long long unsigned hashed_index = hashed_key % new_cap; // hasher gamle key til ny hashtable

            if (!new_hashtable[hashed_index]) // Om plassen er ledig
            {  
                h_node *new_node = create_newnode(iter_node);
                new_hashtable[hashed_index] = new_node;
                iter_node = iter_node->next;
            }
            else // hvis ikke må det itereres til neste ledige plass
            { 
                h_node *tmp = new_hashtable[hashed_index];
                while (tmp->next != NULL){
                    tmp = tmp->next;
                }
                h_node *new_node = create_newnode(iter_node);
                tmp->next = new_node;
                iter_node  = iter_node->next; //går til neste node hvis det er flere i listen
            }
        }
    }
    hashtable_destroy(map); // ødelegger gamle hashtable
    map->capacity = new_cap; // setter den nye kapasitet over den gamle
    map->hashtables = new_hashtable; // legger til ny hashtable
}

void *map_insert(struct map_t *map, void *key, size_t key_size, void *value) {

    // Hvis hashtable er over en gitt størrelse, skal den økes for å unngå for mye lenket liste
    // Kan kommenteres ut for å ha en statisk hashmap, men da må SIZE_CAPACITY endres slik at hashmappen er stor nok for antall insertions.
    if (((float)(map->length)/(float)(map->capacity)) > 0.7 ) 
    {
        new_hashtable(map);
    }
    
    // Hvis en NULL key blir gitt, returneres NULL for å unngå segment fault med hashing
    if (key == NULL){
        return NULL;
    }
    // lager node for entry i hash table
    h_node *node = calloc(1, sizeof(h_node));

    // Hvis noe galt skjer med allokering av minne
    if (node == NULL){
        printf("\nERROR: Failed to malloc h_node in *map_insert");
    }

    // Setter inn variabler
    node->key = key; 
    node->value = value; 
    node->key_size = key_size;


    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
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
                free(node); // frigjører opprettelsen av ny node siden den ikke trengs å brukes
                node = NULL;
                return return_value; // Returnere forrige verdi
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
    // lager en hashed key av nøkkelen
    uint64_t hashed_key = map->hashfn(key);

    // modolu av array størrelse for å få unik index til array
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
            if (map->cmpfn(tmp->next->key, key) == 0)
            {
                map->iterator++;
                return tmp->next->value;
            }
            else
            {
                map->iterator++;
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



