#include <Arduino.h>

#include "ds18b20_driver.h"
#include "uart_interface.h"

#define DEFAULT_TEMP_LIMIT 30.0
#define MONITOR_INTERVAL 2000

float highTempLimit = DEFAULT_TEMP_LIMIT;

unsigned long lastMonitorTime = 0;

void monitorTemperature()
{
    float temperature = ds18b20_read_temperature();

    if (temperature == DS18B20_DISCONNECTED)
    {
        uart_send("[MONITOR] ERROR: DS18B20 disconnected!");
        return;
    }

    Serial.print("[MONITOR] Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C | Limit: ");
    Serial.print(highTempLimit, 2);
    Serial.print(" °C | Status: ");

    if (temperature >= highTempLimit)
    {
        Serial.println("HIGH");
        uart_send("[ALARM] High temperature detected!");
    }
    else
    {
        Serial.println("NORMAL");
    }
}

void readTemperature()
{
    float temperature = ds18b20_read_temperature();

    if (temperature == DS18B20_DISCONNECTED)
    {
        uart_send("ERROR: DS18B20 disconnected!");
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.println(" °C");
}

void printStatus()
{
    if (ds18b20_is_connected())
    {
        uart_send("Sensor: OK");
    }
    else
    {
        uart_send("Sensor: DISCONNECTED");
    }
}

void printLimit()
{
    Serial.print("Current temperature limit: ");
    Serial.print(highTempLimit, 2);
    Serial.println(" °C");
}

void setLimit(String command)
{
    String value = command.substring(9);

    float newLimit = value.toFloat();

    if (newLimit <= -55.0 || newLimit >= 125.0)
    {
        uart_send("ERROR: Invalid temperature limit.");
        uart_send("Allowed range: -55 to 125 °C");
        return;
    }

    highTempLimit = newLimit;

    Serial.print("Temperature limit set to: ");
    Serial.print(highTempLimit, 2);
    Serial.println(" °C");
}

void printHelp()
{
    uart_send("");
    uart_send("Available commands:");
    uart_send("TEMP       - Read temperature");
    uart_send("STATUS     - Check sensor status");
    uart_send("GETLIMIT   - Show temperature limit");
    uart_send("SETLIMIT X - Set temperature limit");
    uart_send("HELP       - Show commands");
    uart_send("");
}

void processCommand(String command)
{
    if (command == "TEMP")
    {
        readTemperature();
    }
    else if (command == "STATUS")
    {
        printStatus();
    }
    else if (command == "GETLIMIT")
    {
        printLimit();
    }
    else if (command.startsWith("SETLIMIT "))
    {
        setLimit(command);
    }
    else if (command == "HELP")
    {
        printHelp();
    }
    else
    {
        uart_send("ERROR: Unknown command");
        uart_send("Type HELP for available commands.");
    }
}

void setup()
{
    uart_init();

    ds18b20_init();

    uart_send("");
    uart_send("================================");
    uart_send(" SENSOR INTERFACE SYSTEM");
    uart_send("================================");
    uart_send("UART: 115200 8N1");
    uart_send("DS18B20 Driver: READY");
    uart_send("Temperature Monitoring: READY");
    uart_send("Type HELP for commands.");
    uart_send("");
}

void loop()
{
    // Automatic temperature monitoring
    if (millis() - lastMonitorTime >= MONITOR_INTERVAL)
    {
        lastMonitorTime = millis();

        monitorTemperature();
    }

    // UART command processing
    if (uart_command_available())
    {
        String command = uart_read_command();

        processCommand(command);
    }
}