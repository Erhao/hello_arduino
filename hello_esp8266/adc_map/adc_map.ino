void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc_val = analogRead(A0);
  int mapped_adc_val = map(adc_val, 0, 1023, 0, 255);
  Serial.write(mapped_adc_val);
  delay(150);
}
