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

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a PIR sensor.
 *
 */
typedef struct
{
    GPIO_TypeDef *p_port;     /*!< GPIO where the PIR sensor is connected */
    uint16_t pin;             /*!< Pin/line where the PIR sensor is connected */
    IRQn_Type exti_irq;       /*!< EXTI IRQn */
    uint8_t exti_irq_prio;    /*!< EXTI IRQn priority */
    uint8_t exti_irq_subprio; /*!< EXTI IRQn subpriority */
    bool pir_sensor_status;   /*!< Status of the PIR sensor */
} stm32f4_pir_sensor_hw_t;

/* Global variables -----------------------------------------------------------*/
/**
 * @brief Array of PIR sensor structures to define the HW dependencies of the PIR sensors.
 *
 */
static stm32f4_pir_sensor_hw_t pir_sensors_arr[] = {
    [PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID] = {
        .p_port = STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PORT,
        .pin = STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_GPIO_PIN,
        .exti_irq = STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_EXTI_IRQn,
        .exti_irq_prio = STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_IRQ_PRIO,
        .exti_irq_subprio = STM32F4_PIR_SENSOR_AUTOMATIC_DOOR_IRQ_SUBPRIO,
        .pir_sensor_status = false, // Initialize the PIR sensor status
    },
};

/* Private functions ---------------------------------------------------------*/
/**
 * @brief Configure the EXTI for the PIR sensor.
 *
 * @param pir_sensor_id PIR sensor ID.
 */
void _pir_sensor_exti_config(uint32_t pir_sensor_id)
{
    // Retrieve the PIR sensor from the array
    stm32f4_pir_sensor_hw_t *p_pir_sensor = &pir_sensors_arr[pir_sensor_id];

    /* Enable the SYSCFG Clock to enable the EXTI */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Configure the NVIC for the external interrupt */
    HAL_NVIC_SetPriority(p_pir_sensor->exti_irq, p_pir_sensor->exti_irq_prio, p_pir_sensor->exti_irq_subprio);
    HAL_NVIC_EnableIRQ(p_pir_sensor->exti_irq);
}

/* Function definitions ------------------------------------------------------*/
bool port_pir_sensor_get_status(uint32_t pir_sensor_id)
{
    // Retrieve the PIR sensor from the array
    stm32f4_pir_sensor_hw_t *p_pir_sensor = &pir_sensors_arr[pir_sensor_id];

    // Return the status of the PIR sensor
    return p_pir_sensor->pir_sensor_status;
}

void port_pir_sensor_set_status(uint32_t pir_sensor_id, bool status)
{
    // Retrieve the PIR sensor from the array
    stm32f4_pir_sensor_hw_t *p_pir_sensor = &pir_sensors_arr[pir_sensor_id];

    // Set the status of the PIR sensor
    p_pir_sensor->pir_sensor_status = status;
}

bool port_pir_sensor_read_value(uint32_t pir_sensor_id)
{
    // Retrieve the PIR sensor from the array
    stm32f4_pir_sensor_hw_t *p_pir_sensor = &pir_sensors_arr[pir_sensor_id];
    GPIO_TypeDef *p_port = p_pir_sensor->p_port;
    uint16_t pin = p_pir_sensor->pin;

    // Read the GPIO value of the PIR sensor
    return HAL_GPIO_ReadPin(p_port, pin);
}

void port_pir_sensor_init(uint32_t pir_sensor_id)
{
    // Retrieve the PIR sensor from the array
    stm32f4_pir_sensor_hw_t *p_pir_sensor = &pir_sensors_arr[pir_sensor_id];
    GPIO_TypeDef *p_port = p_pir_sensor->p_port;
    uint16_t pin = p_pir_sensor->pin;

    // Create a generic PIR sensor structure
    GPIO_InitTypeDef pir_sensor = {0}; // Initialize to 0 all fields by default

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

    // Initialize the PIR sensor structure
    pir_sensor.Pin = pin; // Pin where the PIR sensor is connected
    pir_sensor.Mode = GPIO_MODE_IT_RISING_FALLING; // A PIR sensor is an input device
    pir_sensor.Pull = GPIO_NOPULL;                 // No pull-up or pull-down internal resistors

    /* Initialize GPIO pin */
    HAL_GPIO_Init(p_port, &pir_sensor);

    // Configure the EXTI for the PIR sensor
    _pir_sensor_exti_config(pir_sensor_id);

    // Ensure the PIR sensor is not activated at the beginning
    port_pir_sensor_set_status(pir_sensor_id, false);
}

