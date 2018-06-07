//---------- Kutuphaneler ----------

#include "Keypad.h"
#include <LiquidCrystal.h>
#include <Servo.h>
 
//---------- LCD ----------

LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

//---------- Step Motor ----------

#define stepPin 7
#define dirPin 8

//---------- Servo Motor ----------

Servo snippers;
#define servo 10
#define openAngle 180
#define closedAngle 0

//---------- Keypad ----------
const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] =
{
{'1','2','3' },
{'4','5','6' },
{'7','8','9' },
{'*','0','#' }
};
byte rowPins[ROWS] = {5, 6, 7, 8};
byte colPins[COLS] = {2, 3, 4};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//---------- Kullanici Ayarlari ----------

unsigned int wireLength = 0;
unsigned int wireQuantity = 0;

//---------- Sistem Ayarlari ----------

int z=0, a=0, u=0;
long g=0;
int state = 0;
int incrementSpeed = 1;
int previousWireLength = 0;
int previousWireQuantity = 0;
float mmPerStep = 0.18096;

//---------- Baslangic Bolumu ----------
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  
  snippers.attach(servo);
  
  snippers.write(openAngle);
  
  delay(1000);
}

//---------- Ana Dongu ----------
void loop() {
/*  if (!digitalRead(rightButton)){
    if(state == 5){
      state = 0;
    }else{
      state += 1;
    }
    delay(200);
    lcd.clear();
  }
  if (!digitalRead(leftButton) && state > 0 && state < 4){
    state -=1;
    delay(200);
    lcd.clear();
  }
*/ 
 
  switch (state){
    case 0:
      homeScreen();
      break;
    case 1:
      Uzunluk();
      break;
    case 2:
      Adet();
      break;
    case 3:
      confirm();
      break;
    case 4:
      currentlyCutting();
      break;
    case 5:
      finishedCutting();
      break;
  }
 
}
void homeScreen(){
  lcd.setCursor(0, 0);
  lcd.print("Kablo Kesim");
  lcd.setCursor(10, 1);
  lcd.print("Devam #");
  delay(100);
}
void Uzunluk(){
  wireLength = g;
  if(previousWireLength != wireLength){
    lcd.clear();
    previousWireLength = wireLength;
  }
  //LCD Bilgi Bolumu
  lcd.setCursor(0, 0);
  lcd.print("Uzunluk:" + (String)wireLength + "mm");
  displayNavigation();
}
void Adet(){
  wireQuantity = g;
  if(previousWireQuantity != wireQuantity){
    lcd.clear();
    previousWireQuantity = wireQuantity;
  }
  //LCD Bilgi Bolumu
  lcd.setCursor(0, 0);
  lcd.print("Adet:" + (String)wireQuantity);
  displayNavigation();
}
void KeypadOku(){
  char key = keypad.getKey();
    switch(key){
    case '*':
      g=0;
      KeypadOku();
      break;
    case '#':
      delay(100);
      if(a==0)
        Uzunluk();
      else if(u==0);
        Adet();
      break;
    default:
      lcd.setCursor(0,0);
      g = g * 10 + (key - '0');
      lcd.print(g);
      break;
    }
}
void confirm(){
  lcd.setCursor(0, 0);
  lcd.print((String)wireLength + "mm x " + (String)wireQuantity + "ad.");
  lcd.setCursor(0, 1);
  lcd.print("* Iptal");
  lcd.setCursor(10, 1);
  lcd.print("Onay #");
  delay(100);
}

void currentlyCutting(){
  lcd.setCursor(0, 0);
  lcd.print((String)0 + "/" + (String)wireQuantity);
  lcd.setCursor(0, 1);
  lcd.print("???s");
  int stepsToTake = (int)wireLength/mmPerStep;
  for(int i = 0; i < wireQuantity; i++){
    unsigned long timeForOneCycle = millis();
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x < stepsToTake; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(500);
    }
    lcd.setCursor(0, 0);
    lcd.print((String)(i+1) + "/" + (String)wireQuantity);
   
    snippers.write(closedAngle);
    delay(600);
    snippers.write(openAngle);
    delay(600);
    
    lcd.setCursor(0, 1);
    
    unsigned long timeRemaining = ((millis() - timeForOneCycle)*(wireQuantity - (i+1)))/1000;
    lcd.print((String)timeRemaining + "s    ");
    
  }
  wireLength = 0;
  wireQuantity = 0;
  state = 5;
}
void finishedCutting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ISLEM TAMAM");
  lcd.setCursor(10, 1);
  lcd.print("Devam #");
  delay(100);
}
void displayNavigation(){
  lcd.setCursor(0, 1);
  lcd.print("* Geri");
  lcd.setCursor(10, 1);
  lcd.print("Devam #");
  delay(100);
}
