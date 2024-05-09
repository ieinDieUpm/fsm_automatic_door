/**
 * @file main.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Main file for the automatic door system.
 * @version 0.1
 * @date 2024-05-01
 *
 */

/* INCLUDES */
#include <stdio.h>
#include "port_system.h"
#include "fsm_automatic_door.h"

/* MAIN FUNCTION */

/**
 * @brief Main function
 * 
 * @return int 
 */
int main()
{
    // Local variables

    bool previous_presence_status = false;

    /* Init board */
    port_system_init();

    // Create an automatic door FSM system
    fsm_t *p_fsm_automatic_door = fsm_automatic_door_new(&button_emergency, &led_opening, &led_closing, &pir_sensor_automatic_door, &motor_automatic_door);

    while (1)
    {
        // Launch the FSM
        fsm_fire(p_fsm_automatic_door);

        bool current_presence_status = fsm_automatic_door_get_presence_status(p_fsm_automatic_door);
        if (current_presence_status != previous_presence_status)
        {
            uint32_t last_time_presence_or_button = fsm_automatic_door_get_last_time_presence(p_fsm_automatic_door);
            if (current_presence_status)
            {
                printf("PRESENCE!!! Presence detected at %ld. Opening door...\n", last_time_presence_or_button);
            }
            previous_presence_status = current_presence_status;
        }
    }
    return 0;
}