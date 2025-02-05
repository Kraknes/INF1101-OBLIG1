/**
 * @authors
 * Steffen Viken Valvaag
 * Odin Bjerke <odin.bjerke@uit.no>
 */

#include "list.h"
#include "printing.h"

#include <stdlib.h>


// Previous linked-list implementation oblig 0
// ./bin/debug/wordfreq data/oxford-dict.txt 20 4 10 --test

typedef struct lnode lnode_t;
struct lnode {
    lnode_t *next;
    lnode_t *prev;
    void *item;
};

struct list {
    lnode_t *head;
    lnode_t *tail;
    size_t length;
    cmp_fn cmpfn;
};

struct list_iter {
    list_t *list;
    lnode_t *node;
};


static lnode_t *newnode(void *item) {
    lnode_t *node = malloc(sizeof(lnode_t));
    node->item = item;
    return node;
}

list_t *list_create(cmp_fn cmpfn) {
    list_t *list = malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->cmpfn = cmpfn;
    return list;
}

void list_destroy(list_t *list, free_fn item_free) {
    
    for (size_t i = 0; i < list->length; i++)
    {   
        lnode_t *tmp = list->head;
        list->head = list->head->next;
        item_free(tmp);
    }
    free(item_free); // Denne må fikses! Itemfree er en funksjon, bruk den på alle noder
    free(list);
}


size_t list_length(list_t *list) {
    if (list->length == 0) {
        return 0;
    }
    return list->length;
}

int list_addfirst(list_t *list, void *item) {
    lnode_t *node = newnode(item);
    if (node == NULL)
    {
        printf("\nERROR: Failed to allocate memory to new node to head");
        return -1;
    }

    if (list->length == 0) {
        list->tail = node;
        list->head = node;
    }
    else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->length++;
    return 0;
}

int list_addlast(list_t *list, void *item) {
    lnode_t *node = newnode(item);
        if (node == NULL)
    {
        printf("\nERROR: Failed to allocate memory to new node to tail");
        return -1;
    }
    node->item = item;
    if (list->length == 0){
        list->head = node;
        list->tail = node;
        list->length++;
    }
    else{ 
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
        list->length++;
    }
    return 0;
}

void *list_popfirst(list_t *list) {
        if (list->head == NULL){
        printf("\nERROR: List has no head");
        exit(EXIT_FAILURE);
    }
    lnode_t *tmp_node = list->head;
    if (tmp_node == NULL){
        printf("\nERROR: Failed to allocate memory to temporary item");
        exit(EXIT_FAILURE);
    }
    void *tmp_item = tmp_node->item;
    list->head = list->head->next;
    free(tmp_node);
    list->length--;
    return tmp_item;
}

void *list_poplast(list_t *list) {
    if (list->tail == NULL)
    {
        printf("\nERROR: List has no tail");
        return NULL;
    }
    lnode_t *tmp_node = list->tail;
    void *tmp_item = list->tail->item;
    list->tail = list->tail->prev;
    free(tmp_node);
    tmp_node = NULL;
    list->length--;
    return tmp_item; 
}

int list_contains(list_t *list, void *item) {
    if (list == NULL) {
        printf("\nERROR: List does not exist");
        return 0;
    }
    lnode_t *node_cmp = list->head;
    for (size_t i=1; i <= list->length; i++)
    {
        if (list->cmpfn(item, node_cmp->item) == 0)
        {
            return 1;
        }
        else {
            node_cmp = node_cmp->next;
        }
        
    }
    printf("FAILURE: Ingen likheter funnet");
    return 0;
}

void list_sort(list_t *list) {
 // hva skal hit? Hentet nedenfor fra Oblig 0, men vet ikke helt hva den gjrø

    // bubblesort her 

     /* Recursively sort the list */
    // list->head = mergesort_(list->head, list->cmpfn);

    // /* Fix the tail and prev links */
    // lnode_t *prev = NULL;
    // for (lnode_t *n = list->head; n != NULL; n = n->next) {
    //     n->prev = prev;
    //     prev = n;
    // }
    // list->tail = prev;
}

list_iter_t *list_createiter(list_t *list) {
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    iter->node = list->head;
    iter->list = list;
    if (iter == NULL){
        printf("\nERROR: No iter created");
        return NULL;
    }
    else {
        return iter;
    }
}

void list_destroyiter(list_iter_t *iter) {
    if (iter == NULL){
        printf("\nERROR: No iter to destroy");
    }
    free(iter);
    iter = NULL;
}

int list_hasnext(list_iter_t *iter) {
    if (iter->node == NULL)
    {
        return 0;
    }
    else {
        return 1;
    }
}

void *list_next(list_iter_t *iter) {
    void *tmp = iter->node->item;
    iter->node = iter->node->next;
    return tmp;
}

void list_resetiter(list_iter_t *iter) {
    if (iter->list->head == NULL)
    {
        printf("\nERROR: List has no head to restart");
    }
    
    iter->node = iter->list->head;    
}
