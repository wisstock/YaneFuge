#include <Servo.h> 
#include <LiquidCrystal.h>

#define BTN_UP   1
#define BTN_DOWN 2
#define BTN_LEFT 3
#define BTN_RIGHT 4
#define BTN_SELECT 5
#define BTN_NONE 10

#define STATE_ONE 11
#define STATE_TWO 22

#define SHORT_SPIN 33

Servo esc;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int escPin = 11;
int minPulse = 1000;
int maxPulse = 2000;
int escSpeed = 1110;

int RPM;
int rotorRadius = 75; // mm

int button;
int dispState;

int detectButton() {
  int keyAnalog =  analogRead(A0);
  if (keyAnalog < 100) {
    return BTN_RIGHT;
  } else if (keyAnalog < 200) {
    return BTN_UP;
  } else if (keyAnalog < 400) {
    return BTN_DOWN;
  } else if (keyAnalog < 600) {
    return BTN_LEFT;
  } else if (keyAnalog < 800) {
    return BTN_SELECT;
  } else {
    return BTN_NONE;
  }
}

void clearLine(int line){
  lcd.setCursor(0, 1);
  lcd.print("                ");
}


void setup() {
  lcd.begin(16, 2); 
  
  esc.attach(escPin, minPulse, maxPulse);
  esc.write(1000);
  
  dispState = STATE_ONE;
}


void loop() {
  lcd.clear();
  
  button = detectButton();
  
  if (button == BTN_RIGHT) {
    esc.write(1900);
    lcd.setCursor(0, 0);
    lcd.print("YaneFuge v1.4");
    lcd.setCursor(0, 1);
    lcd.print("Short Spin");
  } else {
  
  switch (button) {
    case BTN_UP:
      if (escSpeed < 2000) {
        escSpeed = escSpeed + 10;
        break;
      } else {
          break; 
        }
    case BTN_DOWN:
      if (escSpeed > 1110) {
        escSpeed = escSpeed - 10;
        break;
      } else {
          break;
        }
    case BTN_SELECT:
      if (dispState == STATE_ONE){
        dispState = STATE_TWO;
      } else {
          dispState = STATE_ONE;
      }  
    default:
      break;
   }
  
  
  
  esc.write(escSpeed);
  
  if (escSpeed > 1120) {
    RPM = map(escSpeed, 1120, 2000, 0, 7200);
  } else {
    RPM = 0;
  }
  
  float RPMfloat = RPM;
  float RPMsqr = (RPMfloat / 1000) * (RPMfloat / 1000);
  float RCF = 1.12 * rotorRadius * RPMsqr; 
 
  lcd.setCursor(0, 0);
  lcd.print("YaneFuge v1.4");
  
  switch(dispState){
    case STATE_ONE:
      lcd.setCursor(0, 1);
      lcd.print("Speed: ");
      lcd.print(RPM);
      lcd.print(" RPM");
      break;
    case STATE_TWO:
      lcd.setCursor(0, 1);
      lcd.print("RCF: ");
      lcd.print(RCF);
      lcd.print(" g");
      break;
    default:
      clearLine(1);
      break;
  }
  }
  
  delay(150);
}
