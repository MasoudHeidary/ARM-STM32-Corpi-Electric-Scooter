#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

char corpi = 0;
void loop()
{
  mySerial.write('@');
  mySerial.write(corpi);
  delay(1000);
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
}
