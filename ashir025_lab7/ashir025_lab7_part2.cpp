/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #7 Exercise #2 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/watch?v=4kYmQpIdTaE
*/
#include <LiquidCrystal.h>

// Array of Output Pin variables, set to the pins being used
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int xAxis = A1; // Recall that A0 is really a number under the hood.
const int yAxis = A0;

const int light_pattern_size = 2;
const char light_pattern[light_pattern_size] = {0x00, 0x0F};

const char sound_pattern_size = 3;
const double sound_pattern_win[sound_pattern_size] = {261.63, 293.66, 329.63};
const double sound_pattern_lose[sound_pattern_size] = {329.63, 293.66, 261.63};

const int buzzer = 3;
int buzzerCounter = 0;

const int xAxis_median = 100; // Adjust this if needed
const int xAxis_UpperMedian = 923; // Adjust this if needed

const int yAxis_median = 100; // Adjust this if needed
const int yAxis_UpperMedian = 923; // Adjust this if needed

String wordBank[] = {"crazy", "candy", "yield", "catch", "cozey", "texas", "pizza", "swiss", "avoid", "aloud"};
String wordToGuess;

char progressWord[] = "_____";
char lettersGuessedCorrectly[] = "     ";
String lettersInWrongOrder;
int progressWordPos;


char alphabet [2][13] = {
  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'},
  {'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y' , 'z'}
};

bool moveToCharSelection = false;

char selectedChar;

bool win = false;

int winStateSpeaker = 0;

int noOfTries = 6;

//int for cursor
int jsPos[] = {0, 0};


const int numRows = 2;
const int numCols = 16;

int lastJoystickState = LOW;  // the previous state of button
int currentJoystickState = LOW; // the current state of button
int command = 0;
void readJoystickCommand() {

  lastJoystickState = currentJoystickState;      // save the last state

  //left
  if (analogRead(xAxis) < xAxis_median) {
    command = 1;
    currentJoystickState = HIGH;
    //right
  } else  if (analogRead(xAxis) > xAxis_UpperMedian) {
    command = 2;
    currentJoystickState = HIGH;
    //up
  } else if (analogRead(yAxis) < yAxis_median) {
    command = 3;
    currentJoystickState = HIGH;
    //down
  } else if (analogRead(yAxis) > yAxis_UpperMedian) {
    command = 4;
    currentJoystickState = HIGH;
  } else {
    currentJoystickState = LOW;
  }

  //when button is pressed then released...
  if (lastJoystickState == HIGH && currentJoystickState == LOW) {
    //Serial.println(command);
    if (command == 1) {
      if (jsPos[0] > 0) {
        jsPos[0]--;
      }
    } else if (command == 2) {
      if (jsPos[0] < 12) {
        jsPos[0]++;
      }
    } else if (command == 3) {
      if (jsPos[1] > 0) {
        jsPos[1]--;
      }
    } else if (command == 4) {
      if (jsPos[1] < 1) {
        jsPos[1]++;
      }
    }
  }
}


void selectRandomWord() {
  int randomNumber = random(10);
  wordToGuess = wordBank[randomNumber];
  Serial.println("*****************");
  Serial.print("Word to guess: "); Serial.println(wordToGuess);
  Serial.println("*****************");
}


void drawMainScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(progressWord);
  lcd.print("E");
  lcd.setCursor(0, 1);
  lcd.print(lettersGuessedCorrectly);
  if (lettersInWrongOrder.length() > 0) {
    lcd.setCursor(8, 0);
    lcd.print(lettersInWrongOrder[0]);
  }
  if (lettersInWrongOrder.length() > 1) {
    lcd.setCursor(12, 0);
    lcd.print(lettersInWrongOrder[1]);
  }
  if (lettersInWrongOrder.length() > 2) {
    lcd.setCursor(8, 1);
    lcd.print(lettersInWrongOrder[2]);
  }
  if (lettersInWrongOrder.length() > 3) {
    lcd.setCursor(12, 1);
    lcd.print(lettersInWrongOrder[3]);
  }
  lcd.setCursor(15, 0);
  lcd.print(noOfTries);
}

void drawWinScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YOU WIN!!!");
  lcd.setCursor(0, 1);
  lcd.print("Press to Reset");
}

void drawFailScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YOU LOSE :(");
  lcd.setCursor(0, 1);
  lcd.print("Press to Reset");
}

void drawAlphabet() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 13; j++) {
      lcd.setCursor(j, i);
      lcd.print(alphabet[i][j]);
    }
  }
}




const int joystickPushButton = 2;
int lastButtonState;
int currentButtonState;
unsigned long previousMillis = 0;
int timer = 0;

void handleButtonPressMain() {
  lastButtonState = currentButtonState;      // save the last state
  currentButtonState = digitalRead(joystickPushButton); // read new state

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    //if 'E' selected
    if (jsPos[0] == 5 && jsPos[1] == 0) {
      Serial.println("E selected!");
      noOfTries--;
      checkWords();
    } else {
      progressWordPos = jsPos[0];
      Serial.println("pressed " + String(progressWordPos) + " word position");
      moveToCharSelection = true;
    }
  }
}


void handleButtonPressCharSelect() {
  lastButtonState    = currentButtonState;      // save the last state
  currentButtonState = digitalRead(joystickPushButton); // read new state

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    selectedChar = alphabet[jsPos[1]][jsPos[0]];
    Serial.println("selected letter " + selectedChar);
    progressWord[progressWordPos] = selectedChar;
    moveToCharSelection = false;
  }
}


void checkWords() {
  byte correctCounter = 0;
  for (int i = 0; i < 5; i++) {
    if (progressWord[i] == wordToGuess[i]) {
      lettersGuessedCorrectly[i] = wordToGuess[i];
      correctCounter++;
    } else {
      if (wordToGuess.indexOf(progressWord[i]) > 0) {
        lettersInWrongOrder += progressWord[i];
      }
    }
  }
  Serial.println(String(correctCounter) + " letters guessed correctly!");
  for (int i = 0; i < 4; i++) {
    Serial.println("Array of letters in wrong order: " + String(lettersInWrongOrder[i]));
  }

  if (correctCounter == 5) {
    win = true;
  } else {
    win = false;
  }

}

void resetTextHolderVars() {
  for (int i = 0; i < 5; i++) {
    progressWord[i] = '_';
  }
  for (int i = 0; i < 5; i++) {
    lettersGuessedCorrectly[i] = ' ';
  }
  lettersInWrongOrder = "";
  winStateSpeaker = 0;
  noOfTries = 6;
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

enum SM1_States { SM1_INIT, MAIN, CHAR_SELECT, WIN, FAIL};
int SM1_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM1_INIT:
      selectRandomWord();
      state = MAIN;
      break;
    case MAIN:
      drawMainScreen();
      if (moveToCharSelection == true) {
        lcd.clear();
        jsPos[0] = 0;
        jsPos[1] = 0;
        state = CHAR_SELECT;
      }
      if (win == true) {
        state = WIN;
      }
      if (noOfTries == 0) {
        state = FAIL;
      }
      break;
    case CHAR_SELECT:
      drawAlphabet();
      if (moveToCharSelection == false) {
        lcd.clear();
        jsPos[0] = 0;
        jsPos[1] = 0;
        state = MAIN;
      }
      break;
    case WIN:
      drawWinScreen();
      winStateSpeaker = 1;
      lastButtonState = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joystickPushButton); // read new state
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        resetTextHolderVars();
        win = false;
        state = SM1_INIT;
      }
      break;
    case FAIL:
      drawFailScreen();
      winStateSpeaker = 2;
      lastButtonState = currentButtonState;      // save the last state
      currentButtonState = digitalRead(joystickPushButton); // read new state
      if (lastButtonState == HIGH && currentButtonState == LOW) {
        resetTextHolderVars();
        winStateSpeaker = 0;
        state = SM1_INIT;
      }
      break;

  }
  switch (state) { // State Action
    case SM1_INIT:
      //State Action
      break;
    case MAIN:
      //Serial.println("in main");
      readJoystickCommand();
      //Serial.print("jsPos[0]: "); Serial.println(jsPos[0]);
      lcd.setCursor(jsPos[0], jsPos[1]);
      handleButtonPressMain();
      break;
    case CHAR_SELECT:
      //Serial.println("in char select");
      readJoystickCommand();
      //Serial.print("jsPos[0]: "); Serial.println(jsPos[0]);
      lcd.setCursor(jsPos[0], jsPos[1]);
      handleButtonPressCharSelect();
      break;
    case WIN:
      break;
    case FAIL:
      break;

  }

  return state;
}



enum SM2_States { SM2_INIT, NO_MUSIC, WIN_MUSIC, FAIL_MUSIC};
int SM2_Tick(int state) {
  //Read thing
  switch (state) { // State transitions
    case SM1_INIT:
      //State Transition
      state = NO_MUSIC;
      break;
    case NO_MUSIC:
      if (winStateSpeaker == 1) {
        state = WIN;
      } else if (winStateSpeaker == 2) {
        state = FAIL_MUSIC;
      }
      break;
    case WIN_MUSIC:
      if (winStateSpeaker == 0) {
        state = NO_MUSIC;
      }
      break;
    case FAIL_MUSIC:
      if (winStateSpeaker == 0) {
        state = NO_MUSIC;
      }
      break;
  }
  switch (state) { // State Action
    case SM2_INIT:
      //State Action
      break;
    case NO_MUSIC:
      noTone(buzzer);
      break;
    case WIN_MUSIC:
      tone(buzzer, sound_pattern_win[buzzerCounter]);
      if (buzzerCounter > 3) {
        buzzerCounter = 0;
      } else {
        buzzerCounter++;
      }
      break;
    case FAIL_MUSIC:
      tone(buzzer, sound_pattern_lose[buzzerCounter]);
      if (buzzerCounter > 3) {
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

  Serial.begin(9600);


  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 500; // GCD
  lcd.begin(16, 2);

  lcd.blink();

  pinMode(joystickPushButton, INPUT_PULLUP);
  currentButtonState = digitalRead(joystickPushButton);

  randomSeed(analogRead(2));

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

