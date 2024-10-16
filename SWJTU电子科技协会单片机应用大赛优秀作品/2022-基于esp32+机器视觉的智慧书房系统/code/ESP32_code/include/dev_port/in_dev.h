//输入驱动
#ifndef _IN_DEV_H_
#define _IN_DEV_H_

#define Encoder_DEV //使用编码器输入
//#define Button_DEV  //使用按键输入

#include <lvgl.h>
#include "mywatch.h"
#include "..\my_ui\mygui.h"

extern lv_obj_t * label;

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;
lv_indev_t * indev_mouse;
lv_indev_t * indev_keypad;
lv_indev_t * indev_encoder;
lv_indev_t * indev_button;

static int32_t encoder_diff = 0;

void handleInterruptA(void);
void handleInterruptB(void);
static void encoder_init(void);
static bool encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

static void button_init(void);
static bool button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;
    /* Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */
#ifdef Encoder_DEV
    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

    /* Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     * add objects to the group with `lv_group_add_obj(group, obj)`
     * and assign this input device to group to navigate in it:
     * `lv_indev_set_group(indev_encoder, group);` */
#endif

#ifdef Button_DEV
    /*------------------
     * Button
     * -----------------*/

    /*Initialize your button if you have*/
    button_init();

    /*Register a button input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.read_cb = button_read;
    indev_button = lv_indev_drv_register(&indev_drv);

    /*Assign buttons to points on the screen*/
    static const lv_point_t btn_points[3] = {
            {30, 20},   /*Button 0 -> x:10; y:10*/
            {120, 20},  /*Button 1 -> x:40; y:100*/
            {210, 20},  /*Button 1 -> x:40; y:100*/
    };
    lv_indev_set_button_points(indev_button, btn_points);
#endif

}

/*------------------
 * Encoder
 * -----------------*/

/* Initialize your keypad */
static void encoder_init(void)
{
  //按键配置
  pinMode(UP_KEY,INPUT_PULLUP);
  pinMode(M_KEY,INPUT_PULLUP);
  pinMode(DOWN_KEY,INPUT_PULLUP);
}

void handleInterruptA() {
  delay(3);  //延时20ms作为消抖，如果是很稳定的中断可以不加或者加很少的消抖时间
  up_pres_time = millis();
  if(KEYU == LOW)    //因为是下拉触发，所以在消抖时间完后读取引脚高低电平，如果还是为低那么就代表出现了一次稳定的中断
    encoder_diff++;
  while(KEYU==LOW)
  {
    if(millis()-up_pres_time>key_long_pres_time)
      return;
  }
}

void handleInterruptB() {
  delay(3);  //延时20ms作为消抖，如果是很稳定的中断可以不加或者加很少的消抖时间
  down_pres_time = millis();
  if(KEYD == LOW)    //因为是下拉触发，所以在消抖时间完后读取引脚高低电平，如果还是为低那么就代表出现了一次稳定的中断
    encoder_diff--;
  while(KEYD==LOW)
  {
    if(millis()-down_pres_time>key_long_pres_time)
      return;
  }
}

/* Will be called by the library to read the encoder */
static bool encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    handleInterruptA();
    handleInterruptB();

    static int32_t last_diff = 0;
    static int enc_diff_buff = 0;
    int32_t diff = encoder_diff;

    data->state = !KEYM;

    data->enc_diff = diff - last_diff;

    if(data->enc_diff != 0)
    {
      Serial.printf("-----------------cotter_d is %d------------------\n",data->enc_diff);
      enc_diff_buff = data->enc_diff;
    }

    char date[128]; 
    sprintf(date, "cotter_d : %d\ncotter : %d\nbtn state : %d\n",enc_diff_buff,encoder_diff,!KEYM);
    lv_label_set_text(label, date);

    last_diff = diff;

    return false;
}

/*------------------
 * Button
 * -----------------*/

/* Initialize your buttons */
static void button_init(void)
{
  //按键配置
  pinMode(UP_KEY,INPUT_PULLUP);
  pinMode(M_KEY,INPUT_PULLUP);
  pinMode(DOWN_KEY,INPUT_PULLUP);
}

/* Will be called by the library to read the button */
static bool button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    static uint8_t last_btn = 0;

    /*Get the pressed button's ID*/
    int8_t btn_act = button_get_pressed_id();

    if(btn_act >= 0) {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    uint8_t i;

    /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    for(i = 0; i < 3; i++) {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i)) {
            return i;
        }
    }

    /*No button pressed*/
    return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{
    /*Your code comes here*/
    bool button_state;

    switch (id)
    {
    case 0:
      button_state = KEYU;
      break;
    case 1:
      button_state = KEYM;
      break;
    case 2:
      button_state = KEYD;
      break;
    default:
      return 0;
      break;
    }
    
    button_state = !button_state;
    return button_state;
}

#endif