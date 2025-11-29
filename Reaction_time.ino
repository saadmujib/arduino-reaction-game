#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C scrn(0x27,16,2);

int b1 = 2;
int b2 = 4;

int gLed = 11;
int rLed = 12;
int buzz = 8;

unsigned long t0 = 0;
bool gOn = false;

int s1 = 0;
int s2 = 0;
int roundNum = 1;

void flashBoth(){
  for(int i=0;i<3;i++){
    digitalWrite(rLed,1);
    digitalWrite(gLed,1);
    delay(150);
    digitalWrite(rLed,0);
    digitalWrite(gLed,0);
    delay(150);
  }
}

void holdToStart(){
  scrn.clear();
  scrn.print("Press both btns");
  scrn.setCursor(0,1);
  scrn.print("to start");

  while( digitalRead(b1)!=LOW || digitalRead(b2)!=LOW ){
    delay(10);
  }
}

void setup(){
  scrn.init();
  scrn.backlight();

  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(gLed, OUTPUT);
  pinMode(rLed, OUTPUT);
  pinMode(buzz, OUTPUT);

  digitalWrite(gLed,0);
  digitalWrite(rLed,0);

  flashBoth();
  holdToStart();
  introScroll();
  delay(500);
}

void introScroll(){
  String t = "   Wikommen to the Reaction Game!   ";
  scrn.clear();
  for(int i=0;i<=t.length()-16;i++){
    scrn.setCursor(0,0);
    scrn.print(t.substring(i,i+16));
    delay(180);
  }
}

void loop(){
  if(roundNum <= 3){
    delay(500);
    playRound();
  } else {
    finalResult();
    endGame();
    waitRestart();
    resetGame();
  }
}

void playRound(){
  scrn.clear();
  scrn.print("Round ");
  scrn.print(roundNum);
  delay(800);

  scrn.clear();
  scrn.print("Wait for");
  scrn.setCursor(0,1);
  scrn.print("Green LED ^-^");

  digitalWrite(rLed,1);
  digitalWrite(gLed,0);
  gOn = false;

  unsigned long w = millis() + 1500 + random(900,2600);

  while(millis() < w){
    if(!digitalRead(b1)){ cheated(2); return; }
    if(!digitalRead(b2)){ cheated(1); return; }
  }

  digitalWrite(rLed,0);
  digitalWrite(gLed,1);
  scrn.clear();
  scrn.print("GO!");

  gOn = true;
  t0 = millis();

  tone(buzz,880);

  while(1){
    if(!digitalRead(b1)){
      noTone(buzz);
      pressed(1);
      return;
    }
    if(!digitalRead(b2)){
      noTone(buzz);
      pressed(2);
      return;
    }
  }
}

void cheated(int winner){
  scrn.clear();
  scrn.print("Player ");
  scrn.print(winner == 1 ? "2" : "1");
  scrn.print(" cheated");

  if(winner==1) s1++;
  else s2++;

  for(int i=0;i<4;i++){
    digitalWrite(rLed,1);
    delay(150);
    digitalWrite(rLed,0);
    delay(150);
  }

  roundNum++;
}

void pressed(int p){
  unsigned long dt = millis() - t0;
  float sec = dt / 1000.0;

  scrn.clear();
  scrn.print("P");
  scrn.print(p);
  scrn.print(": ");
  scrn.print(sec,3);
  scrn.print("s");

  if(p==1) s1++;
  else s2++;

  for(int i=0;i<4;i++){
    digitalWrite(gLed,1);
    delay(120);
    digitalWrite(gLed,0);
    delay(120);
  }

  delay(1100);
  roundNum++;
}

void finalResult(){
  scrn.clear();
  scrn.print("Scores");
  scrn.setCursor(0,1);
  scrn.print("P1:");
  scrn.print(s1);
  scrn.print(" P2:");
  scrn.print(s2);
  delay(2000);

  scrn.clear();
  if(s1 > s2) scrn.print("Player 1 Wins");
  else if(s2 > s1) scrn.print("Player 2 Wins");
  else scrn.print("Draw Game");
  delay(2000);
}

void endGame(){
  scrn.clear();
  scrn.print("Good Bye :)");

  for(int i=0;i<4;i++){
    digitalWrite(rLed,1);
    digitalWrite(gLed,1);
    delay(180);
    digitalWrite(rLed,0);
    digitalWrite(gLed,0);
    delay(180);
  }

  digitalWrite(rLed,0);
  digitalWrite(gLed,0);
  noTone(buzz);
}

void waitRestart(){
  scrn.clear();
  scrn.print("Press both Btns");
  scrn.setCursor(0,1);
  scrn.print("to restart");

  while( digitalRead(b1)!=LOW || digitalRead(b2)!=LOW ){
    delay(10);
  }
}

void resetGame(){
  s1 = 0;
  s2 = 0;
  roundNum = 1;
  flashBoth();
  introScroll();
}
