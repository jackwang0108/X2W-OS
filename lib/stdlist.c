/**
 * @file stdlist.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdlist.c`是`stdlist`库的实现函数
 * @version 0.1
 * @date 2023-04-12
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "types.h"
#include "stdlist.h"


void list_init(list_t *list_ptr){
    list_ptr->head.prev = NULL;
    list_ptr->head.next = &list_ptr->tail;
    list_ptr->tail.prev = &list_ptr->head;
    list_ptr->tail.next = NULL;
}

void list_insert(list_elem_t *before_ptr, list_elem_t *add_ptr){
    // 需要使用锁保护
    before_ptr->prev->next = add_ptr;
    add_ptr->prev = before_ptr->prev;
    add_ptr->next = before_ptr;
    before_ptr->prev = add_ptr;
}

void list_append(list_elem_t *elem_ptr, list_t *list_ptr){
    // 就是在在list.tail前插入elem, 默认list的tail是不会被使用的, 仅标记结尾用
    list_insert(&list_ptr->tail, elem_ptr);
}

void list_remove(list_elem_t *elem_ptr){
    // 同样还是需要锁来保护
    // 删除节点就是设置上下游的链接
    elem_ptr->prev->next = elem_ptr->next;
    elem_ptr->next->prev = elem_ptr->prev;
}

void list_push(list_elem_t *elem_ptr, list_t *list_ptr){
    // 在list.head后加入, list.head默认仅标记用
    list_insert(list_ptr->head.next, elem_ptr);
}

list_elem_t* list_pop(list_t *list_ptr){
    list_elem_t *deleted_ptr = list_ptr->head.next;
    list_remove(list_ptr->head.next);
    return deleted_ptr;
}

Bool list_empty(list_t *list_ptr){
    return list_ptr->head.next == &list_ptr->tail ? True : False;
}

size_t list_size(list_t *list_ptr){
    size_t size = 0;
    list_elem_t *walking_ptr = list_ptr->head.next;
    while (walking_ptr->next != &list_ptr->tail){
        walking_ptr = walking_ptr->next;
        size++;
    }
    return size - 1;
}

Bool list_search(list_elem_t *elem_ptr, list_t *list_ptr){
    list_elem_t *walking_ptr = list_ptr->head.next;
    while (walking_ptr != &list_ptr->tail && walking_ptr != NULL){
        if (walking_ptr == elem_ptr)
            return True;
        walking_ptr = walking_ptr->next;
    }
    return False;
}

list_elem_t* list_walking(list_t *list_ptr, list_walking_func_t func, int arg){
    list_elem_t *walking_ptr = list_ptr->head.next;
    if (list_empty(list_ptr))
        return NULL;
    while (walking_ptr != &list_ptr->tail){
        if (func(walking_ptr, arg))
            return walking_ptr;
        walking_ptr = walking_ptr->next;
    }
    return NULL;
}