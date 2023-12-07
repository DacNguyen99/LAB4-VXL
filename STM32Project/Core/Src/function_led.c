# include "main.h"
# include "function_led.h"

void blink_RED(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

void blink_YELLOW(void) {
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}

void blink_GREEN(void) {
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

void blink_BLUE(void) {
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
}

void blink_PURPLE(void) {
	HAL_GPIO_TogglePin(LED_PURPLE_GPIO_Port, LED_PURPLE_Pin);
}

