//------ Kutuphaneler ------
#include <LiquidCrystal.h>
#include "Keypad.h"

//------ LCD ------

//LCD Pinlerinin Ayarlandigi Bolum

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//------ Step Motor ------


//------ Keypad ------

//Kaclik Keypad Kullandigimizi Tanimliyoruz 
const byte Satir= 4; 
const byte Stun= 3; 

//Keypadin Tus Konumlarini Tanimliyoruz
char TusKonum[Satir][Stun]=
{
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};

//Keypadin Arduinodaki Baglantisini Tanimliyoruz
byte SatirPin[Satir]= {A3,A4,A5,A6};
byte StunPin[Stun]= {A2,A1,A0};

//Keypad Kutuphanesine Bilgileri Tanimliyoruz
Keypad Tuslar= Keypad(makeKeymap(TusKonum), SatirPin, StunPin, Satir, Stun);

//------ Degiskenler ------

//Keypad Basamak Kontrol icin
long g=0;
int durum=0;
long Uzunluk=0,Adet=0;

void setup() {
  // LCD Kaç x Kaç Belirtildi
  lcd.begin(16, 2);

  Serial.begin(9600);
}

void loop() {

if(durum==0)
  AnaEkran();
else if(durum==1)
  Keypad();
else if(durum==2)
  Keypad();
else if(durum==3)
  Onay();
/* 
else if(durum==4)
  AktifKesim();
else if(durum==5)
  KesimBitimi();
*/
else 
  durum=0;

/*  switch(durum){
    case 0:
      AnaEkran();
      break;
    case 1:
    Keypad();
      break;
    case 2:
    Keypad();
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
  default:
    durum=0;
      lcd.setCursor(0, 1);
    lcd.print("Dongu Bozuk" + (string)durum;
    break;
    */
  Keypad();
}

void Keypad(){
    char Tus = Tuslar.getKey();
    if (Tus != NO_KEY){
    lcd.clear();
  EkranYonlendirici();
      switch (Tus){
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
          g= (g * 10) + (Tus - '0');
          lcd.setCursor(0, 0);
      if(durum==1)
      lcd.print((String)g + " mm");
      else if(durum==2)
      lcd.print((String)g + " ad");
          Serial.println(g);
          break;
    case '#':
      if(durum==1)
      KabloUzunluk();
    else if(durum==2)
      KabloAdet();
    else
    durum++;
      break;
  case '*':
    durum--;
    g=0;
    break;
    }
    }
}
void KabloUzunluk(){
  g=Uzunluk;
  g=0;
  durum++;
}
void KabloAdet(){
  g=Adet;
  g=0;
  durum++;
}
void AnaEkran(){
  lcd.setCursor(0, 0);
  lcd.print("Kablo Kesici");
  lcd.setCursor(9, 1);
  lcd.print("Basla #");
  Keypad();
}
void EkranYonlendirici(){
  lcd.setCursor(0, 1);
  lcd.print("* Geri");
  lcd.setCursor(9, 1);
  lcd.print("Devam #");
  delay(100);
}
void Onay(){
  lcd.setCursor(0, 0);
  lcd.print("Kesim Baslasinmi");
  EkranYonlendirici();
}