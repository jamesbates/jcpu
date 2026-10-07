#define CLK 2
#define _IOW 12
#define _IOE 3
#define ALIVE 13

#include "pgm.h"


unsigned int pointer = 0;

inline void setDataInput() {

    // Set pins 4-7 to input: the MSB nibble
    PORTD &= 0b00001111; 
    DDRD &= 0b00001111;

    // Set pins 8-11 to input: the LSB nibble
    PORTB &= 0b11110000; 
    DDRB &= 0b11110000;
}

inline void setDataOutput(byte databyte) {

    // Set pins 4-7 to output: the MSB nibble
    DDRD |= 0b11110000;
    PORTD = (databyte & 0xF0) | (PORTD & 0x0F);

    // Set pins 8-11 to output: the LSB nibble
    DDRB |= 0b00001111;
    PORTB = (databyte & 0x0F) | (PORTB & 0xF0);
}

void setup() {

    pgm[0]=(byte) (sizeof(pgm)/2); // integer division will automatically strip off the extra length byte.

    pinMode(CLK, INPUT);
    pinMode(_IOW, INPUT);
    pinMode(_IOE, INPUT);
    pinMode(ALIVE, OUTPUT);
    digitalWrite(13,HIGH);
    
    setDataInput();

    //Serial.begin(115200);

    attachInterrupt(digitalPinToInterrupt(_IOE), handleIOEnable, CHANGE);
}

void handleIOEnable() {

    if (digitalRead(_IOE) == LOW) {
        if (pointer < sizeof(pgm)) {
      
            setDataOutput(pgm[pointer++]);
            //Serial.println("tx");
        }
    } else {
    
        setDataInput();
    }
}


void loop() {

}
