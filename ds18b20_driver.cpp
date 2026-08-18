#include "ds18b20_driver.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_PIN 4

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

void ds18b20_init()
{
    sensors.begin();
}

float ds18b20_read_temperature()
{
    sensors.requestTemperatures();

    float temperature = sensors.getTempCByIndex(0);

    if (temperature == DEVICE_DISCONNECTED_C)
    {
        return DS18B20_DISCONNECTED;
    }

    return temperature;
}

bool ds18b20_is_connected()
{
    float temperature = ds18b20_read_temperature();

    return temperature != DS18B20_DISCONNECTED;
}