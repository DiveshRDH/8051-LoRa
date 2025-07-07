# 📡 LoRa SX1278 Driver for Nuvoton MS51 (8051 MCU)

A lightweight C library for interfacing **SX1278 LoRa modules** (e.g. Ai-Thinker LoRa-02) with **Nuvoton MS51 microcontrollers**. This library provides low-level SPI control, initialization, transmission, and reception functions for LoRa communication in the 433 MHz band.

---

## 📋 Table of Contents

- [Features](#features)
- [Supported Hardware](#supported-hardware)
- [Pin Connections](#pin-connections)
- [Getting Started](#getting-started)
- [Usage](#usage)
  - [Initialization](#initialization)
  - [Check LoRa Chip](#check-lora-chip)
  - [Send LoRa Packet](#send-lora-packet)
  - [Receive LoRa Packet](#receive-lora-packet)
- [Functions Reference](#functions-reference)
- [Dependencies](#dependencies)
- [Configuration](#configuration)
- [License](#license)
- [Author](#author)

---

## 🚀 Features

- Minimal and efficient C driver
- Compatible with 433 MHz LoRa modules (SX1278)
- Works with Nuvoton MS51 (8051)
- Basic LoRa modes: Sleep, Standby, TX, RX
- No external libraries required
- Easily extensible for other MCU families

---

## 🔌 Supported Hardware

- ✅ **Microcontroller**: Nuvoton MS51 Series (8051 Core)
- ✅ **LoRa Module**: SX1278-based (e.g. AiThinker LoRa-02)
- ✅ **Interface**: SPI + GPIO (DIO0, RESET)

---

## 🧩 Pin Connections

| SX1278 Pin | Description     | MS51 Pin |
|------------|-----------------|----------|
| `NSS`      | SPI Chip Select | `P15`    |
| `DIO0`     | IRQ (TxDone/RxDone) | `P12` |
| `RESET`    | Hardware Reset  | `P13`    |
| `MOSI`     | SPI MOSI        | SPI Line |
| `MISO`     | SPI MISO        | SPI Line |
| `SCK`      | SPI Clock       | SPI Line |
| `VCC` / `GND` | Power (3.3V) | MCU Supply |

> ⚠️ **Important**: Add a 100 µF capacitor close to the LoRa module’s VCC-GND pins to ensure stable power during transmission.

---

## 🛠️ Getting Started

1. Connect your LoRa module to the MS51 as described.
2. Make sure your SPI and delay functions (`Spi_Write_Byte`, `Spi_Read_Byte`, `Timer0_Delay`) are working.
3. Include the library in your project and call `LoRa_Init()`.

---

## 📦 Usage

### 🔧 Initialization
Initializes the SPI interface and configures the LoRa module to 433 MHz, max power, and standby mode.

```c
LoRa_Init();
```
### Check LoRa Chip
This function reads the version register (0x42). A correct SX1278 will return 0x12. This helps confirm that communication with the module is functional.
```c
if (LoRa_Check()) {
    // SX1278 is detected and working
} else {
    // LoRa module not found or not responding
}
```
### Send LoRa Packet
```c
char msg[] = "Hello, LoRa!";
LoRa_Send(msg, strlen(msg));

```
- This function:
 
  - Writes the message to FIFO (0x00)
  - Sets LoRa to transmit mode (0x83)
  - Waits for DIO0 to go HIGH (TxDone)
  - Clears interrupt flags
  - Use this to send any binary or ASCII data over LoRa.

### Receive LoRa Packet
Receives a LoRa packet and reads it into a buffer.
```c
char buffer[64];
unsigned char len = LoRa_Receive(buffer, sizeof(buffer));
buffer[len] = '\0';
printf("Received: %s\n", buffer);

```
### 🧠 Built with ❤️ by **RDH Robotics**

Want more help? Reach out or fork and improve this repo!
