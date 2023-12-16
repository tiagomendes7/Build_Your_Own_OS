
#include "port.h"



Port::Port(uint16_t portnumber)
{
    this->portnumber = portnumber;
}

Port::~Port()
{
}




Port8bit::Port8bit(uint8_t portnumber)
: Port(portnumber)
{
}

Port8bit::~Port8bit()
{
}

void Port8bit::Write(uint8_t data)
{
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8_t Port8bit::Read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));

    return result;
}




SlowPort8bit::SlowPort8bit(uint8_t portnumber)
: Port8bit(portnumber)
{
}

SlowPort8bit::~SlowPort8bit()
{
}

void SlowPort8bit::Write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
}




Port16bit::Port16bit(uint16_t portnumber)
: Port(portnumber)
{
}

Port16bit::~Port16bit()
{
}

void Port16bit::Write(uint16_t data)
{
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint16_t Port16bit::Read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portnumber));

    return result;
}




Port32bit::Port32bit(uint32_t portnumber)
: Port(portnumber)
{
}

Port32bit::~Port32bit()
{
}

void Port32bit::Write(uint32_t data)
{
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint32_t Port32bit::Read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portnumber));

    return result;
}



