/**
 * @file stm32f4_motor.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for the motor connected to an STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-30
 *
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <math.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_motor.h"

/* Microcontroller dependent includes */
#include "stm32f4_motor.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of an RGB LED.
 *
 */
typedef struct
{
    GPIO_TypeDef *p_port;                /*!< GPIO where the motor is connected */
    uint16_t pin;                        /*!< Pin/line where the motor is connected */
    TIM_TypeDef *p_timer;                /*!< Timer to control the motor */
    IRQn_Type timer_irq;                 /*!< Timer IRQn */
    uint8_t timer_irq_prio;              /*!< Timer IRQn priority */
    uint8_t timer_irq_subprio;           /*!< Timer IRQn subpriority */
    uint32_t timer_period;               /*!< Timer period in milliseconds */
    TIM_HandleTypeDef handler_tim_motor; /*!< Timer handler */

    bool motor_timeout_status; /*!< Status of the motor timeout */
} stm32f4_motor_hw_t;

/* Global variables -----------------------------------------------------------*/
/**
 * @brief Array of motor structures to define the HW dependencies of the motors.
 *
 */
static stm32f4_motor_hw_t motors_arr[] = {
    [PORT_MOTOR_AUTOMATIC_DOOR_ID] = {
        .p_port = STM32F4_MOTOR_AUTOMATIC_DOOR_GPIO_PORT,
        .pin = STM32F4_MOTOR_AUTOMATIC_DOOR_PIN,
        .p_timer = STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER,
        .timer_irq = STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER_IRQ,
        .timer_irq_prio = STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER_IRQ_PRIO,
        .timer_irq_subprio = STM32F4_MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER_IRQ_SUBPRIO,
        .timer_period = 0,             // Set to 0 initially
        .motor_timeout_status = false, // Initialize the motor timeout status
    },
};

/* Private functions ---------------------------------------------------------*/
/**
 * @brief Configure a timer for a given motor.
 *
 * @param motor_id Motor ID.
 */
void _timer_motor_config(uint32_t motor_id)
{
    // Get the motor structure from the array
    stm32f4_motor_hw_t *p_motor = &motors_arr[motor_id];

    // Get the timer handler
    TIM_HandleTypeDef *p_handler_tim_motor = &p_motor->handler_tim_motor;

    // Get the timer instance
    TIM_TypeDef *p_timer = p_motor->p_timer;

    // Enable the timer clock
    if (p_timer == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
    else
    {
        return; // Invalid timer
    }

    // Assign the timer instance to the handler
    p_handler_tim_motor->Instance = p_timer;

    // Stop the timer
    HAL_TIM_Base_Stop_IT(p_handler_tim_motor); // Disable the timer

    // Reset the counter value
    __HAL_TIM_SET_COUNTER(p_handler_tim_motor, 0); // Reset the counter value

    // Configure the timer
    p_handler_tim_motor->Init.Prescaler = 0;                                     // Set prescaler to 0
    p_handler_tim_motor->Init.Period = 0;                                        // Set period to 0 initially
    p_handler_tim_motor->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Enable auto-reload preload

    // Set priority and enable IRQ in NVIC
    HAL_NVIC_SetPriority(p_motor->timer_irq, p_motor->timer_irq_prio, p_motor->timer_irq_subprio);
}

/* Function definitions ------------------------------------------------------*/
// TO-DO: Implement the functions related to the PWM of the motor

void port_motor_set_timeout_status(uint32_t motor_id, bool timeout_status)
{
    // Get the motor structure from the array
    stm32f4_motor_hw_t *p_motor = &motors_arr[motor_id];

    // Set the timeout status
    p_motor->motor_timeout_status = timeout_status;

    // Stop the timer
    HAL_TIM_Base_Stop_IT(&p_motor->handler_tim_motor); // Disable the timer
}

bool port_motor_get_timeout_status(uint32_t motor_id)
{
    // Get the motor structure from the array
    stm32f4_motor_hw_t *p_motor = &motors_arr[motor_id];

    // Return the timeout status
    return p_motor->motor_timeout_status;
}

void port_motor_timeout_timer_activate(uint32_t motor_id, uint32_t timeout_ms)
{
    // Get the motor structure from the array
    stm32f4_motor_hw_t *p_motor = &motors_arr[motor_id];

    // Get the timer handler
    TIM_HandleTypeDef *p_handler_tim_motor = &p_motor->handler_tim_motor;

    // Stop the timer
    HAL_TIM_Base_Stop_IT(p_handler_tim_motor); // Disable the timer

    // Set the timeout flag to false
    port_motor_set_timeout_status(motor_id, false);

    // Compute the prescaler and auto-reload register values
    // Compute ARR and PSC to match the duration in milliseconds. Check if the duration is too long and adapt prescaler and ARR
    double sec = (double)timeout_ms / 1000.0;
    double scc = (double)HSI_VALUE;
    double psc = round(((scc * sec) / (65535.0 + 1.0)) - 1.0);
    double arr = round(((scc * sec) / (psc + 1.0)) - 1.0);

    // Adjust psc and arr if necessary
    while (arr > 0xFFFF)
    {
        psc += 1.0;
        arr = round((scc * sec) / (psc + 1.0) - 1.0);
    }

    // Set the timer parameters
    p_handler_tim_motor->Init.Prescaler = (uint32_t)(round(psc)); // Set the prescaler value
    p_handler_tim_motor->Init.Period = (uint32_t)(round(arr));    // Set the auto-reload value

    // Initialize the timer
    HAL_TIM_Base_Init(p_handler_tim_motor);

    // The PSC and ARR values are currently in the preload registers. To load them into the active registers we
    // need an update event. We can do this manually as follows (or we could wait for the timer to expire).
    // Generate an update event to load the new PSC and ARR values immediately
    HAL_TIM_GenerateEvent(p_handler_tim_motor, TIM_EVENTSOURCE_UPDATE); // 6) Update generation: Re-inicializa el contador y actualiza los registros. IMPORTANTE que esté lo último

    // Reset the counter value
    __HAL_TIM_SET_COUNTER(p_handler_tim_motor, 0); // Reset the counter value

    // Set priority and enable IRQ in NVIC
    HAL_NVIC_EnableIRQ(p_motor->timer_irq); // Enable the timer interrupt in NVIC

    // limpiar flag de update (MUY IMPORTANTE para evitar interrupción falsa)
    __HAL_TIM_CLEAR_FLAG(p_handler_tim_motor, TIM_FLAG_UPDATE);

    // Start the timer
    HAL_TIM_Base_Start_IT(p_handler_tim_motor); // Enable the timer
}

void port_motor_timeout_timer_deactivate(uint32_t motor_id)
{
    // Get the motor structure from the array
    stm32f4_motor_hw_t *p_motor = &motors_arr[motor_id];

    // Get the timer handler
    TIM_HandleTypeDef *p_handler_tim_motor = &p_motor->handler_tim_motor;

    // Get the timer IRQn
    IRQn_Type tim_irq = p_motor->timer_irq;

    HAL_TIM_Base_Stop_IT(p_handler_tim_motor); // Disable the timer
    HAL_NVIC_DisableIRQ(tim_irq);              // Disable the timer interrupt in NVIC
}

void port_motor_init(uint32_t motor_id)
{
    // Initialize the GPIO if the motor is connected to the Nucleo board
    // TODO: Implement the initialization of the GPIO

    // Initialize the PWM if the motor is connected to the Nucleo board
    // TODO: Implement the initialization of the PWM

    // Initialize the timeout timer
    _timer_motor_config(motor_id);
}

TIM_HandleTypeDef *stm32f4_get_motor_timer_handler(uint32_t motor_id)
{
    // Get the motor structure from the array
    stm32f4_motor_hw_t *p_motor = &motors_arr[motor_id];

    // Return the timer handler
    return &p_motor->handler_tim_motor;
}