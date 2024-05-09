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

/* Other includes */
#include <fsm.h>
#include "port_button.h"
#include "port_led.h"
#include "port_pir_sensor.h"
#include "port_motor.h"

/* Defines and enums ----------------------------------------------------------*/
#define AUTOMATIC_DOOR_OPENING_CLOSING_TIMEOUT_MS  /*!< Timeout for the automatic door to open or close */
#define AUTOMATIC_DOOR_INACTIVITY_TIMEOUT_MS      /*!< Timeout for the automatic door to leave the door open or closed */

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
/**
 * @brief Structure to define the automatic door FSM.
 */
typedef struct
{
    fsm_t f;                               /*!< FSM structure. Important to be the first element of the structure */
    port_button_hw_t *p_button;            /*!< Pointer to the button structure */
    port_led_hw_t *p_led_open;             /*!< Pointer to the opening LED structure */
    port_led_hw_t *p_led_close;            /*!< Pointer to the closing LED structure */
    port_pir_hw_t *p_pir_sensor;           /*!< Pointer to the PIR sensor structure */
    port_motor_hw_t *p_motor;              /*!< Pointer to the motor structure */
    bool presence_or_button_status;        /*!< Presence status in front of the door  or button pressed */
    bool motor_timeout;                    /*!< Timeout of the automatic door for opening or closing */
    uint32_t last_time_presence_or_button; /*!< Last time a presence was detected */
} fsm_automatic_door_t;

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
fsm_t *fsm_automatic_door_new(port_button_hw_t *p_button, port_led_hw_t *p_led_open, port_led_hw_t *p_led_close, port_pir_hw_t *p_pir, port_motor_hw_t *p_motor);

/**
 * @brief Gets the last time a presence was detected.
 *
 * @param p_this Pointer to the FSM structure of the automatic door.
 */
uint32_t fsm_automatic_door_get_last_time_presence(fsm_t *p_this);

/**
 * @brief Gets the presence status of the door.
 *
 * @param p_this Pointer to the FSM structure of the automatic door.
 */
bool fsm_automatic_door_get_presence_status(fsm_t *p_this);

#endif /* FSM_AUTOMATIC_DOOR_H */