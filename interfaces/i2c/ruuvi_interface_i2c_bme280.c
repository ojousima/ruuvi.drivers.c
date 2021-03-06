#include "ruuvi_driver_enabled_modules.h"
#if RUUVI_INTERFACE_ENVIRONMENTAL_BME280_ENABLED && RUUVI_INTERFACE_ENVIRONMENTAL_BME280_I2C_ENABLED
/**
 * @addtogroup I2C
 *
 */
/*@{*/
/**
 * @file ruuvi_interface_i2c_bme280.c
 * @author Otso Jousimaa <otso@ojousima.net>
 * @date 2019-03-04
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause.
 * @brief Wrapper for BME280 I2C calls
 *
 */
#include <stdint.h>
#include <string.h>

#include "ruuvi_boards.h"
#include "ruuvi_driver_error.h"
#include "ruuvi_interface_gpio.h"
#include "ruuvi_interface_i2c.h"
#include "ruuvi_interface_yield.h"

/*
 * Data on the bus should be like
 * |------------+---------------------|
 * | I2C action | Data                |
 * |------------+---------------------|
 * | Start      | -                   |
 * | Write      | (reg_addr)          |
 * | Write      | (reg_data[0])       |
 * | Write      | (....)              |
 * | Write      | (reg_data[len - 1]) |
 * | Stop       | -                   |
 * |------------+---------------------|
 */
int8_t ruuvi_interface_i2c_bme280_write(uint8_t dev_id, uint8_t reg_addr,
                                        uint8_t* reg_data, uint16_t len)
{
  ruuvi_driver_status_t err_code = RUUVI_DRIVER_SUCCESS;

  // Support only 1-byte writes
  if(1 > len || 2 < len) { return -1; }

  uint8_t wbuf[2] = {0};
  wbuf[0] = reg_addr;
  wbuf[1] = reg_data[0];
  err_code |= ruuvi_interface_i2c_write_blocking(dev_id, wbuf, 2, true);
  return (RUUVI_DRIVER_SUCCESS == err_code) ? 0 : -1;
}

/*
 * Data on the bus should be like
 * |------------+---------------------|
 * | I2C action | Data                |
 * |------------+---------------------|
 * | Start      | -                   |
 * | Write      | (reg_addr)          |
 * | Stop       | -                   |
 * | Start      | -                   |
 * | Read       | (reg_data[0])       |
 * | Read       | (....)              |
 * | Read       | (reg_data[len - 1]) |
 * | Stop       | -                   |
 * |------------+---------------------|
 */

int8_t ruuvi_interface_i2c_bme280_read(uint8_t dev_id, uint8_t reg_addr,
                                       uint8_t* reg_data, uint16_t len)
{
  ruuvi_driver_status_t err_code = RUUVI_DRIVER_SUCCESS;
  err_code |= ruuvi_interface_i2c_write_blocking(dev_id, &reg_addr, 1, true);
  err_code |= ruuvi_interface_i2c_read_blocking(dev_id, reg_data, len);
  return (RUUVI_DRIVER_SUCCESS == err_code) ? 0 : -1;
}
#endif