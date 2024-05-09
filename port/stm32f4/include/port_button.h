/**
 * @file port_button.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the button port layer.
 * @version 0.1
 * @date 2024-04-01
 *
 */

#ifndef PORT_BUTTON_H
#define PORT_BUTTON_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines --------------------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define BUTTON_EMERGENCY_GPIO GPIOC /*!< GPIO port of the button in the Nucleo board */
#define BUTTON_EMERGENCY_PIN  13    /*!< GPIO pin of the button in the Nucleo board */

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a button.
 */
typedef struct
{
    GPIO_TypeDef *p_port; /*!< GPIO where the button is connected */
    uint8_t pin;          /*!< Pin/line where the button is connected */
    bool flag_pressed;    /*!< Flag to indicate that the button has been pressed */
    bool flag_released;   /*!< Flag to indicate that the button has been released */
} port_button_hw_t;

/* Global variables -----------------------------------------------------------*/
extern port_button_hw_t button_emergency; /*!< Button of the automatic door system. Public for access to interrupt handlers. */

/**
 * @brief Initializes the button.
 *
 * @param p_button Pointer to the button structure.
 */
void port_button_init(port_button_hw_t *p_button);

/**
 * @brief Gets the status of the button.
 *
 * @param p_button Pointer to the button structure.
 * @return true if the button is pressed, false otherwise.
 */
bool port_button_read_gpio(port_button_hw_t *p_button);

/**
 * @brief Gets the status of the button. The button is considered pressed when it has been both pressed; it is not necessary to be released.
 *
 * @param p_button Pointer to the button structure.
 * @return true if the button is pressed, false otherwise.
 */
bool port_button_is_pressed(port_button_hw_t *p_button);

#endif /* PORT_BUTTON_H */