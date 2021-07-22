/*  COMPONENTE DEL GRUPPO: GIOVANNI MESSINA 0000890082  */
/*  PROGETTO #1 TRACK_THE_LED_FLY                       */

#define POT A0
#define Ls 6

#define T1 2
#define T2 3
#define T3 4
#define T4 5

#define L1 9 
#define L2 10
#define L3 11
#define L4 12

int currLed, score, t, tStart, tEnd, K;
boolean loseFlag, rightFlag;
unsigned int TMIN;
int before, num, after;

void setup() {
  //red led
  pinMode(Ls, OUTPUT);
  //init pot
  pinMode(POT, INPUT);
  //init leds
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  //init buttons
  pinMode(T1, INPUT);
  pinMode(T2, INPUT);
  pinMode(T3, INPUT);
  pinMode(T4, INPUT);

  K = 2;

  Serial.begin(9600);
 }

void loop() {
//waiting for start
  preStart();
//start the Game
  startGame();
//end game
  gameOver();
}



void preStart(){
  //init game var
  loseFlag=false;
  rightFlag = false;
  num = 0;
  score = 0;
  Serial.println("\nWelcome to the Track to Led Fly Game. Press Key T1 to Start");
  //FadeIn and FadeOut
  int currIntensity = 0;
  int fadeAmount = 5;
  while(!digitalRead(T1)){
    analogWrite(Ls, currIntensity);
    currIntensity += fadeAmount;
    if(currIntensity == 0 || currIntensity == 255){
      fadeAmount = -fadeAmount;
    }
    delay(30);
  }
  digitalWrite(Ls, LOW);
  TMIN = 1280 / (analogRead(POT)+1) * 500; //Lv.1 64000ms, Lv.8 625 ms
  Serial.println("\nGO!");
  delay(300);
}



/*Map number of led(1..4) to number of pin(L1..L4) */
int getLed(int num){
  switch(num){
  case 1: return L1;
  case 2: return L2;
  case 3: return L3;
  case 4: return L4;
  }
}
 
void chooseLed(){
  if(score == 0){
    num = random(1, 4 + 1);
     } else{
    num = random(0,2) == 0 ? before : after; 
  }
  currLed = getLed(num);
    if(num == 1){
      before = 4;
      after = 2;
    } else if(num == 4){
      before = 3;
      after = 1;
    } else{
      before = num - 1;
      after = num + 1;
    }
}

void check(){
  //decrement timer
  timer();
  //if pressed right
  if(num == 1 && digitalRead(T1) == HIGH || num == 2 && digitalRead(T2) == HIGH
  || num == 3 && digitalRead(T3) == HIGH || num == 4 && digitalRead(T4) == HIGH){
    if(!rightFlag){
     rightFlag = true;
      digitalWrite(getLed(num),LOW);
      t = 0;
    }
 
  }
  // if pressed wrong
  else if( t == 0 || (num != 1 && digitalRead(T1) == HIGH) || (num != 2 && digitalRead(T2) == HIGH)
  || (num != 3 && digitalRead(T3) == HIGH) || (num != 4 && digitalRead(T4) == HIGH)){
    if(!loseFlag && !rightFlag){
    loseFlag = true;
    }
  }
 
}


void timer(){
  tEnd = millis() - tStart;
  
if(tEnd > t){
  t = 0;
}

}


void startGame(){
  while(!loseFlag){
  t = random(TMIN, (K*TMIN)); 
  tStart = millis();
  chooseLed();
  digitalWrite(currLed, HIGH); 
      
  do{
    check();
    if(rightFlag){
      score ++;
      Serial.print("\nTracking the fly: pos ");
      Serial.println(num);
      TMIN = TMIN * 7/8;
     }
  }while(t>0 && !rightFlag && !loseFlag);

     digitalWrite(currLed, LOW);
     rightFlag = false;
     //debouncing
     delay(200);
  }
}

void gameOver(){
  digitalWrite(Ls, HIGH);
  Serial.print("Game Over - Score: ");
  Serial.println(score);
  
  delay(2000);
  digitalWrite(Ls, LOW);
}
