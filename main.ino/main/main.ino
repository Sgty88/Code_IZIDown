void setup() {
  // put your setup code here, to run once:

serial.begin(9600);
serial.println("Hello, World!");




}

void loop() {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);
  // put your main code here, to run repeatedly:

}
