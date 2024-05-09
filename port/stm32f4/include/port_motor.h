/**
 * @file port_motor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the port layer of a motor connected to an STM32F4 Nucleo board.
 * @version 0.1
 * @date 2024-05-05
 *
 */
#ifndef PORT_MOTOR_H
#define PORT_MOTOR_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define MOTOR_AUTOMATIC_DOOR_GPIO NULL          /*!< TO-DO: GPIO port of the motor of the automatic door */
#define MOTOR_AUTOMATIC_DOOR_PIN 0              /*!< TO-DO: GPIO pin of the motor of the automatic door */
#define MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER TIM2 /*!< Timer to control the timeout of the automatic door */

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a motor.
 */
typedef struct
{
    GPIO_TypeDef *p_port;         /*!< GPIO where the LED is connected */
    uint8_t pin;                  /*!< Pin/line where the LED is connected */
    TIM_TypeDef *p_timer_timeout; /*!< Timer to control the timeout of the motor */
    bool timeout;                 /*!< Timeout status */
    // TO-DO: Add timer to control the PWM of the motor
} port_motor_hw_t;

/* Global variables -----------------------------------------------------------*/
extern port_motor_hw_t motor_automatic_door; /*!< Motor for the automatic door. Public for access to interrupt handlers. */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Initializes the motor.
 *
 * @param p_motor Pointer to the motor structure.
 */
void port_motor_init(port_motor_hw_t *p_motor);

/**
 * @brief Activate the motor timer to start or stop the countdown.
 *
 * @param p_motor Pointer to the motor structure.
 * @param timeout_ms Time in milliseconds.
 */
void port_motor_timeout_timer_activate(port_motor_hw_t *p_motor, uint32_t timeout_ms);

/**
 * @brief Deactivate the timeout timer.
 *
 * @param p_motor Pointer to the motor structure.
 */
void port_motor_timeout_timer_deactivate(port_motor_hw_t *p_motor);

/**
 * @brief Set the status of the timer if has finished or not.
 *
 * @param p_motor Pointer to the motor structure.
 * @param timeout Status of the timeout.
 */
void port_motor_set_timeout_status(port_motor_hw_t *p_motor, bool timeout);

#endif /* PORT_MOTOR_H */