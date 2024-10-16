#ifndef _DEMO1_H_
#define _DEMO1_H_

/**********************
 *      TYPEDEFS
 **********************/

/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>

lv_obj_t * gauge1;

static void gauge1_adjust(lv_task_t *t)
{
	static uint8_t value = 0;
	static uint8_t dir = 1;
	if (dir)
		value++;
	else
		value--;
	if (value >= 12)
		dir = 0;
	if (value == 0)
		dir = 1;
	lv_gauge_set_value(gauge1, 0, value);
}

void lv_ex_gauge_1(void)
{
    //设置指针颜色
    static lv_color_t needle_colors[3];
    needle_colors[0] = LV_COLOR_BLUE;
    needle_colors[1] = LV_COLOR_ORANGE;
    needle_colors[2] = LV_COLOR_PURPLE;

    //控件设置
    gauge1 = lv_gauge_create(lv_scr_act(), NULL);

    lv_gauge_set_needle_count(gauge1, 3, needle_colors);//指定指针的数量，为每根指针设置颜色。
    lv_gauge_set_scale(gauge1, 330, 23, 12);//刻度和标签
    lv_gauge_set_range(gauge1, 1, 12);//指定其最大值和最小值。
    lv_gauge_set_critical_value(gauge1, 10);//设置临界值

    lv_obj_set_size(gauge1, 240, 240);//尺寸
    lv_obj_align(gauge1, NULL, LV_ALIGN_CENTER, 0, 0);//显示位置及偏移量

    //设置指针值
    lv_gauge_set_value(gauge1, 0, 3);
    lv_gauge_set_value(gauge1, 1, 4);
    lv_gauge_set_value(gauge1, 2, 5);

    lv_task_create(gauge1_adjust, 1000, LV_TASK_PRIO_LOW, gauge1);	/* 创建任务刷新gauge指针的值 */
}

#endif