#ifndef FSM_FSM_H

#define FSM_FSM_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "nf_fsm.h"

// #include <H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\n_frame.h>
/* 状态机对象声明 */
extern NF_FSM FSM_fsm;

// /* 转换执行的外部函数声明 */
extern void Light_OFF(void);
extern void LED_adc_work(void);
extern void LED_adc_keep(void);
extern void Light_ON(void);

void FSM_fsm_Light_mode_keep(NF_FSM* me, NF_Event event);
void FSM_fsm_Light_mode_change(NF_FSM* me, NF_Event event);
void FSM_fsm_Light_mode_OFF(NF_FSM* me, NF_Event event);
void FSM_fsm_Light_mode_ON(NF_FSM* me, NF_Event event);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 

