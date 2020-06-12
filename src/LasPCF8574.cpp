//LasPCF8574.cpp Version 1.0
//Code fuer Arduino und Attiny

/*
Ansteuerung des Portexpander PCF8574
Beispiel siehe unter:
http://arduino-projekte.webnode.at/meine-libraries/portexpander-pcf8574/

Funktionen siehe unter:
http://arduino-projekte.webnode.at/meine-libraries/portexpander-pcf8574/funktionen/
*/

#include "LasPCF8574.h"

#include "Arduino.h"


LasPCF8574Class::LasPCF8574Class()
{
}

//***************************************************************

LasPCF8574Class::LasPCF8574Class(byte i2cAdd)
{
	 _i2cAdd = i2cAdd;
}

//***************************************************************


void LasPCF8574Class::begin()
{
  Wire.begin();

  lastByte = 0x00;
}

//***************************************************************

void LasPCF8574Class::begin(byte i2cAdd)
{
  _i2cAdd = i2cAdd;
  Wire.begin();
  lastByte = 0x00;
}

//***************************************************************
//Abfrage, ob der Baustein ueber I2C ansprechbar ist

boolean LasPCF8574Class::isReady()
{
  Wire.beginTransmission(_i2cAdd);
  if(Wire.endTransmission(true) == 0) return(true);
  else return(false);
}

//***************************************************************
//Schreiben eines Byte auf das Port

void LasPCF8574Class::writeByte(byte sByte)
{
  _sByte = sByte;

  Wire.beginTransmission(_i2cAdd);
  Wire.write(_sByte);
  Wire.endTransmission(true);
  lastByte = _sByte;
}

//***************************************************************
//Schreiben eines einzelnen Bit auf das Port

void LasPCF8574Class::writeBit(byte pinNumber, bool bitWert)
{
  _pinNumber = pinNumber;
  _bitWert = bitWert;

  byte bitMask = 0x01 << _pinNumber;
  
  if(_bitWert == 1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(lastByte | bitMask);
    Wire.endTransmission(true);
    lastByte = lastByte | bitMask;
  }

  if(bitWert == 0)
  {
    bitMask = ~bitMask;

    Wire.beginTransmission(_i2cAdd);
    Wire.write(lastByte & bitMask);
    Wire.endTransmission(true);
    lastByte = lastByte & bitMask;
  }
}

//***************************************************************
//Setzen eines einzelnen Bit des Ports auf HIGH

void LasPCF8574Class::writeBitHigh(byte pinNumber)
{
  _pinNumber = pinNumber;
  byte bitMask = 0x01 << _pinNumber;

  Wire.beginTransmission(_i2cAdd);
  Wire.write(lastByte | bitMask);
  Wire.endTransmission(true);
  lastByte = lastByte | bitMask;
}

//***************************************************************
//Setzen eines einzelnen Bit des Ports auf LOW

void LasPCF8574Class::writeBitLow(byte pinNumber)
{
  _pinNumber = pinNumber;
  byte bitMask = ~(0x01 << _pinNumber);

  Wire.beginTransmission(_i2cAdd);
  Wire.write(lastByte & bitMask);
  Wire.endTransmission(true);
  lastByte = lastByte & bitMask;
}

//***************************************************************
//Ruecklesen des als Ausgang gesetztes Port

byte LasPCF8574Class::readBackByte()
{
  Wire.requestFrom(int(_i2cAdd), 1);
  while(Wire.available() == 0);
  byte wert = Wire.read();
  return(wert);
}

//***************************************************************
//Ruecklesen eines als Ausgang gesetzten Bit des Ports

byte LasPCF8574Class::readBackBit(byte pinNumber)
{
  _pinNumber = pinNumber;
  byte bitMask = (0x01 << _pinNumber);
    
  Wire.requestFrom(int(_i2cAdd), 1);
  while(Wire.available() == 0);
  byte wert = Wire.read();
  wert = ((wert & bitMask) >> _pinNumber);
  return(wert);
}


//***************************************************************
//Negieren des als Ausgang gesetzten Port

void LasPCF8574Class::toggleByte()
{
  byte statusByte = readBackByte();
  statusByte = ~statusByte;
  writeByte(statusByte);
}

//***************************************************************
//Negieren eines einzelnen als Ausgang gesetzten Bit

void LasPCF8574Class::toggleBit(byte pinNumber)
{
  _pinNumber = pinNumber;
    
  byte statusBit = readBackBit(_pinNumber);
  if (statusBit == 0) writeBitHigh(_pinNumber);
  else writeBitLow(_pinNumber);
}

//***************************************************************
//Lesen eines Byte

byte LasPCF8574Class::readByte()
{ 
  //Pins, die als Eingaenge verwendet werden, muessen vor
  //dem Einlesen auf logisch 1 gesetzt werden!
  //(Aktivierung der internen Pullup-Widerstaende!)
  writeByte(0xFF);
  
  Wire.requestFrom(int(_i2cAdd), 1);
  while(Wire.available() == 0);
  byte wert = Wire.read();
  return(wert);
}

//***************************************************************
//Lesen eines einzelnen Bit

byte LasPCF8574Class::readBit(byte pinNumber)
{
  _pinNumber = pinNumber;
  byte bitMask = (0x01 << _pinNumber);

  //Pins, die als Eingaenge verwendet werden, muessen vor
  //dem Einlesen auf logisch 1 gesetzt werden!
  //(Aktivierung der internen Pullup-Widerstaende!)
  writeBitHigh(_pinNumber);
  
  Wire.requestFrom(int(_i2cAdd), 1);
  while(Wire.available() == 0);
  byte wert = Wire.read();
  wert = ((wert & bitMask) >> _pinNumber);
  return(wert);
}
