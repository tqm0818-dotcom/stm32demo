#include "gpio.h"
#include "led.h"
#include "main.h"

/*
 * Function: LED_Status
 * Description: Sets the status of the specified LED
 * Parameters:
 *   GPIOx: Pointer to the GPIO port
 *   GPIO_Pin: The pin number of the LED
 *   PinState: The state to set the pin (GPIO_PIN_SET or GPIO_PIN_RESET)
 */
void LED_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{  
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

