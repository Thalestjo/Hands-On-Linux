#define LDR_PIN  32

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.println(ldrValue);
  delay(500);
}
