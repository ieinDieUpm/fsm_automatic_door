/**
 * @file port_motor.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for the motor connected to an STM32F4 Nucleo board.
 * @version 0.1
 * @date 2024-05-05
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <math.h>

/* Project includes */
#include "port_motor.h"

/* Global variables -----------------------------------------------------------*/
port_motor_hw_t motor_automatic_door = {.p_port = MOTOR_AUTOMATIC_DOOR_GPIO, .pin = MOTOR_AUTOMATIC_DOOR_PIN, .p_timer_timeout = MOTOR_AUTOMATIC_DOOR_TIMEOUT_TIMER, .timeout = false};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initializes the timer for the timeout of the motor.
 * 
 * @param p_motor Pointer to the motor structure.
 */
static void _motor_timeout_timer_init(port_motor_hw_t *p_motor)
{
    // Enable the peripheral clock
    if (p_motor->p_timer_timeout == TIM2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }

    // Disable the timer
    p_motor->p_timer_timeout->CR1 &= ~TIM_CR1_CEN;

    // Auto-reload preload enable
    p_motor->p_timer_timeout->CR1 |= TIM_CR1_ARPE;

    // Set the prescaler and the auto-reload register with any value
    p_motor->p_timer_timeout->PSC = 0;
    p_motor->p_timer_timeout->ARR = 0xFFFF;

    // Clean interrupt flags
    p_motor->p_timer_timeout->SR = 0;

    if (p_motor->p_timer_timeout == TIM2)
    {
        // Enable the interrupt in the NVIC
        NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0)); /* Priority 2, sub-priority 0 */
        NVIC_EnableIRQ(TIM2_IRQn);
    }
}

/* Function definitions ------------------------------------------------------*/
// TO-DO: Implement the functions related to the PWM of the motor

void port_motor_set_timeout_status(port_motor_hw_t *p_motor, bool timeout)
{
    if (timeout)
    {
        p_motor->p_timer_timeout->DIER &= ~TIM_DIER_UIE; // Disable the timer so that it does not interrupt again
    }
    p_motor->timeout = timeout;
}

void port_motor_timeout_timer_activate(port_motor_hw_t *p_motor, uint32_t timeout_ms)
{
    // Disable the timer
    p_motor->p_timer_timeout->CR1 &= ~TIM_CR1_CEN;

    // Set the timeout flag to false
    port_motor_set_timeout_status(p_motor, false);

    p_motor->p_timer_timeout->CNT = 0;
    
    // Set the timeout value
    // Compute ARR and PSC to match the duration in milliseconds. Check if the duration is too long and adapt prescaler and ARR
    double sec = (double)timeout_ms / 1000.0;
    double scc = (double)SystemCoreClock;
    double psc = round(((scc * sec) / (65535.0 + 1.0)) - 1.0);
    double arr = round(((scc * sec) / (psc + 1.0)) - 1.0);

    // Adjust psc and arr if necessary
    if (arr > 0xFFFF)
    {
        psc += 1.0;
        arr = round((scc * sec) / (psc + 1.0) - 1.0);
    }

    // Load the values
    p_motor->p_timer_timeout->ARR = (uint32_t)(round(arr));
    p_motor->p_timer_timeout->PSC = (uint32_t)(round(psc));

    // Enable the timer
    p_motor->p_timer_timeout->CR1 |= TIM_CR1_CEN;

    p_motor->p_timer_timeout->EGR |= TIM_EGR_UG; // Generate an update event to update the prescaler and the ARR

    // WARNING: for some reason, the update event flag is set after the update event is generated and the ISR is called immediately. 
    // To solve this (see https://electronics.stackexchange.com/questions/161967/stm32-timer-interrupt-works-immediately/161982#161982)
    // Clear the update interrupt flag
    // Configure so that the interrupt flag is only set upon overflow
    // Enable the timer Update Interrupt type. 
    // IMPORTANT: The UIE bit should not be set in the initialization of the timer (in _motor_timeout_timer_init())

    p_motor->p_timer_timeout->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag

    // Configure so that the interrupt flag is only set upon overflow
    p_motor->p_timer_timeout->CR1 |= TIM_CR1_URS;

    // Enable the timer Update Interrupt type
    p_motor->p_timer_timeout->DIER |= TIM_DIER_UIE;
}

void port_motor_timeout_timer_deactivate(port_motor_hw_t *p_motor)
{
    // Disable the timer
    p_motor->p_timer_timeout->CR1 &= ~TIM_CR1_CEN;
}

void port_motor_init(port_motor_hw_t *p_motor)
{
    // Initialize the GPIO if the motor is connected to the Nucleo board
    // TO-DO: Implement the initialization of the GPIO

    // Initialize the PWM if the motor is connected to the Nucleo board
    // TO-DO: Implement the initialization of the PWM

    // Initialize the timeout timer
    _motor_timeout_timer_init(p_motor);
}
