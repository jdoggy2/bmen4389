/*
1. include libraries
2. define variables
3. write functions
4. call functions in main
*/

// =============================
// 1. include libraries
// =============================
#include "ti_msp_dl_config.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "empty_library.h"          // FIXED: was <empty_library.h>


// =============================
// 2. define variables
// =============================
#define NUM_CHANNELS  8
#define TABLE_SIZE    256
#define SAMPLE_RATE   10000         
/*
// FIXED: was 100000. At 2MHz SPI, 8 channels x 3 bytes
= 24 bytes per ISR ~ 96us total. 10kHz gives 100us/tick.
// Raise SPI clock to 20MHz in SysConfig to use higher rates.*/

void processCommand(char *cmd);

// UART buffer
#define BUFFER_SIZE 32
char    rxBuffer[BUFFER_SIZE];
uint8_t rxIndex = 0;

// control flags
volatile uint8_t enableOUT = 0;
volatile uint8_t runSYS    = 0;


// =============================
// a. SINE LUT
// =============================
uint16_t sineTable[TABLE_SIZE];

void genSineTable(void)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        float angle = (2.0f * 3.14159265f * i) / TABLE_SIZE;
        float s     = (sinf(angle) + 1.0f) * 0.5f;
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
        dds[i].step  = (uint32_t)(
            (freqs[i] * (float)(1ULL << 32)) / SAMPLE_RATE
        );
    }
}


// =============================
// e. DAC80508 initialization
//    ADDED: must call before writing any channel.
//    Sends soft reset, disables SYNC latching so
//    every SPI write updates the output immediately.
// =============================
void DAC80508_init(void)
{
    // Soft reset — clears all registers to power-on defaults
    DAC80508_WRITE(DAC80508_REG_TRIGGER, dacSOFTRESET_VAL);

    // Short delay to let the DAC finish resetting
    for (volatile int i = 0; i < 10000; i++);

    // SYNC = 0x0000: all 8 channels update asynchronously
    // (output changes on the rising edge of CS for each write).
    // This is correct when LDAC is tied to GND.
    DAC80508_WRITE(DAC80508_REG_SYNC, 0x0000);

    // GAIN = 0x01FF: REF-DIV/2 disabled, all channel gains = 1x
    // Adjust if you need 2x gain on any channel.
    DAC80508_WRITE(DAC80508_REG_GAIN, 0x01FF);
}


// =============================
// f. TIMER ISR
//    FIXED: removed local DAC_write_channel call.
//    Now uses dacSetChannel() from empty_library.h
//    which has correct CS/busy sequencing.

void myTIMER_INST_IRQHandler(void)
{
    if (enableOUT && runSYS)
    {
        for (int i = 0; i < NUM_CHANNELS; i++)
        {
            dds[i].phase += dds[i].step;

            uint8_t  index = (uint8_t)(dds[i].phase >> 24);
            uint16_t value = sineTable[index];

            dacSetChannel(i, value);            // FIXED: was DAC_write_channel()
        }
    }

    DL_Timer_clearInterruptStatus(
        myTIMER_INST,
        DL_TIMER_INTERRUPT_ZERO_EVENT
    );
}


// ============================= g. UART ISR
void myUART_INST_IRQHandler(void)
{
    if (DL_UART_getEnabledInterruptStatus(
            myUART_INST,
            DL_UART_INTERRUPT_RX))
    {
        char c = DL_UART_receiveData(myUART_INST);

        if (c == '\n')
        {
            rxBuffer[rxIndex] = '\0';
            processCommand(rxBuffer);
            rxIndex = 0;
        }
        else if (rxIndex < BUFFER_SIZE - 1)
        {
            rxBuffer[rxIndex++] = c;
        }
    }
}


// ============================= command parser
void processCommand(char *cmd)
{
    if (strcmp(cmd, "START") == 0)
    {
        runSYS = 1;
        LED_ON();
    }
    else if (strcmp(cmd, "STOP") == 0)
    {
        runSYS = 0;
        LED_OFF();
    }
    else if (strcmp(cmd, "ENABLE") == 0)
    {
        enableOUT = 1;
    }
    else if (strcmp(cmd, "DISABLE") == 0)
    {
        enableOUT = 0;
    }
}


// ============================= main 
int main(void)
{
    SYSCFG_DL_init();
    DAC80508_init();// reset + configure DAC before use
    genSineTable();
    init_DDS();

    // enable interrupts
    NVIC_EnableIRQ(myTIMER_INST_INT_IRQN);
    NVIC_EnableIRQ(myUART_INST_INT_IRQN);

    DL_Timer_startCounter(myTIMER_INST);

    while (1)
    {
        __WFI();
    }
}

/*
currently i have the mcu and dac hardware.
im going to wire out the 8 signals. will this generate an EM field when i read it oon an osciliscope?

i also have 8 pcb coils for the final design. will i need to use this for testing? ultimately, how will i program it 

*/