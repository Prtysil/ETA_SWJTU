#ifndef NF_SIGNAL_H
#define NF_SIGNAL_H

#include <nf_type.h>
#include <nf_conf.h>
// #include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_type.h"
// #include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_conf.h"

/* 信号值类型 */
/* 可按需求修改数据类型 */
typedef NF_Int8U NF_SignalValue;

/* 信号名类型 */
typedef NF_CString NF_SignalName;

/* 信号ID */
typedef NF_ID NF_SignalID;

/* 接口函数列表 */
extern NF_SignalID    NF_Signal_Set   (NF_SignalName name, NF_SignalValue val); // 通过字符串名称给信号赋值，如信号不存在则创建，时间O(n)
extern void           NF_Signal_SetID (NF_SignalID id, NF_SignalValue val);     // 通过ID给信号赋值，如信号不存在则无效，时间O(1)
extern NF_SignalValue NF_Signal_Get   (NF_SignalName name); // 通过字符串名称获得信号值，如信号不存在返回0，时间O(n)
extern NF_SignalValue NF_Signal_GetID (NF_SignalID id);     // 通过ID获得信号值，如信号不存在返回0，时间O(1)


#endif