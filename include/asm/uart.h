/**
 * @file asm/uart.h
 * @author Shihong Wang (jack4shihong@gamil.com)
 * @brief `asm/uart.h`文件中定义了`AXI UART 16550`相关的常量
 * @version 0.1
 * @date 2023-04-09
 * 
 * @note 关于`AXI UART 16550`芯片介绍: https://wenku.baidu.com/view/6f7a176048d7c1c708a14521.html?_wkts_=1681021571718
 * @note 关于`AXI UART 16550`相关常量, 参考`AXI UART 16550`编程手册(`Programming Table`): http://byterunner.com/16550.html
 * 
 * @warning `asm/uart.h`仅提供`AXI UART 16550`相关常量的定义, `UART`相关功能定义在`uart.h`中
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#ifndef __INCLUDE_ASM_UART_H
#define __INCLUDE_ASM_UART_H

/// `AXI UART 16550`芯片的时钟频率, 设置波特率时将根据时钟频率计算除数
#define UART16550_CLOCK 1843200

/* ------------------------------ UART 寄存器 ------------------------------ */
/*
 * 1. RISC-V中采用MMIO方式访问设备寄存器
 * 2. UART 16550 芯片部分寄存器在读写时候的作用不一样, 例如:
 *      - 读取BASE + 0x00寄存器时候表示读取对方发送来的数据
 *      - 写入BASE + 0x00寄存器时候表示向对方发送数据
 */

/**
 * @brief `CLINT`中断控制器的寄存器`MMIO`后的占用内存字节数
 * 
 * @note 该值参考`SiFive FU740`手册`P80`的`Memory Map`
 */
#define UART_MMIO_SIZE                  4096

/// `UART`设备在内存中映射的起始单元
#define UART_BASE_ADDR                  0x10000000

/// `UART`设备在内存中映射的起始单元
#define UART_END_ADDR                   (UART_BASE_ADDR + UART_MMIO_SIZE)

/// 根据UART寄存器的偏移量计算在内存中的地址
#define _UART_REG(offset)                (UART_BASE_ADDR + offset)

/// `UART`的数据寄存器(`Data Register`), 在发送模式和接受模式下有不同的作用
#define UART_DAT                        _UART_REG(0x00)
/// `UART`的发送数据寄存器(`Transmit Holding Register`), `UART`设备处于发送模式时候存储要发送的数据
#define UART_THR                        _UART_REG(0x00)
/// `UART`的接受数据寄存器(`Receiving Holding Register`), `UART`设备处于接收模式时候存储要接受的数据
#define UART_RHR                        _UART_REG(0x00)
/**
 * @brief `UART`的中断使能寄存器(`Interrupt Enable Register`)
 * 
 * @note `16550`支持四种中断, 优先级从高到低依次为:
 * - 接受出错中断
 * - 接受缓冲区满中断
 * - 发送缓冲区空中断
 * - Modem状态变化中断
 * 四种中断源的使能状态由`IER`寄存器中的控制
 */
#define UART_IER                        _UART_REG(0x01)
/**
 * @brief `UART`的中断状态寄存器(`Interrupt Status Register`)
 * 
 * @note `16550`的四种中断源共用一条中断请求线`INTR`向`CPU`申请中断, 因此在`CPU`响应中断之后, 需要判断是那一个中断.
 * 因此`16550`使用`ISR`寄存器表明有无中断和当前最高级别的中断
 */
#define UART_ISR                        _UART_REG(0x02)
/**
 * @brief `UART`的FIFO控制寄存器(`FIFO Control Register`)
 * 
 * @note `UART`设备每次通信发送/接受1bit数据, 因此`16550`内部设置了缓冲区, 将发送/接受的数据保存在缓冲区中, 缓冲区空/满之后再向CPU发送中断.
 *  `16550`的缓冲区最大为16字节, 可以通过设置`FCR`设置使用的缓冲区的大小
 */
#define UART_FCR                        _UART_REG(0x02)
/// `UART`的线路控制寄存器(`Line Control Register`)
#define UART_LCR                        _UART_REG(0x03)
/// `UART`的`Moden`控制寄存器(`Modem Control Register`)
#define UART_MCR                        _UART_REG(0x04)
/// `UART`的线路状态寄存器(`Line Status Register`)
#define UART_LSR                        _UART_REG(0x05)
/// `UART`的`Moden`状态寄存器(`Modem Status Register`)
#define UART_MSR                        _UART_REG(0x06)

/// `UART`的除数寄存器低八位(`Least Significant Bits`, `LSB of Divisor Latch`), 用于进行预分频, 即设置波特率
#define UART_DL_LSB                     _UART_REG(0x00)
/// `UART`的除数寄存器高八位(`Most Significant Bits`, `MSB of Divisor Latch`), 用于进行预分频, 即设置波特率
#define UART_DL_MSB                     _UART_REG(0x01)



/* ------------------------------ UART 线路状态寄存器LCR位标志 ------------------------------ */
/*
 *  参考: TECHNICAL DATA ON 16550, http://byterunner.com/16550.html
*/
/// 传输出错, FIFO Error
#define UART_LSR_ERR                    0b10000000
/// 传输缓冲区(FIFO)和移位寄存器为空, Transmit Empty
#define UART_LSR_TSE                    0b01000000
/// 传输缓冲区(FIFO)为空, Transmit Holding Empty
#define UART_LSR_THE                    0b00100000
/// 传输被打断, Break Interrupt
#define UART_LSR_BKI                    0b00010000
/// 接受帧出错, 没有接收到停止位的帧, Framing Error
#define UART_LSR_FME                    0b00001000
/// 奇偶校验出错, Parity Error
#define UART_LSR_PRE                    0b00000100
/// 数据溢出, Overrun Error
#define UART_LSR_ORE                    0b00000010
/// FIFO有数据, Receive Data Ready
#define UART_LSR_RDR                    0b00000001

#endif