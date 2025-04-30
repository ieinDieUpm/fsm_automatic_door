/**
 * @file fsm_automatic_door.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the automatic door FSM.
 * @date 2024-04-01
 *
 */

#ifndef FSM_AUTOMATIC_DOOR_H
#define FSM_AUTOMATIC_DOOR_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include <fsm.h>
#include "port_button.h"
#include "port_led.h"
#include "port_pir_sensor.h"
#include "port_motor.h"

/* Defines and enums ----------------------------------------------------------*/
#define AUTOMATIC_DOOR_OPENING_CLOSING_TIMEOUT_MS  /*!< Timeout for the automatic door to open or close @hideinitializer */
#define AUTOMATIC_DOOR_INACTIVITY_TIMEOUT_MS       /*!< Timeout for the automatic door to leave the door open or closed @hideinitializer */

/* Enums */
/**
 * @brief Enumerates the states of the automatic door FSM.
 *
 */
enum FSM_AUTOMATIC_DOOR_STATES
{
    CLOSED = 0, /*!< The door is closed */
    OPENING,    /*!< The door is opening */
    OPEN,       /*!< The door is open */
    CLOSING     /*!< The door is closing */
};

/* Typedefs ------------------------------------------------------------------*/
typedef struct fsm_automatic_door_t fsm_automatic_door_t; /*!< Forward declaration of the FSM structure */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Creates a new automatic door FSM.
 *
 * @param p_button Pointer to the button of the automatic door.
 * @param p_led_open Pointer to the opening LED of the automatic door.
 * @param p_led_close Pointer to the closing LED of the automatic door.
 * @param p_pir Pointer to the PIR sensor of the automatic door.
 * @param p_motor Pointer to the motor of the automatic door.
 * @return fsm_automatic_door_t* Pointer to the new automatic door FSM.
 */
fsm_automatic_door_t *fsm_automatic_door_new(uint32_t button_id, uint32_t led_opening_id, uint32_t led_closing_id, uint32_t pir_sensor_id, uint32_t motor_id);

/**
 * @brief Gets the last time a presence was detected.
 *
 * @param p_this Pointer to the FSM structure of the automatic door.
 */
uint32_t fsm_automatic_door_get_last_time_presence(fsm_automatic_door_t *p_this);

/**
 * @brief Gets the presence status of the door.
 *
 * @param p_this Pointer to the FSM structure of the automatic door.
 */
bool fsm_automatic_door_get_presence_status(fsm_automatic_door_t *p_this);

/**
 * @brief Fires the FSM of the automatic door.
 * 
 * @param p_fsm Pointer to the FSM structure of the automatic door. 
 */
void fsm_automatic_door_fire(fsm_automatic_door_t *p_fsm);

#endif /* FSM_AUTOMATIC_DOOR_H */