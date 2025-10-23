#ifndef MY_UARTS_H_
#define MY_UARTS_H_

#include <ch32v20x.h>
#include <io.hpp>
#include "stdio.h"
#include "string.h"

uint8_t uart_rx_bf[2048] = {0};
uint8_t uart_pntr = 0;

namespace U1 // UART 1
{
    void UART_Init(uint32_t u_speed) // Remap
    {
        GPIO_InitTypeDef GPIO_InitStructure = {0};
        USART_InitTypeDef USART_InitStructure = {0};
        // NVIC_InitTypeDef  NVIC_InitStructure = {0};

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
        /* USART1 TX_1-->B.6   RX_1-->B.7  rw-->B.5*/

        /* USART1 TX-->A.9   RX-->A.10 */
        // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        // GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        USART_InitStructure.USART_BaudRate = u_speed;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

        USART_Init(USART1, &USART_InitStructure);
        // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

        // NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        // NVIC_Init(&NVIC_InitStructure);

        USART_Cmd(USART1, ENABLE);
    }

    void UART_write_byte(uint8_t _byte)
    {
        // if(!UART1_tx_busy())
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        USART_SendData(USART1, _byte);
       // USART1->DATAR = _byte & (uint16_t)0x01FF;
    }

    void UART_write_byteS(uint8_t* mass, uint8_t lenght)
    {
        for (int i = 0; i < lenght; i++)
        {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
            USART_SendData(USART1, mass[i]);
            // transmit complete
            delay_ms(1);
                }
    }

    void UART_print(char* message)
    {
        UART_write_byteS((uint8_t*)message, strlen(message));
    }

    void UART_println(char* message)
    {
        UART_write_byteS((uint8_t*)message, strlen(message));
        UART_write_byte(10); /* \n */
    }

    void UART_printNum(uint8_t number)
    {
        char xstr[18];  // 4 млдр = 1+3*3 = 10 цифр + '\0'
        sprintf(xstr, "%d", number);
        UART_write_byteS((uint8_t*)xstr, strlen(xstr));
    }

    void rs485_write_byte(uint8_t _byte)
    {
        IO::SetOutput(RS485_rw, 1);

        USART_SendData(USART1, _byte);
        // transmit complete
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;

        IO::SetOutput(RS485_rw, 0);
    }

    void rs485_write_byteS(uint8_t *mass, uint8_t lenght)
    {
        IO::SetOutput(RS485_rw, 1);

        for (int i = 0; i < lenght; i++)
        {
            USART_SendData(USART1, mass[i]);
            // transmit complete
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                ;
        }

        IO::SetOutput(RS485_rw, 0);
    }

    void rs485_print(char *message)
    {
        rs485_write_byteS((uint8_t *)message, strlen(message));
    }

    void rs485_println(char *message)
    {
        rs485_write_byteS((uint8_t *)message, strlen(message));
        rs485_write_byte(10); /* \n */
    }

    void rs485_printNum(uint8_t number)
    {
        char xstr[11]; // 4 млдр = 1+3*3 = 10 цифр + '\0'
        sprintf(xstr, "%d", number);
        rs485_write_byteS((uint8_t *)xstr, strlen(xstr));
    }


}

namespace U2 // UART 2
{
    void UART_Init(uint32_t u_speed) // Remap
    {
        GPIO_InitTypeDef GPIO_InitStructure = {0};
        USART_InitTypeDef USART_InitStructure = {0};
        // NVIC_InitTypeDef  NVIC_InitStructure = {0};

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

        /* USART1 TX_1-->B.6   RX_1-->B.7  rw-->B.5*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        USART_InitStructure.USART_BaudRate = u_speed;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

        USART_Init(USART1, &USART_InitStructure);
        // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

        // NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        // NVIC_Init(&NVIC_InitStructure);

        USART_Cmd(USART1, ENABLE);
    }

    void UART_write_byte(uint8_t _byte)
    {
        // if(!UART1_tx_busy())
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        USART_SendData(USART1, _byte);
        USART1->DATAR = _byte & (uint16_t)0x01FF;
    }

    void rs485_write_byte(uint8_t _byte)
    {
        IO::SetOutput(RS485_rw, 1);

        USART_SendData(USART1, _byte);
        // transmit complete
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;

        IO::SetOutput(RS485_rw, 0);
    }

    void rs485_write_byteS(uint8_t *mass, uint8_t lenght)
    {
        IO::SetOutput(RS485_rw, 1);

        for (int i = 0; i < lenght; i++)
        {
            USART_SendData(USART1, mass[i]);
            // transmit complete
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                ;
        }

        IO::SetOutput(RS485_rw, 0);
    }

}

namespace U3 // UART 3
{
    void UART_Init(uint32_t u_speed) // Remap
    {
        GPIO_InitTypeDef GPIO_InitStructure = {0};
        USART_InitTypeDef USART_InitStructure = {0};
        // NVIC_InitTypeDef  NVIC_InitStructure = {0};

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

        /* USART1 TX_1-->B.6   RX_1-->B.7  rw-->B.5*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        USART_InitStructure.USART_BaudRate = u_speed;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

        USART_Init(USART1, &USART_InitStructure);
        // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

        // NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        // NVIC_Init(&NVIC_InitStructure);

        USART_Cmd(USART1, ENABLE);
    }

    void UART_write_byte(uint8_t _byte)
    {
        // if(!UART1_tx_busy())
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        USART_SendData(USART1, _byte);
        USART1->DATAR = _byte & (uint16_t)0x01FF;
    }

    void rs485_write_byte(uint8_t _byte)
    {
        IO::SetOutput(RS485_rw, 1);

        USART_SendData(USART1, _byte);
        // transmit complete
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;

        IO::SetOutput(RS485_rw, 0);
    }

    void rs485_write_byteS(uint8_t *mass, uint8_t lenght)
    {
        IO::SetOutput(RS485_rw, 1);

        for (int i = 0; i < lenght; i++)
        {
            USART_SendData(USART1, mass[i]);
            // transmit complete
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                ;
        }

        IO::SetOutput(RS485_rw, 0);
    }

}

uint16_t crc_mdb(unsigned char *data, unsigned char length)
{
    int j;
    uint16_t reg_crc = 0xffff;
    while (length--)
    {
        reg_crc ^= *data++;
        for (j = 0; j < 8; j++)
        {
            if (reg_crc & 0x01) /* LSB(b0)=1 */
            {
                reg_crc = (reg_crc >> 1) ^ 0xA001;
            }
            else
            {
                reg_crc = reg_crc >> 1;
            }
        }
    }
    return reg_crc;
}

#endif /* MY_UARTS_H_ */