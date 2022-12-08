/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #5  Exercise #3 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/VAjH6ZiGFSc
*/
enum States {INIT, UP, UP2, DOWN, DOWN2, LEFT, LEFT2, RIGHT, RIGHT2, CLICK, CLICK2, SUCCESS} SM1_state;
unsigned long lastRan;
const unsigned long period = 500;

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A1; // Recall that A0 is really a number under the hood.
const int yAxis = A0;
const int light_pattern_size = 5;
const char light_pattern[light_pattern_size] = {0b1000, 0b0100, 0b0010, 0b0001, 0b0000};//left,right,up,down,clear

const char sound_pattern_size = 5;
const double sound_pattern[sound_pattern_size] = {261.63, 293.66, 329.63, 349.23, 392.00}; // C4, D4,E4,F4,G4

const int buzzer = 3;

const int xAxis_median = 100; // Adjust this if needed
const int xAxis_UpperMedian = 923; // Adjust this if needed

const int yAxis_median = 100; // Adjust this if needed
const int yAxis_UpperMedian = 923; // Adjust this if needed


const int joystickPushButton = 2;
// Variables will change:
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button


int buzzerCounter = 0;

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

int lastJoystickState = LOW;  // the previous state of button
int currentJoystickState = LOW; // the current state of button
int command = 0;

int readJoystickCommand() {

  lastJoystickState = currentJoystickState;      // save the last state

  //left
  if (analogRead(xAxis) < xAxis_median) {
    tone(buzzer, sound_pattern[0]);
    command = 1;
    currentJoystickState = HIGH;
    //right
  } else  if (analogRead(xAxis) > xAxis_UpperMedian) {
    tone(buzzer, sound_pattern[1]);
    command = 2;
    currentJoystickState = HIGH;
    //up
  } else if (analogRead(yAxis) < yAxis_median) {
    tone(buzzer, sound_pattern[2]);
    command = 3;
    currentJoystickState = HIGH;
    //down
  } else if (analogRead(yAxis) > yAxis_UpperMedian) {
    tone(buzzer, sound_pattern[3]);
    command = 4;
    currentJoystickState = HIGH;
  } else {
    noTone(buzzer);
    currentJoystickState = LOW;
  }


  if (lastJoystickState == HIGH && currentJoystickState == LOW) {
    //Serial.println(command);
    return command;
    command = 0;
  }
}

int SM1_Tick(int state) {

  //Read thing
  switch (state) { // State transitions
    case INIT:
      //State Transition
      state = UP;
      break;
    case UP:
      if (readJoystickCommand() == 3) {
        state = UP2;
      }
      break;
    case UP2:
      if (readJoystickCommand() == 3) {
        state = DOWN;
      }
      break;
    case DOWN:
      if (readJoystickCommand() == 4) {
        state = DOWN2;
      }
      break;
    case DOWN2:
      if (readJoystickCommand() == 4) {
        state = LEFT;
      }
      break;
    case LEFT:
      if (readJoystickCommand() == 1) {
        state = LEFT2;
      }
      break;
    case LEFT2:
      if (readJoystickCommand() == 1) {
        state = RIGHT;
      }
      break;
    case RIGHT:
      if (readJoystickCommand() == 2) {
        state = RIGHT2;
      }
      break;
    case RIGHT2:
      if (readJoystickCommand() == 2) {
        state = CLICK;
      }
      break;
    case CLICK:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joystickPushButton); // read new state
      if (currentButtonState == LOW) {
        tone(buzzer, sound_pattern[4]);
      } else {
        noTone(buzzer);
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        state = CLICK2;
      }
      break;
    case CLICK2:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joystickPushButton); // read new state
      if (currentButtonState == LOW) {
        tone(buzzer, sound_pattern[4]);
      } else {
        noTone(buzzer);
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        state = SUCCESS;
      }
      break;
    case SUCCESS:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joystickPushButton); // read new state
      if (currentButtonState == LOW) {
        tone(buzzer, sound_pattern[4]);
      } else {
        noTone(buzzer);
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        state = UP;
      }
      break;
  }

  switch (state) { // State Action
    case INIT:
      break;
    case UP:
      Serial.println("1");
      break;
    case UP2:
      Serial.println("2");
      break;
    case DOWN:
      Serial.println("3");
      break;
    case DOWN2:
      Serial.println("4");
      break;
    case LEFT:
      Serial.println("5");
      break;
    case LEFT2:
      Serial.println("6");
      break;
    case RIGHT:
      Serial.println("7");
      break;
    case RIGHT2:
      Serial.println("8");
      break;
    case CLICK:
      Serial.println("9");
      break;
    case CLICK2:
      Serial.println("10");
      break;
    case SUCCESS:
      Serial.println("SUCCESS!");
      tone(buzzer, sound_pattern[buzzerCounter]);
      if (buzzerCounter > 5) {
        buzzerCounter = 0;
      } else {
        buzzerCounter++;
      }
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

  pinMode(joystickPushButton, INPUT_PULLUP);
  currentButtonState = digitalRead(joystickPushButton);

  // We will also read from the serial monitor
  Serial.begin(9600);
}

void loop() {

  if ((millis() - lastRan) > period) {
    SM1_state = (States)SM1_Tick(SM1_state);
    lastRan = millis();
  }

}
