#ifndef APP_LIGHT_CTRL_H
#define APP_LIGHT_CTRL_H

#include <stdint.h>
#include <stdbool.h>

#define LIGHT_CTRL_MIN_LUX_THRESHOLD        (100.0f)
#define LIGHT_CTRL_MAX_LUX_THRESHOLD        (800.0f)
#define LIGHT_CTRL_SENSOR_MAX_LUX           (2000.0f)
#define LIGHT_CTRL_SENSOR_MIN_LUX           (0.0f)
#define LIGHT_CTRL_WINDOW_SIZE              (5U)
#define LIGHT_CTRL_MAX_FAILSAFE_COUNT       (3U)

typedef enum
{
    LIGHT_MODE_AUTO = 0,
    LIGHT_MODE_MANUAL
} LightMode_te;

typedef enum
{
    LIGHT_STATE_NORMAL = 0,
    LIGHT_STATE_SAFE_MODE
} LightState_te;

typedef enum
{
    LIGHT_STATUS_OK = 0,
    LIGHT_STATUS_INVALID_PARAM,
    LIGHT_STATUS_SENSOR_FAULT
} LightStatus_te;

typedef struct
{
    float f32CurrentLux;
    float f32FilteredLux;
    uint8_t u8DutyCycle;
    LightMode_te eMode;
    LightState_te eState;
    bool bIsNightMode;
    bool bIsFault;
} LightData_ts;

/**
 * @brief Khoi tao module dieu khien chieu sang thong minh.
 * @return LightStatus_te:
 * - LIGHT_STATUS_OK: Khoi tao thanh cong.
 */
LightStatus_te LightCtrl_Init(void);

/**
 * @brief Tinh toan gia tri Duty Cycle cho PWM dua tren do sang dau vao.
 * @param[in] f32Lux: Do sang hien tai (don vi: Lux).
 * @param[out] pu8DutyCycle: Con tro chua ket qua Duty Cycle (0 - 100%).
 * @return LightStatus_te:
 * - LIGHT_STATUS_OK: Tinh toan thanh cong.
 * - LIGHT_STATUS_INVALID_PARAM: Con tro dau ra bi NULL.
 * - LIGHT_STATUS_SENSOR_FAULT: Gia tri do sang vuot dai do cho phep (< 0 hoac > 2000).
 */
LightStatus_te LightCtrl_CalculateDuty(float f32Lux, uint8_t *pu8DutyCycle);

/**
 * @brief Ham xu ly logic chinh theo chu ky 100ms (doc cam bien, tinh toan va dieu khien PWM).
 */
void LightCtrl_ProcessLoop(void);

/**
 * @brief Thiet lap che do hoat dong (Tu dong hoac Thu cong).
 * @param[in] eNewMode: Che do can thiet lap.
 */
void LightCtrl_SetMode(LightMode_te eNewMode);

/**
 * @brief Thiet lap muc cong suat den khi o che do thu cong.
 * @param[in] u8DutyCycle: Gia tri cong suat (0 - 100%).
 */
void LightCtrl_SetManualDuty(uint8_t u8DutyCycle);

/**
 * @brief Lay du lieu trang thai hien tai cua he thong.
 * @param[out] pOutData: Con tro toi cau truc luu du lieu trang thai.
 * @return LightStatus_te:
 * - LIGHT_STATUS_OK: Lay du lieu thanh cong.
 * - LIGHT_STATUS_INVALID_PARAM: Con tro pOutData bi NULL.
 */
LightStatus_te LightCtrl_GetData(LightData_ts *pOutData);

#endif /* APP_LIGHT_CTRL_H */
