/**
 * @file stdbitmap.c
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `stdbimap.c`是`stdbitmap`库的实现
 * @version 0.1
 * @date 2023-04-13
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "types.h"
#include "string.h"
#include "stdbitmap.h"

/// `BYTE_BITS`定义了一个字节有多少个位
#define BYTE_BITS 8


void bitmap_init(bitmap_t *btmp_ptr, size_t size){
    btmp_ptr->size = size;
    memset(btmp_ptr->bits, (byte)BITMAP_FREE, size);
}

void bitmap_set(bitmap_t *btmp_ptr, offset_t bit_idx, Bool taken){
    // 字节偏移量
    offset_t byte_idx = (offset_t)(bit_idx / BYTE_BITS);
    // 字节内bit偏移量, 即相对(字节)偏移量, relative bit
    offset_t rbit_idx = (offset_t)(bit_idx % BYTE_BITS);

    if (taken)
        // (BITMAP_TAKEN << rbit_idx) = 0b00001000
        btmp_ptr->bits[byte_idx] |= (BITMAP_TAKEN << rbit_idx);
    else
        // ~(BITMAP_TAKEN << rbit_idx) = 0b11110111
        btmp_ptr->bits[byte_idx] &= ~(BITMAP_TAKEN << rbit_idx);
}

Bool bitmap_test(bitmap_t *btmp_ptr, offset_t bit_idx){
    // 字节偏移量
    offset_t byte_idx = (offset_t)(bit_idx / sizeof(byte));
    // 字节内bit偏移量, 即相对(字节)偏移量, relative bit
    offset_t rbit_idx = (offset_t)(bit_idx % sizeof(byte));
    return (btmp_ptr->bits[byte_idx] & (BITMAP_TAKEN << rbit_idx)) == 0 ? False : True; 
}

offset_t bitmap_scan(bitmap_t *btmp_ptr, size_t cnt){
    // 字节偏移量, 字节内bit偏移量, 返回的全局字节bit
    offset_t byte_idx, rbit_idx, gbit_idx;
    byte_idx = rbit_idx = gbit_idx = 0;

    // 第一个非满字节 或 超过了最大字节数
    while (btmp_ptr->bits[byte_idx] == 0xFF && byte_idx < btmp_ptr->size)
        byte_idx++;
    // 如果全都满了, 直接返回-1
    if (byte_idx == btmp_ptr->size)
        return -1;

    // 字节内逐位比较, (BITMAP_TAKEN << rbit_idx) = 0b00001000
    while ((byte) (BITMAP_TAKEN << rbit_idx) & btmp_ptr->bits[byte_idx])
        rbit_idx++;

    // 找到空闲位后, 开始向后寻找
    gbit_idx = byte_idx * 8 + rbit_idx;
    
    // 只寻找一位, 直接返回
    if (cnt == 1)
        return gbit_idx;
    
    // 寻找多个位则继续向后寻找
    size_t num_bits = btmp_ptr->size * 8 - gbit_idx;
    offset_t next_bit = gbit_idx + 1;
    size_t found_bits = 1;
    gbit_idx = -1;                  // 没找到要返回-1
    while (num_bits-- > 0){
        if (!bitmap_test(btmp_ptr, next_bit))
            found_bits++;
        else
            found_bits = 0;
        
        if (found_bits == cnt){
            gbit_idx = next_bit - cnt + 1;
            break;
        }
        next_bit++;
    }
    return gbit_idx;
}

Bool bitmap_acquire(bitmap_t *btmp_ptr, offset_t out[], size_t cnt){
    size_t i = 0;
    offset_t gbit_idx = 0;
    while (gbit_idx != -1 && i++ < cnt){
        gbit_idx = bitmap_scan(btmp_ptr, 1);
        out[i] = gbit_idx;
    }
    // 如果无法满足需求, 则清空所有的位
    if (gbit_idx == -1){
        while (i-- >= 0)
            out[i] = -1;
        return False;
    }
    // 占用位
    for (i = 0; i < cnt; i++)
        bitmap_set(btmp_ptr, out[i], True);
    return True;
}

void bitmap_release(bitmap_t *btmp_ptr, offset_t in[], size_t cnt){
    for (size_t i = 0; i < cnt; i++)
        bitmap_set(btmp_ptr, in[i], False);
}