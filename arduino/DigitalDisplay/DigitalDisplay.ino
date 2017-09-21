#include <MyEEPROM.h>
#include <stdint.h>

#define DUMP 1

#define DP_HOR_TOP (1 << 5)
#define DP_HOR_MID (1 << 7)
#define DP_HOR_BOTTOM (1 << 0)
#define DP_VER_LTOP (1 << 6)
#define DP_VER_LBOTTOM (1 << 3)
#define DP_VER_RTOP (1 << 4)
#define DP_VER_RBOTTOM (1 << 1)
#define DP_POINT (1 << 2)

uint8_t digits[] = {
  /* 0 */ DP_HOR_TOP | DP_HOR_BOTTOM | DP_VER_LTOP | DP_VER_RTOP | DP_VER_LBOTTOM | DP_VER_RBOTTOM,
  /* 1 */ DP_VER_RTOP | DP_VER_RBOTTOM,
  /* 2 */ DP_HOR_TOP | DP_VER_RTOP | DP_HOR_MID | DP_VER_LBOTTOM | DP_HOR_BOTTOM,
  /* 3 */ DP_HOR_TOP | DP_VER_RTOP | DP_HOR_MID | DP_VER_RBOTTOM | DP_HOR_BOTTOM,
  /* 4 */ DP_VER_LTOP | DP_VER_RTOP | DP_HOR_MID | DP_VER_RBOTTOM,
  /* 5 */ DP_HOR_TOP | DP_VER_LTOP | DP_HOR_MID | DP_VER_RBOTTOM | DP_HOR_BOTTOM,
  /* 6 */ DP_HOR_TOP | DP_VER_LTOP | DP_HOR_MID | DP_VER_LBOTTOM | DP_VER_RBOTTOM | DP_HOR_BOTTOM,
  /* 7 */ DP_HOR_TOP | DP_VER_RTOP | DP_VER_RBOTTOM,
  /* 8 */ DP_HOR_TOP | DP_HOR_MID | DP_HOR_BOTTOM | DP_VER_LTOP | DP_VER_RTOP | DP_VER_LBOTTOM | DP_VER_RBOTTOM,
  /* 9 */ DP_HOR_TOP | DP_HOR_MID | DP_HOR_BOTTOM | DP_VER_LTOP | DP_VER_RTOP | DP_VER_RBOTTOM
};

void write_display_digits(bool issigned) {

  int16_t min_val = issigned ? -128 : 0;
  int16_t max_val = issigned ? 127 : 255;

  uint16_t start_addr = issigned ? 0x400 : 0;

  Serial.print("Writing "); Serial.print(issigned ? "signed" : "unsigned"); Serial.print(" ones ");
  for (int16_t value = min_val; value <= max_val; value++) {

    MyEEPROM.write_eeprom(start_addr + ((value < 0) ? 256 : 0) + value, digits[abs(value) % 10]);
    if ((value % 10) == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done.");
  Serial.print("Writing "); Serial.print(issigned ? "signed" : "unsigned"); Serial.print(" 10s ");
  for (int16_t value = min_val; value <= max_val; value++) {

    if (abs(value) < 10) {
      
      MyEEPROM.write_eeprom(start_addr + ((value < 0) ? 256 : 0) + value + 0x100, 0);
    } else {

      MyEEPROM.write_eeprom(start_addr + ((value < 0) ? 256 : 0) + value + 0x100, digits[(abs(value) / 10) % 10]);
    }
    if ((value % 10) == 0) {
       Serial.print(".");
    }
  }
  Serial.println(" done.");
  Serial.print("Writing "); Serial.print(issigned ? "signed" : "unsigned"); Serial.print(" 100s ");
  for (int16_t value=min_val; value <= max_val; value++) {

    if (abs(value) < 100) {
      
      MyEEPROM.write_eeprom(start_addr + ((value < 0) ? 256 : 0) + value + 0x200, 0);
    } else {

      MyEEPROM.write_eeprom(start_addr + ((value < 0) ? 256 : 0) + value + 0x200, digits[(abs(value) / 100) % 10]);
    }
    if ((value % 10) == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done.");
  Serial.print("Writing "); Serial.print(issigned ? "signed" : "unsigned"); Serial.print(" 1000s ");
  for (int16_t value=min_val; value <= max_val; value++) {

    MyEEPROM.write_eeprom(start_addr + ((value < 0) ? 256 : 0) + value + 0x300, (issigned && (value < 0)) ? DP_HOR_MID : 0);
    if ((value % 10) == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done.");
}



// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.print("About to program EEPROM for 7-digit display. Press 'y' to continue ... ");
  while (Serial.available() <= 0);
  char answer = Serial.read();
  Serial.println(answer);
  
  if (answer == 'y') {
  
    MyEEPROM.init();
  
    write_display_digits(false);
    write_display_digits(true);
    
    #ifdef DUMP
    MyEEPROM.dump_eeprom(0, 0x7FF);
    #endif
  }
  Serial.println("Have a nice day.");
}

// the loop function runs over and over again forever
void loop() {
}
