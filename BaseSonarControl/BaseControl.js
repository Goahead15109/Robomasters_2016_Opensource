var Sonar  = require("HC-SR04");
var SonarLValue = 100;
var SonarRValue = 100;
var SonarFValue = 100;
var SonarEValue = 100;
var LastSonarLValue = 0;
var LastSonarRValue = 0;
var LastSonarFValue = 0;
var LastSonarEValue = 0;
var LCnt = 0;
var RCnt = 0;
var FCnt = 0;
var ECnt = 0;

var led = 0;
var MinDest = 50; //接近距离 cm
var SpeedMax = 300; //摇杆增量
var SpeedDelay = 100; //接近减缓
var SpeedDuplex = 2.5;

var DBus = function() {};
DBus.prototype.rc = {
  ch0: 1024,
  ch1: 1024,
  ch2: 1024,
  ch3: 1024,
  s1: 1,
  s2: 1
};
DBus.prototype.mouse = {
  x: 0,
  y: 0,
  z: 0,
  lbutton: 0,
  rbutton: 0
};
DBus.prototype.keyboard = {
  value: 0,
  KEY_W: 0b1,
  KEY_S: 0b10,
  KEY_A: 0b100,
  KEY_D     : 0b1000,
  KEY_Q     : 0b10000,
  KEY_E     : 0b100000,
  KEY_SHIFT : 0b1000000,
  KEY_CTRL  : 0b10000000
};

DBus.prototype.encode = function(){
  var pData = [];
  pData[0] = this.rc.ch0 & 0xFF;
  pData[1] = ((this.rc.ch0 >> 8) & 0x07) + ((this.rc.ch1 << 3) & 0xF8);
  pData[2] = ((this.rc.ch1 >> 5) & 0x3F) + ((this.rc.ch2 << 6) & 0xC0);
  pData[3] = (this.rc.ch2 >> 2) & 0xFF;
  pData[4] = ((this.rc.ch2 >> 10) & 0x01) + ((this.rc.ch3 << 1) & 0xFE);
  pData[5] = ((this.rc.ch3 >> 7) & 0x0F) + ((this.rc.s1 << 6) & 0xC0) + ((this.rc.s2 << 4) & 0x30);
  pData[6] = this.mouse.x & 0xFF;
  pData[7] = (this.mouse.x >> 8) & 0xFF;
  pData[8] = this.mouse.y & 0xFF;
  pData[9] = (this.mouse.y >> 8) & 0xFF;
  pData[10] = this.mouse.z & 0xFF;
  pData[11] = (this.mouse.z >> 8) & 0xFF;
  pData[12] = this.mouse.lbutton;
  pData[13] = this.mouse.rbutton;
  pData[14] = this.keyboard.value;
  pData[15] = 0;
  pData[16] = 0;
  pData[17] = 0;
  return pData;
};

DBus.prototype.reset = function() {
  this.rc.ch0 = 1024;
  this.rc.ch1 = 1024;
  this.rc.ch2 = 1024;
  this.rc.ch3 = 1024;
  this.rc.s1 = 1;
  this.rc.s2 = 1;
  this.mouse.x = 0;
  this.mouse.y = 0;
  this.mouse.z = 0;
  this.mouse.lbutton = 0;
  this.mouse.rbutton = 0;
  this.keyboard.value = 0;
};

dbusFrame = new DBus();

Serial1.setup(100000, {
  rx: B7,
  tx: B6,
  bytesize: 8,
  parity: 'e',
  stopbits: 1
});


setInterval(function(){
  var c = dbusFrame.encode();
  Serial1.write(c);
}, 7);

var SonarL = Sonar.connect(A0, B10, function(dist) {
  if(dist > 450)
    return;
  SonarLValue = dist;
  //console.log(SonarLValue + "L");
});

var SonarR = Sonar.connect(A1, B4, function(dist) {
  if(dist > 450)
    return;
  SonarRValue = dist;
  //console.log(SonarRValue + "R");
});

var SonarF = Sonar.connect(A4, B5, function(dist) {
  if(dist > 450)
    return;
  SonarFValue = dist;
  //console.log(SonarFValue + "F");
});

var SonarE = Sonar.connect(B0, B3, function(dist) {
  if(dist > 450)
    return;
  SonarEValue = dist;
  //console.log(SonarEValue + "B");
});

setInterval(function(){
  SonarL.trigger();
  SonarR.trigger();
  SonarF.trigger();
  SonarE.trigger();
}, 20);

var NextPos = 1; // 0 1 2 3 //LF  LB  RB  RF
var PrevPos = 0;

function GoBack() {
  if(SonarEValue == LastSonarEValue) {
    return ;
  }
  LastSonarEValue = SonarEValue;
  dbusFrame.rc.ch0 = 1024;
  if(SonarEValue > MinDest) {
    dbusFrame.rc.ch1 = 1024 - (SonarEValue > SpeedMax ? SpeedMax :Math.floor(SonarEValue * SpeedDuplex));
    ECnt = 0;
  } else {
    dbusFrame.rc.ch1 = 1024 - SpeedDelay;
    ++ECnt;
  }
  if(ECnt > 5) {
    ECnt = 0;
    DealNextPosition();
  }
}

function GoFront() {
  if(SonarFValue == LastSonarFValue) {
    return;
  }
  LastSonarFValue = SonarFValue;
  dbusFrame.rc.ch0 = 1024;
  if(SonarFValue > MinDest) {
    dbusFrame.rc.ch1 = 1024 + (SonarFValue > SpeedMax ? SpeedMax :Math.floor(SonarFValue * SpeedDuplex));
    FCnt = 0;
  } else {
    dbusFrame.rc.ch1 = 1024 + SpeedDelay;
    ++FCnt;
  }
  if(FCnt > 5) {
    FCnt = 0;
    DealNextPosition();
  }
}
function GoRight() {
  if(SonarRValue == LastSonarRValue) {
    return;
  }
  LastSonarRValue = SonarRValue;
  dbusFrame.rc.ch1 = 1024;
  if(SonarRValue > MinDest) {
    dbusFrame.rc.ch0 = 1024 + (SonarRValue > SpeedMax ? SpeedMax :Math.floor(SonarEValue* SpeedDuplex));
    RCnt = 0;
  } else {
    dbusFrame.rc.ch0 = 1024 + SpeedDelay;
    ++RCnt;
  }
  if(RCnt > 5) {
    RCnt = 0;
    DealNextPosition();
  }
}
function GoLeft() {
  if(SonarLValue === LastSonarLValue) {
    return;
  }
  LastSonarLValue = SonarLValue;
  dbusFrame.rc.ch1 = 1024;
  if(SonarLValue > MinDest) {
    dbusFrame.rc.ch0 = 1024 - (SonarLValue > SpeedMax ? SpeedMax :Math.floor(SonarEValue * SpeedDuplex));
    LCnt = 0;
  } else {
    dbusFrame.rc.ch0 = 1024 - SpeedDelay;
    ++LCnt;
  }
  if(LCnt > 5) {
    LCnt = 0;
    DealNextPosition();
  }
}

function Stop() {
  dbusFrame.reset();
}


function DealNextPosition() {
  direction = Math.floor(Math.random()*10 % 2) == 1 ? 1 : -1;
  PrevPos = NextPos;
  NextPos += direction;
  NextPos %= 4;
}

var going = false;
var GoCnt = 0;
function DealGoing() {
  if(going) {
    return;
  }
  going = true;
  if(PrevPos === 0 && NextPos === 1) {
    GoBack();
  }
  else if(PrevPos === 1 && NextPos === 2) {
    GoRight();
  }
  else if(PrevPos === 2 && NextPos === 3) {
    GoFront();
  }
  else if(PrevPos === 3 && NextPos === 0) {
    GoLeft();
  }
  else if(PrevPos === 0 && NextPos === 3) {
    GoRight();
  }
  else if(PrevPos === 3 && NextPos === 2) {
    GoBack();
  }
  else if(PrevPos === 2 && NextPos === 1) {
    GoLeft();
  }
  else if(PrevPos === 1 && NextPos === 0) {
    GoFront();
  }
  else {
    PrevPos = 0;
    NextPos = 1;
    Stop();
  }
  ++GoCnt;
  if(GoCnt % 50 === 0){
    led = !led;
    digitalWrite(LED, led);
  }
  going = false;
}

setInterval(DealGoing, 5);

