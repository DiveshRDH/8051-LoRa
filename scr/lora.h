#ifndef __LORA_H__
#define __LORA_H__

bit LoRa_Check(void);
void LoRa_Reset(void);
void LoRa_Init(void);
void LoRa_Send(char* msg, unsigned char len);
unsigned char LoRa_Receive(char* buf, unsigned char maxLen);
int LoRa_ReadRSSI(void);

#endif
