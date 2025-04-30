/**
 * @file stm32f4_button.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for buttons connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-01
 *
 */

/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_button.h"

/* Microcontroller dependent includes */
#include "stm32f4_button.h"

/* Global variables -----------------------------------------------------------*/
static volatile bool button_automatic_door_flag_pressed = false;

/* Function definitions ------------------------------------------------------*/
bool port_button_get_status(uint32_t button_id)
{
    if (button_id == PORT_BUTTON_AUTOMATIC_DOOR_ID)
    {
        return button_automatic_door_flag_pressed;
    }
    else 
    {
        return false; // Invalid button sensor ID
    }
}

void port_button_set_status(uint32_t button_id, bool status)
{
    if (button_id == PORT_BUTTON_AUTOMATIC_DOOR_ID)
    {
        button_automatic_door_flag_pressed = status;
    }
    else
    {
        return; // Invalid button ID
    }
}

bool port_button_read_value(uint32_t button_id)
{
    if (button_id == PORT_BUTTON_AUTOMATIC_DOOR_ID)
    {
        // Read the GPIO value of the button
        return HAL_GPIO_ReadPin(STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PORT, STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PIN);
    }
    else
    {
        return false; // Invalid button ID
    }
}

void port_button_exti_config(uint32_t button_id)
{
    /* Enable the SYSCFG Clock to enable the EXTI */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    if (button_id == PORT_BUTTON_AUTOMATIC_DOOR_ID)
    {

        /* Configure the NVIC for the external interrupt */
        HAL_NVIC_SetPriority(STM32F4_BUTTON_AUTOMATIC_DOOR_EXTI_IRQn, STM32F4_BUTTON_AUTOMATIC_DOOR_IRQ_PRIO, STM32F4_BUTTON_AUTOMATIC_DOOR_IRQ_SUBPRIO);
        HAL_NVIC_EnableIRQ(STM32F4_BUTTON_AUTOMATIC_DOOR_EXTI_IRQn);
    }
}


void port_button_init(uint32_t button_id)
{
    // Create a generic button structure
    GPIO_InitTypeDef button = {0}; // Initialize to 0 all fields by default

    if (button_id == PORT_BUTTON_AUTOMATIC_DOOR_ID)
    {
        /* Enable the GPIO Clock */
        __HAL_RCC_GPIOC_CLK_ENABLE();

        // Initialize the PIR sensor structure
        button.Pin = STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PIN;
        button.Mode = GPIO_MODE_IT_RISING_FALLING; // A PIR sensor is an input device
        button.Pull = GPIO_NOPULL;                 // No pull-up or pull-down internal resistors

        /* Initialize GPIO pin */
        HAL_GPIO_Init(STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PORT, &button);

        port_button_exti_config(button_id);

        // Ensure the button flag is reset at the beginning
        button_automatic_door_flag_pressed = false;
    }
}