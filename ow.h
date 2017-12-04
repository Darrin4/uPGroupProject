/* Copyright © 2011 Demetris Stavrou
 * version 1.0
 * ---------------------------------
 * OneWire Library for the C18 Compiler
 * Only the basic functions are included
 * ow_reset() for OneWire Reset
 * ow_write_byte(char) for writting byte
 * char ow_read_byte() for reading byte
 *
 * Just include the ow.h file in your project and define three required
 * names - OW_LAT, OW_PIN, OW_TRIS
 *
 * You can use this library for interfacing the Maxim thermometers like DS18S20
 * NOTE: This library works for 40MHz Frequency i.e. TCY = 0.1us
 * You can adjust the timings accordingly, at least for now!
 */
#ifndef __OW_H
#define __OW_H

#define OW_LAT LATBbits.LATB5
#define OW_PIN PORTBbits.RB5
#define OW_TRIS TRISBbits.TRISB5

#define OUTPUT 0
#define INPUT 1
#define Nop() {_asm nop _endasm}

extern unsigned char ow_reset(void);
extern void ow_write_byte(unsigned char data);
extern char ow_read_byte(void);

#endif