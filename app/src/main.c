/*
 * main.c
 */

#include <inttypes.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#include "my_state_machine.h"

#define SLEEP_MS 1

int main(void) {
  int rv;

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  state_machine_init();

  while(1) {
    // rv = state_machine_run();
    // if (0 > rv) {
    //   return 0;
    // }
    

    k_msleep(SLEEP_MS);
  }
	return 0;
}
