// #include <main.h>
#include <ch32v20x.h>
#include <debug.h>
#include <my_sys.h>
#include <my_time.h>
#include <my_uarts.h>
#include <stdlib.h>

#include <w25qxx.h>
#include <io.hpp>

// SYSCLK_FREQ_96MHz_HSI !!!

uint32_t ti_A = 0;
uint32_t ti_B = 0;
uint32_t ti_C = 0;

uint8_t test_FF = 1;
uint8_t* p_test_FF = &test_FF;

bool mas_FF[8];
bool* p_mas_FF = mas_FF;
bool* bit = (bool*)&test_FF;

bool EN_write_diag = 0;  // temp

uint32_t my_id_4;

extern "C" void
NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));




//=====================================================//
//для сылок 
// #define SPI_DELAY 100

// void SPISendBytes(uint8_t* sendData, uint32_t length)
// {
//     uint32_t loop = 0;
//     uint8_t tmp = 0;
//     for (loop = 0; loop < length; loop++)
//     {
//         // Send SPI Byte
//         while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  // wait while flag is zero or TX buffer not empty
//         SPI_I2S_SendData(SPI1, sendData[loop]);

//         // Receive SPI Byte
//         while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);  // wait while flag is zero or RX buffer is empty
//         tmp = SPI_I2S_ReceiveData(SPI1);
//     }
// }
//=====================================================//




//== MAIN == == MAIN == == MAIN == == MAIN ==//
void MAIN_WHILE()
{

    if (millis() - ti_A > 400)
    {
        ti_A = millis();
        IO::ToggleOutput(C_13);
    }

    if (millis() - ti_B > 900)
    {
        ti_B = millis();

        if (my_id_4)
        {
            U1::UART_printNum(my_id_4);
            my_id_4 = 0;
        }
        //blink_6();

        // uint8_t str_44[] = " temp";
        // U1::UART_write_byteS(str_44, sizeof(str_44));
        //U1::UART_printNum(222);
        
    }
}
//=========== END MAIN ======================//

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    //SystemCoreClockUpdate();
    my_clock_config_HSE_Q8_96M();
    Delay_Init();
    GPIO_Init();
    //TIM2_Init(); // stepper motor
    TIM3_Init(); // millis() micros()
    U1::UART_Init(115200);
    SPI_FullDuplex_Init();

    //==================================================================

    U1::UART_println("start flash test");

    //====== ИНИЦИАЛИЗАЦИЯ ФЛЕШКИ ======//
    W25qxx_Init();

    //====== ОЧИСТКА ВСЕЙ ФЛЕШКИ ======//
     //W25qxx_EraseChip();
     
     //====== ОЧИСТКА БЛОКА ======//
     // W25qxx_EraseBlock(0);  // 65536 байт

     //====== ОЧИСТКА СЕКТОРА ======//
      W25qxx_EraseSector(0); // 4096 байт

     uint8_t buf[64];
     buf[0] = 0;
     buf[1] = 0;
     buf[2] = 0;
     buf[3] = 0;
     buf[4] = 0;
     buf[5] = 0;
     buf[6] = 0;
     buf[7] = 0;

     //ok
     ///*
     W25qxx_ReadByte(&buf[0], 22);
     W25qxx_ReadByte(&buf[1], 23);
     W25qxx_ReadByte(&buf[2], 24);
     W25qxx_ReadByte(&buf[3], 25);  //
     W25qxx_ReadByte(&buf[4], 26);  //
     W25qxx_ReadByte(&buf[5], 27);  //
     W25qxx_ReadByte(&buf[6], 28);
     W25qxx_ReadByte(&buf[7], 29);
     //*/

     //W25qxx_ReadBytes(buf, 22, 8);  // <=

     U1::UART_println("clean test...");
     U1::UART_printNum(buf[0]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[1]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[2]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[3]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[4]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[5]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[6]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[7]);
     U1::UART_print(" ");
     U1::UART_println("!");

     //====== ЗАПИСЬ ПОБАЙТНО ======//
     // uint8_t b0 = 's';
     // uint8_t b1 = 't';
     // uint8_t b2 = 'D';

     uint8_t a0 = 8;
     uint8_t a1 = 16;  //
     uint8_t a2 = 32;

     W25qxx_WriteByte(a0, 24);
     W25qxx_WriteByte(a1, 26);
     W25qxx_WriteByte(a2, 28);

     //====== ЧТЕНИЕ ПОБАЙТНО ======//

     buf[0] = 0;
     buf[1] = 0;
     buf[2] = 0;

     W25qxx_ReadBytes(buf, 22, 8);// <=

     U1::UART_println("write test...");

     Delay_Ms(400);
     U1::UART_printNum(buf[0]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[1]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[2]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[3]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[4]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[5]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[6]);
     U1::UART_print(" ");
     U1::UART_printNum(buf[7]);
     U1::UART_print(" ");
     U1::UART_println("!");
     U1::UART_println(" ");

     while (1)
     {
         MAIN_WHILE();
         // blink();
     }
}

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}
