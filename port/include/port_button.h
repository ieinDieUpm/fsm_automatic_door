/**
 * @file port_button.h
 * @brief Header for the portable functions to interact with the HW of the buttons. The functions must be implemented in the platform-specific code.
 * @author Josue Pagán Ortiz (j.pagan@upm.es)
 * @date 2025-04-01
 */
#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define PORT_BUTTON_AUTOMATIC_DOOR_ID 0 /*!< Nucleo board button identifier @hideinitializer */

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Configure the HW specifications of a given button.
 *
 * @param button_id Button ID.
 */
void port_button_init(uint32_t button_id);

/**
 * @brief Gets the status of the button. The button is considered pressed when it has been both pressed and released.
 *
 * @param button_id Button ID.
 *
 * @return true If the button is pressed
 * @return false If the button is not pressed
 */
bool port_button_get_status(uint32_t button_id);

/**
 * @brief Set the button as pressed. This function is used to set the button status to pressed when it has been pressed and released.
 *
 * @param button_id Button ID.
 * @param pressed If true, the button is set as pressed. If false, the button is set as not pressed.
 */
void port_button_set_status(uint32_t button_id, bool pressed);

/**
 * @brief Get the value of the GPIO connected to the button.
 *
 * @param button_id Button ID.
 * @return true If the button value is high
 * @return false If the button value is low
 */
bool port_button_read_value(uint32_t button_id);

#endif /* PORT_BUTTON_H_ */
