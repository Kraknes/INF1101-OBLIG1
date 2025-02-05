
#include "list.h"
#include <stdio.h>

#include <stdlib.h>

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

/* Prøvde å hente mergesort fra Oblig 0, men funket ikke*/

// static lnode_t *merge(lnode_t *a, lnode_t *b, cmp_fn cmpfn) {
//     lnode_t *head, *tail;

//     /* Pick the smallest head node */
//     if (cmpfn(a->item, b->item) < 0) {
//         head = tail = a;
//         a = a->next;
//     } else {
//         head = tail = b;
//         b = b->next;
//     }

//     /* Now repeatedly pick the smallest head node */
//     while (a && b) {
//         if (cmpfn(a->item, b->item) < 0) {
//             tail->next = a;
//             tail = a;
//             a = a->next;
//         } else {
//             tail->next = b;
//             tail = b;
//             b = b->next;
//         }
//     }

//     /* Append the remaining non-empty list (if any) */
//     if (a) {
//         tail->next = a;
//     } else {
//         tail->next = b;
//     }

//     return head;
// }

// static lnode_t *splitlist(lnode_t *head) {
//     /* Move two pointers, a 'slow' one and a 'fast' one which moves
//      * twice as fast.  When the fast one reaches the end of the list,
//      * the slow one will be at the middle.
//      */
//     lnode_t *slow = head;
//     lnode_t *fast = head->next;

//     while (fast != NULL && fast->next != NULL) {
//         slow = slow->next;
//         fast = fast->next->next;
//     }

//     /* Now 'cut' the list and return the second half */
//     lnode_t *half = slow->next;
//     slow->next = NULL;

//     return half;
// }


// static lnode_t *mergesort_(lnode_t *head, cmp_fn cmpfn) {
//     if (head->next == NULL) {
//         return head;
//     }


//     lnode_t *half = splitlist(head);
//     head = mergesort_(head, cmpfn);
//     half = mergesort_(half, cmpfn);

//     return merge(head, half, cmpfn);
// }

// void list_sort(list_t *list) {
//     /* Recursively sort the list */
//     list->head = mergesort_(list->head, list->cmpfn);

//     /* Fix the tail and prev links */
//     lnode_t *prev = NULL;
//     for (lnode_t *n = list->head; n != NULL; n = n->next) {
//         n->prev = prev;
//         prev = n;
//     }
//     list->tail = prev;
// }

// Hentet ifra https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/

void swap(lnode_t *a, lnode_t *b)  
{  
    int temp = a->item;  
    a->item = b->item;  
    b->item = temp;  
}  

void list_sort(list_t *list) {
    int swapped;
    int i;
    lnode_t *start;
    lnode_t *node_ptr = NULL;
    if (start == NULL)
    {
        printf("ERROR: list->head does not exist");
        return;
    }
    do {
        swapped = 0;
        start = list->head;
        while (start->next != node_ptr)
        {
            if (start->item > start->next->item)
            {
                swap(start, start->next);
                swapped = 1;
            }
            start = start->next;
        }
        node_ptr > start;
    }
    while(swapped);

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
