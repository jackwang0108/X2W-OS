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

#define OS_BANNER \
    "`YMM'   `MP'      `7MMF'     A     `7MF'               .g8\"\"8q.    .M\"\"\"bgd\n" \
    "  VMb.  ,P          `MA     ,MA     ,V               .dP'    `YM. ,MI    \"Y\n" \
    "   `MM.M'     pd*\"*b.VM:   ,VVM:   ,V                dM'      `MM `MMb.    \n" \
    "     MMb     (O)   j8 MM.  M' MM.  M'    **********  MM        MM   `YMMNq.\n" \
    "   ,M'`Mb.       ,;j9 `MM A'  `MM A'                 MM.      ,MP .     `MM\n" \
    "  ,P   `MM.   ,-='     :MM;    :MM;                  `Mb.    ,dP' Mb     dM\n" \
    ".MM:.  .:MMa.Ammmmmmm   VF      VF                     `\"bmmd\"'   P\"Ybmmd\" \n" \
    "Copyright Shihong Wang (c) 2023 with GNU Public License V3.0\n" \
    "Enjoy!\n"

/**
 * @brief `kernel_main`是内核的入口函数
 * 
 */
void kernel_main(void);         // make gcc happy :)


#endif