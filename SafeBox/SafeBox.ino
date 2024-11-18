#include <Servo.h>

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 7;    // the number of the LED pin

int debounceDelay = 50;

const int buttons[] = {2, 3, 4, 5};
const int leds[] = {6, 7, 8, 9};
const int maxCodeLength = 12;

long buttonsLastDebounceTime[4] = {0};
int buttonsState[4] = {0};
int buttonsPrevState[4] = {0};
int code[maxCodeLength] = {2, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1};
int ledsState[4] = {0};

int setCodeButtonPin = 11;
int setCodeButtonState = 1;
int setCodeButtonPrevState = 1;
long setCodeButtonLastDebounceTime = 0;
int codeLength = 0;
int servoPin = 10;
bool isLock = true;
Servo servo;

bool readButton(int buttonPin, int& buttonState, int& prevButtonState, long& lastDebounceTime, int triggerState = 1){
  int reading = digitalRead(buttonPin);
  bool buttonReleased = false;
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:
  if (reading != prevButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
   
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      if(reading == triggerState){
        buttonReleased = true;
      }
    }
  }

  prevButtonState = reading;
  return buttonReleased;
}

void setAllLedLeds(int state, int delayTime = 0){
  for( int i= 0; i < 4; i++){
    digitalWrite(leds[i], state);
    ledsState[i] = state;
    delay(delayTime);
  }
}

bool checkCode(int buttonIndex, int codePosition){
  return (buttonIndex == (code[codePosition] - 1)); 
}

int readCodeLength(){
  for(int i =0; i< maxCodeLength; i++){
    if(code[i] == -1){
      return i;
    }
  }
  return maxCodeLength;
}

void blinkLeds(int iteration){
  for(int i = 0; i < iteration; i++){
    setAllLedLeds(LOW);
    delay(100);
    setAllLedLeds(HIGH);
    delay(100);
  }
}

void setAllButtonState(int state = 0){
  for(int i =0; i < 4; i++){
    buttonsState[i] = state;
    buttonsPrevState[i] = state;
  }
}

void stopUpdatingCode(int codePosition){
  // Serial.println();
  // Serial.println("code :");
  for(int i = 0; i < maxCodeLength; i++){
    if(i < codePosition){
      // Serial.print(code[i]);
      // Serial.print(" ");
    }
    else{
      code[i] = -1;
    }
  }
  // Serial.println();
  setAllLedLeds(0);
  setAllButtonState(1);
}



void unlock(){
  servo.write(90);;
  isLock = false;
}

void lock(){
  if(!isLock){
    servo.writeMicroseconds(0);;
    isLock = true;
  }
}

void setup() {
  // initialize the LED pin as an output:
  Serial.begin(115200);
  delay(100);
  Serial.println("Setup done !");

  for(int index = 0; index < 4; index++){
    pinMode(leds[index], OUTPUT);
    pinMode(buttons[index], INPUT_PULLUP);
  }

  pinMode(setCodeButtonPin, INPUT_PULLUP);

  // initialize the pushbutton pin as an input:
  // pinMode(buttonPin, INPUT);
  servo.attach(servoPin);
  servo.write(0);
  Serial.begin(115200);
  setAllButtonState(1);

}




void loop(){
  int count = 0;
  long startBlink;
  bool isUpdatingCode = false;
  int codePosition = 0;
  codeLength = readCodeLength();
  Serial.println("Loop start !!");
  unlock();
  while(true){
    
    if(readButton(buttons[count], buttonsState[count], buttonsPrevState[count], buttonsLastDebounceTime[count])){
      Serial.print("Button pressed - ");
      Serial.println(count);
      if(isUpdatingCode){
        code[codePosition] = count + 1;
        codePosition++;
        if(codePosition == maxCodeLength){
          stopUpdatingCode(codePosition);
          isUpdatingCode = false;
          codeLength = readCodeLength();
        }
      }
      else{
        if(checkCode(count, codePosition)){
          float percent = 4.0 * codePosition / codeLength;
          digitalWrite(leds[(int) percent], HIGH);
          codePosition++;
          if(codePosition == codeLength){
            unlock();
            blinkLeds(10);
            codePosition = 0;
          }
        }
        else{
          lock();
          setAllLedLeds(0);
          codePosition = 0;
        }
      }
    }

    if(readButton(setCodeButtonPin, setCodeButtonState, setCodeButtonPrevState, setCodeButtonLastDebounceTime) && !isLock){
      isUpdatingCode = !isUpdatingCode;
      if(isUpdatingCode){
        startBlink = millis();
      }
      else{
        stopUpdatingCode(codePosition);
        codeLength = readCodeLength();
      }
      codePosition = 0;
    }

    if(isUpdatingCode){
      if((millis() - startBlink) > 500){
        setAllLedLeds(1 - ledsState[0]);
        startBlink = millis();
      }
    }



    count = (count + 1) % 4;
  }
}
