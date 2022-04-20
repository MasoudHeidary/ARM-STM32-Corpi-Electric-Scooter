#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("TEST Branch for CORPI");
  Serial.println("0 -> LOCK: Lock");
  Serial.println("1 -> LOCK: Free");
  Serial.println("2 -> LOCK: Find");
  Serial.println("7 -> FRONT LED: Off");
  Serial.println("8 -> FRONT LED: On");
  Serial.println("9 -> FRONT LED: Toggle");
}

char corpi = 0;
void loop()
{
  while(mySerial.available()){
    unsigned char d = mySerial.read();

    Serial.print((char)d);
    Serial.print("\t::\t");
    Serial.println((int)d);
  }
}

void serialEvent() {
  char d = Serial.read();
      
    if(d == '0') {
      bitWrite(corpi, 0, 0);
      bitWrite(corpi, 1, 0);
      Serial.println("Lock: LOCKED");
    } 
    else if (d == '1') {
      bitWrite(corpi, 0, 1);
      bitWrite(corpi, 1, 0);
      Serial.println("Lock: Free");
    } 
    else if (d == '2') {
      bitWrite(corpi, 0, 0);
      bitWrite(corpi, 1, 1);
      Serial.println("Lock: Find");
    } 
    else if (d == '7') {
      bitWrite(corpi, 2, 0);
      bitWrite(corpi, 3, 0);
      Serial.println("Front Led: off");
    } 
    else if (d == '8') {
      bitWrite(corpi, 2, 1);
      bitWrite(corpi, 3, 0);
      Serial.println("Front Led: on");
    } 
    else if (d == '9') {
      bitWrite(corpi, 2, 0);
      bitWrite(corpi, 3, 1);
      Serial.println("Front Led: toggle");
    }

    mySerial.write('@');
    mySerial.write(corpi);
}
