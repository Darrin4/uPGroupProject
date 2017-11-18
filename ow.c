#include <p18f452.h>
#include <stdio.h>
#include <delays.h>
#include "ow.h"

extern unsigned char ow_reset(void)
{
    short device_found=0;
    // DQ High
    OW_TRIS=INPUT;
    // DQ Low
    OW_LAT=0;
    OW_TRIS=OUTPUT;
    // delay 480us
    Delay10TCYx(48);
    // DQ High
    OW_TRIS=INPUT;
    // Wait for the sensors to respond
    Delay10TCYx(24);
    // Determine is a device has responded
    device_found = !OW_PIN;
    return device_found;
}

extern void ow_write_byte(unsigned char data)
{
    char i;
    for (i=0;i<8;i++)
    {
        // DQ Low
        OW_LAT=0;
        OW_TRIS=OUTPUT;
        // Keep it low for 10us to start the WRITE
        Delay10TCYx(1);
        // Keep low i.e. keep output mode and low if WRITE0
        // or release line i.e. make input to take it high if WRITE1
        OW_TRIS = data & 0x01;
        Delay10TCYx(5);
        // Release the line. Total of 60us
        OW_TRIS=INPUT;
        // Some recovery time between the bits 2us
        Delay10TCYx(2);
        data =data >>1;
    }
}

extern char ow_read_byte(void)
{
    char data=0;
    char i;
    for (i=0;i<8;i++)
    {
        // DQ Low. Initiate READ
        OW_LAT=0;
        OW_TRIS=OUTPUT;
        Nop();
        Nop();
        // DQ High. Release
        OW_TRIS=INPUT;
        // Wait for total of 10 us since READ initiation
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        // Read the bit and place it in the data at the right position
        data = (OW_PIN << i) | data;
        // Additional time to complete the minimum 60 us READ cycle
        Delay10TCYx(5);
    }
    return data;
}
