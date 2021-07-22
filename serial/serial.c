#include "uart_driver.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void print_sensor_data(sensor_data_t *sensor_data);

int main() {
	sensor_data_t sensor_data;
	get_sensor_data(&sensor_data);
	print_sensor_data(&sensor_data);
}

/** the data from the sensor comes in as following:
 * 1. id            [char,  1 bytes]
 * 2. int_data      [int,   4 bytes]
 * 3. short_data    [short, 2 bytes]
 * 4. char_data     [char,  1 bytes]
 */
void get_sensor_data(sensor_data_t *sensor_data) {
	uart_driver_start_communication();

	/*
	 * We assume that the endianness of the data matches
	 * the endianness of the current machine for simplicity.
	 */
#define COPY_MEMBER(member)                         \
	do {                                            \
		int __size = sizeof(sensor_data->member);   \
		uint8_t __data[__size];                       \
		for (int i = 0; i < __size; i++) {          \
			__data[i] = uart_driver_get_byte();       \
		}                                           \
		memcpy(&sensor_data->member, __data, __size); \
	} while (0)

	COPY_MEMBER(id);
	COPY_MEMBER(int_data);
	COPY_MEMBER(short_data);
	COPY_MEMBER(char_data);

#undef COPY_MEMBER
	uart_driver_stop_communication();
}

void print_sensor_data(sensor_data_t *sensor_data) {
	printf("id: %d\n", sensor_data->id);
	printf("int_data: %d\n", sensor_data->int_data);
	printf("short_data: %d\n", sensor_data->short_data);
	printf("char_data: %d\n", sensor_data->char_data);
}