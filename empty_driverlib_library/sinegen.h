/* UART SETUP 
1. include libraries
2. define variables
3. write functions
4. call functions in main
*/

// 1. include libraries
#include "ti_msp_dl_config.h"
#include <math.h>
#include <stdint.h>

// 2. define variables
volatile uint8_t enableOUT = 0;
volatile uint8_t runSYS = 0;


// =============================
// a. SINE LUT
// =============================
uint16_t sineTable[TABLE_SIZE];

void genSineTable(void)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        float angle = (2.0f * 3.14159265f * i) / TABLE_SIZE;
        float s = (sinf(angle) + 1.0f) * 0.5f;
        sineTable[i] = (uint16_t)(s * 65535);
    }
}

// =============================
// b. DDS definition
// =============================
typedef struct
{
    uint32_t phase;
    uint32_t step;
} DDS_t;

DDS_t dds[NUM_CHANNELS];

// =============================
// c. Define frequencies
// =============================
float freqs[NUM_CHANNELS] = {
    20500, 21500, 22500, 23500,
    24500, 25500, 26500, 27500
};

// =============================
// d. Initialize DDS
// =============================
void init_DDS(void)
{
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        dds[i].phase = 0;

        dds[i].step = (uint32_t)(
            (freqs[i] * (float)(1ULL << 32)) / SAMPLE_RATE
        );
    }
}

// =============================
// e. Write to DAC via SPI
// =============================
void DAC_write_channel(uint8_t ch, uint16_t val)
{
    uint8_t addr = 0x08 + ch;

    DAC_CS_LOW();

    while (!DL_SPI_isTXFIFOEmpty(SPI_0_INST));
    DL_SPI_transmitData8(SPI_0_INST, addr);

    while (!DL_SPI_isTXFIFOEmpty(SPI_0_INST));
    DL_SPI_transmitData8(SPI_0_INST, val >> 8);

    while (!DL_SPI_isTXFIFOEmpty(SPI_0_INST));
    DL_SPI_transmitData8(SPI_0_INST, val & 0xFF);

    while (DL_SPI_isBusy(SPI_0_INST));

    DAC_CS_HIGH();
}

// =============================
// f. Timer ISR
// =============================
void TIMER_0_INST_IRQHandler(void)
{
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        dds[i].phase += dds[i].step;

        uint16_t index = dds[i].phase >> 24;
        uint16_t value = sineTable[index];

        DAC_write_channel(i, value);
    }

    DL_Timer_clearInterruptStatus(TIMER_0_INST);
}

// =============================
// g. main
// =============================
int main(void)
{
    SYSCFG_DL_init();

    genSineTable();
    init_DDS();

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

    DL_Timer_startCounter(TIMER_0_INST);

    while (1)
    {
        __WFI();
    }
}
