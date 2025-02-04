# ESP_CountFreq para ESP32-S3

Esta librería permite medir la frecuencia de una señal utilizando el periférico PCNT y un timer en el ESP32-S3.

En el ESP32-S3, el periférico PCNT (Pulse Counter) está disponible en varios pines GPIO, pero no todos los pines son compatibles. La compatibilidad depende de la configuración interna del hardware y de las limitaciones de enrutamiento de señales en el ESP32-S3.

Pines compatibles con PCNT en el ESP32-S3
El ESP32-S3 tiene 8 unidades de PCNT (PCNT_UNIT_0 a PCNT_UNIT_7), y cada unidad puede conectarse a cualquier pin GPIO configurable. Sin embargo, debes asegurarte de que el pin que elijas no esté siendo utilizado por otro periférico (como SPI, I2C, UART, etc.).

Aquí hay una lista de pines GPIO que generalmente son compatibles con PCNT en el ESP32-S3:

GPIO 0 a GPIO 21 (excepto GPIO 6 a GPIO 11, que están reservados para la flash SPI interna).

GPIO 26 a GPIO 48 (si están disponibles en tu placa).

Pines no recomendados:
GPIO 6 a GPIO 11: Estos pines están conectados a la flash SPI interna y no deben usarse para otros propósitos.

GPIO 34 a GPIO 39: Estos pines son solo de entrada (no tienen resistencia pull-up/pull-down interna), pero aún pueden usarse para PCNT si solo necesitas contar pulsos.

Si deseas usar el PCNT en el ESP32-S3, puedes configurar cualquier pin GPIO compatible como entrada para el contador de pulsos. 

Consideraciones adicionales.
Frecuencia máxima: El PCNT en el ESP32-S3 puede contar pulsos a una frecuencia de hasta 40 MHz (dependiendo de la configuración del sistema y la calidad de la señal).

Filtro de ruido: El PCNT tiene un filtro de ruido integrado que puedes habilitar para evitar conteos erróneos debido a ruido en la señal.

Múltiples unidades: Puedes usar varias unidades de PCNT simultáneamente para contar pulsos en diferentes pines.

## Instalación

1. Descarga la librería como un archivo ZIP.
2. En el IDE de Arduino, ve a Sketch > Include Library > Add .ZIP Library.
3. Selecciona el archivo ZIP descargado.

## Uso básico

#include <ESP_CountFreq.h>

void setup() {
    Serial.begin(115200);
    ESP_CountFreq.begin(1000000, 14);  // Medir frecuencia cada 1 segundo, Pin entrada de la frecuencia a medir.
}

void loop() {
    if (ESP_CountFreq.available()) {
        uint32_t frequency = ESP_CountFreq.read();
        Serial.println(frequency);
    }
}

Explicación del Código:
Configuración del PCNT: Se configura el contador de pulsos (PCNT) para contar los pulsos en un pin específico (en este caso, el pin 14). El contador se incrementa en cada flanco positivo de la señal.

Configuración del Timer: Se configura un timer para generar una interrupción después de un período específico (en microsegundos). El timer se configura para contar en incrementos de 1 microsegundo.

Interrupción del Timer: Cuando el timer alcanza el valor de alarma, se genera una interrupción. En la rutina de interrupción (onTimer), se lee el valor del contador de pulsos, se almacena en count_output, y se marca count_ready como 1 para indicar que hay un nuevo valor disponible.

Lectura del Valor: La función read() devuelve el valor del contador y marca count_ready como 0.

Finalización: La función end() detiene el timer y el contador de pulsos.
