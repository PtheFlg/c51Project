#include<REGX52.H>
#include "Delay.h"
#include "Nixie.h"

/**
 * 多模块编程
 * 
 * 1. #ifndef #endif #ifdef ...
 *      这个东西直接影响编译，其实就一种编译选择器，根据条件选择下面内容是否编译，比如：
 *      #ifndef 就是 if not define，如果 #ifndef 后面的东西还没有被定义，则截止到 #endif 的内容会被编译
 *      #ifdef 后面的东西如果被定义，则截止 #endif 里面的内容被编译，
 *      看代码里的演示
 * 
 * 2. 多模块一般来说，弄一个 .h 头文件，里面写模块内函数的定义，只有定义，没有实现
 *      再创建一个同名的 .c 文件，里面直接写函数的实现，不需要 #include "前一个头文件.h"
 *      
 * 3. 头文件的格式
 *      头文件前两行一般来说是 #ifndef __XXX_H__(文件名) \n #define __XXX_H__ \n 末尾来一个 #endif
 *      一上来，如果没有定义 __XXX_H__ ，才会编译下面的头文件内容，
 *      假如项目目录比较乱，里面已经有一个 xxx.h 了，我又写了一个，通过 #ifndef 就可以避免头文件冲突，重复编译
 * 
 * 4. c 文件内不可以直接使用 h 文件里的宏定义
 *      比如 h 文件里面 #define uchar unsigned char，对应 c 文件里不可以直接使用 uchar，需要再定义一次
 */

void main() {

    while(1) {
        nixie(8, 2);
        nixie(7, 5);
        nixie(6, 13);
    }

    /**
     * 这里 #ifdef AAA，如果 AAA 被定义，则里面的内容参与编译
     * 里面是乱码，正常情况下肯定不会编译通过，但是编译一下看看，没有问题
     */
    #ifdef AAA

    sldfjksldkjflskdf

    #endif


}