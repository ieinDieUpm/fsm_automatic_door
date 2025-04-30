/**
 * @file stm32f4_led.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the LED port layer connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-01
 *
 */

#ifndef STM32F4_LED_H_
#define STM32F4_LED_H_

/* Includes ------------------------------------------------------------------*/
/* HW dependent includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define STM32F4_LED_OPENING_GPIO_PORT         /*!< GPIO port of the LED for opening in the automatic door @hideinitializer */
#define STM32F4_LED_OPENING_PIN_PORT          /*!< GPIO pin of the LED for opening in the automatic door @hideinitializer */
#define STM32F4_LED_CLOSING_GPIO_PORT         /*!< GPIO port of the LED for closing in the automatic door @hideinitializer */
#define STM32F4_LED_CLOSING_PIN_PORT          /*!< GPIO pin of the LED for closing in the automatic door @hideinitializer */
#define STM32F4_LED_OPENING_TIMER             /*!< Timer to control the blinking of the opening LED @hideinitializer */
#define STM32F4_LED_OPENING_TIMER_IRQ         /*!< Timer IRQn of the opening LED @hideinitializer */
#define STM32F4_LED_OPENING_TIMER_IRQ_PRIO    /*!< IRQ priority of the opening LED @hideinitializer */
#define STM32F4_LED_OPENING_TIMER_IRQ_SUBPRIO /*!< IRQ subpriority of the opening LED @hideinitializer */
#define STM32F4_LED_CLOSING_TIMER TIM4        /*!< Timer to control the blinking of the closing LED @hideinitializer */
#define STM32F4_LED_CLOSING_TIMER_IRQ         /*!< Timer IRQn of the closing LED @hideinitializer */
#define STM32F4_LED_CLOSING_TIMER_IRQ_PRIO    /*!< IRQ priority of the closing LED @hideinitializer */
#define STM32F4_LED_CLOSING_TIMER_IRQ_SUBPRIO /*!< IRQ subpriority of the closing LED @hideinitializer */

/**
 * @brief Get the timer handler for a given LED.
 *
 * @param led_id  LED ID.
 * @return TIM_HandleTypeDef* Pointer to the timer handler for the LED.
 */
TIM_HandleTypeDef *stm32f4_get_led_timer_handler(uint32_t led_id);

#endif /* STM32F4_LED_H */
