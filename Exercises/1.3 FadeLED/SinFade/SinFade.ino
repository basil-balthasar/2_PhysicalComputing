void setup() {
  pinMode(3, OUTPUT);
}

void loop() {
  analogWrite(3, (int)((sin(millis()*0.0005)+1)/2*255));
}
