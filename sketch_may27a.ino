#include <Wire.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int buzz=A1;
int ledred=A2;
int ledgreen=A0;
int ledblue=A3;

int pos=0;

char keypressed;
char code[] = {'1', '2', '3', '4'};

char check1[sizeof(code)];
char check2[sizeof(code)];

short a = 0, i = 0, s = 0, j = 0;

byte rowPins[numRows] = {2, 3, 4, 5};
byte colPins[numCols] = {6, 7, 8, 9};

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup(){
  lcd.init();
  lcd.backlight();
  lcd.begin (16, 2);
  lcd.setCursor(0, 0);
  lcd.print("*ENTER THE CODE*");
  lcd.setCursor(1 , 1);

  lcd.print("PRESS '*'");
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(buzz, OUTPUT);
  myservo.attach(11); 
}


void loop(){

  keypressed = myKeypad.getKey();
  if (keypressed == '*') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("*ENTER THE CODE*");
    ReadCode(); 
    if (a == sizeof(code)) {
      OpenDoor();
      for(pos = 180; pos>=90; pos-=5){
        myservo.write(pos);
        delay(5);
      }

    }else {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CODE"); 
      lcd.setCursor(6, 0);
      lcd.print("INCORRECT");
      lcd.setCursor(15, 1);
      lcd.print(" ");
      lcd.setCursor(4, 1);
      lcd.print("GET AWAY!!!");
      
      unlockbuzzandledred();
    }
    delay(2000);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*ENTER THE CODE*");
    lcd.setCursor(1 , 1);

    lcd.print("PRESS '*'");
  }

  if (keypressed == '#') { 
    ChangeCode();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*ENTER THE CODE*");
    lcd.setCursor(1 , 1);
    lcd.print("PRESS '*'");
  }
}

void ReadCode() { 
  i = 0;
  a = 0;
  j = 0;

  while (keypressed != 'A') { 
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 1);
      lcd.print("*");
      j++;
      if (keypressed == code[i] && i < sizeof(code)) {
        a++;
        i++;
      }
      else
        a--;
    }
  }
  keypressed = NO_KEY;

}

void ChangeCode() {
  lcd.clear();
  lcd.print("Changing code");
  delay(1000);
  lcd.clear();
  lcd.print("Enter old code");
  ReadCode();

  if (a == sizeof(code)) {
    lcd.clear();
    lcd.print("Changing code");
    GetNewCode1();
    GetNewCode2();
    s = 0;
    for (i = 0 ; i < sizeof(code) ; i++) {
      if (check1[i] == check2[i])
        s++;
    }
    if (s == sizeof(code)) {

      for (i = 0 ; i < sizeof(code) ; i++) {
        code[i] = check2[i];
        EEPROM.put(i, code[i]);

      }
      lcd.clear();
      lcd.print("Code Changed");
      
      unlockledblue();
      delay(2000);
    }
    else { 
      lcd.clear();
      lcd.print("Codes are not");
      lcd.setCursor(0, 1);
      lcd.print("matching !!");
      delay(2000);
    }

  }

  else {                    //In case the old code is wrong you can't change it
    lcd.clear();
    lcd.print("Wrong");
    delay(2000);
  }
}

void GetNewCode1() {
  i = 0;
  j = 0;
  lcd.clear();
  lcd.print("Enter new code");
  lcd.setCursor(0, 1);
  lcd.print("and press A");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("and press A");

  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 0);
      lcd.print("*");
      check1[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void GetNewCode2() {
  i = 0;
  j = 0;

  lcd.clear();
  lcd.print("Confirm code");
  lcd.setCursor(0, 1);
  lcd.print("and press A");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("and press A");

  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 0);
      lcd.print("*");
      check2[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void OpenDoor() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  lcd.setCursor(4, 1);
  lcd.print("WELCOME!!");
  
  for(pos = 90; pos <= 180; pos +=5) {
    myservo.write(pos);
    delay(5);
  }
  
  unlockledgreen();

  delay(7000);
}
void unlockbuzzandledred(){
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(ledred, HIGH);
  delay(80);
  
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(80);
  
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(ledred, HIGH);
  delay(80);
  
  digitalWrite(buzz, LOW);
  delay(200);
  digitalWrite(ledred, LOW);
  delay(200);
  
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(ledred, HIGH);
  delay(80);
  
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(80);
  
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(ledred, HIGH);
  delay(80);
  
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(80);
}

void unlockledred(){
  digitalWrite(ledred, HIGH);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(80);
  digitalWrite(ledred, HIGH);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(200);
  digitalWrite(ledred, HIGH);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(80);
  digitalWrite(ledred, HIGH);
  delay(80);
  digitalWrite(ledred, LOW);
  delay(80);
}
void unlockledgreen(){
  digitalWrite(ledgreen, HIGH);
  delay(80);
  digitalWrite(ledgreen, LOW);
  delay(80);
  digitalWrite(ledgreen, HIGH);
  delay(80);
  digitalWrite(ledgreen, LOW);
  delay(200);
  digitalWrite(ledgreen, HIGH);
  delay(80);
  digitalWrite(ledgreen, LOW);
  delay(80);
  digitalWrite(ledgreen, HIGH);
  delay(80);
  digitalWrite(ledgreen, LOW);
  delay(80);
}
void unlockledblue(){
  digitalWrite(ledblue, HIGH);
  delay(80);
  digitalWrite(ledblue, LOW);
  delay(80);
  digitalWrite(ledblue, HIGH);
  delay(80);
  digitalWrite(ledblue, LOW);
  delay(200);
  digitalWrite(ledblue, HIGH);
  delay(80);
  digitalWrite(ledblue, LOW);
  delay(80);
  digitalWrite(ledblue, HIGH);
  delay(80);
  digitalWrite(ledblue, LOW);
  delay(80);
}
