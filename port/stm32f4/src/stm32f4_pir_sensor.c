/**
 * @file stm32f4_pir_sensor.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for a PIR sensor connected to an STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-04-30
 *
 */

/* Standard C includes */
#include <stdint.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_pir_sensor.h"

/* Microcontroller dependent includes */
#include "stm32f4_pir_sensor.h"

/* Global variables -----------------------------------------------------------*/
static volatile bool pir_sensor_automatic_door_status = false; /*!< Status of the PIR sensor home alarm */

/* Function definitions ------------------------------------------------------*/
bool port_pir_sensor_get_status(uint32_t pir_sensor_id)
{
    if (pir_sensor_id == PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID)
    {
        return pir_sensor_automatic_door_status;
    }
    else
    {
        return false; // Invalid PIR sensor ID
    }
}

void port_pir_sensor_set_status(uint32_t pir_sensor_id, bool status)
{
    if (pir_sensor_id == PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID)
    {
        pir_sensor_automatic_door_status = status;
    }
    else
    {
        return; // Invalid PIR sensor ID
    }
}

bool port_pir_sensor_read_value(uint32_t pir_sensor_id)
{
    if (pir_sensor_id == PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID)
    {
        // Read the GPIO value of the PIR sensor
        return HAL_GPIO_ReadPin(STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PORT, STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PIN);
    }
    else
    {
        return false; // Invalid PIR sensor ID
    }
}

void port_pir_sensor_exti_config(uint32_t pir_sensor_id)
{
    /* Enable the SYSCFG Clock to enable the EXTI */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    if (pir_sensor_id == PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID)
    {

        /* Configure the NVIC for the external interrupt */
        HAL_NVIC_SetPriority(STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_EXTI_IRQn, STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_IRQ_PRIO, STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_IRQ_SUBPRIO);
        HAL_NVIC_EnableIRQ(STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_EXTI_IRQn);
    }
}

void port_pir_sensor_init(uint32_t pir_sensor_id)
{
    // Create a generic PIR sensor structure
    GPIO_InitTypeDef pir_sensor = {0}; // Initialize to 0 all fields by default

    if (pir_sensor_id == PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID)
    {
        /* Enable the GPIO Clock */
        __HAL_RCC_GPIOB_CLK_ENABLE();

        // Initialize the PIR sensor structure
        pir_sensor.Pin = STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PIN;
        pir_sensor.Mode = GPIO_MODE_IT_RISING_FALLING; // A PIR sensor is an input device
        pir_sensor.Pull = GPIO_NOPULL;                 // No pull-up or pull-down internal resistors

        /* Initialize GPIO pin */
        HAL_GPIO_Init(STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PORT, &pir_sensor);

        port_pir_sensor_exti_config(pir_sensor_id);

        // Ensure the PIR sensor is not activated at the beginning
        pir_sensor_automatic_door_status = false;
    }
}