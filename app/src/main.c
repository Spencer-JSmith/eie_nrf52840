/*
 * main.c
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1



int main(void) {
  if (0 > BTN_init() || 0 > LED_init())
    return 0;

  int sum = 0;


  while(1) {
    if (BTN_check_clear_pressed(BTN0)) {
      sum++;
      if (16 == sum)
        sum = 0;
      
      printk("%d\n", sum);
    }
    
    LED_set(LED0, sum&1);
    LED_set(LED1, sum&2);
    LED_set(LED2, sum&4);
    LED_set(LED3, sum&8);


    k_msleep(SLEEP_MS);
  }
	return 0;
}
