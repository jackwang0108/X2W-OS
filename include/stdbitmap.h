/**
 * @file stdbitmap.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `stdbitmap.h`是`X2W-OS`的标准位图实现
 * @version 0.1
 * @date 2023-04-13
 * 
 * @todo
 *  1. 测试stdbitmap库
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_STDBITMAP_H
#define __INCLUDE_STDBITMAP_H

#include "types.h"

/// `bitmap`中位为1, 表示资源被占用
#define BITMAP_TAKEN 1
/// `bitmap`中位为0, 表示资源未被占用
#define BITMAP_FREE 0

/**
 * @brief `bitmap_t`是位图结构体
 * 
 * @note `bitmap`用于管理大量资源, 若对应位为1, 则表示该资源已经被占用; 若对应位位0, 则表示该资源未被占用
 */
typedef struct __bitmap_t {
    /// @brief 位图的字节数
    size_t size;
    /// @brief 位图的字节数组
    byte *bits;
} bitmap_t;


/**
 * @brief `bitmap_init`用于初始化`btmp_ptr`指向的位图
 * 
 * @param btmp_ptr 指向需要初始化的位图的指针
 * @param size 位图的字节数
 */
void bitmap_init(bitmap_t *btmp_ptr, size_t size);

/**
 * @brief `bitmap_set`用于设置`btmp_ptr`指向的位图中偏移量为`bit_idx`的位
 * 
 * @param btmp_ptr 指向被设置的位图的指针
 * @param bit_idx 需要设置的位在位图中的偏移
 * @param taken 若为True, 则将位设置为1, 表示资源被占用; 若为False, 则将位设置为0, 表示资源没有没占用
 */
void bitmap_set(bitmap_t *btmp_ptr, offset_t bit_idx, Bool taken);

/**
 * @brief `bitmap_test`用于测试`btmp_ptr`指向的位图中`bit_idx`位是否被占用
 * 
 * @param btmp_ptr 指向需要测试的位图的指针
 * @param bit_idx 需要测试的位的偏移
 * @return True 测试的位已经被占用
 * @return False 测试的位未被占用
 */
Bool bitmap_test(bitmap_t *btmp_ptr, offset_t bit_idx);

/**
 * @brief `bitmap_scan`用于向`btmp_ptr`指向的位图中循找连续`cnt`个空闲位/资源
 * 
 * @param btmp_ptr 指向被检查的位图
 * @param cnt 连续位数
 * @return offset_t 若不存在连续`cnt`个位, 则返回-1; 若存在则返回起始位的偏移
 * 
 * @note `bitmap_scan`只会寻找位, 不会设置位
 */
offset_t bitmap_scan(bitmap_t *btmp_ptr, size_t cnt);

/**
 * @brief `bitmap_acquire`会从`btmp_ptr`指向的位图中申请`cnt`个位/资源, 得到的位的偏移将会放到数组`out`中
 * 
 * @param btmp_ptr 指向被申请资源所在的位图
 * @param out 存放得到的位的偏移的数组
 * @param cnt 需要申请的资源数
 * @return True 申请成功, `out`会存放申请得到的位
 * @return False 申请失败, `out`会被填充为 -1
 * 
 * @note `bitmap_acquire`找到位后会设置位
 * 
 * @warning `bitmap_acquire`不会管理`out`是否会溢出, 需要调用者负责管理`out`的大小
 */
Bool bitmap_acquire(bitmap_t *btmp_ptr, offset_t out[], size_t cnt);

/**
 * @brief `bitmap_release`会释放从`btmp_ptr`中申请得到的资源
 * 
 * @param btmp_ptr 指向需要释放的资源所在的位图
 * @param out 存放需要释放的资源的数组
 */
void bitmap_release(bitmap_t *btmp_ptr, offset_t out[], size_t cnt);

#endif