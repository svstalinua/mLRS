//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// ESP UartB
//********************************************************

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


#ifndef ESPLIB_UARTB_H
#define ESPLIB_UARTB_H


#ifdef UARTB_USE_SERIAL
#define UARTB_SERIAL_NO Serial
#elif defined UARTB_USE_SERIAL1
#define UARTB_SERIAL_NO Serial1
#endif


IRAM_ATTR static inline uint16_t uartb_putc(char c)
{
    UARTB_SERIAL_NO.write(c);
    return 1;
}


IRAM_ATTR static inline char uartb_getc(void)
{
    return (char)UARTB_SERIAL_NO.read();
}


IRAM_ATTR static inline void uartb_rx_flush(void)
{
    while (UARTB_SERIAL_NO.available() > 0) UARTB_SERIAL_NO.read();
}


IRAM_ATTR static inline void uartb_tx_flush(void)
{
    UARTB_SERIAL_NO.flush();
}


IRAM_ATTR static inline uint16_t uartb_rx_bytesavailable(void)
{
    return (UARTB_SERIAL_NO.available() > 0) ? UARTB_SERIAL_NO.available() : 0;
}


IRAM_ATTR static inline uint16_t uartb_rx_available(void)
{
    return (UARTB_SERIAL_NO.available() > 0) ? 1 : 0;
}


//-------------------------------------------------------
// INIT routines
//-------------------------------------------------------

void uartb_setprotocol(uint32_t baud, UARTPARITYENUM parity, UARTSTOPBITENUM stopbits)
{
    UARTB_SERIAL_NO.end();
    UARTB_SERIAL_NO.setRxBufferSize(UARTB_RXBUFSIZE);
#ifdef ESP32
    UARTB_SERIAL_NO.begin(baud, SERIAL_8N1, UARTB_RX, UARTB_TX);  // to do - fix this
    UARTB_SERIAL_NO.setRxFIFOFull(8);  // > 57600 baud sets to 120 which is too much, buffer only 127 bytes
    UARTB_SERIAL_NO.setRxTimeout(1);   // wait for 1 symbol (~11 bits) to trigger Rx ISR, default 2
#endif
}


void uartb_init(void)
{
    UARTB_SERIAL_NO.end();
    UARTB_SERIAL_NO.setRxBufferSize(UARTB_RXBUFSIZE);
#ifdef ESP32
    UARTB_SERIAL_NO.begin(UARTB_BAUD, SERIAL_8N1, UARTB_RX, UARTB_TX);  // to do - fix this
    UARTB_SERIAL_NO.setRxFIFOFull(8);
    UARTB_SERIAL_NO.setRxTimeout(1);
#endif
}


#endif // ESPLIB_UARTB_H
