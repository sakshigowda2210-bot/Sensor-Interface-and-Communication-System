#ifndef DS18B20_DRIVER_H
#define DS18B20_DRIVER_H

#include <Arduino.h>

#define DS18B20_DISCONNECTED -127.0

void ds18b20_init();

float ds18b20_read_temperature();

bool ds18b20_is_connected();

#endif