/*
 * main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

#define my_service_UUID BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x1234, 0x1234, 0x1234567890ab)
#define my_char_UUID    BT_UUID_128_ENCODE(0xabcdefab, 0xcdef, 0xcdef, 0xcdef, 0xabcdefabcdef)

#define BLE_CUSTOM_CHARACTERISTIC_MAX_DATA_LEN 20

static const struct bt_data ad_data[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

static uint8_t ble_custom_char_user_data[20] = {};

static ssize_t ble_custom_char_read_cb(struct bt_conn *conn, const struct bt_gatt_attr* attr,
                                       void* buf, uint16_t len, uint16_t offset) {
  const char* value = attr->user_data;
  return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}

static ssize_t ble_custom_char_write_cb(struct bt_conn *conn, const struct bt_gatt_attr* attr,
                                        const void* buf, uint16_t len, uint16_t offset,
                                        uint8_t flags) {
  uint8_t* value_ptr = attr->user_data;
  if (offset + len > BLE_CUSTOM_CHARACTERISTIC_MAX_DATA_LEN) {
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
  }
  memcpy(value_ptr + offset, buf, len);
  return len;
}

static const struct bt_uuid_128 ble_custum_service_uuid = BT_UUID_INIT_128(my_service_UUID);
static const struct bt_uuid_128 ble_custom_char_uuid = BT_UUID_INIT_128(my_char_UUID);

BT_GATT_SERVICE_DEFINE(
  ble_custom_service,
  BT_GATT_PRIMARY_SERVICE(&ble_custum_service_uuid),

  BT_GATT_CHARACTERISTIC(
    &ble_custom_char_uuid.uuid,
    (BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    (BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    ble_custom_char_read_cb,
    ble_custom_char_write_cb,
    ble_custom_char_user_data
  )
);



int main(void) {

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  int rv = bt_enable(NULL);

  if (rv) {
    printf("Nahh because that didn't work chump with error %d\n", rv);
    return 0;
  }

  rv = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad_data, ARRAY_SIZE(ad_data), NULL, 0);

  if (rv) {
    printk("Failed frfr with real error %d\n", rv);
    return 0;
  }




  while(1) {
    k_msleep(SLEEP_MS);
  }
	return 0;
}
