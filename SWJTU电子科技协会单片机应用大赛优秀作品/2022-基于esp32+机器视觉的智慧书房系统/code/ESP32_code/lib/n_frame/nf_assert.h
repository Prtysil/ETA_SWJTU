#ifndef NF_ASSERT_H
#define NF_ASSERT_H

#include "H:\GeXingHuaLAB_TeacherPAN\ESP32_v2.x\ESP32_LVGL_V7.1.x\lib\n_frame\nf_type.h"

#ifdef NF_NDEBUG
#define NF_ASSERT(_exp) ((void)0)
#else
#define NF_ASSERT(_exp) ((_exp) ? (void)0 : NF_Assert_Failed((const char *)__FILE__, __LINE__))
extern void NF_Assert_Failed(const char* file, NF_Int32U line);
#endif

#endif