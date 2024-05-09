/**
 * @file port_led.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for a LED.
 * @date 01-05-2024
 */
/* Standard C includes */
#include <stdio.h>
#include <math.h>

/* HW dependent includes */
#include "stm32f4xx.h"
#include "port_led.h"
#include "port_system.h"

/* Global variables -----------------------------------------------------------*/
port_led_hw_t led_opening = {.p_port = LED_OPENING_GPIO, .pin = LED_OPENING_PIN, .p_timer = LED_OPENING_TIMER, .timer_blink_semi_period_ms = LED_OPENING_TIMER_BLINK_SEMI_PERIOD_MS};
port_led_hw_t led_closing = {.p_port = LED_CLOSING_GPIO, .pin = LED_CLOSING_PIN, .p_timer = LED_CLOSING_TIMER, .timer_blink_semi_period_ms = LED_CLOSING_TIMER_BLINK_SEMI_PERIOD_MS};

bool port_led_get_status(port_led_hw_t *p_led)
{
    return (p_led->p_port->IDR & BIT_POS_TO_MASK(p_led->pin)) != 0;
}

void port_led_on(port_led_hw_t *p_led)
{
    p_led->p_port->ODR |= BIT_POS_TO_MASK(p_led->pin);
}

void port_led_off(port_led_hw_t *p_led)
{
    p_led->p_port->ODR &= ~BIT_POS_TO_MASK(p_led->pin);
}

void port_led_toggle(port_led_hw_t *p_led)
{
    p_led->p_port->ODR ^= BIT_POS_TO_MASK(p_led->pin);
}

void port_led_timer_setup(port_led_hw_t *p_led)
{
    // Enable the peripheral clock
    if (p_led->p_timer == TIM3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    }
    else if (p_led->p_timer == TIM4)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }

    // Disable the timer
    p_led->p_timer->CR1 &= ~TIM_CR1_CEN;

    // Autoreload preload enabled
    p_led->p_timer->CR1 |= TIM_CR1_ARPE;

    // Reset the values of the timer
    p_led->p_timer->CNT = 0;

    // Set the timeout value
    // Compute ARR and PSC to match the duration in milliseconds. Check if the duration is too long and adapt prescaler and ARR
    double sec = (double)p_led->timer_blink_semi_period_ms / 1000.0;
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
    p_led->p_timer->ARR = (uint32_t)(round(arr));
    p_led->p_timer->PSC = (uint32_t)(round(psc));

    // Clean interrupt flags
    p_led->p_timer->SR &= ~TIM_SR_UIF;

    // Enable the update interrupt
    p_led->p_timer->DIER |= TIM_DIER_UIE;

    if (p_led->p_timer == TIM3)
    {
        // Enable the interrupt in the NVIC
        NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0)); /* Priority 2, sub-priority 0 */
        NVIC_EnableIRQ(TIM3_IRQn);
    }
    else if (p_led->p_timer == TIM4)
    {
        // Enable the interrupt in the NVIC
        NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0)); /* Priority 2, sub-priority 0 */
        NVIC_EnableIRQ(TIM4_IRQn);
    }
}

void port_led_timer_activate(port_led_hw_t *p_led)
{
    // Enable the timer
    p_led->p_timer->CR1 |= TIM_CR1_CEN;

    // The PSC and ARR values are currently in the preload registers. To load them into the active registers we
    // need an update event. We can do this manually as follows (or we could wait for the timer to expire).
    p_led->p_timer->EGR |= TIM_EGR_UG; // 6) Update generation: Re-inicializa el contador y actualiza los registros. IMPORTANTE que esté lo último y que se haga siempre.
    }

void port_led_timer_deactivate(port_led_hw_t *p_led)
{
    // Disable the timer
    p_led->p_timer->CR1 &= ~TIM_CR1_CEN;
}

void port_led_init(port_led_hw_t *p_led)
{
    port_system_gpio_config(p_led->p_port, p_led->pin, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
    port_led_timer_setup(p_led);
}