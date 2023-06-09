/**
 * @file test_string.c
 * @author Shihong Wang (jack4shihong@gmail.com)
 * @brief `test_string.h`是`string`的测试文件
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright Shihong Wang (c) 2023 with GNU Public License V3.0
 */

#include "kernel/kstdio.h"
#include "test/test_string.h"

// 声明外部符号, 避免include
extern void uart_puts(const char*);


int test_string(void){
    uart_puts("=> "), uart_puts(__func__), uart_puts(": \n");
    char s[1024]="123456789qwertyuiop@#$$&*())\0";
    char t[1024]="eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\0";
    char* dst = "ffffffffffffffffffffffff\0"; 
    char* src = "tttttttttttttttttttttttt\0";

    size_t lm;
    lm=memset(s,'@',19);
    kprintf("\ttest memset within bound:%s:return val=%d;\n",s,lm);
    memcpy(t,s,strlen(s)/2);
    kprintf("\ttest memcpy within bound%s;no return val\n",t);
    kprintf("\ttest memcmp within bound%d\n",memcmp(s,t,20));
    kprintf("\ttest strcpy wthin bound %s;\n",strcpy(s,t));
    int8_t test5 = memcmp("abbbbbbbbbc","abbbbbbbbbd",6);
    kprintf("\ttest memcmp,return num:%d \n",test5);
    strcat(dst,src);
    kprintf("\ttest strcat:%s\n",dst);
    //test strchr
    kprintf("\ttest strchr:%s\n",strchr("abbc",'b'));
    //test strrchr
    kprintf("\ttest strrchr:%s\n",strrchr("abbc\0",'b'));
    //test strchrs
    size_t test6 = strchrs("abbbbc",'b');
    kprintf("\ttest strchrs:%d\n",test6);


    char *t1="#############################3333333########\0";
    char *t2="@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\0";
    char *t3="\0";
    lm=strmset(t1,'$',10);
    kprintf("\ttest strmset within bound:%s:return val=%d;\n",t1,lm);
    lm=strmset(t2,'$',10000);
    kprintf("\ttest strmset out of bound:%s:return val=%d;\n",t1,lm);
    lm=strmset(t1,'@',10);
    lm=strslice(t1,t3,3,20);
    kprintf("\ttest strslice within bound:%s:return val=%d;\n",t3,lm);
    lm=strslice(t1,t3,3,200000);
    kprintf("\ttest strslice within bound:%s:return val=%d;\n",t3,lm);

    encrypt_decrypt(t2,strlen(t2)-3,'T');
    kprintf("\ttest encrypt_decrypt in coding thr string : after coding:%s\n",t2);
    encrypt_decrypt(t2,strlen(t2)-3,'T');
    kprintf("\ttest encrypt_decrypt in break-coding thr string : after break-coding:%s\n",t2);
    char* test="abcmmm";
    kprintf("\ttest memmove ,return value: %d\n",memmove(test,"def\n",3));

    // char str[2048] = "Hello X2W-OS! Hello our operating system!";
    // kprintf("\tTesting Regular Expression Matching and Replacement:\n\tOriginal string: %s\n", str);
    // regu_replace(str, "Hello", "Hi", 2048);
    // kprintf("\tTesting Regular Expression Matching and Replacement:\n\tReplaced string: %s\n", str);
    
    return -1;
}