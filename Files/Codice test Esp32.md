int LED_PIN = 2;
void setup() {
 pinMode(LED_PIN, OUTPUT);
 Serial.begin(9600);
}
void loop() {
 Serial.println("loop");
 digitalWrite(2, HIGH);
 delay(1000);
 digitalWrite(2, LOW);
}