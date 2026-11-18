#pragma once
#include <stdint.h>


int testingwtf(int c);

uint8_t setPortPinDir( uint32_t port, uint8_t pin, uint8_t isOutput);

uint8_t setPinDir(uint8_t pin, uint8_t isOutput);

uint8_t setPortPinWrtie( uint32_t port, uint8_t pin, uint8_t Output);

uint8_t setPinWrtie(uint32_t pin, uint8_t Output);

uint8_t readPortPin( uint32_t port, uint8_t pin);

uint8_t readPin(uint32_t pin);