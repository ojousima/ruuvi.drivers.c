/**
 * @file ruuvi_nrf5_sdk15_power.c
 * @author Otso Jousimaa
 * @date 2019-08-01
 * @copyright Ruuvi Innovations Ltd, license BSD-3-Clause
 * @brief Power handling functions, such as enabling internal regulators.
 *
 * License: BSD-3
 * Author: Otso Jousimaa <otso@ojousima.net>
 **/

#include "ruuvi_driver_enabled_modules.h"
#if RUUVI_NRF5_SDK15_ENABLED
#include <stdint.h>
#include "nrf_bootloader_info.h"
#include "nrfx_power.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "ruuvi_driver_error.h"
#include "ruuvi_nrf5_sdk15_error.h"
#include "ruuvi_interface_power.h"
#include "sdk_errors.h"


static bool m_is_init = false;

ruuvi_driver_status_t ruuvi_interface_power_regulators_enable(const
    ruuvi_interface_power_regulators_t regulators)
{
  ret_code_t err_code = NRF_SUCCESS;
  nrfx_power_config_t config = {0};

  if(regulators.DCDC_INTERNAL)
  {
    config.dcdcen = true;
  }

  if(regulators.DCDC_HV)
  {
    #if NRF_POWER_HAS_VDDH
    config.dcdcenhv = true;
    #else
    err_code |= RUUVI_DRIVER_ERROR_NOT_SUPPORTED;
    #endif
  }

  if(m_is_init)
  {
    nrfx_power_uninit();
    m_is_init = false;
  }

  err_code |= nrfx_power_init(&config);
  m_is_init = true;
  return ruuvi_nrf5_sdk15_to_ruuvi_error(err_code);
}

void ruuvi_interface_power_reset(void)
{
  NVIC_SystemReset();
}

void ruuvi_interface_power_enter_bootloader(void)
{
  if(nrf_sdh_is_enabled())
  {
    sd_power_gpregret_clr(0, 0xffffffff);
    sd_power_gpregret_set(0, BOOTLOADER_DFU_START);
    sd_nvic_SystemReset();
  }
  else
  {
    NRF_POWER->GPREGRET = BOOTLOADER_DFU_START;
    NVIC_SystemReset();
  }
}

#endif