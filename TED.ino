//Til MP3 shield & SD kort
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
SdFat sd;
SFEMP3Shield MP3player;

//PINs
int motionSensor = A2;
int tilt = A1;
int magKnapp = 5;

//Bamsens state
int bearState = 0;
unsigned long previousMillis, currentMillis, careMillis;
int val = 0;
int val2 = 0;

//IDLE
char trackTyveTo[] = "track022.mp3";
char trackTyveFire[] = "track024.mp3";
char trackTyveFem[] = "track025.mp3";

//State 3
char trackEn[] = "track001.mp3";
char trackTo[] = "track002.mp3";
char trackFire[] = "track004.mp3";

//State 4
char trackSyv[] = "track007.mp3";
char trackAtte[] = "track008.mp3";

//Gi kommando
char trackTretten[] = "track013.mp3";
char trackSeksten[] = "track016.mp3";
char trackSytten[] = "track017.mp3";
char trackAtten[] = "track018.mp3";

//Gitt-Trykk-Knapp
char trackTyve[] = "track020.mp3";

//loftmegopp
char trackTyveSeks[]="track026.mp3";
char trackTyveSyv[]="track027.mp3";

void setup(){
  Serial.begin(115200);
  uint8_t result;
  
  //SD kort og starter MP3
  Serial.println("Initialiserer SD kort");  
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  result = MP3player.begin();
  Serial.print("MP3player.begin() ble: ");
  //Hvis 0 eller 6, OK
  Serial.println(result);
  
  previousMillis = millis();
  while(millis() < previousMillis + 5000){

  }
  
  pinMode(motionSensor, INPUT);
  pinMode(tilt, INPUT);
  pinMode(magKnapp, INPUT);
  
  //Setter volum
  MP3player.setVolume(40, 40);
  previousMillis = millis();
}
 
void loop(){
   MP3player.available();

  switch(bearState){
    case 0:
    start();
    break;
    case 1:
    motionSens(); // Idle
    break;
    case 3:
    gyroTilt(); // Lofter opp
    break;
    case 4:
    kommando(); //Gir kommando
    break;
    case 5:
    hjelpKnapp(); 
    break;
    case 666:
    knapp();
    break;
  }
  
}
void start(){
   while (digitalRead(magKnapp) != HIGH){
   Serial.println("Venter på start");
  }
   bearState = 1;
   Serial.println("KNAPP START GO!");
   previousMillis = millis();
   delay(5000);
   spillAv(1);
}

void motionSens(){
  val = 0;
  val2 = 0;
  currentMillis = millis();
  if(previousMillis + 60000 < currentMillis){
   spillAv(1);
   previousMillis = millis();
  } 
  
 if(analogRead(motionSensor) != 0){
  Serial.print("MotionSens: ");
  Serial.println(analogRead(motionSensor));
  bearState = 3;
  MP3player.stopTrack();
  spillAv(3);
  previousMillis = millis();
  
}else{
  Serial.print("MotionSens: ");
  Serial.println(analogRead(motionSensor));
  delay(50);
  } 
}

void gyroTilt(){ 
  currentMillis = millis();
  if(previousMillis + 20000 < currentMillis){
    bearState = 1;
  }

  ventMP3();
  
  if(val == 0){
    delay(2000);
  spillAv(1000); //Kan du lofte meg opp?
  val = 1;
  }
  
  Serial.print("TILT: ");
  Serial.println(analogRead(tilt));
  if(analogRead(tilt) > 100){
   MP3player.stopTrack();
    spillAv(123);

      ventMP3();

    bearState = 4; // EGENTLIG 5
    previousMillis = millis();
  }
  delay(50);

}
void hjelpKnapp(){
  currentMillis = millis();
  if(previousMillis + 20000 < currentMillis){
    bearState = 2;
  }
  if(val2 == 0){
   // Trykk på knappen i hånden min om du kan/vil hjelpe meg
  val2 = 1;
  }
  
  if(digitalRead(magKnapp) != LOW){
    bearState = 4;
    previousMillis = millis();
  }
}

void kommando(){
  currentMillis = millis();
  if(previousMillis + 20000 < currentMillis){
    bearState = 2;
  }
    ventMP3();
  delay(2000);
  spillAv(666);
    ventMP3();
  delay(2000);
  spillAv(20);
    ventMP3();
  bearState = 666;
  previousMillis = millis();
  careMillis = millis();

}

void knapp(){
  currentMillis = millis();
  if(previousMillis + 120000 < currentMillis){
    bearState = 1;
  }
  
  if(!MP3player.isPlaying() && careMillis + 30000 < currentMillis ){
    spillAv(20);
    careMillis = millis();
  }
  
  Serial.print("Knapp: ");
  Serial.println(digitalRead(magKnapp));
  if(digitalRead(magKnapp) == 1) {
    MP3player.playMP3(trackFire);
    bearState = 4; // EGENTLIG 5
  }
  delay(50);
}

void ventMP3(){
 while(MP3player.isPlaying()){
  //vent
 } 
}

void spillAv(int s){
 uint8_t result;
 MP3player.available();

  if(!MP3player.isPlaying()){
//**********************************
if(s == 1){ //I DLE (I motion)
  int r = random(1,4);
  
  if(r == 1){
    MP3player.playMP3(trackTyveTo);
  }else if(r == 2){
    MP3player.playMP3(trackTyveFire);
  }else if(r == 3){
    MP3player.playMP3(trackTyveFem);
  }
  Serial.print("IDLE r: ");
  Serial.println(r);
//**********************************  
 }else if(s == 3){ // I GYROS
  int r = random(1,4);
    
  if(r == 1){
    MP3player.playMP3(trackEn);
  }else if(r == 2){
    MP3player.playMP3(trackTo);
  }else if(r == 3){
    MP3player.playMP3(trackFire);
  }
  Serial.print("I Gyros(sitter) r: ");
  Serial.println(r);
//**********************************  
 }else if(s == 123){ //GYROS (loftet opp)
  int r=random(1,3);
  
  if(r==1){
   MP3player.playMP3(trackSyv); 
    
  }else if(r==2){
    MP3player.playMP3(trackAtte);
    
  }
  Serial.print("I Gyros(loft) r: ");
  Serial.println(r);
//**********************************
}else if(s == 666){ //I kommando
  int r=random(1,5);
  
  if(r==1){
     MP3player.playMP3(trackTretten);
    
  }else if(r==2){
     MP3player.playMP3(trackSeksten);
    
  }else if(r==3){
     MP3player.playMP3(trackSytten);
  }else if(r==4){
     MP3player.playMP3(trackAtten);
    
  }
  Serial.print("I Knapp r: ");
  Serial.println(r);
//**********************************  
}else if(s == 20){ //Kommando
  MP3player.playMP3(trackTyve);
}else if(s == 1000){
  MP3player.playMP3(trackTyveSyv);
}
  }
  delay(1000);
}


















