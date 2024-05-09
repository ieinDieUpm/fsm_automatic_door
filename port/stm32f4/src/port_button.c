/**
 * @file port_button.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for the p_button of the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2024-04-01
 *
 */

/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "stm32f4xx.h"
#include "port_system.h"
#include "port_button.h"

/* Global variables -----------------------------------------------------------*/
port_button_hw_t button_emergency = {.p_port = BUTTON_EMERGENCY_GPIO, .pin = BUTTON_EMERGENCY_PIN, .flag_pressed = false, .flag_released = false};

/* Function definitions ------------------------------------------------------*/
void port_button_init(port_button_hw_t *p_button)
{
    // Initialize the GPIO
    port_system_gpio_config(p_button->p_port, p_button->pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
    port_system_gpio_config_exti(p_button->p_port, p_button->pin, TRIGGER_BOTH_EDGE | TRIGGER_ENABLE_INTERR_REQ); /* EXTI both edges */

    port_system_gpio_exti_enable(p_button->pin, 3, 0);
}

bool port_button_is_pressed(port_button_hw_t *p_button)
{
    return p_button->flag_pressed;
}

bool port_button_read_gpio(port_button_hw_t *p_button)
{    
    return (p_button->p_port->IDR & BIT_POS_TO_MASK(p_button->pin)) != 0;
}