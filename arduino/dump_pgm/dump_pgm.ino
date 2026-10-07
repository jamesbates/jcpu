#define CLK 2
#define _IOW 12
#define _IOE 3
#define ALIVE 13

volatile bool data_available = false;
byte data = 0;

void setup() {

    pinMode(CLK, INPUT);
    pinMode(_IOW, INPUT);
    pinMode(_IOE, INPUT);
    pinMode(ALIVE, OUTPUT);

    digitalWrite(ALIVE, HIGH);
    
    // Set pins 4-7 to input: the MSB nibble
    PORTD &= 0b00001111; 
    DDRD &= 0b00001111;

    // Set pins 8-11 to input: the LSB nibble
    PORTB &= 0b11110000; 
    DDRB &= 0b11110000;

    attachInterrupt(digitalPinToInterrupt(CLK), handleClockRising, RISING);
    Serial.begin(115200);    
}

void handleClockRising() {

    if (digitalRead(_IOW) == LOW) {

         data = (PIND & 0xF0) | (PINB & 0x0F);
         data_available = true;
    }
}

void loop() {

    static char buffer[15];

    if (data_available) {
        sprintf(buffer,"0x%02X = %3d",data,data);
        Serial.println(buffer);
        data_available = false;
    }
}
