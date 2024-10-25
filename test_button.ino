// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 7;    // the number of the LED pin

int debounceDelay = 50;

const int buttons[] = {2, 3, 4, 5};
const int leds[] = {7, 8, 9, 10};

int ledStates[4] = {0};
long lastDebounceTime[4] = {0};
int buttonsState[4] = {0};
int lastButtonsState[4] = {0};
int code[4] = {2, 2, 3, 1};
int codePosition = 0;

// variables will change:
bool readButton(int index){
  int reading = digitalRead(buttons[index]);
  bool buttonReleased = false;
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonsState[index]) {
    // reset the debouncing timer
    lastDebounceTime[index] = millis();
  }

  if ((millis() - lastDebounceTime[index]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonsState[index]) {
      buttonsState[index] = reading;
      if(reading == LOW){
        buttonReleased = true;
      }
    }
  }

  lastButtonsState[index] = reading;
  return buttonReleased;
}

void setAllLedLeds(int state, int delayTime = 0){
  for( int i= 0; i < 4; i++){
    digitalWrite(leds[i], state);
    delay(delayTime);
  }
}

bool checkCode(int buttonIndex){
  return (buttonIndex == (code[codePosition] - 1)); 
}

void blinkLeds(int iteration){
  for(int i = 0; i < iteration; i++){
    setAllLedLeds(LOW);
    delay(100);
    setAllLedLeds(HIGH);
    delay(100);
  }
}

void clearButtonState(){
  for( int index = 0; index < 4; index++){
    buttonsState[index] = 0;
    lastButtonsState[index] = 0;
  }
}




void setup() {
  // initialize the LED pin as an output:
  for(int index = 0; index < 4; index++){
    pinMode(leds[index], OUTPUT);
    pinMode(buttons[index], INPUT);
  }

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

int buttonState[4];
int count = 0;

void loop() {
  if(readButton(count)){
    if(checkCode(count)){
      digitalWrite(leds[codePosition], HIGH);
      codePosition++;
      if(codePosition == 4){
        blinkLeds(4);
        codePosition = 0;
      }
    }
    else{
      setAllLedLeds(0);
      codePosition = 0;
      clearButtonState();
    }
  }

  count = (count + 1) % 4;
}
