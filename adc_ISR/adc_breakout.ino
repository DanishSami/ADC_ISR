/*
   Thank for original source code from http://www.glennsweeney.com/tutorials/interrupt-driven-analog-conversion-with-an-atmega328p
*/
byte readFlag = 0;
int analogVal = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


  // clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  ADMUX &= B11011111;
  
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  ADMUX |= B01000000;
  
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
  // input
  ADMUX &= B11110000;
  
  ADMUX |= 0;
  
  
  // Set ADEN in ADCSRA (0x7A) to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
  ADCSRA |= B10000000;
  
  // Set ADATE in ADCSRA (0x7A) to enable auto-triggering.
  ADCSRA |= B00100000;
  
  // Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
  // This means that as soon as an ADC has finished, the next will be
  // immediately started.
  ADCSRB &= B11111000;
  
  // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  ADCSRA |= B00000111;
  
  // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  ADCSRA |= B00001000;

  sei();
  
  readFlag = 0;
 
  ADCSRA |=B01000000;

}
unsigned long timer;
unsigned long duration;
void loop() {
  // put your main code here, to run repeatedly:
  /* max 116 us*/
  /*
    timer = micros();
    int val = analogRead(A0);
    Serial.println(micros() - timer);
  */
  
  if (readFlag == 1) {
    Serial.print(duration);

    int val = analogVal;
    Serial.print(" : ");
    Serial.println(val);
    readFlag = 0;



  }
  
}
ISR(ADC_vect) {
  duration = micros() - timer;
  timer = micros();
  readFlag = 1;
  analogVal = ADCL | (ADCH << 8);//alway read Low byte first!!


}
