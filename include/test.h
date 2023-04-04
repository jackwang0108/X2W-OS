/**
 * @file test.h
 * @author Jack Wang (jackwang0108@gmail.com)
 * @brief test if documentation system works normal
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef __TEST_H
#define __TEST_H

/**
 * @brief 人的结构体
 */
typedef struct __person_t {
    /// 人的名字
    char *name;
    /// 人的年龄
    int age;
    /// 人的体重
    float weight;
};


/**
 * @brief 计算deg角度的sin值并将结果存入*res中
 * 
 * @param deg 要计算的角度值，弧度制或者角度值均可
 * @param res 存储结果的变量
 * @return int 若计算成功则返回0, 否则返回-1
 */
int sin(char *deg, float *res);



#endif