#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "app_light_ctrl.h"
#include "app_telemetry.h"
#include "driver_adc.h"
#include "driver_pwm.h"

/* Chuong trinh chinh mo phong he thong firmware chieu sang thong minh */
int main(void)
{
    LightData_ts SystemData;
    uint32_t u32Tick100ms = 0U;

    (void)printf("=== HE THONG GIOI HAN & DIEU KHIEN CHIEU SANG THONG MINH (SLSC) ===\n\n");

    (void)LightCtrl_Init();
    (void)Telemetry_Init();

    /* Kich ban 1: Troi chuyen dan tu sang den toi o che do AUTO */
    (void)printf("--- GIAI DOAN 1: CHE DO AUTO (Chieu sang tu dong theo cam bien) ---\n");
    uint16_t aSimulatedLuxRaw[] = {
        2000U, 1900U, 1800U, 1700U, 1640U, /* ~800 Lux: troi sang */
        1200U, 1100U, 1000U, 950U,  900U,  /* ~450 Lux: buoi chieu */
        300U,  250U,  200U,  180U,  150U   /* ~80 Lux: troi toi */
    };
    uint32_t u32TotalSamples = (uint32_t)(sizeof(aSimulatedLuxRaw) / sizeof(aSimulatedLuxRaw[0]));

    for (uint32_t u32Idx = 0U; u32Idx < u32TotalSamples; u32Idx++)
    {
        Adc_SetMockValue(aSimulatedLuxRaw[u32Idx], false);
        LightCtrl_ProcessLoop();
        u32Tick100ms++;

        if ((u32Tick100ms % 5U) == 0U)
        {
            (void)LightCtrl_GetData(&SystemData);
            (void)Telemetry_SendTx(&SystemData);
        }
    }

    /* Kich ban 2: Nhan lenh qua UART */
    (void)printf("\n--- GIAI DOAN 2: DIEU KHIEN THU CONG QUA LENH UART ---\n");
    (void)Telemetry_ProcessRx("SET_MODE=MANUAL");
    (void)Telemetry_ProcessRx("SET_LEVEL=75");
    LightCtrl_ProcessLoop();
    (void)LightCtrl_GetData(&SystemData);
    (void)Telemetry_SendTx(&SystemData);

    (void)Telemetry_ProcessRx("SET_MODE=AUTO");
    LightCtrl_ProcessLoop();

    /* Kich ban 3: Kich hoat che do an toan Failsafe khi mat tin hieu cam bien */
    (void)printf("\n--- GIAI DOAN 3: XU LY SU CO MAT CAM BIEN (FAILSAFE > 300ms) ---\n");
    Adc_SetMockValue(0U, true);

    for (uint32_t u32FaultTick = 0U; u32FaultTick < 4U; u32FaultTick++)
    {
        LightCtrl_ProcessLoop();
        u32Tick100ms++;
        (void)LightCtrl_GetData(&SystemData);
        (void)printf("[Tick %u] Trang thai: %s | Duty: %u%% | Co loi: %s\n",
                     u32FaultTick + 1U,
                     (SystemData.eState == LIGHT_STATE_SAFE_MODE) ? "SAFE_MODE" : "NORMAL",
                     SystemData.u8DutyCycle,
                     SystemData.bIsFault ? "TRUE" : "FALSE");
    }

    (void)Telemetry_SendTx(&SystemData);

    (void)printf("\n=== MO PHONG HOAN TAT THANH CONG ===\n");
    return 0;
}
