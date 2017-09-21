#include "MyEEPROM.h"
#include "Arduino.h"
#include <stdint.h>

#define PIN_SHCLK 3
#define PIN_STCLK 4
#define PIN_DS 2

#define PIN_IO0 5
#define PIN_IO1 6
#define PIN_IO2 7
#define PIN_IO3 8
#define PIN_IO4 9
#define PIN_IO5 10
#define PIN_IO6 11
#define PIN_IO7 12

#define PIN_WE 13

#define MODE_READ 0x0000
#define MODE_WRITE 0x8000

static void pulse(int pin) {

  digitalWrite(pin, HIGH);
  delay(1);
  digitalWrite(pin, LOW);
  delay(1);
}

static void shift_out2(uint16_t value) {

  digitalWrite(PIN_SHCLK, LOW);
  digitalWrite(PIN_STCLK, LOW);
  shiftOut(PIN_DS, PIN_SHCLK, LSBFIRST, value & 0xFF);
  shiftOut(PIN_DS, PIN_SHCLK, LSBFIRST, (value >> 8) & 0xFF);
  pulse(PIN_STCLK);
}

static void set_iopin_mode(int pin_mode) {

  for (int pin = PIN_IO0; pin <= PIN_IO7; pin++) {

    pinMode(pin, pin_mode);
  }
}

static void select_address(uint16_t address, uint16_t mode) {

  if (mode == MODE_READ) {
    set_iopin_mode(INPUT);
  }
  shift_out2(address | mode);
  if (mode == MODE_WRITE) {

    set_iopin_mode(OUTPUT);
  }
}

uint8_t MyEEPROM::read_eeprom(uint16_t address) {

  select_address(address, MODE_READ);
  delay(2);
  uint8_t result = 0;
  for (int io_bit=0; io_bit < 8; io_bit++) {

    result |= ((digitalRead(PIN_IO0 + io_bit) == HIGH) ? (1 << io_bit) : 0);
  }

  return result;
}

void MyEEPROM::write_eeprom(uint16_t address, uint8_t value) {

  select_address(address, MODE_WRITE);
  for (int io_bit=0; io_bit < 8; io_bit++) {

    digitalWrite(PIN_IO0 + io_bit, value & 1 ? HIGH : LOW);
    value >>=1;
  }
  delay(15);
  digitalWrite(PIN_WE, LOW);
  delay(1);
  digitalWrite(PIN_WE, HIGH);
  delay(15);
}

void MyEEPROM::init() {
  pinMode(PIN_WE, OUTPUT);
  digitalWrite(PIN_WE, HIGH);
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_SHCLK, OUTPUT);
  pinMode(PIN_STCLK, OUTPUT);
}

void MyEEPROM::dump_eeprom(uint16_t start_address, uint16_t end_address) {

  char output_buffer[80];

  start_address &= 0xFFF0;
  end_address |= 0xF;

  for (uint16_t address = start_address; address < end_address; address+=0x10) {

    sprintf(output_buffer, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
        address,
	read_eeprom(address),
	read_eeprom(address + 1),
	read_eeprom(address + 2),
	read_eeprom(address + 3),
	read_eeprom(address + 4),
	read_eeprom(address + 5),
	read_eeprom(address + 6),
	read_eeprom(address + 7),
	read_eeprom(address + 8),
	read_eeprom(address + 9),
	read_eeprom(address + 10),
	read_eeprom(address + 11),
	read_eeprom(address + 12),
	read_eeprom(address + 13),
	read_eeprom(address + 14),
	read_eeprom(address + 15));
    Serial.println(output_buffer);
  }
}

class MyEEPROM MyEEPROM;
