/**
 * @file port_led.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header for the portable functions to interact with the HW of the LEDs. The functions must be implemented in the platform-specific code.
 * @date 01-04-2025
 */
#ifndef PORT_LED_H_
#define PORT_LED_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and macros --------------------------------------------------------*/
/* Defines */
#define PORT_LED_OPENING_ID 0                      /*!< Opening LED identifier @hideinitializer */
#define PORT_LED_CLOSING_ID 1                      /*!< Closing LED identifier @hideinitializer */
#define PORT_LED_OPENING_TIMER_BLINK_SEMI_PERIOD_MS 500 /*!< Semi-period of the blinking of the opening LED @hideinitializer */
#define PORT_LED_CLOSING_TIMER_BLINK_SEMI_PERIOD_MS 100 /*!< Semi-period of the blinking of the closing LED */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Configure the HW specifications of a given LED.
 *
 * @param led_id LED ID.
 */
void port_led_init(uint32_t led_id);

/**
 * @brief Get the value of the GPIO connected to the LED.
 *
 * @param led_id LED ID.
 *
 * @return true if the LED value is high
 * @return false if the LED value is low
 */
bool port_led_get_value(uint32_t led_id);

/**
 * @brief Set the value of the GPIO connected to the LED.
 *
 * @param led_id LED ID.
 * @param value If true, the LED is set as on. If false, the LED is set as off.
 */
void port_led_set_value(uint32_t led_id, bool value);

/**
 * @brief Toggles the LED state.
 *
 * @param led_id LED ID.
 *
 */
void port_led_toggle(uint32_t led_id);

/**
 * @brief Activates the timer for the LED for blinking.
 *
 * @param led_id LED ID.
 * @param activate If true, the timer is activated. If false, the timer is deactivated.
 *
 */
void port_led_timer_activate(uint32_t led_id, bool activate);

#endif // PORT_LED_H_
