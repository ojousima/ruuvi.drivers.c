/**
 * @addtogroup Error
 * @{
 */
/**
 * @file ruuvi_driver_error.c
 * @author Otso Jousimaa
 * @date 2019-01-31
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause
 * @brief Check given error code, log warning on non-fatal error and reset on fatal error
 *
 */
#include "ruuvi_driver_enabled_modules.h"

#include "ruuvi_driver_error.h"
#include "ruuvi_interface_log.h"

#if APPLICATION_POWER_ENABLED
#include "ruuvi_interface_power.h"
#endif

#include <stdio.h>
#include <string.h>

/** Store all occured errors until errors are checked by application here */
static ruuvi_driver_status_t m_errors = RUUVI_DRIVER_SUCCESS;

/** Application callback on errors **/
static ruuvi_driver_error_cb m_cb;

void ruuvi_driver_error_check(ruuvi_driver_status_t error,
                              ruuvi_driver_status_t non_fatal_mask, const char* file, int line)
{
  // Do nothing on success
  if(RUUVI_DRIVER_SUCCESS == error) { return; }
  m_errors |= error;
  char message[APPLICATION_LOG_BUFFER_SIZE];
  size_t index = 0;
  bool fatal = ~non_fatal_mask & error;
  // Cut out the full path
  const char* filename = strrchr(file, '/');

  // If on Windows
  if(NULL == filename) { filename = strrchr(file, '\\'); }

  // In case the file was already only the name
  if(NULL == filename) { filename = file; }
  // Otherwise skip the slash
  else { filename++; }

  // Reset on fatal error
  if(fatal)
  {
    index += snprintf(message, sizeof(message), "%s:%d FATAL: ", filename, line);
    index += ruuvi_interface_error_to_string(error, (message + index),
             (sizeof(message) - index));
    snprintf((message + index), (sizeof(message) - index), "\r\n");
    ruuvi_interface_log_flush();
    ruuvi_interface_log(RUUVI_INTERFACE_LOG_ERROR, message);
    ruuvi_interface_log_flush();
  }
  // Log non-fatal errors
  else if(RUUVI_DRIVER_SUCCESS != error)
  {
    index += snprintf(message, sizeof(message), "%s:%d WARNING: ", filename, line);
    index += ruuvi_interface_error_to_string(error, (message + index),
             (sizeof(message) - index));
    snprintf((message + index), (sizeof(message) - index), "\r\n");
    ruuvi_interface_log(RUUVI_INTERFACE_LOG_WARNING, message);
  }
  // Call error callback
  if(RUUVI_DRIVER_SUCCESS != error && NULL != m_cb) 
  {
    m_cb(error, fatal, filename, line);
  }
}

/*
 * @brief reset global error flags and return their value.
 *
 * @return errors occured after last call to this function.
 */
ruuvi_driver_status_t ruuvi_driver_errors_clear()
{
  ruuvi_driver_status_t errors = m_errors;
  m_errors = RUUVI_DRIVER_SUCCESS;
  return errors;
}

void ruuvi_driver_error_cb_set(ruuvi_driver_error_cb cb)
{
  m_cb = cb;
}

/** @} */
