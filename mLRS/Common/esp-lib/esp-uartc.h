//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// ESP UartC
//********************************************************
#ifdef DEVICE_HAS_NO_SERIAL // ugly, fix

// these come from the STM files, not sure if they are needed
#define LL_USART_PARITY_NONE 1
#define LL_USART_PARITY_EVEN 1
#define LL_USART_PARITY_ODD 1
#define LL_USART_STOPBITS_0_5 1
#define LL_USART_STOPBITS_1 1
#define LL_USART_STOPBITS_2 1

typedef enum {
    XUART_PARITY_NO = LL_USART_PARITY_NONE, // XUART_xxx to avoid overlap with HAL
    XUART_PARITY_EVEN = LL_USART_PARITY_EVEN,
    XUART_PARITY_ODD = LL_USART_PARITY_ODD,
    XUART_PARITY_MAKEITU32 = UINT32_MAX,
} UARTPARITYENUM;

typedef enum {
    UART_STOPBIT_0_5 = LL_USART_STOPBITS_0_5,
    UART_STOPBIT_1 = LL_USART_STOPBITS_1,
    UART_STOPBIT_2 = LL_USART_STOPBITS_2,
    UART_STOPBIT_MAKEITU32 = UINT32_MAX,
} UARTSTOPBITENUM;

#endif

#ifndef ESPLIB_UARTC_H
#define ESPLIB_UARTC_H


#if defined(UARTC_USE_SERIAL)
#define UARTC_SERIAL_NO Serial
#elif defined(UARTC_USE_SERIAL1)
#define UARTC_SERIAL_NO Serial1
#endif


IRAM_ATTR static inline uint16_t uartc_putc(char c)
{
    UARTC_SERIAL_NO.write(c);
    return 1;
}

IRAM_ATTR static inline char uartc_getc(void)
{
    return (char)UARTC_SERIAL_NO.read();
}

IRAM_ATTR static inline uint16_t uartc_rx_available(void)
{
    return (UARTC_SERIAL_NO.available() > 0) ? 1 : 0;
}

//-------------------------------------------------------
// INIT routines
//-------------------------------------------------------

void uartc_setprotocol(uint32_t baud, UARTPARITYENUM parity, UARTSTOPBITENUM stopbits)
{
    UARTC_SERIAL_NO.end();
    UARTC_SERIAL_NO.setRxBufferSize(UARTC_RXBUFSIZE);
#ifdef ESP32
    UARTC_SERIAL_NO.begin(baud, SERIAL_8N1, UARTC_RX, UARTC_TX);  // to do - fix this
    UARTC_SERIAL_NO.setRxFIFOFull(8);  // > 57600 baud sets to 120 which is too much, buffer only 127 bytes
    UARTC_SERIAL_NO.setRxTimeout(1);   // wait for 1 symbol (~11 bits) to trigger Rx ISR, default 2
#endif
}


void uartc_init(void)
{
    UARTC_SERIAL_NO.setRxBufferSize(UARTC_RXBUFSIZE);
#ifdef ESP32
    UARTC_SERIAL_NO.begin(UARTC_BAUD, SERIAL_8N1, UARTC_RX, UARTC_TX);  // to do - fix this
    UARTC_SERIAL_NO.setRxFIFOFull(8);
    UARTC_SERIAL_NO.setRxTimeout(1);
#endif
}


#endif // ESPLIB_UARTC_H
