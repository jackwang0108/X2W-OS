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