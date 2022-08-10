/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_adc.c

* ���ݼ�����ADC�����ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��

* 1.0.0a 	2020-02-22	���		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_ADC_ENABLE

#define POLL_NUM	10		/* ��ѯ���� */

/**
 * @brief ADCʱ�����ú���
 * @param NULL
 * @retval Null
*/
static void S_ADC_CLKConfig(void)
{
#ifdef STM32F1_SGA_ENABLE
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
#endif
}

/**
 * @brief ADCʱ��ʹ�ܺ���
 * @param *_tADC-ADC�ṹ��ָ��
 * @retval Null
*/
static void S_ADC_CLKEnable(tagADC_T *_tADC)
{
	if(_tADC->tADCHandle.Instance == ADC1)
	{
#ifdef STM32F1_SGA_ENABLE
		__HAL_RCC_ADC1_CLK_ENABLE();
#endif
	}
	else if(_tADC->tADCHandle.Instance == ADC2)
	{
#ifdef STM32F1_SGA_ENABLE
		__HAL_RCC_ADC2_CLK_ENABLE();
#endif
	}
	else if(_tADC->tADCHandle.Instance == ADC3)
	{
#ifdef STM32F1_SGA_ENABLE
		__HAL_RCC_ADC3_CLK_ENABLE();
#endif
	}
}

/**
 * @brief ADC�������ú���
 * @param *_adcHandle-ADC���ָ��
 * @retval Null
*/
static void S_ADC_GPIOConfig(tagADC_T *_tADC)
{
	Drv_GPIO_Init(&_tADC->tGPIO, 1);
}

/**
 * @brief ADC�������ú���
 * @param *_adcHandle-ADC���ָ��
 * @retval Null
*/
static void S_ADC_ParamConfig(tagADC_T *_tADC)
{
	if (HAL_ADC_Init(&_tADC->tADCHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}

#ifdef STM32F1_SGA_ENABLE	
	HAL_ADCEx_Calibration_Start(&_tADC->tADCHandle);				  /* У׼ADC */
#endif
	
	if (HAL_ADC_ConfigChannel(&_tADC->tADCHandle, &_tADC->tADCChannel) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief ADC��ʼ������
 * @param *_adcHandle-ADC���ָ��
 * @retval Null
*/
void Drv_ADC_Init(tagADC_T *_tADC, uint8_t _ucNum)
{
	uint8_t index;
	
	for(index = 0; index < _ucNum; index++)
	{
		S_ADC_CLKConfig();					/* adcʱ�������� */
		S_ADC_CLKEnable(&_tADC[index]);		/* adcʱ��ʹ�� */
		S_ADC_GPIOConfig(&_tADC[index]);	/* adc��GPIO���� */
		S_ADC_ParamConfig(&_tADC[index]);	/* adc�������� */
	}
}

/**
 * @brief ADC��ѯ��ȡ����
 * @param *_tADC-ADC���ָ��
 * @retval uint16_t-��ȡ����ADCֱ��ֵ
*/
uint16_t Drv_ADC_PollGetData(tagADC_T *_tADC)
{
	HAL_ADC_ConfigChannel(&_tADC->tADCHandle, &_tADC->tADCChannel);	/* ͨ������ */
		
	HAL_ADC_Start(&_tADC->tADCHandle);								/* ����ADC */
	HAL_ADC_PollForConversion(&_tADC->tADCHandle, POLL_NUM);		/* ��ѯת�� */
	
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&_tADC->tADCHandle),HAL_ADC_STATE_REG_EOC))
		return (uint16_t)HAL_ADC_GetValue(&_tADC->tADCHandle);
	else	
		return (uint16_t)HAL_ADC_GetState(&_tADC->tADCHandle);
}

/**
 * @brief ADC��ѯ��ȡ����
 * @param *_tADC-ADC���ָ��
 * @retval float-��ȡ����ADCת�����ʵ��ֵ
*/
float Drv_ADC_PollGetValue(tagADC_T *_tADC)
{
	uint32_t ulADCData = 0;
	uint8_t ucNum;
	float fADCVal;
	
	/* ѭ����ȡ��ȡƽ��ֵ */
	for(ucNum = 0;ucNum < _tADC->ucAverageNum;ucNum++)
	{
		ulADCData += Drv_ADC_PollGetData(_tADC);		/* �������һ��ADC1�������ת����� */
		Drv_Delay_Ms(5);
	}
	
	ulADCData /= _tADC->ucAverageNum;	/* ȡƽ��ֵ */
	fADCVal = (float)ulADCData * (3.3 / 4096);	/* ��ʽ���� */
	
	return fADCVal;
}

#endif