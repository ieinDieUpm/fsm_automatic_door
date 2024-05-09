/**
 * @file port_pir_sensor.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for a PIR sensor connected to an STM32F4 Nucleo board.
 * @version 0.1
 * @date 2024-04-01
 *
 */

/* Standard C includes */
#include <stdint.h>

/* HW dependent includes */
#include "port_pir_sensor.h"
#include "stm32f4xx.h"
#include "port_system.h"

/* Global variables -----------------------------------------------------------*/
port_pir_hw_t pir_sensor_automatic_door = {.p_port = PIR_SENSOR_AUTOMATIC_DOOR_GPIO, .pin = PIR_SENSOR_AUTOMATIC_DOOR_PIN, .sensor_status = false, .last_time_presence_or_button = 0};

/* Function definitions ------------------------------------------------------*/
bool port_pir_sensor_get_status(port_pir_hw_t* p_pir)
{
    return p_pir->sensor_status;
}

void port_pir_sensor_set_status(port_pir_hw_t* p_pir, bool status)
{
    p_pir->sensor_status = status;
}

bool port_pir_sensor_read_gpio(port_pir_hw_t *p_pir)
{    
    return (p_pir->p_port->IDR & BIT_POS_TO_MASK(p_pir->pin));
}

void port_pir_sensor_init(port_pir_hw_t* p_pir)
{
    // Initialize the GPIO
    port_system_gpio_config(p_pir->p_port, p_pir->pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
    port_system_gpio_config_exti(p_pir->p_port, p_pir->pin, TRIGGER_BOTH_EDGE | TRIGGER_ENABLE_INTERR_REQ);
    
    port_system_gpio_exti_enable(p_pir->pin, 1, 0);
}