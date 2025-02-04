/* Biblioteca ESP_CountFreq para la medición de frecuencia en los ESP32

MIT License Copyright © 2025 EA2EBK
Versión: 1.0.0

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "ESP_CountFreq.h"

volatile uint32_t ESP_CountFreqClass::count_output = 0;
volatile uint8_t ESP_CountFreqClass::count_ready = 0;
pcnt_unit_t ESP_CountFreqClass::pcnt_unit = PCNT_UNIT_0;
timer_group_t ESP_CountFreqClass::timer_group = TIMER_GROUP_0;
timer_idx_t ESP_CountFreqClass::timer_idx = TIMER_0;
int8_t ESP_CountFreqClass::pcnt_input_pin = -1;

void ESP_CountFreqClass::begin(uint32_t usec, int8_t input_pin) {
    pcnt_input_pin = input_pin;  // Almacena el pin de entrada

    // Configuración del contador de pulsos (PCNT)
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = pcnt_input_pin,  // Usa la variable almacenada
        .ctrl_gpio_num = -1,   // No se usa pin de control
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_KEEP,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DIS,
        .counter_h_lim = 0,
        .counter_l_lim = 0,
        .unit = pcnt_unit,
        .channel = PCNT_CHANNEL_0,
    };
    pcnt_unit_config(&pcnt_config);
    pcnt_counter_pause(pcnt_unit);
    pcnt_counter_clear(pcnt_unit);
    pcnt_counter_resume(pcnt_unit);

    // Configuración del timer
    timer_config_t timer_config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 80,  // 80 MHz / 80 = 1 MHz (1 tick por microsegundo)
    };
    timer_init(timer_group, timer_idx, &timer_config);
    timer_set_counter_value(timer_group, timer_idx, 0);
    timer_set_alarm_value(timer_group, timer_idx, usec);
    timer_enable_intr(timer_group, timer_idx);
    timer_isr_callback_add(timer_group, timer_idx, onTimer, NULL, 0);
    timer_start(timer_group, timer_idx);
}

uint8_t ESP_CountFreqClass::available(void) {
    return count_ready;
}

uint32_t ESP_CountFreqClass::read(void) {
    count_ready = 0;
    return count_output;
}

void ESP_CountFreqClass::end(void) {
    timer_pause(timer_group, timer_idx);
    pcnt_counter_pause(pcnt_unit);
}

bool IRAM_ATTR ESP_CountFreqClass::onTimer(void* arg) {
    timer_group_clr_intr_status_in_isr(timer_group, timer_idx);
    timer_group_enable_alarm_in_isr(timer_group, timer_idx);

    int16_t count;
    pcnt_get_counter_value(pcnt_unit, &count);
    count_output = count;
    count_ready = 1;
    pcnt_counter_clear(pcnt_unit);

    return true;  // Devuelve true para indicar que la ISR fue manejada
}

ESP_CountFreqClass ESP_CountFreq;

