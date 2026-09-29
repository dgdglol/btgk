#include <stddef.h>
#include "driver_pwm.h"

static uint8_t s_u8CurrentDuty = 0U;

/* Khoi tao phan cung PWM */
PwmStatus_te Pwm_Init(void)
{
    s_u8CurrentDuty = 0U;
    return PWM_STATUS_OK;
}

/* Thiet lap gia tri xung PWM */
PwmStatus_te Pwm_SetDuty(uint8_t u8DutyCycle)
{
    if (u8DutyCycle > PWM_MAX_DUTY_CYCLE)
    {
        return PWM_STATUS_INVALID_PARAM;
    }

    s_u8CurrentDuty = u8DutyCycle;
    return PWM_STATUS_OK;
}

/* Doc gia tri xung PWM hien tai */
PwmStatus_te Pwm_GetDuty(uint8_t *pu8DutyCycle)
{
    if (pu8DutyCycle == NULL)
    {
        return PWM_STATUS_INVALID_PARAM;
    }

    *pu8DutyCycle = s_u8CurrentDuty;
    return PWM_STATUS_OK;
}
