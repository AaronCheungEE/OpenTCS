#include "nil.h"
#include "hwinit.h"
#include "threads.h"

/*
 * All clocks are enabled in hwInit();
 * All Pin Mux are set in board.h
 */

/*
 * Thread 0.
 */
NIL_WORKING_AREA(waThread0, 64);
NIL_THREAD(Thread0, arg) {

  (void)arg;
  /* WWDG clock counter = (PCLK1 (48MHz)/4096)/8 = 1464Hz (~683 us)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  /* Set Window value to 126; WWDG counter should be refreshed only when the counter
    is below 126 (and greater than 64) otherwise a reset will be generated */
  WWDG_SetWindowValue(126);

  /* Enable WWDG and set counter value to 127, WWDG timeout = ~683 us * 64 = 43.7 ms
     In this case the refresh window is: ~683 * (127-126)= 0.683ms < refresh window < ~683 * 64 = 43.7ms
     */
  WWDG_Enable(127);
  while (true) {
    nilThdSleepMilliseconds(20);
    gpioTogglePad(GPIOC, GPIOC_LED3); /* Watchdog heartbeat */
    WWDG_SetCounter(127);
  }
}

/*
 * Thread 1.
 */
NIL_WORKING_AREA(waThread1, 128);
NIL_THREAD(Thread1, arg) {

  (void)arg;
  while (true) {
    startLight();
  }
}

/*
 * Thread 2.
 */
NIL_WORKING_AREA(waThread2, 256);
NIL_THREAD(Thread2, arg) {

  (void)arg;
  while (true) {
    startDisplay();
  }
}

/*
 * Thread 3.
 */
NIL_WORKING_AREA(waThread3, 128);
NIL_THREAD(Thread3, arg) {

  (void)arg;
  while (true) {
    startIgnition();
  }
}

/*
 * Thread 4.
 */
NIL_WORKING_AREA(waThread4, 128);
NIL_THREAD(Thread4, arg) {

  (void)arg;
  startAdc(); /* ADC runs in continuous mode with DMA */
  while (true) {
    startSensors();
  }
}


/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
NIL_THREADS_TABLE_BEGIN()
  NIL_THREADS_TABLE_ENTRY("Watchdog", Thread0, NULL, waThread0, sizeof(waThread0))
  NIL_THREADS_TABLE_ENTRY("Light", Thread1, NULL, waThread1, sizeof(waThread1))
  NIL_THREADS_TABLE_ENTRY("Display", Thread2, NULL, waThread2, sizeof(waThread2))
  NIL_THREADS_TABLE_ENTRY("Ignition", Thread3, NULL, waThread3, sizeof(waThread3))
  NIL_THREADS_TABLE_ENTRY("Sensors", Thread4, NULL, waThread4, sizeof(waThread4))
NIL_THREADS_TABLE_END()

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations:
   * - HW specific initialization.
   * - Nil RTOS initialization.
   */
  hwInit();
  settingsInit();
  nilSysInit();

  /* This is now the idle thread loop, you may perform here a low priority
     task but you must never try to sleep or wait in this loop.*/
  while (true) {
  }
}
