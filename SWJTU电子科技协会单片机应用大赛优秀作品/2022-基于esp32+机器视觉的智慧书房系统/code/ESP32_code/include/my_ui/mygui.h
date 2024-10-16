#ifndef _MY_GUI_H_
#define _MY_GUI_H_

/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <math.h>
#include "..\dev_port\in_dev.h"

lv_obj_t *tabview;
lv_obj_t *tab1;
lv_obj_t *tab2;
lv_obj_t *tab3;
lv_obj_t * label;
lv_obj_t * table;

#define DATE_LABEL_TEXT_LEN 20
#define TIME_LABEL_TEXT_LEN 20


lv_obj_t *date_label; // 全局日期标签对象
lv_obj_t *time_label; // 全局时间标签对象

  static char time_str[TIME_LABEL_TEXT_LEN];
// 更新日期标签和时间标签的文本内容
  static char date_str[DATE_LABEL_TEXT_LEN];

LV_IMG_DECLARE(img_cogwheel_argb);

void lv_ex_tabview_1(void)
{
    //翻页
    /*Create a Tab view object*/
//     创建选项卡视图对象

// 参数
// par -- 指向一个对象的指针，它将是新选项卡的父级

// copy -- 指向选项卡对象的指针，如果不是 NULL，则将从中复制新对象

// 返回
// 指向“已创建”选项卡的指针
    tabview = lv_tabview_create(lv_scr_act(), NULL);///创建选项卡视图对象
    //已活动屏幕作为lv_scr_act()
//可以使用 添加新选项卡。它将返回一个指向 Page 对象的指针，可在其中创建选项卡的内容。
    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    tab1 = lv_tabview_add_tab(tabview, "MEMU");
    tab2 = lv_tabview_add_tab(tabview, "Time");
    tab3 = lv_tabview_add_tab(tabview, "swjtu");

    /*Add content to the tabs*/
    label = lv_label_create(tab1, NULL);
    lv_label_set_text(label, "This the first tab\n"
                             "If the content\n"
                             "of a tab\n");

#define DATE_LABEL_TEXT_LEN 20
#define TIME_LABEL_TEXT_LEN 20

 // 创建主屏幕对象
  lv_obj_t *scr = lv_scr_act();

  // 创建日期标签对象
  date_label = lv_label_create(tab2, NULL);
  lv_label_set_text(date_label, "0000-00-00");
  lv_obj_set_pos(date_label, 0, 0);
  lv_obj_set_size(date_label, 135, 100);
  lv_label_set_align(date_label, LV_LABEL_ALIGN_CENTER);

  // 创建时间标签对象
  time_label = lv_label_create(tab2, NULL);
  lv_label_set_text(time_label, "00:00:00");
  lv_obj_set_pos(time_label, 0, 100);
  lv_obj_set_size(time_label, 135, 100);
  lv_label_set_align(time_label, LV_LABEL_ALIGN_CENTER);







    //----------------------------------------------------------------
    //天气表格
    // table = lv_table_create(tab2, NULL);
    // lv_table_set_col_cnt(table, 2);
    // lv_table_set_row_cnt(table, 7);
    // lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);
    //  lv_table_set_col_width(table, 0, 45);//设置列的宽度
    //  lv_table_set_col_width(table,1, 45);//设置列的宽度
   
    // /*Align the price values to the right in the 2nd column*/
    // lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 1, 1, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 2, 1, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 3, 1, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 4, 1, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 5, 1, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 6, 1, LV_LABEL_ALIGN_CENTER);

    // lv_table_set_cell_align(table, 0, 0, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 1, 0, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 2, 0, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 3, 0, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 4, 0, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 5, 0, LV_LABEL_ALIGN_CENTER);
    // lv_table_set_cell_align(table, 6, 0, LV_LABEL_ALIGN_CENTER);

    // lv_table_set_cell_type(table, 0, 0, 2);
    // lv_table_set_cell_type(table, 0, 1, 2);

    // /*Fill the first column*/
    // lv_table_set_cell_value(table, 0, 0, "Year");
    // lv_table_set_cell_value(table, 1, 0, "Month");
    // lv_table_set_cell_value(table, 2, 0, "Day");
    // lv_table_set_cell_value(table, 3, 0, "Week");
    // lv_table_set_cell_value(table, 4, 0, "Hour");
    // lv_table_set_cell_value(table, 5, 0, "Minute");
    // lv_table_set_cell_value(table, 6, 0, "Second");

    // /*Fill the second column*/
    // lv_table_set_cell_value(table, 0, 1, "00");
    // lv_table_set_cell_value(table, 1, 1, "00");
    // lv_table_set_cell_value(table, 2, 1, "00");
    // lv_table_set_cell_value(table, 3, 1, "00");
    // lv_table_set_cell_value(table, 4, 1, "00");
    // lv_table_set_cell_value(table, 5, 1, "00");
    // lv_table_set_cell_value(table, 6, 1, "00");

    //-----------------------------------------------------------
    //图片
    lv_obj_t * img1 = lv_img_create(tab3, NULL);
    lv_img_set_src(img1, &img_cogwheel_argb);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t * img2 = lv_img_create(tab3, NULL);
    lv_img_set_src(img2, LV_SYMBOL_OK "Accept"); 
    lv_obj_align(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
   //按钮
    lv_group_t * group = lv_group_create();
    lv_group_add_obj(group, tabview);
    //lv_group_add_obj(group, tab1);
    //lv_group_add_obj(group, tab2);
    //lv_group_add_obj(group, tab3);
    lv_indev_set_group(indev_encoder, group);
}

#endif /*LV_TEST_CHART_H*/
