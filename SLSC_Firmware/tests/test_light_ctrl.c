#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "app_light_ctrl.h"

typedef struct
{
    float f32InputLux;
    uint8_t *pu8OutputDuty;
    LightStatus_te eExpectedStatus;
    uint8_t u8ExpectedDuty;
    const char *szDescription;
} TestCase_ts;

/* Ham thuc thi toan bo testbench kiem thu tinh Duty Cycle */
int main(void)
{
    uint8_t u8ActualDuty = 0U;
    uint32_t u32PassedCount = 0U;
    uint32_t u32TotalTests = 0U;

    TestCase_ts aTestCases[] = {
        {-10.0f,  &u8ActualDuty, LIGHT_STATUS_SENSOR_FAULT,   0U,   "Ngoai dai do am (Fault)"},
        {0.0f,    &u8ActualDuty, LIGHT_STATUS_OK,             100U, "Troi toi hoan toan (0 Lux)"},
        {50.0f,   &u8ActualDuty, LIGHT_STATUS_OK,             100U, "Troi rat toi (50 Lux)"},
        {99.9f,   &u8ActualDuty, LIGHT_STATUS_OK,             100U, "Can duoi nguong tuyen tinh (99.9 Lux)"},
        {100.0f,  &u8ActualDuty, LIGHT_STATUS_OK,             90U,  "Moc bat dau tuyen tinh (100.0 Lux)"},
        {450.0f,  &u8ActualDuty, LIGHT_STATUS_OK,             50U,  "Diem giua dải tuyen tinh (450.0 Lux)"},
        {800.0f,  &u8ActualDuty, LIGHT_STATUS_OK,             10U,  "Moc ket thuc tuyen tinh (800.0 Lux)"},
        {800.1f,  &u8ActualDuty, LIGHT_STATUS_OK,             0U,   "Can tren ngoai tuyen tinh (800.1 Lux)"},
        {1200.0f, &u8ActualDuty, LIGHT_STATUS_OK,             0U,   "Troi nang sang (1200.0 Lux)"},
        {2500.0f, &u8ActualDuty, LIGHT_STATUS_SENSOR_FAULT,   0U,   "Vuot gioi han do cam bien (2500.0 Lux)"},
        {300.0f,  NULL,          LIGHT_STATUS_INVALID_PARAM,  0U,   "Con tro dau ra NULL (Invalid Param)"}
    };

    u32TotalTests = (uint32_t)(sizeof(aTestCases) / sizeof(aTestCases[0]));

    (void)printf("==================================================\n");
    (void)printf("      RUNNING UNIT TESTS FOR LIGHT CONTROLLER     \n");
    (void)printf("==================================================\n");

    for (uint32_t u32Idx = 0U; u32Idx < u32TotalTests; u32Idx++)
    {
        TestCase_ts *pCase = &aTestCases[u32Idx];
        u8ActualDuty = 0xFFU;

        LightStatus_te eStatus = LightCtrl_CalculateDuty(pCase->f32InputLux, pCase->pu8OutputDuty);

        bool bStatusMatch = (eStatus == pCase->eExpectedStatus);
        bool bDutyMatch = true;

        if (eStatus == LIGHT_STATUS_OK)
        {
            bDutyMatch = (u8ActualDuty == pCase->u8ExpectedDuty);
        }

        if (bStatusMatch && bDutyMatch)
        {
            (void)printf("[PASS] Test %2u: %-40s | Status=%d, Duty=%u%%\n",
                         u32Idx + 1U, pCase->szDescription, (int)eStatus,
                         (eStatus == LIGHT_STATUS_OK) ? u8ActualDuty : 0U);
            u32PassedCount++;
        }
        else
        {
            (void)printf("[FAIL] Test %2u: %-40s | Expected [Status=%d, Duty=%u%%], Got [Status=%d, Duty=%u%%]\n",
                         u32Idx + 1U, pCase->szDescription,
                         (int)pCase->eExpectedStatus, pCase->u8ExpectedDuty,
                         (int)eStatus, u8ActualDuty);
        }
    }

    (void)printf("==================================================\n");
    (void)printf("Test Result: %u / %u tests passed (%.1f%%)\n",
                 u32PassedCount, u32TotalTests,
                 ((float)u32PassedCount / (float)u32TotalTests) * 100.0f);
    (void)printf("==================================================\n");

    return (u32PassedCount == u32TotalTests) ? 0 : 1;
}
