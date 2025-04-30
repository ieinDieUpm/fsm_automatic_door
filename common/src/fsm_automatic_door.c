/**
 * @file fsm_automatic_door.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Finite State Machine for an automatic door system.
 * @date 2024-04-01
 *
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>

/* Project includes */
#include "port_system.h"
#include "port_button.h"
#include "port_led.h"
#include "port_pir_sensor.h"
#include "port_motor.h"

/* Project includes */
#include "fsm.h"
#include "fsm_automatic_door.h"

/**
 * @brief Structure to define the automatic door FSM.
 */
struct fsm_automatic_door_t
{
    fsm_t f;                 /*!< FSM structure. Important to be the first element of the structure */
    uint32_t button_id;      /*!< Button ID */
    uint32_t led_opening_id; /*!< Opening LED ID */
    uint32_t led_closing_id; /*!< Closing LED ID */
    uint32_t pir_sensor_id;  /*!< PIR sensor ID */
    uint32_t motor_id;       /*!< Motor ID */

    bool presence_or_button_status;        /*!< Presence status in front of the door  or button pressed */
    bool motor_timeout;                    /*!< Timeout of the automatic door for opening or closing */
    uint32_t last_time_presence_or_button; /*!< Last time a presence was detected */
};

/* State machine input or transition functions */

/**
 * @brief Check if there is a presence in front of the door or the button has been pressed (only pressed, it is not necessary to check if it has been released)
 *
 * @param p_this Pointer to the FSM structure
 * @return true If there is a presence in front of the door or the button has been pressed
 * @return false If there is no presence in front of the door or the button has not been pressed
 */
bool check_open(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the PIR sensor
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Get the status of the PIR sensor
    bool pir_status = port_pir_sensor_get_status(p_fsm->pir_sensor_id);

    // Get the status of the button
    bool button_status = port_button_get_status(p_fsm->button_id);

    // Check if there is a new presence or the button has been pressed
    return (pir_status || button_status);
}

/**
 * @brief Check if the opening timeout has expired
 *
 * @param p_this Pointer to the FSM structure
 * @return true If the opening timeout has expired
 * @return false If the opening timeout has not expired
 */
bool check_opening_timeout(fsm_t *p_this)
{
    // Retrieve the FSM structure
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Check if the opening timeout has expired
    bool timeout = port_motor_get_timeout_status(p_fsm->motor_id);
    return timeout;
}

/**
 * @brief Check if there is a presence in front of the door or the button has been pressed (only pressed, it is not necessary to check if it has been released)
 *
 * @param p_this Pointer to the FSM structure
 * @return true If there is a presence in front of the door or the button has been pressed
 * @return false If there is no presence in front of the door or the button has not been pressed
 */
bool check_keep_open(fsm_t *p_this)
{
    return check_open(p_this);
}

/**
 * @brief Check if the inactivity timeout has expired
 *
 * @param p_this Pointer to the FSM structure
 * @return true If the inactivity timeout has expired
 * @return false If the inactivity timeout has not expired
 */
bool check_inactivity_timeout(fsm_t *p_this)
{
    // Retrieve the FSM structure
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Check if the closing timeout has expired
    bool timeout = port_motor_get_timeout_status(p_fsm->motor_id);
    return timeout;
}

/**
 * @brief Check if there is a presence in front of the door or the button has been pressed (only pressed, it is not necessary to check if it has been released)
 *
 * @param p_this Pointer to the FSM structure
 * @return true If there is a presence in front of the door or the button has been pressed
 * @return false If there is no presence in front of the door or the button has not been pressed
 */
bool check_presence_or_button(fsm_t *p_this)
{
    return check_open(p_this);
}

/**
 * @brief Check if the closing timeout has expired
 *
 * @param p_this Pointer to the FSM structure
 * @return true
 * @return false
 */
bool check_closing_timeout(fsm_t *p_this)
{
    // Retrieve the FSM structure
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Check if the closing timeout has expired
    bool timeout = port_motor_get_timeout_status(p_fsm->motor_id);
    return timeout;
}

/* State machine output or action functions */

/**
 * @brief Open the door
 *
 * @param p_this Pointer to the FSM structure
 */
void do_open_door(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Turn off the closing LED
    port_led_set_value(p_fsm->led_closing_id, false);

    // Activate the opening LED timer
    port_led_timer_activate(p_fsm->led_opening_id, true);

    // Activate the timer to start the motor and open the door
    port_motor_timeout_timer_activate(p_fsm->motor_id, AUTOMATIC_DOOR_OPENING_CLOSING_TIMEOUT_MS);

    // Update the last time there was a presence or the button was pressed
    p_fsm->presence_or_button_status = true; // If the button is pressed or the PIR sensor detects a presence
    p_fsm->last_time_presence_or_button = port_system_get_millis();

    // If the interrupt was triggered by the button, reset the button status
    port_button_set_status(p_fsm->button_id, false);
}

/**
 * @brief Stay with the door open
 *
 * @param p_this Pointer to the FSM structure
 */
void do_stay_open(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Leave the opening LED on
    port_led_set_value(p_fsm->led_opening_id, true);

    // Deactivate the opening LED timer
    port_led_timer_activate(p_fsm->led_opening_id, false);

    // Activate the timer to block the motor for a while
    port_motor_timeout_timer_activate(p_fsm->motor_id, AUTOMATIC_DOOR_INACTIVITY_TIMEOUT_MS);
}

/**
 * @brief Restart the motor timeout timer if a presence is detected or the button is pressed
 *
 * @param p_this Pointer to the FSM structure
 */
void do_keep_open(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Restart the motor timeout timer
    // Activate the timer to block the motor for a while
    port_motor_timeout_timer_activate(p_fsm->motor_id, AUTOMATIC_DOOR_INACTIVITY_TIMEOUT_MS);

    // If the interrupt was triggered by the button, reset the button status
    port_button_set_status(p_fsm->button_id, false);
}

/**
 * @brief Close the door
 *
 * @param p_this Pointer to the FSM structure
 */
void do_close_door(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Turn off the opening LED
    port_led_set_value(p_fsm->led_opening_id, false);

    // Activate the closing LED timer
    port_led_timer_activate(p_fsm->led_closing_id, true);

    // Activate the timer to start the motor and close the door
    port_motor_timeout_timer_activate(p_fsm->motor_id, AUTOMATIC_DOOR_OPENING_CLOSING_TIMEOUT_MS);

    p_fsm->presence_or_button_status = false;
}

/**
 * @brief Stop closing the door and open it again if a presence is detected or the button is pressed
 *
 * @param p_this Pointer to the FSM structure
 */
void do_stop_closing_door(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Deactivate the closing LED timer
    port_led_timer_activate(p_fsm->led_closing_id, false);

    // Deactivate the current motor timeout timer
    port_motor_timeout_timer_deactivate(p_fsm->motor_id);

    // Call the function to open the door
    do_open_door(p_this);
}

/**
 * @brief Stay with the door closed
 *
 * @param p_this Pointer to the FSM structure
 */
void do_stay_closed(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;

    // Deactivate the closing LED timer
    port_led_timer_activate(p_fsm->led_closing_id, false);

    // Deactivate the motor timeout timer
    port_motor_timeout_timer_deactivate(p_fsm->motor_id);

    // Leave the closing LED on
    port_led_set_value(p_fsm->led_closing_id, true);
}

/* Transitions table ---------------------------------------------------------*/
/* Esto tiene que estar aquí porque las funciones de la FSM son internas */
/*
 * Maquina de estados: lista de transiciones
 * { EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
 */

/**
 * @brief Transitions table for the automatic door FSM
 *
 * @hideinitializer
 *
 * @image html fsm_automatic_door.png
 */
fsm_trans_t fsm_trans_automatic_door[] = {

    
};

uint32_t fsm_automatic_door_get_last_time_presence(fsm_automatic_door_t *p_this)
{
    return p_this->last_time_presence_or_button;
}

bool fsm_automatic_door_get_presence_status(fsm_automatic_door_t *p_this)
{
    return p_this->presence_or_button_status;
}

/* Initialize the FSM */

/**
 * @brief Initialize the automatic door system FSM
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. Initialize the FSM structure with the FSM library calling the `fsm_init()` with the original FSM (fsm_t type) and the transition table. \n
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 **Do not forget to cast the pointer to the FSM structure** to type `fsm_automatic_door_t` after the initialization. \n
 * > ✅ 2. Assign the peripherals to the FSM structure: button, LEDs, PIR sensor, and motor. \n
 * > ✅ 3. Initialize the last time the presence was detected in the FSM structure. \n
 * > ✅ 4. Initialize the presence status flag in the FSM structure. \n
 * > ✅ 5. Initialize the peripherals: button, LEDs, PIR sensor, and motor calling the corresponding initialization functions from the port layer: `port_button_init()`, `port_led_init()`, `port_pir_sensor_init()`, and `port_motor_init()`. \n
 * > ✅ 6. Turn the red LED on calling the `port_led_set_value()` function. \n
 *
 * @param p_this Pointer to the FSM structure
 * @param p_button Pointer to the button structure
 * @param led_opening_id Pointer to the LED structure
 * @param led_closing_id Pointer to the LED structure
 * @param p_pir Pointer to the PIR sensor structure
 * @param motor_id Pointer to the motor structure
 */
void fsm_automatic_door_init(fsm_automatic_door_t *p_fsm_automatic_door, uint32_t button_id, uint32_t led_opening_id, uint32_t led_closing_id, uint32_t pir_sensor_id, uint32_t motor_id)
{
    fsm_init(&p_fsm_automatic_door->f, fsm_trans_automatic_door);

    // Assign the peripherals to the FSM
    

    // Initialize the last time the  was activated
    

    // Initialize the presence status
    

    // Initialize the peripherals

    // Set the LED closed
    
}

/* Create FSM */
fsm_automatic_door_t *fsm_automatic_door_new(uint32_t button_id, uint32_t led_opening_id, uint32_t led_closing_id, uint32_t pir_sensor_id, uint32_t motor_id)
{
    // Create the FSM structure
    fsm_automatic_door_t *p_fsm_automatic_door = malloc(sizeof(fsm_automatic_door_t));

    // Initialize the FSM
    fsm_automatic_door_init(p_fsm_automatic_door, button_id, led_opening_id, led_closing_id, pir_sensor_id, motor_id);

    return p_fsm_automatic_door;
}

void fsm_automatic_door_fire(fsm_automatic_door_t *p_fsm)
{
    fsm_fire((fsm_t *)p_fsm); // Call the FSM fire function
}