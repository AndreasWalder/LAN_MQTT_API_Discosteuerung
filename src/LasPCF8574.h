//LasPCF8574.h Version 1.0

#ifndef _LASPCF8574_H_
#define _LASPCF8574_H_

#define defined_ATtiny (defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__))

#if defined_ATtiny
  #include "TinyWireM.h"
  #define Wire TinyWireM
#else
  #include <Wire.h>
#endif

#include "Arduino.h"

#define ein false
#define aus true
#define on false
#define off true

//Umsetzung Relais zu Relaispins
#define Relais1 0
#define Relais2 1
#define Relais3 2
#define Relais4 3
#define Relais5 4
#define Relais6 5
#define Relais7 6
#define Relais8 7
#define Relais9 0
#define Relais10 1
#define Relais11 2
#define Relais12 3
#define Relais13 4
#define Relais14 5
#define Relais15 6
#define Relais16 7
/*
//volatile deshalb damit der compiler die variable nicht wegoptimiert
volatile bool stateRelais1;
volatile bool stateRelais2;
volatile bool stateRelais3;
volatile bool stateRelais4;
volatile bool stateRelais5;
volatile bool stateRelais6;
volatile bool stateRelais7;
volatile bool stateRelais8;
volatile bool stateRelais9;
volatile bool stateRelais10;
volatile bool stateRelais11;
volatile bool stateRelais12;
volatile bool stateRelais13;
volatile bool stateRelais14;
volatile bool stateRelais15;
volatile bool stateRelais16;
*/

class LasPCF8574Class
{
  public:

	explicit LasPCF8574Class(byte I2CAdd);
    explicit LasPCF8574Class();

    void begin();
    void begin(byte I2CAdd);
    void writeByte(byte sByte);
    byte readByte(void);
    byte readBit(byte bitNumber);
    boolean isReady(void);
    void writeBit(byte bitNumber, bool bitWert);
    void writeBitHigh(byte bitNumber);
    void writeBitLow(byte bitNumber);
    byte readBackBit(byte bitNumber);
    byte readBackByte();
    void toggleBit(byte bitNumber);
    void toggleByte();
        
  private:
    byte _i2cAdd;
    byte lastByte;
    byte _sByte;
    byte _pinNumber;
    byte _bitWert;

     
};

#endif
