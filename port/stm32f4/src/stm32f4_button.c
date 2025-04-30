/**
 * @file stm32f4_button.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for buttons connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-30
 *
 */

/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_button.h"

/* Microcontroller dependent includes */
#include "stm32f4_button.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a button.
 *
 */
typedef struct
{
    GPIO_TypeDef *p_port;     /*!< GPIO where the button is connected */
    uint16_t pin;             /*!< Pin/line where the button is connected */
    IRQn_Type exti_irq;       /*!< EXTI IRQn */
    uint8_t exti_irq_prio;    /*!< EXTI IRQn priority */
    uint8_t exti_irq_subprio; /*!< EXTI IRQn subpriority */
    bool button_flag_pressed; /*!< Flag to indicate if the button has been pressed */
} stm32f4_button_hw_t;

/* Global variables -----------------------------------------------------------*/
/**
 * @brief Array of button structures to define the HW dependencies of the buttons.
 *
 */
static stm32f4_button_hw_t buttons_arr[] = {
    [PORT_BUTTON_AUTOMATIC_DOOR_ID] = {
        .p_port = STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PORT,
        .pin = STM32F4_BUTTON_AUTOMATIC_DOOR_GPIO_PIN,
        .exti_irq = STM32F4_BUTTON_AUTOMATIC_DOOR_EXTI_IRQn,
        .exti_irq_prio = STM32F4_BUTTON_AUTOMATIC_DOOR_IRQ_PRIO,
        .exti_irq_subprio = STM32F4_BUTTON_AUTOMATIC_DOOR_IRQ_SUBPRIO,
        .button_flag_pressed = false, // Initialize the button flag
    },
};

/* Private functions --------------------------------------------------------*/
/**
 * @brief Configure the EXTI for the button.
 *
 * @param button_id Button ID.
 */
void _button_exti_config(uint32_t button_id)
{
    // Retrieve the button from the array
    stm32f4_button_hw_t *p_button = &buttons_arr[button_id];

    /* Enable the SYSCFG Clock to enable the EXTI */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Configure the NVIC for the external interrupt */
    HAL_NVIC_SetPriority(p_button->exti_irq, p_button->exti_irq_prio, p_button->exti_irq_subprio);
    HAL_NVIC_EnableIRQ(p_button->exti_irq);
}

/* Function definitions ------------------------------------------------------*/
bool port_button_get_status(uint32_t button_id)
{
    // Retrieve the button from the array
    stm32f4_button_hw_t *p_button = &buttons_arr[button_id];

    // Return the button flag status
    return p_button->button_flag_pressed;
}

void port_button_set_status(uint32_t button_id, bool status)
{
    // Retrieve the button from the array
    stm32f4_button_hw_t *p_button = &buttons_arr[button_id];

    // Set the button flag status
    p_button->button_flag_pressed = status;
}

bool port_button_read_value(uint32_t button_id)
{
    // Retrieve the button from the array
    stm32f4_button_hw_t *p_button = &buttons_arr[button_id];

    // Read the GPIO value of the button
    return HAL_GPIO_ReadPin(p_button->p_port, p_button->pin);
}

void port_button_init(uint32_t button_id)
{
    // Retrieve the button from the array
    stm32f4_button_hw_t *p_button = &buttons_arr[button_id];

    // Get the GPIO port and pin
    GPIO_TypeDef *p_port = p_button->p_port;
    uint16_t pin = p_button->pin;

    // Create a generic button structure
    GPIO_InitTypeDef button = {0}; // Initialize to 0 all fields by default

    /* GPIO Ports Clock Enable */
    if (p_port == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (p_port == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (p_port == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    // Initialize the button sensor structure
    button.Pin = pin;                          // Pin where the button is connected
    button.Mode = GPIO_MODE_IT_RISING_FALLING; // A button sensor is an input device
    button.Pull = GPIO_NOPULL;                 // No pull-up or pull-down internal resistors

    /* Initialize GPIO pin */
    HAL_GPIO_Init(p_port, &button);

    // Configure the EXTI for the button
    _button_exti_config(button_id);

    // Ensure the button flag is reset at the beginning
    port_button_set_status(button_id, false);
}