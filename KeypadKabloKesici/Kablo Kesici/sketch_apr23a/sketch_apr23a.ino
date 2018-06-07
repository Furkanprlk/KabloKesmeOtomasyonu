#include "Keypad.h"
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
char KEY[4] = {'1','2','3','4'}; // default secret key
char attempt[4] = {0,0,0,0};
int z=0;
void setup()
{
   Serial.begin(9600);
}
void correctKEY() // do this if the correct KEY is entered
{
   Serial.println(" KEY ACCEPTED...");
}
void incorrectKEY() // do this if an incorrect KEY is entered
{
   Serial.println("KEY REJECTED!");
}
void checkKEY()
{
   int correct=0;
   int i;
   for ( i = 0; i < 4 ; i++ )
   {
      if (attempt[i]==KEY[i])
      {
         correct++;
      }
   }
   if (correct==4)
   {
      correctKEY();
   }
   else
   {
      incorrectKEY();
   }
   for (int zz=0; zz<4; zz++) // clear previous key input
   {
      attempt[zz]=0;
   } 
}
void readKeypad()
{
   char key = keypad.getKey();
   if (key != NO_KEY)
   {
      switch(key)
      {
      case '*':
         z=0;
         break;
      case '#':
         delay(100); // added debounce
        checkKEY();
         break;
      default:
         attempt[z]=key;
         z++;
      }
   }
}
void loop()
{
   readKeypad();
}
