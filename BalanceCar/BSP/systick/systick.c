/**
  ******************************************************************************
  * @file    systick.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   ����SysTick��ϵͳʱ�����ʱ����ʵ��
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t _ms_tick = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  initialize systick ��1ms��
  * @param  None
  * @retval None
  */
void systick_init(void) 
{
  _ms_tick = 0;  
  
  /* SystemCoreClock / 1000     1ms�ж�һ�� */
	if(SysTick_Config(SystemCoreClock / 1000))
  {
    /*capture error*/
    while(1);
  }
}

/**
  * @brief  ��ȡϵͳ��ǰ��ms����ֵ
  * @param  None
  * @retval ϵͳ��ǰʱ��ms
  */
uint32_t getCurrentMillis(void)
{
  return _ms_tick;
}

/**
  * @brief  ��ȡϵͳ��ǰ��us����ֵ
  * @param  delay time
  * @retval ϵͳ��ǰʱ��us
  */
uint32_t getCurrentMicros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  //LL_SYSTICK_IsActiveCounterFlag();
  uint32_t m = getCurrentMillis(); 
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk)) {
    m = getCurrentMillis(); 
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}

/**
  * @brief  ��λϵͳ����
  * @param  None
  * @retval None
  */
void systick_reset(void) 
{
  _ms_tick = 0;
}

/**
  * @brief  delay (ms)
  * @param  delay time
  * @retval None
  */
//void delay_ms(uint32_t ms) 
//{ 
//	uint32_t target;
//	
//	target = millis() + ms;
//	while(millis() < target);
//} 
void delay_ms(uint32_t ms)
{
  while(ms--)
  {    
    for(int i = 0; i < 2000; ++i);
  }
}
/**
  * @brief  delay (us)
  * @param  delay us time
  * @retval None
  * @attention ��ʱ��us����Ӧ�ر�󣬷���ᵼ��nbTicks�ڼ���ʱ�������
  */
void delay_us(uint32_t us)
{
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
   uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}

/**
  * @brief  SYSTICK�жϷ�����
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//  _ms_tick++;
//}
