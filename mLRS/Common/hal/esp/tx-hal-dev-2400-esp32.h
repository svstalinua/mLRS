//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
// OlliW @ www.olliw.eu
//*******************************************************
// hal
//*******************************************************

#define DEVICE_HAS_SINGLE_LED

//#define DEVICE_HAS_NO_SERIAL  
//#define DEVICE_HAS_NO_COM
#define DEVICE_HAS_NO_DEBUG

//-- UARTS
// UARTB = serial port or COM (CLI)
// UARTC = --
// UART = --
// UARTE = --
// UARTF = debug port

#define UARTB_USE_SERIAL
#define UARTB_BAUD                TX_SERIAL_BAUDRATE
#define UARTB_TXBUFSIZE           TX_SERIAL_TXBUFSIZE
#define UARTB_RXBUFSIZE           TX_SERIAL_RXBUFSIZE

#define UARTC_USE_SERIAL1
#define UARTC_BAUD                TX_SERIAL_BAUDRATE
#define UARTC_TXBUFSIZE           TX_SERIAL_TXBUFSIZE
#define UARTC_RXBUFSIZE           TX_SERIAL_RXBUFSIZE

#define UARTF_USE_SERIAL1
#define UARTF_BAUD                115200


//-- SX1: SX12xx & SPI
#define SPI_CS_IO                 IO_P27
#define SPI_MISO                  IO_P33
#define SPI_MOSI                  IO_P32
#define SPI_SCK                   IO_P25
#define SPI_FREQUENCY             10000000L
#define SX_RESET                  IO_P26
#define SX_BUSY                   IO_P15
#define SX_DIO1                   IO_P35
#define SX_TX_EN                  IO_P18
#define SX_RX_EN                  IO_P19

IRQHANDLER(void SX_DIO_EXTI_IRQHandler(void);)

void sx_init_gpio(void)
{
    gpio_init(SX_DIO1, IO_MODE_INPUT_ANALOG);
    gpio_init(SX_BUSY, IO_MODE_INPUT_PU);
    gpio_init(SX_TX_EN, IO_MODE_OUTPUT_PP_LOW);
    gpio_init(SX_RX_EN, IO_MODE_OUTPUT_PP_LOW);
    gpio_init(SX_RESET, IO_MODE_OUTPUT_PP_HIGH);
}

IRAM_ATTR static inline bool sx_busy_read(void)
{
    return (gpio_read_activehigh(SX_BUSY)) ? true : false;
}

IRAM_ATTR static inline void sx_amp_transmit(void)
{
    gpio_low(SX_RX_EN);
    gpio_high(SX_TX_EN);
}

IRAM_ATTR static inline void sx_amp_receive(void)
{
    gpio_low(SX_TX_EN);
    gpio_high(SX_RX_EN);
}

void sx_dio_enable_exti_isr(void)
{
    attachInterrupt(SX_DIO1, SX_DIO_EXTI_IRQHandler, RISING);
}

void sx_dio_init_exti_isroff(void) {}
void sx_dio_exti_isr_clearflag(void) {}


//-- Button
#define BUTTON                    IO_P0

void button_init(void)
{
    gpio_init(BUTTON, IO_MODE_INPUT_PU);
}

IRAM_ATTR bool button_pressed(void)
{
    return gpio_read_activelow(BUTTON) ? true : false;
}


//-- LEDs
#define LED_RED                   5

void leds_init(void)
{
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);// LED_RED_OFF
}

void led_green_off(void) {  }
void led_green_on(void) {  }
void led_green_toggle(void) {  }

void led_red_off(void) { gpio_high(LED_RED); }
void led_red_on(void) { gpio_low(LED_RED); }
void led_red_toggle(void) { gpio_toggle(LED_RED); }


//-- POWER
#define POWER_GAIN_DBM            28 // gain of a PA stage if present
#define POWER_SX1280_MAX_DBM      SX1280_POWER_3_DBM  // maximum allowed sx power
#define POWER_USE_DEFAULT_RFPOWER_CALC

#define RFPOWER_DEFAULT           1 // index into rfpower_list array

const rfpower_t rfpower_list[] = {
    { .dbm = POWER_MIN, .mW = INT8_MIN },
    { .dbm = POWER_10_DBM, .mW = 10 },
    { .dbm = POWER_17_DBM, .mW = 50 },
    { .dbm = POWER_20_DBM, .mW = 100 },
};

