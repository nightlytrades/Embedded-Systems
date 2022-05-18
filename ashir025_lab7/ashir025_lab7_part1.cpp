/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #7  Exercise #1 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/ajHvnECvTbo
*/
#include <LiquidCrystal.h>

// Array of Output Pin variables, set to the pins being used
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int xAxis = A1; // Recall that A0 is really a number under the hood.
const int yAxis = A0;

const int light_pattern_size = 2;
const char light_pattern[light_pattern_size] = {0x00, 0x0F};

const char sound_pattern_size = 2;
const double sound_pattern[sound_pattern_size] = {261.63, 293.66}; // C4, D4

const int buzzer = 3;

const int xAxis_median = 100; // Adjust this if needed
const int xAxis_UpperMedian = 923; // Adjust this if needed

const int yAxis_median = 100; // Adjust this if needed
const int yAxis_UpperMedian = 923; // Adjust this if needed



bool shift = false;



int lastJoystickState = LOW;  // the previous state of button
int currentJoystickState = LOW; // the current state of button
int command = 0;
int readJoystickCommand() {

  lastJoystickState = currentJoystickState;      // save the last state

  //top left
  if ((analogRead(xAxis) < xAxis_median) && (analogRead(yAxis) < yAxis_median)) {
    command = 8;
    currentJoystickState = HIGH;
    //top right
  } else if ((analogRead(yAxis) < yAxis_median) && (analogRead(xAxis) > xAxis_UpperMedian)) {
    command = 2;
    currentJoystickState = HIGH;
    //bottom right
  } else if ((analogRead(yAxis) > yAxis_UpperMedian) && (analogRead(xAxis) > xAxis_UpperMedian)) {
    command = 4;
    currentJoystickState = HIGH;
    //bottom left
  } else if ((analogRead(yAxis) > yAxis_UpperMedian) && (analogRead(xAxis) < xAxis_median)) {
    command = 6;
    currentJoystickState = HIGH;
    //left
  } else if (analogRead(xAxis) < xAxis_median) {
    command = 7;
    currentJoystickState = HIGH;
    //right
  } else  if (analogRead(xAxis) > xAxis_UpperMedian) {
    command = 3;
    currentJoystickState = HIGH;
    //up
  } else if (analogRead(yAxis) < yAxis_median) {
    command = 1;
    currentJoystickState = HIGH;
    //down
  } else if (analogRead(yAxis) > yAxis_UpperMedian) {
    command = 5;
    currentJoystickState = HIGH;
  } else {
    currentJoystickState = LOW;
  }

  //when button is pressed then released...
  if (lastJoystickState == HIGH && currentJoystickState == LOW) {
    //Serial.println(command);
    if (command == 1) {
      if (shift == true) {
        writeToLCD('A');
        shift = false;
      } else {
        writeToLCD('a');
      }
    } else if (command == 2) {
      if (shift == true) {
        writeToLCD('B');
        shift = false;
      } else {
        writeToLCD('b');
      }
    } else if (command == 3) {
      if (shift == true) {
        writeToLCD('C');
        shift = false;
      } else {
        writeToLCD('c');
      }
    } else if (command == 4) {
      if (shift == true) {
        writeToLCD('D');
        shift = false;
      } else {
        writeToLCD('d');
      }
    } else if (command == 5) {
      if (shift == true) {
        writeToLCD('E');
        shift = false;
      } else {
        writeToLCD('e');
      }
    } else if (command == 6) {
      if (shift == true) {
        writeToLCD('F');
        shift = false;
      } else {
        writeToLCD('f');
      }
    } else if (command == 7) {
      if (shift == true) {
        writeToLCD('G');
        shift = false;
      } else {
        writeToLCD('g');
      }
    } else if (command == 8) {
      if (shift == true) {
        writeToLCD('H');
        shift = false;
      } else {
        writeToLCD('h');
      }
    }
    return command;
    command = 0;
  }
}

//first digit handles horizontal, second handles vertical
int counters[] = {0, 0};
void writeToLCD(char x) {
  if (counters[0] < 16) {
    counters[1] = 0;
    lcd.setCursor(counters[0], counters[1]);
  } else {
    counters[1] = 1;
    lcd.setCursor(counters[0] - 16, counters[1]);
  }
  lcd.print(x);
  counters[0]++;
}

void resetCursor() {
  counters[0] = 0;
  counters[1] = 0;
}


const int joystickPushButton = 2;
int lastButtonState;
int currentButtonState;
unsigned long previousMillis = 0;
int timer = 0;

void handleButtonPress() {
  lastButtonState    = currentButtonState;      // save the last state
  currentButtonState = digitalRead(joystickPushButton); // read new state

  if (currentButtonState == LOW) {
    if (timer == 4) {
      lcd.clear();
      Serial.println("LCD RESET!");
      resetCursor();
      timer = 0;
    }
    timer++;
  }

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    timer = 0;
    shift = true;
    Serial.println(shift);
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

enum SM1_States { SM1_INIT, SM1_S0, SM1_S1};
int SM1_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM1_INIT:
      //State Transition
      state = SM1_S0;
      break;
    case SM1_S0:
      Serial.println(readJoystickCommand());
      handleButtonPress();
      //State Transition
      break;
      /*
        case SM1_S1:
        if (analogRead(xAxis) > xAxis_median) {
        state = SM1_S0;
        }
        else {
        state = SM1_S1;
        }
        //State Transition
        break;
      */
  }
  switch (state) { // State Action
    case SM1_INIT:
      //State Action
      break;
    case SM1_S0:
      //State Action
      //  lcd.clear();
      // lcd.setCursor(0, 0);
      // lcd.print("EMBEDDED");
      //State Action
      break;
      /*
        case SM1_S1:
        //State Action
        // lcd.clear();
        // lcd.setCursor(0, 1);
        // lcd.print("SYSTEMS");
        //State Action
        break;
      */
  }

  return state;
}

enum SM2_States { SM2_INIT, SM2_S0, SM2_S1};
int SM2_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM1_INIT:
      //State Transition
      state = SM1_S0;
      break;
    case SM1_S0:
      state = SM1_S1;
      //State Transition
      break;
    case SM1_S1:
      state = SM1_S0;
      //State Transition
      break;
  }
  switch (state) { // State Action
    case SM2_INIT:
      //State Action
      break;
    case SM1_S0:
      //State Action
      tone(buzzer, sound_pattern[0]);

      //State Action
      break;
    case SM1_S1:
      //State Action

      tone(buzzer, sound_pattern[1]);
      //State Action
      break;
  }

  return state;
}






void setup() {
  //some set up (Default Arduino Function)

  // LEDs

  Serial.begin(9600);


  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 500; // GCD
  lcd.begin(16, 2);

  pinMode(joystickPushButton, INPUT_PULLUP);
  currentButtonState = digitalRead(joystickPushButton);

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
}

