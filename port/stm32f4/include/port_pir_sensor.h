/**
 * @file port_pir_sensor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the PIR sensor port layer.
 * @version 0.1
 * @date 2024-04-01
 *
 */

#ifndef PORT_PIR_SENSOR_H
#define PORT_PIR_SENSOR_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define PIR_SENSOR_AUTOMATIC_DOOR_GPIO GPIOA /*!< GPIO port of the PIR sensor of the automatic door */
#define PIR_SENSOR_AUTOMATIC_DOOR_PIN 10     /*!< GPIO pin of the PIR sensor of the automatic door */

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a PIR sensor.
 */
typedef struct
{
    GPIO_TypeDef *p_port;                  /*!< GPIO where the PIR is connected */
    uint8_t pin;                           /*!< Pin/line where the PIR is connected */
    bool sensor_status;                    /*!< Wether the sensor is detecting movement or not */
    uint32_t last_time_presence_or_button; /*!< Last time a presence was detected */
} port_pir_hw_t;

/* Global variables -----------------------------------------------------------*/
extern port_pir_hw_t pir_sensor_automatic_door; /*!< PIR sensor of the automatic door system. Public for access to interrupt handlers. */

/**
 * @brief Gets the status of the PIR sensor.
 *
 * @return true if the PIR sensor is detecting movement, false otherwise.
 */
bool port_pir_sensor_get_status(port_pir_hw_t *pir_sensor);

/**
 * @brief Sets the status of the PIR sensor.
 *
 * @param pir_sensor Pointer to the PIR sensor structure.
 * @param status true if the PIR sensor is detecting movement, false otherwise.
 */
void port_pir_sensor_set_status(port_pir_hw_t *pir_sensor, bool status);

/**
 * @brief Initializes the PIR sensor.
 *
 * @param pir_sensor Pointer to the PIR sensor structure.
 */
void port_pir_sensor_init(port_pir_hw_t *pir_sensor);

/**
 * @brief Reads the GPIO of the PIR sensor.
 *
 * @param pir_sensor Pointer to the PIR sensor structure.
 * @return true
 * @return false
 */
bool port_pir_sensor_read_gpio(port_pir_hw_t *pir_sensor);

#endif /* PORT_PIR_SENSOR_H */