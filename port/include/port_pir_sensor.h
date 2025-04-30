/**
 * @file port_pir_sensor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header for the portable functions to interact with the HW of the PIR sensors. The functions must be implemented in the platform-specific code.
 * @date 01-04-2025
 */
#ifndef PORT_PIR_SENSOR_H_
#define PORT_PIR_SENSOR_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and macros --------------------------------------------------------*/
/* Defines */
#define PORT_PIR_SENSOR_AUTOMATIC_DOOR_ID 0 /*!< Nucleo PIR SENSOR button identifier @hideinitializer */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Configure the HW specifications of a given PIR SENSOR.
 *
 * @param pir_sensor_id PIR SENSOR ID.
 */
void port_pir_sensor_init(uint32_t pir_sensor_id);

/**
 * @brief Get the status of the PIR SENSOR.
 *
 * @param pir_sensor_id PIR SENSOR ID.
 *
 * @return true if the PIR SENSOR value has detected a movement
 * @return false if the PIR SENSOR value hsa not detected a movement
 */
bool port_pir_sensor_get_status(uint32_t pir_sensor_id);

/**
 * @brief Sets the status of the PIR sensor.
 *
 * @param pir_sensor_id PIR sensor ID.
 * @param status New status of the PIR sensor.
 */
void port_pir_sensor_set_status(uint32_t pir_sensor_id, bool status);

/**
 * @brief Reads the GPIO value of the PIR sensor.
 *
 * @param pir_sensor_id PIR sensor ID.
 * @return true
 * @return false
 */
bool port_pir_sensor_read_value(uint32_t pir_sensor_id);

#endif // PORT_PIR_SENSOR_H_
