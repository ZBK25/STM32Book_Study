#include "key_led.h"

/**
 * @brief 轮询方式扫描按键
 * @param timeout 扫描超时时间，若timeout=0表示一直扫描
 * @return 按键值
 */
KEYS ScanPressedKey(uint32_t timeout)
{
    KEYS key = KEY_NONE;
    uint32_t tickstart = HAL_GetTick();     //当前滴答计数值
    const uint16_t btnDelay = 20;           //按键按下阶段的抖动，延时在采样时间
    GPIO_PinState keyState;                 //引脚输入状态
    while (1)
    {
        keyState = HAL_GPIO_ReadPin(LED_GPIO_Port,LED_Pin);
        if (keyState == GPIO_PIN_RESET)
        {
            HAL_Delay(btnDelay);
            keyState = HAL_GPIO_ReadPin(LED_GPIO_Port,LED_Pin);
            if (keyState == GPIO_PIN_RESET)
            {
                return KEY_DOWN;
            }
        }

        if (timeout != KEY_WAIT_ALWAYS)
        {
            if ((HAL_GetTick() - tickstart) > timeout)
            {
                break;
            }
        }
    }
    return key;
}