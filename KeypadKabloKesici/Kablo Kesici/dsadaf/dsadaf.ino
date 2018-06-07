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
#define acikAci 180
#define kapaliAci 0

//---------- Keypad ----------
const byte ROWS = 4; // 4 Satir
const byte COLS = 3; // 3 Sutun
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

unsigned int kabloUzunluk = 0;
unsigned int KabloMiktari = 0;

//---------- Sistem Ayarlari ----------

long g=0;
int Durum = 0;
int OncekiKabloUzunluk = 0;
int OncekiKabloMiktari = 0;
float mmPerStep = 0.18096;

//---------- Baslangic Bolumu ----------
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  
  snippers.attach(servo);
  
  snippers.write(acikAci);
  
  delay(1000);
}

//---------- Ana Dongu ----------
void loop() {
  char key = keypad.getKey();
  if(key){
    switch(key){
      case '*':
        if(Durum == 5)
          Durum = 0;
        else
          Durum += 1;
        delay(200);
        lcd.clear();
        break;
      case '#':
        Durum -=1;
        delay(200);
        lcd.clear();
        break;
    }
  }
  switch (Durum){
    case 0:
      AnaEkran();
      break;
    case 1:
      KeypadOku();
      break;
    case 2:
      KeypadOku();
      break;
    case 3:
      Onay();
      break;
    case 4:
      AktifKesim();
      break;
    case 5:
      KesimBitimi();
      break;
  }
  
 
}
void AnaEkran(){
  lcd.setCursor(0, 0);
  lcd.print("Kablo Kesim");
  lcd.setCursor(10, 1);
  lcd.print("Devam #");
  delay(100); g=0;
  KeypadOku();
}
void Uzunluk(){
  kabloUzunluk = g;
  if(OncekiKabloUzunluk != kabloUzunluk){
    lcd.clear();
    OncekiKabloUzunluk = kabloUzunluk;
  }
  lcd.setCursor(0, 0);
  lcd.print("Uzunluk:" + (String)kabloUzunluk + "mm");
  displayNavigation();
  g=0; Durum += 1;
}
void Adet(){
  KabloMiktari = g;
  if(OncekiKabloMiktari != KabloMiktari){
    lcd.clear();
    OncekiKabloMiktari = KabloMiktari;
  }
  lcd.setCursor(0, 0);
  lcd.print("Adet:" + (String)KabloMiktari);
  displayNavigation();
  g=0; Durum += 1;
}
void KeypadOku(){
  char key = keypad.getKey();
    switch(key){
    case '*':
      if(Durum == 1 || Durum == 2){
        g=0;
        KeypadOku();
        break;
      }else{
        if(Durum == 5)
          Durum = 0;
        else
          Durum -= 1;
        delay(200);
        lcd.clear();
        break;
      }
    case '#':
      delay(100);
      if(Durum==1)
        Uzunluk();
      else if(Durum ==2)
        Adet();
      else
        Durum += 1;
      break;
    default:
      lcd.setCursor(0,0);
      g = g * 10 + (key - '0');
      lcd.print(g);
      KeypadOku();
      break;
    }
}
void Onay(){
  lcd.setCursor(0, 0);
  lcd.print((String)kabloUzunluk + "mm x " + (String)KabloMiktari + "ad.");
  lcd.setCursor(0, 1);
  lcd.print("* Geri");
  lcd.setCursor(10, 1);
  lcd.print("Onay #");
  delay(100);
  KeypadOku();
}

void AktifKesim(){
  lcd.setCursor(0, 0);
  lcd.print((String)0 + "/" + (String)KabloMiktari);
  lcd.setCursor(0, 1);
  lcd.print("???s");
  int stepsToTake = (int)kabloUzunluk/mmPerStep;
  for(int i = 0; i < KabloMiktari; i++){
    unsigned long DonguSuresi = millis();
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x < stepsToTake; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(500);
    }
    lcd.setCursor(0, 0);
    lcd.print((String)(i+1) + "/" + (String)KabloMiktari);
   
    snippers.write(kapaliAci);
    delay(600);
    snippers.write(acikAci);
    delay(600);
    
    lcd.setCursor(0, 1);
    
    unsigned long KalanSure = ((millis() - DonguSuresi)*(KabloMiktari - (i+1)))/1000;
    lcd.print((String)KalanSure + "s    ");
    
  }
  kabloUzunluk = 0;
  KabloMiktari = 0;
  Durum = 5;
}
void KesimBitimi(){
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
