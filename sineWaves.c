/*
 * PulsePoint — 2-channel sine wave test
 * CONFIRMED WORKING version from 4/10
 *
 * Watch points (View -> Expressions in CCS):
 *   loopCount     — increments every loop
 *   lastVal0      — CH0 code, should vary 6554-58981
 *   lastVal1      — CH1 code, different from lastVal0
 *   sine_lut[64]  — expect ~58981 (peak)
 *   sine_lut[192] — expect ~6554  (trough)
 *
 * Scope: 500ms/div, 500mV/div, Auto trigger
 * Probe DAC_Out_0 pad, ground to GND pad
 */

#include "ti_msp_dl_config.h"
#include "dacH.h"
#include <stdint.h>
#include <math.h>

#define SINE_N          256u
#define MID_CODE        32768u
#define SINE_AMPL       0.8f
#define SAMPLE_DELAY    32000u    /* 32MHz/32000 = 1000 samples/sec */

volatile uint32_t loopCount = 0;
volatile uint16_t lastVal0  = 0;
volatile uint16_t lastVal1  = 0;

static uint16_t sine_lut[SINE_N];

static void build_sine(void)
{
    for (uint32_t i = 0; i < SINE_N; i++)
    {
        float s = sinf(2.0f * 3.1415926f * (float)i / (float)SINE_N);
        float x = (float)MID_CODE + s * (SINE_AMPL * 32767.0f);
        if (x < 0.0f)     x = 0.0f;
        if (x > 65535.0f) x = 65535.0f;
        sine_lut[i] = (uint16_t)(x + 0.5f);
    }
}

int main(void)
{
    SYSCFG_DL_init();

    /* CLR HIGH — required for DAC80508MC variant on PulsePoint board */
    DAC_CLR_HIGH();
    delay_cycles(32000);
    delay_cycles(320000);           /* 10ms power-on */

    DAC80508_soft_reset();
    delay_cycles(320000);           /* 10ms after reset */

    /* CLR HIGH again after reset */
    DAC_CLR_HIGH();
    delay_cycles(32000);

    DAC80508_write_reg(DAC80508_REG_CONFIG, 0x0000);
    DAC80508_write_reg(DAC80508_REG_GAIN,   0x00FF); /* 1x gain, 0-2.5V */
    DAC80508_write_reg(DAC80508_REG_SYNC,   0x0000);

    for (uint8_t ch = 0; ch < 8; ch++)
        DAC80508_set_channel(ch, MID_CODE);

    build_sine();

    uint8_t idx[2] = {0, 0};

    while (1)
    {
        loopCount++;

        /* CH0 — step 1 -> ~2 Hz */
        idx[0] += 1;
        lastVal0 = sine_lut[idx[0]];
        DAC80508_set_channel(0, lastVal0);
        delay_cycles(SAMPLE_DELAY);

        /* CH1 — step 2 -> ~4 Hz */
        idx[1] += 2;
        lastVal1 = sine_lut[idx[1]];
        DAC80508_set_channel(1, lastVal1);
        delay_cycles(SAMPLE_DELAY);
    }
}