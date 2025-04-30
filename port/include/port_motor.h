/**
 * @file port_motor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the port layer of a motor connected to an STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-16
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
#define PORT_MOTOR_AUTOMATIC_DOOR_ID 0 /*!< Motor identifier @hideinitializer */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Initializes the motor.
 *
 * @param motor_id Motor ID.
 */
void port_motor_init(uint32_t motor_id);

/**
 * @brief Activate the motor timer to start or stop the countdown.
 *
 * @param motor_id Motor ID.
 * @param timeout_ms Time in milliseconds.
 */
void port_motor_timeout_timer_activate(uint32_t motor_id, uint32_t timeout_ms);

/**
 * @brief Deactivate the timeout timer.
 *
 * @param motor_id Motor ID.
 */
void port_motor_timeout_timer_deactivate(uint32_t motor_id);

/**
 * @brief Set the status of the timer if has finished or not.
 *
 * @param motor_id Motor ID.
 * @param timeout Status of the timeout.
 */
void port_motor_set_timeout_status(uint32_t motor_id, bool timeout);

/**
 * @brief Get the status of the timeout.
 *
 * @param motor_id Motor ID.
 * @return true If the timeout has finished
 * @return false If the timeout has not finished
 */
bool port_motor_get_timeout_status(uint32_t motor_id);

#endif /* PORT_MOTOR_H */