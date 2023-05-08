#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/types.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/zephyr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

/* struct to hold BT packet data */
struct __attribute__((__packed__)) BT_Message {
	uint8_t preamble;
	uint8_t type;
	uint8_t length;
	uint8_t data[16];
	uint8_t did; 
};

/* struct to hold BT Node packet data */
struct __attribute__((__packed__)) BT_Node_Message {
	uint8_t preamble;
	uint8_t type;
	uint8_t length;
	int8_t rssiVals[12];
	//IDs will be the numeric value of letter ('A' 'B' etc.)
	// int8_t rssiID[12]; 
};

/* struct to hold sensor data*/
struct __attribute__((__packed__)) sensor_values {
	struct sensor_value temp;
	struct sensor_value press;
	struct sensor_value accelX;
	struct sensor_value accelY;
	struct sensor_value accelZ;
	struct sensor_value gyroX;
	struct sensor_value gyroY;
	struct sensor_value gyroZ;
	struct sensor_value magnX;
	struct sensor_value magnY;
	struct sensor_value magnZ;
	int ultraSonic;
};

#define DID_SAMPLE		0
#define DID_TEMP		1
#define DID_PRESSURE	2
#define DID_ACCELX		3
#define DID_ACCELY		4
#define DID_ACCELZ		5
#define DID_GYROX		6
#define DID_GYROY		7
#define DID_GYROZ		8
#define DID_MAGNX		9
#define DID_MAGNY		10
#define DID_MAGNZ		11
#define DID_US			12
#define DID_ORIENTATION 13
#define DID_ALTITUDE    14
#define DID_LED0		15
#define DID_LED1		16
#define DID_PB			17

#define BLU_UUID_VAL 		0xDEAD
#define BLU_CHRC_UUID_VAL 	0xD00D

#define AHU_UUID_VAL 		0xB00B
#define AHU_CHRC_UUID_VAL 	0x1350

#define BLU_UUID \
	BT_UUID_DECLARE_16(BLU_UUID_VAL)

#define BLU_CHRC_UUID \
	BT_UUID_DECLARE_16(BLU_CHRC_UUID_VAL)

#define AHU_UUID \
	BT_UUID_DECLARE_16(AHU_UUID_VAL)

#define AHU_CHRC_UUID \
	BT_UUID_DECLARE_16(AHU_CHRC_UUID_VAL)

#define MAX_NODES 12

extern struct k_sem bt_recv_sem;
extern struct k_sem bt_send_sem;
extern struct k_msgq bt_queue;
void bt_init(void);
void blu_send(uint16_t handle, struct BT_Node_Message *packet);
void ahu_bt_thread(void);
int get_blu_handle();
int get_ble_connected();
