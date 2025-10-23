#ifndef MY_TIME_H_
#define MY_TIME_H_

#include <ch32v20x.h>

uint32_t _millis = 0;
uint32_t _micros = 0;
uint16_t _mic_mil = 0;

static uint8_t p_us = 0;
static uint16_t p_ms = 0;

extern "C" void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM3_Init() // millis micros
{
    TIM_TimeBaseInitTypeDef TIMBase_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /*SYSCLK_FREQ_96MHz_HSI tim=1ms*/
    TIMBase_InitStruct.TIM_Period = 15; // 190
    TIMBase_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIMBase_InitStruct.TIM_Prescaler = 5; // 500
    TIMBase_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIMBase_InitStruct);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM3, ENABLE);
}

uint32_t millis()
{
    return _millis;
}

uint32_t micros()
{
    return _micros;
}

void TIM3_IRQHandler(void)
{
    /* 1ms_timer */
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        _micros++;
        _mic_mil++;
        if (_mic_mil > 999)
        {
            _mic_mil = 0;
            _millis++;
        }

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //
    }
}

void delayInit(void)
{
    p_us = SystemCoreClock / 8000000;
    p_ms = (uint16_t)p_us * 1000;
}

void delay_us(uint32_t n)
{
    uint32_t i;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * p_us;

    SysTick->CMP = i;
    SysTick->CTLR |= (1 << 4);
    SysTick->CTLR |= (1 << 5) | (1 << 0);

    while ((SysTick->SR & (1 << 0)) != (1 << 0))
        ;
    SysTick->CTLR &= ~(1 << 0);
}

void delay_ms(uint32_t n)
{
    uint32_t i;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * p_ms;

    SysTick->CMP = i;
    SysTick->CTLR |= (1 << 4);
    SysTick->CTLR |= (1 << 5) | (1 << 0);

    while ((SysTick->SR & (1 << 0)) != (1 << 0))
        ;
    SysTick->CTLR &= ~(1 << 0);
}

#endif /* MY_TIME_H_ */