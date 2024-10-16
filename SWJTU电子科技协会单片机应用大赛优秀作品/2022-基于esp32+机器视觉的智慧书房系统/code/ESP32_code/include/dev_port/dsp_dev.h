//屏幕驱动

#ifndef _DSP_DEV_H_
#define _DSP_DEV_H_

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "mywatch.h"

//LCD屏幕初始化
TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

//屏幕数据缓存数组，大小与屏幕宽长有关
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

//主界面LVGL对象，以及屏幕宽长
lv_obj_t * slider_label;

static void disp_init(void);
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

/* Initialize your display and the required peripherals. */
static void disp_init(void)
{
    //LCD屏幕初始化
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_WHITE);
    tft.setTextSize(1);
    tft.setTextColor(TFT_MAGENTA);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
    tft.setSwapBytes(true);
    //显示屏背光
    ledcSetup(10, 5000/*freq*/, 10 /*resolution*/);
    ledcAttachPin(TFT_BL, 10);
    analogReadResolution(10);
    ledcWrite(10,512);
}

void lv_port_disp_init(void)
{
    disp_init();
    //下面为驱动接口
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
    /*初始化显示*/
    lv_disp_drv_t disp_drv;
    disp_drv.rotated = 1;//让LVGL知道垂直旋转啦
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenHeight;
    disp_drv.ver_res = screenWidth;
    //disp_drv.hor_res = screenWidth;
    //disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}

/* Display flushing *//*显示驱动*/
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;
  static int dis_cotter=0;

  tft.startWrite(); /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite(); /* terminate TFT transaction */

 //yjt Serial.printf("刷新显示中：%d \n\r",dis_cotter++);
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

#endif