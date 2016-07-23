
#define Funnel_Pin_1 9
#define Funnel_Pin_2 10
#define Funnel_Speed 128
#define Funnel_Up_Key '1'
#define Funnel_Down_Key '2'

#define Shoot_Key 's'
#define Shoot_Speed 200
#define Shoot_Pin_OUT 12
#define Shoot_Pin_GND 11

#define TIMEOUT 50

void FunnelStop()
{
  Serial.println("FSt");
  digitalWrite(Funnel_Pin_1, LOW);
  digitalWrite(Funnel_Pin_2, LOW);
}

void FunnelUp()
{
  Serial.println("FUp");
  digitalWrite(Funnel_Pin_1, HIGH);
  digitalWrite(Funnel_Pin_2, LOW);
}

void FunnelDown()
{
  Serial.println("FDn");
  digitalWrite(Funnel_Pin_1, LOW);
  digitalWrite(Funnel_Pin_2, HIGH);
}


void ShootStart()
{
  Serial.println("SSt");
  digitalWrite(Shoot_Pin_OUT, HIGH);
  digitalWrite(Shoot_Pin_GND, LOW);
}

void ShootStop()
{
  Serial.print("STo");
  digitalWrite(Shoot_Pin_OUT, LOW);
  digitalWrite(Shoot_Pin_GND, LOW);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(Funnel_Pin_1, OUTPUT);
  pinMode(Funnel_Pin_2, OUTPUT);
  pinMode(Shoot_Pin_OUT, OUTPUT);
  pinMode(Shoot_Pin_GND, OUTPUT);
}
char ch;
int currentT, missionFU, missionFD, missionST;

void loop() {
  ch = 0;
  if(Serial.available())
  {
    ch = Serial.read();
    Serial.println(ch);
    switch(ch)
    {
    case Funnel_Up_Key:
      missionFU = millis();
      FunnelUp();
      break;
    case Funnel_Down_Key:
      missionFD = millis();
      FunnelDown();
      break;
    case Shoot_Key:
      missionST = millis();
      ShootStart();
      break;
    }
  }
  currentT = millis();
  if(missionFU && currentT - missionFU > TIMEOUT)
  {
    FunnelStop();
    missionFU = 0;
  }
  if(missionFD && currentT - missionFD > TIMEOUT)
  {
    FunnelStop();
    missionFD = 0;
  }
  if(missionST && currentT - missionST > TIMEOUT)
  {
    ShootStop();
    missionST = 0;
  }
  
}
