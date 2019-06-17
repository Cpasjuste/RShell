//
// Created by cpasjuste on 17/06/19.
//

#ifndef LIBRSHELL_RS_LIST_H
#define LIBRSHELL_RS_LIST_H

#include <stddef.h>

typedef struct rs_node {
    struct rs_node *next;
    struct rs_node *previous;
    void *data;
} rs_node;

typedef struct rs_list {
    rs_node *head;
    rs_node *tail;
    unsigned int count;
} rs_list;

typedef void (*rs_list_forfn)(void *);

rs_list *rs_list_create(void);

void rs_list_empty(rs_list *list);

void rs_list_delete(rs_list *list);

void rs_list_add_head(rs_list *list, void *data);

void rs_list_add_tail(rs_list *list, void *data);

void rs_list_insert_before(rs_list *list, rs_node *node, void *data);

void rs_list_insert_after(rs_list *list, rs_node *node, void *data);

void *rs_list_remove_head(rs_list *list);

void *rs_list_remove_tail(rs_list *list);

void *rs_list_remove(rs_list *list, void *data);

void *rs_list_remove_node(rs_list *list, rs_node *node);

void rs_list_for_each(const rs_list *list, rs_list_forfn fun);

unsigned int rs_list_get_count(const rs_list *list);

#endif //LIBRSHELL_RS_LIST_H
