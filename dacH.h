#ifndef dacH_h
#define dacH_h
/*
 * DAC80508MC driver for PulsePoint board
 *   PB7 = CLR pin (active low) — must stay HIGH for outputs to work
 *   LDAC tied to GND on board — outputs update on CS rising edge
 *   Internal 2.5V reference
 *   VIO = 3.3V (SPI logic)
 *   VDD = 5V  (analog output stage)
 *
 * SPI connections (onboard, from schematic):
 *   PB6 = CS   (hardware SPI peripheral)
 *   PB9 = SCLK
 *   PB8 = SDI  (MOSI) dac receives 
 *   PB7 = CLR  (GPIO output, driven HIGH)
 *   PB14= MISO (reassigned away from PB7, unused)
 */

#include "ti_msp_dl_config.h"
#include <stdint.h>

/* ============================================================
   DAC80508 Register Map
   ============================================================ */
#define DAC80508_REG_NOP      0x00
#define DAC80508_REG_DEVID    0x01
#define DAC80508_REG_SYNC     0x02
#define DAC80508_REG_CONFIG   0x03
#define DAC80508_REG_GAIN     0x04
#define DAC80508_REG_TRIGGER  0x05
#define DAC80508_REG_BRDCAST  0x06
#define DAC80508_REG_STATUS   0x07

//output regs
#define DAC80508_REG_DAC0     0x08
#define DAC80508_REG_DAC1     0x09
#define DAC80508_REG_DAC2     0x0A
#define DAC80508_REG_DAC3     0x0B
#define DAC80508_REG_DAC4     0x0C
#define DAC80508_REG_DAC5     0x0D
#define DAC80508_REG_DAC6     0x0E
#define DAC80508_REG_DAC7     0x0F

#define dacSOFTRESET_VAL      (0x000Au)

/* CS PB6 */
#define DAC_CS_LOW()   
#define DAC_CS_HIGH() 
/* CLR PB7*/
#define DAC_CLR_PORT    GPIOB
#define DAC_CLR_PIN     DL_GPIO_PIN_7
#define DAC_CLR_HIGH()  DL_GPIO_setPins(DAC_CLR_PORT,   DAC_CLR_PIN)
#define DAC_CLR_LOW()   DL_GPIO_clearPins(DAC_CLR_PORT, DAC_CLR_PIN)
/*BLUE LED pb22 220ohm  */
//define ledon

/*SPI FRAME - 24 BITS, 3 BYTES * 8 CH*/ 
 /* Byte 0: register address [4:0] Byte 1: data [15:8] Byte 2: data [7:0]*/
 
//1
static inline void DAC80508_WRITE(uint8_t addr, uint16_t data)
{
    while (DL_SPI_isBusy(mySPI_INST)) {}
    DL_SPI_transmitData8(mySPI_INST, addr & 0x1Fu); // please explain this. this value is 32? 2^5
    DL_SPI_transmitData8(mySPI_INST, (data >> 8) & 0xFFu);
    DL_SPI_transmitData8(mySPI_INST,  data       & 0xFFu);
    while (DL_SPI_isBusy(mySPI_INST)) {}
}

/*2 WRITE A VALUE TO A DAC REGISTER */
static inline void DAC80508_write_reg(uint8_t reg, uint16_t val)
{
    DAC80508_WRITE(reg, val);
}


   /*SOFT RESET USING TRIGGER REG - POWER ON DEFAULTS*/
static inline void DAC80508_soft_reset(void)
{
    DAC80508_WRITE(DAC80508_REG_TRIGGER, dacSOFTRESET_VAL);
}

/* ============================================================
   Set one DAC channel output
   ch:   0-7  maps to DAC0-DAC7 registers 0x08-0x0F
   code: 0x0000 = 0V
         0x8000 = 1.25V midscale (zero current through coil)
         0xFFFF = 2.5V full scale
   Output updates immediately (LDAC tied to GND on board)
   ============================================================ */

/* SET ANY DAC CHANNEL TO freq step*/
/* async update, no trigger needed */
//0x0000 = 0V, 0x8000 = midcode, 0xFFFF = fullscale 5v
static inline void DAC80508_set_channel(uint8_t ch, uint16_t code)
{
    DAC80508_WRITE(
        (uint8_t)(DAC80508_REG_DAC0 + (ch & 0x07u)),
        code
    );
}

#endif /* dacHeader_h */