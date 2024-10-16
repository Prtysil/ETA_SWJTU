#ifndef NF_FSM_H
#define NF_FSM_H

#include <nf_type.h>
#include <nf_signal.h>

// #include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_type.h"
// #include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_signal.h"

/* 名称类型 */
typedef NF_CString NF_FSM_Name;
typedef NF_CString NF_Event_Name;

/* 事件对象类型 */
typedef struct _nf_event
{
    NF_Event_Name Name;
}NF_Event;

/* 状态对象类型 */
typedef struct _nf_state_machine NF_FSM;
/* 既是状态变量，又是状态转换处理回调函数 */
typedef void (*NF_FSM_Handler)(NF_FSM* me, NF_Event event);
typedef struct _nf_state
{
	NF_FSM_Handler Dispatch;
}NF_State;

/* 状态机对象类型 */
typedef struct _nf_state_machine
{
    NF_State State;
}NF_FSM;

/* 状态转换宏 */
#define NF_FSM_TRAN(_sta) NF_FSM_Translate(me, NF_FSM_State(_sta))
 
/* 接口函数列表 */
extern void     NF_FSM_Dispatch  (NF_FSM* me, NF_Event event);           // 将事件派发给状态机，同一状态机的Dispatch函数间不能互相打断
extern void     NF_FSM_Translate (NF_FSM* me, NF_State state);           // 转换状态机状态
extern NF_Event NF_FSM_Event     (NF_Event_Name name);                   // 语法糖，用于事件赋值
extern NF_State NF_FSM_State     (NF_FSM_Handler handler);               // 语法糖，用于状态赋值
extern NF_Bool  NF_FSM_NameIs    (NF_FSM_Name name1, NF_FSM_Name name2); // 语法糖，用于判断状态名

#endif