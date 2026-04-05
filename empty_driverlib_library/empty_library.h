#ifndef EMPTY_LIBRARY_H
#define EMPTY_LIBRARY_H

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>


/* ===============================================
   DAC80508 REGISTER MAP
   =============================================== */
#define DAC80508_REG_NOP      0x00
#define DAC80508_REG_DEVID    0x01
#define DAC80508_REG_SYNC     0x02
#define DAC80508_REG_CONFIG   0x03
#define DAC80508_REG_GAIN     0x04
#define DAC80508_REG_TRIGGER  0x05
#define DAC80508_REG_BRDCAST  0x06
#define DAC80508_REG_STATUS   0x07
#define DAC80508_REG_DAC0     0x08
#define DAC80508_REG_DAC1     0x09
#define DAC80508_REG_DAC2     0x0A
#define DAC80508_REG_DAC3     0x0B
#define DAC80508_REG_DAC4     0x0C
#define DAC80508_REG_DAC5     0x0D
#define DAC80508_REG_DAC6     0x0E
#define DAC80508_REG_DAC7     0x0F


/* ===============================================
   CONTROL VALUES
   =============================================== */
#define dacTRIGGERLDAC    (1u << 4)
#define dacSOFTRESET_VAL  (0x000Au)


/* ===============================================
   CHIP SELECT
   Matches PA28 assigned in SysConfig (GPIO1 / gpioCS / cspin).
   =============================================== */
#define DAC_CS_PORT  GPIOA
#define DAC_CS_PIN   DL_GPIO_PIN_28
#define DAC_CS_LOW()   DL_GPIO_clearPins(DAC_CS_PORT, DAC_CS_PIN)
#define DAC_CS_HIGH()  DL_GPIO_setPins(DAC_CS_PORT, DAC_CS_PIN)


/* ===============================================
   LED
   FIXED: was DL_GPIO_PIN_5. SysConfig assigns
   GPIO4 / led1 to PA3, so PIN_3 is correct.
   =============================================== */
#define LED_PORT  GPIOA
#define LED_PIN   DL_GPIO_PIN_3             // FIXED: was DL_GPIO_PIN_5
#define LED_ON()   DL_GPIO_setPins(LED_PORT,   LED_PIN)
#define LED_OFF()  DL_GPIO_clearPins(LED_PORT, LED_PIN)


/* ===============================================
   SPI HELPERS
   Wait until the SPI peripheral is no longer busy.
   Call BEFORE raising CS so the last byte has
   fully shifted out.
   =============================================== */
static inline void DAC80508_spi_wait_idle(void)
{
    while (DL_SPI_isBusy(mySPI_INST)) {}
}


/* ===============================================
   BASIC WRITE  (24-bit frame: 8-bit addr + 16-bit data)

   Frame format (DAC80508 SPI, MODE 1 or MODE 2):
     Byte 0 : [7]   RW  = 0 (write)
              [6:4] Reserved = 0
              [3:0] Register address
     Byte 1 : data[15:8]
     Byte 2 : data[7:0]

   CS sequence:
     CS low → transmit 3 bytes → wait idle → CS high
   =============================================== */
static inline void DAC80508_WRITE(uint8_t addr, uint16_t data)
{
    DAC_CS_LOW();
    DL_SPI_transmitData8(mySPI_INST, addr & 0x1F);      // mask to 5-bit address field
    DL_SPI_transmitData8(mySPI_INST, (data >> 8) & 0xFF);
    DL_SPI_transmitData8(mySPI_INST,  data       & 0xFF);
    DAC80508_spi_wait_idle();                            // wait BEFORE releasing CS
    DAC_CS_HIGH();
}


/* ===============================================
   HIGH LEVEL CHANNEL WRITE
   ch  : 0–7 (maps to DAC0–DAC7 registers 0x08–0x0F)
   code: 16-bit output code (0x0000–0xFFFF)
   =============================================== */
static inline void dacSetChannel(uint8_t ch, uint16_t code)
{
    DAC80508_WRITE((uint8_t)(DAC80508_REG_DAC0 + (ch & 0x07)), code);
}


#endif /* EMPTY_LIBRARY_H */