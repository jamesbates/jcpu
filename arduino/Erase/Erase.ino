#include <MyEEPROM.h>
#include <stdint.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("About to erase EEPROM. Press 'y' to contine ... ");
  while (Serial.available() <= 0);
  char answer = Serial.read();
  Serial.println(answer);
  
  if (answer == 'y') {
    
    MyEEPROM.init();
  
    Serial.print("Erasing ");
    for (uint16_t add=0; add < 0x2000; add++) {
  
      MyEEPROM.write_eeprom(add, 0xFF);
  
      if ((add % 0x100) ==0) {
  
        Serial.print(".");
      }
      
    }
    Serial.println("done.");
  
    MyEEPROM.dump_eeprom(0, 0x1FFF);
  }
  Serial.println("Have a nice day.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
