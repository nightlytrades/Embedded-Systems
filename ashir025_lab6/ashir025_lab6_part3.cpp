/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #6  Exercise #3 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/VAjH6ZiGFSc
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

int randomSeries[5];
int numOfElements;

int input_result = 0;

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

int lastJoystickState = LOW;  // the previous state of button
int currentJoystickState = LOW; // the current state of button
int command = 0;
int readJoystickCommand() {

  lastJoystickState = currentJoystickState;      // save the last state

  //left
  if (analogRead(xAxis) < xAxis_median) {
    command = 8;
    currentJoystickState = HIGH;
    //right
  } else  if (analogRead(xAxis) > xAxis_Uppermedian) {
    command = 11;
    currentJoystickState = HIGH;
    //up
  } else if (analogRead(yAxis) < yAxis_median) {
    command = 9;
    currentJoystickState = HIGH;
    //down
  } else if (analogRead(yAxis) > yAxis_Uppermedian) {
    command = 10;
    currentJoystickState = HIGH;
  } else {
    currentJoystickState = LOW;
  }


  if (lastJoystickState == HIGH && currentJoystickState == LOW) {
    Serial.println(command);
    return command;
    command = 0;
  } else {
    return 0;
  }
}


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

void blinkLEDS() {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < b_size; i++) {
      digitalWrite(b[i], HIGH);
    }
    delay(100);
    for (int i = 0; i < b_size; i++) {
      digitalWrite(b[i], LOW);
    }
    delay(100);
  }
}

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum SM1_States { SM1_INIT, GENERATE, SHOW, INPUT1, INPUT2, INPUT3, INPUT4, INPUT5};
int SM1_Tick(int state) {
  //controls switching of the states
  switch (state) {
    case SM1_INIT:
      state = GENERATE;
      break;
    case GENERATE:
      state = SHOW;
      break;
    case SHOW:
      state = INPUT1;
      command = 0;
      break;
    case INPUT1:
      //Serial.print("MY COMMAND "); Serial.println(command);
      if (command == randomSeries[0]) {
        blinkLEDS();
        state = INPUT2;
        command = 0;
      } else if (command != 0 && command != randomSeries[0]) {
        state = GENERATE;
      }
      break;
    case INPUT2:
      //Serial.print("MY COMMAND "); Serial.println(command);
      if (command == randomSeries[1]) {
        blinkLEDS();
        state = INPUT3;
        command = 0;
      } else if (command != 0 && command != randomSeries[1]) {
        state = GENERATE;
      }
      break;
    case INPUT3:
      //Serial.print("MY COMMAND "); Serial.println(command);
      if (command == randomSeries[2]) {
        blinkLEDS();
        state = INPUT4;
        command = 0;
      } else if (command != 0 && command != randomSeries[2]) {
        state = GENERATE;
      }
      break;
    case INPUT4:
      if (numOfElements > 3) {
        //Serial.print("MY COMMAND "); Serial.println(command);
        if (command == randomSeries[3]) {
          blinkLEDS();
          state = INPUT5;
          command = 0;
        } else if (command != 0 && command != randomSeries[3]) {
          state = GENERATE;
        }
      } else {
        state = GENERATE;
      }
      break;
    case INPUT5:
      if (numOfElements > 3) {
        //Serial.print("MY COMMAND "); Serial.println(command);
        if (command == randomSeries[4]) {
          blinkLEDS();
          state = GENERATE;
          command = 0;
        } else if (command != 0 && command != randomSeries[4]) {
          state = GENERATE;
        }
      } else {
        state = GENERATE;
      }
      break;
  }
  //controls the outputs of each state
  switch (state) {
    case SM1_INIT:
      break;
    case GENERATE:
      numOfElements = random(3, 6);
      Serial.print("pattern size: "); Serial.println(numOfElements);
      for (int i = 0; i < numOfElements; i++) {
        randomSeries[i] = random(8, 12);
      }
      Serial.println("*****Random Sequence*****");
      for (int i = 0; i < numOfElements; i++) {
        Serial.println(randomSeries[i]);
      }
      Serial.println();
      Serial.println();
      break;
    case SHOW:
      for (int i = 0; i < numOfElements; i++) {
        digitalWrite(randomSeries[i], HIGH);
        delay(500);
        digitalWrite(randomSeries[i], LOW);
        delay(500);
      }
      break;
    case INPUT1:
      readJoystickCommand();
      //Serial.println("In input 1");
      break;
    case INPUT2:
      readJoystickCommand();
      //Serial.println("In input 2");
      break;
    case INPUT3:
      readJoystickCommand();
      //Serial.println("In input 3");
      break;
    case INPUT4:
      readJoystickCommand();
      //Serial.println("In input 4");
      break;
    case INPUT5:
      readJoystickCommand();
      //Serial.println("In input 5");
      break;
  }
  return state;
}

enum SM2_States { SM2_INIT, NOTE1, NOTE2, NOTE3, NOTE4, NOTE5};
int SM2_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM2_INIT:
      //State Transition
      state = NOTE1;
      break;
    case NOTE1:
      state = NOTE2;
      break;
    case NOTE2:
      state = NOTE3;
      break;
    case NOTE3:
      state = NOTE4;
      break;
    case NOTE4:
      state = NOTE5;
      break;
    case NOTE5:
      state = NOTE1;
      break;

  }
  switch (state) { // State Action
    case SM2_INIT:
      break;
    case NOTE1:
      tone(buzzer, sound_pattern[0]);
      break;
    case NOTE2:
      tone(buzzer, sound_pattern[1]);
      break;
    case NOTE3:
      tone(buzzer, sound_pattern[2]);
      break;
    case NOTE4:
      tone(buzzer, sound_pattern[3]);
      break;
    case NOTE5:
      tone(buzzer, sound_pattern[4]);
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

  randomSeed(analogRead(A2));

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

  delay_gcd = 100; // GCD

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
