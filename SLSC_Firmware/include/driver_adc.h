#ifndef DRIVER_ADC_H
#define DRIVER_ADC_H

#include <stdint.h>
#include <stdbool.h>

#define ADC_MAX_RAW_VALUE           (4095U)
#define ADC_MAX_LUX_RANGE           (2000.0f)

typedef enum
{
    ADC_STATUS_OK = 0,
    ADC_STATUS_ERROR,
    ADC_STATUS_INVALID_PARAM
} AdcStatus_te;

/**
 * @brief Khoi tao module ADC cho cam bien anh sang.
 * @return AdcStatus_te:
 * - ADC_STATUS_OK: Khoi tao thanh cong.
 */
AdcStatus_te Adc_Init(void);

/**
 * @brief Doc gia tri ADC tho tu kenh cam bien.
 * @param[out] pu16AdcVal: Con tro luu gia tri tho (0 - 4095).
 * @return AdcStatus_te:
 * - ADC_STATUS_OK: Doc thanh cong.
 * - ADC_STATUS_INVALID_PARAM: Con tro dau ra NULL.
 * - ADC_STATUS_ERROR: Loi phan cung hoac mat ket noi cam bien.
 */
AdcStatus_te Adc_ReadRawValue(uint16_t *pu16AdcVal);

/**
 * @brief Chuyen doi gia tri ADC sang gia tri do sang (Lux).
 * @param[in] u16AdcVal: Gia tri ADC tho (0 - 4095).
 * @param[out] pf32Lux: Con tro luu gia tri do sang tinh duoc (Lux).
 * @return AdcStatus_te:
 * - ADC_STATUS_OK: Chuyen doi thanh cong.
 * - ADC_STATUS_INVALID_PARAM: Con tro dau ra NULL hoac gia tri ADC ngoai dai do.
 */
AdcStatus_te Adc_ConvertToLux(uint16_t u16AdcVal, float *pf32Lux);

/**
 * @brief Gia lap cap nhat gia tri ADC va trang thai loi cho viec chay thu nghiem.
 * @param[in] u16MockValue: Gia tri ADC gia lap.
 * @param[in] bSimulateFault: True de mo phong loi cam bien mat tin hieu.
 */
void Adc_SetMockValue(uint16_t u16MockValue, bool bSimulateFault);

#endif /* DRIVER_ADC_H */
