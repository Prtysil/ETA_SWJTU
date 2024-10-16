#ifndef NF_TYPE_H
#define NF_TYPE_H

/* 这里使用stdint.h头文件进行整数类型定义 */
#include <stdint.h>



/* 跨平台数据类型 */
/* 移植时请替换为本平台基础类型 */
typedef uint8_t  NF_Int8U;
typedef uint16_t NF_Int16U;
typedef uint32_t NF_Int32U;

typedef int8_t   NF_Int8S;
typedef int16_t  NF_Int16S;
typedef int32_t  NF_Int32S;

/* ID号类型,可按需求修改 */
typedef NF_Int32U NF_ID;

/* C常量字符串 */
typedef const char* NF_CString;

/* 布尔类型 */
typedef enum _nf_bool
{
	NF_Bool_True = 1,
	NF_Bool_False = 0
}NF_Bool;

/* 空指针 */
#define NF_NULL_PTR ((void*)0)

#endif