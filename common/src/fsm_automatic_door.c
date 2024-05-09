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
#include "fsm_automatic_door.h"
#include "port_button.h"
#include "port_led.h"
#include "port_pir_sensor.h"

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
    bool pir_status = port_pir_sensor_get_status(p_fsm->p_pir_sensor);

    // Get the status of the button
    bool button_status = port_button_is_pressed(p_fsm->p_button);

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
    return p_fsm->p_motor->timeout;
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
    return p_fsm->p_motor->timeout;
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
    return p_fsm->p_motor->timeout;
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
    port_led_off(p_fsm->p_led_close);

    // Activate the opening LED timer
    port_led_timer_activate(p_fsm->p_led_open);

    // Activate the timer to start the motor and open the door
    port_motor_timeout_timer_activate(p_fsm->p_motor, AUTOMATIC_DOOR_OPENING_CLOSING_TIMEOUT_MS);

    // Update the last time there was a presence or the button was pressed
    p_fsm->presence_or_button_status = true; // If the button is pressed or the PIR sensor detects a presence
    p_fsm->last_time_presence_or_button = port_system_get_millis();
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
    port_led_on(p_fsm->p_led_open);

    // Deactivate the opening LED timer
    port_led_timer_deactivate(p_fsm->p_led_open);

    // Activate the timer to block the motor for a while
    port_motor_timeout_timer_activate(p_fsm->p_motor, AUTOMATIC_DOOR_INACTIVITY_TIMEOUT_MS);
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
    port_motor_timeout_timer_activate(p_fsm->p_motor, AUTOMATIC_DOOR_INACTIVITY_TIMEOUT_MS);
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
    port_led_off(p_fsm->p_led_open);

    // Activate the closing LED timer
    port_led_timer_activate(p_fsm->p_led_close);

    // Activate the timer to start the motor and close the door
    port_motor_timeout_timer_activate(p_fsm->p_motor, AUTOMATIC_DOOR_OPENING_CLOSING_TIMEOUT_MS);

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
    port_led_timer_deactivate(p_fsm->p_led_close);

    // Deactivate the current motor timeout timer
    port_motor_timeout_timer_deactivate(p_fsm->p_motor);

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

    // Leave the closing LED on
    port_led_on(p_fsm->p_led_close);

    // Deactivate the closing LED timer
    port_led_timer_deactivate(p_fsm->p_led_close);

    // Deactivate the motor timeout timer
    port_motor_timeout_timer_deactivate(p_fsm->p_motor);
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
 */
fsm_trans_t fsm_trans_automatic_door[] = {
    
};

uint32_t fsm_automatic_door_get_last_time_presence(fsm_t *p_this)
{
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;
    return p_fsm->last_time_presence_or_button;
}

bool fsm_automatic_door_get_presence_status(fsm_t *p_this)
{
    fsm_automatic_door_t *p_fsm = (fsm_automatic_door_t *)p_this;
    return p_fsm->presence_or_button_status;
}

/* Initialize the FSM */

/**
 * @brief Initialize the automatic door system FSM
 *
 * * > **TO-DO alumnos:**
 * >
 * > ✅ 1. Initialize the FSM structure with the FSM library calling the `fsm_init()` with the original FSM (fsm_t type) and the transition table. 
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 **Do not forget to cast the pointer to the FSM structure** to type `fsm_automatic_door_t` after the initialization.
 * > ✅ 2. Assign the peripherals to the FSM structure: button, LEDs, PIR sensor, and motor.
 * > ✅ 3. Initialize the last time the presence was detected in the FSM structure.
 * > ✅ 4. Initialize the presence status flag in the FSM structure.
 * > ✅ 5. Initialize the peripherals: button, LEDs, PIR sensor, and motor calling the corresponding initialization functions from the port layer: `port_button_init()`, `port_led_init()`, `port_pir_sensor_init()`, and `port_motor_init()`.
 * > ✅ 6. Turn the red LED on calling the `port_led_on()` function.
 * 
 * @param p_this Pointer to the FSM structure
 * @param p_button Pointer to the button structure
 * @param p_led_open Pointer to the LED structure
 * @param p_led_close Pointer to the LED structure
 * @param p_pir Pointer to the PIR sensor structure
 * @param p_motor Pointer to the motor structure
 */
void fsm_automatic_door_init(fsm_t *p_this, port_button_hw_t *p_button, port_led_hw_t *p_led_open, port_led_hw_t *p_led_close, port_pir_hw_t *p_pir, port_motor_hw_t *p_motor)
{
    
}

/* Create FSM */
fsm_t *fsm_automatic_door_new(port_button_hw_t *p_button, port_led_hw_t *p_led_open, port_led_hw_t *p_led_close, port_pir_hw_t *p_pir, port_motor_hw_t *p_motor)
{
    // Do malloc for the whole FSM structure to reserve memory for the rest of the FSM, although I interpret it as fsm_t which is the first field of the structure so that the FSM library can work with it
    fsm_t *p_fsm = malloc(sizeof(fsm_automatic_door_t));

    // Initialize the FSM
    fsm_automatic_door_init(p_fsm, p_button, p_led_open, p_led_close, p_pir, p_motor);

    return p_fsm;
}
