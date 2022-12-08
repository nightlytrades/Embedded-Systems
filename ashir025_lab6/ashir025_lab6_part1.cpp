/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #6  Exercise #1 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/GcmuCgPUjC0
*/
// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A1; // Recall that A0 is really a number under the hood.
const int yAxis = A0;
const int joyStickBtn = 2;
const int light_pattern_size = 2;
const char light_pattern[light_pattern_size] = {0x00, 0x0F};

const char sound_pattern_size = 5;
const double sound_pattern[sound_pattern_size] = {261.63, 293.66, 329.63, 349.23, 392.00}; // C4, D4,E4,F4,G4

const int buzzer = 3;

const int xAxis_median = 100; // Adjust this if needed
const int xAxis_Uppermedian = 923; // Adjust this if needed
const int yAxis_median = 100; // Adjust this if needed
const int yAxis_Uppermedian = 923; // Adjust this if needed

int buzzerCounter = 0;

int joystickState = 0;

int ledState = LOW;     // the current state of LED
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button

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


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum SM1_States { SM1_INIT, UP1, UP2, DOWN1, DOWN2, LEFT1, LEFT2, RIGHT1, RIGHT2, CLICK1, CLICK2, SUCCESS};
int SM1_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM1_INIT:
      //State Transition
      state = UP1;
      break;
    case UP1:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(yAxis) < yAxis_median) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 1;
        state = UP2;
      }
      break;
    case UP2:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(yAxis) < yAxis_median) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 2;
        state = DOWN1;
      }
      break;
    case DOWN1:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(yAxis) > yAxis_Uppermedian) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 3;
        state = DOWN2;
      }
      break;
    case DOWN2:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(yAxis) > yAxis_Uppermedian) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 4;
        state = LEFT1;
      }
      break;
    case LEFT1:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(xAxis) < xAxis_median) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 5;
        state = LEFT2;
      }
      break;
    case LEFT2:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(xAxis) < xAxis_median) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 6;
        state = RIGHT1;
      }
      break;
    case RIGHT1:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(xAxis) > xAxis_Uppermedian) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 7;
        state = RIGHT2;
      }
      break;
    case RIGHT2:
      lastButtonState = currentButtonState;      // save the last state
      if (analogRead(xAxis) > xAxis_Uppermedian) {
        currentButtonState = HIGH;
      } else {
        currentButtonState = LOW;
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 8;
        state = CLICK1;
      }
      break;
    case CLICK1:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joyStickBtn); // read new state
      if (currentButtonState == LOW) {
      } else {
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 9;
        state = CLICK2;
      }
      break;
    case CLICK2:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joyStickBtn); // read new state
      if (currentButtonState == LOW) {
      } else {
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 10;
        state = SUCCESS;
      }
      break;
    case SUCCESS:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joyStickBtn); // read new state
      if (currentButtonState == LOW) {
      }
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        joystickState = 1;
        state = UP1;
      }
      break;

  }
  switch (state) { // State Action
    case SM1_INIT:
      //State Action
      break;
    case UP1:
      Serial.print("In state "); Serial.println(state);
      break;
    case UP2:
      Serial.print("In state "); Serial.println(state);
      break;
    case DOWN1:
      Serial.print("In state "); Serial.println(state);
      break;
    case DOWN2:
      Serial.print("In state "); Serial.println(state);
      break;
    case LEFT1:
      Serial.print("In state "); Serial.println(state);
      break;
    case LEFT2:
      Serial.print("In state "); Serial.println(state);
      break;
    case RIGHT1:
      Serial.print("In state "); Serial.println(state);
      break;
    case RIGHT2:
      Serial.print("In state "); Serial.println(state);
      break;
    case CLICK1:
      Serial.print("In state "); Serial.println(state);
      break;
    case CLICK2:
      Serial.print("In state "); Serial.println(state);
      break;
    case SUCCESS:
      Serial.println("SUCCESS!");
      break;
  }

  return state;
}

enum SM2_States { SM2_INIT, SM2_UP1, SM2_UP2, SM2_DOWN1, SM2_DOWN2, SM2_LEFT1, SM2_LEFT2, SM2_RIGHT1, SM2_RIGHT2, SM2_CLICK1, SM2_CLICK2, SM2_SUCCESS};
int SM2_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM2_INIT:
      //State Transition
      state = SM2_UP1;
      break;
    case SM2_UP1:
      if (analogRead(yAxis) < yAxis_median) {
        tone(buzzer, sound_pattern[0]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 1) {
        state = SM2_UP2;
      }
      break;
    case SM2_UP2:
      if (analogRead(yAxis) < yAxis_median) {
        tone(buzzer, sound_pattern[0]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 2) {
        state = SM2_DOWN1;
      }
      break;
    case SM2_DOWN1:
      if (analogRead(yAxis) > yAxis_Uppermedian) {
        tone(buzzer, sound_pattern[1]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 3) {
        state = SM2_DOWN2;
      }
      break;
    case SM2_DOWN2:
      if (analogRead(yAxis) > yAxis_Uppermedian) {
        tone(buzzer, sound_pattern[1]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 4) {
        state = SM2_LEFT1;
      }
      break;
    case SM2_LEFT1:
      if (analogRead(xAxis) < xAxis_median) {
        tone(buzzer, sound_pattern[2]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 5) {
        state = SM2_LEFT2;
      }
      break;
    case SM2_LEFT2:
      if (analogRead(xAxis) < xAxis_median) {
        tone(buzzer, sound_pattern[2]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 6) {
        state = SM2_RIGHT1;
      }
      break;
    case SM2_RIGHT1:
      if (analogRead(xAxis) > xAxis_Uppermedian) {
        tone(buzzer, sound_pattern[3]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 7) {
        state = SM2_RIGHT2;
      }
      break;
    case SM2_RIGHT2:
      if (analogRead(xAxis) > xAxis_Uppermedian) {
        tone(buzzer, sound_pattern[3]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 8) {
        state = SM2_CLICK1;
      }
      break;
    case SM2_CLICK1:
      if (currentButtonState == LOW) {
        tone(buzzer, sound_pattern[4]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 9) {
        state = SM2_CLICK2;
      }
      break;
    case SM2_CLICK2:
      if (currentButtonState == LOW) {
        tone(buzzer, sound_pattern[4]);
      } else {
        noTone(buzzer);
      }
      if (joystickState == 10) {
        state = SM2_SUCCESS;
      }
      break;
    case SM2_SUCCESS:
      if (joystickState == 1) {
        state = SM2_UP1;
      }
      break;

  }
  switch (state) { // State Action
    case SM2_INIT:
      //State Transition
      state = SM2_UP1;
      break;
    case SM2_UP1:
      break;
    case SM2_UP2:
      break;
    case SM2_DOWN1:
      break;
    case SM2_DOWN2:
      break;
    case SM2_LEFT1:
      break;
    case SM2_LEFT2:
      break;
    case SM2_RIGHT1:
      break;
    case SM2_RIGHT2:
      break;
    case SM2_CLICK1:
      break;
    case SM2_CLICK2:
      break;
    case SM2_SUCCESS:
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
  //some set up (Default Arduino Function)

  // LEDs
  for (int i = 0; i < b_size; i++)
  {
    pinMode(b[i], OUTPUT);
  }
  Serial.begin(9600);

  pinMode(joyStickBtn, INPUT_PULLUP);

  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 500; // GCD

}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran

    }
  }
  //Serial.println(analogRead(xAxis));
  delay(delay_gcd); // GCD.
}
