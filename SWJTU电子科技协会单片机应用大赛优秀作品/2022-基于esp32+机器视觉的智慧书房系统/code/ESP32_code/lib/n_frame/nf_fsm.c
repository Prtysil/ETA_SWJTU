#include <nf_fsm.h>
#include <nf_assert.h>

// #include "string.h"
// #include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_fsm.h"
// #include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_assert.h"

#include "string.h"
/* 将事件派发给状态机，执行转换函数 */
/* 备注:不支持同一状态机同时在中断和主函数中派发 */
/* 同一个状态机，Dispatch函数不能互相打断 */
void NF_FSM_Dispatch(NF_FSM* me, NF_Event event)
{
	/* 对象指针不可为空 */
	NF_ASSERT( me != NF_NULL_PTR );

	me->State.Dispatch(me, event);
}

/* 语法糖，用于事件赋值 */
NF_Event NF_FSM_Event(NF_Event_Name name)
{
	NF_Event event;

	/* 函数指针不可为空 */
	NF_ASSERT(name != NF_NULL_PTR);

	event.Name = name;
	return event;
}

/* 转换状态机状态 */
void NF_FSM_Translate(NF_FSM* me, NF_State state) 
{
    /* 对象指针不可为空 */
    NF_ASSERT( me != NF_NULL_PTR );

    me->State = state;
}   

/* 语法糖，用于状态赋值 */
NF_State NF_FSM_State(NF_FSM_Handler handler)
{
	NF_State state;
	
	/* 函数指针不可为空 */
	NF_ASSERT(handler != NF_NULL_PTR);

	state.Dispatch = handler;
	return state;
}

/* 语法糖，用于判断状态名 */
NF_Bool NF_FSM_NameIs(NF_FSM_Name name1, NF_FSM_Name name2)
{
	/* 名称指针不可为空 */
	NF_ASSERT( (name1 != NF_NULL_PTR) && (name2 != NF_NULL_PTR) );

	if ( strcmp(name1, name2) == 0 )
	{
		return NF_Bool_True;
	} 
	else
	{
		return NF_Bool_False;
	}
}
