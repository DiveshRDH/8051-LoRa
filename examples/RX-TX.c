#include "MS51_16K.h"
#include <stdio.h>
#include "lora.h"

// === CONFIG ===
#define MODE_TX     // comment out for RX mode

void UART0_Init(void)
{
    MODIFY_HIRC(HIRC_24);
    P06_QUASI_MODE;
    UART_Open(24000000, UART0_Timer3, 115200);
    ENABLE_UART0_PRINTF;
}

void main(void)
{
    char buffer[64];
    unsigned char len;
    unsigned int counter = 0;  // Initialize counter

    UART0_Init();
    printf("LoRa Example Start\n");

    LoRa_Init();

    if (!LoRa_Check()) {
        printf("LoRa module NOT detected!\n");
        while (1);
    }
    printf("LoRa module OK\n");

#ifdef MODE_TX
    while (1)
    {
        sprintf(buffer, "Count: %u", counter);  // Convert counter to string
        printf("Sending: %s\n", buffer);
        LoRa_Send(buffer, strlen(buffer));
        printf("Sent!\n");
        counter++;  // Increment counter
        Timer0_Delay(16000000, 1000, 1000);  // 1s delay
    }
#else
    while (1)
    {
        printf("Listening...\n");
        len = LoRa_Receive(buffer, 64);
        buffer[len] = '\0';
        printf("Received: %s\n", buffer);
    }
#endif
}
