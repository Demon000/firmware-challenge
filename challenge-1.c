#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Define a fixed size for the display, as the problem states.
 */
#define LED_PANEL_W 8
#define LED_PANEL_H 8

/*
 * Define a helper to extract the n-th bit of a number.
 */
#define GET_BIT(value, n) ((value >> n) & 0x1)

/*
 * If output as matrix of characters means printing to console.
 */
void led_panel_print_byte_array(uint8_t *data) {
	for (int i = 0; i < LED_PANEL_H; i++) {
		for (int j = 0; j < LED_PANEL_W; j++) {
			bool value = GET_BIT(data[i], j);
			printf("%c", value ? '#' : '.');
		}
		printf("\n");
	}
}

/*
 * Otherwise, if output as matrix means actually converting the byte array to a matrix of characters.
 */
char **led_panel_byte_array_to_char_matrix(uint8_t *data) {
	char **output = malloc(LED_PANEL_H * sizeof(char *));
	for (int i = 0; i < LED_PANEL_H; i++) {
		output[i] = malloc(LED_PANEL_W * sizeof(char));
		for (int j = 0; j < LED_PANEL_W; j++) {
			bool value = GET_BIT(data[i], j);
			output[i][j] = value ? '#' : '.';
		}
	}

	return output;
}

void led_panel_print_char_matrix(char **data) {
	for (int i = 0; i < LED_PANEL_H; i++) {
		for (int j = 0; j < LED_PANEL_W; j++) {
			printf("%c", data[i][j]);
		}
		printf("\n");
	}
}

void led_panel_free_char_matrix(char **data) {
	for (int i = 0; i < LED_PANEL_H; i++) {
		free(data[i]);
	}
	free(data);
}

int main() {
	uint8_t A[] = {0x00, 0x18, 0x24, 0x24, 0x7E, 0x42, 0x42, 0x00};
	uint8_t B[] = {0x00, 0x3f, 0x41, 0x61, 0x3f, 0x61, 0x41, 0x3f};

	led_panel_print_byte_array(A);
	printf("\n");

	led_panel_print_byte_array(B);
	printf("\n");

	char **A_char_matrix = led_panel_byte_array_to_char_matrix(A);
	led_panel_print_char_matrix(A_char_matrix);
	led_panel_free_char_matrix(A_char_matrix);
	printf("\n");

	char **B_char_matrix = led_panel_byte_array_to_char_matrix(B);
	led_panel_print_char_matrix(B_char_matrix);
	led_panel_free_char_matrix(B_char_matrix);
	printf("\n");
}
