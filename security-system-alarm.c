#include <LiquidCrystal.h>
#include <Keypad.h>

LiquidCrystal lcd(1, 9, 10, 11, 12, 13);


const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2};
byte colPins[COLS] = {A1, A0, 6};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int buzzer=5;

int magnSenWindows=7;
int magnSenDoors=8;

int ledTwoRed=4;
int ledTwoGreen=2;

boolean statusSwitch=false;
boolean stateAlarm=false;
boolean checkPassword=false;
char keyPressed;
char keyPressedState;

#define Password_Lenght 5
char Data[Password_Lenght];
char password[Password_Lenght]="1234";
int data_count=0;
int data=0;
bool Pass_is_good;
char customKey;


void setup(){
  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(magnSenWindows, INPUT);
  pinMode(magnSenDoors, INPUT);
  digitalWrite(magnSenDoors, HIGH);
  digitalWrite(magnSenWindows, HIGH);
  pinMode(ledTwoRed, LOW);
  pinMode(ledTwoGreen, LOW);
  keypad.setHoldTime(500);
  lcd.setCursor(0,0);
  lcd.print("Press #.");

}

void toneAlarm() {
  tone(buzzer, 500, 300);
  delay(500);
  tone(buzzer, 400);
  delay(400);
}

void noToneAlarm() {
  noTone(buzzer);
}


void unlocked() {
  retry:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter pass:");
  while (stateAlarm) {
  char key=keypad.getKey();
  if (key) {
    Data[data_count] = key;
    lcd.setCursor(data_count,1);
    lcd.print(Data[data_count]);
    data_count++;
  }
  if(data_count == Password_Lenght-1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password is ");

    if(!strcmp(Data, password)) {
      lcd.print("Good");
      delay(100);
      stateAlarm=false;
      noToneAlarm();
      clearData();
      break;

    } else {
      lcd.print("Bad");
      delay(1000);
      lcd.clear();
      clearData();
      goto retry;
    }
    }
  }
}

void clearData() {
  while(data_count !=0) {
    Data[data_count--] = 0;
  }
  return;
}




void enableAlarm() {
  int count=5;
  while (!stateAlarm) {
    lcd.clear();
    lcd.print(count);
    if (count>1) {
      count--;
      delay(1000);
    } else {
      stateAlarm=true;
      break;
    }
  }
  lcd.clear();
  lcd.print("Alarm is enable.");
  stateAlarm=true;
  while (stateAlarm) {
    int doors=digitalRead(magnSenDoors);
    int windows=digitalRead(magnSenWindows);
    if (doors==HIGH) {
      lcd.clear();
      lcd.print("Open doors.");
      unlocked();
      stateAlarm=false;
      checkPassword=true;
      break;
    }
    if (windows==HIGH) {
      lcd.clear();
      lcd.print("Open windows");
      break;
  }
  }
}


void loop() {
  home();
  }



void home() {
  if (checkPassword) {
    lcd.clear();
    lcd.setCursor(0,0);
  lcd.print("Press #.");
  }
  statusSwitch=false;
  stateAlarm=false;
  checkPassword=false;
  keyPressed=keypad.getKey();
  keyPressedState=keypad.getState();

  if (keyPressed==keys[3][2]) {
    tone(buzzer,300,50);
    stateSwitch();
    if (statusSwitch) {
      enableAlarm();
    }
    else if (!statusSwitch) {
      lcd.clear();
      lcd.print("Close windows/doors.");
    }
   }
}


void stateSwitch() {
  lcd.clear();
  lcd.print("Wait...");
  delay(300);
  lcd.clear();
  lcd.print(digitalRead(magnSenDoors));
  int doors=digitalRead(magnSenDoors);
  int widnows=digitalRead(magnSenWindows);
  if (doors==LOW && widnows==LOW) {
    statusSwitch=true;
  }
}

