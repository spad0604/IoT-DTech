void setup() {
  // put your setup code here, to run once:
  pinMode(17, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(17, HIGH);
  delay(5000);
  digitalWrite(17, LOW);
  delay(5000);
}
