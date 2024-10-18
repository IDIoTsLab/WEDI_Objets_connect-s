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


// variables will change:
void readButton(int index){
  int reading = digitalRead(buttons[index]);

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

      // only toggle the LED if the new button state is HIGH
      if (buttonsState[index] == HIGH) {
        ledStates[index] = !ledStates[index];
      }
    }
  }

  lastButtonsState[index] = reading;
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
  readButton(count);

  digitalWrite(leds[count], ledStates[count]);

  count = (count + 1) % 4;


}
