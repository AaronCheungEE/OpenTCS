#ifndef _THREADS_H_
#define _THREADS_H_

#include "hal.h"
#include "nil.h"
#include "stm32f0xx.h"
#include "messages.pb.h"

#define DBG_USART USART1

/*
 * This file contains all the function prototypes
 * and global structures used in threads.
 */

/**
 * @name  IRQ VECTOR names
 * @{
 */
#define WWDG_IRQHandler                 Vector40    /**< Window Watchdog.           */
#define PVD_IRQHandler                  Vector44    /**< PVD through EXTI Line
                                                    detect.                    */
#define RTC_IRQHandler                  Vector48    /**< RTC through EXTI Line
                                                    Interrupt.                 */
#define FLASH_IRQHandler                Vector4C    /**< Flash.                     */
#define RCC_IRQHandler                  Vector50    /**< RCC.                       */
#define EXTI0_1_IRQHandler              Vector54    /**< EXTI Line 0..1.            */
#define EXTI2_3_IRQHandler              Vector58    /**< EXTI Line 2..3.            */
#define EXTI4_15_IRQHandler             Vector5C    /**< EXTI Line 4..15.           */
#define TS_IRQHandler                   Vector60    /**< TS.                        */
#define DMA1_Ch1_IRQHandler             Vector64    /**< DMA1 Channel 1.            */
#define DMA1_Ch2_3_IRQHandler           Vector68    /**< DMA1 Channels 2 and 3.     */
#define DMA1_Ch4_5_IRQHandler           Vector6C    /**< DMA1 Channels 4 and 5.     */
#define ADC1_COMP_IRQHandler            Vector70    /**< ADC1 comparators 1 and 2.  */
#define TIM1_BRK_UP_TRG_COM_IRQHandler  Vector74    /**< TIM1 common.           */
#define TIM1_CC_IRQHandler              Vector78    /**< TIM1 Capture Compare.      */
#define TIM2_IRQHandler                 Vector7C    /**< TIM2.                      */
#define TIM3_IRQHandler                 Vector80    /**< TIM3.                      */
#define TIM6_DAC_IRQHandler             Vector84    /**< TIM6 and DAC.              */
#define TIM14_IRQHandler                Vector8C    /**< TIM14.                     */
#define TIM15_IRQHandler                Vector90    /**< TIM15.                     */
#define TIM16_IRQHandler                Vector94    /**< TIM16.                     */
#define TIM17_IRQHandler                Vector98    /**< TIM17.                     */
#define I2C1_IRQHandler                 Vector9C    /**< I2C1.                      */
#define I2C2_IRQHandler                 VectorA0    /**< I2C2.                      */
#define SPI1_IRQHandler                 VectorA4    /**< SPI1.                      */
#define SPI2_IRQHandler                 VectorA8    /**< SPI2.                      */
#define USART1_IRQHandler               VectorAC    /**< USART1.                    */
#define USART2_IRQHandler               VectorB0    /**< USART2.                    */
#define CEC_IRQHandler                  VectorB8    /**< CEC.                       */


/*
 * Misc functions used by threads
 */

void itoa(int n, char s[]);

extern semaphore_t usart1_sem;
extern semaphore_t spi1_sem;
extern semaphore_t i2c1_sem;
extern char serial_dbg;

/* Communications */

#define USART_TXBUF_SIZE 64
#define USART_RXBUF_SIZE 64

extern char usart_txbuf[USART_TXBUF_SIZE];
extern char usart_rxbuf[USART_RXBUF_SIZE];

void spiInit(SPI_TypeDef* SPIx);
uint8_t spiSendS(SPI_TypeDef* SPIx, uint8_t* buffer, uint16_t len);
uint8_t spiSendI(SPI_TypeDef* SPIx, uint8_t* buffer, uint16_t len);

void i2cInit(I2C_TypeDef* I2Cx);
uint8_t i2cSendS(I2C_TypeDef* I2Cx, const uint8_t addr, uint8_t* buffer, uint8_t len);
uint8_t i2cReceiveS(I2C_TypeDef* I2Cx, const uint8_t addr, uint8_t* buffer, uint8_t len);

void usartInit(USART_TypeDef* USARTx);
uint8_t usartSendI(USART_TypeDef* USARTx, const char *buffer, uint16_t len);
uint8_t usartSendS(USART_TypeDef* USARTx, const char *buffer, uint16_t len);
inline void usartPrintString(USART_TypeDef* USARTx, const char *str);
inline void serDbg(const char *str);

/* End of Communications */

/* Last flash page is used to store settings */
#define SETTINGS_FUNCTION_TC 0x1
#define SETTINGS_FUNCTION_SHIFTER 0x2
#define SETTINGS_FUNCTION_LED 0x4

#define SETTINGS_CUT_DISABLED 0x0
#define SETTINGS_CUT_NORMAL 0x1
#define SETTINGS_CUT_PROGRESSIVE 0x2

#define SETTINGS_SENSOR_NORMAL 0
#define SETTINGS_SENSOR_REVERSE 1

extern settings_t settings;

void settingsInit(void);
settings_t readSettings(void);
uint8_t writeSettings(settings_t* st);

/* End of misc functions */


/* CONTROL */

extern status_t status;
void startControl(void) __attribute__ ((noreturn));

/* End of CONTROL */

/* Light */
#define LIGHT_STATE_OFF 0
#define LIGHT_STATE_STILL 1
#define LIGHT_STATE_BLINK 2
#define LIGHT_STATE_PULSE 3

extern light_settings_t light_settings;
void startLight(void) __attribute__ ((noreturn));

/* End of Light */


/* ADC */
#define ADC_STOPPED 0
#define ADC_STARTED 1

#define ADC_CHANNELS 4

extern uint16_t adc_samples[32];

void startAdc(void);

/* End of ADC */


/* Ignition */
void startIgnition(void) __attribute__ ((noreturn));

/* End of Ignition */


/* Display */
#define DISPLAY_OFF 0
#define DISPLAY_ON 1

struct __display {
    uint8_t state;
};
typedef struct __display display_t;
extern display_t display;

void startDisplay(void) __attribute__ ((noreturn));

/* End of Display */

/* Sensors */
#define SENSORS_OFF 0
#define SENSORS_ON 1

extern sensors_t sensors;

void startSensors(void) __attribute__ ((noreturn));
uint8_t getCurCutTime(void);

/* End of Sensors */

/* Serial protocol */

void startSerialCom(void) __attribute__ ((noreturn));

/* End of Serial protocol */

#endif
