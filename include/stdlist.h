/**
 * @file stdlist.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdlist.h`是`X2W-OS`的标准链表实现
 * @version 0.1
 * @date 2023-04-12
 * 
 * @todo
 *  1. 测试stdlist库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STDLIST_H
#define __INCLUDE_STDLIST_H

#include "types.h"
#include "stdlib.h"

/**
 * @brief `list_elem_t`是链表节点结构体
 * 
 * @note `list_elem_t`需要作为结构体的成员, 而后通过`member2struct`宏获得指向结构体的指针
 */
typedef struct __list_elem_t {
    /// @brief 指向上一个链表节点的指针
    struct __list_elem_t* prev;
    /// @brief 指向下一个链表节点的指针
    struct __list_elem_t* next;
} list_elem_t;

/**
 * @brief `list_t`是链表结构体
 */
typedef struct __list_t {
    /// @brief 链表头结点, 仅标记用, 第一个节点是head->next
    list_elem_t head;
    /// @brief 链表尾结点, 仅标记用, 最后一个节点是tail->prev
    list_elem_t tail;
} list_t;

/**
 * @brief `list_walking_func_t`是自定义函数类型, 用于在链表遍历函数`list_walking`中作为回调函数
 * 
 * @param elem 链表节点指针
 * @param arg 参数
 */
typedef Bool (list_walking_func_t)(list_elem_t *elem_ptr, int arg);


/**
 * @brief `list_init`用于初始化`list_ptr`指向的链表
 * 
 * @param list_ptr 指向需要初始化的链表的指针
 */
void list_init(list_t *list_ptr);

/**
 * @brief `list_insert`用于在`before_ptr`指向的节点前插入`add_ptr`指向的节点
 * 
 * @param before_ptr 指向链表中被插入的节点的指针
 * @param add_ptr 指向要插入到链表中的节点的指针
 * 
 * @warning 在实现锁或者中断之后需要保护一下
 */
void list_insert(list_elem_t *before_ptr, list_elem_t *add_ptr);

/**
 * @brief `list_append`用于将`elem_ptr`指向的节点添加到`list_ptr`指向的链表后面
 * 
 * @param elem_ptr 指向要添加的节点的指针
 * @param list_ptr 指向被添加的链表的指针
 */
void list_append(list_elem_t *elem_ptr, list_t *list_ptr);

/**
 * @brief `list_remove`用于将`elem_ptr`指向的节点从在其所在的链表中删除
 * 
 * @param elem_ptr 指向要删除的节点的指针
 */
void list_remove(list_elem_t *elem_ptr);

/**
 * @brief `list_push`将链表视为一个栈, 用于向`list_ptr`指向的链表头前(栈顶)压入`elem_ptr`指向的节点
 * 
 * @param elem_ptr 指向要被压入的节点的指针
 * @param list_ptr 指向要被压入的链表的指针
 */
void list_push(list_elem_t *elem_ptr, list_t *list_ptr);

/**
 * @brief `list_pop`将链表视为一个栈, 用于向`list_ptr`指向的链表头前(栈顶)弹出一个节点
 * 
 * @param list_ptr 指向要被弹出的链表的指针
 * 
 * @return list_elem_t 指向被弹出的节点的指针
 */
list_elem_t* list_pop(list_t *list_ptr);


/**
 * @brief `list_empty`用于判断`list_ptr`指向的链表是否为空
 * 
 * @param list_ptr 指向被判断的链表的指针
 * @return True 链表为空
 * @return False 链表不为空
 */
Bool list_empty(list_t *list_ptr);

/**
 * @brief `list_size`用于计算`list_ptr`指向的链表的长度(元素个数)
 * 
 * @param list_ptr 被计算长度的链表
 * @return size_t 链表的长度(元素个数)
 */
size_t list_size(list_t *list_ptr);

/**
 * @brief `list_search`用于在`list_ptr`指向的链表中查找是否存在`elem_ptr`指向的节点
 * 
 * @param elem_ptr 指向被查找的节点的指针
 * @param list_ptr 指向被查找的链表的指针
 * 
 * @return True `elem_ptr`指向的节点在`list_ptr`指向的链表中
 * @return False `elem_ptr`指向的节点不在`list_ptr`指向的链表中
 */
Bool list_search(list_elem_t *elem_ptr, list_t *list_ptr);

/**
 * @brief `list_walking`用于遍历`list_ptr`指向的链表, 并对每一个节点调用`func`判断是否继续遍历
 * 
 * @param list_ptr 指向要遍历的链表的指针
 * @param func 将应用在每一个节点上的函数
 * @param arg 传给`func`的参数
 * @return list_elem_t* 若将`func`应用在当前节点后返回值为True, 则停止遍历, 且返回指向当前节点的指针, 否则继续遍历; 若将func应用于链表中所有元素后, 没有一个节点返回True, 则返回NULL
 */
list_elem_t* list_walking(list_t *list_ptr, list_walking_func_t func, int arg);

#endif