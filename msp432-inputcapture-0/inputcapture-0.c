#include <stdlib.h>
#include "display.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

Timer_A_ContinuousModeConfig continuousModeConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // SMCLK/1 = 3MHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_SKIP_CLEAR                   // Skup Clear Counter
};

/* Timer_A Capture Mode Configuration Parameter */
const Timer_A_CaptureModeConfig captureModeConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_4,        // CC Register 4
        TIMER_A_CAPTUREMODE_RISING_EDGE,          // Rising Edge
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,        // CCIxA Input Select
        TIMER_A_CAPTURE_SYNCHRONOUS,              // Synchronized Capture
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,  // Enable interrupt
        TIMER_A_OUTPUTMODE_OUTBITVALUE            // Output bit value
};


uint16_t previouscapture = 0;
uint16_t delta;

void TA2_N_IRQHandler(void) {
    uint16_t thiscapture;

    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_4);

    thiscapture =
            Timer_A_getCaptureCompareCount(TIMER_A2_BASE,
                                           TIMER_A_CAPTURECOMPARE_REGISTER_4);

    delta = thiscapture - previouscapture;
    previouscapture = thiscapture;
}

int main(void) {

    WDT_A_holdTimer();
    InitDisplay();

    // input capture pin
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN7,
                                               GPIO_PRIMARY_MODULE_FUNCTION);

    // blue LED pin
    GPIO_setAsOutputPin    (GPIO_PORT_P5,    GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5,    GPIO_PIN6);

    // Enable input capture with interrupts
    Timer_A_initCapture(TIMER_A2_BASE, &captureModeConfig);
    Interrupt_enableInterrupt(INT_TA2_N);
    Interrupt_enableMaster();

    // Enable continuous mode counter
    Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

    volatile unsigned tmp;

    while (1) {
        // toggle blue LED here
        GPIO_setOutputHighOnPin (GPIO_PORT_P5, GPIO_PIN6);
        GPIO_setOutputLowOnPin  (GPIO_PORT_P5, GPIO_PIN6);

        for (tmp = 0; tmp < 50; tmp ++) ;

        // toggle blue LED again
        GPIO_setOutputHighOnPin (GPIO_PORT_P5, GPIO_PIN6);
        GPIO_setOutputLowOnPin  (GPIO_PORT_P5, GPIO_PIN6);

        // delta contains number of cycles between toggles
        DrawUnsigned16 (1, "DLTA", (unsigned) delta);
    }
}
