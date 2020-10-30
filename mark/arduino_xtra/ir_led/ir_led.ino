#define IR 21

void IRCarrier(uint16_t IRtimemicroseconds)
{
  for(uint16_t i = 0; i < (IRtimemicroseconds / 26); i++)
  {
    digitalWrite(IR, HIGH);   //turn on the IR LED
    //NOTE: digitalWrite takes about 8us to execute, so we need to factor that into the timing.
    delayMicroseconds(12);     //delay for 13us (9us + digitalWrite), half the carrier frequency
    digitalWrite(IR, LOW);    //turn off the IR LED
    delayMicroseconds(12);     //delay for 13us (9us + digitalWrite), half the carrier frequency
  }
}

void sendIRMessage(uint16_t code) {
  for(int i = 0; i < 16; i++) {
    if(code & (1 << ((sizeof(uint16_t) * 8) - 1))) {
      IRCarrier(1600);
      delayMicroseconds(800);
    }
    else {
      IRCarrier(800);
      delayMicroseconds(1600);
    }
    code <<= 1;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(IR, OUTPUT);
  
  uint16_t code = 0b1010101010101010;
  
  sendIRMessage(code);
  delay(3);
  sendIRMessage(code);
}

void loop() {
  // do nothing
}
