#ifndef APP_TELEMETRY_H
#define APP_TELEMETRY_H

#include <stdint.h>
#include <stdbool.h>
#include "app_light_ctrl.h"

typedef enum
{
    TELEMETRY_STATUS_OK = 0,
    TELEMETRY_STATUS_INVALID_PARAM,
    TELEMETRY_STATUS_CMD_ERROR
} TelemetryStatus_te;

/**
 * @brief Khoi tao module truyen thong Telemetry qua UART.
 * @return TelemetryStatus_te:
 * - TELEMETRY_STATUS_OK: Khoi tao thanh cong.
 */
TelemetryStatus_te Telemetry_Init(void);

/**
 * @brief Gui goi tin dinh ky JSON qua duong truyen UART (TX chu ky 1s).
 * @param[in] pData: Con tro toi du lieu he thong can dong goi JSON.
 * @return TelemetryStatus_te:
 * - TELEMETRY_STATUS_OK: Gui thanh cong.
 * - TELEMETRY_STATUS_INVALID_PARAM: Con tro du lieu NULL.
 */
TelemetryStatus_te Telemetry_SendTx(const LightData_ts *pData);

/**
 * @brief Xu ly va phan tich lenh dieu khien nhan duoc tu UART (RX).
 * @param[in] pRxCmd: Chuoi ky tu chua lenh nhan tu UART.
 * @return TelemetryStatus_te:
 * - TELEMETRY_STATUS_OK: Thuc thi lenh hop le.
 * - TELEMETRY_STATUS_CMD_ERROR: Cu phap lenh khong dung hoac tham so loi.
 */
TelemetryStatus_te Telemetry_ProcessRx(const char *pRxCmd);

#endif /* APP_TELEMETRY_H */
