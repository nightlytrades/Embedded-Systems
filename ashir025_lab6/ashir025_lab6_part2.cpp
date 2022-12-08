/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #6  Exercise #2 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/nhYHukvXPZM
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

enum SM1_States { SM1_INIT, L1A, L1B, L1C};
int SM1_Tick(int state) {
  //controls switching of the states
  switch (state) {
    case SM1_INIT:
      state = L1A;
      break;
    case L1A:
      state = L1B;
      break;
    case L1B:
      state = L1C;
      break;
    case L1C:
      state = L1A;
      break;
  }
  //controls the outputs of each state
  switch (state) {
    case SM1_INIT:
      break;
    case L1A:
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case L1B:
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      break;
    case L1C:
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      break;
  }
  return state;
}

enum SM2_States { SM2_INIT, ON, OFF};
int SM2_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM2_INIT:
      //State Transition
      state = OFF;
      break;
    case OFF:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joyStickBtn); // read new state
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        state = ON;
      }
      break;
    case ON:
      lastButtonState    = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joyStickBtn); // read new state
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        state = OFF;
      }
      break;

  }
  switch (state) { // State Action
    case SM2_INIT:
      //State Transition
      state = OFF;
      break;
    case OFF:
      digitalWrite(11, LOW);
      break;
    case ON:
      digitalWrite(11, HIGH);
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
  tasks[i].period = 100;
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
