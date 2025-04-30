/**
 * @file stm32f4_button.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the BUTTON port layer connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-01
 *
 */

#ifndef STM32F4_BUTTON_H
#define STM32F4_BUTTON_H

/* Includes ------------------------------------------------------------------*/
/* HW dependent includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

// HW Nucleo-STM32F446RE:
#define STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PORT GPIOC          /*!< GPIO port of the BUTTON in the Nucleo board @hideinitializer */
#define STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PIN GPIO_PIN_13     /*!< GPIO pin of the BUTTON in the Nucleo board @hideinitializer */
#define STM32F4_BUTTON_AUTOMATIC_DOOR_EXTI_IRQn EXTI15_10_IRQn /*!< EXTI IRQn of the BUTTON in the Nucleo board @hideinitializer */
#define STM32F4_BUTTON_AUTOMATIC_DOOR_IRQ_PRIO 3               /*!< IRQ priority of the BUTTON in the Nucleo board @hideinitializer */
#define STM32F4_BUTTON_AUTOMATIC_DOOR_IRQ_SUBPRIO 0            /*!< IRQ subpriority of the BUTTON in the Nucleo board @hideinitializer */

#endif /* STM32F4_BUTTON_H */