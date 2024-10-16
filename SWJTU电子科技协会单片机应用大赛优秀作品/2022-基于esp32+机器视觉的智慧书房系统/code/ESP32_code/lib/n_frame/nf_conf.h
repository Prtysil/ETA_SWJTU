#ifndef NF_CONF_H
#define NF_CONF_H


/* 默认开启调试模式，发布版本请定义NF_NDEBUG宏 */
/* #define NF_NDEBUG */


/* 同时存在最大信号数量 */
/* 使用数组静态分配，每个信号占1字节空间 */
#define NF_MAX_SIGNAL_NUM (50U)

#endif