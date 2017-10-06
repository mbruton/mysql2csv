//
// Created by matt on 03/10/17.
//

#ifndef MYSQL2CSV_LINKED_LIST_H
#define MYSQL2CSV_LINKED_LIST_H

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL char
#endif

typedef struct linked_list_meta {
    unsigned long item_count;
    struct linked_list *first;
    struct linked_list *last;
    struct linked_list *current;
};

typedef struct linked_list {
    struct linked_list *next;
    struct linked_list *previous;
    struct linked_list_meta *meta;
    void *item;
};

typedef union item_type{
    void *item;
    unsigned long index;
};

struct linked_list *linked_list_init(void *item);
void linked_list_append(struct linked_list *linked_list, void *item);
void linked_list_prepend(struct linked_list *linked_list, void *item);
void linked_list_insert_before(struct linked_list *linked_list, void *before, void *item);
void linked_list_insert_after(struct linked_list *linked_list, void *after, void *item);
void linked_list_insert_at_index(struct linked_list *linked_list, unsigned long index, void *item);
void linked_list_remove(struct linked_list *linked_list, void *item);
void linked_list_remove_at_index(struct linked_list *linked_list, unsigned long index);
BOOL linked_list_next(struct linked_list *linked_list);
BOOL linked_list_previous(struct linked_list *linked_list);
void *linked_list_current_item(struct linked_list *linked_list);
void *linked_list_next_item(struct linked_list *linked_list);
void *linked_list_previous_item(struct linked_list *linked_list);
void *linked_list_item_at_index(struct linked_list *linked_list, unsigned long index);

#endif //MYSQL2CSV_LINKED_LIST_H
