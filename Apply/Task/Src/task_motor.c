#include "task_conf.h"

#include "bsp_io.h"


/**
 * @brief 电机初始化
 * @param null
 * @retval null
*/
void Task_Motor_Init(void)
{
	Drv_Delay_Ms(6000);		/* 初始停转信号后等待稳定 */
	
	/* 停转，1500hz(7.5%占空比) */
	Drv_PMW_FreqSet(&tPWMDemo[0], STOP_PWM_VALUE);
	Drv_PMW_FreqSet(&tPWMDemo[1], STOP_PWM_VALUE);
	Drv_PMW_FreqSet(&tPWMDemo[2], STOP_PWM_VALUE);
	Drv_PMW_FreqSet(&tPWMDemo[3], STOP_PWM_VALUE);
	Drv_Delay_Ms(2000);
}

/**
 * @brief 电机转速设置子函数
 * @param index 选择几号推进器
 * @param _freq 频率大小	_freq可设置为500-2500,1500停止
							500-1500 逆时针旋转 1500-2500顺时针旋转
 * @retval null
*/
void Task_Motor_SpeedSet(int index,uint16_t _freq)
{
	Drv_PMW_FreqSet(&tPWMDemo[index-1], _freq);
}	

/**
 * @brief 电机开始工作
 * @param index 选择几号推进器
 * @param _freq 频率大小
 * @retval null
*/
void Task_Motor_Start(int index,uint16_t _freq)
{
	/* 电机运行速度设置 */
	Task_Motor_SpeedSet(index,_freq);
}

/**
 * @brief 所有电机开始工作
 * @param *adress 存放四个推进器的分量数组的首地址
 * @retval null
*/
void Task_Motor_AllStart(float *adress)
{
	Task_Motor_Start(1,*adress+STOP_PWM_VALUE);
	Task_Motor_Start(2,*(adress+1)+STOP_PWM_VALUE);
	Task_Motor_Start(3,*(adress+2)+STOP_PWM_VALUE);
	Task_Motor_Start(4,*(adress+3)+STOP_PWM_VALUE);
}

/**
 * @brief 电机停止工作
 * @param index 选择几号推进器
 * @param _fVal 占空比大小
 * @retval null
*/
void Task_Motor_Stop(int index)
{
	/* 电机运行速度设置 */
	Drv_PMW_FreqSet(&tPWMDemo[index-1], STOP_PWM_VALUE);
}

/**
 * @brief 所有电机停止工作
 * @param null
 * @retval null
*/
void Task_Motor_AllStop(void)
{
	Task_Motor_Stop(1);
	Task_Motor_Stop(2);
	Task_Motor_Stop(3);
	Task_Motor_Stop(4);
}

