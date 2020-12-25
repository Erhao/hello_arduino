
void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc_value = analogRead(A0);
  Serial.println(adc_value);
  delay(1000);
}
