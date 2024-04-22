//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// ESP UartC
//********************************************************
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

void uartc_init(void)
{
    UARTC_SERIAL_NO.begin(UARTC_BAUD);
}


#endif // ESPLIB_UARTC_H
