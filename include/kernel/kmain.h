/**
 * @file kmain.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `kmain.h`中提供了`kmain.c`中的宏定义、函数原型定义
 * @brief 
 * @version 0.1
 * @date 2023-04-15
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_KERNEL_KMAIN_H
#define __INCLUDE_KERNEL_KMAIN_H

/// `X2WSBI_BANNER`是在内核启动时输出字符串
#define OS_BANNER \
    "`YMM'   `MP'      `7MMF'     A     `7MF'               .g8\"\"8q.    .M\"\"\"bgd\n" \
    "  VMb.  ,P          `MA     ,MA     ,V               .dP'    `YM. ,MI    \"Y\n" \
    "   `MM.M'     pd*\"*b.VM:   ,VVM:   ,V                dM'      `MM `MMb.    \n" \
    "     MMb     (O)   j8 MM.  M' MM.  M'    **********  MM        MM   `YMMNq.\n" \
    "   ,M'`Mb.       ,;j9 `MM A'  `MM A'                 MM.      ,MP .     `MM\n" \
    "  ,P   `MM.   ,-='     :MM;    :MM;                  `Mb.    ,dP' Mb     dM\n" \
    ".MM:.  .:MMa.Ammmmmmm   VF      VF                     `\"bmmd\"'   P\"Ybmmd\" \n" \
    "Copyright Shihong Wang, Shaofei Wang, Zhuiri Xiao (c) 2023 with GNU Public License V3.0\n" \
    "Enjoy!\n"

/**
 * @brief 内核的入口点
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char kernel_entry[];
/**
 * @brief 内核的起始地址和终止地址
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char _s_kernel[],         _e_kernel[];
/**
 * @brief 内核的`.text.boot`段的起始地址和终止地址
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char _s_text_boot[],      _e_text_boot[];
/**
 * @brief 内核的`.text`段的起始地址和终止地址
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char _s_text[],           _e_text[];
/**
 * @brief 内核的`.rodata`段的起始地址和终止地址
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char _s_rodata[],         _e_rodata[];
/**
 * @brief 内核的`.data`段的起始地址和终止地址
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char _s_data[],           _e_data[];
/**
 * @brief 内核的`.bss`段的起始地址和终止地址
 * 
 * @note 
 *  1. 定义在`kernel.ld`中
 *  2. char[] 转 addr_t 交给编译器完成
 */
extern char _s_bss[],            _e_bss[];

/**
 * @brief `kernel_main`是内核的入口函数
 */
void kernel_main(void);         // make gcc happy :)

/**
 * @brief `print_kmem`用于输出内核在内存中的地址信息
 * 
 * @note 需要和`kernel.ld`配合
 */
void print_kmem(void);

#endif