#ifndef KEY_LED
#define KEY_LED

#include "main.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
//按键枚举类型
typedef enum {
    KEY_NONE = 0,
    KEY_DOWN
} KEYS;

#define KEY_WAIT_ALWAYS 0       //按键一直等待，作为ScanPressedKey()的一种参数

#define LED_Toggle() HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin)           //输出翻转
#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET) //点亮LED
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)  //熄灭LED

KEYS ScanPressedKey(uint32_t timeout);

#endif
