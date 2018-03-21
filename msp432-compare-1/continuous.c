#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

Timer_A_ContinuousModeConfig contConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1, // 3 MHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_SKIP_CLEAR
};

Timer_A_CompareModeConfig cmpConfig = {
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_SET_RESET,
        0x0000
};

Timer_A_CompareModeConfig cmpConfig2 = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_SET_RESET,
        0x2000
};

int main(void) {

  // Stop WDT
  WDT_A_holdTimer();

  // RED LED
  GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN6);
  GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN6);

  // GREEN LED
  GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN4);
  GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN4);

  GPIO_setAsPeripheralModuleFunctionOutputPin(
          GPIO_PORT_P2,
          GPIO_PIN6,
          GPIO_PRIMARY_MODULE_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(
          GPIO_PORT_P2,
          GPIO_PIN4,
          GPIO_PRIMARY_MODULE_FUNCTION);

  Timer_A_setCompareValue         (TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, 0x3FFF);
  Timer_A_configureContinuousMode (TIMER_A0_BASE, &contConfig);
  Timer_A_initCompare             (TIMER_A0_BASE, &cmpConfig);
  Timer_A_initCompare             (TIMER_A0_BASE, &cmpConfig2);
  Timer_A_startCounter            (TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

  while (1) ;

}
