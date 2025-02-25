
#include "list.h"
#include <stdio.h>
#include <stdlib.h>


// Hentet paramtere fra OBLIG 0

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

typedef struct list list_t;
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
        item_free(tmp->item);
        free(tmp);
    }
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


// Hentet ifra inspirasjon fra https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/

void swap(lnode_t *start, lnode_t *next)  
{  
    void *temp = start->item;  
    start->item = next->item;  
    next->item = temp;  
}  
// Bubblesort funksjon sorterer listen
void list_sort(list_t *list) {
    // Lager en variable som styrer om bubblesort skal termineres eller ikke
    int swapped = 1;
    // 
    lnode_t *start = list->head;
    if (start == NULL)
    {
        printf("ERROR: list->head does not exist in list_sort");
        return;
    }

    // While funksjon som blir styrt av swapped. Hvis to elementer blir byttet i sorten, vil swapped forbli 1.
    // Hvis det er ikke noe flere elementer som skal bli byttet vil swapped = 0, og da termineres while loopen. 
    while(swapped == 1) {
        swapped = 0;
        start = list->head;
        // While loop som initeres så lenge start noden er ikke det samme som siste node i listen
        while (start->next != list->tail->next)
        {
            // Hvis start noden har en item større en neste i listen, vil de bytte plass
            if (list->cmpfn(start->item,start->next->item) > 0)
            {
                swap(start, start->next);
                swapped = 1;
            }
            start = start->next; // Hvis start noden er i riktige plassering i forhold til neste node, går funksjon videre til neste node. 
        }
    }
}


list_iter_t *list_createiter(list_t *list) {
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    iter->node = list->head;
    iter->list = list;
    if (iter == NULL){
        printf("\nERROR: No iter created in *list_createiter");
        return NULL;
    }
    else {
        return iter;
    }
}

void list_destroyiter(list_iter_t *iter) {
    if (iter == NULL){
        printf("\nERROR: No iter to destroy in list_destroyiter");
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
        printf("\nERROR: List has no head to restart in list_resetiter");
    }
    
    iter->node = iter->list->head;    
}
