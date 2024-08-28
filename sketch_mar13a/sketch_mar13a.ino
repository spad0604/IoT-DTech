#define moto2 27
#define moto1 16
void setup() {
  // put your setup code here, to run once:
  pinMode(moto2, OUTPUT);
  pinMode(moto1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(moto1, HIGH);
    digitalWrite(moto2, HIGH);
}
