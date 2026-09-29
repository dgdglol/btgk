#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "app_telemetry.h"

/* Khoi tao module telemetry */
TelemetryStatus_te Telemetry_Init(void)
{
    return TELEMETRY_STATUS_OK;
}

/* Gui ban tin telemetry JSON qua UART */
TelemetryStatus_te Telemetry_SendTx(const LightData_ts *pData)
{
    char szBuffer[128];
    const char *szStatus = "OK";

    if (pData == NULL)
    {
        return TELEMETRY_STATUS_INVALID_PARAM;
    }

    if (pData->eState == LIGHT_STATE_SAFE_MODE)
    {
        szStatus = "SAFE_MODE";
    }

    (void)snprintf(szBuffer, sizeof(szBuffer),
                   "{\"lux\": %.1f, \"light_duty\": %u, \"status\": \"%s\"}\r\n",
                   (double)pData->f32FilteredLux,
                   (unsigned int)pData->u8DutyCycle,
                   szStatus);

    (void)printf("[UART TX] %s", szBuffer);
    return TELEMETRY_STATUS_OK;
}

/* Xu ly lenh nhan tu UART */
TelemetryStatus_te Telemetry_ProcessRx(const char *pRxCmd)
{
    if (pRxCmd == NULL)
    {
        return TELEMETRY_STATUS_INVALID_PARAM;
    }

    if (strcmp(pRxCmd, "SET_MODE=AUTO") == 0)
    {
        LightCtrl_SetMode(LIGHT_MODE_AUTO);
        (void)printf("[UART RX] Set mode: AUTO\n");
        return TELEMETRY_STATUS_OK;
    }

    if (strcmp(pRxCmd, "SET_MODE=MANUAL") == 0)
    {
        LightCtrl_SetMode(LIGHT_MODE_MANUAL);
        (void)printf("[UART RX] Set mode: MANUAL\n");
        return TELEMETRY_STATUS_OK;
    }

    if (strncmp(pRxCmd, "SET_LEVEL=", 10) == 0)
    {
        int s32Level = atoi(&pRxCmd[10]);
        if ((s32Level >= 0) && (s32Level <= 100))
        {
            LightCtrl_SetManualDuty((uint8_t)s32Level);
            (void)printf("[UART RX] Set manual light level: %d%%\n", s32Level);
            return TELEMETRY_STATUS_OK;
        }
    }

    (void)printf("[UART RX] Unknown or invalid command: %s\n", pRxCmd);
    return TELEMETRY_STATUS_CMD_ERROR;
}
