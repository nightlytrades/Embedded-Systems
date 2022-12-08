/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #3 Exercise #3 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/ru6_C3XsLRk
*/
enum States {INIT, L1, L2} SM1_state = INIT;
int X_button = 8;
int Y_button = 9;

const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

int setLEDS = 0;
int myLightValue = 0;

int ledXState = LOW;
int lastButtonXState;
int currentButtonXState;

int ledYState = LOW;
int lastButtonYState;
int currentButtonYState;

int ledPattern1[] = {0b0001, 0b0011, 0b0111, 0b1111};

int ledPattern2[] = {0b1111, 0b0000, 0b1100, 0b0011};

void checkButtonStates() {

  lastButtonXState    = currentButtonXState;      // save the last state
  currentButtonXState = digitalRead(X_button); // read new state

  if (lastButtonXState == HIGH && currentButtonXState == LOW) {

    if (setLEDS < 3) {
      setLEDS++;
    } else {
      setLEDS = 0;
    }

  }

  lastButtonYState    = currentButtonYState;      // save the last state
  currentButtonYState = digitalRead(Y_button); // read new state

  if (lastButtonYState == HIGH && currentButtonYState == LOW) {

    if (setLEDS > 0) {
      setLEDS--;
    } else {
      setLEDS = 3;
    }

  }
}


void readData() {
  if (Serial.available()) {
    s_buf = Serial.parseInt();

    for (int i = (s_size - 1); i >= 0; i--) {
      s[i] = (s_buf >> i) & 0x01;
    }
  }
}

void resetBuffer() {
  for (int i = 0; i < b_size; i++) {
    digitalWrite(b[i], LOW);
  }
}

void writeBuffer(unsigned char b_temp, int size = b_size) {
  for (int i = (size - 1); i >= 0; i--) {
    if ((b_temp >> i) & 0x01) {
      digitalWrite(b[i], HIGH);
    }
  }
}

void setup() {

  pinMode(X_button, INPUT);
  pinMode(Y_button, INPUT_PULLUP);

  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);

}

bool triggerOnce = false;
bool triggerOnce2 = false;

int SM1_Tick(int state) {

  switch (state) {
    case INIT:
      state = L1;
      break;
    case L1:
      if (myLightValue <= 200) {
        if (triggerOnce == false) {
          state = L2;
        }
      } else {
        triggerOnce = false;
      }
      break;
    case L2:
      if (myLightValue <= 200) {
        if (triggerOnce == true) {
          state = L1;
        }
      } else {
        triggerOnce = true;
      }
      break;
  }
  switch (state) {
    case INIT:
      break;
    case L1:
      myLightValue = analogRead(A1);
      checkButtonStates();
      resetBuffer();
      writeBuffer(ledPattern1[setLEDS]);
      break;
    case L2:
      myLightValue = analogRead(A1);
      checkButtonStates();
      resetBuffer();
      writeBuffer(ledPattern2[setLEDS]);
      break;
  }
  return state;
}

void loop() {

  SM1_state = (States)SM1_Tick(SM1_state);

}
