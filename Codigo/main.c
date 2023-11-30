//Incluyo Librerias
#include <stdio.h>
#include "pico/stdlib.h"
#include <dht.h>
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lcd1602_i2c.h"

#define SENSOR_PIN 28 
#define SENSOR_PIN2 27
const int numMediciones=10;
 
int main()
{
    // Inicializo USB
    stdio_init_all();
    adc_init();
    adc_gpio_init(SENSOR_PIN);
    adc_gpio_init(SENSOR_PIN2);
    // Inicializo I2C
    i2c_init(i2c0, 400 * 1000);
    // Habilito GPIO
    gpio_set_function(4,GPIO_FUNC_I2C);
    gpio_set_function(5,GPIO_FUNC_I2C);
    float SumatoriaTemp= 0.0;
    // inicializo LCD   
    lcd_init();
    //Variable para texto
    char str [16];

    while(1)
    {
        adc_select_input(2);
        uint16_t sensor_value = adc_read();
        // Realiza la conversión de lectura a porcentaje de humedad según las especificaciones del sensor
        float humidity_percentage = (int16_t)sensor_value * (-100.0)/4095.0 + 100;
        // Muestro en pantalla la humedad
        printf("Humedad del suelo: %.2f%%\n", humidity_percentage);

        adc_select_input(1);
        for (int a=0; a < numMediciones; a++){
        uint16_t sensor_value2 = adc_read();
        //Realizo la conversion de lectura del LM35
        float voltaje = (sensor_value2 / 4095.0 ) * 3.3;
        float temperatura = (voltaje * 100.0);
        SumatoriaTemp += temperatura;
        sleep_ms (200);
        }

        float TemperaturaProm= (SumatoriaTemp / numMediciones);
        //MUestro en Pantalla
        printf("Temperatura Ambiente: %.0f", TemperaturaProm);
        SumatoriaTemp=0.0;
        sleep_ms(200);

        lcd_clear();
        // Armo String
        sprintf(str,"Temp: %.0f C", TemperaturaProm);
        lcd_string(str);
        lcd_set_cursor (1,0);
        sprintf(str,"Humedad: %.0f %%", humidity_percentage);
        lcd_string(str);

        sleep_ms(200);
    }

    return 0;
}
