#ifndef IO_HPP_
#define IO_HPP_

// ВНИМАНИЕ!!! ПОМАНЯТЬ НА СВОЙ МК
#include "ch32v20x_gpio.h"

/*
//заполнить
#define INPUT01
#define INPUT02


*/

#define RS485_rw GPIOB, GPIO_Pin_5

#define C_13 GPIOC, GPIO_Pin_13

#define B_12 GPIOB, GPIO_Pin_12
#define B_13 GPIOB, GPIO_Pin_13

#define B_14 GPIOB, GPIO_Pin_14
#define B_15 GPIOB, GPIO_Pin_15

#define ROL_sw GPIOB, GPIO_Pin_4 // roll open limit switch

#define buzz GPIOA, GPIO_Pin_6 // roll main rele

#define rol_main GPIOA, GPIO_Pin_3  // roll main rele
#define rol_open GPIOA, GPIO_Pin_5  // roll open rele
#define rol_close GPIOA, GPIO_Pin_4 // roll close rele

#define act_open GPIOA, GPIO_Pin_7  // actuator open rele
#define act_close GPIOB, GPIO_Pin_0 // actuator close rele
/*
//ПРИМЕРЫ:

//запись
IO::SetOutput(A_11, 1);
IO::SetOutput(A_12, 0);

//чтение
bool x = IO::GetInput(A_11);
*/

class IO
{
public:
    static bool GetInput(GPIO_TypeDef *const GPIOx, const uint16_t GPIO_Pin)
    {
        return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_SET;
    }

    static void SetOutput(GPIO_TypeDef *const GPIOx, const uint16_t GPIO_Pin, const bool PinState)
    {
        GPIO_WriteBit(GPIOx, GPIO_Pin, PinState ? Bit_SET : Bit_RESET);
    }

    static bool GetOutput(GPIO_TypeDef *const GPIOx, const uint16_t GPIO_Pin)
    {
        return GetInput(GPIOx, GPIO_Pin);
    }

    static void ToggleOutput(GPIO_TypeDef *const GPIOx, const uint16_t GPIO_Pin)
    {
        SetOutput(GPIOx, GPIO_Pin, !GetInput(GPIOx, GPIO_Pin));
    }
};

#endif /* IO_HPP_ */