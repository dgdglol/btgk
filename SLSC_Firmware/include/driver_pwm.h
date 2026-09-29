#ifndef DRIVER_PWM_H
#define DRIVER_PWM_H

#include <stdint.h>

#define PWM_MIN_DUTY_CYCLE          (0U)
#define PWM_MAX_DUTY_CYCLE          (100U)

typedef enum
{
    PWM_STATUS_OK = 0,
    PWM_STATUS_INVALID_PARAM
} PwmStatus_te;

/**
 * @brief Khoi tao kenh phat xung PWM cho den chieu sang.
 * @return PwmStatus_te:
 * - PWM_STATUS_OK: Khoi tao thanh cong.
 */
PwmStatus_te Pwm_Init(void);

/**
 * @brief Thiet lap do rong xung (Duty Cycle) cho dau ra den.
 * @param[in] u8DutyCycle: Ty le phan tram do sang (0 - 100%).
 * @return PwmStatus_te:
 * - PWM_STATUS_OK: Thiet lap thanh cong.
 * - PWM_STATUS_INVALID_PARAM: Gia tri vuot qua 100%.
 */
PwmStatus_te Pwm_SetDuty(uint8_t u8DutyCycle);

/**
 * @brief Lay gia tri Duty Cycle hien tai dang phat ra.
 * @param[out] pu8DutyCycle: Con tro luu gia tri Duty Cycle hien tai.
 * @return PwmStatus_te:
 * - PWM_STATUS_OK: Lay gia tri thanh cong.
 * - PWM_STATUS_INVALID_PARAM: Con tro dau ra NULL.
 */
PwmStatus_te Pwm_GetDuty(uint8_t *pu8DutyCycle);

#endif /* DRIVER_PWM_H */
