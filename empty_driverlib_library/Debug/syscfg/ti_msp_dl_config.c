/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gmyTIMERBackup;
DL_SPI_backupConfig gmySPIBackup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_myTIMER_init();
    SYSCFG_DL_myUART_init();
    SYSCFG_DL_mySPI_init();
    /* Ensure backup structures have no valid state */
	gmyTIMERBackup.backupRdy 	= false;

	gmySPIBackup.backupRdy 	= false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(myTIMER_INST, &gmyTIMERBackup);
	retStatus &= DL_SPI_saveConfiguration(mySPI_INST, &gmySPIBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(myTIMER_INST, &gmyTIMERBackup, false);
	retStatus &= DL_SPI_restoreConfiguration(mySPI_INST, &gmySPIBackup);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(myTIMER_INST);
    DL_UART_Main_reset(myUART_INST);
    DL_SPI_reset(mySPI_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(myTIMER_INST);
    DL_UART_Main_enablePower(myUART_INST);
    DL_SPI_enablePower(mySPI_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    
	DL_GPIO_initPeripheralOutputFunction(
		 GPIO_myUART_IOMUX_TX, GPIO_myUART_IOMUX_TX_FUNC);
	DL_GPIO_initPeripheralInputFunctionFeatures(
		 GPIO_myUART_IOMUX_RX, GPIO_myUART_IOMUX_RX_FUNC,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_mySPI_IOMUX_SCLK, GPIO_mySPI_IOMUX_SCLK_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_mySPI_IOMUX_PICO, GPIO_mySPI_IOMUX_PICO_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_mySPI_IOMUX_POCI, GPIO_mySPI_IOMUX_POCI_FUNC);
    
	DL_GPIO_initPeripheralOutputFunction(
		 GPIO_mySPI_IOMUX_CS0, GPIO_mySPI_IOMUX_CS0_FUNC);

    DL_GPIO_initDigitalOutput(gpioCS_cspin_IOMUX);

    DL_GPIO_initDigitalOutput(gpioThermistor_thermistor_IOMUX);

    DL_GPIO_initDigitalOutput(gpioRef_reference_IOMUX);

    DL_GPIO_initDigitalOutput(gpioLED_led1_IOMUX);

    DL_GPIO_clearPins(GPIOA, gpioCS_cspin_PIN |
		gpioThermistor_thermistor_PIN |
		gpioRef_reference_PIN |
		gpioLED_led1_PIN);
    DL_GPIO_enableOutput(GPIOA, gpioCS_cspin_PIN |
		gpioThermistor_thermistor_PIN |
		gpioRef_reference_PIN |
		gpioLED_led1_PIN);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_enableExternalClock(DL_SYSCTL_CLK_OUT_SOURCE_SYSOSC,
		DL_SYSCTL_CLK_OUT_DIVIDE_DISABLE);

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gmyTIMERClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * myTIMER_INST_LOAD_VALUE = (0 ms * 32000000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gmyTIMERTimerConfig = {
    .period     = myTIMER_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC_UP,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_myTIMER_init(void) {

    DL_TimerA_setClockConfig(myTIMER_INST,
        (DL_TimerA_ClockConfig *) &gmyTIMERClockConfig);

    DL_TimerA_initTimerMode(myTIMER_INST,
        (DL_TimerA_TimerConfig *) &gmyTIMERTimerConfig);
    DL_TimerA_enableClock(myTIMER_INST);





}


static const DL_UART_Main_ClockConfig gmyUARTClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gmyUARTConfig = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_myUART_init(void)
{
    DL_UART_Main_setClockConfig(myUART_INST, (DL_UART_Main_ClockConfig *) &gmyUARTClockConfig);

    DL_UART_Main_init(myUART_INST, (DL_UART_Main_Config *) &gmyUARTConfig);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 115200
     *  Actual baud rate: 115211.52
     */
    DL_UART_Main_setOversampling(myUART_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(myUART_INST, myUART_IBRD_32_MHZ_115200_BAUD, myUART_FBRD_32_MHZ_115200_BAUD);


    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(myUART_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);


    DL_UART_Main_enable(myUART_INST);
}

static const DL_SPI_Config gmySPI_config = {
    .mode        = DL_SPI_MODE_CONTROLLER,
    .frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0,
    .parity      = DL_SPI_PARITY_NONE,
    .dataSize    = DL_SPI_DATA_SIZE_8,
    .bitOrder    = DL_SPI_BIT_ORDER_MSB_FIRST,
    .chipSelectPin = DL_SPI_CHIP_SELECT_0,
};

static const DL_SPI_ClockConfig gmySPI_clockConfig = {
    .clockSel    = DL_SPI_CLOCK_BUSCLK,
    .divideRatio = DL_SPI_CLOCK_DIVIDE_RATIO_1
};

SYSCONFIG_WEAK void SYSCFG_DL_mySPI_init(void) {
    DL_SPI_setClockConfig(mySPI_INST, (DL_SPI_ClockConfig *) &gmySPI_clockConfig);

    DL_SPI_init(mySPI_INST, (DL_SPI_Config *) &gmySPI_config);

    /* Configure Controller mode */
    /*
     * Set the bit rate clock divider to generate the serial output clock
     *     outputBitRate = (spiInputClock) / ((1 + SCR) * 2)
     *     2000000 = (32000000)/((1 + 7) * 2)
     */
    DL_SPI_setBitRateSerialClockDivider(mySPI_INST, 7);
    /* Set RX and TX FIFO threshold levels */
    DL_SPI_setFIFOThreshold(mySPI_INST, DL_SPI_RX_FIFO_LEVEL_1_2_FULL, DL_SPI_TX_FIFO_LEVEL_1_2_EMPTY);

    /* Enable module */
    DL_SPI_enable(mySPI_INST);
}

