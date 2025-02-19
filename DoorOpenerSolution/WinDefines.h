
// *****************************************************************************
// This file is only required if you compile on Visual Studio.
// It assures that you don't get compiler errors.
// The idea is only to check the encryption stuff on Windows.
// But you can also write your own code that communicates with a PN532 board
// through additional hardware (e.g. a PCI card that has digital in/outputs)
// *****************************************************************************

#pragma once

// disable "sprintf deprecated" warning
#pragma warning(disable: 4996) 

#include <windows.h>
#include <stdio.h>   // sprintf()
#include <conio.h>   // getch()

typedef unsigned __int64 uint64_t;
typedef __int64           int64_t;
typedef unsigned int     uint32_t;
typedef int               int32_t;
typedef unsigned short   uint16_t;
typedef short             int16_t;
typedef unsigned char     uint8_t;
typedef char               int8_t;
typedef unsigned char        byte;


static uint32_t millis()
{
    return GetTickCount();
}

static void delay(int s32_MilliSeconds)
{
    Sleep(s32_MilliSeconds);
}

// Windows is not a real time operating system (Linux neither).
// There is no API in Windows that supports delays shorter than approx 20 milliseconds. 
// Sleep(1) will sleep approx 20 ms
// This function occupies 100% of a CPU core. Not nice, but there is no other way to do this in Windows.
static void delayMicroseconds(int s32_MicroSeconds)
{
    static __int64 s64_Frequ = 0;
    if (s64_Frequ == 0)
        QueryPerformanceFrequency((LARGE_INTEGER*)&s64_Frequ); // called only once

    __int64 s64_Counter;
    QueryPerformanceCounter((LARGE_INTEGER*)&s64_Counter);

    __int64 s64_Finished = s64_Counter + (s64_Frequ * s32_MicroSeconds) / 1000000;
    do
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&s64_Counter);
    }
    while (s64_Counter < s64_Finished);
}

static void pinMode(byte u8_Pin, byte u8_Mode)
{
    // To test this library completely on Windows you would need a PCI hardware that connects to the PN532 via SPI bus
    // This function defines for all SPI pins if they are input or output
}

static void digitalWrite(byte u8_Pin, byte u8_Status)
{
    // To test this library completely on Windows you would need a PCI hardware that connects to the PN532 via SPI bus
    // This function writes one of the SPI pins
}

static byte digitalRead(byte u8_Pin)
{
    // To test this library completely on Windows you would need a PCI hardware that connects to the PN532 via SPI bus
    // This function reads one of the SPI pins
    return 0;
}


class SerialSubstitue
{
public:
    void begin(uint32_t u32_Baud) 
    {
        // not used
    }
    int available() 
    {
        return 0; // not used
    }
    int read() 
    {
        return getch(); // not used
    }
    void print(const char* s8_Text)
    {
        // Print to the Console
        printf("%s", s8_Text);
    }
};

static SerialSubstitue Serial;
