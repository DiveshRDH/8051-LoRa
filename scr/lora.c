#include "MS51_16K.h"
#include <stdio.h>
#include "lora.h"

#define NSS     P15
#define DIO0    P12
#define RESET   P13

void SPI_Write(unsigned char reg, unsigned char val)
{
    NSS = 0;
    Spi_Write_Byte(reg | 0x80);
    Spi_Write_Byte(val);
    NSS = 1;
}

unsigned char SPI_Read(unsigned char reg)
{
    unsigned char val;
    NSS = 0;
    Spi_Write_Byte(reg & 0x7F);
    val = Spi_Read_Byte(0xFF);
    NSS = 1;
    return val;
}

void LoRa_Reset(void)
{
    RESET = 0;
    Timer0_Delay(16000000, 1, 100);
    RESET = 1;
    Timer0_Delay(16000000, 1, 100);
}

bit LoRa_Check(void)
{
    unsigned char version = SPI_Read(0x42);  // RegVersion
    return version == 0x12 ? 1 : 0;
}

void LoRa_Init(void)
{
    P15_QUASI_MODE; P10_QUASI_MODE; P00_QUASI_MODE; P01_QUASI_MODE;
    P12_QUASI_MODE; P13_QUASI_MODE;

    set_SPSR_DISMODF;
    clr_SPCR_SSOE;
    clr_SPCR_LSBFE;
    clr_SPCR_CPOL;
    clr_SPCR_CPHA;
    set_SPCR_MSTR;
    SPICLK_FSYS_DIV16;
    set_SPCR_SPIEN;
    clr_SPSR_SPIF;

    LoRa_Reset();

    SPI_Write(0x01, 0x80);  // Sleep
    Timer0_Delay(16000000, 1, 10);
    SPI_Write(0x01, 0x81);  // Standby
    Timer0_Delay(16000000, 1, 10);

    // 433 MHz
    SPI_Write(0x06, 0x6C);
    SPI_Write(0x07, 0x80);
    SPI_Write(0x08, 0x00);

    SPI_Write(0x09, 0xFF);      // Max TX power
    SPI_Write(0x0E, 0x00);      // FIFO TX base
    SPI_Write(0x0D, 0x00);      // FIFO ptr
    SPI_Write(0x12, 0xFF);      // Clear IRQ
}

void LoRa_Send(char* msg, unsigned char len)
{
    unsigned char i;

    SPI_Write(0x0D, 0x00);
    NSS = 0;
    Spi_Write_Byte(0x00 | 0x80);
    for (i = 0; i < len; i++) Spi_Write_Byte(msg[i]);
    NSS = 1;

    SPI_Write(0x22, len);    // Payload length
    SPI_Write(0x01, 0x83);   // TX mode

    while (DIO0 == 0);
    SPI_Write(0x12, 0x08);   // Clear TxDone
}

unsigned char LoRa_Receive(char* buf, unsigned char maxLen)
{
    unsigned char i, len, addr;

    SPI_Write(0x0F, 0x00);  // RX base
    SPI_Write(0x0D, 0x00);  // FIFO ptr
    SPI_Write(0x01, 0x85);  // RX continuous

    while (DIO0 == 0);      // Wait RxDone

    len = SPI_Read(0x13);
    addr = SPI_Read(0x10);
    if (len > maxLen) len = maxLen;

    SPI_Write(0x0D, addr);

    NSS = 0;
    Spi_Write_Byte(0x00 & 0x7F);
    for (i = 0; i < len; i++) buf[i] = Spi_Read_Byte(0xFF);
    NSS = 1;

    SPI_Write(0x12, 0x40);  // Clear RxDone
    return len;
}
