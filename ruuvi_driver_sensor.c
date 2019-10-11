#include "ruuvi_driver_error.h"
#include "ruuvi_driver_sensor.h"
#include <stddef.h>
#include <string.h>

static const char m_init_name[] = "NOTINIT";

ruuvi_driver_status_t ruuvi_driver_sensor_configuration_set(const ruuvi_driver_sensor_t*
    sensor, ruuvi_driver_sensor_configuration_t* config)
{
  ruuvi_driver_status_t err_code = RUUVI_DRIVER_SUCCESS;

  if(NULL == sensor || NULL == config) { return RUUVI_DRIVER_ERROR_NULL; }

  if(NULL == sensor->samplerate_set) { return RUUVI_DRIVER_ERROR_INVALID_STATE; }

  uint8_t sleep = RUUVI_DRIVER_SENSOR_CFG_SLEEP;
  err_code |= sensor->mode_set(&sleep);
  err_code |= sensor->samplerate_set(&(config->samplerate));
  err_code |= sensor->resolution_set(&(config->resolution));
  err_code |= sensor->scale_set(&(config->scale));
  err_code |= sensor->dsp_set(&(config->dsp_function), &(config->dsp_parameter));
  err_code |= sensor->mode_set(&(config->mode));
  return err_code;
}

ruuvi_driver_status_t ruuvi_driver_sensor_configuration_get(const ruuvi_driver_sensor_t*
    sensor, ruuvi_driver_sensor_configuration_t* config)
{
  ruuvi_driver_status_t err_code = RUUVI_DRIVER_SUCCESS;

  if(NULL == sensor || NULL == config) { return RUUVI_DRIVER_ERROR_NULL; }

  if(NULL == sensor->samplerate_set) { return RUUVI_DRIVER_ERROR_INVALID_STATE; }

  err_code |= sensor->samplerate_get(&(config->samplerate));
  err_code |= sensor->resolution_get(&(config->resolution));
  err_code |= sensor->scale_get(&(config->scale));
  err_code |= sensor->dsp_get(&(config->dsp_function), &(config->dsp_parameter));
  err_code |= sensor->mode_get(&(config->mode));
  return err_code;
}

static ruuvi_driver_sensor_timestamp_fp millis = NULL;

ruuvi_driver_status_t ruuvi_driver_sensor_timestamp_function_set(
  ruuvi_driver_sensor_timestamp_fp timestamp_fp)
{
  millis = timestamp_fp;
  return RUUVI_DRIVER_SUCCESS;
}

// Calls the timestamp function and returns it's value. returns 0 if timestamp function is NULL
uint64_t ruuvi_driver_sensor_timestamp_get(void)
{
  if(NULL == millis)
  {
    return 0;
  }

  return millis();
}

bool ruuvi_driver_sensor_is_init(const ruuvi_driver_sensor_t* const sensor)
{
  return (strcmp(sensor->name, m_init_name));
}

static ruuvi_driver_status_t ruuvi_driver_fifo_enable_ni(const bool enable)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_fifo_interrupt_enable_ni(const bool enable)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_fifo_read_ni(size_t* num_elements, ruuvi_driver_sensor_data_t* data)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_level_interrupt_set_ni(const bool enable, float* limit_g)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_data_get_ni(void* const data)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_init_ni(ruuvi_driver_sensor_t* const
    p_sensor, const ruuvi_driver_bus_t bus, const uint8_t handle)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_setup_ni(uint8_t* const value)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_level_interrupt_use_ni(const bool enable,
    float* limit_g)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_dsp_ni(uint8_t* const dsp, uint8_t* const parameter)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

static ruuvi_driver_status_t ruuvi_driver_sensor_configuration_ni(const ruuvi_driver_sensor_t*
    sensor, ruuvi_driver_sensor_configuration_t* config)
{
  return RUUVI_DRIVER_ERROR_NOT_INITIALIZED;
}

void ruuvi_driver_sensor_initialize(ruuvi_driver_sensor_t* const p_sensor)
{
  p_sensor->name                  = m_init_name;
  p_sensor->configuration_get     = ruuvi_driver_sensor_configuration_ni;
  p_sensor->configuration_set     = ruuvi_driver_sensor_configuration_ni;
  p_sensor->data_get              = ruuvi_driver_data_get_ni;
  p_sensor->dsp_get               = ruuvi_driver_dsp_ni;
  p_sensor->dsp_set               = ruuvi_driver_dsp_ni;
  p_sensor->fifo_enable           = ruuvi_driver_fifo_enable_ni;
  p_sensor->fifo_interrupt_enable = ruuvi_driver_fifo_interrupt_enable_ni;
  p_sensor->fifo_read             = ruuvi_driver_fifo_read_ni;
  p_sensor->init                  = ruuvi_driver_init_ni;
  p_sensor->uninit                = ruuvi_driver_init_ni;
  p_sensor->level_interrupt_set   = ruuvi_driver_level_interrupt_use_ni;
  p_sensor->mode_get              = ruuvi_driver_setup_ni;
  p_sensor->mode_set              = ruuvi_driver_setup_ni;
  p_sensor->resolution_get        = ruuvi_driver_setup_ni;
  p_sensor->resolution_set        = ruuvi_driver_setup_ni;
  p_sensor->samplerate_get        = ruuvi_driver_setup_ni;
  p_sensor->samplerate_set        = ruuvi_driver_setup_ni;
  p_sensor->scale_get             = ruuvi_driver_setup_ni;
  p_sensor->scale_set             = ruuvi_driver_setup_ni;
}

void ruuvi_driver_sensor_uninitialize(ruuvi_driver_sensor_t* const p_sensor)
{
  ruuvi_driver_sensor_initialize(p_sensor);
}