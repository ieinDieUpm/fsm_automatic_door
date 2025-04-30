/**
 * @file stm32f4_motor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the motor port layer connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-16
 *
 */

#ifndef STM32F4_MOTOR_H
#define STM32F4_MOTOR_H

/* Includes ------------------------------------------------------------------*/
/* HW dependent includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define STM32F4_MOTOR_AUTOMATIC_DOOR_GPIO_PORT NULL              /*!< TO-DO: GPIO port of the motor of the automatic door */
#define STM32F4_MOTOR_AUTOMATIC_DOOR_PIN GPIO_PIN_0              /*!< TO-DO: GPIO pin of the motor of the automatic door */
#define STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER TIM2          /*!< Timer to control the timeout of the automatic door */
#define STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER_IRQ TIM2_IRQn /*!< Timer IRQn of the automatic door */
#define STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER_IRQ_PRIO 2    /*!< IRQ priority of the automatic door */
#define STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER_IRQ_SUBPRIO 0 /*!< IRQ subpriority of the automatic door */

/**
 * @brief Get the timer handler for a given motor. 
 * 
 * @param motor_id  Motor ID.
 * @return TIM_HandleTypeDef* Pointer to the timer handler for the motor. 
 */
TIM_HandleTypeDef *stm32f4_get_motor_timer_handler(uint32_t motor_id);

#endif /* STM32F4_MOTOR_H */