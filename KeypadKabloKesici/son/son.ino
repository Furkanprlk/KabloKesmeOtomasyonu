//------ Kutuphaneler ------
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h> 
#include <math.h>

//------ LCD ------

//LCD Pinlerinin Ayarlandigi Bolum
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//LCD Bar Bolumu icin Karakter Olusturumu

byte j[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte h[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
};

//------ Servo Motor ------

//Servo Motor zservo olarak atandi
Servo zservo; 
//Servo Motorun Acik Kapali Durum Acilari
int acik = 0, kapali = 180;

//------ Step Motor ------

//Nema 17 Motorunun Bagli Oldugu A9844 Motor Surucusunun Komut Pinleri
int SEnable= 10, SStep= 9, SDir= 8;

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
long g=0, uz=0, buz=0;
int durum=0;
long Uzunluk=0, Adet=0;

//------ Ayarlar ------

//Ayarsiz Uzunluk Girdisi
long q=100,e=10;

void setup() {
  //PC de Gostermek icin
  Serial.begin(9600);
  
  lcd.createChar(0, j);
  lcd.createChar(1, h);
  
  //LCD Kaç x Kaç Belirtildi
  lcd.begin(16, 2);
  //Servo Motor 6 pine atandi
  zservo.attach(6);
  
  pinMode(SDir,OUTPUT); //Dir
  pinMode(SStep,OUTPUT); //Step
  pinMode(SEnable,OUTPUT);  //Enable
  digitalWrite(SEnable, LOW);
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
else if(durum==4)
  AktifKesim();
else if(durum==5)
  KesimBitimi();
else 
  durum=0;

}

void Keypad(){
    char Tus = Tuslar.getKey();
    if (Tus != NO_KEY){
    lcd.clear();
  EkranYonlendirici();
  lcd.setCursor(0, 0);
    if(durum==0)
      lcd.print("Uzunluk:"+  (String)q + " mm");
    else if(durum==1)
      lcd.print("Adet:" + (String)e + " ad");
      switch (Tus){
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
          g= (g * 10) + (Tus - '0');
          lcd.setCursor(0, 0);
      if(durum==1)
      lcd.print("Uzunluk:"+  (String)g + " mm");
      else if(durum==2)
      lcd.print("Adet:" + (String)g + " ad");
          Serial.println(g);
          break;
    case '#':
      if(durum==1)
      KabloUzunluk();
    else if(durum==2)
      KabloAdet();
    else
    durum++;
    Serial.println(durum);
      break;
  case '*':
    durum--;
    g=0;
    break;
    }
    }
}
void KabloUzunluk(){
  if(g==0)
    Uzunluk=q;
  else
    Uzunluk=g;
  uz= Uzunluk/1.25;
  g=0;
  durum++;
}
void KabloAdet(){
  if(g==0)
    Adet=e;
  else
    Adet=g;
  buz=Adet;
  Serial.println("g: " + (String)Adet);
  g=0;
  Serial.println("xx: " + (String)buz);
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
  Keypad();
}

void AktifKesim(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lutfen Bekleyin.");
  for(int i=0; i<Adet; i++){
  lcd.setCursor(0, 1);
  lcd.print((String)(i+1) + " / " + (String)Adet);
  digitalWrite(SEnable,LOW);
  digitalWrite(SDir,HIGH);
  for(int ii=0; ii<uz; ii++){
    digitalWrite(SStep,HIGH);
    delay(2);
    digitalWrite(SStep, LOW);
  }
  zservo.write(kapali);
  delay(600);
  zservo.write(acik);

  delay(600);
  }
  durum +=1;
}
void KesimBitimi(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Islem Bitti");
  lcd.setCursor(0, 1);
  lcd.print("    Yeni Kesim #");
  delay(100);
  Keypad();

}

