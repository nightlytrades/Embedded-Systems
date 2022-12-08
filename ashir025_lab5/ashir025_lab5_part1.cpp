/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #5  Exercise #1 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/_8oMFpD2pqg
*/
enum States {INIT, S0, S1, S2, S3, S4} SM1_state;
unsigned long lastRan;
const unsigned long period = 500;

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int light_pattern_size = 5;
const char light_pattern[light_pattern_size] = {0b0000, 0b1000, 0b01100, 0b1110, 0b1111};

const char sound_pattern_size = 5;
const double sound_pattern[sound_pattern_size] = {261.63, 293.66, 329.63, 349.23, 392.00}; // C4, D4,E4,G4

const int buzzer = 3;

const int xAxis_median = 100; // Adjust this if needed



// Variables will change:
bool paused = false;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

const int joystickButtonPin = 2;

void checkButton() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(joystickButtonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        paused = !paused;
        Serial.println(paused);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

// Reset the Output Buffer.
void resetBuffer() {
  for (int i = 0; i < b_size; i++) {
    // Note this is an arduino function call to the pins
    digitalWrite(b[i], LOW);
  }
}
// Writes to the buffer. Note this function ORs the current value with the new value
// Note that size is an optional argument with default size the same as the buffer
void writeBuffer(unsigned char b_temp, int size = b_size)
{
  for (int i = (size - 1); i >= 0; i--) {
    if ((b_temp >> i) & 0x01) {
      digitalWrite(b[i], HIGH);
    }
  }

}

int SM1_Tick(int state) {

  //Read thing
  switch (state) { // State transitions
    case INIT:
      //State Transition
      state = S0;
      break;
    case S0:
      if (paused == true) {
        state = S0;
      } else {
        state = S1;
      }
      //State Transition
      break;
    case S1:
      if (paused == true) {
        state = S1;
      } else {
        state = S2;
      }
      break;
    case S2:
      if (paused == true) {
        state = S2;
      } else {
        state = S3;
      }
      break;
    case S3:
      if (paused == true) {
        state = S3;
      } else {
        state = S4;
      }
      break;
    case S4:
      if (paused == true) {
        state = S4;
      } else {
        state = S0;
      }
      //State Transition
      break;
  }

  switch (state) { // State Action
    case INIT:
      break;
    case S0:
      resetBuffer();
      writeBuffer(light_pattern[0]);
      tone(buzzer, sound_pattern[0]);
      break;
    case S1:
      resetBuffer();
      writeBuffer(light_pattern[1]);
      tone(buzzer, sound_pattern[1]);
      break;
    case S2:
      resetBuffer();
      writeBuffer(light_pattern[2]);
      tone(buzzer, sound_pattern[2]);
      break;
    case S3:
      resetBuffer();
      writeBuffer(light_pattern[3]);
      tone(buzzer, sound_pattern[3]);
      break;
    case S4:
      resetBuffer();
      writeBuffer(light_pattern[4]);
      tone(buzzer, sound_pattern[4]);
      break;
  }

  return state;
}


void setup() {
  // OUTPUT is a defined macro in Arduino!
  for (int i = 0; i < b_size; i++)
  {
    pinMode(b[i], OUTPUT);
  }

  pinMode(joystickButtonPin, INPUT_PULLUP);

  // We will also read from the serial monitor
  Serial.begin(9600);
}

void loop() {

  if ((millis() - lastRan) > period) {
    SM1_state = (States)SM1_Tick(SM1_state);
    lastRan = millis();
  }

  checkButton();

}
