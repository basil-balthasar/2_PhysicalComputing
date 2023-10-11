#include <EEPROM.h>

byte ledState = LOW;
bool buttonReleased = true;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  ledState = EEPROM.read(0);
  digitalWrite(LED_BUILTIN, ledState);
}

void loop() {
  if(digitalRead(2) == LOW && buttonReleased == true){
    ledState = !ledState;
    EEPROM.write(0, ledState);
    digitalWrite(LED_BUILTIN, ledState);
    buttonReleased = false;
    delay(5);
  }else if(digitalRead(2) == HIGH){
    buttonReleased = true;
    delay(5);
  }
}
