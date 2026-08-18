#include "uart_interface.h"

#define UART_BAUD_RATE 115200

void uart_init()
{
    Serial.begin(UART_BAUD_RATE);
}

bool uart_command_available()
{
    return Serial.available() > 0;
}

String uart_read_command()
{
    String command = Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();

    return command;
}

void uart_send(const String &message)
{
    Serial.println(message);
}