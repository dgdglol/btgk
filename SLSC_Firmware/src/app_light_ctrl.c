#include <stddef.h>
#include "app_light_ctrl.h"
#include "driver_adc.h"
#include "driver_pwm.h"

static float s_f32LuxBuffer[LIGHT_CTRL_WINDOW_SIZE];
static uint8_t s_u8BufferIndex = 0U;
static uint8_t s_u8SampleCount = 0U;
static uint8_t s_u8SensorErrorCount = 0U;
static uint8_t s_u8ManualDuty = 50U;
static LightData_ts s_LightData;

/* Khoi tao bo dieu khien */
LightStatus_te LightCtrl_Init(void)
{
    for (uint8_t u8Idx = 0U; u8Idx < LIGHT_CTRL_WINDOW_SIZE; u8Idx++)
    {
        s_f32LuxBuffer[u8Idx] = 0.0f;
    }

    s_u8BufferIndex = 0U;
    s_u8SampleCount = 0U;
    s_u8SensorErrorCount = 0U;
    s_u8ManualDuty = 50U;

    s_LightData.f32CurrentLux = 0.0f;
    s_LightData.f32FilteredLux = 0.0f;
    s_LightData.u8DutyCycle = 0U;
    s_LightData.eMode = LIGHT_MODE_AUTO;
    s_LightData.eState = LIGHT_STATE_NORMAL;
    s_LightData.bIsNightMode = false;
    s_LightData.bIsFault = false;

    (void)Adc_Init();
    (void)Pwm_Init();
    (void)Pwm_SetDuty(0U);

    return LIGHT_STATUS_OK;
}

/* Tinh toan muc phat PWM dua theo do sang */
LightStatus_te LightCtrl_CalculateDuty(float f32Lux, uint8_t *pu8DutyCycle)
{
    if (pu8DutyCycle == NULL)
    {
        return LIGHT_STATUS_INVALID_PARAM;
    }

    if ((f32Lux < LIGHT_CTRL_SENSOR_MIN_LUX) || (f32Lux > LIGHT_CTRL_SENSOR_MAX_LUX))
    {
        return LIGHT_STATUS_SENSOR_FAULT;
    }

    if (f32Lux < LIGHT_CTRL_MIN_LUX_THRESHOLD)
    {
        *pu8DutyCycle = 100U;
    }
    else if (f32Lux > LIGHT_CTRL_MAX_LUX_THRESHOLD)
    {
        *pu8DutyCycle = 0U;
    }
    else
    {
        float f32Range = LIGHT_CTRL_MAX_LUX_THRESHOLD - LIGHT_CTRL_MIN_LUX_THRESHOLD;
        float f32Fraction = (f32Lux - LIGHT_CTRL_MIN_LUX_THRESHOLD) / f32Range;
        float f32Duty = 90.0f - (f32Fraction * 80.0f);
        *pu8DutyCycle = (uint8_t)(f32Duty + 0.5f);
    }

    return LIGHT_STATUS_OK;
}

/* Vong lap xu ly dinh ky */
void LightCtrl_ProcessLoop(void)
{
    uint16_t u16RawAdc = 0U;
    AdcStatus_te eAdcStatus = Adc_ReadRawValue(&u16RawAdc);

    if (eAdcStatus != ADC_STATUS_OK)
    {
        s_u8SensorErrorCount++;
        if (s_u8SensorErrorCount >= LIGHT_CTRL_MAX_FAILSAFE_COUNT)
        {
            s_LightData.eState = LIGHT_STATE_SAFE_MODE;
            s_LightData.bIsFault = true;
            s_LightData.u8DutyCycle = 100U;
            (void)Pwm_SetDuty(100U);
        }
        return;
    }

    s_u8SensorErrorCount = 0U;
    s_LightData.eState = LIGHT_STATE_NORMAL;
    s_LightData.bIsFault = false;

    float f32Lux = 0.0f;
    (void)Adc_ConvertToLux(u16RawAdc, &f32Lux);
    s_LightData.f32CurrentLux = f32Lux;

    s_f32LuxBuffer[s_u8BufferIndex] = f32Lux;
    s_u8BufferIndex = (s_u8BufferIndex + 1U) % LIGHT_CTRL_WINDOW_SIZE;

    if (s_u8SampleCount < LIGHT_CTRL_WINDOW_SIZE)
    {
        s_u8SampleCount++;
    }

    float f32Sum = 0.0f;
    for (uint8_t u8Idx = 0U; u8Idx < s_u8SampleCount; u8Idx++)
    {
        f32Sum += s_f32LuxBuffer[u8Idx];
    }
    s_LightData.f32FilteredLux = f32Sum / (float)s_u8SampleCount;

    if (s_LightData.eMode == LIGHT_MODE_AUTO)
    {
        uint8_t u8Duty = 0U;
        (void)LightCtrl_CalculateDuty(s_LightData.f32FilteredLux, &u8Duty);
        s_LightData.u8DutyCycle = u8Duty;
        s_LightData.bIsNightMode = (s_LightData.f32FilteredLux < LIGHT_CTRL_MIN_LUX_THRESHOLD);
        (void)Pwm_SetDuty(u8Duty);
    }
    else
    {
        s_LightData.u8DutyCycle = s_u8ManualDuty;
        (void)Pwm_SetDuty(s_u8ManualDuty);
    }
}

/* Chuyen doi che do dieu khien */
void LightCtrl_SetMode(LightMode_te eNewMode)
{
    s_LightData.eMode = eNewMode;
}

/* Dat cong suat den thu cong */
void LightCtrl_SetManualDuty(uint8_t u8DutyCycle)
{
    if (u8DutyCycle <= 100U)
    {
        s_u8ManualDuty = u8DutyCycle;
    }
}

/* Lay du lieu trang thai he thong */
LightStatus_te LightCtrl_GetData(LightData_ts *pOutData)
{
    if (pOutData == NULL)
    {
        return LIGHT_STATUS_INVALID_PARAM;
    }

    *pOutData = s_LightData;
    return LIGHT_STATUS_OK;
}
