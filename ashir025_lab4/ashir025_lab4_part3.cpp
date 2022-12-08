/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #4  Exercise #3 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://youtube.com/shorts/gOXUVOsnay0?feature=share
enum States {INIT, L1A, L1B, L1C} SM1_state = INIT;

enum secondStates {ON, OFF} SM2_state = ON;

unsigned long period;
unsigned long lastRan;

unsigned long lastRan2;
unsigned long period2;

const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

int myLightValue = 0;

int ledPattern1[] = {0b1000, 0b0100, 0b0010};


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

  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);
  period = 1000;
  lastRan = 0;

  period2 = 50;
  lastRan2 = 0;

}


int SM1_Tick(int state) {

  //controls switching of the states
  switch (state) {
    case INIT:
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
    case INIT:
      break;
    case L1A:
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(3, LOW);
      break;
    case L1B:
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(3, LOW);
      break;
    case L1C:
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
      digitalWrite(3, HIGH);
      break;
  }
  return state;
}


int SM2_Tick(int state) {

  //controls switching of the states
  switch (state) {
    case ON:
      myLightValue = analogRead(A1);
      if (myLightValue <= 200) {
        state = OFF;
      }
      break;
    case OFF:
      myLightValue = analogRead(A1);
      if (myLightValue > 200) {
        state = ON;
      }
      break;
  }
  //controls the outputs of each state
  switch (state) {
    case ON:
      digitalWrite(2, HIGH);
      break;
    case OFF:
      digitalWrite(2, LOW);
      break;
  }
  return state;
}


void loop() {

  if ((millis() - lastRan) > period) {
    SM1_state = (States)SM1_Tick(SM1_state);
    lastRan = millis();
  }

  if ((millis() - lastRan2) > period2) {
    SM2_state = (secondStates)SM2_Tick(SM2_state);
    lastRan2 = millis();
  }

}
