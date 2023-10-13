#include <Adafruit_VL6180X.h>
#include <MIDIUSB.h>

Adafruit_VL6180X vl = Adafruit_VL6180X();

#define buttonPin 0

bool buttonPressed = false;

uint8_t pressedRange = 0;
uint8_t note = 0;

int16_t bend = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);

  Serial.println("Adafruit VL6180x test!");
  if (!vl.begin()) {
    Serial.println("Failed to find sensor");
    //while (1);
  }
  Serial.println("Sensor found!");
}

void loop() {
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();


  //onPress
  if (status == VL6180X_ERROR_NONE) {
    bend = range - pressedRange;
    bend = map(bend, -190, 190, 0, 16383);
    bend = constrain(bend, 0, 16383);
  }

  // First parameter is the event type (0x09 = note on, 0x08 = note off).
  // Second parameter is note-on/note-off, combined with the channel.
	// Channel can be anything between 0-15. Typically reported to the user as 1-16.
	// Third parameter is the note number (48 = middle C).
	// Fourth parameter is the velocity (64 = normal, 127 = fastest).

  if (digitalRead(buttonPin) == LOW && !buttonPressed) {
    pressedRange = range;
    note = map(range, 0, 190, 0, 127);
    note = constrain(note, 0, 127);
    noteOn(0, note, 64);
    MidiUSB.flush();

    //Serial.println("pressed");
    buttonPressed = true;
    delay(5);
  }
  if (digitalRead(buttonPin) == HIGH && buttonPressed) {
    noteOff(0, note, 64);
    MidiUSB.flush();
    
    //Serial.println("released");
    buttonPressed = false;
    delay(5);
  }

  if(buttonPressed == true){
    pitchBendChange(0, bend);
  }

  // Some error occurred, print it out!
  /*
  if ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  } else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  } else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  } else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  } else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  } else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  } else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  } else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  } else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  */
  delay(50);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void pitchBendChange(byte channel, int value) {
  byte lowValue = value & 0x7F;
  byte highValue = value >> 7;
  midiEventPacket_t bendEvent = {0x0E, 0xE0 | channel, lowValue, highValue};
  MidiUSB.sendMIDI(bendEvent);
}