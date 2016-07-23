#include <Servo.h> 
#include <stdint.h>
typedef struct
{
  int16_t roll;
  int16_t pitch;
  int16_t yaw;
  int16_t throttle;
  int16_t mode;
  int16_t gear;
} RCData;
uint8_t buffer[64];
char strBuf[64];
int readPos = 0;
RCData *pRc;
Servo Cab;
int CabStatus = 0;

void closeCab()
{
  if(CabStatus == 0)
  {
    return ;
  }
  Cab.write(90);
  Serial.print("Close.\n");
  CabStatus = 0;
}

void openCab()
{
  if(CabStatus == 1)
  {
    return ;
  }
  Cab.write(180);
  Serial.print("Open.\n");
  CabStatus = 1;
}

void dataHandler()
{
  if(buffer[0] != 0xAA 
      || buffer[12] != 0x02 || buffer[13] != 0x00
      || buffer[14] != 0x80 || buffer[15] != 0x00 )
  {
    return ; 
  }
  pRc = (RCData *)(buffer + 16);
  if(pRc->gear == -10000)
  {
    openCab();
  }
  else
  {
    closeCab();
  }
  return ;
}
void setup() {
    Serial.begin(115200);
    Cab.attach(9);
    readPos = 0;
    Serial.println("Finish Init.");
}

void loop() {
  int more;
  int offset;
  int i;
  readPos = 0;
  uint8_t ch;
  int count = 0;
  while(readPos < 32)
  {
    while(Serial.available())
    {
      ch = Serial.read();
      buffer[readPos] = ch;
      ++readPos;
    }
  }
  offset=0;
  while(buffer[offset] != 0xAA && buffer[offset + 1] != 0x20 ) ++offset;
  more = offset;
  while(more)
  {
    while(Serial.available())
    {
      ch = Serial.read();
      buffer[readPos] = ch;
      ++readPos;
      --more;
    }
  }
 dataHandler();

}