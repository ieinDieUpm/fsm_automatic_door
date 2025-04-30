/**
 * @file stm32f4_pir_sensor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the PIR SENSOR port layer connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-01
 *
 */

#ifndef STM32F4_PIR_SENSOR_H
#define STM32F4_PIR_SENSOR_H

/* Includes ------------------------------------------------------------------*/
/* HW dependent includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PORT GPIOB /*!< GPIO port of the PIR sensor in the Nucleo board */
#define STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PIN GPIO_PIN_10     /*!< GPIO pin of the PIR sensor in the Nucleo board */
#define STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_EXTI_IRQn EXTI15_10_IRQn /*!< EXTI IRQn of the PIR sensor in the Nucleo board */
#define STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_IRQ_PRIO 1               /*!< IRQ priority of the PIR sensor in the Nucleo board */
#define STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_IRQ_SUBPRIO 0            /*!< IRQ subpriority of the PIR sensor in the Nucleo board */

#endif /* STM32F4_PIR_SENSOR_H */