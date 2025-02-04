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
*/

#ifndef ESP_CountFreq_h
#define ESP_CountFreq_h

#include <Arduino.h>
#include "driver/pcnt.h"
#include "driver/timer.h"

class ESP_CountFreqClass {
public:
    static void begin(uint32_t usec, int8_t input_pin);  // Acepta el pin de entrada
    static uint8_t available(void);
    static uint32_t read(void);
    static void end(void);

private:
    static volatile uint32_t count_output;
    static volatile uint8_t count_ready;
    static pcnt_unit_t pcnt_unit;
    static timer_group_t timer_group;
    static timer_idx_t timer_idx;
    static int8_t pcnt_input_pin;  // Variable para almacenar el pin de entrada
    static bool IRAM_ATTR onTimer(void* arg);  // Cambia la firma de la función
};

extern ESP_CountFreqClass ESP_CountFreq;

#endif

