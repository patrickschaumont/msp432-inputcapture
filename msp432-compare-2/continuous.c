#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

Timer_A_UpDownModeConfig udConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1, // 3 MHz
        0x8000,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_SKIP_CLEAR
};

Timer_A_CompareModeConfig cmpConfig = {
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_TOGGLE_SET,
        0x6000
};

Timer_A_CompareModeConfig cmpConfig2 = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_TOGGLE_SET,
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

  Timer_A_configureUpDownMode     (TIMER_A0_BASE, &udConfig  );
  Timer_A_initCompare             (TIMER_A0_BASE, &cmpConfig );
  Timer_A_initCompare             (TIMER_A0_BASE, &cmpConfig2);
  Timer_A_startCounter            (TIMER_A0_BASE, TIMER_A_UPDOWN_MODE);

  while (1) ;

}
