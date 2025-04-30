/**
 * @file stm32f4_led.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for LEDs connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-01
 *
 */

/* Standard C includes */
#include <stdbool.h>
#include <math.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_led.h"

/* Microcontroller dependent includes */
#include "stm32f4_led.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of an RGB LED.
 *
 */
typedef struct
{
    GPIO_TypeDef *p_port;              /*!< GPIO where the LED is connected */
    uint16_t pin;                      /*!< Pin/line where the LED is connected */
    TIM_TypeDef *p_timer;              /*!< Timer to control the LED */
    IRQn_Type timer_irq;               /*!< Timer IRQn */
    uint8_t timer_irq_prio;            /*!< Timer IRQn priority */
    uint8_t timer_irq_subprio;         /*!< Timer IRQn subpriority */
    uint32_t timer_period;             /*!< Timer period in milliseconds */
    TIM_HandleTypeDef handler_tim_led; /*!< Timer handler */
} stm32f4_led_hw_t;

/* Global variables -----------------------------------------------------------*/
/**
 * @brief Array of LED structures to define the HW dependencies of the LEDs.
 *
 */
static stm32f4_led_hw_t leds_arr[] = {
    [PORT_LED_OPENING_ID] = {
        .p_port = STM32F4_LED_OPENING_GPIO_PORT,
        .pin = STM32F4_LED_OPENING_PIN_PORT,
        .p_timer = STM32F4_LED_OPENING_TIMER,
        .timer_irq = STM32F4_LED_OPENING_TIMER_IRQ,
        .timer_irq_prio = STM32F4_LED_OPENING_TIMER_IRQ_PRIO,
        .timer_irq_subprio = STM32F4_LED_OPENING_TIMER_IRQ_SUBPRIO,
        .timer_period = PORT_LED_OPENING_TIMER_BLINK_SEMI_PERIOD_MS,
    },
    [PORT_LED_CLOSING_ID] = {
        .p_port = STM32F4_LED_CLOSING_GPIO_PORT,
        .pin = STM32F4_LED_CLOSING_PIN_PORT,
        .p_timer = STM32F4_LED_CLOSING_TIMER,
        .timer_irq = STM32F4_LED_CLOSING_TIMER_IRQ,
        .timer_irq_prio = STM32F4_LED_CLOSING_TIMER_IRQ_PRIO,
        .timer_irq_subprio = STM32F4_LED_CLOSING_TIMER_IRQ_SUBPRIO,
        .timer_period = PORT_LED_CLOSING_TIMER_BLINK_SEMI_PERIOD_MS,
    }};

/* Private functions ---------------------------------------------------------*/
/**
 * @brief Configure a timer for a given period in milliseconds.
 *
 * @param led_id LED ID.
 */
void _timer_led_config(uint32_t led_id)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Get the timer handler
    TIM_HandleTypeDef *p_handler_timer = &p_led->handler_tim_led;

    // Get the timer instance
    TIM_TypeDef *p_timer = p_led->p_timer;

    // Enable the timer clock using HAL macro
    if (p_timer == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    else if (p_timer == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
    else
    {
        // Invalid timer
        return;
    }

    // Assign the timer instance to the handler
    p_handler_timer->Instance = p_timer;

    // Stop timer with interrupt enabled
    HAL_TIM_Base_Stop_IT(p_handler_timer);

    // Compute ARR and PSC to match the duration in milliseconds. Check if the duration is too long and adapt prescaler and ARR
    double ms = (double)p_led->timer_period;
    double scc = HSI_VALUE; // HSI clock frequency in Hz
    double psc = round((((scc / 1000.0) * ms) / (65535.0 + 1.0)) - 1.0);
    double arr = round((((scc / 1000.0) * ms) / (psc + 1.0)) - 1.0);

    // Adjust psc and arr if necessary
    while (arr > 0xFFFF)
    {
        psc += 1.0;
        arr = round(((scc / 1000.0) * ms) / (psc + 1.0) - 1.0);
    }

    // Set the timer parameters
    // Set the prescaler and auto-reload register
    // Update timer initialization structure with computed PSC and ARR values
    p_handler_timer->Init.Prescaler = (uint32_t)(round(psc));
    p_handler_timer->Init.Period = (uint32_t)(round(arr));

    // Enable the auto-reload preload
    p_handler_timer->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    // Initialize the timer with the new PSC and ARR values
    HAL_TIM_Base_Init(p_handler_timer);

    // Reset counter value
    __HAL_TIM_SET_COUNTER(p_handler_timer, 0);

    // Set priority and enable IRQ in NVIC
    HAL_NVIC_SetPriority(p_led->timer_irq, p_led->timer_irq_prio, p_led->timer_irq_subprio);
}

/* Function definitions ------------------------------------------------------*/
void port_led_init(uint32_t led_id)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Get the GPIO port and pin
    GPIO_TypeDef *p_port = p_led->p_port;
    uint16_t pin = p_led->pin;

    // Initialize the GPIO port and pin
    // Create a generic LED structure to configure the GPIO
    GPIO_InitTypeDef led = {0}; // Initialize to 0 all fields by default

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

    /* Configure GPIO pin */
    led.Pin = pin;
    led.Mode = MODE_OUTPUT; // A LED is an output device
    led.Pull = GPIO_NOPULL; // No pull-up or pull-down internal resistors

    /* Initialize GPIO pin */
    HAL_GPIO_Init(p_port, &led);

    /* Set GPIO pin output level to low */
    HAL_GPIO_WritePin(p_port, pin, GPIO_PIN_RESET);

    // Initialize the timer for the LED
    _timer_led_config(led_id);
}

bool port_led_get_value(uint32_t led_id)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Get the port and pin
    GPIO_TypeDef *p_port = p_led->p_port;
    uint16_t pin = p_led->pin;

    // Return the LED state
    return HAL_GPIO_ReadPin(p_port, pin);
}

void port_led_set_value(uint32_t led_id, bool value)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Get the port and pin
    GPIO_TypeDef *p_port = p_led->p_port;

    uint16_t pin = p_led->pin;

    // Set the LED state
    HAL_GPIO_WritePin(p_port, pin, value);
}

void port_led_toggle(uint32_t led_id)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Get the port and pin
    GPIO_TypeDef *p_port = p_led->p_port;
    uint16_t pin = p_led->pin;

    // Toggle the LED state
    HAL_GPIO_TogglePin(p_port, pin);
}

void port_led_timer_activate(uint32_t led_id, bool activate)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Get the timer handler
    TIM_HandleTypeDef *p_handler_timer = &p_led->handler_tim_led;

    // Get the timer IRQn
    IRQn_Type tim_irq = p_led->timer_irq;

    // limpiar flag de update (MUY IMPORTANTE para evitar interrupción falsa)
    __HAL_TIM_CLEAR_FLAG(p_handler_timer, TIM_FLAG_UPDATE);

    if (activate)
    {
        // Enable the timer
        HAL_TIM_Base_Start_IT(p_handler_timer);
        HAL_NVIC_EnableIRQ(tim_irq);
    }
    else
    {
        // Disable the timer
        HAL_TIM_Base_Stop_IT(p_handler_timer);
        HAL_NVIC_DisableIRQ(tim_irq);
    }
}

TIM_HandleTypeDef *stm32f4_get_led_timer_handler(uint32_t led_id)
{
    // Get the LED structure from the array
    stm32f4_led_hw_t *p_led = &leds_arr[led_id];

    // Return the timer handler
    return &p_led->handler_tim_led;
}