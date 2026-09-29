#include <stddef.h>
#include "driver_adc.h"

static uint16_t s_u16SimulatedRawAdc = 1000U;
static bool s_bSimulatedFault = false;

/* Khoi tao phan cung ADC */
AdcStatus_te Adc_Init(void)
{
    s_u16SimulatedRawAdc = 1000U;
    s_bSimulatedFault = false;
    return ADC_STATUS_OK;
}

/* Doc gia tri ADC tu cam bien */
AdcStatus_te Adc_ReadRawValue(uint16_t *pu16AdcVal)
{
    if (pu16AdcVal == NULL)
    {
        return ADC_STATUS_INVALID_PARAM;
    }

    if (s_bSimulatedFault)
    {
        return ADC_STATUS_ERROR;
    }

    *pu16AdcVal = s_u16SimulatedRawAdc;
    return ADC_STATUS_OK;
}

/* Chuyen doi gia tri ADC sang do sang Lux */
AdcStatus_te Adc_ConvertToLux(uint16_t u16AdcVal, float *pf32Lux)
{
    if (pf32Lux == NULL)
    {
        return ADC_STATUS_INVALID_PARAM;
    }

    if (u16AdcVal > ADC_MAX_RAW_VALUE)
    {
        return ADC_STATUS_INVALID_PARAM;
    }

    *pf32Lux = ((float)u16AdcVal / (float)ADC_MAX_RAW_VALUE) * ADC_MAX_LUX_RANGE;
    return ADC_STATUS_OK;
}

/* Mo phong tin hieu cam bien de kiem thu */
void Adc_SetMockValue(uint16_t u16MockValue, bool bSimulateFault)
{
    s_u16SimulatedRawAdc = u16MockValue;
    s_bSimulatedFault = bSimulateFault;
}
