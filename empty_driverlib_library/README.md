## Example Summary

This example shows a basic empty library project using DriverLib.
using:
- MSPM0G3507 - LP
- DAC80508 EVM


## Peripherals & Pin Assignments

N/A

## BoosterPacks, Board Resources & Jumper Settings

N/A


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile example to build library.
/*

#include "ti_msp_dl_config.h"
#include "empty_library.h"

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h> 

// /* ================= SPI TRANSFER ================= */
uint8_t spi_txrx(uint8_t data)
{
    while (!DL_SPI_isTXFIFOEmpty(SPI_0_INST));
    DL_SPI_transmitData8(SPI_0_INST, data);

    while (DL_SPI_isRXFIFOEmpty(SPI_0_INST));
    return DL_SPI_receiveData8(SPI_0_INST);
}

/* ================= DAC WRITE ================= */
void DAC80508_WRITE(uint8_t addr, uint16_t data)
{
    DAC_CS_LOW();

    spi_txrx(addr & 0x3F);
    spi_txrx((data >> 8) & 0xFF);
    spi_txrx(data & 0xFF);

    DAC_CS_HIGH();
}

/* ================= DAC READ ================= */
uint16_t DAC80508_Read(uint8_t addr)
{
    uint8_t msb, lsb;

    DAC_CS_LOW();

    spi_txrx(0x80 | (addr & 0x3F));
    msb = spi_txrx(0x00);
    lsb = spi_txrx(0x00);

    DAC_CS_HIGH();

    return ((uint16_t)msb << 8) | lsb;
}

/* ================= MAIN ================= */
int main(void)
{
    SYSCFG_DL_init();

    uint16_t status;
    uint16_t dac0_read;

    /* Read status */
    status = DAC80508_Read(DAC80508_REG_STATUS);

    /* Configure DAC */
    DAC80508_WRITE(DAC80508_REG_CONFIG, 0x0000);
    DL_Common_delayCycles(32000);

    DAC80508_WRITE(DAC80508_REG_GAIN, 0x0000);
    DL_Common_delayCycles(32000);

    DAC80508_WRITE(DAC80508_REG_SYNC, 0x0000);
    DL_Common_delayCycles(1000);

    /* Set DAC0 full scale */
    DAC80508_WRITE(DAC80508_REG_DAC0, 0xFFFF);
    DL_Common_delayCycles(1000);

    dac0_read = DAC80508_Read(DAC80508_REG_DAC0);

    while (1)
    {
        __WFI();
    }
}


next steps

- thermistor
- 8 diff frequencies
- reference signal 
- how to talk with rx team micro - UART in case they need to send commands to us.
ex have a code that can tell us to start 
- adc channel 

*/