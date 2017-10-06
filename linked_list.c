//
// Created by matt on 03/10/17.
//

#include <malloc.h>
#include "linked_list.h"

// TODO: Bounds checking on next / previous calls

struct linked_list *linked_list_init(void *item) {
    struct linked_list *linked_list;
    struct linked_list_meta *meta;
    linked_list = malloc(sizeof(struct linked_list));
    linked_list->next = NULL;
    linked_list->previous = NULL;
    linked_list->item = item;
    
    meta = malloc(sizeof(struct linked_list_meta));
    linked_list->meta = meta;
    meta->current = linked_list;
    meta->first = linked_list;
    meta->last = linked_list;
    meta->item_count = 1;

    return linked_list;
}

void linked_list_append(struct linked_list *linked_list, void *item) {
    struct linked_list *new_list_item;
    new_list_item = malloc(sizeof(struct linked_list));
    new_list_item->previous = linked_list->meta->last;

    linked_list->meta->last->next = new_list_item;
    linked_list->meta->last = new_list_item;
    new_list_item->next = NULL;
    new_list_item->item = item;
    new_list_item->meta = linked_list->meta;
    new_list_item->meta->item_count++;
}

void linked_list_prepend(struct linked_list *linked_list, void *item) {
    struct linked_list *new_list_item;

    new_list_item = malloc(sizeof(linked_list));
    new_list_item->next = linked_list->meta->first;
    linked_list->meta->first->previous = new_list_item;
    linked_list->meta->first = new_list_item;
    new_list_item->previous = NULL;
    new_list_item->item = item;
    new_list_item->meta->item_count++;
}

void linked_list_insert_before(struct linked_list *linked_list, void *before, void *item) {
    struct linked_list *head = linked_list->meta->first;
    struct linked_list *new;

    while(head != NULL) {
        if (head->item == before) {
            new = malloc(sizeof(linked_list));
            new->next = head;
            new->previous = head->previous;
            new->item = item;
            head->previous->next = new;
            new->meta = head->meta;
            head->meta->item_count++;
            head = NULL;
        } else {
            head = head->next;
        }
    }
}

void linked_list_insert_after(struct linked_list *linked_list, void *after, void *item) {
    struct linked_list *head = linked_list->meta->first;
    struct linked_list *new;

    while(head != NULL) {
        if (head->item == after) {
            new = malloc(sizeof(linked_list));
            new->next = head->next;
            new->previous = head;
            new->item = item;
            head->next->previous = new;
            head->next = new;
            new->meta = head->meta;
            head->meta->item_count++;
            head = NULL;
        } else {
            head = head->next;
        }
    }
}

void linked_list_insert_at_index(struct linked_list *linked_list, unsigned long index, void *item) {
    unsigned long i;
    struct linked_list *head = linked_list->meta->first;
    struct linked_list *new;

    if (index < head->meta->item_count && index >= 0) {
        for (i = 0; i < head->meta->item_count; i++) {
            if (i == index) {
                new = malloc(sizeof(linked_list));
                new->next = head->next;
                new->previous = head;
                new->item = item;
                head->next->previous = new;
                head->next = new;
                new->meta = head->meta;
                i = head->meta->item_count + 2;
                head->meta->item_count++;
            } else {
                head = head->next;
            }
        }
    }
}

void linked_list_remove(struct linked_list *linked_list, void *item) {
    struct linked_list *head = linked_list->meta->first;

    while (head != NULL) {
        if (head->item == item) {
            if (head->previous && head->next) {
                head->previous->next = head->next;
                head->next->previous = head->previous;

                if (head == head->meta->current) {
                    head->meta->current = head->next;
                }
                head->meta->item_count--;
                free(head);

                head = NULL;
            }
        } else {
            head = head->next;
        }
    }
}

void linked_list_remove_at_index(struct linked_list *linked_list, unsigned long index) {
    unsigned long i;
    struct linked_list *head = linked_list->meta->first;

    if (index >= 0 && index < head->meta->item_count) {
        for(i = 0; i < head->meta->item_count; i++) {
            if (index == i) {
                i = linked_list->meta->item_count--;
                if (head->previous == NULL && head->next == NULL) {
                    free(head->meta);
                    free(head);
                } else if (head->previous == NULL) {
                    head->next->previous = NULL;
                    head->meta->first = head->next;
                    free(head);
                } else if (head->next == NULL) {
                    head->previous->next = NULL;
                    head->meta->last = head->previous;
                    free(head);
                } else {
                    head->previous->next = head->next;
                    head->next->previous = head->previous;
                    free(head);
                }

            } else {
                head = head->next;
            }
        }
    }
}

BOOL linked_list_next(struct linked_list *linked_list) {
    if (linked_list->meta->current->next != NULL) {
        linked_list->meta->current = linked_list->meta->current->next;
        return TRUE;
    }

    return FALSE;
}

BOOL linked_list_previous(struct linked_list *linked_list) {
    if (linked_list->meta->current->previous != NULL) {
        linked_list->meta->current = linked_list->meta->current->previous;
        return TRUE;
    }

    return FALSE;
}

void *linked_list_current_item(struct linked_list *linked_list) {
    return linked_list->meta->current->item;
}

void *linked_list_next_item(struct linked_list *linked_list) {
    if (linked_list->meta->current->next) {
        return linked_list->meta->current->next->item;
    }

    return NULL;
}

void *linked_list_previous_item(struct linked_list *linked_list) {
    if (linked_list->meta->current->previous) {
        return linked_list->meta->current->previous->item;
    }

    return NULL;
}

void *linked_list_item_at_index(struct linked_list *linked_list, unsigned long index) {
    unsigned long i;
    struct linked_list *head = linked_list->meta->first;

    if (index >= 0 && index < linked_list->meta->item_count) {
        for(i = 0; i < linked_list->meta->item_count; i++) {
            if (i == index) {
                return head->item;
            } else {
                head = head->next;
            }
        }
    }

    return NULL;
}
