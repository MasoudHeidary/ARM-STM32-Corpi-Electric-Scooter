#include <SoftwareSerial.h>

#define RX 2
#define TX 3

SoftwareSerial Corpi( RX , TX );

///////////////////////////////////////////////////////////

#define KeyFree 0
#define KeyLock 1
#define KeyRese 2
#define KeyFire 3

//Lock
#define LockStateFree 0
#define LockStateLock 1
#define LockStateRese 2

#define FrontLedOn 1
#define FrontLedOff 0
#define FrontLedFire 2

#define TXHeader '$'
#define BrakeTakeVal 1
#define BrakeFreeVal 0

#define ReceiveHeader '$'
#define ReceiveBufferLength 2

uint8_t res[3];
uint8_t counter = 0;

////////////////////////////////////////////////////////////

void RecieveRefresh( void )
{
  if( res[0] != ReceiveHeader )
  return;

  Serial.print( F("GAS:\t") );
  Serial.println( res[1] );

  
  uint8_t Brake = (res[2]>>6) & 0b00000011;
  if( Brake == BrakeTakeVal )
    Serial.println( F("BrakeTakeVal") );
  else if ( Brake == BrakeFreeVal )
    Serial.println( F("BrakeFreeVal") );


  uint8_t Key = (res[2]>>4) & 0b00000011;
  switch ( Key )
  {
    case KeyFree:
      Serial.println( F("key free") );
      break;
    case KeyLock:
      Serial.println( F("Key Lock") );
      break;
    case KeyRese:
      Serial.println( F("key reserver") );
      break;
    case KeyFire:
      Serial.println( F("Key Fire") );
      break;
  }
}
////////////////////////////////////////////////////////////

//a Bit Read in uint8_t
uint8_t BitRead(uint8_t X, uint8_t number) {
  return (X >> number) & (0X01);
}

//a Bit Write in uint8_t
void BitWrite(uint8_t *X, uint8_t number, uint8_t Value) {
  if (BitRead(*X, number) == Value)
    return;

  if (Value == 1)
    *X += 1 << number;
  else if (Value == 0)
    *X -= 1 << number;
}

//Write any number in a part of a number
void BitNumber(uint8_t *X, uint8_t number, uint8_t Value, uint8_t ValueLength) {
  for (uint8_t i = 0; i < ValueLength; i++) {
    BitWrite(X, number + i, BitRead(Value, i));
  }
}

void Send( uint8_t Lock , uint8_t Led )
{

  uint8_t X = 0;
  BitNumber( &X , 6 , Lock , 2 );
  BitNumber( &X , 4 , Led , 2 );
  
  Corpi.write( (char)TXHeader );
  delay(10);
  Corpi.write( (char)X );
}

//////////////////////////////////////////////////////////////

//uint8_t Sen[] = { '$' , 

void setup()
{
  Serial.begin( 115200 );
  Corpi.begin( 9600 );

  Serial.println( "Start" );

//  delay ( 5000 );
//  Send( LockStateFree , FrontLedFire );

//  delay ( 5000 );
//  Send( LockStateFree , FrontLedOn );
//
//  delay (5000 );
//  Send( LockStateFree , FrontLedOff );
  
}

uint32_t TXTime = millis();
bool A = true;

int cou = 0;

void loop()
{
  while( Corpi.available() )
  {
//    uint8_t X = Corpi.read();
//    Serial.print( X );
//    Serial.print( "\t:\t" );
//    Serial.println( X , BIN );
    if( Corpi.read() == '$' )
    {
      res[0] = '$';
      delay(50);
      res[1] = Corpi.read();
      delay(50);
      res[2] = Corpi.read();
      RecieveRefresh();
    }
  }
//if( millis() - TXTime >= 1000 )
//{
//  TXTime = millis();
//  Send( LockStateFree , FrontLedOn );
//  Serial.println( "send" );
//}

  if ( A == true )
  if ( millis() - TXTime > 5000 )
  {
    switch( cou )
    {
      case 0:
      Send( LockStateFree , FrontLedOff );
      break;
      case 1:
      Send( LockStateLock , FrontLedFire );
      break;
      case 2:
      Send( LockStateLock , FrontLedOff );
      break;
    }
    
    cou ++;
//    if( cou == 3 )
//    cou = 0;
    if( cou == 1 )
      A = false;
    
    Serial.println( "send" );
    TXTime = millis();
  }
}
