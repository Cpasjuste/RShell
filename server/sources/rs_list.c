//
// Created by cpasjuste on 17/06/19.
//

#include <stdlib.h>
#include "rs_list.h"

static rs_node *rs_node_create(void *data) {
    rs_node *node = malloc(sizeof(rs_node));
    if (node) {
        node->next = NULL;
        node->previous = NULL;
        node->data = data;
    }
    return node;
}

rs_list *rs_list_create(void) {
    rs_list *list = malloc(sizeof(rs_list));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
        list->count = 0;
    }
    return list;
}

void rs_list_empty(rs_list *list) {
    while (list->head != NULL) {
        rs_list_remove_tail(list);
    }
}

void rs_list_delete(rs_list *list) {
    if (list) {
        rs_list_empty(list);
        free(list);
    }
}

void rs_list_add_head(rs_list *list, void *data) {
    rs_node *node;
    node = rs_node_create(data);
    if (list->head != NULL) {
        list->head->previous = node;
        node->next = list->head;
        list->head = node;
    } else {
        list->head = node;
        list->tail = node;
    }
    list->count++;
}

void rs_list_add_tail(rs_list *list, void *data) {
    rs_node *node;
    node = rs_node_create(data);
    if (list->tail != NULL) {
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
    } else {
        list->head = node;
        list->tail = node;
    }
    list->count++;
}

void rs_list_insert_before(rs_list *list, rs_node *node, void *data) {
    rs_node *newnode;
    if (node == list->head) {
        rs_list_add_head(list, data);
    } else {
        newnode = rs_node_create(data);
        newnode->next = node;
        newnode->previous = node->previous;
        node->previous->next = newnode;
        node->previous = newnode;
        list->count++;
    }
}

void rs_list_insert_after(rs_list *list, rs_node *node, void *data) {
    rs_node *newnode;
    if (node == list->tail) {
        rs_list_add_tail(list, data);
    } else {
        newnode = rs_node_create(data);
        newnode->previous = node;
        newnode->next = node->next;
        node->next->previous = newnode;
        node->next = newnode;
        list->count++;
    }
}

void *rs_list_remove_head(rs_list *list) {
    void *data = NULL;
    if (list->head != NULL) {
        rs_node *temp = list->head;
        list->head = list->head->next;
        if (list->head == NULL) {
            list->tail = NULL;
        } else {
            list->head->previous = NULL;
            if (list->head->next != NULL) {
                list->head->next->previous = list->head;
            } else {
                list->tail = list->head;
            }
        }
        data = temp->data;
        free(temp);
        list->count--;
    }
    return data;
}

void *rs_list_remove_tail(rs_list *list) {
    void *data = NULL;
    if (list->tail != NULL) {
        rs_node *temp = list->tail;
        list->tail = list->tail->previous;
        if (list->tail == NULL) {
            list->head = NULL;
        } else {
            list->tail->next = NULL;
            if (list->tail->previous != NULL) {
                list->tail->previous->next = list->tail;
            } else {
                list->head = list->tail;
            }
        }
        data = temp->data;
        free(temp);
        list->count--;
    }

    return data;
}

void *rs_list_remove(rs_list *list, void *data) {

    void *ret = NULL;
    rs_node *node = NULL;
    for (node = list->head; node != NULL; node = node->next) {
        if (data == node->data) {
            break;
        }
    }

    if (node != NULL) {
        if (node == list->head) {
            ret = rs_list_remove_head(list);
        } else if (node == list->tail) {
            ret = rs_list_remove_tail(list);
        } else {
            node->previous->next = node->next;
            node->next->previous = node->previous;
            ret = node->data;
            free(node);
            list->count--;
        }
    }

    return ret;
}

void *rs_list_remove_node(rs_list *list, rs_node *node) {
    void *data;
    if (node == list->head) {
        data = rs_list_remove_head(list);
    } else if (node == list->tail) {
        data = rs_list_remove_tail(list);
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        data = node->data;
        free(node);
        list->count--;
    }
    return data;
}

void rs_list_for_each(const rs_list *list, rs_list_forfn fun) {
    rs_node *node;
    for (node = list->head; node != NULL; node = node->next) {
        fun(node->data);
    }
}

unsigned int rs_list_get_count(const rs_list *list) {
    return list->count;
}
