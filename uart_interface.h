#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#include <Arduino.h>

void uart_init();

bool uart_command_available();

String uart_read_command();

void uart_send(const String &message);

#endif