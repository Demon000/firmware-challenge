#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	// By the way, this line was wrong
	// char[10] source;
	char source[10];
	int id;
	unsigned long timestamp;
	int temperature;
	int humidity;
	int pressure;
} measurement_t;

void measurement_print_struct(measurement_t *measurement) {
#define PRINT(member, format) \
	printf(#member ": " format "\n", measurement->member)

	PRINT(source, "%s");
	PRINT(id, "%d");
	PRINT(timestamp, "%lu");
	PRINT(temperature, "%d");
	PRINT(humidity, "%d");
	PRINT(pressure, "%d");

#undef PRINT
}

/*
 * If the compilers of all the involved architectures have the same ABI,
 * and we do not care about structure padding, we can simply cast the structure
 * to a byte array.
 */
uint8_t *measurement_to_byte_array_simple(measurement_t *measurement, size_t *size) {
	*size = sizeof(*measurement);
	return (uint8_t *) measurement;
}

/*
 * But usually, ABI compatibility cannot be guaranteed.
 * Compilers might generate different structure layouts.
 * We can pack all the fields into a byte array one after the other.
 * Warning: the architectures between which the data transfer is done
 * might use different endianness, but I haven't handled endianness here
 * for simplicity.
 * Note: the compiler added 6 bytes of padding here, after the source member.
 */
uint8_t *measurement_to_byte_array(measurement_t *measurement, size_t *size) {
	*size = 0;

	/*
     * Add up all the sizes, to know how much to allocate.
     * Use macros to avoid duplicating the operation.
     */
#define ADD_SIZE(member) \
	*size += sizeof(measurement->member)

	ADD_SIZE(source);
	ADD_SIZE(id);
	ADD_SIZE(timestamp);
	ADD_SIZE(temperature);
	ADD_SIZE(humidity);
	ADD_SIZE(pressure);

#undef ADD_SIZE

	uint8_t *data = malloc(*size);

	/*
	 * Copy each member and move the data pointer forward.
	 */
#define COPY_MEMBER(member)                                              \
	do {                                                                 \
		memcpy(data, &measurement->member, sizeof(measurement->member)); \
		data += sizeof(measurement->member);                             \
	} while (0)

	COPY_MEMBER(source);
	COPY_MEMBER(id);
	COPY_MEMBER(timestamp);
	COPY_MEMBER(temperature);
	COPY_MEMBER(humidity);
	COPY_MEMBER(pressure);

#undef COPY_MEMBER

	/*
	 * Move the data pointer back to the start of the allocated memory.
	 */
	return data - *size;
}

void measurement_print_byte_array(uint8_t *data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		printf("%02X", data[i]);
	}
	printf("\n");
}

measurement_t *measurement_byte_array_to_struct(uint8_t *data, size_t size) {
	measurement_t *measurement = malloc(sizeof(*measurement));

#define COPY_MEMBER(member)                                              \
	do {                                                                 \
		memcpy(&measurement->member, data, sizeof(measurement->member)); \
		data += sizeof(measurement->member);                             \
	} while (0)

	COPY_MEMBER(source);
	COPY_MEMBER(id);
	COPY_MEMBER(timestamp);
	COPY_MEMBER(temperature);
	COPY_MEMBER(humidity);
	COPY_MEMBER(pressure);

#undef COPY_MEMBER

	return measurement;
}

int main() {
	measurement_t measurement = {
			.source = "BME280",
			.id = 1,
			.timestamp = 1626942387,
			.temperature = 22,
			.humidity = 61,
			.pressure = 1019,
	};
	printf("Initial struct:\n");
	measurement_print_struct(&measurement);
	printf("\n");

	size_t size;
	uint8_t *data = measurement_to_byte_array(&measurement, &size);
	printf("Byte array:\n");
	measurement_print_byte_array(data, size);
	printf("\n");

	measurement_t *converted_measurement = measurement_byte_array_to_struct(data, size);
	printf("Converted struct:\n");
	measurement_print_struct(converted_measurement);
	printf("\n");

	free(converted_measurement);
	free(data);
}
