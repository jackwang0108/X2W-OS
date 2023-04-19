/**
 * @file smain.h
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `smain.h`中提供了`smain.c`中的宏定义、函数原型定义
 * @version 0.1
 * @date 2023-04-14
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_SBI_SMAIN_H
#define __INCLUDE_SBI_SMAIN_H

#include "types.h"

/// `X2WSBI_BANNER`是在`SBI`启动时输出字符串
#define X2WSBI_BANNER \
    "`YMM'   `MP'      `7MMF'     A     `7MF'             .M\"\"\"bgd `7MM\"\"\"Yp, `7MMF'\n" \
    "  VMb.  ,P          `MA     ,MA     ,V              ,MI    \"Y   MM    Yb   MM  \n" \
    "   `MM.M'     pd*\"*b.VM:   ,VVM:   ,V               `MMb.       MM    dP   MM  \n" \
    "     MMb     (O)   j8 MM.  M' MM.  M'   **********    `YMMNq.   MM\"\"\"bg.   MM  \n" \
    "   ,M'`Mb.       ,;j9 `MM A'  `MM A'                .     `MM   MM    `Y   MM  \n" \
    "  ,P   `MM.   ,-='     :MM;    :MM;                 Mb     dM   MM    ,9   MM  \n" \
    ".MM:.  .:MMa.Ammmmmmm   VF      VF                  P\"Ybmmd\"  .JMMmmmd9  .JMML.\n\n" \
    "Copyright Shihong Wang, Shaofei Wang, Zhuiri Xiao (c) 2023 with GNU Public License V3.0\n" \
    "Enjoy!\n"

/**
 * @brief `sbi_main`是`SBI`的主函数, 在进行一些准备后跳转到内核中运行
 * 
 * @note `sbi_main`函数运行在M模式
 * 
 * @note `sbi_main`函数干的事情:
 *  1. 跳转到内核: 调用`jump_to_kernel`函数实现
 */
NO_RETURN void sbi_main(void);


/**
 * @brief `jump_to_kernel`在伪装中断返回后调用`mret`指令以跳转到内核中去
 * 
 * @note  `jump_to_kernel`函数需要注意的点如下:
 * 1. 该函数通过伪装`S`模式中断返回的模式跳转到内核的主函数
 * 2. 该函数伪装中断返回的地址是由`KERNEL_JUMP_ADDR`宏决定, 该宏定义在`constrains.h`中
 * 3. `KERNEL_JUMP_ADDR`宏的地址需要指向`kernel_start`函数, 该函数定义在`kernel/boot.S`中
 * 
 * @note `jump_to_kernel`函数在跳转到内核前会进行如下的准备工作以伪装成中断返回:
 * 1. 伪装从S模式中断进入M模式
 * 2. 设置M模式的中断返回地址
 * 3. 设置S模式中断向量表入口
 * 4. 关闭S模式的中断
 * 5. 关闭S模式的页表转换
 */
NO_RETURN void jump_to_kernel(void);

#endif