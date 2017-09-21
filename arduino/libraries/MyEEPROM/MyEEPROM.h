#ifndef _MYEEPROM_H
#define _MYEEPROM_H

extern class MyEEPROM {
public:
  void init();
  static unsigned char read_eeprom(unsigned int address);
  void write_eeprom(unsigned int address, unsigned char value);
  void dump_eeprom(unsigned int start_address, unsigned int end_address);
} MyEEPROM;

#endif
